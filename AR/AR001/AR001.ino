#include <SPI.h>
#include <SD.h>
#include <HX711.h> 

File myFile;
HX711 balanca;

#define sck 2
#define dt 3
#define calibracao 185000
#define tamArray 120

float peso, newton;
float armz[120];
bool enc; 
int i, itemp, in;

void setup(){
  Serial.begin(9600);
  SD.begin(4);
  i = 0;
  itemp = 0;
  balanca.begin(dt, sck);
  balanca.set_scale(calibracao);
  balanca.tare();
  long zero_factor = balanca.read_average();

  myFile = SD.open("valid.txt", FILE_WRITE);
  if(myFile){
    Serial.println("o modulo sd iniciou");
    myFile.println("Dados:");
  }else{
    Serial.println("modulo nao iniciou");
  }
  myFile.close();
}

void loop(){
  if(!enc){
    peso = balanca.get_units(4); 
    newton = Calc(peso);
    Serial.println(newton);
    GravTemp(newton);
    delay(500); 
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
  if(dif() || (armz[i] <= 0.1)){
    itemp++; 
    if(itemp == 20){
      enc = true; 
    }
  }else{
    itemp = 0; 
  }
  i++;
}

bool dif(){  
  if(((armz[(i-1)]* 1.1) > armz[i]) && ((armz[(i-1)] * 0.9) < armz[i])){
    return true;
  }else{
    return false; 
  }
}

void finish(){
  myFile = SD.open("valid.txt", FILE_WRITE);
  Serial.println("Terminou");
  if (myFile){
    Serial.println("GRAVANDO");
    for (i = 0 ; i < 120; i++){
      myFile.print(armz[i]);
      myFile.print(";");
      myFile.print((float)i/2);
      myFile.println(";"); 
    }
    Serial.println("Gravou");
  } else{
    Serial.println("O cartao nao iniciou");
  }
  myFile.close();
  delay(1000);
  exit(0);
}
