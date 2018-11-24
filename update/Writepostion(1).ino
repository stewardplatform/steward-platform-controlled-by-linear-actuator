#define length 3.0
#include<math.h>

int input;
int input1=9;
int input2=10;
int time=1;
float a1[]={-2.8,-3.0,0};
float a2[]={2.8,-3.0,0};
float a3[]={3.9,-0.94,0};
float a4[]={1.0,3.5,0};
float a5[]={-1.0,3.5,0};
float a6[]={-3.9,-0.94,0};
float b1[]={-1.1,-2.7,0};
float b2[]={1.1,-2.7,0};
float b3[]={3.0,0.6,0};
float b4[]={1.9,2.5,0};
float b5[]={-1.9,2.5,0};
float b6[]={-3.0,0.6,0};


typedef struct Actuator{
  int input1;
  int input2;
  float speed;
  float minlen;
  float maxlen;
  int posfeedback;
}Actuator;

Actuator actuator1;
Actuator actuator2;
Actuator actuator3;
Actuator actuator4;
Actuator actuator5;
Actuator actuator6;

float ll(float x,float y,float z,float a,float b,float c,float *A,float *B){    //运动学反解
  float ca,sa,cb,sb,cc,sc;
  float l;
  ca=cos(a);sa=sin(a);cb=cos(b);sb=sin(b);cc=cos(c);sc=sin(c);
  l=sqrt(pow(z - A[2] + B[2]*cb + B[1]*cc*sb + B[0]*sb*sc,2) + pow(A[1] - y - B[0]*(cc*sa + ca*cb*sc) + B[1]*(sa*sc - ca*cb*cc) + B[2]*ca*sb,2)+pow(x - A[0] + B[0]*(ca*cc - cb*sa*sc) - B[1]*(ca*sc + cb*cc*sa) + B[2]*sa*sb,2));
  return(l);
}

float Position(Actuator actuator){
  return(length-analogRead(actuator.posfeedback)*length/1023.0);
}

void WritePosition1(float des,Actuator actuator){
  float position;
  position=Position(actuator);
  if(des>2.92) des=2.92;
  if(des<0.08) des=0.08;
  int i;
  while(i<200&&(position>des+0.02||position<des-0.02)){
    if(position<des){
      digitalWrite(actuator.input2,HIGH);
      digitalWrite(actuator.input1,LOW);
    }
    else{
      digitalWrite(actuator.input1,HIGH);
      digitalWrite(actuator.input2,LOW);
    }
    position=Position(actuator);
    Serial.println(des);
    Serial.println(-Position(actuator));
    i++;
  }
  digitalWrite(actuator.input2,LOW);
  digitalWrite(actuator.input1,LOW);
}
/*void WritePosition2(float des){
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
}*/
float ax0,ax1,ax2,ax3,ax4,ax5,ay0,ay1,ay2,ay3,ay4,ay5,az0,az1,az2,az3,az4,az5,aa0,aa1,aa2,aa3,aa4,aa5,ab0,ab1,ab2,ab3,ab4,ab5,ac0,ac1,ac2,ac3,ac4,ac5;
void paramset(float x0,float xn,float y0,float yn,float z0,float zn,float a0,float an,float b0,float bn,float c0,float cn,float tn){   //五次路径规划系数设定
  ax0=x0;
  ax1=0;
  ax2=0;
  ax3=10.0*(xn-x0)/pow(tn,3);
  ax4=-15.0*(xn-x0)/pow(tn,4);
  ax5=6.0*(xn-x0)/pow(tn,5);
  ay0=y0;
  ay1=0;
  ay2=0;
  ay3=10.0*(yn-y0)/pow(tn,3);
  ay4=-15.0*(yn-y0)/pow(tn,4);
  ay5=6.0*(yn-y0)/pow(tn,5);
  az0=z0;
  az1=0;
  az2=0;
  az3=10.0*(zn-z0)/pow(tn,3);
  az4=-15.0*(zn-z0)/pow(tn,4);
  az5=6.0*(zn-z0)/pow(tn,5);
  aa0=a0;
  aa1=0;
  aa2=0;
  aa3=10.0*(an-a0)/pow(tn,3);
  aa4=-15.0*(an-a0)/pow(tn,4);
  aa5=6.0*(an-a0)/pow(tn,5);
  ab0=b0;
  ab1=0;
  ab2=0;
  ab3=10.0*(bn-b0)/pow(tn,3);
  ab4=-15.0*(bn-b0)/pow(tn,4);
  ab5=6.0*(bn-b0)/pow(tn,5);
  ac0=c0;
  ac1=0;
  ac2=0;
  ac3=10.0*(cn-c0)/pow(tn,3);
  ac4=-15.0*(cn-c0)/pow(tn,4);
  ac5=6.0*(cn-c0)/pow(tn,5);
}

