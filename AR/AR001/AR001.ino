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
  calibra = 200000.00;
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
    GravTemp(newton);
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
  armz[i] =  Newton; 
  if(Arredondar(armz[i]) == Arredondar(armz[i-1])){ //Solução temp pois não esta encerrando corretamzaente, pois float tem 7 casas decimais, tentando filtrar para duas sem sucesso;  
    itemp++; 
    Serial.println(Arredondar(armz[i-1]));
    if(itemp == 10){
      enc = false; 
    }
  }else{
    itemp = 0; 
  }
  i++;
}

float Arredondar(float armz){
    float myFloat = armz;
    int myInt = round(myFloat);
    return myInt;
}

void finish(){
  Serial.println("Terminou");
  for (i = 0 ; i <= 100 ; i++){
    Serial.println(armz[i]);
  }
  delay(1000);
  exit(0);
}
