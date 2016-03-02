#include "Datastructures.h"
#include <Servo.h> 

// centered 
int TopServoPosition = 128;

//opposite eachother
int LeftServoPosition = 100;
int RightServoPosition = 155;

// sets up the solenoids
boolean ChannelValveOpen = true;

Motor TopBeam = {6, 7, 8};
Servo TopServo;
Servo LeftServo;
Servo RightServo;

int ServoIncrement = 5;
String Command = "";

void InitializeMotor(Motor ToInitialize)
{
  pinMode(ToInitialize.EnablePin, OUTPUT);
  pinMode(ToInitialize.InputOne, OUTPUT);
  pinMode(ToInitialize.InputTwo, OUTPUT);
}

void setup() 
{
  //setup the solenoids
  pinMode(13, OUTPUT);  
  
  //setup the serial connection
  Serial.begin(9600);
  
  //setup all of the motor controllers
  InitializeMotor(TopBeam); 
  
  //setup the servos
  TopServo.attach(9);
  LeftServo.attach(3);
  RightServo.attach(5);
  
  Initialize();
}

//enA is motor 1 and enB is motor 2
//speed is from -255 to 255
void SetMotorSpeed(int Speed, Motor ToMove)
{
  analogWrite(ToMove.EnablePin, Speed);
  if(Speed > 0)
  {
    digitalWrite(ToMove.InputOne, HIGH); 
    digitalWrite(ToMove.InputTwo, LOW); 
  }
  else if (Speed < 0)
  {  
    digitalWrite(ToMove.InputOne, LOW); 
    digitalWrite(ToMove.InputTwo, HIGH); 
  } 
  else if(Speed == 0)
  {
    digitalWrite(ToMove.InputOne, LOW); 
    digitalWrite(ToMove.InputTwo, LOW); 
  }
}

void ExecuteCommand(String Action)
{
  if(Action == "top-servo-down" && TopServoPosition >= ServoIncrement)
  {  
      TopServoPosition -= ServoIncrement;
  }
  else if(Action == "top-servo-up" && TopServoPosition <= 180 - ServoIncrement)
  {
      TopServoPosition += ServoIncrement;
  }
  else if(Action == "bottom-servos-down") // && LeftServoPosition >= ServoIncrement
  {
      LeftServoPosition -= ServoIncrement;
      RightServoPosition += ServoIncrement;
  }
  else if(Action == "bottom-servos-up") // && LeftServoPosition <= 255 - ServoIncrement
  {
     LeftServoPosition += ServoIncrement;
     RightServoPosition -= ServoIncrement;
  }
  else if (Action == "allign-up")
  {
    LeftServoPosition += 1;
    RightServoPosition += 1;
  }
  else if (Action == "allign-down")
  {
    LeftServoPosition -= 1;
    RightServoPosition -= 1;
  }
  else if(Action == "top-beam-forward")
  {
    SetMotorSpeed(255, TopBeam);
    // Serial.println("moving top beam forward");
  }
  else if(Action == "top-beam-backward")
  {
    SetMotorSpeed(-255, TopBeam);
    // Serial.println("moving top beam backward");
  }
  else if(Action == "top-beam-stop")
  {
    SetMotorSpeed(0, TopBeam);
    // Serial.println("stopping top beam");
  }
  else if (Action == "channel-valve-open")
  {
    ChannelValveOpen = true;
  }
  else if (Action == "channel-valve-close")
  {
    ChannelValveOpen = false;
  }
  else
  {
    // Serial.println("Invalid command. '" + Action + "'");
  }
  
  // control the channel valve
  if(ChannelValveOpen)
  { 
    digitalWrite(13, HIGH);
  }
  else
  {    
    digitalWrite(13, LOW);
  }
  
  // actually set the positions
  TopServo.write(TopServoPosition);
  LeftServo.write(LeftServoPosition);
  RightServo.write(RightServoPosition);
}

void Initialize()
{
  // move the servos to their correct positions
  TopServo.write(TopServoPosition);
  LeftServo.write(LeftServoPosition);
  RightServo.write(RightServoPosition);
}

void loop()
{
  if (Serial.available() > 0) {
      char Character = (char)Serial.read();  
      
      if(Character == '\n')
      {
        ExecuteCommand(Command);
        Command = "";
      }
      else
      {
        Command += Character;
      }
  }
}

