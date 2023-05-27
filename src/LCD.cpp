//#include <LCD.h>
#include <GLOBALS.h>


//========================================================
//         DIGITAL PINS USED FOR I2C LCD COMMS
//========================================================
//             I2C_SDA (PC4) AT328P_PIN_27 (ADC4)
//             I2C_SCL (PC5) AT328P_PIN_28 (ADC5)
//========================================================






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
        LCD.print("[SYSTEM STARTED]");
    } 
}



//====================================
//         UPDATE LCD SYSTEM STATS    
//====================================
bool Update_LCD(void *)
{

    // LCD_AC_Volts = analogRead(AC_VOLTAGE_AI_Pin);

    char lcdbuf[21];
    char someLine1[]     = "                    ";  // 20 chars
    char someLine2[]     = "                    ";  // 20 chars
    char someLine3[]     = "                    ";  // 20 chars
    char someLine4[]     = "                    ";  // 20 chars
    char sState[]        = "                    ";
    char Starter_State[] = "                    ";
    char Fuel_State[]    = "                    ";

    char sAC_Volts[4];
    char sAC_Amps[4];
    char sDC_Volts[5];

    
    // UPDATE CURRENT STATE
    sprintf(sState, "%s", "Idle");
    sprintf(Starter_State, "%s", "ON");
    sprintf(Fuel_State, "%s", "ON");
     

    // TEST:
    if (LCD_AC_Amps <= 10)
    {
        sprintf(Starter_State, "%s", "OFF");
        sprintf(Fuel_State, "%s", "OFF");
        sprintf(sState, "%s", "Shutting Down"); 
    }

    // BUILD LINE 1
    dtostrf(LCD_AC_Volts, 3, 0, sAC_Volts);              // 4 is mininum width, 2 is precision; float value is copied
    dtostrf(LCD_AC_Amps, 2, 0, sAC_Amps);
    dtostrf(LCD_DC_Volts, 3, 1, sDC_Volts);              // 4 is mininum width, 2 is precision; float value is copied
    

    sprintf(someLine1, "%s %sVDC %sVAC %sA ", "  ", sDC_Volts, sAC_Volts, sAC_Amps);

    // BUILD LINE 2
    sprintf(someLine2, "%s %s [ %s ]", "Starter:", "   ", Starter_State);

    //BUILD LINE 3
    sprintf(someLine3, "%s %s [ %s ]", "Fuel:", "      ", Fuel_State);

    // BUILD LINE 4
    sprintf(someLine4, "%s",sState);


    // fill the screen buffer
    strncpy(&LCD_Lines[0][0], someLine1 , 20);
    strncpy(&LCD_Lines[1][0], someLine2 , 20);
    strncpy(&LCD_Lines[2][0], someLine3 , 20);
    strncpy(&LCD_Lines[3][0], someLine4 , 20);
    // prepare screen
    LCD.home();
    LCD.clear();
    LCD.setCursor(0, 0);   

    strncpy(lcdbuf, &LCD_Lines[0][0], 20); lcdbuf[20] = '\0'; // create a termineted text line  
    LCD.print(lcdbuf);                                         // print the line to screen
   
     LCD.setCursor(0, 1); 
    strncpy(lcdbuf, &LCD_Lines[1][0], 20); lcdbuf[20] = '\0';
    LCD.print(lcdbuf); 
   
     LCD.setCursor(0, 2); 
    strncpy(lcdbuf, &LCD_Lines[2][0], 20); lcdbuf[20] = '\0';
    LCD.print(lcdbuf); 
   
    LCD.setCursor(0, 3); 
    strncpy(lcdbuf, &LCD_Lines[3][0], 20); lcdbuf[20] = '\0';
    LCD.print(lcdbuf);

  return true;    // Retun True if this function must be called next time by timer lbrary.
}




