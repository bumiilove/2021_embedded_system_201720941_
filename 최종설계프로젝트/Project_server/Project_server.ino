 /*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/
//BLE
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"      
#define CHARACTERISTIC_UUID "0d563a58-196a-48ce-ace2-dfec78acc814" 
//

/* 전역변수 설정 */
int norepeat_B=0;
// 경범 추가
String rx_data ;// 헬멧 착용 감지상태를 입력받는 변수(시리얼 수신)1: 감지,0:미감지,-1:횟수 오버->4로 전달 // 날씨 데이터 7: 맑음/ 8: 맑지 않은 경우
int tx_data;    // 시리얼 송신으로 정보를 전달하는 변수 (시리얼 송신)1: 헬멧 감지 후 사용 시작알림, 8: 인식 횟수 초과로 사용불가 알림
int state = 0;  // 킥보드의 동작 상태를 표현하는 변수 0: /on 1:running/ stop: 2/ park: 3/ off: 4 
int cost = 0;   // 요금 변수 
int battery = 100;// 잔여 배터리양 
int battery_print = 0;
int finish =0;  // 킥보드의 사용이 종료됐음을 표현하는 변수 0: 동작중 1: 사용 종료
int park =0 ;   // 주차 플래그 0: 주차spot에 주차하지 않음, 1: 주차spot에 주차함 
// LED Matrix 드라이버 제어를 위한 라이브러리
#include <Adafruit_GFX.h> 
#include <Adafruit_IS31FL3731.h>

Adafruit_IS31FL3731 matrix = Adafruit_IS31FL3731();
//////////////////////////////////////

//State Display
static const uint8_t PROGMEM bmp_ON[]=
{ B00000000,B01001001,B10101101,B10101101,B10101011,B10101011,B01001001,B00000000 };

static const uint8_t PROGMEM bmp_OFF[]=
{ B00000000,B01001111,B10101010,B10101111,B10101010,B10101010,B01001010,B00000000 };

static const uint8_t PROGMEM bmp_R[]=
{ B01111100,B01000100,B01000100,B01111100,B01100000,B01010000,B01001000,B01000100 };

static const uint8_t PROGMEM bmp_S[]=
{ B00111100,B01000010,B01000010,B01100000,B00011100,B00000010,B01000010,B00111100 };
//

//Battery capacity print
static const uint8_t PROGMEM bmp_B[]=
{ B00011000,B01111110,B01000010,B01000010,B01111110,B01111110,B01111110,B01111110 };//battery capacity
//

//park in parking zone
static const uint8_t PROGMEM bmp_P[]=
{ B01111110,B01000010,B01000010,B01111110,B01000000,B01000000,B01000000,B01000000 };

static const uint8_t PROGMEM bmp_D[]=
{ B01111000,B01000100,B01000010,B01000010,B01000010,B01000010,B01000100,B01111000 };//discount
//
static const uint8_t PROGMEM bmp_badWeather[]=
{ B00000000,B01100110,B01100110,B00000000,B00011000,B00100100,B01000010,B00000000 };

static const uint8_t PROGMEM bmp_goodWeather[]=
{ B00000000,B01100110,B01100110,B00000000,B10000001,B01000010,B00111100,B00000000 };

static const uint8_t PROGMEM bmp_speedDec[]=
{ B00011000,B00011000,B00011000,B10011001,B01011010,B00111100,B00011000,B00000000 };//아래방향 화살표

static const uint8_t PROGMEM bmp_B_full[]=
{ B00011000,B01111110,B01111110,B01111110,B01111110,B01111110,B01111110,B01111110 };//battery capacity

static const uint8_t PROGMEM bmp_B_80[]=
{ B00011000,B01111110,B01000010,B01000010,B01111110,B01111110,B01111110,B01111110 };//battery capacity

static const uint8_t PROGMEM bmp_B_40[]=
{ B00011000,B01111110,B01000010,B01000010,B01000010,B01000010,B01111110,B01111110 };//battery capacity

static const uint8_t PROGMEM bmp_B_15[]=
{ B00011000,B01111110,B01000010,B01000010,B01000010,B01000010,B01000010,B01111110 };//battery capacity


// 킥보드 상태 초기화 함수
void pm_init(){
  
  state = 0;  // 킥보드의 동작 상태를 표현하는 변수 0: /on 1:running/ stop: 2/ off: 4 
  cost = 0;   // 요금 변수 
  battery = 100;
  battery_print = 0;
  finish = 0;  // 킥보드의 사용이 종료됐음을 표현하는 변수 0: 동작중 1: 사용 종료
  park = 0;   // 주차 플래그 0: 주차spot에 주차하지 않음, 1: 주차spot에 주차함 
  matrix.setRotation(0);
  matrix.clear();
  delay(500);
  return;
}

