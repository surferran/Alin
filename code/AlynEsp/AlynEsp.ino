/** 
 * file : AlynEsp.ino 
*/

#include "AlynEsp.h"
///////////////////////////////

long            loopCounter_big = 0;
EEPROMvars      eepromVariables = {0};
systemVariables systemVars      = {0};     // or systemData

///////////////////////////////////////////////////////////////////////////////////////////
/*
connecting Rotary encoder (using interupts)
CLK (A pin) - to any microcontroler intput pin with interrupt -> in this example pin 32->21
DT (B pin) - to any microcontroler intput pin with interrupt -> in this example pin 21->32
SW (button pin) - to any microcontroler intput pin -> in this example pin 25 - not in use
VCC - to microcontroler VCC (then set ROTARY_ENCODER_VCC_PIN -1) or in this example pin 25
GND - to microcontroler GND
*/
#define ROTARY_ENCODER_A_PIN      4 // clk Clock    // GPIO_NUM_21  // <- ESP32
#define ROTARY_ENCODER_B_PIN      0 // DT Data      // GPIO_NUM_32  // <- ESP32

//#define ROTARY_ENCODER_VCC_PIN    -1 /*put -1 of Rotary encoder Vcc is connected directly to 3,3V; else you can use declared output pin for powering rotary encoder */
//#define MAX_ROTATION_RESOLUTION   100   // number of steps
//AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, ROTARY_ENCODER_VCC_PIN); //esp32


RotaryEncoder rotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN);


int test_limits = 2;

int RightVal=0, LeftVal=0,  legMoveCount=0, flagLeft, flagRight; //YARC 
int acceptRang = 40; // minimal encoder range to increese conter // for ESP32 was 60; 

int oldPos=0;
int newPos=0;

int16_t encoderChanged(){
  newPos = rotaryEncoder.getPosition();
  int16_t res = newPos>oldPos ? 1 : ( newPos<oldPos ? -1 : 0 );
  oldPos = newPos;
  return res;
}


void rotary_loop() {
  rotaryEncoder.tick();
  
  //lets see if anything changed
  int16_t encoderDelta = encoderChanged();  //YARC rotaryEncoder.encoderChanged();

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
   //YARC int16_t encoderValue = rotaryEncoder.readEncoder();

    ////========================
    if (encoderDelta == 1) 
    {
        RightVal = RightVal + encoderDelta; // encoderValue;
        LeftVal = 0; flagLeft = 0;
        if ((RightVal > acceptRang) && (flagRight==0))
        {
          flagRight = 1; legMoveCount++;
          LeftVal =0; 
        }
    }
   else if (encoderDelta == -1)
    {
        LeftVal  = LeftVal + abs(encoderDelta); //encoderValue;
        RightVal = 0; flagRight = 0;
       if ((LeftVal > acceptRang) && (flagLeft==0))
        {
            flagLeft = 1; legMoveCount++;
            RightVal =0; 
        }
   }
    ////========================
   
    //process new value. Here is simple output.
    Serial.print(", delta: ");        Serial.print(encoderDelta);
    Serial.print(", RightVal: ");     Serial.print(RightVal); 
    Serial.print(", LeftVal: ");      Serial.print(LeftVal); 
    Serial.print(", Cnt: ");          Serial.println(legMoveCount);  
  }  
}

//===============================================================
// This routine is executed when you open its IP in browser
//===============================================================
void handleRoot() 
{
  String s = MAIN_page;              //Read HTML contents // depend on readADC()
  server.send(200, "text/html", s);  //Send web page
}
 
void handleADC() {
 int a = analogRead(ANALOG_PIN);
 String adcValue = String(a);
 
 server.send(200, "text/plane", adcValue); //Send ADC value only to client ajax request
}

void getEncoder_ESP32() // function operated by client HTML (AJAX)
{
 //int16_t encoderDelta = read_and_print_analog(); // rotaryEncoder.encoderChanged();
 double encoderDelta  = read_and_print_analog(); // rotaryEncoder.encoderChanged();
 String encValue      = String(encoderDelta);

 //String encValue = String(millis());
 server.send(200, "text/plane", encValue);
 
 systemVars.potentiometer_Raw = encoderDelta;
}


void getEncoder() {
 int16_t encoderDelta = encoderChanged();  //yarc  rotaryEncoder.encoderChanged();
 String encValue =  String(legMoveCount); ///Send Counter ############################ //String(encoderDelta);
 
 //server.send(200, "text/plane", encValue); //Send encoder value only to client ajax request
 //debug: String encValue = String(millis());
 server.send(200, "text/plane", encValue);
}

