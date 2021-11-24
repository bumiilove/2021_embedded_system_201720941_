//수정 테스트

#define _D0_PIR_GPIO 2
#define _D1_LED_GPIO 15
int val;

void setup() {
  Serial.begin(115200);
  pinMode( _D0_PIR_GPIO, INPUT); //D0핀 입력설정
  pinMode( _D1_LED_GPIO, OUTPUT); //D1핀 출력설정
  
}

void loop() {
  val = digitalRead(_D0_PIR_GPIO);
  delay(2);
  Serial.print(val);
  delay(2);
  //PIR상태에 따른 LED 출력 변화
  if(val==HIGH){
    digitalWrite(_D1_LED_GPIO, HIGH);
   // delay(2);
  }
  else  {
    digitalWrite(_D1_LED_GPIO, LOW);
    //delay(2);
  }
  
}
