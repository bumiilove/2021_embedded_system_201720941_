#include <SparkFunBME280.h>
#define BME280_ADDR 0x77
BME280 myBME280;
void setup()
{
Serial.begin( 115200);
myBME280.setI2CAddress(BME280_ADDR);
myBME280.beginI2C(); //BME 280I2C 장치 호출
}
void
loop()
{
Serial.println(myBME280.readTempC(), 2);
delay(2000);
}
