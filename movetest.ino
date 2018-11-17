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
 float positionw=0.03;
  float position;
  position=Position();
  int i;
  while(i<200&&(position>des+positionw||position<des-positionw)){
    if(position<des){
      digitalWrite(input2,HIGH);
      digitalWrite(input1,LOW);
    }
    else{
      digitalWrite(input1,HIGH);
      digitalWrite(input2,LOW);
    }
    position=Position();
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
    //Serial.println(99);
    //if(i++>500) break;
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
float settime=0.5;

float Movetest(float t){     //test   hahahahahaha
  float pos;
  float distance=2.6;
  pos=distance*(-t*t+settime*t)/(1.0/4*settime*settime);
  return pos;
}
float timet; 
float movet(float t){
  float pos;
  timet=0.5;
  pos=t*2.7/timet;
  return(pos);
}
void loop() {
  int a;
  float t;
  if(Serial.available()>0){
    a=Serial.parseInt();
    if(a==1){
    for(t=0;t<=settime;t=t+0.01){
      WritePosition1(Movetest(t)+0.15);
      //Serial.println(Movetest(t)+0.15);
      
      delay(10);
    }
    Serial.println("hhh");
  }
    if(a==2){
    for(t=0;t<=settime;t=t+0.01){
      WritePosition2(Movetest(t)+0.15);
      //Serial.println(Movetest(t)+0.15);
      
      delay(10);
    }
    Serial.println("hhh");
  }

    if(a==3){
      WritePosition1(2.7);
      WritePosition2(0.2);
    }
    if(a==4){
      for(t=0;t<=timet;t=t+0.02){
        WritePosition2(movet(t)+0.15);
        //Serial.println(Movetest(t)+0.15);
        
        delay(20);
      }
    }
  a=0;
  }
  
  //Serial.println(Position());
}
  // put your main code here, to run repeatedly:


