#include <SoftwareSerial.h>

SoftwareSerial serial1(10, 11);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  serial1.begin(9600);
  serial1.listen();
}

void loop() {
  if (Serial1.available() > 0) {
    byte data = Serial1.read();
    Serial.print("Serial 1: ");
    Serial.print(data, HEX);
    Serial.print("\n");
  }
  if (serial1.available() > 0) {
    byte data = serial1.read();
    Serial.print("Software Serial 1: ");
    Serial.print(data, HEX);
    Serial.print("\n");
  }
}
