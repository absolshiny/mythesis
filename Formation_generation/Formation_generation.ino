#include <DynamixelMotor.h>
#include "SoftwareSerial.h"
#include <math.h>
#include <stdio.h>

#define PI_2 3.1415926535897932
#define uint8_t byte 

int Permiso=0;

float X = 28;
float Y = 17;
float xthe=1;
float ythe=0;

float Xtget = 0;
float Ytget = 0;

long espera1 = 300;
long lasttime;
long lastcalculo;
const uint8_t id1 = 16; //izq 
const uint8_t id2 = 10; //der

int IDbot;
int Idcompanero=3;
long unsigned int lastcom = 0;
int Statecom = 1;

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


float wl;
float wr;
const float d = 13;
const float r = 3;
float theta = 0;
float P=100, alpha;
int vmot1 = 0;
int vmot2 = 0;
int incommingmsg=0;
int trigPin1 = 6;    // Trigger
int echoPin1 = 7;    // Echo izquierdo
int trigPin2 = 8;
int echoPin2 = 9;     // Echo Centro
int trigPin3 = A0;
int echoPin3 = A1;    // Echo derecho
int dist1cm=100, dist2cm=100, dist3cm=100;


void setup() {
  // iniciamos el Serial, la interface con los motores, y el modulowifi
  Serial.begin(baudrate);

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
  //Se solicita una identificacion
  while (Serial.available() > 0)  Serial.read();
  ask4id();
  //Envio de posicion actual
  SendPositions(X, Y, IDbot, 3);
  delay(1300);
  SendPositions(X, Y, IDbot, 4);
  //Se pide un objetivo
  ask4tgetsid(IDbot,5);
  delay(1600);
  ask4tgetsid(IDbot,6);
  //Calculo de la Id de compañero
  Idcompanero=companero(IDbot,1);//0=Linea, 1=figura centrica  
  //esperar al permiso
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

delay_4_part(int (IDbot), int(Idcompanero));
}

bool intget=false;

void loop() {
  //el valor actual de tiempo
  long currenttime = millis();
// Pedir permiso para empezar a moverse

    // Parte de movimiento
  if (((currenttime - lasttime)>espera1) && (!intget)){
    //dist1cm = medirdist(trigPin1, echoPin1);
    //dist2cm = medirdist(trigPin2, echoPin2);
    //dist3cm = medirdist(trigPin3, echoPin3);
    
    odometria(vmot1, vmot2, X, Y, xthe,ythe);

    if (dist2cm < 15.0f)
   {
      if (dist1cm < 13.0f)
     {
      vmot1=400;
      vmot2=130;
      }
      if (dist3cm < 13.0f)
     {
      vmot1=130;
      vmot2=400;
      }
      else{
      vmot1=450;
      vmot2=120;
      }
      espera1=100;
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
  if (Statecom == 1){
    Ask4allpositions(Statecom);
    lastcom = millis();
    Statecom = 2;
    incommingmsg=1;
  }

  if ((Statecom == 2) and ((millis() - lastcom) > 1500))
  {

    Ask4allpositions(Statecom);
    Statecom = 3;
    incommingmsg=0;
  }
  
  if (Statecom == 3)
    {
      SendPositions(X, Y, IDbot, Statecom);
      lastcom = millis();
      incommingmsg=1;
      Statecom = 4;
    }
    
  if ((Statecom == 4) && ((millis() - lastcom) > 1300))
  {
    SendPositions(X, Y, IDbot, Statecom);
    
    if(intget){
      Statecom = 5;   
    }
    else{
      Statecom=1;
    }
      
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
    if (Statecom == 5)
  {
    forma(Idcompanero); 
    intget=!intget; 
    Statecom=1;
  }



  if ((P <= 10))
  {
    intget=!intget;
    motor(20,20);
  }

}
