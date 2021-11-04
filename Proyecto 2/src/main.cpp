//***************************************************************************************************************
// Universidad del Valle de Guatemala 
// BE3015 - Electrónica Digital 2
// Jeremy Mejía 
// Proyecto 2
// Arduino
// Carné: 19846
//***************************************************************************************************************

//***************************************************************************************************************
// Librerías
//***************************************************************************************************************

#include <Arduino.h>

//***************************************************************************************************************
// Definición de Pines
//***************************************************************************************************************

//Pin del mic
#define Mic 13

#define led1 2
#define led2 4
#define led3 16
#define led4 17
#define led5 5

//***************************************************************************************************************
// Prototipo de Funciones
//***************************************************************************************************************

//***************************************************************************************************************
// Variables Globales
//***************************************************************************************************************
int voltaje; 

//***************************************************************************************************************
// Configuración
//***************************************************************************************************************


void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);

  pinMode(Mic, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);
  
}

//***************************************************************************************************************
// Loop Principal
//***************************************************************************************************************

void loop() {
  voltaje = analogRead(Mic);
  //voltaje = map(voltaje, 0, 4095, 1, 10); 
  Serial.println(voltaje);
  
  Serial2.write(voltaje);  

  if (voltaje > 20 && voltaje <=25){

    digitalWrite(led1, HIGH);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
  }

  else if (voltaje > 25 && voltaje <= 30){

    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
  }

  else if (voltaje > 30 && voltaje <= 35){

    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
  }

  else if (voltaje > 35 && voltaje <= 40){

    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, LOW);
  }

  else if (voltaje > 40 && voltaje <= 42){

    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  }
  delay(20);

}