float p(char a,float t){                                   //五次路径规划函数
  float p;
  if(a=='x') p=ax0+ax3*pow(t,3)+ax4*pow(t,4)+ax5*pow(t,5);
  else if(a=='y') p=ay0+ay3*pow(t,3)+ay4*pow(t,4)+ay5*pow(t,5);
  else if(a=='z') p=az0+az3*pow(t,3)+az4*pow(t,4)+az5*pow(t,5);
  else if(a=='a') p=aa0+aa3*pow(t,3)+aa4*pow(t,4)+aa5*pow(t,5);
  else if(a=='b') p=ab0+ab3*pow(t,3)+ab4*pow(t,4)+ab5*pow(t,5);
  else if(a=='c') p=ac0+ac3*pow(t,3)+ac4*pow(t,4)+ac5*pow(t,5);
  return(p);
}
void Move(float x0,float xn,float y0,float yn,float z0,float zn,float a0,float an,float b0,float bn,float c0,float cn,float tn){
  float t;
  float l1,l2,l3,l4,l5,l6;
  float x,y,z,a,b,c;
  paramset(x0,xn,y0,yn,z0,zn,a0,an,b0,bn,c0,cn,tn);
  for(t=0;t<tn;t=t+0.01){
    x=p('x',t);
      y=p('y',t);
      z=p('z',t);
      a=p('a',t);
      b=p('b',t);
      c=p('c',t);
      l1=ll(x,y,z,a,b,c,a1,b1)-8.4;
      l2=ll(x,y,z,a,b,c,a2,b2)-8.4;
      l3=ll(x,y,z,a,b,c,a3,b3)-8.4;
      l4=ll(x,y,z,a,b,c,a4,b4)-8.4;
      l5=ll(x,y,z,a,b,c,a5,b5)-8.4;
      l6=ll(x,y,z,a,b,c,a6,b6)-8.4;
      WritePosition1(l1,actuator1);
      WritePosition1(l2,actuator2);
      WritePosition1(l3,actuator3);
      WritePosition1(l4,actuator4);
      WritePosition1(l5,actuator5);
      WritePosition1(l6,actuator6);
      delay(10);
  }
  for(t=tn;t>0;t=t-0.01){
    x=p('x',t);
      y=p('y',t);
      z=p('z',t);
      a=p('a',t);
      b=p('b',t);
      c=p('c',t);
      l1=ll(x,y,z,a,b,c,a1,b1)-8.4;
      l2=ll(x,y,z,a,b,c,a2,b2)-8.4;
      l3=ll(x,y,z,a,b,c,a3,b3)-8.4;
      l4=ll(x,y,z,a,b,c,a4,b4)-8.4;
      l5=ll(x,y,z,a,b,c,a5,b5)-8.4;
      l6=ll(x,y,z,a,b,c,a6,b6)-8.4;
      WritePosition1(l1,actuator1);
      WritePosition1(l2,actuator2);
      WritePosition1(l3,actuator3);
      WritePosition1(l4,actuator4);
      WritePosition1(l5,actuator5);
      WritePosition1(l6,actuator6);
      delay(10);
  }
}

void Speedwrite(Actuator actuator,float v){ //
  int intput1=actuator.input1;
  int intput2=actuator.input2;
  
  if(v>=0){
    if(v>5){
      analogWrite(input2,255);
      analogWrite(input1,0);
    }
    else{
      analogWrite(input2,v/5*255);
      analogWrite(input1,0);
    }
    actuator.speed=v/5*255;
  }
  else{
    if(v<-5){
      analogWrite(input1,255);
      analogWrite(input2,0);
    }
    else{
      analogWrite(input1,-v/5*255);
      analogWrite(input2,0);
    }
    actuator.speed=v/5*255;
  }
}
void Protect(Actuator actuator){
  if((Position(actuator)<=0.08&&actuator.speed<0)||(Position(actuator)>=2.92&&actuator.speed>0))  //protect
    Speedwrite(actuator,0);
}

float settime=3;
float Movetest(float t){     //test   hahahahahaha
  float spe;
  float distance=7;
  spe=distance*(-t*2+settime)/(1.0/4*settime*settime);
  return spe;
}
void setup() {
  //x0=0;y0=0;z0=9.6;a0=0;b0=0;c0=0;//zxz
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  pinMode(1,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  actuator1.input1=13;
  actuator1.input2=2;
  actuator1.posfeedback=1;
  actuator2.input1=3;
  actuator2.input2=4;
  actuator2.posfeedback=2;
  actuator3.input1=5;
  actuator3.input2=6;
  actuator3.posfeedback=3;
  actuator4.input1=7;
  actuator4.input2=8;
  actuator4.posfeedback=4;
  actuator5.input1=9;
  actuator5.input2=10;
  actuator5.posfeedback=5;
  actuator6.input1=11;
  actuator6.input2=12;
  actuator6.posfeedback=6;
}
String comdata = "";
void loop() {
  float t;
  while (Serial.available() > 0)    
  {  
    comdata =char(Serial.read());  
    delay(2);  
  }  
  if(comdata=="a"){
    //Move(x0,xn,y0,yn,z0,zn,a0,an,b0,bn,c0,cn,tn)
    Move(0,0,0,0,9.6,9.6,-20,20,0,0,0,0,2);
  }
  if(comdata=="b"){
    //Move(x0,xn,y0,yn,z0,zn,a0,an,b0,bn,c0,cn,tn)
    Move(0,0,0,0,9.6,9.6,0,0,0,-20,0,0,2);
  }
  if(comdata=="c"){
    for(t=0;t<=settime;t=t+0.01){
      Speedwrite(actuator1,Movetest(t));
      Speedwrite(actuator2,Movetest(t));
      Speedwrite(actuator3,Movetest(t));
      Speedwrite(actuator4,Movetest(t));
      Speedwrite(actuator5,Movetest(t));
      Speedwrite(actuator6,Movetest(t));
      Protect(actuator1);
      Protect(actuator2);
      Protect(actuator3);
      Protect(actuator4);
      Protect(actuator5);
      Protect(actuator6);
      delay(10);
    }
  }
  
  Serial.println();
  Serial.print(Position(actuator1));
  Serial.print(" ");
  Serial.print(Position(actuator2));
  Serial.print(" ");
  Serial.print(Position(actuator3));
  Serial.print(" ");
  Serial.print(Position(actuator4));
  Serial.print(" ");
  Serial.print(Position(actuator5));
  Serial.print(" ");
  Serial.print(Position(actuator6));
  Serial.println("\n");
  
}
  // put your main code here, to run repeatedly:


