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
    #include <firetimer.h>
    
    
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
    extern volatile long gLCD_DC_Volts;                  
    extern volatile long Fail_Count;                   //   STARTUP_SHUTDOWN FAIL COUNTER (increases if system reset)
    extern volatile long Total_System_Starts;          //   Increases by 1 on every system bootup
    
    //==============================
    // NO CURRENT CUTOFF DETECTION
    //==============================
    extern volatile bool gZERO_CURRENT_TRIGGER_ACTIVE;      //  True when gCUTOFF_ADC_Value_mV is >= 4.2V
    extern volatile long gCUTOFF_ADC_Value_mV;               
  


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
    extern volatile uint8_t gSYSTEM_STATE;




#endif