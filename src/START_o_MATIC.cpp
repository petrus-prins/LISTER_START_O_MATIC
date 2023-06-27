/************************************************************************
*    START-o-MATIC- Petrus Prins 2023
*************************************************************************/
#include <GLOBALS.h>





// STATE MACHINE
YA_FSM STATE_MACHINE;
enum State {S0_INIT, S1_DISCOVERY_MODE, S2_ENGINE_STARTING, S3_ENGINE_RUNNING, S4_POWERING_HOUSE, S5_ENGINE_SHUTDOWN, S6_SYSTEM_SHUTDOWN};         // STATES

const char * const stateName[] PROGMEM = { "S0_INIT", "S1_DISOVERY_MODE", "S2_ENGINE_STARTING", "S3_ENGINE_RUNNING", "S4_POWERING_HOUSE", "S5_ENGINE_SHUTDOWN", "S6_SYSTEM_SHUTDOWN"};

enum Input {xINIT, xDISCOVERY_MODE, xSTART_ENGINE, xENGINE_RUNNING, xPOWERING_HOUSE, xSHUTDOWN_ENGINE, xSHUTDOWN_SYSTEM};            // State Change Trigger Variables
Input STATE_INPUT;                                                                                                                    // Latest State Change Input


FireTimer S0_INIT_STATE_Timer;
FireTimer S1_DISCOVERY_MODE_Timer;
FireTimer S2_ENGINE_STARTING_Timer;
FireTimer S3_ENGINE_RUNNING_Timer;
FireTimer S4_POWERING_HOUSE_Timer;
FireTimer S5_ENGINE_SHUTDOWN_Timer;
FireTimer S6_SYSTEM_SHUTDOWN_Timer;




//============================================================
//                         S0_INIT
//============================================================
void onState_S0_INIT_Enter()
{
    S0_INIT_STATE_Timer.start();                                  
}
void onState_S0_INIT()                                                // This state is the default startup state.                                
{
    if (S0_INIT_STATE_Timer.fire())
    {
        STATE_INPUT = Input::xDISCOVERY_MODE;                         
    }
}
//============================================================


//============================================================
//                   S1_DISCOVERY_MODE
//============================================================
//   Check that system is in complete shutdown state
//   If not assume that system reset while running
//   If system is running, state Will go to 
//   onState_S3_ENGINE_RUNNING
//============================================================
void onState_S1_DISCOVERY_MODE_Enter()
{
    S1_DISCOVERY_MODE_Timer.start();                                  // Fires ever 1 second
}

void onState_S1_DISCOVERY_MODE()                                      // This callback function will be called continuously while state is active 
{    
    SET_RELAY1_KEEP_SYSTEM_ALIVE(true);
    SET_RELAY2_ENABLE_230VAC_LINE(false);                            // Keep 230V Off 
    SET_RELAY3_ENABLE_FUEL_SOLENOID(false);                         
    SET_RELAY4_ENABLE_STARTER_MOTOR(false);    

    if (S1_DISCOVERY_MODE_Timer.fire())
    {
        if  (gENGINE_IS_IDLE_FLAG)                                        
        {
            STATE_INPUT = Input::xSTART_ENGINE;                         // If system is in idle state, goto to start engine 
        } 
        else
        {
            STATE_INPUT = Input::xENGINE_RUNNING;
        } 
    }
}
//============================================================






//============================================================
//                   S2_ENGINE_STARTING
//============================================================
void onState_S2_ENGINE_STARTING_Enter()                                              
{
    S2_ENGINE_STARTING_Timer.start();                                     // Timer to keep starter on for about 25s.
}

void onState_S2_ENGINE_STARTING()                                                    
{
    SET_RELAY1_KEEP_SYSTEM_ALIVE(true);
    SET_RELAY2_ENABLE_230VAC_LINE(true);                                 
    SET_RELAY3_ENABLE_FUEL_SOLENOID(true);                                // Switch Fuel ON  
    SET_RELAY4_ENABLE_STARTER_MOTOR(true);                                // Enable Starter Motor

    if(S2_ENGINE_STARTING_Timer.fire()) 
    { 
        STATE_INPUT = Input::xENGINE_RUNNING; 
    }   
}
//============================================================




//============================================================
//                   S3_ENGINE_RUNNING
//============================================================
void onState_S3_ENGINE_RUNNING_Enter()                                              
{
    S3_ENGINE_RUNNING_Timer.start();
}

