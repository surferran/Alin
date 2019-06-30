/** 
 * file : esp8266code.ino * 
 * todo: change to EspAlin.ino
*/

/**
 * serial output is set at 115200:
 *  http connection will be to address (todo: build those as tests, after implementation)
 *  http://10.0.0.70/
 *  http://10.0.0.70/?hhvjv 
 *  http://10.0.0.70/arg?arg1=5;analog1=3
 *  http://10.0.0.70/inline
 * connections are to local touter, or acting as stand alone Access Point.

todo:
  server operation:
  https://www.instructables.com/id/Programming-a-HTTP-Server-on-ESP-8266-12E/
  ide: vscode with esp8266
  read analog (copal jc10 10k ohm). red wire of the HW potentiometer will be the Analog input.
    yellow, and green are to the 0,5V points.

 *  why must it connect to local router to act as wifi server??
 */
 /* more ref from
  *  https://robotzero.one/sending-data-esp8266-to-esp8266/
  */
///////////////////////////////
#include "general_defs.h"
#define MY_CARD_IS_ESP8266
//#define MY_CARD_IS_ESP32
//#define MY_WIFI_TYPE_IS_AP
//#define MY_WIFI_TYPE_IS_STA
#define MY_WIFI_TYPE_IS_BOTH
///////////////////////////////

#ifdef MY_CARD_IS_ESP8266

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
ESP8266WebServer server(80);
#define ANALOG_PIN        A0
const int led = LED_BUILTIN; //13;
#define DI_INPUT_SWITCH  D1

#else

#ifdef MY_CARD_IS_ESP32

/* for web server */
#include <WiFi.h>
#include <WebServer.h>
#include <mDNS.h>

WebServer server(80);

#endif // MY_CARD_IS_ESP32

#endif  // MY_CARD_IS_ESP8266
///////////////////////////////

#include <WiFiClient.h>

#include "index.h"                //Web page header file
#include "user_ssid_details.h"    // inside: AP_SSID and AP_PASSWORD
#include "private_server_details.h"
#ifndef STASSID
 #define STASSID "your-ssid" 
 #define STAPSK  "your-password"
#endif

#define MDNS_desired_name "AlinEspBoard"
///////////////////////////////

long loopCounter_big        =0;
//long loopCounter_read_analog=0; // all sensors are measured by interrupt

const int led_by_Board        = led;
#define ANALOG_PIN_by_Board   ANALOG_PIN
#define DI_INPUT_SWITCH_by_Board DI_INPUT_SWITCH

#define SERIAL_BAUD_RATE      115200
#define DIGITAL_ON            LOW
#define DIGITAL_OFF           HIGH
#define HALF_SEC_IN_mS           500
#define MINIMUM_1_mS               1

struct sensorsValues
{
  int     systemTime;              // controller counter
  int     potentiometerRaw;        // 0-1023 value range
  bool    digitalInput;            // 0 or 1
  int     number_of_moves;         // calculated value for number of excercise moves
} systemSensors;  // or systemData

///////////////////////////////

 /** functions headers */
void  handleRoot();
void  handleNotFound();
void  setup();
void  loop();
void  print_led_state();
void  blink_example_cycle();
int   read_and_print_analog();
////////////////////////////////////

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page;              //Read HTML contents // depend on readADC()
 server.send(200, "text/html", s);  //Send web page
}
 
