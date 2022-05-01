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
  calibra = 1600000.00;
  balanca.begin(dt, sck);
  balanca.set_scale(calibra);
  balanca.tare();
  long zero_factor = balanca.read_average();
}

void loop(){
  if(enc == false){
    peso = balanca.get_units(), 10;
    if(peso < abs(0)){
      peso = 0; 
    }
    newton = Calc(peso); 
    Serial.println(newton); 
    Serial.print("LOG: ");
    Serial.println(itemp);
    GravTemp(newton);
    Serial.println();
    Serial.println();
    Serial.println();
    delay(1000); 
  }else{
    finish(); 
  }
}

float Calc(float Peso){
  float Newton = Peso * 9.81;
  return Newton;
}

void GravTemp(float Newton){
  Serial.println("Gravação");
  armz[i] =  Newton; 
  if(armz[i]< 0.05){ //Solução temp pois não esta encerrando corretamente, pois float tem 7 casas decimais, tentando filtrar para duas sem sucesso;  
    Serial.print("Entrou, log =");
    Serial.println(armz[i-1]); 
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
  Serial.println("Terminou");
  delay(1000);
  exit(0);
}
