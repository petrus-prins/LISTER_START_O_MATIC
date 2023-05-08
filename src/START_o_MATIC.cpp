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

//=================================================================================


//==========================ANALOG PINS============================================
int AC_CURRENT_AI_Pin  =  PIN_A0;              // AT328P_PIN_23
int AC_VOLTAGE_AI_Pin  =  PIN_A1;              // AT328P_PIN_24
//=================================================================================

//===============================DIGITAL PINS======================================
int RELAY1_PIN  =  PIN_PB0;              // AT328P_PIN_14
int RELAY2_PIN  =  PIN_PB1;              // AT328P_PIN_15
int RELAY3_PIN  =  PIN_PB2;              // AT328P_PIN_16
int RELAY4_PIN  =  PIN_PB3;              // AT328P_PIN_17

//=================================================================================


//=====================================LEDs========================================
const int ledPin =  PIN_PB5;             // AT328P_PIN_19
int ledState = LOW;
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


//==========================240V STATS =============================================
float PHASE1_Voltage = 0;                     // Voltage
float RMS_Voltage = 0;


//PHASE1
statistic::Statistic<float, uint32_t, true> PHASE1_Raw;
statistic::Statistic<float, uint32_t, true> PHASE1_Min;
statistic::Statistic<float, uint32_t, true> PHASE1_Max;


 

//=================================================================================




void SET_RELAY1(bool STATE)
{
    digitalWrite(RELAY1_PIN, STATE);
}

void SET_RELAY2(bool STATE)
{
    digitalWrite(RELAY2_PIN, STATE);
}

void SET_RELAY3(bool STATE)
{
    digitalWrite(RELAY3_PIN, STATE);
}


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




//==========================================
//             PROCESS LED STATES      
//==========================================
void Process_LEDS()
{
    if (ledState == LOW)    {   ledState = HIGH;    }
    else                    {   ledState = LOW;     }
    digitalWrite(ledPin, ledState);
}





/*
 float Calculate_Phase_Voltage(float ADC_Max, float ADC_Min)

 Description:
 ------------
    Calculate line voltage based on MAX and MIN ADC values.
    ADC has constant DC offset with AC ponent on top.
    Mesurement shows that 210 ADC points == 236V.  

 Inputs:
 -------
    float ADC_Max, float ADC_Min

 Return:
 -------
    float - correctly scaled AC voltage in VOLTS.
*/

//===============================================================================
//             CALCUALTE PHASE VOLTAGE
//             Calculate AC LN voltage based on max and min ADC values.
//             ADCmin-ADCmax =  210 ADC points = 236V.  
//             ADCmin-ADCmax =  182 ADC points = 226V.
//===============================================================================
float Calculate_Phase_Voltage(float ADC_Max, float ADC_Min)
{
    float Volt_Error_Offset = 5.0;
    float fval = 0;
    fval = fabs(ADC_Max-ADC_Min);           //  ADC difference in max and min
    fval = fval * VREF;                     // * 5.05V
    fval = fval / 1024.0;                   // convert ADC to Volts So AC part of signal p-p voltage. 226VAC is equal to ==> 0.9VADC
    fval = fval * 240;                      // Constant to scale ADCV to ACV
    fval = fval-Volt_Error_Offset;
    if (fval < 1.0) {fval = 0;}
    return fval;
}



int Read_ADC_Average(int ADC_PIN, int num_samples = 1)
{
    int local_ADC = 0;
    for (int i=1; i <= num_samples; i++)
    {
        local_ADC += analogRead(ADC_PIN);
    }
    local_ADC = local_ADC/num_samples;
    return local_ADC;  
}



void READ_PHASE_Voltage_Stats()
{
    PHASE1_Voltage = Read_ADC_Average(AC_VOLTAGE_AI_Pin,1); 
    PHASE1_Raw.add(PHASE1_Voltage);
    PHASE1_Min.add(PHASE1_Raw.minimum());
    PHASE1_Max.add(PHASE1_Raw.maximum());
}


float CALCULATE_RMS_Voltage_From_Stats()
{
    float PHASE1 = Calculate_Phase_Voltage(PHASE1_Max.average(), PHASE1_Min.average());
    //strcpy(SERIAL_BUFFER1,"");
    //strcat(SERIAL_BUFFER1, "[P1: ");
    //dtostrf(PHASE1,3,0,SERIAL_BUFFER2);
    //strcat(SERIAL_BUFFER1, SERIAL_BUFFER2);
    //strcat(SERIAL_BUFFER1, "V] ");
    float RMS = sqrt(square(PHASE1));   // 380V
    return RMS/sqrt(2);    // 220V
}



void CLEAR_PHASE_Voltage_Stats()
{
        PHASE1_Raw.clear();
        PHASE1_Min.clear();
        PHASE1_Max.clear();
}



//===============================================================================
//             PROCESS VOLTAGE
//===============================================================================
void Process_VOLTAGE()
{
    
    uint32_t  Sample_count = 0;
    
    READ_PHASE_Voltage_Stats();
    RMS_Voltage = CALCULATE_RMS_Voltage_From_Stats();
    Sample_count = PHASE1_Raw.count();
    Sample_count = Sample_count;   // get rid of warning for now
    CLEAR_PHASE_Voltage_Stats();
}


//====================================
//         WIP SCRATCH PAD    
//====================================
void Process_Scratch_Pad()
{
    float t = millis()/1000.0; // get program time
    t = t;// get rid of warning
    float var_sin = RMS_Voltage;
    var_sin = var_sin; // remove warning.
}



//====================================
//         PROCESS TIMERS    
//====================================
void Process_Timers()
{
    LCD_Update_Timer.tick();                          // tick the timer
}



//====================================
//           INIT PIN MODES      
//====================================
void INIT_PIN_Modes()
{
    pinMode(ledPin, OUTPUT);
    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);
    pinMode(RELAY3_PIN, OUTPUT); 
}

//====================================
//           INIT TIMERS      
//====================================
void INIT_Timers()
{
    unsigned long delta_us = 1000000;
    LCD_Update_Timer.every(delta_us, Update_LCD);                 
}


//====================================
//              SETUP       
//====================================
void setup() 
{
    INIT_I2C_LCD();
    INIT_PIN_Modes();
    INIT_Timers();
}

//====================================
//              MAIN LOOP       
//====================================
void loop() 
{
     Process_Timers();
     Process_VOLTAGE();                     // Aquire Voltage Samples
     Process_Scratch_Pad();
     LCD_AC_Volts = random(0,240);
     LCD_AC_Amps  = random(0, 301)/10; 

           
}













