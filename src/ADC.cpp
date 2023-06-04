#include <GLOBALS.h>



int AC_VOLTAGE_AI_Pin  =  PIN_A0;    
int AC_CURRENT_AI_Pin  =  PIN_A1;  
int DC_VOLTAGE_AI_Pin  =  PIN_A3;

float VREF = 5.0;

//=========================================================================
//                           INIT ANALOG PIN MODES      
//=========================================================================
void INIT_ANALOG_PIN_Modes()
{
    pinMode(AC_VOLTAGE_AI_Pin, INPUT);
}


void INIT_ADC_Stats()
{
    RESET_AC_Voltage_Stats();
}


void RESET_AC_Voltage_Stats()
{
    AC_VOLTS_Raw.clear();
    AC_VOLTS_Min.clear();
    AC_VOLTS_Max.clear();
}



void RESET_DC_Voltage_Stats()
{
    DC_VOLTS_Raw.clear();
    DC_VOLTS_Max.clear();
}



//===============================================================================
//             Calculate AC LN voltage based on max and min ADC values.
//===============================================================================
float Calculate_Real_AC_Voltage(float ADC_Max, float ADC_Min)
{
    float Volt_Error_Offset = 0.0;
    float fval = 0;
    fval = fabs(ADC_Max-ADC_Min);           //  ADC difference in max and min
    fval = fval * VREF;                     // * 5.00V
    fval = fval / 1024.0;                   // convert ADC to Volts So AC part of signal p-p voltage. 226VAC is equal to ==> 0.9VADC
    fval = fval * 122;                      // Constant to scale ADC 2VACpp to 230VACpp
    fval = fval-Volt_Error_Offset;
    if (fval < 1.0) {fval = 0;}
    return fval;
}



//=============================================================
//                  AC220V
//==============================================================
void Update_ADC_220VAC_VOLTAGE_Stats(void)
{
    // AC VOLTAGE 
    float AC_Voltage = 0;
    AC_VOLTS_Raw.add(analogRead(AC_VOLTAGE_AI_Pin));
    if (AC_VOLTS_Raw.count() > 100)                               // Take 100 Samples then start guessing min and max from them
    {
        AC_VOLTS_Min.add(AC_VOLTS_Raw.minimum());                            
        AC_VOLTS_Max.add(AC_VOLTS_Raw.maximum());
    } 
    
    if (AC_VOLTS_Raw.count() > 500)                               // 500 Samples should be enough for us to calculate voltage.                   
    {
        AC_Voltage = Calculate_Real_AC_Voltage(AC_VOLTS_Raw.maximum(),AC_VOLTS_Raw.minimum());
        LCD_AC_Volts = AC_Voltage;
        RESET_AC_Voltage_Stats();                                 // Kill all stats. 
    }
}



//=============================================================
//                     DC24V 
//==============================================================
void Update_ADC_24VDC_Stats(void)
{ 
    float local_DC_Voltage = 0;
    DC_VOLTS_Raw.add(analogRead(DC_VOLTAGE_AI_Pin));
    
    if (DC_VOLTS_Raw.count() > 100)                               // 500 Samples should be enough for us to calculate voltage.                   
    {
         DC_VOLTS_Max.add(DC_VOLTS_Raw.maximum());
    }

    if (DC_VOLTS_Raw.count() > 500)                               // 500 Samples should be enough for us to calculate voltage.                   
     {
        local_DC_Voltage = DC_VOLTS_Max.average();
        LCD_DC_Volts = (local_DC_Voltage*5.0/1024.0)*11.2 +0.62;   // Protecton Diode offset is 0.62 volt.
        if (LCD_DC_Volts <= 0.7) {LCD_DC_Volts = 0.0;}
        RESET_DC_Voltage_Stats();                                  // Reset 24V stats.
     }
                   
}




//====================================================
//  Read and Store All ADC Live System Values
//====================================================
bool Update_ALL_ADC_Values(void *)
{   
    Update_ADC_220VAC_VOLTAGE_Stats();
    Update_ADC_24VDC_Stats();
    // TODO: Update_ADC_220VAC_AMPS_Stats
    // TODO: Update_ADC_220VAC_CURRENT_SwitchOff_Trigger
    return true;                                                    // Retun True if this function must be called next time by timer lbrary.
}



