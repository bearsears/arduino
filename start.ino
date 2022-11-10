// C++ code
//

//the right motor will be controlled by the motor A pins on the motor driver
const int AIN1 = 12;           //control pin 1 on the motor driver for the right motor
const int AIN2 = 13;            //control pin 2 on the motor driver for the right motor
const int PWMA = 11;            //speed control pin on the motor driver for the right motor

//the left motor will be controlled by the motor B pins on the motor driver
const int PWMB = 3;           //speed control pin on the motor driver for the left motor
const int BIN2 = 2;           //control pin 2 on the motor driver for the left motor
const int BIN1 = 4;           //control pin 1 on the motor driver for the left motor

const int trigPinFront = 6; // Trigger Pin of Front Ultrasonic Sensor
const int echoPinFront = 5; // Echo Pin of Front Ultrasonic Sensor
const int trigPinBack= 10; // Trigger Pin Back Ultrasonic Sensor;
const int echoPinBack = 9; // Echo pin Back Ultrasonic Sensor;

int backupTime = 3000;
int forwardTime = 3000;
float distance = 0;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(trigPinFront, OUTPUT); // initialising pin 7 as output
  pinMode(echoPinFront, INPUT); // initialising pin 6 as input
  pinMode(trigPinBack, OUTPUT); // pin 10 as output
  pinMode(echoPinFront, INPUT); // pin 9 as input

  //set the motor control pins as outputs
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);

  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  
  Serial.begin(9600);
  Serial.print("to infinity and beyond!");
  
}

void loop()
{
  /* This is ultrasensor front and back.*/
  distance = getDistance(trigPinFront, echoPinFront);
  Serial.print("Front Distance: ");
  Serial.print(distance);
  Serial.println(" cm"); 
  delay(50);   //delay 50
  distance = getDistance(trigPinBack, echoPinBack);
  Serial.print("Back Distance: ");
  Serial.print(distance);
  Serial.println(" cm"); 

  /* Joystick input to engine speed converter */
  double left;
  double right;
  // put your main code here, to run repeatedly:
  int x = analogRead(A0);
  x -= 512;
  int y = analogRead(A1);
  y -= 512;
  engine_speed((double) x, (double) y, &left, &right);
  Serial.print("left engine:  ");
  Serial.print(left);
  Serial.print("    right engine:  ");
  Serial.println(right);
  leftMotor(left);
  rightMotor(right);

}

/**************************/

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
void leftMotor(int motorSpeed)                        //function for driving the left motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(BIN1, HIGH);                         //set pin 1 to high
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(BIN1, LOW);                          //set pin 1 to low
    digitalWrite(BIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMB, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
}

/********************************************************************************/
void rightMotor(int motorSpeed)                       //function for driving the right motor
{
  if (motorSpeed > 0)                                 //if the motor should drive forward (positive speed)
  {
    digitalWrite(AIN1, HIGH);                         //set pin 1 to high
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  else if (motorSpeed < 0)                            //if the motor should drive backward (negative speed)
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, HIGH);                         //set pin 2 to high
  }
  else                                                //if the motor should stop
  {
    digitalWrite(AIN1, LOW);                          //set pin 1 to low
    digitalWrite(AIN2, LOW);                          //set pin 2 to low
  }
  analogWrite(PWMA, abs(motorSpeed));                 //now that the motor direction is set, drive it at the entered speed
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

