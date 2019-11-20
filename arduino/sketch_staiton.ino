#include<IRremote.h>
#include<VarSpeedServo.h>

VarSpeedServo servo1;
VarSpeedServo servo2;

int servoSpeed = /*サーボのスピード*/; //1から255、10から30程度が最適か
int servo1_RightAngle = ;//サーボ1を右にふるときの角度
int servo1_LeftAngle = ;//0から180
int servo2_RightAngle = ;
int servo2_LeftAngle = ;

void servo1Right(){//サーボを右にふる関数
  servo1.write(servo1_RightAngle, servoSpeed, true);
  }
void servo1Left(){
  servo1.write(servo1_LeftAngle, servoSpeed, true);
  }
void servo2Right(){
  servo2.write(servo2_RightAngle, servoSpeed, true);
  }
void servo2Left(){
  servo2.write(servo2_LefttAngle, servoSpeed, true);
  }




void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  servo1.attach(/*ピン番号*/);
  servo2.attach(/*ピン番号*/);
  }

  
