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

int state = 0; // 0:ON  1:Running  2:Stop  3:OFF  4:Park 
int norepeat_R=0;
//int norepeat_P=0;


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
static const uint8_t PROGMEM bmp_battery[]=
{ B00011000,B01111110,B01000010,B01000010,B01111110,B01111110,B01111110,B01111110 };//battery capacity
//

//park in parking zone
static const uint8_t PROGMEM bmp_P[]=
{ B01111110,B01000010,B01000010,B01111110,B01000000,B01000000,B01000000,B01000000 };

static const uint8_t PROGMEM bmp_D[]=
{ B01111000,B01000100,B01000010,B01000010,B01000010,B01000010,B01000100,B01111000 };//discount
//



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
          if (value[0]=='A')
          { 
            if (value[1]=='A')
            {
              if(norepeat_R==0)
              {
                state=0;
                norepeat_R++;
                matrix.setRotation(0);
                matrix.clear();
                matrix.drawBitmap(0,0,bmp_ON,8,8,128); 
              }
            }
          }
          else if (value[0]=='B' && state<3 )
          {
            if (value[1]=='A')
            {
              matrix.setRotation(0);
              matrix.clear();
              matrix.drawBitmap(0,0,bmp_battery,8,8,128);
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
          else if (value[0]=='P')
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
  // put your main code here, to run repeatedly:
  delay(2000);
}