void handleRawDataPage()  // todo: operate by interupt? because 0.5 sec delay on exit.
{
  digitalWrite(led_by_Board, DIGITAL_ON);

  char intToPrint[5];
  String urlResponse = "hello from 'Alyn Esp board'!  \n saw your led blinks?? \n current readings are:\n ";  
  itoa(systemVars.potentiometer_Raw , intToPrint, 10); //integer to string conversion 
  urlResponse  += "\nanalog_reading \t\t= ";
  urlResponse  += intToPrint; 
  itoa(systemVars.systemTime       , intToPrint, 10);
  urlResponse  += "\ntime_of_reading \t= ";
  urlResponse  += intToPrint;   
  //itoa(systemVars.safetyStop     , intToPrint, 1);
  //intToPrint = systemVars.safetyStop ? "On":"Off"  ;
  urlResponse  += "\ndigital status is\t= ";
  //urlResponse  += intToPrint;
  urlResponse  += systemVars.safetyStop ? "On":"Off";

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
  delay(2 * HALF_SEC_IN_mS); //   

  /* set and read from FLASH memory */
  EEPROM.begin(EEPROM_SIZE);   // initialize EEPROM with predefined size
  /******* do only in production: reset alll Flash data */
  //burn_production_data();
  /**************************************************** */
  int flashData[EEPROM_SIZE];
  for (int ndx=0; ndx<EEPROM_SIZE; ndx++)
  {
    flashData[ndx] = EEPROM.read(ndx);
    Serial.print("flash data ");
    Serial.print(ndx);
    Serial.print(" is ");
    Serial.println(flashData[ndx]);
  }
  // todo: decide what to do with these bytes
  eepromVariables.boardSN = flashData[0];
  eepromVariables.timesOfResets = flashData[1] + 1;
  writeByteToFlash(1, eepromVariables.timesOfResets);

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
  
  /* Put IP Address details */
  IPAddress local_ip(192,168,  1, 1);
  IPAddress gateway (192,168,  1, 1);
  IPAddress subnet  (255,255,255, 0);
  
  WiFi.softAPConfig(local_ip, gateway, subnet);
  
  delay(200);   
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);


#endif //
  ///////////////////////////////

  /* server events */
  server.on("/",            handleRoot);           //This is display page
  server.on("/readADC",     handleADC);     //To get update of ADC Value only
  server.on("/getEncoder",  getEncoder); //To get update of ADC Value only
  server.on("/rawValues",   handleRawDataPage); //To get update on all Values
  server.onNotFound        (handleNotFound);

  server.begin();
  Serial.println("HTTP server started"); 

  String welcomeString = "'ESP Alyn board' init is done";
  Serial.println("");
  Serial.println(welcomeString);

  /* encoder related */
  //we must initialize rorary encoder 
  //YARC rotaryEncoder.begin();
  //YARC rotaryEncoder.setup([]{rotaryEncoder.readEncoder_ISR();});
  //optionally we can set boundaries and if values should cycle or not
  //YARC rotaryEncoder.setBoundaries(0, MAX_ROTATION_RESOLUTION, true); //minValue, maxValue, cycle values (when max go to min and vice versa)
  Serial.print("Encoder is assumed as Ready");


/* init output stucture */
 systemVars.safetyStop     = false;  
 systemVars.potentiometer_Raw = 0;
 systemVars.systemTime       = 0;
 systemVars.number_of_moves  = 0;
}


/** loop function is used for a cyclic operation, and activated over and over */
void loop(void) 
{
  server.handleClient();
  MDNS.update();

  rotary_loop();

  // bad for your debugging eyes.. : to run only for alive test : blink_example_cycle();

  //if (loopCounter_read_analog > 100)  //32000
  //{
  //  systemVars.potentiometer_Raw = read_and_print_analog();
  //  loopCounter_read_analog = 0;
  //}

  ++loopCounter_big;  //millis(); // ?
  //++loopCounter_read_analog;
  systemVars.systemTime = millis() * MILLIS_TO_SEC; 
  delay(MINIMUM_1_mS);
}

//////////////////////////////////////////////////////////////
/** utils functions */

/**
 * @brief 
 *        write data to byte in a given address, 0 to EEPROM_SIZE.
 *        every data can be in 0 to 255 value range.
 * @param address 
 * @param data 
 */
void writeByteToFlash(int address, int data)
{
    EEPROM.write(address, data);
    EEPROM.commit();
    Serial.println("requested data saved in flash memory");
}

void burn_production_data()
{
  eepromVariables.boardSN = 001;
  writeByteToFlash(0, eepromVariables.boardSN); 

  for (int ndx=1; ndx<EEPROM_SIZE; ndx++)
  {
    writeByteToFlash(ndx, 0);
    Serial.print("burned 0 flash data in index ");
    Serial.print(ndx);
  }  
}

void print_led_state()
{
  int ledVal=0;
  ledVal = digitalRead(led_by_Board);

  systemVars.safetyStop = (ledVal ==   0);

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
  //analogVal = loopCounter_big;
  analogVal = simulatied_analog_as_sin(systemVars.systemTime);
  //analogVal = systemVars.systemTime;

  Serial.print("analogVal: ");  
  Serial.println(analogVal);      // can do val/1023 [%] or other calculated ratio..
 
  return analogVal;
}

/**
 * @brief 
 *      produce analog sinus sign
 *      by a = A*sin(w*t + phi)  
 *      ( 2. * 3.14 * f == w )
 *      ( phi = 0 )
 * @param time 
 * @return double 
 */
double simulatied_analog_as_sin(double time)
{
  static double freq      = 100.; //[hz] // but its not realy HZ RealTime
  static double amplitude = 100.; //[-]

  double signal_output = amplitude * sin (2. * 3.14 * freq * time);

  return signal_output;
}
