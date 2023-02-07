#include <PS2X_lib.h>            //澹版槑PS2鎵嬫焺搴�
#include <Servo.h>               //澹版槑鑸垫満搴�
PS2X ps2x;
#define  BEEP_PIN   12           //瀹氫箟铚傞福鍣ㄥ紩鑴欴12
#define  NLED_PIN   13           //瀹氫箟LED鐏紩鑴欴13
#define  NLED_MS_BETWEEN        1000
#define SER1_BAUD   115200
#define  DUOJI_MS_BETWEEN       20.000
unsigned char val = 0;
#define DJ_NUM  6                //瀹忓畾涔変己鏈嶈埖鏈虹殑涓暟  
int SPEED = 100;  //璁剧疆鐢垫満榛樿閫熷害
/*鐢垫満椹卞姩寮曡剼*/
#define PWMB_IN1 11       //瀹氫箟IN1寮曡剼
#define PWMB_IN2  6       //瀹氫箟IN2寮曡剼
#define PWMA_IN1  5        //瀹氫箟IN3寮曡剼
#define PWMA_IN2  3        //瀹氫箟IN4寮曡剼
/******************************************************************
  瀹忓畾涔塒S2鎵嬫焺寮曡剼锛岀敤鏈夋湁鎰忎箟鐨勫瓧绗︿唬琛ㄧ浉搴旀槸寮曡剼锛屼究浜庝娇鐢�
******************************************************************/
#define PS2_DAT      8
#define PS2_CMD      9
#define PS2_ATT      2
#define PS2_CLK      4
#define  PS2_MS_BETWEEN         50
byte type = 0;
String uart1_receive_buf = "";   //澹版槑涓�涓瓧绗︿覆鏁扮粍
int get_byte = 0;       // 鎺ユ敹鍒扮殑 data byte
boolean newLineReceived = false; // 鍓嶄竴娆℃暟鎹粨鏉熸爣蹇�
boolean startBit  = false;  //鍗忚寮�濮嬫爣蹇�
Servo myservo[DJ_NUM];           //鍒涘缓鑸垫満浼烘湇瀵硅薄鏁扮粍
unsigned int duoji_index;       //澹版槑鑸垫満鍙橀噺 锛堣埖鏈哄彿锛�
byte duoji_pin[DJ_NUM] = {10, A2, A3, A0, A1, 7};//瀹氫箟涓�涓暟缁勶紝瀛樺偍寮曡剼鏁板瓧
unsigned int Pwm_Value[6] = {1500, 1500, 1500, 1500, 1500, 1500};
void dida(u8 times, u8 frequency);
void handle_nled();
void handle_ps2();
void Parse_str(String str);
void Motor_Forward(char motor, char pwm);
void Motor_Back(char motor, char pwm);
void Motor_Right(char motor, char pwm);
void Motor_Left(char motor, char pwm);
void Motor_Stop(char motor);
unsigned char handle_ms_between( unsigned long *time_ms, unsigned int ms_between);
void setup() {
  
  for (byte i = 0; i < DJ_NUM; i++)
  { //寰幆浣跨浉搴旂殑鑸垫満浼烘湇瀵硅薄杩炴帴鍒板搴旂殑寮曡剼
    myservo[i].attach(duoji_pin[i]);
  }
  for (byte i = 0; i < DJ_NUM; i++)
  {
    myservo[i].writeMicroseconds(Pwm_Value[i]);//寰幆鍐欏叆鍒濆PWM鐩爣鍊�
  }
  Serial.begin(SER1_BAUD);
  Serial.println("uart1 init OK");
  pinMode(BEEP_PIN, OUTPUT);
  pinMode(NLED_PIN, OUTPUT);
  
//  Motor_Right(1,123);
//  Motor_Left(2,123);
  
  type = ps2x.readType();//杩斿洖PS2绫诲瀷
  switch (type) {
    case 0:
      Serial.println("Unknown Controller type");
      break;
    case 1:
      Serial.println("DualShock Controller Found");
      break;
    case 2:
      Serial.println("GuitarHero Controller Found");
      break;
  }
  digitalWrite(NLED_PIN, 1);
  dida(3, 100);
//  Motor_Stop(1);
//  Motor_Stop(2);//////////////////////////////////////////////////////////

}



void loop()
{
  //  handle_nled();
  handle_ps2();
  Parse_str(uart1_receive_buf);

  
}



