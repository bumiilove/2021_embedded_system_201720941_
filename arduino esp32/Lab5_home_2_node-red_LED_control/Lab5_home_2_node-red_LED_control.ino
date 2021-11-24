#include <SparkFunBME280.h>
#define blink_led 15 // SET blink_led pin num
String msg;
int toggle =0;

void setup()
{
pinMode(blink_led, OUTPUT); // set the LED pin mode
Serial.begin(115200); // set the baudrate of Serial Communication
}

void loop()
{
  msg = Serial.readStringUntil('\n');
  //msg = Serial.read();
  if (toggle ==0){
    if(msg=="1")
       toggle = 1;
    delay(10);      
  }
  else if (toggle == 1){
    if(msg=="0")
       toggle = 0;
    delay(10);  
  } 
  if(toggle == 1)
    digitalWrite(blink_led,HIGH);
  else
    digitalWrite(blink_led,LOW);
}
