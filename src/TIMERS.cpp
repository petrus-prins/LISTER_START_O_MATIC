#include <GLOBALS.h>




//====================================
//           INIT TIMERS      
//====================================
void INIT_Timers()
{
    gLCD_Update_Timer.every(1000000,      Update_LCD);                     // 1000000 = 1 second
    gADC_Update_Timer.every(1000000/1000, Update_ALL_ADC_Values);         // 1/1000 of a second 
}