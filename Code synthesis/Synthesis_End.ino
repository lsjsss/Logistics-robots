
//定义轮子电机引脚
#define PWMB_IN1 11 //定义IN1引轋
#define PWMB_IN2  6 //定义IN2引轋
#define PWMA_IN1  5 //定义IN3引轋
#define PWMA_IN2  3 //定义IN4引轋
#define SPEED 90//定义车轮速度

int red1 = 7;  //定义D7号引脚----近红外
int red2 = A1;  //定义A1号引脚----近红外
int red3 = A0;  //定义A0号引脚----近红外

int rl = 0;//定义传递方向判断

void firstRun();  //定义开始第一步骤运动----1
void secondRun();  //定义开始第二步骤运动--2


void go();//车子走向检测程序
void Motor_Forward(char pwm);//定义轮子向前运动，pwm转速
void forward(char pwm);//定义车子向前运动，pwm转速
void back(char pwm);//定义车子向后运动，pwm转速
void left(char pwm);//定义车子向左运动，pwm转速
void right(char pwm);//定义车子向右运动，pwm转速
void cstop();//定义车子停止

void nearred();//定义近红外传感器执行避障操作
void bauto();//自动避障
void rauto();//自动循迹


void setup() {//程序开始
  Serial.begin(115200); //定义波特率
  
  pinMode(red1,INPUT);//接收红外线传感器D7引脚反馈数据
  pinMode(red2,INPUT);//接收红外线传感器A0引脚反馈数据
  pinMode(red3,INPUT);//接收红外线传感器A1引脚反馈数据
      
}

void loop() {
  delay(1000);  //开机等待1秒
  firstRun(); //开始第一步骤运动--1

//  go();//车子走向检测程序----应急用

  Serial.println("近红外传感器D7检测数值：");
  Serial.println(digitalRead(red1));//在串口监视器上输出近红外反馈数值

  Serial.println("近红外传感器A1、A0检测数值：");
  Serial.println(digitalRead(red2));//在串口监视器上输出近红外A1反馈数值
  Serial.println(digitalRead(red3));//在串口监视器上输出近红外A0反馈数值

  secondRun();

  
//  nearred();//定义近红外传感器执行避障操作



  
//  Serial.println("近红外传感器A1检测数值：");
//  Serial.println(digitalRead(red2));//在串口监视器上输出近红外反馈数值
//
//   if(digitalRead(red2)==LOW){//LOW值为0，表示前方有障碍，车子向后，否则向左
//     back(100);
//     delay(1000);
//     cstop();
//     delay(10000);
//  }else{
//     cstop();
//     delay(10000);
//  }

//  Serial.println("近红外传感器A0检测数值：");
//  Serial.println(digitalRead(red3));//在串口监视器上输出近红外反馈数值
//
//   if(digitalRead(red3)==LOW){//LOW值为0，表示前方有障碍，车子向后，否则向左
//     back(100);
//     delay(1000);
//     cstop();
//     delay(10000);
//  }else{
//     cstop();
//     delay(10000);
//  }

  Serial.println("近红外传感器A1、A0检测数值：");
  Serial.println(digitalRead(red2));//在串口监视器上输出近红外A1反馈数值
  Serial.println(digitalRead(red3));//在串口监视器上输出近红外A0反馈数值

  rauto();


//  forward(100);
//  delay(3000);
  cstop();
  delay(5000);//停止5秒



}

void firstRun(){  //定义开始第一步骤运动--1
  
  analogWrite(PWMA_IN2, 80);  //左侧轮子以80转向前运动
  analogWrite(PWMB_IN2, 80);  //右侧轮子以80转向前运动
  
  delay(750);//向前走0.75秒
  
  analogWrite(PWMA_IN2, 0); //左侧轮子以停止
  analogWrite(PWMB_IN2, 0); //右侧轮子以停止

  delay(1000);//停止1秒
  
}

