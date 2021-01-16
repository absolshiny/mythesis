#include <DynamixelMotor.h>
#include "SoftwareSerial.h"
#include <math.h>
#include <stdio.h>

#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"


#define PI_2 3.1415926535897932
#define uint8_t byte 

int Permiso=0;

float X = 17;
float Y = 90;
float xthe=1;
float ythe=0;

float Xtget = 0;
float Ytget = 0;

long espera1 = 300;
long lasttime;
long lastcalculo;
uint8_t id1=9; //izq 
uint8_t id2=4; //der
int ini_val=4;
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

int spd;
float wl;
float wr;
const float d = 13;
float r = 2.5;
float theta = 0;
float P=100, alpha;
int vmot1 = 0;
int vmot2 = 0;
int incommingmsg=0;
const int trigPin1 = 6;    // Trigger
const int echoPin1 = 7;    // Echo izquierdo
const int trigPin2 = 8;
const int echoPin2 = 9;     // Echo Centro
const int trigPin3 = A0;
const int echoPin3 = A1;    // Echo derecho
int dist1cm=100, dist2cm=100, dist3cm=100;

const int mpuAddress = 0x68;  // Puede ser 0x68 o 0x69
MPU6050 mpu(mpuAddress);
 
int ax, ay, az;
int gx, gy, gz;
int bdgz;
long tiempo_prev, dt;
float girosc_ang_z;// girosc_ang_y;
float girosc_ang_z_prev;//, girosc_ang_y_prev;


bool pr_vez=true;
long e_mov_d;

void updateGiro(int8_t still)
{
   dt = millis() - tiempo_prev;
   tiempo_prev = millis();
  if (still==1){
    bdgz=gz;
  }
  else{
    float tep;
   //girosc_ang_x = (gx / 131)*dt / 1000.0 + girosc_ang_x_prev;
   //girosc_ang_y = (gy / 131)*dt / 1000.0 + girosc_ang_y_prev;
   tep = (((gz-bdgz) / 131)*dt / 1000.0)*(PI/180)+ girosc_ang_z_prev;
   girosc_ang_z=atan2(sin(tep),cos(tep));
   //girosc_ang_x_prev = girosc_ang_x;
   //girosc_ang_y_prev = girosc_ang_y;
   girosc_ang_z_prev = girosc_ang_z;
  }
}




void setup() {
  // iniciamos el Serial, la interface con los motores, y el modulowifi
  Serial.begin(19200);
  setinicio(int (ini_val)); // Inicializar el robot acorde al numero de este
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
  //-----imu
   Wire.begin();
   mpu.initialize();
  //Se solicita una identificacion
  while (Serial.available() > 0)  Serial.read();
  ask4id();
  //Envio de posicion actual
  SendPositions(X, Y, IDbot, 3);
  delay(1300);
  SendPositions(X, Y, IDbot, 4);
  //Calculo de la Id de compañero
  Idcompanero=companero(IDbot,0);//0=Linea, 1=figura centrica  
  //Se pide un objetivo
  ask4tgetsid(int(IDbot),5);
  delay(1500);
  ask4tgetsid(int(IDbot),6);
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
  if (((millis() - lasttime)>espera1) && (!intget)){
    //dist1cm = medirdist(trigPin1, echoPin1);
    dist2cm = medirdist(trigPin2, echoPin2);
    //dist3cm = medirdist(trigPin3, echoPin3);
    mpu.getRotation(&gx, &gy, &gz);
    updateGiro(0);
    odometria(vmot1, vmot2, X, Y, xthe,ythe);

    if (dist2cm < 20)
   {
      if (dist1cm < 18)
     {
      vmot1=300;
      vmot2=20;
      }
     else if (dist3cm < 18)
     {
      vmot1=20;
      vmot2=300;
      }
      else{
      vmot1=300;
      vmot2=120;
      }
      espera1=40;
    }
   else
    {
      cinematica20 (X, Y, Xtget, Ytget);
      

      movfuzzycator(alpha, P);
      espera1 =40;
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

  if ((Statecom == 2) and ((millis() - lastcom) > 1400))
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



  if (P <= 12.0){
    //intget=!intget;
    motor(20,20);
    mpu.getRotation(&gx, &gy, &gz);
    updateGiro(1);
    while (true){
      delay(10);
    }
  }

}
