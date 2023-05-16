#include <GLOBALS.h>





void RESET_AC_Voltage_Stats()
{
        AC_VOLTS_Raw.clear();
        AC_VOLTS_Min.clear();
        AC_VOLTS_Max.clear();
}



//===============================================================================
//             CALCUALTE PHASE VOLTAGE
//             Calculate AC LN voltage based on max and min ADC values.
//             ADCmin-ADCmax =  210 ADC points = 236V.  
//             ADCmin-ADCmax =  182 ADC points = 226V.
//===============================================================================
float Calculate_Real_AC_Voltage(float ADC_Max, float ADC_Min)
{
    float VREF = 5.0;
    float Volt_Error_Offset = 5.0;
    float fval = 0;
    fval = fabs(ADC_Max-ADC_Min);           //  ADC difference in max and min
    fval = fval * VREF;                     // * 5.05V
    fval = fval / 1024.0;                   // convert ADC to Volts So AC part of signal p-p voltage. 226VAC is equal to ==> 0.9VADC
    fval = fval * 115;                      // Constant to scale ADCV to ACV
    fval = fval-Volt_Error_Offset;
    if (fval < 1.0) {fval = 0;}
    return fval;
}





//====================================================
//  Read and Store All ADC Live System Values
//====================================================
bool Update_ADC(void *)
{
    
    float AC_Voltage = 0;
    // LCD_AC_Volts = ((((float)analogRead(AC_VOLTAGE_AI_Pin))/1024.0)*5.0)*110; 
    // AC VOLTAGE STATS
    AC_VOLTS_Raw.add(analogRead(AC_VOLTAGE_AI_Pin));
    if (AC_VOLTS_Raw.count() > 100)                               // Take 100 Samples then 
    {
        AC_VOLTS_Min.add(AC_VOLTS_Raw.minimum());                            
        AC_VOLTS_Max.add(AC_VOLTS_Raw.maximum());
    } 
    
    if (AC_VOLTS_Raw.count() > 500)                              // After 1000 clear stats
    {
        AC_Voltage = Calculate_Real_AC_Voltage(AC_VOLTS_Raw.maximum(),AC_VOLTS_Raw.minimum());
        RESET_AC_Voltage_Stats();  
        LCD_AC_Volts = AC_Voltage;
    }

    return true;    // Retun True if this function must be called next time by timer lbrary.
}