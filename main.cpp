#include <Arduino.h>
#include "i2c.h"
#include "timer.h"
#include "lcd.h"

int main(){
  Serial.begin(115200);

  // intialize 
  initI2C();
  initTimer0();
  initLCD();

  // calibrate INA219
  Write16_to(0x40, 0x00, 0x399F); // calibrate for 32V range
  Write16_to(0x40, 0x05, 0x1061); // calibrate for 10 ohm shunt
  
  // relay control pin
  DDRH  |= (1 << PH5);
  PORTH &= ~(1 << PH5); 

  // set up LCD
  moveCursor(0,0);
  writeString("System Good  ");

  while (1)
    {
        int16_t  raw_current = (int16_t)Read_from(0x40, 0x04);

        float   current_mA = raw_current * 0.000977f; // value from datasheet to set 0.1mA per bit
      
        Serial.print("Current: ");
        Serial.print(current_mA);
        Serial.println(" Amps");

        // convert current float to string for LCD
        char currentArray[10];
        sprintf(currentArray, "%.2f", current_mA);
      
        if (current_mA <= 0.01){ // Current is too low
          PORTH |= (1 << PH5); // trigger switch

          // write to LCD
          moveCursor(0,0);
          writeString("System Fault  ");
          delayMs(200); // delay
        }
        else{ // current is good
          PORTH &= ~(1 << PH5);
          
          moveCursor(0,0);
          writeString("System Good  ");
          delayMs(200);
        }
       // display Current on LCD
      moveCursor(1,0);
      writeString("I: ");
      writeString(currentArray);
      writeString("mA");        
    }

  return 0;
}
