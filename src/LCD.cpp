#include <LCD.h>
#include <GLOBALS.h>


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

/*      /// WORKING
    char lcdbuf[21];
    const char someLine1[] = "00000000000000000000";  // 20 chars
    const char someLine2[] = "88888888888888888888";  // 20 chars
    const char someLine3[] = "BBBBBBBBBBBBBBBBBBBB";  // 20 chars
    const char someLine4[] = "AAAAAAAAAAAAAAAAAAAA";  // 20 chars
    
    // fill the screen buffer
    strncpy(&LCD_Lines[0][0], someLine1 , 20);
    strncpy(&LCD_Lines[1][0], someLine2 , 20);
    strncpy(&LCD_Lines[2][0], someLine3 , 20);
    strncpy(&LCD_Lines[3][0], someLine4 , 20);
    // prepare screen
    LCD.home();
    LCD.clear();
    LCD.setCursor(0, 0);   
    // copy the buffer to the screen
    // 1st line continues at 3d line
    // 2nd line continues at 4th line
    strncpy(lcdbuf, &LCD_Lines[0][0], 20); lcdbuf[20] = '\0'; // create a termineted text line 
    LCD.print(lcdbuf);                                         // print the line to screen
    strncpy(lcdbuf, &LCD_Lines[2][0], 20); lcdbuf[20] = '\0';
    LCD.print(lcdbuf); 
    strncpy(lcdbuf, &LCD_Lines[1][0], 20); lcdbuf[20] = '\0';
    LCD.print(lcdbuf); 
    strncpy(lcdbuf, &LCD_Lines[3][0], 20); lcdbuf[20] = '\0';
    LCD.print(lcdbuf);
*/      


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
    sprintf(someLine1, "%s    %sV %sA ", "AC Line:", sAC_Volts, sAC_Amps);

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
















    //=========
    /*
    char sAC_Volts[4];
    char sAC_Amps[4];
    char sState[] = "[IDLE     ]";
    
    char LCD_Temp_Line[21];  // 20 chars
     




    // BUILD LCD LINE 1 
    strcpy(LCD_Temp_Line,"");
    dtostrf(LCD_AC_Volts, 3, 0, sAC_Volts);              // 4 is mininum width, 2 is precision; float value is copied
    dtostrf(LCD_AC_Amps, 2, 0, sAC_Amps);
    sprintf(LCD_Temp_Line, "%s %sV %sA", sState, sAC_Volts, sAC_Amps);
    strncpy(&LCD_Lines[0][0], LCD_Temp_Line, 20);       // SET LINE 0

    // BUILD LCD LINE 2
    strcpy(LCD_Temp_Line,"");
    dtostrf(RMS_Voltage, 3, 0, sAC_Volts);               //----------------   // 4 is mininum width, 2 is precision; float value is copied
    sprintf(LCD_Temp_Line, "%s %sV", "RMS_Volts = ", sAC_Volts);
    strncpy(&LCD_Lines[1][0], LCD_Temp_Line, 20);      // SET LINE 1
    //----------------
    
    // LINES 2 + 3
    strcpy(LCD_Temp_Line,"");
    sprintf(LCD_Temp_Line, "%s", "Line_2");
    strncpy(&LCD_Lines[2][0], LCD_Temp_Line , 20);     // clear line 2
    strcpy(LCD_Temp_Line,"");
    sprintf(LCD_Temp_Line, "%s", "Line_3");
    strncpy(&LCD_Lines[3][0], LCD_Temp_Line , 20);     // clear line 3
    // fill the screen buffer


    LCD.home();
    LCD.clear();
    LCD.setCursor(0, 0);   
    // copy the buffer to the screen
    // 1st line continues at 3d line
    // 2nd line continues at 4th line
    strncpy(LCD_Buffer, &LCD_Lines[0][0], 20); LCD_Buffer[20] = '\0'; // create a termineted text line 
    LCD.print(LCD_Buffer);                                         // print the line to screen
    strncpy(LCD_Buffer, &LCD_Lines[1][0], 20); LCD_Buffer[20] = '\0';
    LCD.print(LCD_Buffer); 
    strncpy(LCD_Buffer, &LCD_Lines[2][0], 20); LCD_Buffer[20] = '\0';
    LCD.print(LCD_Buffer); 
    strncpy(LCD_Buffer, &LCD_Lines[3][0], 20); LCD_Buffer[20] = '\0';
    LCD.print(LCD_Buffer);
*/
  return true;                                        // Retun True if this function must be called next time by timer lbrary.
}




