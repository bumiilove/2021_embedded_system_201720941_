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
int norepeat_R=0;
int norepeat_P=0;
// 경범 추가
String rx_data ;// 헬멧 착용 감지상태를 입력받는 변수(시리얼 수신)1: 감지,0:미감지,-1:횟수 오버->4로 전달
int tx_data;    // 시리얼 송신으로 정보를 전달하는 변수 (시리얼 송신)1: 헬멧 감지 후 사용 시작알림, 8: 인식 횟수 초과로 사용불가 알림
int state = 0;  // 킥보드의 동작 상태를 표현하는 변수 0: /on 1:running/ stop: 2/ off: 4 
int cost = 0;   // 요금 변수 
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
{ B00000000,B01100110,B01100110,B00000000,B00011000,B00100100,B01000010,B00000000 };//bad weather

static const uint8_t PROGMEM bmp_money[]=
{ B00000000,B01000010,B01000010,B01011010,B11111111,B01011010,B01011010,B00100100 };//dollar

static const uint8_t PROGMEM bmp_speedDec[]=
{ B00011000,B00011000,B00011000,B10011001,B01011010,B00111100,B00011000,B00000000 };//아래방향 화살표



// 킥보드 상태 초기화 함수
void pm_init(){
  
  state = 0;  // 킥보드의 동작 상태를 표현하는 변수 0: /on 1:running/ stop: 2/ off: 4 
  cost = 0;   // 요금 변수 
  finish = 0;  // 킥보드의 사용이 종료됐음을 표현하는 변수 0: 동작중 1: 사용 종료
  park = 0;   // 주차 플래그 0: 주차spot에 주차하지 않음, 1: 주차spot에 주차함 
  matrix.setRotation(0);
  matrix.clear();
  delay(1500);
  return;
}

// 킥보드 동작 함수
void pm_on(){
  // 헬멧 미착용시: 매트릭스에 S를 띄우고 state = 0->2
    if((state == 0 || state == 2) && rx_data =="0"){
                state = 2;
                matrix.setRotation(0); 
                matrix.clear();
                matrix.drawBitmap(0,0,bmp_S,8,8,128);
                //S 만 출력
    }
// 헬멧 착용시 : 매트릭스에 R을 띄우고 시리얼 송신: 1 (킥보드 사용시작 : state =>1)
    else if ((state == 0 || state == 2) && rx_data == "1"){
                state = 1;
                finish =0;                
                tx_data = 1;
                Serial.println(tx_data);
                
                matrix.setRotation(0); 
                matrix.clear();
                matrix.drawBitmap(0,0,bmp_R,8,8,128); 
    }
// 횟수 초과시: state를 4로 바꾸는 동시에 finish도 1로 바꿈, tx_data는 4로 바꿔 사용할 수 없음을 사운드로 알림 
    else if(state == 2 && rx_data == "-1"){
                tx_data = 4;
                state = 4;
                finish = 1;
                Serial.println(tx_data);
    }
// 종료시    : 사용자가 종료버튼을 눌러 rx_data가 4인 경우 
    else if((state == 1 || state == 3) && rx_data == "4"){
      // 주차 스팟 외부에서 주차하여, 디스카운트 없이 종료될때
      if(state ==1){
        Serial.println(cost);
        //matrix.setRotation(0);
        //matrix.clear();
        //matrix.drawBitmap(0,0,bmp_OFF,8,8,128);
      }
      // 주차 스팟에서 주차하여 디스카운트와 함께 종료될때   
      else if(state == 3){
        Serial.println(state);
        cost -= 500;
        Serial.println(cost);
        matrix.setRotation(0);
        matrix.clear();
        matrix.drawBitmap(0,0,bmp_P,8,8,128);
        //delay(1500);
        //matrix.clear();
        //matrix.drawBitmap(0,0,bmp_OFF,8,8,128);
      }
        state =4; 
    }
    else if(state == 4){
        matrix.setRotation(0);
        matrix.clear();
        matrix.drawBitmap(0,0,bmp_OFF,8,8,128);
      }
    else if(state == 0){
        matrix.setRotation(0);
        matrix.clear();
        matrix.drawBitmap(0,0,bmp_ON,8,8,128);
              
    }
    return;
  }
//BLE server : Data receive part
class MyCallbacks: public BLECharacteristicCallbacks {   //RX
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
        {            
          Serial.print(value[i]);
          if (value[0]=='R' && state==4) // 재시작 : 휴대폰으로 'RESTART'보냄 
          { 
            if (value[1]=='E')
            {
              state=0;
              matrix.setRotation(0);
              matrix.clear();
              matrix.drawBitmap(0,0,bmp_ON,8,8,128); 
            }
          }
          else if (value[0]=='B' && state<3 ) // 킥보드 배터리 양 출력
          {
            if (value[1]=='A')
            {
              matrix.setRotation(0);
              matrix.clear();
              matrix.drawBitmap(0,0,bmp_B,8,8,128);
              delay(2000);
              if (state==0)
              {
                matrix.clear();
                matrix.drawBitmap(0,0,bmp_ON,8,8,128);
              }
              else if (state==1)
              {
                matrix.clear();
                matrix.drawBitmap(0,0,bmp_R,8,8,128);
              }
              else if (state==2)
              {
                matrix.clear();
                matrix.drawBitmap(0,0,bmp_S,8,8,128);
              }
            }
          }
          else if (value[0]=='P')  // 주차 SPOT에 가서 주차함 
          {
            if (value[1]=='A')
            {
              if (state==1)
              {
                state=4;
                matrix.setRotation(0);
                matrix.clear();
                matrix.drawBitmap(0,0,bmp_P,8,8,128);
                delay(2000);
                matrix.clear();
                matrix.drawBitmap(0,0,bmp_D,8,8,128);
                delay(2000);
                matrix.clear();
                matrix.drawBitmap(0,0,bmp_OFF,8,8,128);
              }
            }
          }
        }
        Serial.println();
        Serial.println("*********");
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
    }
  delay(2000);
}
