/*
  送信テスト用
*/

#include <IRremote.h>

const unsigned long channel_1 = 0x20DFDB8C;
const unsigned long channel_2 = 0x20DFDB4C;

IRsend irsend;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    char msg = Serial.read();
    if (msg == '1') {
      Serial.println(1);
//      digitalWrite(outPin, HIGH);
      irsend.sendNEC(channel_1, 32);
      delay(1000);
    } else if (msg == '2') {
      Serial.println(2);
//      digitalWrite(outPin, LOW);
      irsend.sendNEC(channel_2, 32);
      delay(1000);
    } else {
//      Serial.println(0);
    }
  }
}
