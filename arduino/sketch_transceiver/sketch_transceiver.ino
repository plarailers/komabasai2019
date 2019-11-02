/*
  母艦に接続された Arduino のプログラム。
  32ビットの赤外線通信の受け渡しを行う。
*/

// https://github.com/z3t0/Arduino-IRremote

#include <IRremote.h>

using u8 = byte;
using u32 = unsigned long;

const int pinSend = 9;  // Arduino -> Devices
const int pinRecv = 11; // Arduino <- Devices

IRsend irsend;  // PIN 9
IRrecv irrecv(pinRecv);

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
}

void loop() {
  decode_results *results;
  if (irrecv.decode(results)) {
    u32 msg = results->value;
    Serial.write( msg >> 24 );
    Serial.write((msg >> 16) & 0xFF);
    Serial.write((msg >> 8)  & 0xFF);
    Serial.write( msg        & 0xFF);
    irrecv.resume();
  }
  if (Serial.available() >= 4) {
    u32 a = Serial.read();
    u32 b = Serial.read();
    u32 c = Serial.read();
    u32 d = Serial.read();
    u32 msg = (a << 24) | (b << 16) | (c << 8) | d;
    irsend.sendNEC(msg, 32);
    irrecv.enableIRIn();
    delay(500);
  }
}
