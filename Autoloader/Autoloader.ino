

#define X_ENDSTOP 9
#define Y_ENDSTOP 10
#define SERVO 12
#define MOTOR_X_ENABLE_PIN 8
#define MOTOR_X_STEP_PIN 2
#define MOTOR_X_DIR_PIN 5
 
#define MOTOR_Y_ENABLE_PIN 8
#define MOTOR_Y_STEP_PIN 3
#define MOTOR_Y_DIR_PIN 6
#define CONTINIOUS TRUE // IS IT A CONTINIOUS Rotation Servo

#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>
Servo Releaser_Servo;

AccelStepper motorX(1, MOTOR_X_STEP_PIN, MOTOR_X_DIR_PIN); 
AccelStepper motorY(1, MOTOR_Y_STEP_PIN, MOTOR_Y_DIR_PIN); 

int runn = 0;

void setup() {
  //Setup the Servo
  Releaser_Servo.attach(SERVO);
  // Setup the Stepper Motors
  motorX.setEnablePin(MOTOR_X_ENABLE_PIN);
  motorX.setPinsInverted(false, false, true);  
  motorX.setAcceleration(100);
  //motorX.enableOutputs();
  
  motorY.setEnablePin(MOTOR_Y_ENABLE_PIN);
  motorY.setPinsInverted(false, false, true);
  motorY.setAcceleration(100);  
  //motorY.enableOutputs();
}

void loop() {
  if (runn == 0){
    tests();
    runn++;
  }
  
 }

void tests (){
 motorX.move(3000);
  motorX.run();
  motorY.move(3000);
  motorY.run();
  CD_release();
 }

 void CD_release(){
  if (CONTINIOUS = TRUE){
  Releaser_Servo.write(180);
  delay(250);
  Releaser_Servo.write(0);
  delay(250);
  Releaser_Servo.write(90);
  }
  else{
  Releaser_Servo.write(180);
  delay(250);
  Releaser_Servo.write(0);
  delay(250);
  }
 }

 
