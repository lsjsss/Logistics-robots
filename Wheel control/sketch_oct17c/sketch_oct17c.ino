#define PWMB_IN1 11       //定义IN1引轋
#define PWMB_IN2  6       //定义IN2引轋
#define PWMA_IN1  5        //定义IN3引轋
#define PWMA_IN2  3  
void setup() {
  // put your setup code here, to run once:
      Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
        analogWrite(PWMA_IN2, 100);
        analogWrite(PWMB_IN2, 100);
        delay(5000);
        analogWrite(PWMA_IN2, 0);
        analogWrite(PWMB_IN2, 0);
        delay(5000);
}
