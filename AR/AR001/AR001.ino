#include "HX711.h"

HX711 balanca;

int sck, dt;
float calibra; 
float peso, newton;
float armz[100];
bool enc; 
int i, itemp;

void setup(){
  Serial.begin(9600);
  i = 0;
  itemp = 0;
  sck = 2; 
  dt = 3; 
  calibra = 130000.00;
  balanca.begin(dt, sck);
  balanca.set_scale(calibra);
  balanca.tare();
  long zero_factor = balanca.read_average();
}

void loop(){
  if(enc == false){
    peso = balanca.get_units(), 10;
    newton = Calc(peso); 
    Serial.println(newton); 
    GravTemp(newton);
    delay(700); 
  }else{
    finish(); 
  }
}

float Calc(float Peso){
  float Newton = Peso * 9.81;
  return Newton;
}

void GravTemp(float Newton){
  armz[i] =  Newton; 
  if(armz[i] == armz[i-1]){
    itemp++; 
    if(itemp == 10){
      enc = true; 
    }
  }else{
    itemp = 0; 
  }
  i++;
}

void finish(){
  //finish ainda não implementado, a finalização lista tudo
  //teste se a função exit(0) encerra o arduino
  Serial.println("Começou");
  int ifinal = i;
  for(i=0;i<=ifinal;i++){
    Serial.println(armz[i]);
  }
  Serial.println("Terminou");
}
