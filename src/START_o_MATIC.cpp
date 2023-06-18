/************************************************************************
*    START-o-MATIC- Petrus Prins 2023
*************************************************************************/
#include <GLOBALS.h>





// STATE MACHINE
YA_FSM STATE_MACHINE;
enum State {S0_INIT, S1_DISCOVERY_MODE, S2_ENGINE_STARTING, S3_ENGINE_RUNNING, S4_ENGINE_SHUTDOWN, S5_SYSTEM_SHUTDOWN};         // STATES

const char * const stateName[] PROGMEM = { "S0_INIT", "S1_DISOVERY_MODE", "S2_ENGINE_STARTING", "S3_ENGINE_RUNNING", "S4_ENGINE_SHUTDOWN", "S5_SYSTEM_SHUTDOWN"};

enum Input {xINIT, xDISCOVERY_MODE, xALREADY_RUNNING, xSTART_ENGINE, xENGINE_RUNNING, xSHUTDOWN_ENGINE, xSHUTDOWN_SYSTEM};            // State Change Trigger Variables
Input STATE_INPUT;        // Latest State Change Input
//uint8_t currentState;




FireTimer S0_Timer;
FireTimer S1_Timer;



//============================================================
//                         S0_INIT
//============================================================
void onState_S0_INIT_Enter()                                              
{
    S0_Timer.start();
}

void onState_S0_INIT()                                                     // This callback function will be called continuously while state is active
{
    if(S0_Timer.fire()) 
    { 
        STATE_INPUT = Input::xDISCOVERY_MODE; 
    }
}


//============================================================
//                   S1_DISCOVERY_MODE
//============================================================
void onState_S1_DISCOVERY_MODE_Enter()
{
    S1_Timer.start();
}

void onState_S1_DISCOVERY_MODE()                                   
{    
    if(S1_Timer.fire()) 
    { 
        //STATE_INPUT = Input::xINIT; 
    } 
     // debug test code...
    // gLCD_DC_Volts = random(9,28);
    // gLCD_AC_Volts = random(220, 240);  
    // gLCD_AC_Amps  = random(0, 301)/10; 
     //....    
}



//============================================================
//                   S2_ENGINE_STARTING
//============================================================
void onState_S2_ENGINE_STARTING_Enter()                                              
{
}

void onState_S2_ENGINE_STARTING()                                                    
{
}





//============================================================
//                   S3_ENGINE_RUNNING
//============================================================
void onState_S3_ENGINE_RUNNING_Enter()                                              
{
}

void onState_S3_ENGINE_RUNNING()                                                    
{
}



//============================================================
//                   S4_ENGINE_SHUTDOWN
//============================================================

void onState_S4_ENGINE_SHUTDOWN_Enter()                                              
{
}

void onState_S4_ENGINE_SHUTDOWN()                                                    
{
}



//============================================================
//                   S5_SYSTEM_SHUTDOWN
//============================================================

void onState_S5_SYSTEM_SHUTDOWN_Enter()                                              
{
}

void onState_S5_SYSTEM_SHUTDOWN()                                                    
{
}




void INIT_STATE_MACHINE() 
{
   
    //==========================
    //       ADD STATES
    //==========================   
    // Follow the order of defined enumeration for the state definition (will be used as index)
    //Add States                   => name,         timeout,       onEnter callback,                onState cb,           onLeave cb
    STATE_MACHINE.AddState(stateName[S0_INIT]            ,  0,  onState_S0_INIT_Enter            ,  onState_S0_INIT            , nullptr);
    STATE_MACHINE.AddState(stateName[S1_DISCOVERY_MODE]  ,  0,  onState_S1_DISCOVERY_MODE_Enter  ,  onState_S1_DISCOVERY_MODE  , nullptr);
    STATE_MACHINE.AddState(stateName[S2_ENGINE_STARTING] ,  0,  onState_S2_ENGINE_STARTING_Enter ,  onState_S2_ENGINE_STARTING , nullptr);
    STATE_MACHINE.AddState(stateName[S3_ENGINE_RUNNING]  ,  0,  onState_S3_ENGINE_RUNNING_Enter  ,  onState_S3_ENGINE_RUNNING  , nullptr);
    STATE_MACHINE.AddState(stateName[S4_ENGINE_SHUTDOWN] ,  0,  onState_S4_ENGINE_SHUTDOWN_Enter ,  onState_S4_ENGINE_SHUTDOWN , nullptr);
    STATE_MACHINE.AddState(stateName[S5_SYSTEM_SHUTDOWN] ,  0,  onState_S5_SYSTEM_SHUTDOWN_Enter ,  onState_S5_SYSTEM_SHUTDOWN , nullptr);

    //==========================
    //       ADD NORMAL STATES
    //==========================   
    // Add transitions with related callback functions ( FROM, TO, Trigger)   
    STATE_MACHINE.AddTransition(S0_INIT                  , S0_INIT             ,  [](){return (STATE_INPUT == xINIT);            });    // System boot initial state
    STATE_MACHINE.AddTransition(S0_INIT                  , S1_DISCOVERY_MODE   ,  [](){return (STATE_INPUT == xDISCOVERY_MODE);  });    // Check all system inputs - determine if engine is off
    STATE_MACHINE.AddTransition(S1_DISCOVERY_MODE        , S2_ENGINE_STARTING  ,  [](){return (STATE_INPUT == xSTART_ENGINE);    });    // Start Engine (takes 30s until full speed)
    STATE_MACHINE.AddTransition(S2_ENGINE_STARTING       , S3_ENGINE_RUNNING   ,  [](){return (STATE_INPUT == xENGINE_RUNNING);  });    // Assume Engine Running - check engine stats.
    STATE_MACHINE.AddTransition(S3_ENGINE_RUNNING        , S4_ENGINE_SHUTDOWN  ,  [](){return (STATE_INPUT == xSHUTDOWN_ENGINE); });    // No more current drawn, so shutting down engine (30s until stop)
    STATE_MACHINE.AddTransition(S4_ENGINE_SHUTDOWN       , S5_SYSTEM_SHUTDOWN  ,  [](){return (STATE_INPUT == xSHUTDOWN_SYSTEM); });    // Put system in endless loop and disconnect keepalive relay.


    //==========================
    //    ADD ERROR STATE      
    //==========================   
    // This state change should never be needed. If System starts up and engine is already running - just keep it running. 
    STATE_MACHINE.AddTransition(S1_DISCOVERY_MODE        , S3_ENGINE_RUNNING  ,     [](){return (STATE_INPUT == xALREADY_RUNNING); });

    
    //==============================
    //    SET SYSTEM INITIAL STATE      
    //==============================   
    STATE_INPUT = Input::xINIT;                       // SET Initial State
    STATE_MACHINE.Update();                           // Update State Machine
    gSYSTEM_STATE = STATE_MACHINE.GetState();         // Set global State to new state
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
    INIT_Timers();
    INIT_STATE_MACHINE();

    S0_Timer.begin(3000);
    S1_Timer.begin(3000);

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






