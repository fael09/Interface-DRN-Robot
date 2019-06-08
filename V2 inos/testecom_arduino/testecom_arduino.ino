// conexiones para arduino
//D4 >> DI (TX)
//D5 >> RO (RX)
// ver si funciona invertido D5 TX Y D4 RX
// invertir senhal de val x <> val y
//Re >> De >> VCC Transmisor
// Usar 5v no MAX485
#include <SoftwareSerial.h>

#define led 13

int ctrl = 1;

SoftwareSerial rs485 = SoftwareSerial(5, 4);

void setup() {
  rs485.begin(9600);
  rs485.setTimeout(100);
  Serial.begin(9600);
  Serial.setTimeout(100);
  pinMode(led, OUTPUT);

  digitalWrite(led, 0);
}

void loop() {
  if (Serial.available() != 0) {
    String rec = Serial.readStringUntil('<');
    Serial.readString();
    Serial.print("Enviando para el receptor: ");
    Serial.println(rec);
    rec = rec + "<";
    rs485.print(rec);
    blink_led();
    ctrl = 0;
  }
}

void blink_led() {
  digitalWrite(led, 1);
  delay(1000);
  digitalWrite(led, 0);
  delay(1000);
}
