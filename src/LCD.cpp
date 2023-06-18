#include <GLOBALS.h>


//========================================================
//         DIGITAL PINS USED FOR I2C LCD COMMS
//========================================================
//             I2C_SDA (PC4) AT328P_PIN_27 (ADC4)
//             I2C_SCL (PC5) AT328P_PIN_28 (ADC5)
//========================================================

LiquidCrystal_PCF8574 LCD(0x27);             // set the LCD address to 0x27 for a 16 chars and 2 line display
char LCD_Line[]     = "                     ";            // 21 chars
int  LCD_Present = 0;


//====================================
//         INIT LCD      
//====================================
void INIT_I2C_LCD()
{
    Wire.begin();                                // Try to connect to LCD
    Wire.beginTransmission(0x27);
    int LCD_Error = Wire.endTransmission();
    if (LCD_Error == 0) { LCD_Present = 1;}     // LCD Is Present Flag Set.
    if(LCD_Present == 1)                        // LCD OK
    {
        LCD.begin(20, 4);                       // initialize the lcd
        LCD.home();                             // Bring cursor home [0,0] 
        LCD.clear();                            // Clear LCD
        LCD.noBlink();                          // Setup Cursor to (not)Blink
        LCD.setBacklight(255);                  // Set Backlight ON
        // LINE 1
        LCD.setCursor(0, 0);                    // Line 1
        LCD.print("[SYSTEM STARTED]");
        // LINE 2
        LCD.setCursor(0, 1);                    // Line 2
        LCD.print("START-o-MATIC v1.0");
        // LINE3 
        sprintf(LCD_Line, "Resets:       %6ld", Fail_Count);  LCD_Line[20] = '\0'; 
        LCD.setCursor(0, 2);     
        LCD.print(LCD_Line);
        // LINE4
        sprintf(LCD_Line, "Startups:     %6ld", Total_System_Starts);  LCD_Line[20] = '\0'; 
        LCD.setCursor(0, 3);     
        LCD.print(LCD_Line);
        delay(10000/10);   
    } 
}


//==========================================================
//         CONTINUOUSLY UPDATE SYSTEM STATS ON LCD   
//==========================================================
bool Update_LCD(void *)
{
    if(LCD_Present == 1)                                       // LCD OK
    { 
    
        //----------------+------------------+-
        // LINE 1  ==>   "230V  25A   Batt:24V"
        //----------------+------------------+-    
        sprintf(LCD_Line, "%3ldV %2ldA   Batt: %2ldV",gLCD_AC_Volts, gLCD_AC_Amps, gLCD_DC_Volts);
        LCD.setCursor(0, 0); 
        LCD.print(LCD_Line);
    
        //----------------+------------------+-
        // LINE 2  ==>   "Starter:OFF Fuel:OFF"
        //----------------+------------------+-    
        sprintf(LCD_Line, "Starter:%s Fuel:%s",(gSTARTER_RELAY_ON) ? "ON " : "OFF",(gFUEL_RELAY_ON) ? "ON " : "OFF");
        LCD.setCursor(0, 1); 
        LCD.print(LCD_Line);
    
        //----------------+------------------+-
        // LINE 3  ==>   "Safe:OFF Request:OFF"
        //----------------+------------------+-  
        sprintf(LCD_Line, "Safe:%s Request:%s",(gSAFETY_RELAY_ON) ? "ON " : "OFF",(gSYSTEM_START_REQUEST) ? "ON " : "OFF");
        LCD.setCursor(0, 2); 
        LCD.print(LCD_Line);
    
        //----------------+------------------+-
        // LINE 4  ==>   "S1: DISOVERY MODE"
        //----------------+------------------+-  
        switch(gSYSTEM_STATE) 
        {
            case 1:             // +------------------+
                sprintf(LCD_Line, "S1: DISCOVERY MODE  ");
            break;
            case 2:
                sprintf(LCD_Line, "S2: ENGINE STARTING ");
            break;
            case 3:
                sprintf(LCD_Line, "S3: ENGINE RUNNING  ");
            break;
            case 4:
                sprintf(LCD_Line, "S4: ENGINE SHUTDOWN ");
            break;
            case 5:
                sprintf(LCD_Line, "S5: SYSTEM SHUTDOWN ");
            break;
            default:
                sprintf(LCD_Line, "S0: INIT            ");
        }

        LCD.setCursor(0, 3); 
        LCD.print(LCD_Line);
    
    } //LCD OK
    
    return true;                                               // Retun True if this function must be called next time by timer lbrary.
}