// Test motor wheel mode

#include "DynamixelMotor.h"
#include "SoftwareSerial.h"

// id of the motor
const uint8_t id1=10;
const uint8_t id2=16;
// speed, between -1023 and 1023
int16_t velocity=400;
// communication baudrate
const long unsigned int baudrate = 9600;

// hardware serial without tristate buffer
// see blink_led example, and adapt to your configuration
#define SOFT_RX_PIN 4
#define SOFT_TX_PIN 5
SoftwareDynamixelInterface interface(SOFT_RX_PIN, SOFT_TX_PIN);
    
DynamixelMotor motor1(interface, id1);
DynamixelMotor motor2(interface, id2);

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

  motor1.enableTorque();  
  motor1.wheelMode();
  motor2.enableTorque();  
  motor2.wheelMode();
}

//change motion direction every 5 seconds
void loop() 
{
  motor1.speed(velocity);
  delay(7);
  motor2.speed(-velocity);
  velocity=-velocity;
  delay(500);
}
