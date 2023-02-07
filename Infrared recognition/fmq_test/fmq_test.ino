int fm = 12;
int
void setup() {
  // put your setup code here, to run once:
     Serial.begin(9600);
     pinMode(fm,OUTPUT);
     pinMode(Echo,INPUT);
     pinMode(Trig,OUTPUT);
 }

void loop() {
//  // put your main code here, to run repeatedly:
//     for(int i=0;i<100;i++){
//      digitalWrite(fm,HIGH);
//      delayMicroseconds(1000);
//     }
//     digitalWrite(fm,LOW);
//     delay(1000);
//
    digitalWrite(Trig,LOW);
    delayMicroseconds(2);
    digitalWrite(Trig,HIGH);
    delayMicroseconds(10);
    digitalWrite(Trig,LOW);
    float a = pulseIn(Echo,HIGH);
    Serial.println(a);
    

     
}
