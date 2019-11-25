#include <DynamixelMotor.h>
#include "SoftwareSerial.h"
#include <math.h>
#include <stdio.h>
SoftwareSerial ESP1 (2, 3);

#define PI_2 3.1415926535897932384626433832795


float X = 0;
float Y = 0;

float Xtget = 0;
float Ytget = 0;

long espera1 = 500;
long lasttime;
long lastcalculo;
const uint8_t id1 = 2;
const uint8_t id2 = 8;

int16_t IDbot;
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
float xn, yn, thetan;
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
  //Serial.print(IDbot);
  // Serial.println();
  //Se solicita un objectivo
  ask4tgets();
  
  // Serial.print(Xtget);
  //Serial.print(" : ");
  //Serial.print(Ytget);
  //Serial.println();
  delay(1000);
}

byte nvtget = 2;

void loop() {
  //el valor actual de tiempo
  long currenttime = millis();
  // Parte de movimiento
  if (currenttime - lasttime > espera1)
  {
    //Serial.println("probando parte 1");
    dist1cm = medirdist(trigPin1, echoPin1);
    dist2cm = medirdist(trigPin2, echoPin2);
    dist3cm = medirdist(trigPin3, echoPin3);
    
    odometria(vmot1, vmot2, X, Y, theta);

    //if (dist1cm < 8 or dist2cm < 10 or dist3cm<8)
   // {
//      if (dist2cm < 10)
//      {
//        if (c == 0) {
//          c = 4;
//        }
//        else {
//          c = c - 1;
//        }
//      }


      //evafuzzycation(dist1cm, dist2cm, dist3cm);
     // espera1 = 1000;
   /// }
    //else
    //{
      cinematica20 (X, Y, Xtget, Ytget);
      movfuzzycator(alpha, P);
      espera1 = 290;
    //}
    motor(vmot1, vmot2);


    lasttime = millis();
  }
  
  // Parte de comunicacion
  if (Statecom == 1)
  {
    SendPositions(X, Y, IDbot, Statecom);
    lastcom = millis();
    Statecom = 2;
  }
  if (Statecom == 2 and (millis() - lastcom) > 1500)
  {

    SendPositions(X, Y, IDbot, Statecom);
    Statecom = 3;
  }

  if (Statecom == 3)
  {
    Ask4allpositions(Statecom);
    lastcom = millis();
    Statecom = 4;
  }

  if ((Statecom == 4) and ((millis() - lastcom) > 1500))
  {

    Ask4allpositions(Statecom);
    Statecom = 1;
    //Serial.print(posi.x[0]);
    //Serial.print("::");
    //Serial.println(posi.y[0]);
  }

  //Alghoritm of movement
  if ((P < 10))
  {
  motor(20, 20);
  float oldtgetx=Xtget;
  float oldtgety=Ytget;
  delay(2000);
  while (true) 
{
   ask4tgets();
   delay(3000);
   if (not (oldtgetx==Xtget))
   {
    break;
   }
}

  }

}
