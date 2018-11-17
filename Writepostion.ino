#define length 3.0
#include<math.h>
int input;
int input1=9;
int input2=10;
int time=1;

float Position(){
  return(length-analogRead(A0)*length/1023.0);
}

void WritePosition1(float des){
  float position;
  position=Position();
  int i;
  while(i<200&&(position>des+0.02||position<des-0.02)){
    if(position<des){
      digitalWrite(input2,HIGH);
      digitalWrite(input1,LOW);
    }
    else{
      digitalWrite(input1,HIGH);
      digitalWrite(input2,LOW);
    }
    position=Position();
    Serial.println(des);
    Serial.println(-Position());
    i++;
  }
  digitalWrite(input2,LOW);
  digitalWrite(input1,LOW);
}
void WritePosition2(float des){
  int i=0;
  float position;
  position=Position();
  if(position>des+0.02){
    digitalWrite(input1,HIGH);
      digitalWrite(input2,LOW);
  }else if(position<des-0.02){
    digitalWrite(input1,LOW);
      digitalWrite(input2,HIGH);
  }
  while(position>des+0.05||position<des-0.05){
    position=Position();
    Serial.println(99);
    if(i++>500) break;
    }
  digitalWrite(input2,LOW);
  digitalWrite(input1,LOW);
}
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  pinMode(input1,OUTPUT);
  pinMode(input2,OUTPUT);
    
}

void loop() {
  float t;
  if(Serial.available()>0){
    float writepos;
    writepos=Serial.parseFloat();
    WritePosition2(writepos);
    
  }
  
  Serial.println(Position());
}
  // put your main code here, to run repeatedly:


