//車両信号は全ての駅に送信するタイプ
//Serial1は東京、Serial2は小田原、Serial3は静岡、SoftwareSerial(Serial4)は名古屋
#include <SoftwareSerial.h>
int data = 0;//データ格納用

SoftwareSerial Serial4(10,11);//RXが10,TXが11

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial4.begin(9600);
  Serial4.listen();
  }

void loop(){
  
  while(Serial1.available() > 0){//東京から受け取った信号をPCに送信
   data = Serial1.read();
   Serial.write(data);
   }
   
  while(Serial2.available() > 0){//小田原から受け取った信号をPCに送信
   data = Serial2.read();
   Serial.write(data);
   }
  
  while(Serial3.available() > 0){//静岡から受け取った信号をPCに送信
   data = Serial3.read();
   Serial.write(data);
   }
  
  while(Serial4.available() > 0){//名古屋から受け取った信号をPCに送信
   data = Serial4.read();
   Serial.write(data);
   }
  
  while(Serial.available() > 0){//PCから受け取った信号を駅のArduinoに送信
    data = Serial.read();
    if(data == 0xE0 ||data == 0x10){//東京のサーボの信号を送信
      Serial1.write(data);
    }else if(data == 0x90 ||data == 0x50 ||data == 0xD0 ||data == 0x30){//小田原のサーボの信号を送信
      Serial2.write(data);
    }else if(data == 0xE8 ||data ==0x18 ||data == 0x98 ||data == 0x58){//静岡のサーボの信号を送信
      Serial3.write(data);
    }else if(data == 0xD8 ||data == 0x38 ||data == 0xE4 ||data == 0x14){//名古屋のサーボの信号を送信
      Serial4.write(data);
    }else{//車両用の信号は全ての駅に送信
      Serial1.write(data);
      Serial2.write(data);
      Serial3.write(data);
      Serial4.write(data);
    }
    
 }
      
   
    
    
 
