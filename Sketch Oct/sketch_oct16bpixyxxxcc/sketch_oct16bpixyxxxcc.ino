#include <Pixy2.h>
#include <PIDLoop.h>
#include <Servo.h>

Servo leftMotor1;
Servo rightMotor1;
Servo leftMotor2;
Servo rightMotor2;

int leftMotorPin1 = 11;
int rightMotorPin1 = 6;
int leftMotorPin2 = 5;
int rightMotorPin2 = 3;


#define MAX_TRANSLATE_VELOCITY  2000

Pixy2 pixy;

//PIDLoop panLoop(350,0,600,true);
//PIDLoop tiltLoop(500,0,700,true);
//PIDLoop rotateLoop(300,600,300,false);
//PIDLoop translateLoop(400,800,300,false);

PIDLoop panLoop(350,0,600,true);
PIDLoop tiltLoop(500,0,700,true);
PIDLoop rotateLoop(300,0,0,true);
PIDLoop translateLoop(400,0,0,true);

void setup() {
  Serial.begin(115200);
  Serial.print("Staring...\n");

  delay(2000);

//  speed();/////////////////////////////////////////////////////////////////
  pinMode(leftMotorPin1, OUTPUT);
  pinMode(rightMotorPin1, OUTPUT);
  pinMode(leftMotorPin2, OUTPUT);
  pinMode(rightMotorPin2, OUTPUT);

  leftMotor1.attach(leftMotorPin1);
  leftMotor2.attach(leftMotorPin2);
  rightMotor1.attach(rightMotorPin1);
  rightMotor2.attach(rightMotorPin2);
  
  pixy.init();
  pixy.changeProg("color_connect_components");
}

int16_t acquireBlock()
{
  if(pixy.ccc.numBlocks && pixy.ccc.blocks[0].m_age>30)
    return pixy.ccc.blocks[0].m_index;

   return -1;
}

Block *trackBlock(uint8_t index)
{
  uint8_t i;

  for(i=0;i<pixy.ccc.numBlocks;i++)
  {
    if(index==pixy.ccc.blocks[i].m_index)
      return &pixy.ccc.blocks[i];
  }

  return NULL;
}

void loop() {
  static int16_t index = -1;
  int32_t panOffset, tiltOffset, headingOffset, left, right;
  Block *block=NULL;

  pixy.ccc.getBlocks();

  if(index == -1)
  {
    Serial.println("Searching for block...");
    index = acquireBlock();
    if(index>=0)
      Serial.println("Found block!");
  }

  if(index>=0)
    block = trackBlock(index);

   if(block)
   {
    panOffset = (int32_t)pixy.frameWidth/2 - (int32_t)block->m_x;
    tiltOffset = (int32_t)block->m_y - (int32_t)pixy.frameHeight/2;

    panLoop.update(panOffset);
    tiltLoop.update(tiltOffset);

    pixy.setServos(panLoop.m_command,tiltLoop.m_command);

    rotateLoop.update(panOffset);
    translateLoop.update(-tiltOffset);

    if(translateLoop.m_command>MAX_TRANSLATE_VELOCITY)
      translateLoop.m_command = MAX_TRANSLATE_VELOCITY;

     left = map(2*panOffset,-100,100,1000,2000);
     right = map(-2*panOffset,-100,100,1000,2000);
     Serial.println(String(left)
      + "," + String(right));

    leftMotor1.write(left);
    leftMotor2.write(left);
    rightMotor1.write(right);
    rightMotor2.write(right);

//    leftMotor.write(left);
//    rightMotor.write(right);
   }
   else
   {
    rotateLoop.reset();
    translateLoop.reset();
//    motors.setLeftSpeed(0);
//    motors.setRightSpeed(0);

    leftMotor1.write(1500);
    leftMotor2.write(1500);
    rightMotor1.write(1500);
    rightMotor2.write(1500);

//    leftMotor.write(2000);
//    rightMotor.write(2000);
    
      index = -1;
   }
}

 

