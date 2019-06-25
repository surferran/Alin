/*
 Project: L298N dual h-bridge motor driver module, 1 DC motor, without speed control
 Function: DC motor rotates one direction then another and stops
 */
//************************************************************
int IN1 = 26;//input 1 of DC motor 2 attached to GPIO 26
int IN2 = 27;//input 2 of DC motor 2 attached to GPIO 27
//************************************************************
void setup()
{
  pinMode (IN1, OUTPUT);//sets IN1 as OUTPUT
  pinMode (IN2, OUTPUT);//sets IN2 as OUTPUT
}
void loop()
{
  //DC motor 1 rotates one direction as signals are different:
  digitalWrite (IN1, HIGH);
  digitalWrite (IN2, LOW); 
  delay(4000);//sets delay for 4 seconds
  //DC motor 1 stopped as signals are the same
  digitalWrite (IN1, LOW); 
  delay(500);//sets delay for half a second
  //DC motor 1 rotates another direction as signals are different:
  digitalWrite (IN2, HIGH);
  delay(4000);//sets delay for 4 seconds
   //DC motor 1 stopped as signals are the same
  digitalWrite (IN2, LOW); 
  delay(4000);//sets delay for 4 seconds
  }
