
#include <Adafruit_GFX.h> 
#include <Adafruit_IS31FL3731.h>
// LED Matrix 드라이버 제어를 위한 라이브러리
// source : Arduino\hardware\espressif\esp32\libraries\Adafruit_IS31FL3731-master

Adafruit_IS31FL3731 matrix = Adafruit_IS31FL3731();

static const uint8_t PROGMEM bmp_C[]=
{ B00011100,B00100010,B01000001,B10000000,B10000000,B01000001,B00100010,B00011100 };

static const uint8_t PROGMEM bmp_m[]=
{ B00000000,B01100110,B10011001,B10011001,B10011001,B10011001,B10000001,B10000001 };

static const uint8_t PROGMEM bmp_j[]=
{ B00001000,B00000000,B00001000,B00001000,B00001000,B10001000,B01001000,B00110000 };

static const uint8_t PROGMEM bmp_K[]=
{ B11000110,B11001100,B11011000,B11110000,B11110000,B11011000,B11001100,B11000110 };

static const uint8_t PROGMEM bmp_k[]=
{B11000000,B11000000,B11000110,B11011000,B11100000,B11100000,B11011000,B11000110 };

static const uint8_t PROGMEM bmp_b[]=
{ B11000000,B11000000,B11001100,B11010010,B11100001,B11100001,B11110010,B11001100 };

void setup() {
  matrix.begin(); // I2C주소값 선언, begin(uint8_t addr = ISSI_ADDR_DEFAULT); 
                // #define ISSI_ADDR_DEFAULT 0x74  
}

void loop() {
  for(int i=8;i>-28;i--){
    matrix.setRotation(0);
    matrix.clear();
    matrix.drawBitmap(i,0,bmp_C,8,8,128);
    matrix.drawBitmap(i+10,0,bmp_m,8,8,128);
    matrix.drawBitmap(i+20,0,bmp_j,8,8,128);
    delay(100); 
  }
  for(int j=8;j>-28;j--){
    matrix.setRotation(0);
    matrix.clear();
    matrix.drawBitmap(j,0,bmp_K,8,8,128);
    matrix.drawBitmap(j+10,0,bmp_k,8,8,128);
    matrix.drawBitmap(j+20,0,bmp_b,8,8,128);
    delay(100); 
  } 
}
