//Serial1は東京、Serial2は小田原、Serial3は静岡、SoftwareSerialは名古屋

int data = 0;//データ格納用

void setup(){
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  SoftwareSerial(10,11);
  SoftwareSerial.begin(9600);
  }
void loop(){
  
  while(Serial1.available() > 0){
   data = Serial1.read();
   Serial.write(data);
   }
   
   
    
    
 
