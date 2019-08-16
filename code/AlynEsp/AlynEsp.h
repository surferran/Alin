/** 
 * file : AlynEsp.h 
*/

/**
 * serial output is set at 115200:
  *
  *  setting the (soft)ip to constant: 192.168.1.1
 *  http connection will be to address (todo: build those as tests, after implementation)
 *  http://192.168.1.1/
 *  http://192.168.1.1/?hhvjv 
 *  http://192.168.1.1/arg?arg1=5;analog1=3
 *  http://192.168.1.1/inline
 * connections are to local touter, or acting as stand alone Access Point.

 *  stting time out to router connection, and alarm to StandAlone mode. TODO

  server operation:
  https://www.instructables.com/id/Programming-a-HTTP-Server-on-ESP-8266-12E/

 /* more ref from
  *  https://robotzero.one/sending-data-esp8266-to-esp8266/
  *  https://code.sololearn.com/WFQ8aQXVM54F/#html    // try html online
  *  https://techtutorialsx.com/2016/10/15/esp8266-http-server-serving-html-javascript-and-css/
  *  https://techtutorialsx.com/2018/09/13/esp32-arduino-web-server-receiving-data-from-javascript-websocket-client/
  *  https://randomnerdtutorials.com/esp32-flash-memory/
  *  
  *  encoder ref: 
  *  1. https://github.com/igorantolic/ai-esp32-rotary-encoder
  *  2. https://circuits4you.com/2018/11/20/web-server-on-esp32-how-to-update-and-display-sensor-values/
  *  3. https://randomnerdtutorials.com/esp32-access-point-ap-web-server/
  *  4. https://www.hackster.io/mitov/esp8266-wi-fi-remote-control-servo-with-rotary-encoder-1a35fa

	thinger IOT server ref:
	http://docs.thinger.io/arduino/#supported-hardware-esp8266-nodemcu

  */
///////////////////////////////
#include "general_defs.h"
#define MY_CARD_IS_ESP8266  //same as #define MY_CARD_IS_D1_MINI_PRO
//#define MY_CARD_IS_ESP32
#define MY_WIFI_TYPE_IS_AP
//#define MY_WIFI_TYPE_IS_STA
//#define MY_WIFI_TYPE_IS_BOTH
#define MY_run_mode_IS_Simulated_HW
//#define MY_run_mode_IS_EncoderOnly_HW
//#define MY_run_mode_IS_FULL_HW      // connected sensors, and motors
///////////////////////////////

#ifdef MY_CARD_IS_ESP8266

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include <arduino.h>
//#include "./RotaryEncoder_lib/RotaryEncoder.h"
#include <RotaryEncoder.h>

ESP8266WebServer server(80);
#define   ANALOG_PIN        A0
#define   DI_INPUT_SWITCH   D1
const int led         =     LED_BUILTIN; //13;

#else

#ifdef MY_CARD_IS_ESP32

/* for web server */
#include <WiFi.h>
#include <WebServer.h>
#include <mDNS.h>

/* for encoder */
#include "AiEsp32RotaryEncoder.h"

WebServer server(80);

//?#define   ANALOG_PIN        A0
//?#define   DI_INPUT_SWITCH   D1
//?const int led         =     LED_BUILTIN; //13;

#else

#ifdef MY_CARD_IS_D1_MINI_PRO

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
ESP8266WebServer server(80);
#define   ANALOG_PIN        A0
#define   DI_INPUT_SWITCH   D1
const int led         =     LED_BUILTIN; //13;
 
#endif //MY_CARD_IS_D1_MINI_PRO

#endif // MY_CARD_IS_ESP32

#endif  // MY_CARD_IS_ESP8266

/////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////


#include <WiFiClient.h>
#include <EEPROM.h> //read and write from flash memory

#include "index.h"                //Web page header file
#include "user_ssid_details.h"    // inside: AP_SSID and AP_PASSWORD
#include "private_server_details.h"
#ifndef STASSID
 #define STASSID "your-ssid" 
 #define STAPSK  "your-password"
#endif

#define MDNS_desired_name "AlynEspBoard"
///////////////////////////////

const int   led_by_Board            = led;
#define     ANALOG_PIN_by_Board       ANALOG_PIN
#define     DI_INPUT_SWITCH_by_Board  DI_INPUT_SWITCH

#define SERIAL_BAUD_RATE      115200
#define HALF_SEC_IN_mS           500
#define MINIMUM_1_mS               1
#define MILLIS_TO_SEC              0.001
#define EEPROM_SIZE               10    // define the total number of bytes to access to

#define DIGITAL_ON            LOW
#define DIGITAL_OFF           HIGH

struct systemVariables
{
  /* sensors */
  int     systemTime;              // controller time since reset [sec]
  int     potentiometer_Raw;       // 0-1023 value range . potentiometer or encoder before translation to physical meaning
  bool    safetyStop;              // T/F. Normally Open (default is true value). only when user press switch, the motor is enabled. 
  /* user parameters */
  int     number_of_moves;         // calculated value for number of excercise moves
};

struct EEPROMvars
{
  int boardSN;
  int timesOfResets;  
};
///////////////////////////////

 /** functions headers */
void  handleRoot();  // direct to user first login and register and setup, or admin setup and reset
void  handleRawDataPage();
void  handleADC();
void  handleNotFound();
void  setup();
void  loop();
void  print_led_state();
void  blink_example_cycle();
int   read_and_print_analog();

////////////////////////////////////