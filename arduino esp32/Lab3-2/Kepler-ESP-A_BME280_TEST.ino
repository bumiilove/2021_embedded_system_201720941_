#include <SparkFunBME280.h>
#define BME280_ADDR 0x77

BME280 myBME280;

void setup()
{
  Serial.begin(115200);
  myBME280.setI2CAddress(BME280_ADDR);
  myBME280.setReferencePressure(101170);  //해수면 기압 [Pa(파스칼)] *2019/07/17 서울기준
  myBME280.beginI2C();
}

void loop()
{
  Serial.println("#BME280 Sensor Data#");
  
  //섭씨온도(℃)
  Serial.print(" Temperature(℃): ");
  Serial.print(myBME280.readTempC(), 2);
  Serial.println(" degrees C");

  //화씨온도(℉)
  Serial.print(" Temperature(℉): ");
  Serial.print(myBME280.readTempF(), 2);
  Serial.println(" degrees F");

  //상대습도%)
  Serial.print(" Humidity: ");
  Serial.print(myBME280.readFloatHumidity(), 2);
  Serial.println(" %");

   //기압(Pa)  1(atm)->1013.25(hPa)->101325(Pa)
  Serial.print(" Pressure: ");
  Serial.print(myBME280.readFloatPressure(), 2);
  Serial.println(" Pa");

   //고도(m)
  Serial.print(" Altitude: ");
  Serial.print(myBME280.readFloatAltitudeMeters(), 2);
  Serial.println("m");

  Serial.println();
  delay(2000);
}
