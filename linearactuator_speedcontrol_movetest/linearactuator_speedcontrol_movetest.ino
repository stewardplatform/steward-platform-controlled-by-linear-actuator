#define length 3.0
#include<math.h>
int input;
int input1=9;
int input2=10;
int time=1;

float Position(){
  return(length-analogRead(A0)*length/1023.0);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(input1,OUTPUT);
  pinMode(input2,OUTPUT);
    
}


float speed=0;    //record speed

void Protect(){
  if((Position()<=0.08&&speed<0)||(Position()>=2.92&&speed>0))  //protect
    Speedwrite(input1,input2,0);
}

void Speedwrite(int input1,int input2,float v){ //
  if(v>=0){
    if(v>5){
      analogWrite(input2,255);
      analogWrite(input1,0);
    }
    else{
      analogWrite(input2,v/5*255);
      analogWrite(input1,0);
      speed=v/5*255;
    }
  }
  else{
    if(v<-5){
      analogWrite(input1,255);
      analogWrite(input2,0);
    }
    else{
      analogWrite(input1,-v/5*255);
      analogWrite(input2,0);
      speed=v/5*255;
    }
  }
}

float settime=3;

float Movetest(float t){     //test   hahahahahaha
  float spe;
  float distance=7;
  spe=distance*(-t*2+settime)/(1.0/4*settime*settime);
  return spe;
}

void loop() {
  float a;
  float t;
  if(Serial.available()>0){
    for(t=0;t<=settime;t=t+0.01){
      Speedwrite(input1,input2,Movetest(t));
      Serial.println(Position());
      delay(10);
    }
   
    
    
    while(Serial.read()>=0){}
  }
  Protect();
  
 
  Serial.println(Position());
}
  // put your main code here, to run repeatedly:
