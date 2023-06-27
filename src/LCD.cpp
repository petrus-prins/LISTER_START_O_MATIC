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
bool Blink_State = 0;


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
void Update_LCD__NORMAL()
{
    
    if(LCD_Present == 1)                                       // LCD OK
    { 
        float A = 0;     // Amps
        float V = 0;     // Volts
        //----------------+------------------+-
        // LINE 1  ==>   "230V  25A   Batt:24V"
        //----------------+------------------+-    
        //sprintf(LCD_Line, "%3ldV %2ldA   Batt: %2ldV",gLCD_AC_Volts, gLCD_AC_MilliAmps/1000, gLCD_DC_Volts);
        A = (float)gLCD_AC_MilliAmps/1000.0;
        V = (float)gLCD_DC_MilliVolts/1000.0;
        if (A >= 9.1)
        {
              sprintf(LCD_Line, "%3ldV 10.+A Bat:%2d.%1dV",gLCD_AC_Volts, (int)V, (int)(V*10)%10); 
        } 
        else  sprintf(LCD_Line, "%3ldV %2d.%1dA Bat:%2d.%1dV",gLCD_AC_Volts, (int)A, (int)(A*10)%10, (int)V, (int)(V*10)%10);
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
        
        Blink_State = !Blink_State;
        if (Blink_State)
        {
            sprintf(LCD_Line, "                    "); 
        }
        else
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
                sprintf(LCD_Line, "S4: POWERING HOUSE  ");            
            break;
            case 5:
                sprintf(LCD_Line, "S5: ENGINE SHUTDOWN ");
            break;
            case 6:
                sprintf(LCD_Line, "S6: SYSTEM SHUTDOWN ");
            break;   
            default:
                sprintf(LCD_Line, "S0: INIT            ");
        }
        
        LCD.setCursor(0, 3); 
        LCD.print(LCD_Line);
    
    } //LCD OK
}   // LCD NORMAL



void Update_LCD__DEBUG()
{
    float A = 0;     // Amps
    float V = 0;     // Volts
    if(LCD_Present == 1)                                       // LCD OK
    { 
        //----------------+------------------+-
        // LINE 1  ==>   "230V  25A   Batt:24V"
        //----------------+------------------+-    
        
        A = (float)gLCD_AC_MilliAmps/1000.0;
        V = (float)gLCD_DC_MilliVolts/1000.0;
        
        if (A >= 9.1)
        {
              sprintf(LCD_Line, "%3ldV 10.+A Bat:%2d.%1dV",gLCD_AC_Volts, (int)V, (int)(V*10)%10); 
        } 
        else  sprintf(LCD_Line, "%3ldV %2d.%1dA Bat:%2d.%1dV",gLCD_AC_Volts, (int)A, (int)(A*10)%10, (int)V, (int)(V*10)%10);
        LCD.setCursor(0, 0); 
        LCD.print(LCD_Line);
    
        //----------------+------------------+-
        // LINE 2  ==>   "
        //----------------+------------------+-    
        sprintf(LCD_Line, "Starter:%s Fuel:%s",(gSTARTER_RELAY_ON) ? "ON " : "OFF",(gFUEL_RELAY_ON) ? "ON " : "OFF");
        LCD.setCursor(0, 1); 
        LCD.print(LCD_Line);
    
        //----------------+------------------+-
        // LINE 3  ==>   "
        //----------------+------------------+-  
        A = (float)gLCD_AC_MilliAmps/1000.0;
        (A <= 9.1)? sprintf(LCD_Line, "Amps: [%2d.%02d] A     ", (int)A, (int)(A*100)%100) :  sprintf(LCD_Line, "Amps: [ 10+ ] A     "); 
        LCD.setCursor(0, 2); 
        LCD.print(LCD_Line);
    
        //----------------+------------------+-
        // LINE 4  ==>   "
        //----------------+------------------+-  
        sprintf(LCD_Line, "CutOff: %4ld [%s]",gCUTOFF_ADC_Value_mV,(gZERO_CURRENT_TRIGGER_ACTIVE)? "True ":"False");
        LCD.setCursor(0, 3); 
        LCD.print(LCD_Line);
    }   // Present
}


//==========================================================
//         SELECT NORMAL OR DEBUG LCD DATA  
//==========================================================
bool Update_LCD(void *)
{
    bool LCD_DEBUG = false;
    (LCD_DEBUG)? Update_LCD__DEBUG() : Update_LCD__NORMAL();
    return true;                                               // Retun True if this function must be called next time by timer lbrary.
}

