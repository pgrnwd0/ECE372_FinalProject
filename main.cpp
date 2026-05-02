#include <Arduino.h>
#include "i2c.h"
#include "timer.h"

enum State {OPEN, DEBOUNCE, CLOSED};
State state;


int main(){
  Serial.begin(115200);
  initI2C();
  initTimer0();
  Write16_to(0x40, 0x00, 0x399F);
  Write16_to(0x40, 0x05, 0x1061);
  
  // relay control pin
  DDRA  |= (1 << PA0);
  PORTA &= ~(1 << PA0); 

  while (1)
    {
        int16_t  raw_current = (int16_t)Read_from(0x40, 0x04);

        float   current_mA = raw_current * 0.000977f; // 0.1mA per bit
        Serial.print("Current: ");
        Serial.print(current_mA);
        Serial.println(" Amps");
        if (current_mA <= 0.01){
          PORTA |= (1 << PA0);
          delayMs(200); 
        }
        else{
          PORTA &= ~(1 << PA0);
          delayMs(200);
        }

      
        
        
    }

  return 0;
}