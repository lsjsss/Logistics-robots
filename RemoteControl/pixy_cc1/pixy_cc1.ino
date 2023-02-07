//#include <PS2X_lib.h>            //声明PS2手柄庿
#include <Servo.h>               //声明舵机롍
#include <Pixy2.h>              //声明图像识别传感器
//PS2X ps2x;

#define  BEEP_PIN   12           //定义蜂鸣器引脚K2
#define  NLED_PIN   13           //定义LED灯引脚K3
#define  NLED_MS_BETWEEN        1000
#define SER1_BAUD   115200
#define  DUOJI_MS_BETWEEN       20.000
unsigned char val = 0;
#define DJ_NUM  6                //宏定义伺服舵机的个斊 
int SPEED = 100;  //设置电机默认速度
/*电机驱动引脚*/
#define PWMB_IN1 11       //定义IN1引辋
#define PWMB_IN2  6       //定义IN2引辋
#define PWMA_IN1  5        //定义IN3引辋
#define PWMA_IN2  3        //定义IN4引辋
Pixy2 pixy;//图像识别传感器
boolean startBit  = false;  //协议弿始标忿
Servo myservo[DJ_NUM];           //创建舵机伺服对象数组
unsigned int duoji_index;       //声明舵机变釒（舵机号͍
byte duoji_pin[DJ_NUM] = {10, A2, A3, A0, A1, 7};//定义丿个数组，存储引脚数檳
unsigned int Pwm_Value[6] = {1500, 1500, 1500, 1500, 1500, 1500};


int fm=12;
#define PWMB_IN1 11       //瀹氫箟IN1寮曡剼
#define PWMB_IN2  6       //瀹氫箟IN2寮曡剼
#define PWMA_IN1  5        //瀹氫箟IN3寮曡剼
#define PWMA_IN2  3        //瀹氫箟IN4寮曡剼
void Motor_Forward(char motor, char pwm);
void Motor_Back(char motor, char pwm);
void Motor_Right(char motor, char pwm);
void Motor_Left(char motor, char pwm);
void Motor_Stop(char motor);
int i=1;
void setup() {
    // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.print("Starting...\n");
  
  for (byte i = 0; i < DJ_NUM; i++)
  { //循环使相应的舵机伺服对象连接到对应的引轋
    myservo[i].attach(duoji_pin[i]);
  }
  for (byte i = 0; i < DJ_NUM; i++)
  {
    myservo[i].writeMicroseconds(Pwm_Value[i]);//循环写入初始PWM目标倿
  }
  Serial.begin(SER1_BAUD);
  Serial.println("uart1 init OK");
//  pinMode(BEEP_PIN, OUTPUT);
//  pinMode(NLED_PIN, OUTPUT);

  

  
//  pixy.init();
  
}

void loop() {
  Motor_Forward(1,255);
  Motor_Forward(2,255);  
  // put your main code here, to run repeatedly:
  Serial.println("Hello world");
//  pixy2();
//    Motor_Stop(1);
//    Motor_Stop(2);
  
//  for(int k=0;k<4;k++){
        speed();
//  }
//   }
}

void pixy2(){//pixy2 hello word
  int i; 
  // grab blocks!
  pixy.ccc.getBlocks();
  
  // If there are detect blocks, print them!
  if (pixy.ccc.numBlocks)
  {
    Serial.print("Detected ");
    Serial.println(pixy.ccc.numBlocks);
    for (i=0; i<pixy.ccc.numBlocks; i++)
    {
      Serial.print("  block ");
      Serial.print(i);
      Serial.print(": ");
      pixy.ccc.blocks[i].print();
    }
  }  
}

void speed(){//小车运动
  Motor_Forward(1,255);
  Motor_Forward(2,255);  
  delay(1000);
//  Motor_Stop(1);
//  Motor_Stop(2);
  delay(1550);
}
 


void Motor_Forward(char motor, char pwm)//
{
  if (motor == 1)
  {
    analogWrite(PWMA_IN1, pwm);
    analogWrite(PWMA_IN2, 255);
  }
  else if (motor == 2)
  {
    analogWrite(PWMB_IN1, pwm);
    analogWrite(PWMB_IN2, 255);
  }
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