void onState_S3_ENGINE_RUNNING()                                                    
{
    SET_RELAY1_KEEP_SYSTEM_ALIVE(true);
    SET_RELAY2_ENABLE_230VAC_LINE(true);
    SET_RELAY3_ENABLE_FUEL_SOLENOID(true);                         
    SET_RELAY4_ENABLE_STARTER_MOTOR(false);

    if (S3_ENGINE_RUNNING_Timer.fire())
    {
        STATE_INPUT = Input::xPOWERING_HOUSE; 
    }
}
//============================================================



//============================================================
//                   S4_POWERING_HOUSE
//============================================================
void onState_S4_POWERING_HOUSE_Enter()                                              
{
    S4_POWERING_HOUSE_Timer.start();
}

void onState_S4_POWERING_HOUSE()
{
    SET_RELAY1_KEEP_SYSTEM_ALIVE(true);
    SET_RELAY2_ENABLE_230VAC_LINE(true);                          // Remove 24V from line, Put 220V back on line to house.
    SET_RELAY3_ENABLE_FUEL_SOLENOID(true);                         
    SET_RELAY4_ENABLE_STARTER_MOTOR(false);

    if (S4_POWERING_HOUSE_Timer.fire())
    {
        if (gZERO_CURRENT_TRIGGER_ACTIVE || gZERO_Amps_Measured_Flag)         // Amps 0 or minimum cutoff point reached technically the same thing. 
        {
            STATE_INPUT = Input::xSHUTDOWN_ENGINE; 
        }
    }
}      



//============================================================
//                   S5_ENGINE_SHUTDOWN
//============================================================

void onState_S5_ENGINE_SHUTDOWN_Enter()                                              
{
    S5_ENGINE_SHUTDOWN_Timer.start();
}

void onState_S5_ENGINE_SHUTDOWN()                                                    
{
    SET_RELAY1_KEEP_SYSTEM_ALIVE(true);
    SET_RELAY2_ENABLE_230VAC_LINE(true);
    SET_RELAY3_ENABLE_FUEL_SOLENOID(false);                         
    SET_RELAY4_ENABLE_STARTER_MOTOR(false);

    if(S5_ENGINE_SHUTDOWN_Timer.fire())
    {
          STATE_INPUT = Input::xSHUTDOWN_SYSTEM;
    } 
}
//============================================================


//============================================================
//                   S5_SYSTEM_SHUTDOWN
//============================================================

void onState_S6_SYSTEM_SHUTDOWN_Enter()                                              
{
    S6_SYSTEM_SHUTDOWN_Timer.start();
    WRITE_New_Shutdown_Count_To_EEPROM();                                         // This prevents the system failure count from increasing.                                    
}

void onState_S6_SYSTEM_SHUTDOWN()                                                  // Endless loop but also kills its own power     
{
    //SET_RELAY1_KEEP_SYSTEM_ALIVE(false);
    SET_RELAY2_ENABLE_230VAC_LINE(false);                                         // Put 
    SET_RELAY3_ENABLE_FUEL_SOLENOID(false);                         
    SET_RELAY4_ENABLE_STARTER_MOTOR(false);

    if (S6_SYSTEM_SHUTDOWN_Timer.fire())
    {
       SET_RELAY1_KEEP_SYSTEM_ALIVE(false); 
    }
}



