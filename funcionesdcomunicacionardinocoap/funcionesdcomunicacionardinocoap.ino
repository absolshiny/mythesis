#include "SoftwareSerial.h"
#include <math.h>
#include <stdio.h>

SoftwareSerial ESP1 (2,3);

float X = 20.2;
float Y= 300.3;

float Xtget = 0;
float Ytget= 0;

int16_t IDbot;

const long unsigned int baudrate = 9600;


struct puntos{
  int x[6];
  int y[6];
};

puntos posi;

void setup() {
  Serial.begin(baudrate);
  ESP1.begin(baudrate);
  while (ESP1.available()>0)  ESP1.read();
    ask4id();
  Serial.print(IDbot);
  Serial.println();
  //delay(1000);
  ask4tgets();
  Serial.print(Xtget);
  Serial.print(" : ");
  Serial.print(Ytget);
  Serial.println();
}

void loop() {
SendPositions(X,Y, IDbot);
for (int i=2; i<7;i++)
{
  SendPositions(X,Y,i);
}

Ask4allpositions();

Serial.print(posi.x[3]);
Serial.print("::");
Serial.println(posi.y[3]);

delay(200);
}
