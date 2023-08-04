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

void INIT_RELAY_STATES()
{
    SET_RELAY1_KEEP_SYSTEM_ALIVE(true);               // Bridge 24V relay as to keep system alive.    
    SET_RELAY2_ENABLE_230VAC_LINE(false);             // Disconnect 230V AC line, connect 24V to acline in stead.
    SET_RELAY3_ENABLE_FUEL_SOLENOID(false);           // Ensure that FUEL solenoid is not active
    SET_RELAY4_ENABLE_STARTER_MOTOR(false);           // Ensure that the starter motor is not active.   
}



void SET_RELAY1_KEEP_SYSTEM_ALIVE(bool STATE)
{
    digitalWrite(RELAY2_PIN, STATE);
}

void SET_RELAY2_ENABLE_230VAC_LINE(bool STATE)
{
    digitalWrite(RELAY1_PIN, STATE);
    gSAFETY_RELAY_ON = STATE;
}

void SET_RELAY3_ENABLE_FUEL_SOLENOID(bool STATE)
{
    digitalWrite(RELAY3_PIN, STATE);
    gFUEL_RELAY_ON = STATE;
}

void SET_RELAY4_ENABLE_STARTER_MOTOR(bool STATE)
{
    digitalWrite(RELAY4_PIN, STATE);
    gSTARTER_RELAY_ON = STATE;
}
