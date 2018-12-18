#define dirPin 4 //Pin which toggles the direction
#define stepPin 3 //Pin that sends the step pulse

int microsDelay = 500; //Delay in microseconds for each pulse. Lower = faster speed, but arduino might mess up low delays, and so might the steppermotor.
int dir = HIGH; //default direction.
long pos = 0; //we count position

void setup() {
  Serial.begin(9600);
  Serial.println("Started");

  pinMode(dirPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
}

bool doRotate = true;

void loop() {
  //read serial, x = stop, s = start.
  if (Serial.available() > 0) {
    char in = Serial.read();

    //compare the char and toggle doRotate.
    if (in == 's') { //start rotating
      Serial.println("doRotate: true");
      doRotate = true;
      
    } else if (in == 'x') { //stop rotating
      Serial.println("doRotate: false");
      doRotate = false;
      
    } else if (in == 'p'){ //print pos
      Serial.print("pos: ");
      Serial.println(pos, DEC);
    }
  }

  if (doRotate) {
    dir = !dir;               //switch direction
    Serial.print("dir: ");
    Serial.print(dir?"HIGH":"LOW");
    Serial.println("\t rotate(200)");
    rotate(200);              //rotate 200 * 1.8 = 360 degrees.
    delay(500);               //delay to prevent spam
  }
}

//steps one step. Be careful, no safety features here.
void stepMotor(int delayM) {
  digitalWrite(stepPin, HIGH);
  delayMicroseconds(delayM);
  digitalWrite(stepPin, LOW);
  delayMicroseconds(delayM);
}

//rotate/move  a certain amount of steps.
void rotate(int steps) {
  digitalWrite(dirPin, dir);
  // Enables the motor to move in a particular direction
  // Makes 200 pulses one full cycle rotation
  for (int i = 0; i < steps; i++) {
    stepMotor(microsDelay);
    //change position each step.
    if (dir) {
      pos++;
    } else {
      pos--;
    }
  }
}
