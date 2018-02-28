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
MultiStepper Motors;
int runn = 0;
char inputString = '0';         // a String to hold incoming data
boolean stringComplete = false;  // whether the string is complete

String helptext = "Enable the Motors with E \n Disale the Motors with D \n Or You can run different Programms by sending/ typing:  a, b, c, d or 'H' to show this text.";



void setup() {
  //Setup the Servo
  Releaser_Servo.attach(SERVO);
  // Setup the Stepper Motors
  motorX.setEnablePin(MOTOR_X_ENABLE_PIN);
  motorX.setPinsInverted(false, false, true);  
  motorX.setAcceleration(360);
  motorX.setMaxSpeed(360);
  
  motorY.setEnablePin(MOTOR_Y_ENABLE_PIN);
  motorY.setPinsInverted(false, false, true);
  motorY.setAcceleration(360);
  motorY.setMaxSpeed(360);  


  Motors.addStepper(motorX);
  Motors.addStepper(motorY);

  // SETUP Serial
  Serial.begin(9600);
  Serial.print(helptext);
  // reserve 200 bytes for the inputString:
}
// HERE IS THE MAIN LOOP
void loop() {
  if (stringComplete) {
      switch (inputString) {
        case 'a':
          simpletests();
          break;
        case 'b':
          simpletests_v2();
          break;
        case 'c':
          init_func();
          break;
        case 'H':
          Serial.print(helptext);
          break;
        case 'D':
          Motors_disable();
          break;
        case 'E':
          Motors_enable();
          break;
      }
      // clear the string:
      inputString = '0';
      stringComplete = false;
    }
}



// HERE ARE ALL REFERENCED FUNCTIONS
void tests (){
  //Motors_enable();
 motorX.move(2000);
  motorX.run();
  motorY.move(2000);
  motorY.run();
 // delay(1000);
  CD_release();
 }

void simpletests (){
   motorX.move(200);
   motorX.run();
   motorY.move(200);
   motorY.run();
  }

void simpletests_v2 (){
   motorX.runToNewPosition(2000);
   motorY.runToNewPosition(2000);
   motorX.runToNewPosition(0);
   motorY.runToNewPosition(0);
  
  }
  
void multitests (){
   long pos[2];
    pos[0] = 2000;
    pos[1] = 2000;
   Motors.moveTo(pos);
   Motors.runSpeedToPosition();
    delay(1000);
    long zero[2];
    zero[0] = 0;
    zero[1] = 0;
   Motors.moveTo(zero);
   Motors.runSpeedToPosition(); 
  }



void searchEndstops (){
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

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    if (inChar == '\n') {
      stringComplete = true;
    }
    else{
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    }
  }
}
