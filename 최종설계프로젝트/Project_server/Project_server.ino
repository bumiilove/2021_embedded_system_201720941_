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

//Battery capacity print
static const uint8_t PROGMEM bmp_72[]=
{ B00000000,B11100110,B10101001,B10100001,B00100110,B00101000,B00101111,B00000000 };

static const uint8_t PROGMEM bmp_percent[]=
{ B00000000,B01100010,B01100100,B00001000,B00010000,B00100110,B01000110,B00000000 };

static const uint8_t PROGMEM bmp_R[]=
{ B01111100,B01000100,B01000100,B01111100,B01100000,B01010000,B01001000,B01000100 };
//

//park in parking zone
static const uint8_t PROGMEM bmp_P[]=
{ B01111110,B01000010,B01000010,B01111110,B01000000,B01000000,B01000000,B01000000 };

static const uint8_t PROGMEM bmp_5[]=
{ B01111110,B01000000,B01000000,B01111100,B00000010,B00000010,B01000100,B00111000 };//current fee

static const uint8_t PROGMEM bmp_minus[]=
{ B00000000,B00000000,B00000000,B01111110,B00000000,B00000000,B00000000,B00000000 };

static const uint8_t PROGMEM bmp_3[]=
{ B00011100,B00100010,B00000010,B00011100,B00000010,B00100010,B00011100,B00000000 };//discount

static const uint8_t PROGMEM bmp_OFF[]=
{ B00000000,B01001111,B10101010,B10101111,B10101010,B10101010,B01001010,B00000000 };
//



//BLE server : Data receive part
class MyCallbacks: public BLECharacteristicCallbacks {   //RX
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();
      
      if (value.length() > 0 ) {
        //Serial.println("*********");
        //Serial.print("New value: ");
        for (int i = 0; i < value.length(); i++)
        {            
          //Serial.print(value[i]);
          if (value[0]=='B')
          { 
            if (value[1]=='A')
            {
              if(norepeat_R==0)
              {
                norepeat_R++;
                matrix.setRotation(0);
                matrix.clear();
                matrix.drawBitmap(0,0,bmp_R,8,8,128); 
              }
            }
          }
          else if (value[0]=='P')
          {
            if (value[1]=='A')
            {
              if(norepeat_P==0)
              {
                norepeat_P++;
                matrix.setRotation(0);
                matrix.clear();
                matrix.drawBitmap(0,0,bmp_P,8,8,128);
              } 
            }
          }
        }
        //Serial.println();
        //Serial.println("*********");
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
  
  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");

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
}

void loop() {
    // 헬멧 감지 여부를 시리얼 수신: 감지=1 미감지=0 횟수 초과 =-1
    rx_data = Serial.readStringUntil('\n');
  
    // 헬멧 미착용시: 매트릭스에 S를 띄우고 상태는 유지.
    if((state == 0 || state == 2) && rx_data =="0"){
                state = 2;
                //S 만 출력
    }
    // 헬멧 착용시 : 매트릭스에 R을 띄우고 시리얼 송신: 1 (킥보드 사용시작 : state =>1)
    else if ((state == 0 || state == 2) && rx_data == "1"){
                state = 1;
                matrix.setRotation(0); 
                matrix.clear();
                matrix.drawBitmap(0,0,bmp_R,8,8,128); 
                
                tx_data = 1;
                Serial.println(tx_data);
                delay(100);
    }
    // 횟수 초과시: state를 4로 바꾸는 동시에 finish도 1로 바꿈, tx_data는 4로 바꿔 사용할 수 없음을 사운드로 알림 
    else if(state == 2 && rx_data == "-1"){
                tx_data = 4;
                state = 4;
                finish = 1;
                Serial.println(tx_data);
    }
    // 종료시    : state를 4로 바꿔줌
    else if(state == 1&& rx_data == "4"){
      state = 4;
    }
    // 사용자가 직접 킥보드를 종료한 경우 (finish =0 -> 1)
    else if(state == 4 && finish ==0){
      // 주차를 한 경우 cost를 500원 절감
      if(park == 1){
        cost -= 500;
        }
      Serial.println(cost);
      finish = 1;
      state = 0;
      matrix.setRotation(0);
      matrix.clear();
      matrix.drawBitmap(0,0,bmp_P,8,8,128);
    }

    // 킥보드의 상태가 running일 경우 요금은 계속 100원씩 증가 
    if(state == 1){
      cost += 100;
    }


  delay(2000);
}
