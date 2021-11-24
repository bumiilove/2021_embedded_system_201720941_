#define _TOUCH_LEFT 27
#define _TOUCH_RIGHT 4

#define _LED_R_GPIO_12 12
#define _LED_B_GPIO_14 14

void setup() {
  Serial.begin(115200); //Baud Rate 115200 Bps로 설정
  pinMode(_LED_R_GPIO_12, OUTPUT);
  pinMode(_LED_B_GPIO_14, OUTPUT);
  digitalWrite(_LED_R_GPIO_12, LOW);
  digitalWrite(_LED_B_GPIO_14, LOW);
}

void loop() {
  //ppt p.6과 같이 Touch Sensor의 Left/Right에 대한 전하량 변화값 출력
  Serial.print("About Left touch = ");
  Serial.print(touchRead(_TOUCH_LEFT)); // Capacitor에 의한 전하량 Read
  Serial.print(" | ");
  Serial.print("About Right Touch = ");
  Serial.println(touchRead(_TOUCH_RIGHT));  
  delay(500);

  if ( ( touchRead(_TOUCH_RIGHT)<15 ) && ( touchRead(_TOUCH_LEFT)!=13 )) {
    digitalWrite(_LED_R_GPIO_12, HIGH);
    digitalWrite(_LED_B_GPIO_14, HIGH);
  }

  if ( ( touchRead(_TOUCH_RIGHT)<15 ) && ( touchRead(_TOUCH_LEFT)==13 )) { 
    digitalWrite(_LED_R_GPIO_12, LOW);
    digitalWrite(_LED_B_GPIO_14, HIGH);  
  }
 
 if ( ( touchRead(_TOUCH_LEFT)!=13 ) && ( touchRead(_TOUCH_RIGHT)>15 )) {
    digitalWrite(_LED_B_GPIO_14, LOW);
    digitalWrite(_LED_R_GPIO_12, HIGH);
 }
 
 if ( ( touchRead(_TOUCH_RIGHT)>15 ) && ( touchRead(_TOUCH_LEFT)==13 ) )
 {
    digitalWrite(_LED_R_GPIO_12, LOW);
    digitalWrite(_LED_B_GPIO_14, LOW);
 } 
}
