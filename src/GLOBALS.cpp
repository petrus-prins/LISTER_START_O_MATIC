#include <GLOBALS.h>


    //========================
    //      TIMERS
    //========================
    Timer <1, micros> gLCD_Update_Timer;         // create a timer with 1 task and microsecond resolution
    Timer <1, micros> gADC_Update_Timer;         // create a timer with 1 task and microsecond resolution

    //========================
    // LCD VARIABLES
    //========================
     volatile long gLCD_AC_Volts = 0;                        // Used by LCD and DIO
     volatile long gLCD_AC_Amps  = 0;
     volatile long gLCD_DC_Volts  = 0;

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
    volatile int gSYSTEM_STATE = 0;

    //========================
    // Live Voltage Stats
    //========================
    statistic::Statistic<float, uint32_t, true> gAC_VOLTS_Raw;
    statistic::Statistic<float, uint32_t, true> gAC_VOLTS_Min;
    statistic::Statistic<float, uint32_t, true> gAC_VOLTS_Max;
    statistic::Statistic<float, uint32_t, true> gDC_VOLTS_Raw;
    statistic::Statistic<float, uint32_t, true> gDC_VOLTS_Max;

