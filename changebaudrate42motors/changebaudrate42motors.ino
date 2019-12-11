// Demonstrate how to change the communication baudrate of a motor
// This can be usefull if you want to reduce baudrate, in order to
// use a software serial, or if you need 2Mbaud for high speed communication

#include "DynamixelMotor.h"

// motor id
const uint8_t id=4;
const uint8_t id2=11;

const unsigned long baudrate_before=1000000;
const unsigned long baudrate_after=9600;

// hardware serial without tristate buffer
// see blink_led example, and adapt to your configuration
HardwareDynamixelInterface interface(Serial);

DynamixelMotor motor(interface, id);
DynamixelMotor motor1(interface, id2);

// the led should blink twice
void setup() 
{
  // start communication at old baudrate
  interface.begin(baudrate_before);
  delay(100);
  
  // make the motor led blink once
  motor.init();
  motor1.init();
  motor1.led(HIGH);
  motor.led(HIGH);
  delay(1000);
  motor1.led(LOW);
  motor.led(LOW);
  delay(1000);
  
  // set motor baudrate to new baudrate
  motor.communicationSpeed(baudrate_after);
  motor1.communicationSpeed(baudrate_after);
  // set arduino baudrate at new baudrate
  delay(100);
  interface.begin(baudrate_after);
  
  // make the motor led blink once
  motor1.led(HIGH);
  motor.led(HIGH);
  delay(1000);
  motor1.led(LOW);
  motor.led(LOW);
  delay(1000);
  
}

void loop(){}
