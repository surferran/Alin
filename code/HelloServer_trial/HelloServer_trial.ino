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
 *  
 *  see this for intro :
 *  https://tttapa.github.io/ESP8266/Chap07%20-%20Wi-Fi%20Connections.html 
 *  a must try:
 *  https://tttapa.github.io/ESP8266/Chap10%20-%20Simple%20Web%20Server.html
 *  https://tttapa.github.io/ESP8266/Chap16%20-%20Data%20Logging.html
 *  
 *  use this to solve esp32-wemos init problem
 *  https://github.com/espressif/arduino-esp32/issues/863
 */
// if using 8266 model #include <ESP8266WiFi.h>
// #include <WiFiClient.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

//-"- #include <ESP8266WebServer.h>
//-"- #include <ESP8266mDNS.h>

#include "private_server_details.h"

#ifndef STASSID
#define STASSID "your-ssid"
#define STAPSK  "your-password"
#endif

const char* ssid = STASSID;
const char* password = STAPSK;

//ESP8266WebServer server(80);
WebServer server(80);

const int led = 13;//LED_BUILTIN; //13;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!.  \n saw your led blinks?? ");
  delay(500);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
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
  digitalWrite(led, 0);
}

void setup(void) 
{
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
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

//  if (MDNS.begin("esp8266")) {
  //  Serial.println("mDNS responder started: you can address esp8266.local instead IP");
 /// }

  server.on("/", handleRoot);

  server.on("/inline", 
    []() {
      server.send(200, "text/plain", "this works as well");
    }
    );

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  //MDNS.update();
}
