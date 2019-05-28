int const led = 13;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);

  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
}

void loop() {
  if (Serial.available() != 0) {
    String msg = Serial.readString();
    Serial.readString(); // limpa

    int sel = msg.toInt();

    switch (sel) {
      case 1:
        digitalWrite(led, 1);
        break;
      case 2:
        digitalWrite(led, 0);
        break;
      case 3:
        for (int i = 0; i < 3 ; i++) {
          digitalWrite(led, 1);
          delay(200);
          digitalWrite(led, 0);
          delay(300);
        }
        break;
    }

  }

}
