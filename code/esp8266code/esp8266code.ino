
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

/**
 * source from Examples
 * 
 * after 'burn' try in http:
 *  http://10.0.0.70/
 *  http://10.0.0.70/?hhvjv 
 *  http://10.0.0.70/arg?arg1=5;analog1=3
 *  http://10.0.0.70/inline
 *  
 *  why must it connect to local router to act as wifi server??
 */
 /* more ref from
  *  https://robotzero.one/sending-data-esp8266-to-esp8266/
  */
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "private_server_details.h"

#ifndef STASSID
 #define STASSID "your-ssid" 
 #define STAPSK  "your-password"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

ESP8266WebServer server(80);

const int led = LED_BUILTIN; //13;
long loopCounter_big=0;
long loopCounter_read_analog=0;

#define ANALOG_PIN        A0
#define SERIAL_BAUD_RATE  115200
#define DIGITAL_ON        LOW
#define DIGITAL_OFF       HIGH
#define HALF_SEC          500

struct sensorsValues
{
        int     systemTime;              // controller counter
        int     potentiometerRaw;        // 0-1023 value range
        bool    digitalInput;            // 0 or 1     
} systemSensors;


 /** functions headers */
void  handleRoot();
void  handleNotFound();
void  setup();
void  loop();
void  print_led_state();
void  blink_example_cycle();
int   read_and_print_analog();
////////////////////////////////////

void handleRoot() 
{
  digitalWrite(led, DIGITAL_ON);

  char intToPrint[5];
  String urlResponse = "hello from esp8266!.  \n saw your led blinks?? \nreadings are:\n ";  
  itoa(systemSensors.potentiometerRaw , intToPrint, 10); //integer to string conversion 
  urlResponse  += "\nanalog_reading=";
  urlResponse  += intToPrint; 
  itoa(systemSensors.systemTime       , intToPrint, 10); //integer to string conversion 
  urlResponse  += "\ntime_of_reading=";
  urlResponse  += intToPrint;   
  itoa(systemSensors.digitalInput     , intToPrint, 1); //integer to string conversion 
  urlResponse  += "\ndigital status is=";
  urlResponse  += intToPrint;

  server.send(200, "text/plain", urlResponse);

  delay(HALF_SEC);
  digitalWrite(led, DIGITAL_OFF);
}

void handleNotFound()
{
  digitalWrite(led, DIGITAL_ON);

  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }

  server.send(404, "text/plain", message);

  digitalWrite(led, DIGITAL_OFF);
}

void setup(void) 
{
  pinMode(led, OUTPUT);
  digitalWrite(led, DIGITAL_OFF);
  Serial.begin(SERIAL_BAUD_RATE);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  /* server events */
  server.on("/", handleRoot);

  server.onNotFound(handleNotFound);
/*
  server.on("/inline", 
    []() {
      server.send(200, "text/plain", "this works as well");
    }
    );
*/

  server.begin();
  Serial.println("HTTP server started"); 

// put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an (digital?) output
  pinMode(ANALOG_PIN , INPUT);

  //Serial.begin(115200);
  String welcomeString = "'d1 mini lite' init is done";
  Serial.println("");
  Serial.println(welcomeString);

 systemSensors.digitalInput     = false;
 systemSensors.potentiometerRaw = 0;
 systemSensors.systemTime       = 0;
}


/** loop function is used for a cyclic operation, and activated over and over */
void loop(void) 
{
  server.handleClient();
  MDNS.update();

  // bad for your debugging eyes.. blink_example_cycle();

  if (loopCounter_read_analog > 1)  //32000
  {
    systemSensors.potentiometerRaw = read_and_print_analog();
    loopCounter_read_analog = 0;
  }

  ++loopCounter_big;
  systemSensors.systemTime = loopCounter_big;
}

//////////////////////////////////////////////////////////////
/** utils functions */

void print_led_state()
{
  int ledVal=0;
  ledVal = digitalRead(LED_BUILTIN);

  systemSensors.digitalInput = (ledVal == 0);

  Serial.print("led: ");  
  Serial.println(ledVal);
}

void blink_example_cycle() 
{
  /* light on */
  digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
  delay(0050);                      // Wait for a 0.5 second
  print_led_state();
  /* ligth off */
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(0450);                      // Wait for 4.5 seconds (to demonstrate the active low LED)
  print_led_state();
}

int read_and_print_analog()
{
  int analogVal=0;
  analogVal = analogRead(ANALOG_PIN);

  Serial.print("analogVal: ");  
  Serial.println(analogVal);  // can do val/1023 [%] or other calculated ratio..
 
  return analogVal;
}