void secondRun(){  //定义开始第二步骤运动--2
  if(digitalRead(red1)==LOW){//LOW值为0，表示左侧有障碍，车子向右，否则向左
    rl=1;
    int j=0;
    while(j==0){
      if(digitalRead(red2)==LOW&&digitalRead(red3)==LOW){//发现路径
         right(100);//向右转向
         delay(1000);
         cstop();
         delay(1000);
         j=1;
      }else{
         forward(60);//前进探测墙面
         delay(500);
         cstop();
         delay(1000);
      }
    }
  }else{
    rl=2;
    int k=0;
    while(k==0){
    if(digitalRead(red2)==LOW&&digitalRead(red3)==LOW){//发现路径
       left(100);//左转
       delay(1000);
       cstop();
       delay(1000);
       k=1;
      }else{
       forward(60);//前进探测墙面
       delay(500);
       cstop();
       delay(1000);
      }
    }
  }
}




void go(){//车子走向检测程序
  
  forward(80);//定义车子向前运动，pwm转速
  delay(1000);

  cstop();//定义车子停止
  delay(1000);

  back(80);//定义车子向后运动，pwm转速
  delay(1000);

  cstop();//定义车子停止
  delay(1000);
  
  left(80);//定义车子向左运动，pwm转速
  delay(1000);

  cstop();//定义车子停止
  delay(1000);

  right(80);//定义车子向右运动，pwm转速
  delay(1000);

  cstop();//定义车子停止
  delay(1000);
}

void forward(char pwm)//定义车子向前运动，pwm转速
{
  analogWrite(PWMA_IN2, pwm);
  analogWrite(PWMB_IN2, pwm);
  Serial.println("车子前进");
}

void back(char pwm)//定义车子向后运动，pwm转速
{
  analogWrite(PWMA_IN1, pwm);
  analogWrite(PWMB_IN1, pwm);
  Serial.println("车子后退");
}

void left(char pwm)//定义车子向左运动，pwm转速
{
  analogWrite(PWMA_IN2, pwm);
  analogWrite(PWMB_IN2, 0);
  Serial.println("车子左转");
}

void right(char pwm)//定义车子向右运动，pwm转速
{
  analogWrite(PWMA_IN2, 0);
  analogWrite(PWMB_IN2, pwm);
  Serial.println("车子右转");
}

void cstop()//定义车子停止
{
  analogWrite(PWMA_IN1, 0);
  analogWrite(PWMB_IN1, 0);
  analogWrite(PWMA_IN2, 0);
  analogWrite(PWMB_IN2, 0);
  Serial.println("车子停止");
}

void nearred(){//定义近红外传感器执行避障操作
  if(digitalRead(red1)==LOW){//LOW值为0，表示前方有障碍，车子停止运动
     forward(0);
  }else{
     forward(100);
  }
}


void bauto(){//自动避障
  int i=0;
  while(i==0){
    if(digitalRead(red2)==LOW&&digitalRead(red3)==LOW){//发现路径
       forward(SPEED);
       delay(1000);
       cstop();
       delay(1000);
       i=1;
    }else if(digitalRead(red2)==HIGH&&digitalRead(red3)==HIGH){
       left(SPEED);
       delay(1000);
       cstop();
       delay(1000);
    }
  }
}

void rauto(){//自动循迹
  int i=0;
  while(i==0){
    if(digitalRead(red2)==LOW&&digitalRead(red3)==LOW){//发现路径
       forward(SPEED);
       delay(300);
       cstop();
       delay(1000);
       i=1;
    }else if(digitalRead(red2)==HIGH&&digitalRead(red3)==LOW){
       left(SPEED);
       delay(300);
       cstop();
       delay(1000);
    }else if(digitalRead(red2)==LOW&&digitalRead(red3)==HIGH){
       right(SPEED);
       delay(300);
       cstop();
       delay(1000);
    }else if(digitalRead(red2)==HIGH&&digitalRead(red3)==HIGH){
       left(SPEED);
       delay(300);
       cstop();
       delay(1000);
    }
  }
}