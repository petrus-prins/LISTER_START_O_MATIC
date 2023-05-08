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
    char sAC_Volts[4];
    char sAC_Amps[4];
    char sState[] = "[IDLE     ]";

   strcpy(someLine1,"");
   dtostrf(LCD_AC_Volts, 3, 0, sAC_Volts);              // 4 is mininum width, 2 is precision; float value is copied
   dtostrf(LCD_AC_Amps, 2, 0, sAC_Amps);
   sprintf(someLine1, "%s %sV %sA", sState, sAC_Volts, sAC_Amps);
  
    //----------------

    
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
    strncpy(LCD_Buffer, &LCD_Lines[0][0], 20); LCD_Buffer[20] = '\0'; // create a termineted text line 
    LCD.print(LCD_Buffer);                                         // print the line to screen
    strncpy(LCD_Buffer, &LCD_Lines[2][0], 20); LCD_Buffer[20] = '\0';
    LCD.print(LCD_Buffer); 
    strncpy(LCD_Buffer, &LCD_Lines[1][0], 20); LCD_Buffer[20] = '\0';
    LCD.print(LCD_Buffer); 
    strncpy(LCD_Buffer, &LCD_Lines[3][0], 20); LCD_Buffer[20] = '\0';
    LCD.print(LCD_Buffer);

  return true;                                        // Retun True if this function must be called next time by timer lbrary.
}




