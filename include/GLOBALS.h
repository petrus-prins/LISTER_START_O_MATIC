#ifndef HEADER_GLOBALS
#define HEADER_GLOBALS

    #include <Arduino.h>
    #include <LCD.h>
    #include <Wire.h>
    #include <arduino-timer.h>
    #include <Statistic.h>      
    #include <math.h>
    #include <LiquidCrystal_PCF8574.h>
    
    
    //===============
    // LCD VARIABLES
    //===============
    extern LiquidCrystal_PCF8574 LCD;             // set the LCD address to 0x27 for a 16 chars and 2 line display
    extern int  LCD_Present;
    extern char LCD_Buffer[21];                         // String Buffer For LCD Use
    extern char LCD_Lines[4][20];                       // 4 lines of 20 character buffer
    extern volatile float LCD_AC_Volts;
    extern volatile float LCD_AC_Amps; 


    extern char someLine1[20];  // 20 chars
    extern char someLine2[20];  // 20 chars
    extern char someLine3[20];  // 20 chars
    extern char someLine4[20];  // 20 chars



#endif