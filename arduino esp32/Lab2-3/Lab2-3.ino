
#include <Adafruit_GFX.h> 
#include <Adafruit_IS31FL3731.h>
// LED Matrix 드라이버 제어를 위한 라이브러리
// source : Arduino\hardware\espressif\esp32\libraries\Adafruit_IS31FL3731-master

Adafruit_IS31FL3731 matrix = Adafruit_IS31FL3731();
const char name[  3  ]={'C','m','j'}; // 출력을 위한 문자 배열 선언

static const uint8_t PROGMEM bmp[]={};

void setup() {
  matrix.begin(); // I2C주소값 선언, begin(uint8_t addr = ISSI_ADDR_DEFAULT); 
                // #define ISSI_ADDR_DEFAULT 0x74  
  matrix.fillScreen(100);               
}

void loop() {
  
  matrix.setTextSize(  1 ); // Text 크기 
  matrix.setTextColor(  0, 100  ); // Text 밝기

   /*
    matrix.setTextSize(s)
     - text 크기 설정
     - (s > 0) ? s : 1
    matrix.setTextColor(c)
     - 글자 밝기 조절
     - 0~100
     - background: 투명
    matrix.setTextColor(c, b)
     - 글자와 background 각각 밝기 조절
    */
    for(int i=0;i<3;i++){ // text[ ] 배열에 대한 반복문 선언
    matrix.clear(); // LED Matrix 화면 초기화)
    matrix.fillScreen(100);
    matrix.setCursor(1,1); // LED Matrix 출력의 좌표값(x, y)
    matrix.printf("%c",name[i]); // 문자 배열값 출력(%c=문자1개)
    delay(800);
     }
    matrix.clear();
    delay(2000);
  
    //1. 조, 조원 이니셜 출력
    //2. 배경과 글자 반전
    //matrix.printf함수와 matrix.drawBitmap함수 사용 (가능하다면 둘 중 하나만 사용해도 무관)




}
