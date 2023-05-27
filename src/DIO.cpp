#include <GLOBALS.h>




// ==============================================================
//                       DIGITAL PIN DEFINES
//===============================================================


int RELAY1_PIN  =  PIN_PD4;
int RELAY2_PIN  =  PIN_PD5;
int RELAY3_PIN  =  PIN_PD6;
int RELAY4_PIN  =  PIN_PD7;



//=================================================================
//                      INIT DIGITAL PIN MODES      
//=================================================================
void INIT_DIGITAL_PIN_Modes()
{
    //pinMode(ledPin, OUTPUT);
    pinMode(RELAY1_PIN, OUTPUT);
    pinMode(RELAY2_PIN, OUTPUT);
    pinMode(RELAY3_PIN, OUTPUT);
    pinMode(RELAY4_PIN, OUTPUT);
}


//=================================================================
//                INIT DIGITAL PIN FUNCTIONS      
//=================================================================



void SET_RELAY1_KEEP_SYSTEM_ALIVE(bool STATE)
{
    digitalWrite(RELAY1_PIN, STATE);
}

void SET_RELAY2_DISABLE_24V_ON_AC(bool STATE)
{
    digitalWrite(RELAY2_PIN, STATE);
}

void SET_RELAY3_ENABLE_FUEL_SOLENOID(bool STATE)
{
    digitalWrite(RELAY3_PIN, STATE);
}

void SET_RELAY4_ENABLE_STARTER_MOTOR(bool STATE)
{
    digitalWrite(RELAY4_PIN, STATE);
}
