//http://robojax.com/learn/arduino/8x8LED/

#include <Adafruit_GFX.h> 
#include <Adafruit_IS31FL3731.h>
// LED Matrix 드라이버 제어를 위한 라이브러리
// source : Arduino\hardware\espressif\esp32\libraries\Adafruit_IS31FL3731-master

Adafruit_IS31FL3731 matrix = Adafruit_IS31FL3731();

static const uint8_t PROGMEM bmp_ON[]=
{ B00000000,B01001001,B10101101,B10101101,B10101011,B10101011,B01001001,B00000000 };

static const uint8_t PROGMEM bmp_OFF[]=
{ B00000000,B01001111,B10101010,B10101111,B10101010,B10101010,B01001010,B00000000 };

static const uint8_t PROGMEM bmp_R[]=
{ B01111100,B01000100,B01000100,B01111100,B01100000,B01010000,B01001000,B01000100 };

static const uint8_t PROGMEM bmp_S[]=
{ B00111100,B01000010,B01000010,B01100000,B00011100,B00000010,B01000010,B00111100 };

static const uint8_t PROGMEM bmp_badWeather[]=
{ B00000000,B01100110,B01100110,B00000000,B00011000,B00100100,B01000010,B00000000 };

static const uint8_t PROGMEM bmp_battery[]=
{ B00011000,B01111110,B01000010,B01000010,B01111110,B01111110,B01111110,B01111110 };//battery capacity

static const uint8_t PROGMEM bmp_money[]=
{ B00000000,B01000010,B01000010,B01011010,B11111111,B01011010,B01011010,B00100100 };//fee display

static const uint8_t PROGMEM bmp_speedDec[]=
{ B00011000,B00011000,B00011000,B10011001,B01011010,B00111100,B00011000,B00000000 };

static const uint8_t PROGMEM bmp_P[]=
{ B01111110,B01000010,B01000010,B01111110,B01000000,B01000000,B01000000,B01000000 };

static const uint8_t PROGMEM bmp_D[]=
{ B01111000,B01000100,B01000010,B01000010,B01000010,B01000010,B01000100,B01111000 };//discount



void setup() {
  matrix.begin(); // I2C주소값 선언, begin(uint8_t addr = ISSI_ADDR_DEFAULT); 
                // #define ISSI_ADDR_DEFAULT 0x74  
}

void loop() {
  matrix.setRotation(0);
  matrix.clear();
  matrix.drawBitmap(0,0,bmp_ON,8,8,128); 
  delay(3000);  
}
