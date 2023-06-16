/************************************************************************
*    START-o-MATIC- Petrus Prins 2023
*************************************************************************/
#include <GLOBALS.h>


//================================TIMERS===========================================
Timer <1, micros> LCD_Update_Timer;         // create a timer with 1 task and microsecond resolution
Timer <1, micros> ADC_Update_Timer;         // create a timer with 1 task and microsecond resolution
//=================================================================================

//====================================
//           INIT TIMERS      
//====================================
void INIT_Timers()
{
    LCD_Update_Timer.every(1000000,      Update_LCD);                     // 1000000 = 1 second
    ADC_Update_Timer.every(1000000/1000, Update_ALL_ADC_Values);         // 1/1000 of a second 
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
}

//====================================
//              MAIN LOOP       
//====================================
void loop() 
{
    ADC_Update_Timer.tick();
    LCD_Update_Timer.tick();    
     
     // debug test code...
     LCD_DC_Volts = random(9,28);
     LCD_AC_Volts = random(220, 240);  
     LCD_AC_Amps  = random(0, 301)/10; 
     gSYSTEM_STATE = random(0,59)/10;
     //....    
}