//serialEvent()鏄疘DE1.0鍙婁互鍚庣増鏈柊澧炵殑鍔熻兘锛屼笉娓呮涓轰粈涔堝ぇ閮ㄤ唤浜轰笉鎰挎剰鐢紝杩欎釜鍙槸鐩稿綋浜庝腑鏂姛鑳戒竴鏍风殑鍟�!
void serialEvent()
{
  while (Serial.available())
  {
    get_byte = Serial.read();              //涓�涓瓧鑺備竴涓瓧鑺傚湴璇伙紝涓嬩竴鍙ユ槸璇诲埌鐨勬斁鍏ュ瓧绗︿覆鏁扮粍涓粍鎴愪竴涓畬鎴愮殑鏁版嵁鍖�
    if ( get_byte == 'B' || get_byte == '<' || get_byte == '#')
    {
      startBit = true;
    }
    if (startBit == true)
    {
      uart1_receive_buf += (char) get_byte;     // 鍏ㄥ弻宸ヤ覆鍙ｅ彲浠ヤ笉鐢ㄥ湪涓嬮潰鍔犲欢鏃讹紝鍗婂弻宸ュ垯瑕佸姞鐨�//
    }
    if (get_byte == '\n' || get_byte == '>')
    {
      newLineReceived = true;
      startBit = false;
    }
  }
}

void Parse_str(String str)
{
  int index = 0, len = 0, i = 0;
  int pwm = 0;
  if (newLineReceived)
  {
    if (str[0] == '#')
    {
      if (str[1] == '1')  index = 1;
      if (str[1] == '2')  index = 2;
      if (str[1] == '3')  index = 3;
      if (str[1] == '4')  index = 4;
      if (str[1] == '5')  index = 5;
      if (str[1] == '6')  index = 6;
      len = str.length();  //鑾峰彇涓插彛鎺ユ敹鏁版嵁鐨勯暱搴�
      pwm = 0;
      for (i = 0; i < len; i++) {             //濡傛灉鏁版嵁娌℃湁鎺ユ敹瀹�
        if (str[i] == 'P') {        //鍒ゆ柇鏄惁涓鸿捣濮嬬鈥淧鈥�
          i++;                                  //涓嬩竴涓瓧绗�
          while ((str[i] != 'T') && (i < len)) { //鍒ゆ柇鏄惁涓�#鍚庨潰鐨勬暟瀛楁娴嬪畬
            pwm =  pwm * 10 + str[i] - '0'; //璁板綍P涔嬪墠鐨勬暟瀛�
            i++;
          }
          //妫�娴嬪畬鍚庤祴鍊�
          Pwm_Value[index - 1] = pwm;
          myservo[index - 1].writeMicroseconds(Pwm_Value[index - 1]);
          index = pwm = 0;
        }
      }
    }

    else if ((str[0] == '<') && (str[1] == 'S'))
    {
      if ((str[2] == 'U') && (str[3] == 'P')) index = 1;
      if ((str[2] == 'D') && (str[3] == 'N')) index = 2;
      if ((str[2] == 'L') && (str[3] == 'T')) index = 3;
      if ((str[2] == 'R') && (str[3] == 'T')) index = 4;
      if ((str[2] == 'D') && (str[3] == 'L')) index = 5;
      if ((str[2] == 'D') && (str[3] == 'R')) index = 6;
      len = str.length();  //鑾峰彇涓插彛鎺ユ敹鏁版嵁鐨勯暱搴�
      pwm = 0;
      for (i = 0; i < len; i++) {             //濡傛灉鏁版嵁娌℃湁鎺ユ敹瀹�
        if (str[i] == '-') {        //鍒ゆ柇鏄惁涓鸿捣濮嬬鈥�#鈥�
          i++;                                  //涓嬩竴涓瓧绗�
          while ((str[i] != '>') && (i < len)) { //鍒ゆ柇鏄惁涓�#鍚庨潰鐨勬暟瀛楁娴嬪畬
            pwm =  pwm * 10 + str[i] - '0'; //璁板綍P涔嬪墠鐨勬暟瀛�
            i++;
          }
          //妫�娴嬪畬鍚庤祴鍊�
          Pwm_Value[index - 1] = pwm;
          myservo[index - 1].writeMicroseconds(Pwm_Value[index - 1]);
          index = pwm = 0;
        }
      }
    }
    else if ((str[0] == 'B' && (str[1] == 'U') && (str[2] == 'P')
              && (str[3] == 'D')) || (str[0] == '<' && (str[1] == 'B') && (str[2] == 'U')
                                      && (str[3] == 'P') && (str[4] == 'D') && (str[5] == '>')))
    {
      Motor_Forward(1, SPEED);
      Motor_Forward(2, SPEED);

      Serial.println("Foward");
    }
    else if ((str[0] == 'B' && (str[1] == 'D') && (str[2] == 'N')
              && (str[3] == 'D')) || (str[0] == '<' && (str[1] == 'B') && (str[2] == 'D')
                                      && (str[3] == 'N') && (str[4] == 'D') && (str[5] == '>')) )
    {

      Motor_Back(1, SPEED);
      Motor_Back(2, SPEED);
      Serial.println("Back");
    }

    else if ((str[0] == 'B' && (str[1] == 'L') && (str[2] == 'T')
              && (str[3] == 'D')) || (str[0] == '<' && (str[1] == 'B') && (str[2] == 'L')
                                      && (str[3] == 'T') && (str[4] == 'D') && (str[5] == '>')) )
    {
      Motor_Forward(1, SPEED);
      Motor_Back(2, SPEED);
      Serial.println("Left");
    }
    else if ((str[0] == 'B' && (str[1] == 'R') && (str[2] == 'T')
              && (str[3] == 'D')) || (str[0] == '<' && (str[1] == 'B') && (str[2] == 'R')
                                      && (str[3] == 'T') && (str[4] == 'D') && (str[5] == '>')) )
    {
      Motor_Forward(2, SPEED);
      Motor_Back(1, SPEED);
      Serial.println("Right");
    }
    else if ((str[0] == 'B' && (str[1] == 'S') && (str[2] == 'T')
              && (str[3] == 'D')) || (str[0] == '<' && (str[1] == 'B') && (str[4] == 'U') && (str[5] == '>')))
    {
      Motor_Stop(1);
      Motor_Stop(2);
    }
    else if (str[0] == 'B' && (str[1] == 'U') && (str[2] == 'A')
             && (str[3] == 'D') )
    {
      SPEED = SPEED - 10;
      if (SPEED <= 50)  SPEED = 50;
    }
    else if (str[0] == 'B' && (str[1] == 'U') && (str[2] == 'M')
             && (str[3] == 'D') )
    {
      SPEED = SPEED + 10;
      if (SPEED >= 150)  SPEED = 150;
    }
    else if (str[0] == 'B' && (str[1] == 'U') && (str[2] == 'K') //寮�鐏�
             && (str[3] == 'D') )
    {
      digitalWrite(NLED_PIN, val);
      val = ~val;
    }
    else if (str[0] == 'B' && (str[1] == 'U') && (str[2] == 'S') //楦ｇ瑳
             && (str[3] == 'D') )
    {
      dida(1, 1000);
    }

    newLineReceived = false;
    uart1_receive_buf = "";
  }
}



