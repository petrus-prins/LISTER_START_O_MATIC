/************************************************************************
*    START-o-MATIC- Petrus Prins 2023
*************************************************************************/
#include <GLOBALS.h>


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
    gADC_Update_Timer.tick();
    gLCD_Update_Timer.tick();    
     
     // debug test code...
     gLCD_DC_Volts = random(9,28);
     gLCD_AC_Volts = random(220, 240);  
     gLCD_AC_Amps  = random(0, 301)/10; 
     gSYSTEM_STATE = random(0,59)/10;
     //....    
}






