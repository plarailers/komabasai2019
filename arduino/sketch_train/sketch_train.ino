//プロセッサについて偽物のArduino nanoはoldなんたらに、本物はoldじゃない方にする

#include <IRremote.h>
const unsigned long channel_1 = 0x20DFDB8C;
const unsigned long channel_2 = 0x20DFDB4C;
const unsigned long channel_3 = 0x20DFDBCC;
const unsigned long channel_4 = 0x20DFDB2C;
const unsigned long channel_5 = 0x20DFDBAC;
const unsigned long channel_no = 0xFFFFFFFF;

const int outPinA = 6;  //モーターのIN1
const int outPinB = 9;  //モーターのIN2
const int recvPin = 5; //赤外線の信号を受信
const int sensorPin = A6; //CdSセンサーの計測
const int volt_sensorPin = A7;  //電圧の計測

int cds[6] =  {};//差分制御用（マーカー）　番号が大きいほど最新
double ave[4] = {};
const int df = 8; //

unsigned long time = 0;//状況判別に使う時間を格納
int status = 3;//車両の状況
//1:ch1を受け取った〜ch1を受け取ってから2秒後
//2:ch1を受け取ってから2秒後〜他の信号(ch2,ch3)を受け取った　　　　　　　　＝前進中
//3:ch2を受け取った〜ch2を受け取ってから2秒後
//4:ch2を受け取ってから2秒後〜他の信号(ch2,ch3)を受け取った　　　　　　　　＝後進中
//5:ch3を受け取った　or マーカを読み取った〜他の信号(ch2,ch3)を受け取った　＝停車中


int value;  //CdSセンサーの計測値を格納
double volt_value;  //電圧の計測値を格納
byte speed; //モーターの回転スピード
IRsend irsend;  //sendPinはArduino nanoだと3番ピン
IRrecv irrecv(recvPin);
int flagBefore, flagNow; //信号を記録

double minVolt = 5.3; //この電圧を下回ったら電池交換が必要
double maxCdS = 185; //CdSがこの値よりも高くなったら銀シールの上を通過した
double ratio = 0.6; //Ahead時は遅くする

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(outPinA, OUTPUT);
  pinMode(outPinB, OUTPUT);
}

void printNumber(decode_results *results) { //赤外線センサーで読み取った信号を表示
  unsigned long value = results->value;
  Serial.print("HEX:");
  Serial.println(value, HEX);
  Serial.print("BIN:");
  Serial.println(value, BIN);
}

void moveAhead(byte speed){ //前進
  analogWrite(outPinA,speed * ratio); //PWMでスピードを変化させる
  analogWrite(outPinB,0);
}

void moveBack(byte speed){  //後進
  analogWrite(outPinA,0);
  analogWrite(outPinB,speed);
}

void stop(){  //停止
  analogWrite(outPinA,0);
  analogWrite(outPinB,0);
}

void move(decode_results *results, byte speed) {  //信号を受け取って動く指令
  if (results->value == channel_1) {
    Serial.println("Ahead");
    flagBefore = flagNow;
    flagNow = 1;
    status = 1;
    time = millis();
    moveAhead(speed);
  }
  else if (results->value == channel_2) {
    Serial.println("Back");
    flagBefore = flagNow;
    flagNow = 2;
    status = 3;
    time = millis();
    moveBack(speed);
  }
  else if (results->value == channel_3) {
    Serial.println("stop");
    status = 5;
    stop();
  }
  else if (results->value == channel_4) { //マーカーがあったときに自分で送った信号を受信した
    Serial.println("marker_sent");
  }
  else if (results->value == channel_no) {  //1111111111みたいな信号を受け取った
    Serial.println("nothing");
  }
  else {
    Serial.println("????"); //不明な信号を受け取った
    //delay(2000);
    //stop();
  }
  if (flagBefore == 0) {
      flagBefore = flagNow;
  }
}

void loop() {
  Serial.print(flagBefore);
  Serial.print(flagNow);

  decode_results  results;  //赤外線の指令を受け取る

  volt_value = analogRead(volt_sensorPin);  //電圧を計測
  volt_value *= 10;  //電圧をV単位に変換
  volt_value /= 1024;
  Serial.print(" ");
  Serial.print(volt_value); //変換後の電圧値を表示
  Serial.print("V");

  if (volt_value < minVolt) { //電圧が最低電圧よりも低かったら
    Serial.print(" battery_shortage");
    //irsend.sendNEC(channel_5, 32);  //電池が少ないことを母艦に伝える
    //irrecv.enableIRIn();
    speed = 255;  //最大の電圧をモーターにかける
    }

  else if (volt_value >= minVolt) { //電池が十分にあれば
    speed = 255*(minVolt/volt_value); //電圧が高い時ほどモーターに電圧をかけないようにして速度を一定にする
  }

  Serial.print(" speed:");  //スピードを表示
  Serial.print(speed); //電圧値によって決定したspeedを表示
  Serial.print(" ");

  if (irrecv.decode(&results)) {  //赤外線の指令を受け取ってモーターを動かす
    printNumber(&results);  //受け取った信号を表示
    move(&results, speed); //受け取った信号に応じた動きをする
    irrecv.resume();  //また信号を受け取れるようにする
  }

  value = analogRead(sensorPin);  //CdSセンサーで明るさを計測
  Serial.print(" light:");
  Serial.println(value);  //読み取った明るさを表示

  if(status == 1 && millis() - time > 2000){//ch1を受け取ってから2秒経過したか判別
    status = 2;
  }else if(status == 3 && millis() - time > 2000){//ch2を受け取ってから2秒経過したか判別
    status = 4;
  }


  for(int i=0; i<5; i++){
    cds[i] = cds[i+1];
  }

  cds[5] = value;
  ave[0] = (cds[0]+cds[1]+cds[2])/3;//移動平均を計算
  ave[1] = (cds[1]+cds[2]+cds[3])/3;
  ave[2] = (cds[2]+cds[3]+cds[4])/3;
  ave[3] = (cds[3]+cds[4]+cds[5])/3;

  if((ave[3]-ave[2]) > df && (ave[2]-ave[1]) > df && (ave[1]-ave[0]) > df && status != 1 && status != 3 && status != 5){
    Serial.print(" marker_exist"); //マーカーがあった
//    irsend.sendNEC(channel_4, 32);  //母艦にマーカーの存在を伝達
//    irrecv.enableIRIn();  //また赤外線の信号を受け取れるようにする
//    ここでコメントアウトしているのはirsendおよびirrecvをするとつぎにリモコンからの指令を受け取れないからである
    if (flagBefore == flagNow) { //赤外線による信号が
      //直前のものと同じなら
      stop();  
    }
  else {//違うならそのまま止める。
    
    delay(2000);//時間をちょっと空けておく
    flagBefore = flagNow;
    }
    irsend.sendNEC(channel_4, 32);  //母艦にマーカーの存在を伝達
    irrecv.enableIRIn();  //また赤外線の信号を受け取れるようにする
    stop();//停車
    status == 5;
  }





}
