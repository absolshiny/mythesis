String ReadString="";
boolean aviso=false;

uint8_t ask4id()
{
  resetincome();
  while (!aviso)
  {
  ESP1.write("1,0,0.000,0.000");
  delay(30);
  respuesta();
  }
  char data[2];
  ReadString.toCharArray(data,2);
  return (int)data[0]-48;
}

void resetincome()
  {
  ReadString="";
  aviso=false;
  return;
  }
  
void respuesta()
{
  while (ESP1.available()){
    ReadString=ESP1.readString();
    aviso=true;
  }
  return;
}

void ask4tgets(){
  resetincome();
  while (!aviso)
  {
  ESP1.write("2,0,0.000,0.000");
  delay(200);
  respuesta();
  }
  char data[9];
  ReadString.toCharArray(data,9);
      char bfferX[4];
      char bfferY[4];
  for (int i = 0; i<4; i++)
  {
    bfferX[i]=data[i+1];
    bfferY[i]=data[i+5];
  }
  Xtget=atof(bfferX);
  Ytget=atof(bfferY);
  return;
  }
void SendPositions(float px, float py, int8_t id)
{
  resetincome();
  char pxc[5];
  char pyc[5];
  char pload[15];
  dtostrf(px,5,3,pxc);
  dtostrf(py,5,3,pyc);
  ReadString="3,"+String(id)+","+pxc+","+pyc;
  ReadString.toCharArray(pload,15);
  while (!aviso)
  {
  ESP1.write(pload);
  delay(100);
  respuesta();
  }
  return;
}

void Ask4allpositions()
{
  resetincome();
  while (!aviso)
  {
  ESP1.write("4,1,0.000,0.000");
  delay(100);
  respuesta();
  }
  char data[48];
  ReadString.toCharArray(data,48);
  int8_t rding=1;
  for (int i=0;i<6;i++)
  {
    char xt[3];
    char yt[3];
    for (int c=0;c<3;c++)
    {
      xt[c]=data[c+rding];
    }
    rding+=4;
    for (int c=0;c<3;c++)
    {
      yt[c]=data[c+rding];
    }
    rding+=4;
    
    posi.x[i]=atof(xt);
    posi.y[i]=atof(yt);
  }
  return;
}
