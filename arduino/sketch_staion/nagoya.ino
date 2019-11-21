//uno
#include<IRremote.h>
#include<VarSpeedServo.h>

VarSpeedServo servo1;
VarSpeedServo servo2;

const unsigned int servo1_Siganl0 = 0x20DFDBD8;//サーボを動かす信号
const unsigned int servo1_Siganl1 = 0x20DFDB38;
const unsigned int servo2_Siganl0 = 0x20DFDBE4;
const unsigned int servo2_Siganl1 = 0x20DFDB14;


const int servoSpeed = 50; //1から255
const int servo1_Angle0 = ;//サーボ1を右にふるときの角度、0から180
const int servo1_Angle1 = ;
const int servo2_Angle0 = ;
const int servo2_Angle1 = ;


const int recvPin = 11;//赤外線の受信ピン
int data = 0;//受信データ格納用
IRrecv irrecv(recvPin);
IRsend irsend;//sendPinはmegaは9,unoは3
decode_results results;//うまくいかないときはloopの中へ


void servo1_0(){//サーボ1を右にふる関数
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
  Serial.begin(9600);
  irrecv.enableIRIn();
  servo1.attach(/*ピン番号*/);
  servo2.attach(/*ピン番号*/);
  }

void loop(){
  
  if(irrecv.decode(&results)){//車両から受け取った信号をシリアルで送信
    data = results.value % 256;//4バイト→1バイト
    irrecv.resume();
    Serial.write(data);//megaの場合はserial1を使う
    }

  while(serial.available() > 0){//シリアルで受け取った信号をもとに車両に送信またはサーボを動かす
    data = serial.read();  
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
  }
  
  }
