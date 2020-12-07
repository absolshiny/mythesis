String ReadString = "";
boolean aviso = false;

void ask4permission(){
  resetincome();
  Serial.write("5,1,100,100");
  delay(1500);
  respuesta();
  char data[4];
  ReadString.toCharArray(data,4);
  Permiso=uint8_t(atoi(data));
  return;
}

void ask4id()
{
  resetincome();
  Serial.write("1,2,2.444,2.444");
  delay(1500);
  respuesta();
  char data[4];
  ReadString.toCharArray(data, 4);
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
    aviso = true;
  }
  return;
}

void ask4tgets() {
  resetincome();
  Serial.write("2,2,2.444,2.444");
  delay(1500);
  respuesta();
  char data[12];
  ReadString.toCharArray(data, 12);
   Recuperartgets(data);
   return;
}
void Recuperartgets(char data[12]){
  char bfferX[4];
  char bfferY[4];
  int8_t cont=1;
  for (int i = 0; i < 4; i++)
  {
    if (data[cont]==','){
      cont+=1;
      bfferX[i]=".";
      break;
    }
    
    bfferX[i] = data[cont];
    cont+=1;
  }
    for (int i = 0; i < 4; i++)
  {
    if (data[cont]==',' or data[cont]=='}'){
      cont+=1;
      bfferY[i]=".";
      break;
    }
    
    bfferY[i] = data[cont];
    cont+=1;
  }
  Xtget = float(atof(bfferX));
  Ytget = float(atof(bfferY));
  return;
}

void ask4tgetsid(int( id), int( comstat)) {
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

void SendPositions(float (px) , float (py) , int (id), int (comstat))
{
  if (comstat ==3)
  {
  resetincome();
  char pload[22];
  ReadString = "3," + String(id) + "," + String(px, 1) + "," + String(py, 1) + ",";
  ReadString.toCharArray(pload, 22);
  //Serial.print(pload);
  Serial.write(pload);
  return;
  }
  if (comstat==4)
  {
    respuesta();
    //Serial.print(ReadString);
    //Serial.println();
  }
  return;
}

void Ask4allpositions(int comstat)
{

  if (comstat==1){
    resetincome();
    Serial.write("4,1,20.00,30.00");
  }
  if (comstat==2){
    respuesta();
  
  char data[110];
  ReadString.toCharArray(data, 110);
  int rding = 1;
  for (int i = 0; i < 6; i++)
  {
    char xt[7];
    char yt[7];
    for (int c = 0; c < 7; c++)
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

    for (int c = 0; c < 7; c++)
    {
      if (data[rding] == ',' or data[rding]=='}')
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
  X=float(posi.x[(IDbot-1)]);
  Y=float(posi.y[(IDbot-1)]);
  }
  return;
}
