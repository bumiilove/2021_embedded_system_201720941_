#define _A0_LIGHT_ADC 36
int pinNum=36;
void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.print("Light Sensor Value = ");
  delay(100);

  Serial.println(analogRead(pinNum));
}
