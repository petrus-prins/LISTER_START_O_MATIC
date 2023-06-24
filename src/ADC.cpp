#include <GLOBALS.h>



int AC_VOLTAGE_AI_Pin       =  PIN_A0;    
int AC_CURRENT_AI_Pin       =  PIN_A1;  
int CURRENT_CUTOFF_AI_Pin   =  PIN_A2;                     // Cut_off_Signal when power draw is less than 40 Watts.
int DC_VOLTAGE_AI_Pin       =  PIN_A3;

float VREF = 5.0;

    
//========================
// Live Voltage Stats
//========================    
    
//AC VOLTS
statistic::Statistic<float, uint32_t, true> gAC_VOLTS_Raw;
statistic::Statistic<float, uint32_t, true> gAC_VOLTS_Min;
statistic::Statistic<float, uint32_t, true> gAC_VOLTS_Max;
//DC VOLTS
statistic::Statistic<float, uint32_t, true> gDC_VOLTS_Raw;
statistic::Statistic<float, uint32_t, true> gDC_VOLTS_Max;
//AC CURRENT
statistic::Statistic<float, uint32_t, true> CUTTOFF_Raw;
statistic::Statistic<float, uint32_t, true> CUTTOFF_Max;

//volatile long CUTOFF_ADC_Value_mV = 0;   


//=========================================================================
//                           INIT ANALOG PIN MODES      
//=========================================================================
void INIT_ANALOG_PIN_Modes()
{
    pinMode(AC_VOLTAGE_AI_Pin, INPUT);
    pinMode(AC_CURRENT_AI_Pin, INPUT);
    pinMode(CURRENT_CUTOFF_AI_Pin, INPUT);
    pinMode(DC_VOLTAGE_AI_Pin, INPUT);
}





// RESET ACV
void RESET_AC_Voltage_Stats()
{
    gAC_VOLTS_Raw.clear();
    gAC_VOLTS_Min.clear();
    gAC_VOLTS_Max.clear();
}

// RESET DCV
void RESET_DC_Voltage_Stats()
{
    gDC_VOLTS_Raw.clear();
    gDC_VOLTS_Max.clear();
}

// RESET CUTOFF
void RESET_CUTOFF_Stats()
{
    CUTTOFF_Raw.clear();
    CUTTOFF_Max.clear();
}


// CALLED BY SETUP
void INIT_ADC_Stats()
{
    RESET_AC_Voltage_Stats();
    RESET_DC_Voltage_Stats();
    RESET_CUTOFF_Stats();
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
    if (fval < 1.0) {fval = 0.0;}        
    if (fval > 999) {fval = 999;}          // Sanity check values are between 0 and 999. 
    return fval;
}



//=============================================================
//                  AC220V
//==============================================================
void Update_ADC_220VAC_VOLTAGE_Stats(void)
{
    // AC VOLTAGE 
    float AC_Voltage = 0;
    gAC_VOLTS_Raw.add(analogRead(AC_VOLTAGE_AI_Pin));
    if (gAC_VOLTS_Raw.count() > 100)                               // Take 100 Samples then start guessing min and max from them
    {
        gAC_VOLTS_Min.add(gAC_VOLTS_Raw.minimum());                            
        gAC_VOLTS_Max.add(gAC_VOLTS_Raw.maximum());
    } 
    
    if (gAC_VOLTS_Raw.count() > 500)                               // 500 Samples should be enough for us to calculate voltage.                   
    {
        AC_Voltage = Calculate_Real_AC_Voltage(gAC_VOLTS_Raw.maximum(),gAC_VOLTS_Raw.minimum());
        gLCD_AC_Volts = (long) AC_Voltage;
        RESET_AC_Voltage_Stats();                                 // Kill all stats. 
    }
}



//=============================================================
//                     DC24V 
//==============================================================
void Update_ADC_24VDC_Stats(void)
{ 
    float local_DC_Voltage = 0;
    gDC_VOLTS_Raw.add(analogRead(DC_VOLTAGE_AI_Pin));
    
    if (gDC_VOLTS_Raw.count() > 100)                               // 100 samples are can start giving proper max stats.                  
    {
         gDC_VOLTS_Max.add(gDC_VOLTS_Raw.maximum());
    }

    if (gDC_VOLTS_Raw.count() > 500)                               // 500 Samples should be enough for us to calculate voltage.                   
     {
        local_DC_Voltage = gDC_VOLTS_Max.average();
        local_DC_Voltage = (local_DC_Voltage*VREF/1024.0)*11.2 +0.62;   // Protecton Diode offset is 0.62 volt.
        if (local_DC_Voltage < 0.7) {local_DC_Voltage = 0.0;}              // Diode drop
        if (local_DC_Voltage > 99.0) {local_DC_Voltage = 99.0;}
        gLCD_DC_Volts =  (long) local_DC_Voltage;            
        RESET_DC_Voltage_Stats();                                  // Reset 24V stats.
     }                 
}




//=============================================================
//                  CURRENT CUTOFF STATS 
//==============================================================
void Update_CUTOFF_Stats(void)
{ 
    
    float local_CUTOFF_Voltage = 0;

    CUTTOFF_Raw.add(analogRead(CURRENT_CUTOFF_AI_Pin));
    if (CUTTOFF_Raw.count() > 100)                                                 
    {
         CUTTOFF_Max.add(CUTTOFF_Raw.maximum());
    }

    if (CUTTOFF_Raw.count() > 500)             
     {
        local_CUTOFF_Voltage = CUTTOFF_Max.average();
        local_CUTOFF_Voltage = (local_CUTOFF_Voltage*VREF/1024.0);         
        if (local_CUTOFF_Voltage < 0.0) {local_CUTOFF_Voltage = 0.0;}           
        if (local_CUTOFF_Voltage >= VREF) {local_CUTOFF_Voltage = VREF;}
        gCUTOFF_ADC_Value_mV =  (long) (local_CUTOFF_Voltage*1000);
        gZERO_CURRENT_TRIGGER_ACTIVE  =   (gCUTOFF_ADC_Value_mV >= 4200)? true : false;           // =4800mV when no current is drawn from system; 3300mV when 40Watt@220V is used. 
        RESET_CUTOFF_Stats();
     }                 
}










//====================================================
//  Read and Store All ADC Live System Values
//====================================================
bool Update_ALL_ADC_Values(void *)
{   
    Update_ADC_220VAC_VOLTAGE_Stats();
    Update_ADC_24VDC_Stats();
    Update_CUTOFF_Stats();
    // TODO: Update_ADC_220VAC_AMPS_Stats

    return true;                                                    // Retun True if this function must be called next time by timer lbrary.
}



