#include <DynamixelMotor.h>
#include "SoftwareSerial.h"
#include <math.h>
#include <stdio.h>
SoftwareSerial ESP1 (2, 3);

#define PI_2 3.1415926535897932


byte Permiso=0;

float X = 0;
float Y = 135;
float xthe=1;
float ythe=0;

float Xtget = 0;
float Ytget = 0;

long espera1 = 300;
long lasttime;
long lastcalculo;
const uint8_t id1 = 2; //izq 
const uint8_t id2 = 8; //der

int16_t IDbot;
int16_t Idcompanero;
long unsigned int lastcom = 0;
byte Statecom = 1;


byte c = 0; // contador de evasiones
byte RinT = 0; // contador de veces en el target
struct puntos {
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


int enc1;
int enc2;

float wl;
float wr;
const float d = 13;
const float r = 2.5;
float theta = 0;
float P=100, alpha;
float v;
float w;
int16_t vmot1 = 0;
int16_t vmot2 = 0;
byte incommingmsg=0;
byte trigPin1 = 6;    // Trigger
byte echoPin1 = 7;    // Echo izquierdo
byte trigPin2 = 8;
byte echoPin2 = 9;     // Echo Centro
int trigPin3 = A0;
int echoPin3 = A1;    // Echo derecho
byte dist1cm, dist2cm, dist3cm;
void setup() {
  // iniciamos el Serial, la interface con los motores, y el modulowifi
  Serial.begin(baudrate);

  ESP1.begin(baudrate);

  interface.begin(baudrate);
  // iniciamos los motores
  motor1.enableTorque();  //motor1 iniciar
  motor1.wheelMode();     //motor1 en giro continuo
  motor2.enableTorque();
  motor2.wheelMode();
  // Iniciar el contador de actividades;
  lasttime = millis();
  //------------
  pinMode(trigPin1, OUTPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(echoPin2, INPUT);
  pinMode(echoPin3, INPUT);
  //--------------- Los motores inician detenidos
  motor(20, 20);
  delay(2000);
  //Se solicita una identificacion
  while (Serial.available() > 0)  Serial.read();
  ask4id();
  SendPositions(X, Y, IDbot, 1);
  delay(2000);
  SendPositions(X, Y, IDbot, 2);
  //Serial.print(IDbot);
  // Serial.println();
  //Se solicita un objectivo
  //ask4tgets();
  ask4tgetsid(IDbot,5);
  delay(1500);
  ask4tgetsid(IDbot,6);
  // Serial.print(Xtget);
  //Serial.print(" : ");
  //Serial.print(Ytget);
  //Serial.println();
  Idcompanero=companero(IDbot,1);//0=Linea, 1=figura centrica  
  if (Permiso == 0)
{
  while (true){
    ask4permission();
    delay (500);
    if (Permiso ==1){
      break;
    }
  }
}

delay(IDbot*4000);
  
}


void loop() {
  //el valor actual de tiempo
  long currenttime = millis();
// Pedir permiso para empezar a moverse

    // Parte de movimiento
  if (currenttime - lasttime > espera1)
  {
    dist1cm = medirdist(trigPin1, echoPin1);
    dist2cm = medirdist(trigPin2, echoPin2);
    dist3cm = medirdist(trigPin3, echoPin3);
    
    odometria(vmot1, vmot2, X, Y, xthe,ythe);

    if (dist2cm < 15)
   {
      if (dist1cm < 10)
     {
      vmot1=400;
      vmot2=130;
      }
      if (dist3cm < 10)
     {
      vmot1=130;
      vmot2=400;
      }
      else{
      vmot1=450;
      vmot2=120;
      }
      espera1=200;
    }
   else
    {
      cinematica20 (X, Y, Xtget, Ytget);
      movfuzzycator(alpha, P);
      espera1 =100;
    }
    motor(vmot1, vmot2);


    lasttime = millis();
  }
  
  // Parte de comunicacion
  if (Statecom == 1)
  {
    SendPositions(X, Y, IDbot, Statecom);
    lastcom = millis();
    incommingmsg=1;
    Statecom = 2;
  }
  if (Statecom == 2 and ((millis() - lastcom) > 1000))
  {

    SendPositions(X, Y, IDbot, Statecom);
    Statecom = 3;
    incommingmsg=0;
  }

  if (Statecom == 3)
  {
    Ask4allpositions(Statecom);
    lastcom = millis();
    Statecom = 4;
    incommingmsg=1;
  }

  if ((Statecom == 4) and ((millis() - lastcom) > 1000))
  {

    Ask4allpositions(Statecom);
    Statecom = 5;
    incommingmsg=0;
  }
//  if ((Statecom == 5))
//  {
//    ask4tgetsid(IDbot,Statecom);
//    lastcom = millis();
//    Statecom = 6;
//    incommingmsg=1;
//  }
//    if ((Statecom == 6) and ((millis() - lastcom) > 1500))
//  {
//
//    ask4tgetsid(IDbot,Statecom);
//    Statecom = 1;
//    incommingmsg=0;
//  }  
  //Alghoritm of movement
    if ((Statecom == 5))
  {
    if (Permiso==0)
    {
    forma(Idcompanero);
    }
    
    Statecom=1;
  }



  if ((P < 7) and (incommingmsg==0))
  {
  motor(10, 10); //Apagar motores
  SendPositions(X, Y, IDbot, 1);
  delay(1500);
  SendPositions(X, Y, IDbot, 2);
   Statecom = 1;
  while (true) 
    {
   ask4permission();
   delay(500);
   if (Permiso==2)
      {
    break;
      }
    }
    switch(IDbot){
      case 3:
      break;
      case 2:
      delay(3000);
      break;
      case 4:
      delay(5000);
      break;
      case 1:
      delay(7000);
      break;
      case 5:
      delay(9000);
      break;
      case 6:
      delay(1100);
      break;
      }
    Ask4allpositions(3);
    delay(700);
    Ask4allpositions(4);
    forma(Idcompanero);
    cinematica20 (X, Y, Xtget, Ytget);
  }

}

int16_t companero(int ID,int forma)
{
  int16_t Idpart;
  switch(forma)
  {
    case 0: //Es una linea
    if (ID==6){
     Idpart=0;
    }
    else{
    Idpart==ID+1;
    }
    break;
    case 1: //Es una figura con centro
     if (ID==3){
     Idpart=0;
    }
    else{
      if (ID<3){
        Idpart==ID+1;
      }
      else{
        Idpart==ID-1;
      }
    }
    break;
  }
  return Idpart;
}

void forma_esp(int16_t ID,int16_t compi){
  bool mark=true;
  while (mark)
  {      
    Ask4allpositions(3);
    delay(700);
    Ask4allpositions(4);
    
    switch(compi){
      case 0:
      mark=false;
      break;
      case 3:
      if (IDbot<3)
        {
        if (posi.x[compi-1]>120)
          {
            mark=false;
          } 
        }
        else{
          if (posi.x[1]>120)
          {
            mark=false;
          } 
        }
        }
      break;
    } 
    return;
  }
 
  
  
