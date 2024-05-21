/* Motor Control w/ Joystick
 *
 * This program uses a jotstick to control a motor driven by
 * an Adafruit Motor Shield v3.
 * Horizontal thrusters (FORWARD, BACK, RIGHT, LEFT) will be controlled by a joystick.
 */

#include <stdio.h>
#include <Adafruit_MotorShield.h>

// MOTOR NEEDS SEPARATE VOLTAGE SUPPLY

// create motor shield object w/ default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield();
// set left motor to port M1, right motor to port M2
Adafruit_DCMotor *leftMotor = AFMS.getMotor(1);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(2);
Adafruit_DCMotor *vertMotor = AFMS.getMotor(3);

// pins A0, A1 are connected to joystick's analog outputs
#define xPin  A0
#define yPin  A1
// pin 7 is connected to joystick's button
#define buttonPin 7
// pin A2 is connected to potentiometer's output
#define potentPin A2

void setup() {

  // set button pin to be active low
  pinMode(buttonPin, INPUT_PULLUP);
  // begin serial communication at 9600 bits per sec
  Serial.begin(9600);
  Serial.println("Starting program...");

  // begin motor shield operation w/ default frequency (1.6kHz)
  if(!AFMS.begin()) {
    Serial.println("Could not find motor shield. Check wiring...");
    while(1);
  } // end if
  Serial.println("Motor shield found.");

} // end setup

void loop() {

  // read joystick values from pins
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  int buttonValue = digitalRead(buttonPin);
  // read potentiometer value
  int potentValue = analogRead(potentPin);
  // save max PWM to pass when running motors
  int motorPWM =  255;

  // vertical and horizontal thrusters must be controlled independently
  // if they are to be directed simultaneously

  // if potentiometer is dialed right
  if(potentValue > 700) {
    // thrust ROV downward
    vertMotor->run(FORWARD);
    vertMotor->setSpeed(motorPWM);
  } // else if potentiometer is dialed left
  else if(potentValue < 300) {
    // thrust ROV upward
    vertMotor->run(BACKWARD);
    vertMotor->setSpeed(motorPWM);
  } // else don't run vertical motor
  else vertMotor->run(RELEASE);

  // if joystick is pushed forward
  if(yValue < 300) {
    leftMotor->run(FORWARD);
    leftMotor->setSpeed(motorPWM);
    rightMotor->run(FORWARD);
    rightMotor->setSpeed(motorPWM);
  } // else if joystick is pushed backward
  else if(yValue > 700) {
    leftMotor->run(BACKWARD);
    leftMotor->setSpeed(motorPWM);
    rightMotor->run(BACKWARD);
    rightMotor->setSpeed(motorPWM);
  } // else if joystick is pushed right
  else if(xValue > 700) {
    //motorPWM = 255;
    leftMotor->run(FORWARD);
    leftMotor->setSpeed(motorPWM);
    rightMotor->run(BACKWARD);
    rightMotor->setSpeed(motorPWM);
  } // else if joystick is pushed left
  else if(xValue < 300) {
    //motorPWM = 255;
    leftMotor->run(BACKWARD);
    leftMotor->setSpeed(motorPWM);
    rightMotor->run(FORWARD);
    rightMotor->setSpeed(motorPWM);
  } // else don't run horizontal motors
  else {
    //motorPWM = 0;
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);
  } // end else 

  Serial.print(xValue);
  Serial.print("\t");
  Serial.print(yValue);
  Serial.print("\t");
  Serial.print(buttonValue);
  Serial.print("\t");
  Serial.println(potentValue);

  // 10 ms delay for stability
  delay(10);
} // end loop