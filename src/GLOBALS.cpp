#include <GLOBALS.h>
#include <LiquidCrystal_PCF8574.h>

//===============================LCD ============================================
     int  LCD_Present = 0;
     char LCD_Buffer[21];                         // String Buffer For LCD Use
     char LCD_Lines[4][20];                       // 4 lines of 20 character buffer
     LiquidCrystal_PCF8574 LCD(0x27);             // set the LCD address to 0x27 for a 16 chars and 2 line display
//=================================================================================


volatile float LCD_AC_Volts = 0.0;
volatile float LCD_AC_Amps  = 0.0; 


//==========================240V STATS =============================================
volatile float PHASE1_Voltage = 0;                     // Voltage
volatile float RMS_Voltage = 0;