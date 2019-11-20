#include<IRremote.h>
#include<VarSpeedServo.h>

VarSpeedServo servo1;
VarSpeedServo servo2;

const int servoSpeed = /*サーボのスピード*/; //1から255、10から30程度が最適か
const int servo1_RightAngle = ;//サーボ1を右にふるときの角度、0から180
const int servo1_LeftAngle = ;
const int servo2_RightAngle = ;
const int servo2_LeftAngle = ;


const int outPin = ;//赤外線の送信ピン
const int recvPin = ;//赤外線の受信ピン
int data = 0;//受信データ格納用
IRrecv irrecv(recvPin);
decode_results results;//うまくいかないときはloopの中へ

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
  pinMode(outPin, OUTPUT);
  }

void loop(){
  if(irrecv.decode(&results)){//車両から受け取った信号をシリアルで送信
    data = results.value % 256;
    irrecv.resume();
    Serial.write(data);//megaの場合はserial1を使う
    }
  

  
  
    
    
  
  
