String ReadString = "";
boolean aviso = false;

void ask4permission(){
  resetincome();
  Serial.write("5,1,100,100");
  delay(1500);
  respuesta();
  char data[3];
  ReadString.toCharArray(data,3);
  Permiso=atof(data);
  return;
}

void ask4id()
{
  resetincome();
  Serial.write("1,2,2.444,2.444");
  delay(1500);
  respuesta();
  char data[3];
  ReadString.toCharArray(data, 3);
  //Serial.println(data);
  IDbot = atof(data);

  return;
}

void resetincome()
{
  ReadString = "";
  aviso = false;
  return;
}

void respuesta()
{
  while (Serial.available()) {
    ReadString = Serial.readString();
    //Serial.print(ReadString);
    //Serial.println();
    aviso = true;
  }
  return;
}

void ask4tgets() {
  resetincome();
  Serial.write("2,2,2.444,2.444");
  delay(1500);
  respuesta();
  char data[9];
  ReadString.toCharArray(data, 9);
   Recuperartgets(data);
   return;
}
void Recuperartgets(char data[9]){
  char bfferX[4];
  char bfferY[4];
  int8_t cont=1;
  for (int i = 0; i < 4; i++)
  {
    if (data[i+1]==','){
      cont+=1;
      bfferX[i]=".";
      break;
    }
    
    bfferX[i] = data[cont];
    cont+=1;
  }
    for (int i = 0; i < 4; i++)
  {
    if (data[i+1]==','){
      cont+=1;
      bfferY[i]=".";
      break;
    }
    
    bfferY[i] = data[cont];
    cont+=1;
  }
  Xtget = atof(bfferX);
  Ytget = atof(bfferY);
  return;
}

void ask4tgetsid(int8_t id, byte comstat) {
  if (comstat==5)
  {  
  resetincome();
  char pload[22];
  ReadString = "6," + String(id) + ",100,100,";
  ReadString.toCharArray(pload, 22);
  //Serial.print(pload);
  Serial.write(pload);
  return;
  }
  if (comstat==6)
  {
  respuesta();
  char data[9];
  ReadString.toCharArray(data, 9);
   Recuperartgets(data);
  return;
  }
}

void SendPositions(float px , float py , int8_t id, byte comstat)
{
  resetincome();
  if (comstat ==1)
  {
  char pload[22];
  ReadString = "3," + String(id) + "," + String(px, 1) + "," + String(py, 1) + ",";
  ReadString.toCharArray(pload, 22);
  //Serial.print(pload);
  Serial.write(pload);
  return;
  }
  if (comstat==2)
  {
    respuesta();
    //Serial.print(ReadString);
    //Serial.println();
  }
  return;
}

void Ask4allpositions(byte comstat)
{

  if (comstat==3){
    resetincome();
    Serial.write("4,1,20.00,30.00");
  }
  if (comstat==4){
    respuesta();
  
  char data[100];
  ReadString.toCharArray(data, 100);
  int rding = 1;
  for (int i = 0; i < 6; i++)
  {
    char xt[4];
    char yt[4];
    for (int c = 0; c < 3; c++)
    {
      if (data[rding] == ',')
      {
        xt[c]=".";
        rding += 1;
        break;
      }
      xt[c] = data[rding];
      rding += 1;
    }

    for (int c = 0; c < 3; c++)
    {
      if (data[rding] == ',')
      {
        yt[c]=".";
        rding += 1;
        break;
      }
      yt[c] = data[rding];
      rding += 1;
    }

    posi.x[i] = atof(xt);
    posi.y[i] = atof(yt);
  }
  }
  return;
}
