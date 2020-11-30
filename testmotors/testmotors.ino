// Test motor wheel mode

#include "DynamixelMotor.h"

// id of the motor
const uint8_t id1=1;
const uint8_t id2=7;
// speed, between -1023 and 1023
int16_t speed=550;
// communication baudrate
const long unsigned int baudrate = 9600;

// hardware serial without tristate buffer
// see blink_led example, and adapt to your configuration
#define SOFT_RX_PIN 3

#define SOFT_TX_PIN 5
SoftwareDynamixelInterface interface(SOFT_RX_PIN, SOFT_TX_PIN);
DynamixelMotor motor1(interface, id1);
DynamixelMotor motor2(interface, id2);
DynamixelDevice readdevice1(interface, id1);
DynamixelDevice readdevice2(interface, id2);


void setup()
{ 
  interface.begin(baudrate);
  delay(100);
  
  // check if we can communicate with the motor
  // if not, we turn the led on and stop here
  uint8_t status=motor1.init();
  if(status!=DYN_STATUS_OK)
  {
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);
    while(1);
  }
  interface.begin(baudrate);
  motor1.enableTorque();  
  motor1.wheelMode();
  motor2.enableTorque();  
  motor2.wheelMode();
}

//change motion direction every 5 seconds
void loop() 
{
  motor1.speed(speed);
  motor2.speed(-speed);
  //speed=-speed;
  delay(5000);
}
