#ifndef HEADER_GLOBALS
#define HEADER_GLOBALS

    #include <Arduino.h>
    #include <DIO.h>
    #include <ADC.h>
    #include <LCD.h> 
    #include <Wire.h>
    #include <arduino-timer.h>
    #include <Statistic.h>      
    #include <math.h>
    #include <LiquidCrystal_PCF8574.h>
    #include <EPROM.h>
    #include <YA_FSM.h>
    #include <TIMERS.h>
    
    
    //========================
    //         TIMERS
    //========================
    extern Timer <1, micros> gLCD_Update_Timer;         // create a timer with 1 task and microsecond resolution
    extern Timer <1, micros> gADC_Update_Timer;         
    
    //========================
    //     LCD VARIABLES
    //========================
    extern volatile long gLCD_AC_Volts;
    extern volatile long gLCD_AC_Amps;
    extern volatile long gLCD_DC_Volts;                //  
    extern volatile long Fail_Count;                   // STARTUP_SHUTDOWN FAIL COUNTER
    extern volatile long Total_System_Starts;          // 
 
    //========================
    //  GLOBAL RELAY STATES
    //========================
    extern volatile bool gSTARTER_RELAY_ON;
    extern volatile bool gFUEL_RELAY_ON;
    extern volatile bool gSAFETY_RELAY_ON;
    extern volatile bool gSYSTEM_START_REQUEST;

    //========================
    //  GLOBAL SYSTEM STATE
    //========================
    extern volatile int gSYSTEM_STATE;

    //========================
    //   Live Voltage Stats
    //========================
    extern statistic::Statistic<float, uint32_t, true> gAC_VOLTS_Raw;
    extern statistic::Statistic<float, uint32_t, true> gAC_VOLTS_Min;
    extern statistic::Statistic<float, uint32_t, true> gAC_VOLTS_Max;
    extern statistic::Statistic<float, uint32_t, true> gDC_VOLTS_Raw;
    extern statistic::Statistic<float, uint32_t, true> gDC_VOLTS_Max;


#endif