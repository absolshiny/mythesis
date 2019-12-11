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
void movfuzzycator(float ean, float Pf)
{
  // fuzzificacion:
  // valores de pertenencia para distancia
  float uzd = trmb(Pf,-30,0,20);
  float und = trmb(Pf,-5,25,50);
  float umd = trmb(Pf,25,50,75);
  float ufd = trmb(Pf,50,75,100);
  float uvfd = trap(Pf,75,100,150,200);
  // valores de pertenencia de las distancias
  
  float ubna = trmb(ean,-1.5*PI_2,-1*PI_2,-0.5*PI_2);
  float una = trmb(ean,-1*PI_2,-0.5*PI_2,0*PI_2);
  float uza=trmb(ean,-0.5*PI_2,0*PI_2,0.5*PI_2);
  float upa=trmb(ean,0*PI_2,0.5*PI_2,1*PI_2);
  float ubpa=trmb(ean,0.5*PI_2,1*PI_2,1.5*PI_2);
  // reglas de pertenencia ruedas:
  float R1=min(ubna,uzd);
  float R2=min(ubna,und);
  float R3=min(ubna,umd);
  float R4=min(ubna,ufd);
  float R5=min(ubna,uvfd);
  float R6=min(una,uzd);
  float R7=min(una,und);
  float R8=min(una,umd);
  float R9=min(una,ufd);
  float R10=min(una,uvfd);
  float R11=min(uza,uzd);
  float R12=min(uza,und);
  float R13=min(uza,umd);
  float R14=min(uza,ufd);
  float R15=min(uza,uvfd);
  float R16=min(upa,uzd);
  float R17=min(upa,und);
  float R18=min(upa,umd);
  float R19=min(upa,ufd);
  float R20=min(upa,uvfd);
  float R21=min(ubpa,uzd);
  float R22=min(ubpa,und);
  float R23=min(ubpa,uzd);
  float R24=min(ubpa,ufd);
  float R25=min(ubpa,uvfd);
  // valor esperado de conjuntos de salida
  float vsl=150;
  float sl=290;
  float me=370;
  float fa=490;
  float vfa=570;
  // defuzzificacion
  // valores de pertenencia conjuntos izquierda
  float uvsll=(max(R11,R16),max(max(R17,R18),max(R21,R22)));
  float usll=(max(max(R6,R9),(max(R12,R16),max(R17,R20))));
  float umel=(max(max(R1,R2),max(max(R8,R13),max(R24,R25))));
  float ufal=max(max(R3,R4),max(R9,R14));
  float uvfal=max(max(R5,R10),R15);
  float s_us_l=uvsll+usll+umel+ufal+uvfal;
  // valores de pertenencia conjuntos derecha
  float uvslr=max(max(R1,R2),max(max(R6,R7),max(R8,R11)));
  float uslr=max(max(R3,R9),max(max(R10,R12),max(R16,R17)));
  float umer=max(max(R4,R5),max(max(R13,R18),max(R21,R22)));
  float ufar=max(max(R14,R19),max(R23,R24));
  float uvfar=max(R19,max(R20,R25));
  float s_us_r=uvslr+uslr+umer+ufar+uvfar;
  
  float Dl=vsl*uvsll+sl*usll+me*umel+fa*ufal+vfa*uvfal;
  float Dr=vsl*uvslr+sl*uslr+me*umer+fa*ufar+vfa*uvfar;
  vmot1=round(Dl/s_us_l);
  vmot2=round(Dr/s_us_r);
  return;
}

void evafuzzycation (float dd, float di)
{
  
  float und=trap(dd,0.3,2,5,13);
  float umd=trap(dd,7,15,21,32);
  float ufd=trap(dd,19,40,100,200);
  
  float uni=trap(di,-3,2,8,13);
  float umi=trap(di,9,15,21,32);
  float ufi=trap(di,19,40,100,200);

  float R1= min(und,uni);
  float R2= min(und,umi);
  float R3= min(und,ufi);
  float R4= min(umd,uni);
  float R5= min(umd,umi);
  float R6= min(umd,ufi);
  float R7= min(ufd,uni);
  float R8= min(ufd,umi);
  float R9= min(ufd,ufi);
// defuzificacion
// sets de salida
  float nFa=-400;
  float nMd=-295;
  float nSl=260;
  float Md=570;
  float Fa=700;
// pertenencia de casa salida:
// derecha
  float unFar=max(R1,max(R4,R7));
  float unMdr=(R3);
  float unSlr=(R2);
  float uMdr=(R8);
  float uFar=max(R5,max(R6,R9));
  float susr=unFar+unMdr+unSlr+uMdr+uFar;
// izquierda
  float unFal=max(R1,max(R2,R3));
  float unMdl=(R7);
  float unSll=(R4);
  float uMdl=max(R5,R6);
  float uFal=max(R8,R9);
  float susl=unFal+unMdl+unSll+uMdl+uFal;

  float Dr=nFa*unFar+nMd*unMdr+nSl*unSlr+Md*uMdr+Fa*uFar; 
  float Dl=nFa*unFal+nMd*unMdl+nSl*unSll+Md*uMdl+Fa*uFal;
  vmot1=round(Dl/susl);
  vmot2=round(Dr/susr);
  return; 
}