void handleADC() {
 int a = analogRead(ANALOG_PIN);
 String adcValue = String(a);
 
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

void getEncoder() // function operated by client HTML (AJAX)
{
 //int16_t encoderDelta = read_and_print_analog(); // rotaryEncoder.encoderChanged();
 double encoderDelta  = read_and_print_analog(); // rotaryEncoder.encoderChanged();
 String encValue      = String(encoderDelta);

 systemSensors.potentiometerRaw = encoderDelta;
 
 //server.send(200, "text/plane", encValue); //Send encoder value only to client ajax request
 //String encValue = String(millis());
 server.send(200, "text/plane", encValue);
}

void handleRawDataPage()  // todo: operate by interupt? because 0.5 sec delay on exit.
{
  digitalWrite(led_by_Board, DIGITAL_ON);

  char intToPrint[5];
  String urlResponse = "hello from 'Alin Esp board'!  \n saw your led blinks?? \n current readings are:\n ";  
  itoa(systemSensors.potentiometerRaw , intToPrint, 10); //integer to string conversion 
  urlResponse  += "\nanalog_reading \t\t= ";
  urlResponse  += intToPrint; 
  itoa(systemSensors.systemTime       , intToPrint, 10);
  urlResponse  += "\ntime_of_reading \t= ";
  urlResponse  += intToPrint;   
  //itoa(systemSensors.digitalInput     , intToPrint, 1);
  //intToPrint = systemSensors.digitalInput ? "On":"Off";
  urlResponse  += "\ndigital status is\t= ";
  //urlResponse  += intToPrint;
  urlResponse  += systemSensors.digitalInput ? "On":"Off";

  server.send(200, "text/plain", urlResponse);

  delay(HALF_SEC_IN_mS);
  digitalWrite(led_by_Board, DIGITAL_OFF);
}

void handleNotFound()
{
  digitalWrite(led_by_Board, DIGITAL_ON);

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

  digitalWrite(led_by_Board, DIGITAL_OFF);
}

////////////////////////////////////
void setup(void) 
{
  /* pins setup */
  pinMode(led_by_Board, OUTPUT);
  digitalWrite(led_by_Board, DIGITAL_OFF);

  pinMode(ANALOG_PIN , INPUT);

  /* communications setup */
  Serial.begin(SERIAL_BAUD_RATE);

  ////////////////
  #if defined(MY_WIFI_TYPE_IS_STA) || defined(MY_WIFI_TYPE_IS_BOTH)
  WiFi.mode(WIFI_STA);
  WiFi.begin(STASSID, STAPSK);

  Serial.println("");
  Serial.println("connection trial:");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(0100);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to local router of : ");
  Serial.println(STASSID);
  Serial.print("and got an inner IP address of : ");
  Serial.println(WiFi.localIP());

  /* set also Alis name for this board on net */
  if (MDNS.begin(MDNS_desired_name)) {
    Serial.print("MDNS responder , of desired name : '");
    Serial.print(MDNS_desired_name);
    Serial.println("', is started");
  }
  #endif // MY_WIFI_TYPE_IS_STA || MY_WIFI_TYPE_IS_BOTH
  ///////////////////////////////
  #if defined(MY_WIFI_TYPE_IS_AP) || defined(MY_WIFI_TYPE_IS_BOTH)
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(AP_ssid, AP_password);
  delay(200);   
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  #endif //
  ///////////////////////////////

  /* server events */
  server.on("/", handleRoot);           //This is display page
  server.on("/readADC", handleADC);     //To get update of ADC Value only
  server.on("/getEncoder", getEncoder); //To get update of ADC Value only
  server.on("/rawValues", handleRawDataPage); //To get update on all Values

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started"); 

  String welcomeString = "'ESP Alin board' init is done";
  Serial.println("");
  Serial.println(welcomeString);

/* init output stucture */
 systemSensors.digitalInput     = false;
 systemSensors.potentiometerRaw = 0;
 systemSensors.systemTime       = 0;
 systemSensors.number_of_moves  = 0;
}


/** loop function is used for a cyclic operation, and activated over and over */
void loop(void) 
{
  server.handleClient();
  MDNS.update();

  // bad for your debugging eyes.. : to run only for alive test : blink_example_cycle();

  //if (loopCounter_read_analog > 100)  //32000
  //{
  //  systemSensors.potentiometerRaw = read_and_print_analog();
  //  loopCounter_read_analog = 0;
  //}

  ++loopCounter_big;  //millis(); // ?
  //++loopCounter_read_analog;
  systemSensors.systemTime = millis();///loopCounter_big / 1000.; // ruffly mS to sec. assuming really 1mS delay between loops
  delay(MINIMUM_1_mS);
}

//////////////////////////////////////////////////////////////
/** utils functions */

void print_led_state()
{
  int ledVal=0;
  ledVal = digitalRead(led_by_Board);

  systemSensors.digitalInput = (ledVal == 0);

  Serial.print("led: ");  
  Serial.println(ledVal);
}

void blink_example_cycle() 
{
  /* light on */
  digitalWrite(led_by_Board, LOW);   // Turn the LED on (Note that LOW is the voltage level
  delay(0050);                      // Wait for a 0.5 second
  print_led_state();
  /* ligth off */
  digitalWrite(led_by_Board, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(0450);                      // Wait for 4.5 seconds (to demonstrate the active low LED)
  print_led_state();
}

int read_and_print_analog()
{
  double analogVal=0;
  //analogVal = analogRead(ANALOG_PIN);
  analogVal = loopCounter_big;//simulatied_analog_as_sin(systemSensors.systemTime);

  Serial.print("analogVal: ");  
  Serial.println(analogVal);      // can do val/1023 [%] or other calculated ratio..
 
  return analogVal;
}

/**
 * @brief 
 *      produce analog sinus sign
 *      by a = A*sin(2*pi*w*t)
 * @param time 
 * @return double 
 */
double simulatied_analog_as_sin(double time)
{
  static double freq      = 1.; //[hz]
  static double amplitude = 1.; //[-]

  double signal_output = amplitude * sin (2.*3.14*freq*time);

  return signal_output;
}
