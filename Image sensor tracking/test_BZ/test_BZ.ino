#include <Pixy2.h>
#include <PIDLoop.h>
#include <Servo.h>
#include <Math.h>

#define PWMB_IN1 11       //定义IN1引轋
#define PWMB_IN2  6       //定义IN2引轋
#define PWMA_IN1  5        //定义IN3引轋
#define PWMA_IN2  3  

//#define  BEEP_PIN   12   

# define MAX_TRANSLATE_VELOCITY 2200
Pixy2 pixy;


PIDLoop panLoop(350,0,600,true);
PIDLoop tiltLoop(500,0,700,true);
PIDLoop rotateLoop(300,0,0,true);
PIDLoop translateLoop(400,0,0,true);

void setup() {
  Serial.begin(115200);
  Serial.print("Staring...\n");
  pixy.init();
  pixy.changeProg("color_connect_components");



//  pinMode(BEEP_PIN, OUTPUT);
//    digitalWrite(BEEP_PIN,LOW);
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

//  analogWrite(PWMA_IN1, 180);
//  analogWrite(PWMB_IN1, 180);
//  delay(2000);/

  if(index>=0)
    block = trackBlock(index);

  if(block)
  {
    panOffset = (int32_t)pixy.frameWidth/2 - (int32_t)block->m_x;
    tiltOffset = (int32_t)block->m_y - (int32_t)pixy.frameHeight/2;

    panLoop.update(panOffset);
    tiltLoop.update(tiltOffset);

//    pixy.setServos(panLoop.m_command,tiltLoop.m_command);

    rotateLoop.update(panOffset);
    translateLoop.update(-tiltOffset);

    if(translateLoop.m_command>MAX_TRANSLATE_VELOCITY)
      translateLoop.m_command = MAX_TRANSLATE_VELOCITY;

     left = (map(2*panOffset,-100,100,1000,2000)/10);
     right = (map(2*tiltOffset,-100,100,1000,2000)/10);

//     left = (map(2*panOffset,-100,100,1000,2000)/10-60);
//     right = (map(2*tiltOffset,-100,100,1000,2000)/10-60);
     
     Serial.println(String(left)
      + "," + String(right));
     
//     if(left<50 &&left>0){
//        analogWrite(PWMA_IN2, left);
//      
//        delay(1000);
//      }else if(left>=-50&&left<0){
//        analogWrite(PWMA_IN2, -(left));
//      }else if(left>=50 && left<=80){
//        analogWrite(PWMA_IN2, 0);
//        analogWrite(PWMB_IN2, 0);
//        delay(1000);
//      }else{
//          analogWrite(PWMB_IN2, 80);
//          delay(1000);
//        }
//   }
//   else
//   {
//    rotateLoop.reset();
//    translateLoop.reset();
//     analogWrite(PWMA_IN2, 0);
//     analogWrite(PWMB_IN2, 0);
//     delay(1000);
//    index = -1;
//   }

    if(left<125 &&left>0){
        analogWrite(PWMA_IN2,(125-left+170));
        delay(100);
    }else if(left<300 && left>175){
        analogWrite(PWMB_IN2, (left-175+170));
        delay(100);
    }else if(left>=125 && left<=175){
        analogWrite(PWMA_IN2, 70);
        analogWrite(PWMB_IN2, 70);
        delay(100);
    }else{
        analogWrite(PWMA_IN2, 0);
        analogWrite(PWMB_IN2, 0);
        delay(100);
    }
  }else{
      rotateLoop.reset();
      translateLoop.reset();
       analogWrite(PWMA_IN2, 0);
       analogWrite(PWMB_IN2, 0);
       delay(100);
      index = -1;
    }
}

