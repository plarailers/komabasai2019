//NODEは停車するところ、EDGEは間の線路
//N0→E1→N1→E2→N2→E3→N3→E4→N4→E5→N5→E0→NO(Nはノード、Eはエッジ)
#include <SoftwareSerial.h>

const int train1_arrival = 0x2C;//車両が到着した信号(車両→Arduino)(channel_4)
const int train2_arrival = 0x20;
const int train3_arrival = 0x28;
const int train4_arrival = 0x24;

const int train1_departure = 0x8C;//出発信号(Arduino→車両)(channel_1)
const int train2_departure = 0x80;
const int train3_departure = 0x88;
const int train4_departure = 0x84;

int data = 0;//信号格納用

//NODE,EDGE共に車両番号が入り、車両がいない場合は0が入る
int NODE[6] = {};//NODEの宣言、初期化
int EDGE[6] = {};//EDGEの宣言、初期化

SoftwareSerial Serial4(10,11);

int serch(int train){//EDGE（配列)の何番目にtrain(整数)があるかを探す関数
for(int i=0; i<6; i++){
  if(EDGE[i] == train){//i番目の要素がtargetなら
    return i;//iを返す
  }
}

void serial_write(int signal){//信号を全ての駅に送信する関数
  Serial1.write(siganl);
  Serial2.write(siganl);
  Serial3.write(signal);
  Serial4.write(signal);
}

void depart(int train){//出発信号を送信する関数
  if(train == 1){
    serial_write(train1_departure);
  }else if(train == 2){
    serial_write(train2_departure);
  }else if(train == 3){
    serial_write(train3_departure);
  }else if(train == 4){
    serial_write(train4_departure);
  }
}

void signal_process(int data){//到着した信号を処理する関数
  if(data == train1_arrival）{//車両1がノードに到着したとき
    NODE[serch(1)] = 1;//そのノードに車両が入る
    EDGE[serch(1)] = 0;//車両がいたエッジが空く
  }else if(data == train2_arrival){
    NODE[serch(2)] = 2;
    EDGE[serch(2)] = 0;
  }else if(data == train3_arrival){
    NODE[serch(3)] = 3;
    EDGE[serch(3)] = 0;
  }else if(data == train4_arrival){
    NODE[serch(4)] = 3;
    EDGE[serch(4)] = 0;
  }
}



void setup(){

  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial4.begin(9600);
  Serial4.listen();


  NODE ={/*初期位置を書く*/};//ノードの初期位置を設定(エッジは最初は全て０)
}




void loop(){


  for(int i=0;i<6;i++){
    if(NODE[i] != 0 && EDGE[(i+1)%6] == 0 && NODE[(i+1)%6] == 0){//車両がノードにいて、1つ先のエッジとノードが空いているとき
      depart(NODE[i]);//出発させる
      EDGE[(i+1)%6] = NODE[i];//1つ先のエッジに車両が入る
      NODE[i] = 0;//車両がいたノードは空く
    }
  }

  while(Serial1.availabale() > 0){//Serial1で受け取った到着信号を処理
    data = Serial1.read();
    signal_process(data);
  }

  while(Serial2.availabale() > 0){//Serial2で受け取った到着信号を処理
    data = Serial2.read();
    signal_process(data);
  }

  while(Serial3.availabale() > 0){//Serial3で受け取った到着信号を処理
    data = Serial3.read();
    signal_process(data);
  }

  while(Serial4.availabale() > 0){//Serial4で受け取った到着信号を処理
    data = Serial4.read();
    signal_process(data);
  }


}
