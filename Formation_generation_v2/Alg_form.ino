
void forma(int (partner))
{
    float Dist = 40;
    float f_ang=0;
    switch (IDbot)
    {
    case 3:
      if (pr_vez){
         pr_vez=!pr_vez;
         e_mov_d=millis();
         //Xtget=Xtget;
         //Ytget=Ytget;
      }
      else if((millis()-e_mov_d)>10000){
          Xtget=Xtget;
          Ytget=Ytget;
      }
      break;
    default:
      if (IDbot < 3){
              f_ang=float(PI_2*2/3);
            }
       else {
              f_ang=float(PI_2*4/3);
            }
      int index = Idcompanero-1;
      Xtget=float((posi.x[index])+(cos(-f_ang)*Dist));
      Ytget=float((posi.y[index])+(sin(-f_ang)*Dist));
      break;
    }

}

int delay_4_part(int (IDbot), int(IDcomp)){
  bool test=true;
  while(test)
     {
      Ask4allpositions(1);
      delay(1400);
      Ask4allpositions(2);
      int index= Idcompanero-1;
      float dist_part= sqrt(pow((posi.x[index]-X),2));
      int ifzero=0;
      if ((dist_part>=50.0) || (IDcomp==ifzero))
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
    if (ID==1){
     Idpart=0;
    }
    else{
    Idpart=(ID-1);
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

void setinicio(int robot){
  switch(robot){
    case (1):
     X=17;Y=17;r=3; spd=7;
    break;
    case (2):
    X=17;Y=41;r=3;spd=2;
    break;
    case (3):
    X=17; Y=71; r=3; spd=-15;
    break;
    case (4):
    X=17;Y=90;r=2.5; spd=0;
    break;
    case (5):
   X=17;Y=121;r=2.5;spd=+10;
    break;
    case (6):
   X=17;Y=141;r=2.5;spd=0;
    break;
    
  }
  return;
}