unsigned char handle_ms_between( unsigned long *time_ms, unsigned int ms_between) {
  if (millis() - *time_ms < ms_between) {
    return 0;
  } else {
    *time_ms = millis();
    return 1;
  }
}

void dida(u8 times, u8 frequency) {
  for (byte i = 0; i < times; i++ ) {
    digitalWrite(BEEP_PIN, LOW);
    delay(frequency);
    digitalWrite(BEEP_PIN, HIGH );
    delay(frequency);
  }
}


void handle_nled() {
  static unsigned long systick_ms_bak = 0;
  if (!handle_ms_between(&systick_ms_bak, NLED_MS_BETWEEN))return;
  digitalWrite(NLED_PIN, val);
  val = ~val;
}


void Motor_Forward(char motor, char pwm)
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


void handle_ps2() {        //鎵嬫焺鍑烘潵鍑芥暟
  static unsigned long systick_ms_bak = 0;
  static unsigned char car_active = 0, car_stoped = 0;
  if (!handle_ms_between(&systick_ms_bak, PS2_MS_BETWEEN))return;
  ps2x.read_gamepad();   //璇籔S2鏁版嵁
  if (ps2x.Button(PSB_L1))  {
    Pwm_Value[4] += 15;
    if (Pwm_Value[4] >= 2500)  Pwm_Value[4] = 2500;
    myservo[4].writeMicroseconds(Pwm_Value[4]);
  } else if (ps2x.Button(PSB_L2)) {
    Pwm_Value[4] -= 15;
    if (Pwm_Value[4] <= 500)  Pwm_Value[4] = 500;
    myservo[4].writeMicroseconds(Pwm_Value[4]);
  } else if (ps2x.Button(PSB_R1)) {
    Pwm_Value[5] += 15;
    if (Pwm_Value[5] >= 2500)  Pwm_Value[5] = 2500;
    myservo[5].writeMicroseconds(Pwm_Value[5]);
  } else if (ps2x.Button(PSB_R2)) {
    Pwm_Value[5] -= 15;
    if (Pwm_Value[5] <= 500)  Pwm_Value[5] = 500;
    myservo[5].writeMicroseconds(Pwm_Value[5]);
  } else if (ps2x.Button(PSB_PAD_UP)) {
    Pwm_Value[1] += 15;
    if (Pwm_Value[1] >= 2500)  Pwm_Value[1] = 2500;
    myservo[1].writeMicroseconds(Pwm_Value[1]);
  } else if (ps2x.Button(PSB_PAD_DOWN)) {
    Pwm_Value[1] -= 15;
    if (Pwm_Value[1] <= 500)  Pwm_Value[1] = 500;
    myservo[1].writeMicroseconds(Pwm_Value[1]);
  } else if (ps2x.Button(PSB_PAD_LEFT)) {
    Pwm_Value[0] += 15;
    if (Pwm_Value[0] >= 2500)  Pwm_Value[0] = 2500;
    myservo[0].writeMicroseconds(Pwm_Value[0]);
    //Serial.println(pwm_value[0], DEC);

  } else if (ps2x.Button(PSB_PAD_RIGHT)) {
    Pwm_Value[0] -= 15;
    if (Pwm_Value[0] <= 500)  Pwm_Value[0] = 500;
    myservo[0].writeMicroseconds(Pwm_Value[0]);
    //Serial.println(pwm_value[0], DEC);
  } else if (ps2x.Button(PSB_PINK)) {
    Pwm_Value[2] += 15;
    if (Pwm_Value[2] >= 2500)  Pwm_Value[2] = 2500;
    myservo[2].writeMicroseconds(Pwm_Value[2]);
  } else if (ps2x.Button(PSB_GREEN)) {
    Pwm_Value[3] += 15;
    if (Pwm_Value[3] >= 2500)  Pwm_Value[3] = 2500;
    myservo[3].writeMicroseconds(Pwm_Value[3]);
  } else if (ps2x.Button(PSB_RED)) {
    Pwm_Value[2] -= 15;
    if (Pwm_Value[2] <= 500)  Pwm_Value[2] = 500;
    myservo[2].writeMicroseconds(Pwm_Value[2]);
  } else if (ps2x.Button(PSB_BLUE))  {
    Pwm_Value[3] -= 15;
    if (Pwm_Value[3] <= 500)  Pwm_Value[3] = 500;
    myservo[3].writeMicroseconds(Pwm_Value[3]);
  } else if (ps2x.Button(PSB_SELECT)) {
    Serial.println("PSB_SELECT pressed");
  }

  if (ps2x.ButtonReleased(PSB_SELECT))  ;
  //鎸夐敭寮硅捣鏃讹紝鍙戦�佲��$DST锛佲�濇寚浠わ紝鑸垫満鍋滃湪姝ゆ椂鐨勪綅缃�
  if (ps2x.ButtonReleased(PSB_L1) || ps2x.ButtonReleased(PSB_R1) ||
      ps2x.ButtonReleased(PSB_L2) || ps2x.ButtonReleased(PSB_R2) ||
      ps2x.ButtonReleased(PSB_PAD_UP) || ps2x.ButtonReleased(PSB_PAD_DOWN) ||
      ps2x.ButtonReleased(PSB_PAD_LEFT) || ps2x.ButtonReleased(PSB_PAD_RIGHT) ||
      ps2x.ButtonReleased(PSB_PINK) || ps2x.ButtonReleased(PSB_RED) ||
      ps2x.ButtonReleased(PSB_BLUE) || ps2x.ButtonReleased(PSB_GREEN)) {
    uart1_receive_buf = "$DST!";
  }

  if ((ps2x.Analog(PSS_RY) >= 100 && ps2x.Analog(PSS_RY) <= 150) && (ps2x.Analog(PSS_LY) >= 100 && ps2x.Analog(PSS_LY) <= 150)) {
    if (!car_stoped) {
      Motor_Stop(1);
      Motor_Stop(2);
      car_stoped = 1;
    }
  }


  if ((ps2x.Analog(PSS_LY) >= 188) && (ps2x.Analog(PSS_LY) <= 255))
  {
    if (car_active >= 0x0f)
    {
      Motor_Forward(1, 100);
      car_stoped = 0;
    }
    car_active |= (1 << 0);
  }

  else if ((ps2x.Analog(PSS_LY) >= 0) && (ps2x.Analog(PSS_LY) < 68))
  {
    if (car_active >= 0x0f)
    {
      Motor_Back(1, 100);
      car_stoped = 0;
    }
    car_active |= (1 << 1);
  }

  if ((ps2x.Analog(PSS_RY) >= 188) && (ps2x.Analog(PSS_RY) <= 255))
  {
    if (car_active >= 0x0f)
    {
      Motor_Forward(2, 100);
      car_stoped = 0;
    }
    car_active |= (1 << 2);
  }
  else if ((ps2x.Analog(PSS_RY) >= 0) && (ps2x.Analog(PSS_RY) < 68))
  {
    if (car_active >= 0x0f)
    {
      Motor_Back(2, 100);
      car_stoped = 0;
    }
    car_active |= (1 << 3);
  }
}








