#include <GLOBALS.h>
#include <LiquidCrystal_PCF8574.h>

//===============================LCD ============================================
     int  LCD_Present = 0;
     char LCD_Buffer[21];                         // String Buffer For LCD Use
     char LCD_Lines[4][20];                       // 4 lines of 20 character buffer
     LiquidCrystal_PCF8574 LCD(0x27);             // set the LCD address to 0x27 for a 16 chars and 2 line display
//=================================================================================


// 220V
volatile float LCD_AC_Volts = 0.0;
volatile float LCD_AC_Amps  = 0.0;

// 24VDC
volatile float LCD_DC_Volts  = 0.0;
int AC_Voltage_Samples_Taken = 0;




//==========================240V STATS =============================================
volatile float AC_Voltage = 0;                     // Expecting 220-240VAC
volatile float DC_Voltage = 0;                     // Expecting 24-28VDC


//volatile float RMS_Voltage = 0;

//PHASE1
statistic::Statistic<float, uint32_t, true> AC_VOLTS_Raw;
statistic::Statistic<float, uint32_t, true> AC_VOLTS_Min;
statistic::Statistic<float, uint32_t, true> AC_VOLTS_Max;


statistic::Statistic<float, uint32_t, true> DC_VOLTS_Raw;
statistic::Statistic<float, uint32_t, true> DC_VOLTS_Max;
