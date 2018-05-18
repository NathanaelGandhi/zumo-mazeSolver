//Include libraries
#include <QTRSensors.h>
#include <ZumoReflectanceSensorArray.h>
#include <ZumoMotors.h>
#include <ZumoBuzzer.h>
#include <Pushbutton.h>

//Define some global variables
#define TRIG 4
#define ECHO 2

Pushbutton button(ZUMO_BUTTON);
ZumoMotors motors;
ZumoReflectanceSensorArray reflectanceSensors;

int lastError = 0;
const int MAX_SPEED = 400;
byte pins[]={5,A2,A0,11,A5};

//motors.setSpeeds(leftSpeed, rightSpeed); //controls each wheel with 400 max and -400 reverse

//Runs once on arduino startup
void setup() {
  Serial.begin(9600);
//  pinMode(TRIG,OUTPUT);
//  pinMode(ECHO,INPUT);
  reflectanceSensors.init();
  Serial.println("HI");
}

//Main program loop
void loop() {
  Serial.println("LET ME LOOSE");
  callibrate();
  Serial.println("Calibration Done");
  pinMode(TRIG,OUTPUT);
  pinMode(ECHO,INPUT);
  Serial.println("Moving over to ultrasonics");
  button.waitForButton();
  goStraight();
  turnRight();
  goStraight();
  turnLeft();
  goStraight();
  turnLeft();
  goStraight();
  turnRight();
  goStraight();
  motors.setSpeeds(0,0);
  Serial.println("Moving over to reflectance sensors");
  //reflectanceSensors.init();
  while(true){
    unsigned int sensors[6];
    int position = reflectanceSensors.readLine(sensors);
    Serial.print("Position: ");Serial.println(position);
    int error = position - 2500;
    int speedDifference = error / 4+6*(error-lastError);
    Serial.print("SpeedDifference"); Serial.println(speedDifference);
    lastError = error;
    int m1Speed = MAX_SPEED + speedDifference;
    int m2Speed = MAX_SPEED - speedDifference;
    if(m1Speed <0)
      m1Speed=0;
    if(m2Speed<0)
      m2Speed=0;
    if(m1Speed>MAX_SPEED)
      m1Speed=MAX_SPEED;
    if(m2Speed>MAX_SPEED)
      m2Speed=MAX_SPEED;
    motors.setSpeeds(m1Speed, m2Speed);
  }
}

void goStraight(){
  while(getDistance() > 4){
     motors.setSpeeds(200, 200);
  }
}
void turnRight(){
  Serial.println("Turning Right");
  int leftSpeed = 200;
  int rightSpeed = -200;
  motors.setSpeeds(leftSpeed, rightSpeed);
  delay(350);
  motors.setSpeeds(0,0);
}
void turnLeft(){
  Serial.println("Turning Left");
  int leftSpeed = -200;
  int rightSpeed = 200;
  motors.setSpeeds(leftSpeed, rightSpeed);
  delay(400);
  motors.setSpeeds(0,0);
}
int getDistance(){
  digitalWrite(TRIG,LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG,LOW);
  float distance = pulseIn(ECHO, HIGH);
  distance = distance /58;
  return distance;
}

void callibrate(){
  for(int i=0;i<80;i++)
  {
    if((i>10&&i<=30)||(i>50&&i<=70))
      motors.setSpeeds(-200,200);
    else
      motors.setSpeeds(200,-200);
    reflectanceSensors.calibrate();
    delay(20);
  }
  motors.setSpeeds(0,0);
}

