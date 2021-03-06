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
#define SLOWSPEED  100
#define FASTSPEED  600
char **empty_1 ;
bool homed = false;
bool Motors_enabled = false;
int runn = 0;
char inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete
int arg = 0;
String helptext = "Enable the Motors with E \nDisable the Motors with D \nOr You can run different Programms by sending/ typing:  a, b, c, d or 'H' to show this text.\n";
int Argument = 0;

#include <AccelStepper.h>
#include <MultiStepper.h>
#include <Servo.h>
#include <Cmd.h>

Servo Releaser_Servo;
AccelStepper motorX(1, MOTOR_X_STEP_PIN, MOTOR_X_DIR_PIN); 
AccelStepper motorY(1, MOTOR_Y_STEP_PIN, MOTOR_Y_DIR_PIN); 
MultiStepper Motors;



void setup() {
  //Setup the Servo
  Releaser_Servo.attach(SERVO);
  // Setup the Stepper Motors
  motorX.setEnablePin(MOTOR_X_ENABLE_PIN);
  motorX.setPinsInverted(false, false, true);  
  motorX.setAcceleration(FASTSPEED);
  motorX.setMaxSpeed(FASTSPEED);
  
  motorY.setEnablePin(MOTOR_Y_ENABLE_PIN);
  motorY.setPinsInverted(false, false, true);
  motorY.setAcceleration(FASTSPEED);
  motorY.setMaxSpeed(FASTSPEED);


  Motors.addStepper(motorX);
  Motors.addStepper(motorY);

  // SETUP Serial
  Serial.begin(9600);
  Serial.print(helptext);
  cmdInit(&Serial);


  cmdAdd("run", run_something);
  cmdAdd("Command",run_Programms);
  cmdAdd("C",run_Programms);
  cmdAdd("Speed",Motors_setSpeed);
  cmdAdd("s",Motors_setSpeed);
  cmdAdd("Init",init_func);
  cmdAdd("I",init_func);
  cmdAdd("Test",tests);
  cmdAdd("T",tests);
  cmdAdd("Help",help);
  cmdAdd("H",help);
  cmdAdd("Enable",Motors_enable);
  cmdAdd("E",Motors_enable);
  cmdAdd("Disable",Motors_disable);
  cmdAdd("D",Motors_disable);
  cmdAdd("Status",Status);
  cmdAdd("S",Status);
  cmdAdd("Release",CD_release);
  cmdAdd("R",CD_release);
  
     
}
// HERE IS THE MAIN LOOP
void loop() {
  cmdPoll();
}


// HERE ARE ALL REFERENCED FUNCTIONS
void run_Programms (int arg_cnt, char **args){
}
void help (int arg_cnt, char **args){
  Serial.println(helptext);
}
void Status (int arg_cnt, char **args){
  Serial.println("Motorsstate: "+ String(Motors_enabled));
  Serial.println("Motorposition: ");
  Serial.println("X: "+String(motorX.currentPosition()));
  Serial.println("Y: "+String(motorY.currentPosition()));
}
void tests (int arg_cnt, char **args){
  char input = args[1][0];
  switch (input) {
        case 'a':
          Serial.println("run simpletest");
          simpletests();
          break;
        case 'b':
          Serial.println("run simpletest_v2");
          simpletests_v2();
          break;
        case 'c':
          Serial.println("run multiple Motors simultaniously");
          multitests();
          break;
          default:
          Serial.println("Command:" + String(input));
          break;
        
        }
}


void run_something (int arg_cnt, char **args){
  Serial.println("I'm doing something with Argument: " + String(cmdStr2Num(args[1],10)+ cmdStr2Num(args[2],10)));
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
    pos[0] = 10;
    pos[1] = 200;
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
    if(digitalRead(X_ENDSTOP) == HIGH && !x_done){
      motorX.setSpeed(SLOWSPEED);
      motorX.move(-1);
      motorX.runSpeed();
      }
     else if (!x_done){x_done = true;
     Serial.println("X-Endstop Touched");
     }
    if(digitalRead(Y_ENDSTOP) == HIGH && !y_done ){
      motorY.setSpeed(SLOWSPEED);
      motorY.move(-1);
      motorY.runSpeed();
      }
     else if (!y_done){y_done = true;
     Serial.println("Y-Endstop Touched");
     }
    if(x_done && y_done){
    motorX.setCurrentPosition(0);
    motorY.setCurrentPosition(0);  
    notdone = false;
    homed = true;
    Serial.println("Init done!");
    }
  }
}



void Motors_setSpeed(int arg_cnt, char **args){
  int Speed = cmdStr2Num(args[2],10);
   motorY.setSpeed(Speed);
   motorX.setSpeed(Speed);
   Serial.println("Speed set to: " + String(Speed));
  }

void Motors_disable (int arg_cnt, char **args){
  motorX.disableOutputs();
  motorY.disableOutputs();
  Motors_enabled = false;
  Serial.println("Motors disabled");
  }
  
void Motors_enable (int arg_cnt, char **args){
  motorX.enableOutputs();
  motorY.enableOutputs();
  Motors_enabled = true;
  Serial.println("Motors enabled");
  }




 void CD_release(int arg_cnt, char **args){
  if (CONTINIOUS){
  Releaser_Servo.write(-180);
  delay(400);
  Releaser_Servo.write(180);
  delay(415);
  Releaser_Servo.write(90);
  }
  else{
  Releaser_Servo.write(180);
  delay(250);
  Releaser_Servo.write(0);
  delay(250);
  }

  Serial.println("CD released");
 }

 void init_func(int arg_cnt, char **args){
  Motors_enable(1,empty_1);
  searchEndstops();
  Motors_disable(1,empty_1);
  args[1] =FASTSPEED;
  Motors_setSpeed(1, args);
  Serial.println("Initialisiation complete!");
  }

