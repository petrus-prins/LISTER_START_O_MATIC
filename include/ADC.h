#ifndef HEADER_ADC
#define HEADER_ACD


//extern int AC_VOLTAGE_AI_Pin;    
//extern int AC_CURRENT_AI_Pin;  
//extern int DC_VOLTAGE_AI_Pin;


void INIT_ANALOG_PIN_Modes();
void RESET_AC_Voltage_Stats();
bool Update_ALL_ADC_Values(void *);
// int  Read_ADC_Average(int ADC_PIN, int num_samples);
void INIT_ADC_Stats();


#endif