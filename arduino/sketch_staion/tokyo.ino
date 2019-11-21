//uno
#include<IRremote.h>
#include<VarSpeedServo.h>

VarSpeedServo servo1;

const unsigned int servo1_RightSiganl =;//サーボを動かす信号
const unsigned int servo1_LeftSiganl =;


const int servoSpeed = 50; //1から255
const int servo1_RightAngle = ;//サーボ1を右にふるときの角度、0から180
const int servo1_LeftAngle = ;


const int recvPin = 11;//赤外線の受信ピン
int data = 0;//受信データ格納用
IRrecv irrecv(recvPin);
IRsend irsend;//sendPinはmegaは9,unoは3
decode_results results;//うまくいかないときはloopの中へ


void servo1Right(){//サーボ1を右にふる関数
  servo1.write(servo1_RightAngle, servoSpeed, true);
  }
void servo1Left(){
  servo1.write(servo1_LeftAngle, servoSpeed, true);
  }


void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  servo1.attach(6);
  }

void loop(){
  
  if(irrecv.decode(&results)){//車両から受け取った信号をシリアルで送信
    data = results.value % 256;//4バイト→1バイト
    irrecv.resume();
    Serial.write(data);//megaの場合はserial1を使う
    }

  while(serial.available() > 0){//シリアルで受け取った信号をもとに車両に送信またはサーボを動かす
    data = serial.read();  
    if(data == servo1_RightSiganl){
      servo1Right();
    }else if(data == servo1_LeftSiganl){
      servo1Left();
    }else{
      irsend.sendNEC(data + 551541504, 32);//1バイト→4バイト
      irrecv.enableIRIn();
    }
  }
  
  }
