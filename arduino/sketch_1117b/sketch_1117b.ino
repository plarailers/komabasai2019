void setup() {
  Serial1.begin(9600);
}

void loop() {
  Serial1.write(0xFF);
  delay(1000);
}
