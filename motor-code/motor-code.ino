#include <SPI.h>
#include "src/Nrf/Nrf.h"

#define forwardMulti 11
#define borwardMulti 11
#define lorwardMulti 15
#define rorwardMulti 15

#define dirPin0 2 //Pin which toggles the direction
#define stepPin0 3 //Pin that sends the step pulse

#define dirPin1 4 // 4
#define stepPin1 5 // 5

#define dirPin2 6 // 6
#define stepPin2 9 // 9

#define dirPin3 10 // 10
#define stepPin3 11 // 11

int minMicrosDelay = 1600;
int maxMicrosDelay = 800;

//const byte address[][12] = {"arduino_read", "pi_read"};
const byte address[][6] = {"00001", "00002"};
//const byte address[][6] = {"00006", "00008"};

RF24 rfradio(7, 8);
Nrf nrf(&rfradio);

void setup() {
  Serial.begin(9600);
  nrf.radio->begin();
  nrf.radio->setRetries(5,15);
  nrf.radio->enableDynamicPayloads();

  // Do not use 0 as reading pipe! This pipe is already in use ase writing pipe
  nrf.radio->openWritingPipe(address[1]);
  nrf.radio->openReadingPipe(1, address[0]);
  nrf.radio->startListening();
  Serial.println("Started");

  pinMode(dirPin0, OUTPUT);
  pinMode(stepPin0, OUTPUT);

  pinMode(dirPin1, OUTPUT);
  pinMode(stepPin1, OUTPUT);

  pinMode(dirPin2, OUTPUT);
  pinMode(stepPin2, OUTPUT);

  pinMode(dirPin3, OUTPUT);
  pinMode(stepPin3, OUTPUT);
}

void loop() {
  //read serial, x = stop, s = start.
//  if (Serial.available() > 0) {
//    char in = Serial.read();
//    Serial.println("> '" + String(in) + "'");
//    moveDir(in, 500);
//  }
  char message[32] = "";
    nrf.readMessage(message, nrf.radio->getDynamicPayloadSize());
    if (strlen(message) != 0)
    {
      Serial.println(message);
      char dir = getDirection(message);
      int distance = getDistance(message);
      moveDir(dir, distance);
    }
  delay(100);
}

char getDirection(char input[]){
  return input[0];
}

long getDistance(char input[]){
  String output = "";
  for(int i = 1; i < 32; i++){
    if(isdigit(input[i])){
      output += input[i];
    }else{
      break;
    }
  }
  return output.toInt();
}

void changeDir(int whichMotor, int dir) {
  switch (whichMotor) {
    case 0:
      digitalWrite(dirPin0, dir);
      break;
    case 1:
      digitalWrite(dirPin1, dir);
      break;
    case 2:
      digitalWrite(dirPin2, dir);
      break;
    case 3:
      digitalWrite(dirPin3, dir);
      break;
    case 4:
      digitalWrite(dirPin0, dir);
      digitalWrite(dirPin1, dir);
      digitalWrite(dirPin2, dir);
      digitalWrite(dirPin3, dir);
      break;
    default:
      break;
  }
}

//steps one step. Be careful, no safety features here.
void stepMotor(int delayM) {
 // Serial.println(delayM);
  
  digitalWrite(stepPin0, HIGH);
  digitalWrite(stepPin1, HIGH);
  digitalWrite(stepPin2, HIGH);
  digitalWrite(stepPin3, HIGH);
  delayMicroseconds(delayM);
  digitalWrite(stepPin0, LOW);
  digitalWrite(stepPin1, LOW);
  digitalWrite(stepPin2, LOW);
  digitalWrite(stepPin3, LOW);
  delayMicroseconds(delayM);
}

//rotate/move  a certain amount of steps.
void takeSteps(int distance, int multiplier) {
  int steps = distance * multiplier;
  // Enables the motor to move in a particular direction
  // Makes 200 pulses one full cycle rotation
  int tenPercent = steps * 0.2;
  int ninetyPercent = steps * 0.8;
  
  for (int i = 0; i < steps; i++) {
    //Update the control center
    
    //
    if(i < tenPercent) {
      stepMotor(map(i, 0, tenPercent, minMicrosDelay, maxMicrosDelay));
    } else if(i > ninetyPercent){
      stepMotor(map(i, ninetyPercent, steps, maxMicrosDelay, minMicrosDelay));    
    } else {
      stepMotor(maxMicrosDelay);  
    }
  }
  delay(100);
  char doneMessage[32] = "Done";
  Serial.println("Done");
   bool ok = nrf.sendMessage(doneMessage, (sizeof(doneMessage)/sizeof(char)));
   while (!ok){
    ok = nrf.sendMessage(doneMessage, (sizeof(doneMessage)/sizeof(char)));
    Serial.println("Sending Done");
    delay(500);
   }
}

#define CLOCKWISE HIGH
#define ANTICLOCKWISE LOW

void moveDir(char whichDir, int steps){
  switch(whichDir){
    case 'F':
      Serial.println("Moving 'f'orward");
      changeDir(0, CLOCKWISE);
      changeDir(1, ANTICLOCKWISE);
      changeDir(2, CLOCKWISE);
      changeDir(3, ANTICLOCKWISE);
      takeSteps(steps, forwardMulti);
    break;
    
    case 'B':
      Serial.println("Moving 'b'orward");
      //move 'b'orward
      changeDir(0, ANTICLOCKWISE);
      changeDir(1, CLOCKWISE);
      changeDir(2, ANTICLOCKWISE);
      changeDir(3, CLOCKWISE);
      takeSteps(steps, borwardMulti);
    break;
    
    case 'R':
      Serial.println("Moving 'r'orward");
      //move 'r'orward

      changeDir(0, CLOCKWISE);
      changeDir(1, CLOCKWISE);
      changeDir(2, ANTICLOCKWISE);
      changeDir(3, ANTICLOCKWISE);
      
      takeSteps(steps, rorwardMulti);
    break;
    
    case 'L':
      Serial.println("Moving 'l'orward");
      //move 'l'orward

      changeDir(0, ANTICLOCKWISE);
      changeDir(1, ANTICLOCKWISE);
      changeDir(2, CLOCKWISE);
      changeDir(3, CLOCKWISE);
      
      takeSteps(steps, lorwardMulti);
    break;
//    case 'e':
//      Serial.println("Rotating rorward (e)");
//
//      changeDir(4, ANTICLOCKWISE);
//      
//      takeSteps(steps);
//    break;
//    case 'q':
//      Serial.println("Rotatling lorward (q)");
//
//      changeDir(4, CLOCKWISE);
//      
//      takeSteps(steps);
//    break;
    default:
      Serial.println("Watchu mean");
    break;
  }
}
