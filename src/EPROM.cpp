#include <GLOBALS.h>
#include <EEPROMWearLevel.h>


#define EEPROM_LAYOUT_VERSION 1
#define AMOUNT_OF_INDEXES 2

#define INDEX_CONFIGURATION_VAR1 0
#define INDEX_CONFIGURATION_VAR2 1

//volatile long Startup_Count;
//volatile long Shutdown_Count;


void INIT_EEPROM()
{
    EEPROMwl.begin(EEPROM_LAYOUT_VERSION, AMOUNT_OF_INDEXES);
    readConfiguration();
    writeConfiguration();
}


void writeConfiguration() 
{
   //Startup_Count = 200;
   Startup_Count += 1;
   if (Startup_Count < 0) {Startup_Count = 0;}
   if (Startup_Count > 999999) {Startup_Count = 0;}
    
   EEPROMwl.put(INDEX_CONFIGURATION_VAR1, Startup_Count);
}

void readConfiguration() 
{
     EEPROMwl.get(INDEX_CONFIGURATION_VAR1, Startup_Count);
    //EEPROMwl.get(INDEX_CONFIGURATION_VAR2, Shutdown_Count);
}