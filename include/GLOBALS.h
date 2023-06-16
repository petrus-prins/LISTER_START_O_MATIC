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
    
    
    //===============
    // LCD VARIABLES
    //===============
    extern LiquidCrystal_PCF8574 LCD;             // set the LCD address to 0x27 for a 16 chars and 2 line display
    extern int  LCD_Present;
    extern volatile long LCD_AC_Volts;
    extern volatile long LCD_AC_Amps;
    extern volatile long LCD_DC_Volts;                //  
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




    ///==========================ANALOG PINS============================================
   // extern int AC_VOLTAGE_AI_Pin; // =  PIN_A0;    
   // extern int AC_CURRENT_AI_Pin; // =  PIN_A1;     
    //=================================================================================





    // Live Stats 
   // extern volatile float AC_Voltage;
   //  extern volatile float RMS_Voltage;

    extern int Voltage_Samples_Taken;
    extern statistic::Statistic<float, uint32_t, true> AC_VOLTS_Raw;
    extern statistic::Statistic<float, uint32_t, true> AC_VOLTS_Min;
    extern statistic::Statistic<float, uint32_t, true> AC_VOLTS_Max;
    extern statistic::Statistic<float, uint32_t, true> DC_VOLTS_Raw;
    extern statistic::Statistic<float, uint32_t, true> DC_VOLTS_Max;


#endif