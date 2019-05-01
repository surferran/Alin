/**
 * starting project with esp8266mod hardware:
 * go by :
 * https://dzone.com/articles/programming-the-esp8266-with-the-arduino-ide-in-3
 * which says:
 * -download Arduino IDE
 * -set some preferences
 *  (and on the it refers to http://arduino.esp8266.com/stable/package_esp8266com_index.json)
 * - then find example in  
 *   https://help.ubidots.com/connect-your-devices/connect-a-nodemcu-esp8266-to-ubidots-over-http#send-one-value-to-ubidots
 *   or others..
 *   (ubidots is a server for IoT data.
 *    relevenat lib can be found here:
 *    https://github.com/ubidots/ubidots-esp8266
 *    )
 *    
 *  hardware used: 
 *  similar to 'Lolin D1 mini Lite'  https://www.instructables.com/id/Programming-the-ESP8266MOD-ESP-12-Module-Using-the/ 
 *  (choose the above in the Arduino IDE for used hardware (in Tools->Board)
 *  (also for saftey choose upload freq to 115200, not higher)
 *  
 *  possible 'literature':
 *  - 'Wemos D1 mini: A first look at this ESP8266 ' https://www.youtube.com/watch?v=AuFXuEYd1Yk
 *  - pins sketch https://escapequotes.net/esp8266-wemos-d1-mini-pins-and-diagram/
 *  - https://www.electroschematics.com/12012/rotary-encoder-arduino/
 *    https://raspi.tv/2017/wireless-remote-sensing-with-wemos-d1-mini-arduino-ide-raspberry-pi-and-lighttpd-web-server
 *  
 *  examplary serial output at 115200:
 *  
led: 0
led: 1
led: 0
10:18:09.716 -> led: 1
10:18:10.019 -> led: 0
10:18:14.517 -> led: 1
10:18:14.855 -> led: 0

10:18:19.353 -> led: 1
10:18:19.661 -> led: 0
10:18:24.185 -> led: 1
10:18:24.493 -> led: 0

10:18:28.991 -> led: 1
10:18:29.297 -> led: 0
10:18:33.828 -> led: 1
10:18:34.133 -> led: 0

10:18:38.645 -> led: 1
10:18:38.953 -> led: 0
10:18:43.446 -> led: 1

.. time is not exact..

todo:
  server operation:
  https://www.instructables.com/id/Programming-a-HTTP-Server-on-ESP-8266-12E/
  ide: vscode with esp8266
  read analog (copal jc10 10k ohm). red wire of the HW potentiometer will be the Analog input.
    yellow, and green are to the 0,5V points.

 */

// todo: any includes ? currently using the builtin from the IDE

 /** functions headers */
void setup();
void loop();
void print_led_state();
void blink_example_cycle();
void read_and_print_analog();

#define ANALOG_PIN A0

int loopCounter=0;

/** program content */
void setup() 
{
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an (digital?) output
  pinMode(ANALOG_PIN, INPUT);

  Serial.begin(115200);
  Serial.println("");
  Serial.println("d1 mini init is done");
}

void loop() 
/** loop function is used for a cyclic operation, and activated over and over */
{
  // put your main code here, to run repeatedly:
  blink_example_cycle();

  if (loopCounter>1)  //32000
  {
    read_and_print_analog();
    loopCounter=0;
  }

  ++loopCounter;
}

/** utils functions */
void print_led_state()
{
  int ledVal=0;
  ledVal = digitalRead(LED_BUILTIN);
  Serial.print("led: ");  
  Serial.println(ledVal);
}

void blink_example_cycle() 
{
  // light on
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  delay(050);                      // Wait for a 0.5 second
  print_led_state();
  // ligth off
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(450);                      // Wait for 4.5 seconds (to demonstrate the active low LED)
  print_led_state();
}

void read_and_print_analog()
{
  int analogVal=0;
  analogVal = analogRead(ANALOG_PIN);
  Serial.print("analogVal: ");  
  Serial.println(analogVal);  // can do val/1023 [%] or other calculated ratio..
}
