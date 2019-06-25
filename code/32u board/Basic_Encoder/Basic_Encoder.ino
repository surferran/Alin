/* 
 *  for Project Alin
 *  
 *  ref: https://github.com/igorantolic/ai-esp32-rotary-encoder
 */

#include "AiEsp32RotaryEncoder.h"
#include "Arduino.h"

/*
connecting Rotary encoder
CLK (A pin) - to any microcontroler intput pin with interrupt -> in this example pin 32->21
DT (B pin) - to any microcontroler intput pin with interrupt -> in this example pin 21->32
SW (button pin) - to any microcontroler intput pin -> in this example pin 25
VCC - to microcontroler VCC (then set ROTARY_ENCODER_VCC_PIN -1) or in this example pin 25
GND - to microcontroler GND
*/
#define ROTARY_ENCODER_A_PIN GPIO_NUM_21
#define ROTARY_ENCODER_B_PIN GPIO_NUM_32
#define ROTARY_ENCODER_BUTTON_PIN 25
#define ROTARY_ENCODER_VCC_PIN -1 /*put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN);

int test_limits = 2;

void rotary_loop() {
  //first lets handle rotary encoder button click
  //if (rotaryEncoder.currentButtonState() == BUT_RELEASED) {
    //we can process it here or call separate function like:
    //removed: rotary_onButtonClick();
  //}

  //lets see if anything changed
  int16_t encoderDelta = rotaryEncoder.encoderChanged();
  
  //Serial.println(" check delta1 ");
  
  //optionally we can ignore whenever there is no change
 // if (encoderDelta == 0) 
  //  return;
  
  //for some cases we only want to know if value is increased or decreased (typically for menu items)
    //Serial.print("check delta");
  if (encoderDelta>0) 
    Serial.print("+");
  if (encoderDelta<0) 
    Serial.print("-");

  //for other cases we want to know what is current value. Additionally often we only want if something changed
  //example: when using rotary encoder to set termostat temperature, or sound volume etc
  
  //if value is changed compared to our last read
  if (encoderDelta!=0) {
    //now we need current value
    int16_t encoderValue = rotaryEncoder.readEncoder();
    //process new value. Here is simple output.
    Serial.print("Value: ");
    Serial.println(encoderValue);
  }  
}

void setup() 
{
  Serial.begin(115200);

  //we must initialize rorary encoder 
  rotaryEncoder.begin();
  rotaryEncoder.setup([]{rotaryEncoder.readEncoder_ISR();});
  //optionally we can set boundaries and if values should cycle or not
  rotaryEncoder.setBoundaries(0, 10, true); //minValue, maxValue, cycle values (when max go to min and vice versa)
  Serial.print("ready");
}

void loop() {
  //in loop call your custom function which will process rotary encoder values
  rotary_loop();
  
  delay(50);                               
  //if (millis()>20000) 
  if (millis()>200)
    rotaryEncoder.enable ();
}
