/************************************************************************
*    START-o-MATIC- Petrus Prins 2023
*************************************************************************
* Description: AVR
* 
*     NOTES:
*     1. ATMEGA328P - (DIP28)
*     2. 
*
************************************************************************/


/************************************************************************
*    START-o-MATIC- Petrus Prins 2023
*************************************************************************
* Hardware Setup: Original UNO_R3 With Atmega328P (5V)
* 
*     PIN_MAPS:
*
*        PIN_14_PB0 : Output Relay1_Control
*        PIN_15_PB1 : Output Relay2_Control 
*        PIN_16_PB2 : Output Relay3_Control     
*        PIN_17_PB3 : Output Relay4_Control
*        
*        PIN_18_PB4 : Output Keep Alive NFET ?
*
*        PIN_23_PC0_ADC0 : Input: 220V AC Current Measurement VIA  
*        PIN_24_PC1_ADC1 : Input: Alternator 220VAC Measurement via ZMPT101B Module 
*                 
*        I2C_SDA (PC4) AT328P_PIN_27
*        I2C_SCL (PC5) AT328P_PIN_28
*
*
*
************************************************************************/


#include <GLOBALS.h>


//================================TIMERS===========================================

Timer <1, micros> LCD_Update_Timer;         // create a timer with 1 task and microsecond resolution
Timer <1, micros> ADC_Update_Timer;         // create a timer with 1 task and microsecond resolution

//=================================================================================





//================================ATOD TIMER========================================
unsigned long ATOD_Prev_ms = 0;              
const long ATOD_Interval_ms = 200;
float VREF = 5.00;                       // Supply Voltage            
//=================================================================================


//===============================LCD===============================================
//const int LCD_RX_Pin = PIN_PD2;
//const int LCD_TX_Pin = PIN_PD3;
//char LCD_LINE0[20];
char buff[20];
//char SERIAL_BUFFER1[20];
//char SERIAL_BUFFER2[20];
float fval;
//=================================================================================




//==========================================
//             PROCESS ATOD      
//==========================================
void Process_ATOD()
{
    unsigned long Current_ms = millis();
    if (Current_ms - ATOD_Prev_ms >= ATOD_Interval_ms) 
    {
        ATOD_Prev_ms = Current_ms;
        fval = analogRead(AC_CURRENT_AI_Pin)*5.0/1024;
        dtostrf(fval,4,2,buff);
    }
}



//====================================
//           INIT TIMERS      
//====================================
void INIT_Timers()
{
    LCD_Update_Timer.every(1000000,     Update_LCD);                     // 1000000 = 1 second
    ADC_Update_Timer.every(1000000/1000, Update_ALL_ADC_Values);         // 1/1000 of a second 
}


void INIT_Stats()
{
    RESET_AC_Voltage_Stats();
}



//====================================
//              SETUP       
//====================================
void setup() 
{
    INIT_I2C_LCD();
    INIT_DIGITAL_PIN_Modes();
    INIT_ANALOG_PIN_Modes();
    INIT_Stats();
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


////////////////////////////////
// TODO:
//
//  1: Add Peripherals
//        -  Analog Input Current Sensor
//        -  Analog Voltage Sensor
//        -  Analog Current Draw Threshold Input 
//        -  24V Battery Voltage         
//
//  2: Add Digital Inputs
//        -  24V Battery Voltage
//
// 3:  Add Digintal Outputs



