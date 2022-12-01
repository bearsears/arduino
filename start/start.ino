// C++ code
//

//the right motor will be controlled by the motor A pins on the motor driver
const int AIN1 = 12;           //control pin 1 on the motor driver for the right motor
const int AIN2 = 13;            //control pin 2 on the motor driver for the right motor
const int PWMA = 11;            //speed control pin on the motor driver for the right motor

//the left motor will be controlled by the motor B pins on the motor driver
const int BIN1 = 8;           //control pin 1 on the motor driver for the left motor
const int BIN2 = 9;           //control pin 2 on the motor driver for the left motor
const int PWMB = 10;           //speed control pin on the motor driver for the left motor

//Ultrasonic sensor.
const int trigPinFront = 6; // Trigger Pin of Front Ultrasonic Sensor
const int echoPinFront = 5; // Echo Pin of Front Ultrasonic Sensor
const int trigPinBack= 3; // Trigger Pin Back Ultrasonic Sensor;
const int echoPinBack = 2; // Echo pin Back Ultrasonic Sensor;
const int warningLight = 4; // Warning light for ultrasonic sensor;
float distanceFront = 0;
float distanceBack = 0;
float distanceWarning = 20;

//Tilt sensor
const int tiltPin = 7;

//joystick pins
const int xaxis = A0;
const int yaxis = A1;


void setup()
{
  //set the motor control pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  //set the ultrasonic sensor
  pinMode(trigPinFront, OUTPUT); // initialising pin 6 as output
  pinMode(echoPinFront, INPUT); // initialising pin 5 as input
  pinMode(trigPinBack, OUTPUT); // pin 10 as output
  pinMode(echoPinFront, INPUT); // pin 9 as input
  pinMode(warningLight, INPUT); // pin 8 for input, warning light

  //set the tilt sensor
  pinMode(tiltPin, INPUT); // initializing pin 7 as tilt
  
  //start serial
  Serial.begin(9600);
  //Serial.print("to infinity and beyond!");
}

void loop()
{
  //check distance using ultrasonic
  check_distance();

  /* Joystick input to engine speed converter */
  double left;
  double right;
  int x = analogRead(xaxis);
  x -= 512;
  int y = analogRead(yaxis);
  y -= 512;
  engine_speed((double) x, (double) y, &left, &right);
  /*
  Serial.print("left engine:  ");
  Serial.print(left);
  Serial.print("    right engine:  ");
  Serial.println(right);
  */
  leftMotor(left);
  rightMotor(right);
  check_tilt();
  

}
void check_distance() {
  /* This is ultrasensor front and back.*/
  distanceFront = getDistance(trigPinFront, echoPinFront);
  //Serial.print("Front Distance: ");
  //Serial.print(distance);
  //Serial.println(" cm"); 
  //delay(50);   //delay 50
  distanceBack = getDistance(trigPinBack, echoPinBack);
  //Serial.print("Back Distance: ");
  //Serial.print(distance);
  //Serial.println(" cm"); 

  /*if the distance low, the warning light should be on.*/
  if (distanceFront < distanceWarning || distanceBack <  distanceWarning) {
    digitalWrite(warningLight, HIGH);
  } else {
    digitalWrite(warningLight, LOW);
  }
}

/**************************/
void check_tilt() { 
  //Serial.println(digitalRead(tiltPin));
  if (digitalRead(tiltPin)) {
    Serial.println("This needs to get to a server");
    delay(10000);
  }
  //digitalWrite(tiltPin, LOW);
  /* 
  else {
    Serial.print(".");
  }
  */
  return;
}

float getDistance(int tp, int ep)
{
  float echoTime;                   //variable to store the time it takes for a ping to bounce off an object
  float calculatedDistance;         //variable to store the distance calculated from the echo time

  //send out an ultrasonic pulse that's 10ms long
  digitalWrite(tp, LOW);
  delayMicroseconds(2);
  digitalWrite(tp, HIGH);
  delayMicroseconds(10);
  digitalWrite(tp, LOW);

  echoTime = pulseIn(ep, HIGH);      //use the pulsein command to see how long it takes for the
                                          //pulse to bounce back to the sensor
  calculatedDistance = echoTime / 148.0;  //calculate the distance of the object that reflected the pulse (half the bounce time multiplied by the speed of sound)
  calculatedDistance *= 2.54; //convert to cm.

  return calculatedDistance;              //send back the distance that was calculated
}

/********************************************************************************/
/* 

This is for left motor control, takes in integer,
and tells to the motor how much to move.

*/
void leftMotor(int motorSpeed)          //function for driving the left motor
{
  if (motorSpeed > 0)             //if the motor should drive forward (positive speed)
  {
    digitalWrite(BIN1, HIGH);     //set pin 1 to high
    digitalWrite(BIN2, LOW);      //set pin 2 to low
  }
  else if (motorSpeed < 0)        //if the motor should drive backward (negative speed)
  {
    digitalWrite(BIN1, LOW);      //set pin 1 to low
    digitalWrite(BIN2, HIGH);     //set pin 2 to high
  }
  else                            //if the motor should stop
  {
    digitalWrite(BIN1, LOW);      //set pin 1 to low
    digitalWrite(BIN2, LOW);      //set pin 2 to low
  }
  analogWrite(PWMB, abs(motorSpeed));  //now that the motor direction is set, drive it at the entered speed
}

/********************************************************************************/
void rightMotor(int motorSpeed)   //function for driving the right motor
{
  if (motorSpeed > 0)             //if the motor should drive forward (positive speed)
  {
    digitalWrite(AIN1, HIGH);     //set pin 1 to high
    digitalWrite(AIN2, LOW);      //set pin 2 to low
  }
  else if (motorSpeed < 0)        //if the motor should drive backward (negative speed)
  {
    digitalWrite(AIN1, LOW);      //set pin 1 to low
    digitalWrite(AIN2, HIGH);     //set pin 2 to high
  }
  else                            //if the motor should stop
  {
    digitalWrite(AIN1, LOW);      //set pin 1 to low
    digitalWrite(AIN2, LOW);      //set pin 2 to low
  }
  analogWrite(PWMA, abs(motorSpeed));   //now that the motor direction is set, drive it at the entered speed
}


/******END***************************END*****************************************/

void engine_speed(double x, double y, double* lp, double* rp) {
  double left;
  double right;
  double r;
  double t;
  r = hypot(x, y);
  t = atan2(y, x);
  //Serial.println(r);
  //Serial.println(t);
  
  t -= M_PI / 4;
  left = r * cos(t);
  right = r * sin(t);
  left *= sqrt(2);
  left /= 2;
  right *= sqrt(2);
  right /= 2;
  //Serial.println(left);
  //Serial.println(right);
  //Division two for converting distance to engine speed,
  //255 / 512
  left = max(-255, min(left, 255)); 
  right = max(-255, min(right, 255));
  *lp = left;
  *rp = right;
    
  return;
}

