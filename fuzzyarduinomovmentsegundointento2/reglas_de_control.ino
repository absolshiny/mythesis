
void cinematica20(float xr, float yr, float xt,float yt)
{
  float beta;
  P= sqrt(pow((xt-xr),2)+pow((yt-yr),2));
  //beta=atan2((yt-yr),(xt-xr));
  //alpha=beta-theta;
  float XL=cos(theta)*(xt-xr)+sin(theta)*(yt-yr);
  float YL=-sin(theta)*(xt-xr)+cos(theta)*(yt-yr);
  alpha=atan2(YL,XL);
  return;
}

void tangyang(float alpa, float Pet) 
{ 
  
  if (abs(alpa)>1)
  {
    v=0;
    w=alpa*1.7;  
//    Serial.println("-----------corregir angulo");

    
  }
  else
  {
    v=13;
    w=0;
//   Serial.println("----------AVANZAR-");

  }
//  Serial.println("v y w  y alpha valores: ");
//  Serial.print(v);
//  Serial.print(" ");
//  Serial.print(w);
//  Serial.print("                ");
//  Serial.println(alpa);
  return;
}


void odometria(int v_mot1, int vmot_2, float xold, float yold, float tethaold)
{
  
  float Ss;
  float Ste;
//  float er = (((-0.216+(0.004*v_mot1))*2.0*PI_2)*(millis()-lastcalculo)/1000);
//  float el = (((-0.216+(0.004*vmot_2))*2.0*PI_2)*(millis()-lastcalculo)/1000);
  float el = (((-0.280+(0.006*v_mot1)))*(millis()-lastcalculo)/1000);
  float er = (((-0.280+(0.006*vmot_2)))*(millis()-lastcalculo)/1000);
  lastcalculo=millis();
  Ss=(r/2)*(er+el);
  Ste=(r/d)*(er-el);
  //Serial.println("Delta s Delta teta");
  //Serial.print(Ss);
  //Serial.print("   ");
  //Serial.println(Ste);
  X= xold+Ss*cos(tethaold);
  Y= yold+Ss*sin(tethaold);
  theta=tethaold+Ste;
  theta=corregirangulo(theta);
  return;
}


//void velocidades (float vel , float wvel)
//{
// wr=(2*vel+wvel*(d))/2*(r);
// wl=(2*vel-wvel*(d))/2*(r);
// return;
//}
//
//void velocidades20(float vel , float wvel, float ang)
//{
//  wr=vel*pow(cos(ang),2)/(r)+vel*(pow(sin(ang),2))/(r) + (d)*wvel/(r);
//
// wl=vel*pow(cos(ang),2)/(r)+vel*(pow(sin(ang),2))/(r) - (d)*wvel/(r);
//  return;
//}

//void movings(float wlt, float wrt)
//{
//  float rpml=wlt*60/(2*PI_2);
//  float rpmr=wrt*60/(2*PI_2);
//  //vmot1=map(wl2,-57,57,-511,511);
//  //vmot2=map(wr2,-57,57,-511,511);
//  vmot1=rpml/0.1114369;
//  vmot1=round(vmot1);
//  vmot2=rpmr/0.1114369;
//  vmot2=round(vmot2);
//  return;
//}

float corregirangulo(float angulo)
{
  angulo=fmod(angulo+PI_2,(2*PI_2));
  if (angulo <0)
  {
    angulo+=2*PI_2;
  }
  return angulo-PI_2;
}
