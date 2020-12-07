bool pr_vez=true;
long e_mov_d;
void forma(int (partner))
{
    float Dist = 45;
    float f_ang=0;
    switch (IDbot)
    {
    case 3:
      if (pr_vez){
         pr_vez=!pr_vez;
         e_mov_d=millis();
         Xtget=110;
         Ytget=45;
      }
      else if((millis()-e_mov_d)>9000){
          Xtget+=15;
          Ytget=45;
      }
      break;
    default:
      if (IDbot < 3){
              f_ang=-float(PI_2*2/3);
            }
       else {
              f_ang=-float(PI_2*4/3);
            }
       int index = Idcompanero-1;
      Xtget=float((posi.x[index])+(cos(f_ang)*Dist));
      Ytget=float((posi.y[index])+(sin(f_ang)*Dist));
      break;
    }

}

int delay_4_part(int (IDbot), int(IDcomp)){
  bool test=true;
  while(test)
     {
      Ask4allpositions(1);
      delay(1500);
      Ask4allpositions(2);
      int index= Idcompanero-1;
      float dist_part= sqrt(pow((posi.x[index]-X),2)+pow((posi.y[index]-Y),2));
      int ifzero=0;
      if ((dist_part>=50.0f) || (IDcomp==ifzero))
        {
          test=!test;
        }
    }
  return 1;
}

int companero(int ID,int forma)
{
  int Idpart;
  switch(forma)
  {
    case 0: //Es una linea
    if (ID==6){
     Idpart=0;
    }
    else{
    Idpart=(ID+1);
    }
    break;
    case 1: //Es una figura con centro
     if (ID==3){
     Idpart=0;
    }
    else{
      if (ID<3){
        Idpart=ID+1;
      }
      else{
        Idpart=(ID-1);
      }
    }
    break;
  }
  return Idpart;
}
