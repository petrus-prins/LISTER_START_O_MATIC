#include <GLOBALS.h>
#include <EEPROMWearLevel.h>


#define EEPROM_LAYOUT_VERSION 1
#define AMOUNT_OF_INDEXES 2

#define INDEX_CONFIGURATION_VAR1 0
#define INDEX_CONFIGURATION_VAR2 1

long Startup_Count;
long Shutdown_Count;
volatile long Fail_Count;
volatile long Total_System_Starts; 



// MUST BE CALLED ON STARTUP OF SYSTEM
void INIT_EEPROM()
{
    EEPROMwl.begin(EEPROM_LAYOUT_VERSION, AMOUNT_OF_INDEXES);
    READ_Startup_Stats_From_EEPROM();
    Sanity_Check_Shutdown_Stats_Before_Save();                     
    WRITE_New_Startup_Count_To_EEPROM();                                         // Increase System Startup Count (Used in INIT state)
    WRITE_New_Shutdown_Count_To_EEPROM();                                        // Prevent Failure Count From Incrementing  (Used In Shutdown State)
    (void) EEPROM; // just shut up warning about not using EEPROM
}

// READ EEPROM STARTUP AND SHITDOWN STATS.
void READ_Startup_Stats_From_EEPROM() 
{
     EEPROMwl.get(INDEX_CONFIGURATION_VAR1, Startup_Count);
     EEPROMwl.get(INDEX_CONFIGURATION_VAR2, Shutdown_Count);
}


// SAVE STARTUP COUNT
void WRITE_New_Startup_Count_To_EEPROM() 
{
   Startup_Count = Startup_Count + 1;
   EEPROMwl.put(INDEX_CONFIGURATION_VAR1, Startup_Count);
}


// SAVE SHUTDOWN COUNT
void WRITE_New_Shutdown_Count_To_EEPROM() 
{
   Shutdown_Count = Shutdown_Count + 1;
   EEPROMwl.put(INDEX_CONFIGURATION_VAR2, Shutdown_Count);
}


// CEHCK STATS - limit all values to between 0 and 999999
void Sanity_Check_Shutdown_Stats_Before_Save()
{
    if ((Startup_Count < 0) || (Startup_Count > 999999) ||          // if any count is out of bounds then reset both counts.
        (Shutdown_Count < 0) || (Shutdown_Count > 999999) ||        
        (Shutdown_Count > Startup_Count))                           // Shutdown may never be more than startip. Logcally not possible.
    {
        Shutdown_Count = 0; 
        Startup_Count = 0;
    }
    
     Total_System_Starts = Startup_Count;
     Fail_Count = Startup_Count - Shutdown_Count;                   // Failcount will increase when system is reset and never reached its final state.
}

