#include <GLOBALS.h>


//========================================================
//         DIGITAL PINS USED FOR I2C LCD COMMS
//========================================================
//             I2C_SDA (PC4) AT328P_PIN_27 (ADC4)
//             I2C_SCL (PC5) AT328P_PIN_28 (ADC5)
//========================================================


char LCD_Line[]     = "                     ";            // 21 chars
char sAC_Volts[4];
char sAC_Amps[4];
char sDC_Volts[5];




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
    // LINE 1  ==>   "230V 25A BATT: 24V"
    sprintf(LCD_Line, "%3ldV  %2ldA   BATT:%2ldV",LCD_AC_Volts, LCD_AC_Amps, LCD_DC_Volts);
    LCD.setCursor(0, 0); 
    LCD.print(LCD_Line);
   
    // LINE 2
    sprintf(LCD_Line, "                    ");
    LCD.setCursor(0, 1); 
    LCD.print(LCD_Line);
 
    // LINE 3
    sprintf(LCD_Line, "                    ");
    LCD.setCursor(0, 2); 
    LCD.print(LCD_Line);
  
  // LINE 3
    sprintf(LCD_Line, "                    ");
    LCD.setCursor(0, 3); 
    LCD.print(LCD_Line);
  



    //dtostrf(LCD_AC_Volts, 3, 0, sAC_Volts);              // 4 is mininum width, 2 is precision; float value is copied
    //dtostrf(LCD_AC_Amps, 2, 0, sAC_Amps);
    //dtostrf(LCD_DC_Volts, 3, 1, sDC_Volts);              // 4 is mininum width, 2 is precision; float value is copied
    
    //dtostrf(240, 3, 0, sAC_Volts);              // 4 is mininum width, 2 is precision; float value is copied
    //dtostrf(12, 3, 0, sAC_Amps);
    //dtostrf(28.1, 3, 1, sDC_Volts);              // 4 is mininum width, 2 is precision; float value is copied
    
    
    //sprintf(LCD_Line, "%sVAC  %sA  %sVDC ", sAC_Volts, sAC_Amps, sDC_Volts);
    //LCD.setCursor(0, 0); 
    //LCD.print(LCD_Line);
   
    //sprintf(LCD_Line, "%4.0fVAC %3.0fA %3.0fDC",LCD_AC_Volts, LCD_AC_Amps, LCD_DC_Volts);
    //sprintf(LCD_Line, "%fVAC %fA %fDC",220.0, 15.0, 24.0);  LCD_Line[20] = '\0'; 


    //sprintf(LCD_Line, "Resets:       %6ld", (long)21);  LCD_Line[20] = '\0';    // THIS IS OKAY
    //sprintf(LCD_Line, "Resets:       %6f", (double)21.0);  LCD_Line[20] = '\0';    // THIS IS OKAY
    //LCD.setCursor(0, 0); 
    //LCD.print(LCD_Line);
  
   return true;    // Retun True if this function must be called next time by timer lbrary.
}






//====================================
//         UPDATE LCD SYSTEM STATS    
//====================================
bool Update_LCD__OLD(void *)
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
    char sFail_Count[7];

    
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
    //sprintf(someLine4, "%s",sState);
    dtostrf(Fail_Count, 6, 0, sFail_Count);                // long Shutdown_Count --> String 
    sprintf(someLine4, "Fails: %s",sFail_Count);


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




