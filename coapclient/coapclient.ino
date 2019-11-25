/*
  ESP-COAP Client
*/
#include <ESP8266WiFi.h>
#include "coap_client.h"
#include <stdlib.h>
//instance for coapclient
coapClient coap;

//WiFi connection info
const char* ssid = "Homeworld";
const char* password = "jazmin123";
//const char* ssid = "TP-LINK_60D8";
//const char* password = "18541953";
const unsigned int MAX_INPUT = 16;
int stadoact = 0;
float x = 0;
float y = 0;
//ip address and default port of coap server in which your interested in
IPAddress ip(192, 168, 100, 102); //take ETH Zurich or coap.me server to run and check client
int port = 5683;


// coap client response callback
void callback_response(coapPacket &packet, IPAddress ip, int port) {
  char p[packet.payloadlen + 1];
  memcpy(p, packet.payload, packet.payloadlen);
  p[packet.payloadlen] = NULL;

  //response from coap server
  if (packet.type == 3 && packet.code == 0) {
    Serial.println("ping ok");
  }

  Serial.write(p);
}

void setup() {

  Serial.begin(9600);

  WiFi.begin(ssid, password);
  //Serial.println(" ");

  // Connection info to WiFi network
  //Serial.println();
  //Serial.println();
  //Serial.print("Connecting to ");
  //Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    //delay(500);
    yield();
    //Serial.print(".");
  }
  //Serial.println("");
  //Serial.println("WiFi connected");
  // Print the IP address of client
  //Serial.println(WiFi.localIP());

  // client response callback.
  // this endpoint is single callback.
  coap.response(callback_response);

  // start coap client
  coap.start();

  //get request to server (arguments ip adrress of server,default port,resource(uri))
  //char msgid = coap.get(ip,port,"hello");

  //observe request (arguments ip adrress of server,deafult port,resource name,interger(0) )
  //int msgid= coap.observe(ip,port,"light",0);

  //reset observe cancel
  //int msgid=coap.observecancel(ip,port,"resoucename");

}
//int i=0;
int idbot = 1;

void loop() {
  char msgid;
  boolean state;
  String IncomingString = "";
  boolean Strirdy = false;
  while (Serial.available ()) {
    //processIncomingByte (Serial.read ());
    IncomingString = Serial.readString();
    Strirdy = true;
  }

  if (Strirdy) {
    char data[22];
    IncomingString.toCharArray(data, 22);
    stadoact = (int)data[0] - 48;
    idbot = (int)data[2] - 48;
    char bfferX[5];
    char bfferY[5];
    int c=10;
    for (int i = 0; i < 5; i++)
    {
      if (data[i + 4] == ',')
      {
        c = i + 4 + 1;
        break;
      }
      bfferX[i] = data[i + 4];
    }
    for (int i = 0; i < 5; i++)
    {
      if (data[i + c] == ',')
      {
        break;
      }
      bfferY[i] = data[i + c];
    }
    x = atof(bfferX);
    y = atof(bfferY);
    //Serial.write("Received");
  }

  switch (stadoact) {
    case 1:
      msgid = coap.get(ip, port, "ID");
      stadoact = 0;
      //Serial.println("caso1");
      break;
    case 2:
      msgid = coap.get(ip, port, "Target");
      stadoact = 0;
      //Serial.println("caso2");
      break;
    case 3:
      {
        String xposition = String(idbot) + "Position";
        char position2[11];
        xposition.toCharArray(position2, 10);
        String ploadt = "{\"x\":" + String(x) + ",\"y\":" + String(y) + "}";
        char pload[26];
        ploadt.toCharArray(pload,26);
        //Serial.println(position2);
        //Serial.println(pload);
        msgid = coap.post(ip, port, position2, pload, strlen(pload));
        stadoact = 0;
        //Serial.println("caso3");
        break;
      }
    case 4:
      msgid = coap.get(ip, port, "Allpositions");
      stadoact = 0;
      //Serial.println("caso4");
      break;

    default:
      //Serial.println("default");
      stadoact = 0;
  }
  // Requests
  // char msgid2 =coap.get(ip,port,"ID");
  //delay(500);
  //get request
  // char msgid1 = coap.get(ip,port,"hello");
  //put request
  //arguments server ip address,default port,resource name, payload,payloadlength
  //int msgid =coap.put(ip,port,"resourcename","0",strlen("0"));

  //post request
  //arguments server ip address,default port,resource name, payload,payloadlength
  //char msgid2 =coap.post(ip,port,"hello","David",strlen("David"));

  //delete request
  //int msgid = coap.delet(ip,port,"resourcename");

  //ping
  //int msgid=coap.ping(ip,port);

  // int msgid=coap.observe(ip,port,"obs",0);

  state = coap.loop();
  // Serial.print("state=");
  //Serial.println(state);
  //if(state==true)
  //i=i+1;

  //Serial.print("i=");
  //Serial.println(i);
  //if(i==3)
  //{
  //Serial.println("cancel observe");
  //coap.observeCancel(ip,port,"resourcename");
  //}

  //Serial.println(msgid);
  //delay(1000);
}

void processIncomingByte (const byte inByte)
{
  static char input_line [MAX_INPUT];
  static unsigned int input_pos = 0;

  switch (inByte)
  {

    case '\n':   // end of text
      input_line [input_pos] = 0;  // terminating null byte

      // terminator reached! process input_line here ...
      process_data (input_line);

      // reset buffer for next time
      input_pos = 0;

      break;

    case '\r':   // discard carriage return
      break;

    default:
      // keep adding if not full ... allow for terminating null byte
      if (input_pos < (MAX_INPUT - 1))
        input_line [input_pos++] = inByte;
      break;

  }  // end of switch

} // end of processIncomingByte


void process_data (const char * data)
{
  // for now just display it
  // (but you could compare it to some value, convert to an integer, etc.)

  stadoact = (int)data[0] - 48;
  idbot = (int)data[2] - 48;
  char bfferX[5];
  char bfferY[5];
  for (int i = 0; i < 5; i++) {
    bfferX[i] = data[i + 4];
    bfferY[i] = data[i + 10];
  }
  x = atof(bfferX);
  y = atof(bfferY);
  Serial.println(data[0]);
  Serial.println(stadoact);
  Serial.println(x);
  Serial.println (data);
}  // end of process_data
