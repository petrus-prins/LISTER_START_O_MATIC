#include <GLOBALS.h>
#include <LiquidCrystal_PCF8574.h>


//===============================LCD ============================================
     int  LCD_Present = 0;
     char LCD_Buffer[21];                         // String Buffer For LCD Use
     char LCD_Lines[4][20];                       // 4 lines of 20 character buffer
     LiquidCrystal_PCF8574 LCD(0x27);             // set the LCD address to 0x27 for a 16 chars and 2 line display
//=================================================================================


// 220V
volatile long LCD_AC_Volts = 0;
volatile long LCD_AC_Amps  = 0;
// 24VDC
volatile long LCD_DC_Volts  = 0;

//==========================240V STATS =============================================
volatile float AC_Voltage = 0;                     // Expecting 220-240VAC
volatile float DC_Voltage = 0;                     // Expecting 24-28VDC


//volatile float RMS_Voltage = 0;

//220_VAC STATS
statistic::Statistic<float, uint32_t, true> AC_VOLTS_Raw;
statistic::Statistic<float, uint32_t, true> AC_VOLTS_Min;
statistic::Statistic<float, uint32_t, true> AC_VOLTS_Max;
//24_VDC STATS
statistic::Statistic<float, uint32_t, true> DC_VOLTS_Raw;
statistic::Statistic<float, uint32_t, true> DC_VOLTS_Max;



    //========================
    //  GLOBAL RELAY STATES
    //========================
    volatile bool gSTARTER_RELAY_ON     = false;               // True when relay is ON
    volatile bool gFUEL_RELAY_ON        = false;               // True when relay is ON
    volatile bool gSAFETY_RELAY_ON      = false;               // True when relay is ON
    volatile bool gSYSTEM_START_REQUEST = false;               // True when System_Start_Request_Digital_Input is True.
    
    //========================
    //  GLOBAL SYSTEM STATE
    //========================
    volatile int gSYSTEM_STATE = 0;


