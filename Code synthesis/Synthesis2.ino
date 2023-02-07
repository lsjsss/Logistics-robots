
//定义轮子电机引脚
#define PWMB_IN1 11 //定义IN1引轋
#define PWMB_IN2  6 //定义IN2引轋
#define PWMA_IN1  5 //定义IN3引轋
#define PWMA_IN2  3 //定义IN4引轋
#define SPEED 90//定义车轮平地前进速度
#define SPEEDUP 100//定义车轮上桥前进速度
#define SPEEDS 100//定义车轮转向速度

int red1 = 7;  //定义D7号引脚----近红外
int red2 = A1;  //定义A1号引脚----近红外
int red3 = A0;  //定义A0号引脚----近红外

int rl = 0;//定义传递方向判断

void firstRun();  //定义开始第一步骤运动----1
void secondRun();  //定义开始第二步骤运动--
void thiredRun();
void forthRun();
void fifthRun();
void sixRun();
void sevenRun();

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
  Serial.begin(9600); //定义波特率
  
  pinMode(red1,INPUT);//接收红外线传感器D7引脚反馈数据     
}

void loop() {
  delay(1000);  //开机等待1秒
  firstRun(); //开始第一步骤运动--1

//  go();//车子走向检测程序----应急用

  Serial.println("近红外传感器D7检测数值：");
  Serial.println(digitalRead(red1));//在串口监视器上输出近红外反馈数值

  secondRun();//直行
  thiredRun();
  
  forthRun();
  fifthRun();

  sixRun();
  sevenRun();

  
  delay(100000);
 
  cstop();
  
//  delay(5000);//停止5秒



}

void firstRun(){  //定义开始第一步骤运动--1------------------------------出发段
  
  analogWrite(PWMA_IN2, SPEED);  //左侧轮子以100转向前运动76cm
  analogWrite(PWMB_IN2, SPEED);  //右侧轮子以100转向前运动
  
  delay(2750);//向前走5.----------------------------------------------------------------------------------------------------第一段直行时间控制
  
  analogWrite(PWMA_IN2, 0); //左侧轮子以停止
  analogWrite(PWMB_IN2, 0); //右侧轮子以停止

  delay(500);//停止0.5秒
  
}

void secondRun(){  //定义开始第二步骤运动--2----------------------------------转向段
  if(digitalRead(red1)==LOW){//LOW值为0，表示左侧有障碍，车子向右，否则向左 
         right(SPEEDS);//向右转向
         delay(2900);//-----------------------------------------------------------------------------------------------------第一段转向时间控制
         cstop();
         delay(1000);
         rl=1;
  }else{       
       left(SPEEDS);//左转
       delay(2900);//-----------------------------------------------------------------------------------------------------第一段转向时间控制
      
       cstop();
       delay(1000);
      rl=2;
    }
  }


void thiredRun(){//定义开始第三步骤运动--3----------------------------------上桥段
  analogWrite(PWMA_IN2, SPEEDUP);  //左侧轮子以100转向前运动76cm
  analogWrite(PWMB_IN2, SPEEDUP);  //右侧轮子以100转向前运动
  
  delay(8250);//向前走8.2秒//-----------------------------------------------------------------------------------------------------第二段上桥时间控制
  
  analogWrite(PWMA_IN2, 0); //左侧轮子以停止
  analogWrite(PWMB_IN2, 0); //右侧轮子以停止

  delay(1000);//停止1秒
  
}


void forthRun(){  //定义开始第四步骤运动--4-----------------------------转向段
  if(rl==1){//LOW值为0，表示左侧有障碍，车子向右，否则向左         
         right(SPEEDS);//向右转向
         delay(1000);//-----------------------------------------------------------------------------------------------------第二段转向时间控制
         cstop();
         delay(1000);
         rl=1;

  }else{
       left(SPEEDS);//左转
       delay(1000);//-----------------------------------------------------------------------------------------------------第二段转向时间控制
       cstop();
       delay(1000);
      rl=2;
    }
  }


void fifthRun(){//定义开始第五步骤运动--5-----------------------------过桥后直行段
  analogWrite(PWMA_IN2, SPEED);  //左侧轮子以100转向前运动76cm
  analogWrite(PWMB_IN2, SPEED);  //右侧轮子以100转向前运动
  
  delay(3450);//向前走2秒//-----------------------------------------------------------------------------------------------------第三段直行时间控制

  analogWrite(PWMA_IN2, 0); //左侧轮子以停止
  analogWrite(PWMB_IN2, 0); //右侧轮子以停止

  delay(1000);//
}

void sixRun(){  //定义开始第六步骤运动--6-----------------------------转向段
  if(rl==1){//LOW值为0，表示左侧有障碍，车子向右，否则向左         
         right(SPEEDS);//向右转向
         delay(1000);//-----------------------------------------------------------------------------------------------------第三段转向时间控制
         cstop();
         delay(1000);
         rl=1;

  }else{
       left(SPEEDS);//左转
       delay(1000);//-----------------------------------------------------------------------------------------------------第三段转向时间控制
       cstop();
       delay(1000);
      rl=2;
    }
  }


void sevenRun(){//定义开始第五步骤运动--5-----------------------------过桥后直行段
  analogWrite(PWMA_IN2, SPEED);  //左侧轮子以100转向前运动76cm
  analogWrite(PWMB_IN2, SPEED);  //右侧轮子以100转向前运动
  
  delay(1450);//向前走2秒//-----------------------------------------------------------------------------------------------------第三段直行时间控制

  analogWrite(PWMA_IN2, 0); //左侧轮子以停止
  analogWrite(PWMB_IN2, 0); //右侧轮子以停止

  delay(1000);//
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
  analogWrite(PWMA_IN2, 0);
  analogWrite(PWMB_IN2, pwm);
  Serial.println("车子左转");
}

void right(char pwm)//定义车子向右运动，pwm转速
{
  analogWrite(PWMA_IN2, pwm);
  analogWrite(PWMB_IN2, 0);
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
    }else if(digitalRead(red2)==HIGH&&digitalRead(red3)==LOW){//
       left(SPEED);
       delay(200);
       cstop();
       delay(1000);
    }else if(digitalRead(red2)==LOW&&digitalRead(red3)==HIGH){
       right(SPEED);
       delay(300);
       cstop();
       delay(1000);
    }
//    else if(digitalRead(red2)==HIGH&&digitalRead(red3)==HIGH){
//       left(SPEED);
//       delay(300);
//       cstop();
//       delay(1000);
//    }
  }
}

void notest(){
  if(digitalRead(red2)==HIGH&&digitalRead(red3)==HIGH){//转向后检测不到桥梁
     left(SPEED);
     delay(300);
     cstop();
     delay(1000);
  }
}

