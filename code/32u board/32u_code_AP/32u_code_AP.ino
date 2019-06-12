/*
 * source:
 * https://circuits4you.com/2018/11/21/esp32-access-point-station-ap-html-web-server-arduino/
 * and
 * http://www.iotsharing.com/2017/05/how-to-turn-esp32-into-access-point.html
 * 
 * 
 * ESP32 Web Server Demo using Accesspoint
 * https://circuits4you.com
 * 21-11-2018
 */
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>

#include "index.h"  //Web page header file

WebServer server(80);

//Enter your WiFi SSID and PASSWORD
const char* ssid = "AlinBoard_1_AP";
//const char* password = "18273645";     //Password length minimum 8 char
const char* password = "";             //Password empty for no security

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() {
 String s = MAIN_page; //Read HTML contents
 server.send(200, "text/html", s); //Send web page
}

void handleTurn()
{
 server.send(200, "text/html", "turning a quarter of a round"); 
 
}

//===============================================================
// Setup
//===============================================================

void setup(void){
  Serial.begin(115200);
  Serial.println();
  Serial.println("Booting Sketch...");

//ESP32 As access point IP: 192.168.4.1
  WiFi.mode(WIFI_AP); //Access Point mode
  WiFi.softAP(ssid, password);   
  
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

//Comment below code if you are using Access point only
//ESP32 connects to your wifi -----------------------------------
/*
  WiFi.mode(WIFI_STA); //Connectto your wifi
  WiFi.begin(ssid, password);

  Serial.println("Connecting to ");
  Serial.print(ssid);

  //Wait for WiFi to connect
  while(WiFi.waitForConnectResult() != WL_CONNECTED){      
      Serial.print(".");
      delay(1000);
    }
    
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP
  */
//----------------------------------------------------------------
 
  server.on("/", handleRoot);      //This is display page

  server.on("/turnQuarterRound", handleTurn);
 
  server.begin();                  //Start server
  Serial.println("HTTP server started");
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void loop(void){
  server.handleClient();
  delay(1);
}
