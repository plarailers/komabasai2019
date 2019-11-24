//mega
//odawara
#include<IRremote.h>
#include<VarSpeedServo.h>

VarSpeedServo servo1;
VarSpeedServo servo2;

const unsigned long servo1_Siganl0 = 0x2C;//サーボを動かす信号
const unsigned long servo1_Siganl1 = 0xAC;
const unsigned long servo2_Siganl0 = 0xD0;
const unsigned long servo2_Siganl1 = 0x30;


const int servoSpeed = 50; //1から255
const int servo1_Angle0 = 0;//サーボ1を直進にするときの角度、0から180
const int servo1_Angle1 = 130;
const int servo2_Angle0 = 0;
const int servo2_Angle1 = 180;


const int recvPin = 11;//赤外線の受信ピン
int data = 0;//受信データ格納用
IRrecv irrecv(recvPin);
IRsend irsend;//sendPinはmegaは9,unoは3
decode_results results;//うまくいかないときはloopの中へ


void servo1_0(){//サーボ1を直進にする関数
  servo1.write(servo1_Angle0, servoSpeed, true);
  }
void servo1_1(){
  servo1.write(servo1_Angle1, servoSpeed, true);
  }
void servo2_0(){
  servo2.write(servo2_Angle0, servoSpeed, true);
  }
void servo2_1(){
  servo2.write(servo2_Angle1, servoSpeed, true);
  }


void setup(){
  Serial1.begin(9600);
  irrecv.enableIRIn();
  servo1.attach(13);
  servo2.attach(6);
  }

void loop(){
  
 if(irrecv.decode(&results)){//車両から受け取った信号をシリアルで送信
    data = results.value;//4バイト→1バイト
    irrecv.resume();
    if(data == 20DFDBAC || 659A20DF){//待避
      servo1_0();
      }else if(data == 20DFDB2C || 659AC03F){//直進
      servo1_1();
      }
 }
      
 

 /* while(Serial1.available() > 0){//シリアルで受け取った信号をもとに車両に送信またはサーボを動かす
    data = Serial1.read();  
    if(data == servo1_Siganl0){
      servo1_0();
    }else if(data == servo1_Siganl1){
      servo1_1();
    }else if(data == servo2_Siganl0){
      servo2_0();
    }else if(data == servo2_Siganl1){
      servo2_1();
    }else{
      irsend.sendNEC(data + 551541504, 32);//1バイト→4バイト
      irrecv.enableIRIn();
    }
  }*/
  
  
  }
