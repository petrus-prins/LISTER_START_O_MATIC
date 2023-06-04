#ifndef HEADER_EPROM
#define HEADER_EPROM
    void READ_Startup_Stats_From_EEPROM();
    void Sanity_Check_Shutdown_Stats_Before_Save();
    void WRITE_New_Startup_Count_To_EEPROM();
    void WRITE_New_Shutdown_Count_To_EEPROM();
    void INIT_EEPROM();
#endif