#include <SoftwareSerial.h>
SoftwareSerial ESP1(2, 3); // RX, TX
 char pload[16];
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  ESP1.begin(9600);
  Serial.println("Empecemos");
  String strpload = "1,4,2.555,3.555\n";
  strpload.toCharArray(pload,16);
  Serial.println("Enviando: ");
  Serial.println(strpload);
  ESP1.write("1,4,2.555,3.555");
}
boolean pe=false;
String ReadString="";
void loop() {
  // put your main code here, to run repeatedly:
  while (ESP1.available()){
    ReadString=ESP1.readString();
    pe=true;
  }
  delay(1000);
  if (pe){
  Serial.print(ReadString);
  }
  delay(1000);
  Serial.println("1,4,2.555,3.555");
  ESP1.write("1,4,2.555,3.555");
}