// 킥보드 동작 함수
void pm_on(){
// 송신값에 따른 동작부
  if((state == 0 || state == 2) &&rx_data =="0")
    state =2;

  else if (battery <10 && norepeat_B <=1 ) {
    Serial.println(6);
    norepeat_B++; 
  }
  // 사용자가 배터리 정보를 확인하려 할때
  else if((state==0 || state == 1 || state == 2) && battery_print==1) {
    matrix.setRotation(0);
    matrix.clear();
    if(battery <=100 && battery >= 70)
            matrix.drawBitmap(0,0,bmp_B_full,8,8,128);
    else if(battery < 70 && battery >=40)
            matrix.drawBitmap(0,0,bmp_B_80,8,8,128);
    else if(battery < 40 && battery >=20)
            matrix.drawBitmap(0,0,bmp_B_40,8,8,128);
    else if(battery < 20 && battery >=0 ){
            matrix.drawBitmap(0,0,bmp_B_15,8,8,128);
      }
    delay(2500);
    battery_print = 0;
  }
  else if((state == 0 || state == 2) && rx_data == "1"){
                state = 1;
                finish =0;                
                tx_data = 1;
                Serial.println(tx_data);
  }
  else if(state == 2 && rx_data == "-1"){
                tx_data = 4;
                state = 4;
                finish = 1;
                Serial.println(tx_data);
      }
  else if((state == 1 || state == 3) && rx_data == "4"){
       // 주차 스팟 외부에서 주차하여, 디스카운트 없이 종료될때
          if(state ==1){
          }
        // 주차 스팟에서 주차하여 디스카운트와 함께 종료될때   
          else if(state == 3){
            Serial.println(state);
            cost -= 500;
            matrix.setRotation(0);
            matrix.clear();
            matrix.drawBitmap(0,0,bmp_P,8,8,128);
            delay(3000);
           }          
  Serial.println(cost); 
  state =4; 
  }
  else if(state ==2 && rx_data == "4"){
  Serial.println(0);     
  state =4; 
}    
  //날씨
  else if(rx_data == "7"){  // 맑음
          matrix.setRotation(0);
          matrix.clear();
            matrix.drawBitmap(0,0,bmp_goodWeather,8,8,128);
            delay(3000);
    }
  else if(rx_data == "8"){  // 맑지 않음
          matrix.setRotation(0);
          matrix.clear();
            matrix.drawBitmap(0,0,bmp_badWeather,8,8,128);
            delay(3000);
    }
// 상태값에 따른 출력부
  if(state == 0){
            
  matrix.setRotation(0);
  matrix.clear();
            matrix.drawBitmap(0,0,bmp_ON,8,8,128);
  }
  else if(state ==1){
   
  matrix.setRotation(0);
  matrix.clear();
        matrix.drawBitmap(0,0,bmp_R,8,8,128);
  }
  else if(state ==2){
  
  matrix.setRotation(0);
  matrix.clear();
                  matrix.drawBitmap(0,0,bmp_S,8,8,128);
  }
  else if(state == 4){
  matrix.setRotation(0);
  matrix.clear();
            matrix.drawBitmap(0,0,bmp_OFF,8,8,128);
  }      
  return;
  }


//BLE server : Data receive part
class MyCallbacks: public BLECharacteristicCallbacks {   //RX
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        if(value == "BATTERY"){
            battery_print=1;
          }
         else if(value == "PARK"){
           if(state ==1){
              state =3;
           }
         } 
      }
    }
};
////////////////////////////////////////////


void setup() {
//matrix part
  matrix.begin(); 
//  

  Serial.begin(115200);
  pinMode(15, OUTPUT); 
  
  BLEDevice::init("MyESP32");
  BLEServer *pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                         CHARACTERISTIC_UUID,
                                         BLECharacteristic::PROPERTY_READ |
                                         BLECharacteristic::PROPERTY_WRITE
                                       );

  pCharacteristic->setCallbacks(new MyCallbacks());

  pCharacteristic->setValue("Hello World");
  pService->start();
  
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();

  matrix.setRotation(0);
  matrix.clear();
  matrix.drawBitmap(0,0,bmp_ON,8,8,128);
}

void loop() {    
// 헬멧 감지 여부를 시리얼 수신: 감지=1 미감지=0 횟수 초과 =-1
    rx_data = Serial.readStringUntil('\n');
    if(rx_data =="9"){
      pm_init();  
    }
    else{
      pm_on();
    }
// 킥보드의 상태가 running일 경우 요금은 계속 100원씩 증가 
    if(state == 1){
      cost += 100;
      battery -= 5;
      if(battery <= 0)
        battery = 0;
    }
    delay(2000);
  //Serial.println(state);
}
