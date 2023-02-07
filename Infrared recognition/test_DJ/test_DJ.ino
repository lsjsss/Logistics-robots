#define PS2_DAT      8
#define PS2_CMD      9
#define PS2_ATT      2
#define PS2_CLK      4
int red = 7;
#define PWMB_IN1 11       //定义IN1引轋
#define PWMB_IN2  6       //定义IN2引轋
#define PWMA_IN1  5        //定义IN3引轋
#define PWMA_IN2  3  
void setup() {
  // put your setup code here, to run once:
     Serial.begin(9600);
     pinMode(red,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(red));
  if(digitalRead(red)==LOW){
     Motor_Forward(0);
  }else{
     Motor_Forward(100);
  }
    
}
void Motor_Forward(char pwm)
{

      analogWrite(PWMA_IN2, pwm);
      analogWrite(PWMB_IN2, pwm);

}

void Motor_Back(char motor, char pwm)
{
  if (motor == 1)
  {
    analogWrite(PWMA_IN1, 255);
    analogWrite(PWMA_IN2, pwm);
  }
  else if (motor == 2)
  {
    analogWrite(PWMB_IN1, 255);
    analogWrite(PWMB_IN2, pwm);
  }
}

void Motor_Right(char motor, char pwm)
{
  if (motor == 1)
  {
    analogWrite(PWMA_IN1, pwm);
    analogWrite(PWMA_IN2, 255);
  }
  else if (motor == 2)
  {
    analogWrite(PWMB_IN1, 255);
    analogWrite(PWMB_IN2, pwm);
  }
}

void Motor_Left(char motor, char pwm)
{
  if (motor == 1)
  {
    analogWrite(PWMA_IN1, 255);
    analogWrite(PWMA_IN2, pwm);
  }
  else if (motor == 2)
  {
    analogWrite(PWMB_IN1, pwm);
    analogWrite(PWMB_IN2, 255);
  }
}
void Motor_Stop(char motor)
{
  if (motor == 1)
  {
    //digitalWrite(PWMA_IN1,LOW );
    //digitalWrite(PWMA_IN2,LOW );
    analogWrite(PWMA_IN1, 255);
    analogWrite(PWMA_IN2, 255);
  }
  else if (motor == 2)
  {
    //digitalWrite(PWMB_IN1,LOW );
    //digitalWrite(PWMB_IN2,LOW );
    analogWrite(PWMB_IN1, 255);
    analogWrite(PWMB_IN2, 255);
  }
}

