#define _BLINK_LED_GPIO_15 15
//GPIO15에 해당하는 Blink LED장치 Define 매크로 함수 정의


void setup() { //프로그램이 시작할 때 한번만 실행되는 함수, 초기설정
  pinMode(_BLINK_LED_GPIO_15, OUTPUT);  //입출력장치 핀설정 및 입출력 모드 설정
}


void loop() { //프로그램이 반복적으로 실행되는 함수
  digitalWrite(_BLINK_LED_GPIO_15, HIGH); //GPIO15 핀 HIGH
  delay(1000); //시간 딜레이 함수(ms)
  digitalWrite(_BLINK_LED_GPIO_15, LOW); //GPIO15 핀 LOW
  delay(1000);
}
