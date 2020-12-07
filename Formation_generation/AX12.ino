void motor (int16_t Vmot_1, int16_t Vmot_2)
{
  motor1.speed(Vmot_1+7);
  delay(7);
  motor2.speed(-Vmot_2+7);
  return;
}

float medirdist (int trigpin, int echopin)
{
  digitalWrite(trigpin,LOW);
  delayMicroseconds(5);
  digitalWrite(trigpin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin,LOW);
  long duration=pulseIn(echopin,HIGH);
  float cm= (duration/2)/29.1;
  return cm;
}
