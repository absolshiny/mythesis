  float trmb (float x, float a, float b, float c)
{
  float x1= (x-a)/(b-a);
  float x2= (c-x)/(c-b);
  float ux=max (min(x1,x2),0);
  return ux;
 }
float trap (float x, float a, float b, float c, float d)
{
  float x1= (x-a)/(b-a);
  float x2= (d-x)/(d-c);
  float ux=max (min(x1,min(1,x2)),0);
  return ux;
 }
void movfuzzycator(float teta, float pf)
{
  //----------------------------------------
  //-----********Fuzzificacion*********----//:
  //----------------------------------------
  // valores de pertenencia para distancia
  //----------------------------------------
  float uvn = trap(pf,-10,-3,8,9.2); //Very Near
  float une = trap(pf,8,9.2,30,33); // Near
  float ufa = trap(pf,30,33,600,800);// Far
  //----------------------------------------
  // valores de pertenencia de los angulos  
  //----------------------------------------
  float ute0 = trmb(teta,-0.1*PI_2,0,0.1*PI_2);//--------------teta cero
  float uten = trap(teta,-3*PI_2,-2*PI_2,-0.2*PI_2,-0.09*PI_2); // teta negativo
  float utep = trap(teta,0.09*PI_2,0.2*PI_2,2*PI_2,3*PI_2);//---------------teta positivo
  //----------------------------------------
  float R1=uvn;
  //float R2=min(une,ute0);
  //float R3=min(ufa,ute0);
  float R2= ute0;
  float R3=R2;
  float R4=min(une,uten);
  float R5=min(une,utep);
  float R6=min(ufa,uten);
  float R7=min(ufa,utep);

  // valor esperado de conjuntos de salida
  int vsl=80;
  int me=210;
  int fas=370;
  // defuzzificacion
  // valores de pertenencia conjuntos izquierda
  float uvsll=max(R1,R5);
  float umel=max(R7,max(R2,R4));
  float ufasl=max(R3,R6);
  float s_us_l=uvsll+umel+ufasl;
  // valores de pertenencia conjuntos derecha
  float uvslr=max(R1,R4);
  float umer=max(R6,max(R2,R5));
  float ufasr=max(R3,R7);
  float s_us_r=uvslr+umer+ufasr;
  
  int Dl=vsl*uvsll+me*umel+fas*ufasl;
  int Dr=vsl*uvslr+me*umer+fas*ufasr;
  vmot1=round(Dl/s_us_l);
  vmot2=round(Dr/s_us_r);
  return;
}

void evafuzzycation (uint8_t si, uint8_t sc, uint8_t sd)
{
  //----------------------------------------
  // valores de pertenencia sensor central
  //----------------------------------------
  float uonc = trap(sc,0.1,2,10,12); // objective near centre
  float uofc = trap(sc,10,20,300,400); // objective near centre
  //----------------------------------------  
  // valores de pertenencia del sensor derecho
  //----------------------------------------
  float uspd = trap (sd,0,2,10,12);// objective near en sensor derecho
  //----------------------------------------  
  // valores de pertenencia del sensor izquierdo
  //----------------------------------------
  float uspi = trap (si,0,2,10,12);// objective near en sensor izquierdo

  float R1= uofc;
  float R2= min(uonc,uspd);
  float R3= min(uonc,uspi);
//  float R5= min(umd,umi);
//  float R6= min(umd,ufi);
//  float R7= min(ufd,uni);
//  float R8= min(ufd,umi);
//  float R9= min(ufd,ufi);
//// defuzificacion
//// sets de salida
  int forw= 500;
  int forwg= 300;
  int backg=-300;
//  float nSl=260;
//  float Md=570;
//  float Fa=700;
//// pertenencia de casa salida:
//// derecha
  float uforwr=R1;
  float ufogr=R2;
  float ubackgr=R3;
//  float unSlr=(R2);
//  float uMdr=(R8);
//  float uFar=max(R5,max(R6,R9));
  float susr=uforwr+ufogr+ubackgr;
//// izquierda
  float uforwl=R1;
  float ufogl=R3;
  float ubackgl=R2;
//  float uMdl=max(R5,R6);
//  float uFal=max(R8,R9);
  float susl=uforwl+ufogl+ubackgl;
//
  int Dr=forw*uforwr+forwg*ufogr+backg*ubackgr; 
  int Dl=forw*uforwl+forwg*ufogl+backg*ubackgl;
  vmot1=round(Dl/susl);
  vmot2=round(Dr/susr);
  return; 
}
