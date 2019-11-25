String ReadString = "";
boolean aviso = false;

void ask4id()
{
  resetincome();
  //while (!aviso)
  //{
  ESP1.write("1,2,2.444,2.444");
  delay(1500);
  respuesta();
  //}
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
  while (ESP1.available()) {
    ReadString = ESP1.readString();
    //Serial.print(ReadString);
    //Serial.println();
    aviso = true;
  }
  return;
}

void ask4tgets() {
  resetincome();
  ESP1.write("2,2,2.444,2.444");
  delay(1500);
  respuesta();
  char data[9];
  ReadString.toCharArray(data, 9);
  char bfferX[4];
  char bfferY[4];
  for (int i = 0; i < 4; i++)
  {
    bfferX[i] = data[i + 1];
    bfferY[i] = data[i + 5];
  }
  Xtget = atof(bfferX);
  Ytget = atof(bfferY);
  return;
}
void SendPositions(float px , float py , int8_t id)
{
  resetincome();

  char pload[22];
  ReadString = "3," + String(id) + "," + String(px, 1) + "," + String(py, 1) + ",";
  ReadString.toCharArray(pload, 22);
  //Serial.print(pload);
  while (!aviso)
  {
    ESP1.write(pload);
    delay(1300);
    respuesta();
    Serial.print(ReadString);
    Serial.println();
  }
  return;
}

void Ask4allpositions()
{
  resetincome();
  while (!aviso)
  {
    ESP1.write("4,1,20.00,30.00");
    delay(2000);
    respuesta();
  }
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
  return;
}
