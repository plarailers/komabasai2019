//Serial1は東京、Serial2は小田原、Serial3は静岡、SoftwareSerialは名古屋

int data = 0;//データ格納用

SoftwareSerial Serial3;

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  SoftwareSerial(10,11);
  SoftwareSerial.begin(9600);
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
  
  while(SoftwareSerial3.available() > 0){//名古屋から受け取った信号をPCに送信
   data = SoftwareSerial3.read();
   Serial.write(data);
   }
   
    
    
 
