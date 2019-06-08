// conexiones para stm
//A9 >> DI (TX)
//A10 >> RO (RX)
//Re >> De >> GND Receptor
// Usar 5v no MAX485
#define led PC13

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
  Serial1.begin(9600);
  Serial1.setTimeout(100);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
}

void loop() {
  if(Serial1.available() != 0){
    String rec = Serial1.readStringUntil('<');
    Serial.readString();
    Serial.print("Llego: ");
    Serial.println(rec);
    blink_led();
   }
}

void blink_led(){
  digitalWrite(led, 0);
  delay(1000);
  digitalWrite(led, 1);
  delay(1000);
}