//============================================================
//                   INIT STATE MACHINE
//============================================================
void INIT_STATE_MACHINE() 
{
   
    //==========================
    //       ADD STATES
    //==========================   
    // Follow the order of defined enumeration for the state definition (will be used as index)
    //Add States                   => name,         timeout,       onEnter callback,                onState cb,           onLeave cb
    STATE_MACHINE.AddState(stateName[S0_INIT]            ,  0,  nullptr            ,  onState_S0_INIT            , nullptr);  // Default State when System Starts.
    STATE_MACHINE.AddState(stateName[S1_DISCOVERY_MODE]  ,  0,  onState_S1_DISCOVERY_MODE_Enter  ,  onState_S1_DISCOVERY_MODE  , nullptr);
    STATE_MACHINE.AddState(stateName[S2_ENGINE_STARTING] ,  0,  onState_S2_ENGINE_STARTING_Enter ,  onState_S2_ENGINE_STARTING , nullptr);
    STATE_MACHINE.AddState(stateName[S3_ENGINE_RUNNING]  ,  0,  onState_S3_ENGINE_RUNNING_Enter  ,  onState_S3_ENGINE_RUNNING  , nullptr);
    STATE_MACHINE.AddState(stateName[S4_POWERING_HOUSE]  ,  0,  onState_S4_POWERING_HOUSE_Enter  ,  onState_S4_POWERING_HOUSE  , nullptr);
    STATE_MACHINE.AddState(stateName[S5_ENGINE_SHUTDOWN] ,  0,  onState_S5_ENGINE_SHUTDOWN_Enter ,  onState_S5_ENGINE_SHUTDOWN , nullptr);
    STATE_MACHINE.AddState(stateName[S6_SYSTEM_SHUTDOWN] ,  0,  onState_S6_SYSTEM_SHUTDOWN_Enter ,  onState_S6_SYSTEM_SHUTDOWN , nullptr);

    //==========================
    //       ADD NORMAL STATES
    //==========================   
    // Add transitions with related callback functions ( FROM, TO, Trigger)   
    STATE_MACHINE.AddTransition(S6_SYSTEM_SHUTDOWN       , S0_INIT             ,  [](){return (STATE_INPUT == xINIT);            });    // System boot initial state
    STATE_MACHINE.AddTransition(S0_INIT                  , S1_DISCOVERY_MODE   ,  [](){return (STATE_INPUT == xDISCOVERY_MODE);  });    // Check all system inputs - determine if engine is off
    STATE_MACHINE.AddTransition(S1_DISCOVERY_MODE        , S2_ENGINE_STARTING  ,  [](){return (STATE_INPUT == xSTART_ENGINE);    });    // Start Engine (takes 30s until full speed)
    STATE_MACHINE.AddTransition(S1_DISCOVERY_MODE        , S3_ENGINE_RUNNING   ,  [](){return (STATE_INPUT == xENGINE_RUNNING);  });    // Assume Engine Running - check engine stats.
    STATE_MACHINE.AddTransition(S2_ENGINE_STARTING       , S3_ENGINE_RUNNING   ,  [](){return (STATE_INPUT == xENGINE_RUNNING);  });    // Assume Engine Running - check engine stats.
    STATE_MACHINE.AddTransition(S3_ENGINE_RUNNING        , S4_POWERING_HOUSE   ,  [](){return (STATE_INPUT == xPOWERING_HOUSE);  });    // No more current drawn, so shutting down engine (30s until stop)
    STATE_MACHINE.AddTransition(S4_POWERING_HOUSE        , S5_ENGINE_SHUTDOWN  ,  [](){return (STATE_INPUT == xSHUTDOWN_ENGINE); });    // Put system in endless loop and disconnect keepalive relay.
    STATE_MACHINE.AddTransition(S5_ENGINE_SHUTDOWN       , S6_SYSTEM_SHUTDOWN  ,  [](){return (STATE_INPUT == xSHUTDOWN_SYSTEM); });    // Put system in endless loop and disconnect keepalive relay.
    
    //==============================
    //    SET SYSTEM INITIAL STATE      
    //==============================   
    STATE_INPUT = Input::xINIT;                       // SET Initial State
}




//====================================
//              SETUP       
//====================================
void setup() 
{
    INIT_DIGITAL_PIN_Modes(); 
    INIT_RELAY_STATES();
    INIT_EEPROM();
    INIT_I2C_LCD();
    INIT_ANALOG_PIN_Modes();
    INIT_ADC_Stats();
    INIT_BACKGROUND_Timers();
    INIT_STATE_MACHINE();

    // INIT TIMERS USED IN STATE MACHINE
    S0_INIT_STATE_Timer.begin(2000);
    S1_DISCOVERY_MODE_Timer.begin(10000);                
    S2_ENGINE_STARTING_Timer.begin(25000);              // takes 25s for engine to start up
    S3_ENGINE_RUNNING_Timer.begin(2000); 
    S4_POWERING_HOUSE_Timer.begin(10000);
    S5_ENGINE_SHUTDOWN_Timer.begin(30000);              // Takes 30s for engine to stop turning
    S6_SYSTEM_SHUTDOWN_Timer.begin(5000);
}



//====================================
//              MAIN LOOP       
//====================================
void loop() 
{
    gADC_Update_Timer.tick();
    gLCD_Update_Timer.tick();     
    if (STATE_MACHINE.Update()) 
    {
       gSYSTEM_STATE = STATE_MACHINE.GetState();
    }
}






