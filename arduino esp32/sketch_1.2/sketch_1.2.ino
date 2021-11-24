#define _LED_R_GPIO_12 12
#define _LED_G_GPIO_13 13
#define _LED_B_GPIO_14 14

int freq = 500; // PWM 주파수 설정 500Hz
int ledChannel = 0; // PWM 채널 설정(0~15)
// RED=0, GREEN=1, BLUE=2
int resolution = 8; // PWM duty cycle resolution(해상도) 설정
// 8bit=0~255, 16bit=0~65535


void setup() {
  Serial.begin(115200);
  pinMode(_LED_R_GPIO_12, OUTPUT);
  pinMode(_LED_G_GPIO_13, OUTPUT);
  pinMode(_LED_B_GPIO_14, OUTPUT);

 /* ledcSetup(ledChannel, freq, resolution); // PWM채널, 주파수, 해상도 설정
  ledcAttachPin(_LED_R_GPIO_12, ledChannel); // GPIO Pin, 채널설정
  ledcSetup(ledChannel+1, freq, resolution);
  ledcAttachPin(_LED_G_GPIO_13, ledChannel+1);  
  ledcSetup(ledChannel+2, freq, resolution);
  ledcAttachPin(_LED_B_GPIO_14, ledChannel+2);
*/}

void loop() {
  digitalWrite(_LED_R_GPIO_12, HIGH);
  delay(1000);
  digitalWrite(_LED_R_GPIO_12, LOW);
  digitalWrite(_LED_G_GPIO_13, HIGH);
  delay(1000);
  digitalWrite(_LED_G_GPIO_13, LOW);
  digitalWrite(_LED_B_GPIO_14, HIGH);
  delay(1000);
  digitalWrite(_LED_B_GPIO_14, LOW);

  digitalWrite(_LED_R_GPIO_12, HIGH);
  digitalWrite(_LED_G_GPIO_13, HIGH);
  digitalWrite(_LED_B_GPIO_14, HIGH);
  delay(1000);

  digitalWrite(_LED_R_GPIO_12, LOW);
  digitalWrite(_LED_G_GPIO_13, LOW);
  digitalWrite(_LED_B_GPIO_14, LOW);
}
