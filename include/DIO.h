#ifndef HEADER_DIO
#define HEADER_DIO

//void RESET_AC_Voltage_Stats();
//bool Update_ALL_ADC_Values(void *);

void INIT_DIGITAL_PIN_Modes();




void SET_RELAY1_KEEP_SYSTEM_ALIVE(bool STATE);
void SET_RELAY2_DISABLE_24V_ON_AC(bool STATE);
void SET_RELAY3_ENABLE_FUEL_SOLENOID(bool STATE);
void SET_RELAY4_ENABLE_STARTER_MOTOR(bool STATE);

#endif