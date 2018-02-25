

#define X_ENDSTOP 9
#define Y_ENDSTOP 10
#define SERVO 12
#define MOTOR_X_ENABLE_PIN 8
#define MOTOR_X_STEP_PIN 2
#define MOTOR_X_DIR_PIN 5
 
#define MOTOR_Y_ENABLE_PIN 8
#define MOTOR_Y_STEP_PIN 3
#define MOTOR_Y_DIR_PIN 6
#define CONTINIOUS true // IS IT A CONTINIOUS Rotation Servo
#define SLOWSPEED 100
#define FASTSPEED 2000
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
  motorX.setAcceleration(2000);
  
  motorY.setEnablePin(MOTOR_Y_ENABLE_PIN);
  motorY.setPinsInverted(false, false, true);
  motorY.setAcceleration(2000);  
}
// HERE IS THE MAIN LOOP
void loop() {
  if (runn == 0){
    Motors_enable();
    tests();
    runn++;
  }
  Motors_disable();
}



// HERE ARE ALL REFERENCED FUNCTIONS




void searchEndstops(){
bool  notdone = true;
bool x_done = false;
bool y_done =false;
  while (notdone){
    if(digitalRead(X_ENDSTOP) == LOW){
      motorX.setSpeed(SLOWSPEED);
      motorX.move(-200);
      motorX.run();
      }
     else{x_done = true;}
    if(digitalRead(Y_ENDSTOP) == LOW ){
      motorY.setSpeed(SLOWSPEED);
      motorY.move(-200);
      motorY.run();
      }
     else{y_done = true;}
    if(x_done && y_done){
    motorX.setCurrentPosition(0);
    motorY.setCurrentPosition(0);  
    notdone =false;
    }
  }
}





void Motors_disable (){
  motorX.disableOutputs();
  motorY.disableOutputs();
  }
  
void Motors_enable (){
  motorX.enableOutputs();
  motorY.enableOutputs();
  }


void tests (){
 motorX.move(2000);
  motorX.run();
  motorY.move(2000);
  motorY.run();
  CD_release();
 }

 void CD_release(){
  if (CONTINIOUS){
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

 void init_func(){
  Motors_enable();
  searchEndstops();
  Motors_disable();
  }
