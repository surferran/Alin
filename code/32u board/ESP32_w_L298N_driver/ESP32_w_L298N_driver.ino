/*
  Project: L298N dual h-bridge motor driver module, 1 DC motor with speed control
  Function: DC motor rotates with minimum, medium,high speed and stops
*/

//#include <Stepper.h>

#define MUST_BE_ONE (1)
int rotDirection = (+1) ; // +1 for Right?
 
const int stepsPerRevolution = 200; // change this to fit the number of steps per revolution
 
// initialize the stepper library on pins 15,2,0,4
//Stepper myStepper(stepsPerRevolution, 15,2,0,4);
 
int stepCount = 0; // number of steps the motor has taken
 
//************************************************************
int IN1 = 26;//input 1 of DC motor 1 attached to gpio 26
int IN2 = 27;//input 2 of DC motor 1 attached to gpio 27
int ENA = 14;//ENA of DC motor attached to gpio 14
//PWM properties
const int freq = 30000;
const int pwmChannel = 0;
const int resolution = 8;
int dutyCycle = 200;
//************************************************************
void setup()
{
  Serial.begin(115200);
  
  pinMode (ENA, OUTPUT); //sets ENA as OUTPUT
  pinMode (IN1, OUTPUT);//sets IN1 as OUTPUT
  pinMode (IN2, OUTPUT);//sets IN2 as OUTPUT
  //configure LED PWM functionalitites
  ledcSetup(pwmChannel, freq, resolution);  
  //attach the channel to the GPIO to be controlled
  ledcAttachPin(ENA, pwmChannel);

}
void loop()  //dc motor (not stepper)
{
  //DC motor is ready for turn
  digitalWrite (IN1, HIGH);
  digitalWrite (IN2, LOW);
  while (dutyCycle <= 255){
    ledcWrite(pwmChannel, dutyCycle);      
    dutyCycle = dutyCycle + 5;
    delay(50);
  }
  dutyCycle = 20;
  
    //DC motor 1 rotates one direction as signals are different:
  digitalWrite (IN1, HIGH);
  digitalWrite (IN2, LOW); 
  delay(400);//sets delay for 4 seconds
  //DC motor 1 stopped as signals are the same
  digitalWrite (IN1, LOW); 
  delay(50);//sets delay for half a second
  //DC motor 1 rotates another direction as signals are different:
  digitalWrite (IN2, HIGH);
  delay(400);//sets delay for 4 seconds
   //DC motor 1 stopped as signals are the same
  digitalWrite (IN2, LOW); 
  delay(400);//sets delay for 4 seconds
}