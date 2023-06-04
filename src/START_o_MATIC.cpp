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
    LCD_Update_Timer.every(1000000,     Update_LCD);                     // 1000000 = 1 second
    ADC_Update_Timer.every(1000000/1000, Update_ALL_ADC_Values);         // 1/1000 of a second 
}


//====================================
//              SETUP       
//====================================
void setup() 
{
    INIT_EEPROM();
    INIT_I2C_LCD();
    INIT_DIGITAL_PIN_Modes();
    INIT_ANALOG_PIN_Modes();
    INIT_ADC_Stats();
    INIT_Timers();

    SET_RELAY1_KEEP_SYSTEM_ALIVE(true);               // Bridge 24V relay as to keep system alive.
    delay(1000);                                      // Wait a bit
    SET_RELAY2_DISABLE_24V_ON_AC(true);               // Then remove the 24V that is sent down the dormant 220VAC line.
    SET_RELAY3_ENABLE_FUEL_SOLENOID(false);           // Ensure that FUEL solenoid is not active
    SET_RELAY4_ENABLE_STARTER_MOTOR(false);           // Ensure that the starter motor is not active.
}

//====================================
//              MAIN LOOP       
//====================================
void loop() 
{
    ADC_Update_Timer.tick();
    LCD_Update_Timer.tick();    
    // LCD_AC_Volts = random(0,240);
    //LCD_AC_Volts = analogRead(PIN_A0);
     LCD_AC_Amps  = random(0, 301)/10;     
}






