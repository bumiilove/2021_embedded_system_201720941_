/*
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleWrite.cpp
    Ported to Arduino ESP32 by Evandro Copercini
*/

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID "91bad492-b950-4226-aa2b-4ede9fa42f59"      
#define CHARACTERISTIC_UUID "0d563a58-196a-48ce-ace2-dfec78acc814" 
int change=0;

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string value = pCharacteristic->getValue();

      if (value.length() > 0) {
        Serial.println("*********");
        Serial.print("New value: ");
        Serial.println(change);
        for (int i = 0; i < value.length(); i++)
        {            
          Serial.print(value[i]);
          if (value[0]=='a')
          { 
            if (value[1]=='c')  {
              change++;
              if ((change%4)==1){
                digitalWrite(15, HIGH);
              }
              else if ((change%4)==0) {
                digitalWrite(15, LOW);
              }
            } 
          }
        }
        Serial.println();
        Serial.println("*********");
      }
    }
};

void setup() {
  Serial.begin(115200);
  pinMode(15, OUTPUT); 
  
  Serial.println("1- Download and install an BLE scanner app in your phone");
  Serial.println("2- Scan for BLE devices in the app");
  Serial.println("3- Connect to MyESP32");
  Serial.println("4- Go to CUSTOM CHARACTERISTIC in CUSTOM SERVICE and write something");
  Serial.println("5- See the magic =)");

  BLEDevice::init("MyESP32_6");
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
