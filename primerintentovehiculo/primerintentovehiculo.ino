#include <DynamixelMotor.h>
#include "SoftwareSerial.h"
#include <math.h>

#define PI_2 3.1415926535897932384626433832795


float X = 0;
float Y= 0;

float Xtget = 0;
float Ytget= 0;

long espera1=500;
long lasttime;

const uint8_t id1=2;
const uint8_t id2=8;

uint8_t IDbot;

byte c= 0; // contador de evasiones

struct puntos{
  int x[6];
  int y[6];
};

puntos posi;

const long unsigned int baudrate = 9600;
#define SOFT_RX_PIN 4
#define SOFT_TX_PIN 5
SoftwareDynamixelInterface interface(SOFT_RX_PIN, SOFT_TX_PIN);
DynamixelMotor motor1(interface, id1);
DynamixelMotor motor2(interface, id2);
DynamixelDevice readdevice1(interface, id1);
DynamixelDevice readdevice2(interface, id2);

SoftwareSerial ESP1 (2,3);

int enc1;
int enc2;
float xn,yn,thetan;
float wl;
float wr;
const float d=10;
const float r=2.5;
float theta=0;
float P=100, alpha;
float v;
float w;
int16_t vmot1=0;
int16_t vmot2=0;

byte trigPin1 = 6;    // Trigger
byte echoPin1 = 7;    // Echo izquierdo
byte trigPin2 = 8;
byte echoPin2 = 9;     // Echo Centro
int trigPin3 = A0;
int echoPin3 = A1;    // Echo derecho
byte dist1cm,dist2cm,dist3cm;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(baudrate);
  interface.begin(baudrate);
  ESP1.begin(baudrate);
  motor1.enableTorque();  
  motor1.wheelMode();
  motor2.enableTorque();  
  motor2.wheelMode();
lasttime=millis();
  
//------------
pinMode(trigPin1,OUTPUT);
pinMode(trigPin2,OUTPUT);
pinMode(trigPin3,OUTPUT);
pinMode(echoPin1,INPUT);
pinMode(echoPin2,INPUT);
pinMode(echoPin3,INPUT);
//---------------
//Xtget += 10.0;
//Ytget=0.3*Xtget+20;
//Xtget =100;
//Ytget =0;    
motor(20,20);
IDbot=ask4id();
Serial.print(IDbot);
ask4tgets();
Serial.print(Xtget,Ytget);
delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  long currenttime=millis();
  if (currenttime-lasttime>espera1)
{
  //Serial.println("probando parte 1");
 dist1cm=medirdist(trigPin1,echoPin1);
 dist2cm=medirdist(trigPin2,echoPin2);
 dist3cm=medirdist(trigPin3,echoPin3);
 
if (dist1cm < 8 or dist2cm <10 or dist3cm<8 or c>0)
 {
//  Serial.println("izq   der");
//  Serial.print(dist1cm);
//  Serial.print("   ");
//  Serial.println(dist2cm);
  if (dist2cm> 10)
  {
    if (c==0){
      c=4;
    }
    else{
      c=c-1;
    }
  }
  
 
 evafuzzycation(dist1cm,dist2cm,dist3cm);
  espera1 = 900;
 }
 else
 {
  cinematica20 (X,Y,Xtget,Ytget); 
  //tangyang(alpha,P);
  //velocidades20(v,w,theta);
  //movings(wl,wr);
  movfuzzycator(alpha,P);
  espera1=500;
 }
  motor(vmot1,vmot2);
  odometria(vmot1,vmot2,X,Y,theta);
  
  Serial.print("timelapse:");
  Serial.println(currenttime-lasttime);  
  lasttime=millis();
//
//  Serial.println("X    y    Y    theta valores: ");
//  Serial.print(X);
//  Serial.print("  ");
//  Serial.print(Y);
//  Serial.print(" ");
//  Serial.println(theta);
//  Serial.print("P:  ");
//  Serial.println(P);
//  Serial.print("a:  ");
//  Serial.println(alpha);

} 

SendPositions(X,Y,IDbot);

Ask4allpositions();
 //Xtget +=11.25;
//Ytget=0.3*Xtget+20; 

    //Serial.println("xtg   ytg");
   // Serial.print(Xtget);
   //Serial.print(" ");
   // Serial.println(Ytget);
    
//if (X>150.0 or P<5)
//{
//    motor(1,1);
//  while (true)
//  {
//    
//    delay(4);
//  }
//}



}
