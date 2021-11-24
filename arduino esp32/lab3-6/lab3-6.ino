#define _A0_LIGHT_ADC 36
int pinNum=36;
int freq =   2000     ; //PWM 주파수 설정
int channel1 =   0      ; //PWM 채널 설정(0~15)
int resolution =  8     ; //PWM duty cycle resolution 설정(1~16bit)
int index_x=0;
int freq_tone[] =  {261, 293, 329, 349, 391, 440, 493, 523 };

void setup() {
  Serial.begin(115200);
  ledcSetup(  channel1     , freq, resolution     ); //ledcSetup(PWM 채널, 주파수, 해상도)
  ledcAttachPin(   25    ,   channel1    ); //ledcAttachPin(GPIO핀, 채널 설정)
  pinMode(   2    ,  OUTPUT     ); //pinMode(GPIO핀, mode), mode: INPUT/OUTPUT
  digitalWrite(   2    ,  HIGH     ); //digitalWrite(GPIO핀, value) value: HIGH/LOW 
}

void loop() {
  Serial.print("Light Sensor Value = ");
  delay(100);
  index_x = ((analogRead(pinNum))/512);
  
    ledcWriteTone(  channel1      , freq_tone[index_x]      ); //ledcWriteTone(PWM Channel, frequenc
    
}
  
