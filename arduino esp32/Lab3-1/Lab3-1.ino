#define MIC 36//ADC1_CH0 pin number
int pinNum=3;

void setup() {
  Serial.begin(15200);
}

void loop() {
    Serial.println(analogRead(pinNum); //analogRead(pin number)함수를 이용해 입력센서값 Read
    delay(2);
}
