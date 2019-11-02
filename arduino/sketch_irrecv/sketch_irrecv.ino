/*
  受信テスト用
*/

#include <IRremote.h>

const unsigned int channel_1 = 0x20DFDB8C;
const unsigned int channel_2 = 0x20DFDB4C;

const int outPin = 2;
const int recvPin = 11;
IRrecv irrecv(recvPin);

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(outPin, OUTPUT);
}

void printNumber(decode_results *results) {
  unsigned long value = results->value;
  Serial.println(value, HEX);
  Serial.println(value, BIN);
}

void light(decode_results *results) {
  if (results->value == channel_1) {
    digitalWrite(outPin, HIGH);
  }
  if (results->value == channel_2) {
    digitalWrite(outPin, LOW);
  }
}

void loop() {
  decode_results  results;

  if (irrecv.decode(&results)) {
    printNumber(&results);
    light(&results);
    irrecv.resume();
  }
}
