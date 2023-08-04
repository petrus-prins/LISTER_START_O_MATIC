#include <GLOBALS.h>


    //========================
    //      TIMERS
    //========================
    Timer <1, micros> gLCD_Update_Timer;         // create a timer with 1 task and microsecond resolution
    Timer <1, micros> gADC_Update_Timer;         // create a timer with 1 task and microsecond resolution

    //========================
    // LCD VARIABLES
    //========================
     volatile long gLCD_AC_Volts      = 0;                            // Used by LCD and DIO
     volatile long gLCD_AC_MilliAmps  = 0;
     volatile long gLCD_DC_MilliVolts = 0;
   

    //==============================
    // NO CURRENT CUTOFF DETECTION
    //==============================
     volatile bool gENGINE_IS_IDLE_FLAG = false; 
     volatile bool gZERO_CURRENT_TRIGGER_ACTIVE = true;             // Flag indicating that no AC current is used any more - i.e. OK to Switch Engine Off.
     volatile bool gZERO_Amps_Measured_Flag = true;
     volatile long gCUTOFF_ADC_Value_mV = 0; 

    //========================
    //  GLOBAL RELAY STATES
    //========================
    volatile bool gSTARTER_RELAY_ON     = false;               // True when relay is ON
    volatile bool gFUEL_RELAY_ON        = false;               // True when relay is ON
    volatile bool gSAFETY_RELAY_ON      = false;               // True when relay is ON
    volatile bool gSYSTEM_START_REQUEST = false;               // True when System_Start_Request_Digital_Input is True.
    
    //========================
    //  GLOBAL SYSTEM STATE
    //========================
    volatile uint8_t gSYSTEM_STATE = 0;



