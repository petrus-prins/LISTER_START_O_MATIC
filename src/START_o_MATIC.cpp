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




FireTimer S1_Timer;
FireTimer S2_Timer;


void onState_INIT_Enter()                                              // This callback functions will bel called continuosly while state is active
{
        S1_Timer.start();
}

void onState_INIT()                                              // This callback functions will bel called continuosly while state is active
{
    if(S1_Timer.fire()) 
    { 
        STATE_INPUT = Input::xDISCOVERY_MODE; 
    }
}

void onState_INIT_Exit()                                              // This callback functions will bel called continuosly while state is active
{
     //S1_Timer.
}




void onState_DISCOVERY_MODE_Enter()
{
    S2_Timer.start();
}


void onState_DISCOVERY_MODE()
{
    if(S2_Timer.fire()) 
    { 
        STATE_INPUT = Input::xINIT; 
    } 
}






void INIT_STATE_MACHINE() {
  // Follow the order of defined enumeration for the state definition (will be used as index)
  //Add States          => name,     timeout,     onEnter callback, onState cb,    onLeave cb
    STATE_MACHINE.AddState(stateName[S0_INIT],            0,  onState_INIT_Enter, onState_INIT, nullptr);
    STATE_MACHINE.AddState(stateName[S1_DISCOVERY_MODE],  0,  onState_DISCOVERY_MODE_Enter, onState_DISCOVERY_MODE, nullptr);
 // stateMachine.AddState(stateName[CLOSING], CLOSE_TIME, nullptr, onStateClosing, nullptr);
 // stateMachine.AddState(stateName[OPENED],  WAIT_OPEN_TIME,  onEnteringOpened, onStateOpened, nullptr);
 // stateMachine.AddState(stateName[OPENING], OPEN_TIME,  nullptr, onStateOpening, nullptr);
 // stateMachine.AddState(stateName[STOP_WAIT], WAIT_FTC_TIME,  nullptr, onStateStopWait, nullptr);

   // Add transitions with related callback functions ( FROM, TO, Trigger)
   STATE_MACHINE.AddTransition(S1_DISCOVERY_MODE, S0_INIT,          [](){return (STATE_INPUT == xINIT);           });
   STATE_MACHINE.AddTransition(S0_INIT, S1_DISCOVERY_MODE,          [](){return (STATE_INPUT == xDISCOVERY_MODE); });
 // stateMachine.AddTransition(OPENED, CLOSING, [](){return input == xWAIT_DONE;  });
 // stateMachine.AddTransition(CLOSING, CLOSED, [](){return input == xCLOSED;     });
 // stateMachine.AddTransition(CLOSING, STOP_WAIT,[](){return input == xFTC;      });
 // stateMachine.AddTransition(STOP_WAIT, OPENING, [](){return input == xREOPEN;  });


    STATE_INPUT = Input::xINIT;
    STATE_MACHINE.Update();
    gSYSTEM_STATE = STATE_MACHINE.GetState();
 
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

    S1_Timer.begin(1000);
    S2_Timer.begin(1000);

}



//====================================
//              MAIN LOOP       
//====================================
void loop() 
{
    gADC_Update_Timer.tick();
    gLCD_Update_Timer.tick();    
     
     // debug test code...
     gLCD_DC_Volts = random(9,28);
     gLCD_AC_Volts = random(220, 240);  
     gLCD_AC_Amps  = random(0, 301)/10; 
     //....    

    
    
    if (STATE_MACHINE.Update()) 
    {
       gSYSTEM_STATE = STATE_MACHINE.GetState();
    }

}






