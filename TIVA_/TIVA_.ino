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

//Pantalla
#include <stdint.h>
#include <stdbool.h>
#include <TM4C123GH6PM.h>
#include "inc/hw_ints.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/debug.h"
#include "driverlib/gpio.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom_map.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"

#include "bitmaps.h"
#include "font.h"
#include "lcd_registers.h"

//SD
#include <SPI.h>
#include <SD.h>


#include "pitches.h"
//Definición de Pines
#define LCD_RST PD_0
#define LCD_CS PD_1
#define LCD_RS PD_2
#define LCD_WR PD_3
#define LCD_RD PE_1
int DPINS[] = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7};


//***************************************************************************************************************
// Definición de Pines
//***************************************************************************************************************


//Pines de los leds
#define ledR PF_1
#define ledA PF_2
#define ledV PF_3

//Botones
#define NOTE_C4_1 260
#define btn1 PF_4
#define btn2 PF_0

//***************************************************************************************************************
// Prototipo de Funciones
//***************************************************************************************************************
void LCD_Init(void);
void LCD_CMD(uint8_t cmd);
void LCD_DATA(uint8_t data);
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2);
void LCD_Clear(unsigned int c);
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c);
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c);
void LCD_Print(String text, int x, int y, int fontSize, int color, int background);

void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]);
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset);

void BTN1(void);
void BTN2(void);
void Pantalla(void);

//SD
void writeSD(void);
void readSD(void);

extern uint8_t fondo[];

//***************************************************************************************************************
// Variables Globales
//***************************************************************************************************************
//Variables del vúmetro
String T1 = "-22";
String T2 = "-16";
String T3 = "-12";
String T4 = "-6";
String T5 = "-3";
String T6 = "0";
String T7 = "3";
String T8 = "6";
String T9 = "10";
String T10 = "16";
String datodB;


File archivo; 

//Valores globales
int valor = 0; 
int dB = 0;

//Valores del Buzzer
int buzzerPin = PF_1;
int melody[] = {
   NOTE_C4_1, NOTE_D4};
   
int melody2[] = {
   NOTE_D4, NOTE_C4_1};

int noteDurations[] = {
  4, 2};

//***************************************************************************************************************
// Configuración
//***************************************************************************************************************


void setup() {
  
  Serial.begin(115200);
  Serial2.begin(115200);
  
  while (!Serial) {
    ; 
  }


  Serial.print("Initializing SD card...");
  pinMode(PA_3, OUTPUT);
  SPI.setModule(0);

  if (!SD.begin(PA_3)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  //Pines de Salida
  pinMode(buzzerPin,OUTPUT);
  pinMode(btn1, INPUT_PULLUP);
  pinMode(btn2, INPUT_PULLUP);

  
  readSD();
  
  
  SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
  Serial.begin(115200);
  GPIOPadConfigSet(GPIO_PORTB_BASE, 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7, GPIO_STRENGTH_8MA, GPIO_PIN_TYPE_STD_WPU);
  Serial.println("Inicio");
  LCD_Init();
  LCD_Clear(0x00);
  FillRect(0, 0, 240, 320, 0x0000);

  LCD_Print(T1, 109, 298, 1, 0xffff, 0x0000);
  LCD_Print(T2, 109, 266, 1, 0xffff, 0x0000);
  LCD_Print(T3, 109, 234, 1, 0xffff, 0x0000);
  LCD_Print(T4, 113, 202, 1, 0xffff, 0x0000);
  LCD_Print(T5, 113, 170, 1, 0xffff, 0x0000);
  LCD_Print(T6, 121, 138, 1, 0xffff, 0x0000);
  LCD_Print(T7, 121, 106, 1, 0xffff, 0x0000);
  LCD_Print(T8, 121, 74, 1, 0xffff, 0x0000);
  LCD_Print(T9, 117, 42, 1, 0xffff, 0x0000);
  LCD_Print(T10, 117, 10, 1, 0xffff, 0x0000);
  
  }

//***************************************************************************************************************
// Loop Principal
//***************************************************************************************************************


void loop() {

  if(digitalRead(btn1) == LOW){
    BTN1();
    Pantalla();
     
   }
   
  if(digitalRead(btn2) == LOW){
    BTN2();
    writeSD();
    
   }

  if(Serial2.available() > 0){
    valor = Serial2.read(); 
    
    
  }
  //Fila 1
  FillRect(0, 288, 48, 31, 0x0000);
  FillRect(49, 288, 48, 31, 0x0000);
  FillRect(147, 288, 48, 31, 0x0000);
  FillRect(196, 288, 48, 31, 0x0000);

  //Fila 2
  FillRect(0, 256, 48, 31, 0x0000);
  FillRect(49, 256, 48, 31, 0x0000);
  FillRect(147, 256, 48, 31, 0x0000);
  FillRect(196, 256, 48, 31, 0x0000);

  //Fila 3
  FillRect(0, 224, 48, 31, 0x0000);
  FillRect(49, 224, 48, 31, 0x0000);
  FillRect(147, 224, 48, 31, 0x0000);
  FillRect(196, 224, 48, 31, 0x0000);

  //Fila 4
  FillRect(0, 192, 48, 31, 0x0000);
  FillRect(49, 192, 48, 31, 0x0000);
  FillRect(147, 192, 48, 31, 0x0000);
  FillRect(196, 192, 48, 31, 0x0000);

  //Fila 5
  FillRect(0, 160, 48, 31, 0x0000);
  FillRect(49, 160, 48, 31, 0x0000);
  FillRect(147, 160, 48, 31, 0x0000);
  FillRect(196, 160, 48, 31, 0x0000);

  //Fila 6
  FillRect(0, 128, 48, 31, 0x0000);
  FillRect(49, 128, 48, 31, 0x0000);
  FillRect(147, 128, 48, 31, 0x0000);
  FillRect(196, 128, 48, 31, 0x0000);

  //Fila 7
  FillRect(0, 96, 48, 31, 0x0000);
  FillRect(49, 96, 48, 31, 0x0000);
  FillRect(147, 96, 48, 31, 0x0000);
  FillRect(196, 96, 48, 31, 0x0000);

  //Fila 8
  FillRect(0, 64, 48, 31, 0x0000);
  FillRect(49, 64, 48, 31, 0x0000);
  FillRect(147, 64, 48, 31, 0x0000);
  FillRect(196, 64, 48, 31, 0x0000);
  
  //Fila 9
  FillRect(0, 32, 48, 31, 0x0000);
  FillRect(49, 32, 48, 31, 0x0000);
  FillRect(147, 32, 48, 31, 0x0000);
  FillRect(196, 32, 48, 31, 0x0000);

  //Fila 10
  FillRect(0, 0, 48, 31, 0x0000);
  FillRect(49, 0, 48, 31, 0x0000);
  FillRect(147, 0, 48, 31, 0x0000);
  FillRect(196, 0, 48, 31, 0x0000);
  
  delay(100);
  
}
//***************************************************************************************************************
// Función ReadSD
//***************************************************************************************************************

void readSD(void){
  archivo = SD.open("vúmetro.txt");
  if (archivo) {
    Serial.println("El archivo contiene lo siguiente:");

    // read from the file until there's nothing else in it:
    while (archivo.available()) {
      Serial.write(archivo.read());
    }
    // close the file:
    archivo.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening vúmetro.txt");
  }
  }

//***************************************************************************************************************
// Función WriteSD
//***************************************************************************************************************
void writeSD(void){
    archivo = SD.open("vumetro.txt", FILE_WRITE);
  
  if (archivo) {
    Serial.print("Writing to vumetro.txt...");
    
    archivo.println(datodB);
    // close the file:
    archivo.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening vumetro.txt");
  }
  
  }

//***************************************************************************************************************
// Función BTN1
//***************************************************************************************************************

void BTN1(void){
  for (int thisNote = 0; thisNote < 2; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote],noteDuration);

    int pauseBetweenNotes = noteDuration + 50;      //delay between pulse
    delay(pauseBetweenNotes);
    
    noTone(buzzerPin);   
  }   
}

//***************************************************************************************************************
// Función BTN2
//***************************************************************************************************************
void BTN2(void){
  for (int thisNote = 0; thisNote < 2; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000/noteDurations[thisNote];
    tone(buzzerPin, melody2[thisNote],noteDuration);

    int pauseBetweenNotes = noteDuration + 50;      //delay between pulse
    delay(pauseBetweenNotes);
    
    noTone(buzzerPin);   
  }   
}

//***************************************************************************************************************
// Función BTN2
//***************************************************************************************************************
void Pantalla(void){
  dB = valor;
    
    if(dB > 10 && dB <= 20){
      analogWrite(ledR, 0); 
      analogWrite(ledA, 0);
      analogWrite(ledV, 255);
      datodB = "Intensidad: -22dB";
      
       //Fila 1
      FillRect(0, 288, 48, 31, 0x07E0);
      FillRect(49, 288, 48, 31, 0x07E0);
      FillRect(147, 288, 48, 31, 0x07E0);
      FillRect(196, 288, 48, 31, 0x07E0);
      Serial.println("Intensidad: -22dB");

      delay(1000);
    }
    else if(dB > 20 && dB <= 25){
      analogWrite(ledR, 0); 
      analogWrite(ledA, 0);
      analogWrite(ledV, 255); 
      datodB = "Intensidad: -16dB";

      //Fila 1
      FillRect(0, 288, 48, 31, 0x07E0);
      FillRect(49, 288, 48, 31, 0x07E0);
      FillRect(147, 288, 48, 31, 0x07E0);
      FillRect(196, 288, 48, 31, 0x07E0);
    
      //Fila 2
      FillRect(0, 256, 48, 31, 0x07E0);
      FillRect(49, 256, 48, 31, 0x07E0);
      FillRect(147, 256, 48, 31, 0x07E0);
      FillRect(196, 256, 48, 31, 0x07E0);

      Serial.println("Intensidad: -16dB");

      delay(1000);
   
    }

    else if(dB > 25 && dB <= 30 ){
      analogWrite(ledR, 0); 
      analogWrite(ledA, 0);
      analogWrite(ledV, 255);

      datodB = "Intensidad: -12dB";
      
        
        //Fila 1
        FillRect(0, 288, 48, 31, 0x07E0);
        FillRect(49, 288, 48, 31, 0x07E0);
        FillRect(147, 288, 48, 31, 0x07E0);
        FillRect(196, 288, 48, 31, 0x07E0);
      
        //Fila 2
        FillRect(0, 256, 48, 31, 0x07E0);
        FillRect(49, 256, 48, 31, 0x07E0);
        FillRect(147, 256, 48, 31, 0x07E0);
        FillRect(196, 256, 48, 31, 0x07E0);
      
        //Fila 3
        FillRect(0, 224, 48, 31, 0x07E0);
        FillRect(49, 224, 48, 31, 0x07E0);
        FillRect(147, 224, 48, 31, 0x07E0);
        FillRect(196, 224, 48, 31, 0x07E0);

        Serial.println("Intensidad: -12dB");

        delay(1000);
    }
    
    else if (dB > 30 && dB <= 35){
      analogWrite(ledR, 0); 
      analogWrite(ledA, 0);
      analogWrite(ledV, 255);
      datodB = "Intensidad: -6dB";
      
      //Fila 1
      FillRect(0, 288, 48, 31, 0x07E0);
      FillRect(49, 288, 48, 31, 0x07E0);
      FillRect(147, 288, 48, 31, 0x07E0);
      FillRect(196, 288, 48, 31, 0x07E0);
    
      //Fila 2
      FillRect(0, 256, 48, 31, 0x07E0);
      FillRect(49, 256, 48, 31, 0x07E0);
      FillRect(147, 256, 48, 31, 0x07E0);
      FillRect(196, 256, 48, 31, 0x07E0);
    
      //Fila 3
      FillRect(0, 224, 48, 31, 0x07E0);
      FillRect(49, 224, 48, 31, 0x07E0);
      FillRect(147, 224, 48, 31, 0x07E0);
      FillRect(196, 224, 48, 31, 0x07E0);
    
      //Fila 4
      FillRect(0, 192, 48, 31, 0x07E0);
      FillRect(49, 192, 48, 31, 0x07E0);
      FillRect(147, 192, 48, 31, 0x07E0);
      FillRect(196, 192, 48, 31, 0x07E0);

      Serial.println("Intensidad: -6dB");

      delay(1000);
      
    }

    else if (dB > 35 && dB <= 40){
      analogWrite(ledR, 0); 
      analogWrite(ledA, 0);
      analogWrite(ledV, 255);
      datodB = "Intensidad: -3dB";
      
      //Fila 1
      FillRect(0, 288, 48, 31, 0x07E0);
      FillRect(49, 288, 48, 31, 0x07E0);
      FillRect(147, 288, 48, 31, 0x07E0);
      FillRect(196, 288, 48, 31, 0x07E0);
    
      //Fila 2
      FillRect(0, 256, 48, 31, 0x07E0);
      FillRect(49, 256, 48, 31, 0x07E0);
      FillRect(147, 256, 48, 31, 0x07E0);
      FillRect(196, 256, 48, 31, 0x07E0);
    
      //Fila 3
      FillRect(0, 224, 48, 31, 0x07E0);
      FillRect(49, 224, 48, 31, 0x07E0);
      FillRect(147, 224, 48, 31, 0x07E0);
      FillRect(196, 224, 48, 31, 0x07E0);
    
      //Fila 4
      FillRect(0, 192, 48, 31, 0x07E0);
      FillRect(49, 192, 48, 31, 0x07E0);
      FillRect(147, 192, 48, 31, 0x07E0);
      FillRect(196, 192, 48, 31, 0x07E0);
    
      //Fila 5
      FillRect(0, 160, 48, 31, 0x07E0);
      FillRect(49, 160, 48, 31, 0x07E0);
      FillRect(147, 160, 48, 31, 0x07E0);
      FillRect(196, 160, 48, 31, 0x07E0);

      Serial.println("Intensidad: -3dB");

      delay(1000);

    }

    else if (dB > 40 && dB <= 45){
      analogWrite(ledR, 255); 
      analogWrite(ledA, 0);
      analogWrite(ledV, 255);
      datodB = "Intensidad: 0dB";
      
      //Fila 1
      FillRect(0, 288, 48, 31, 0x07E0);
      FillRect(49, 288, 48, 31, 0x07E0);
      FillRect(147, 288, 48, 31, 0x07E0);
      FillRect(196, 288, 48, 31, 0x07E0);
    
      //Fila 2
      FillRect(0, 256, 48, 31, 0x07E0);
      FillRect(49, 256, 48, 31, 0x07E0);
      FillRect(147, 256, 48, 31, 0x07E0);
      FillRect(196, 256, 48, 31, 0x07E0);
    
      //Fila 3
      FillRect(0, 224, 48, 31, 0x07E0);
      FillRect(49, 224, 48, 31, 0x07E0);
      FillRect(147, 224, 48, 31, 0x07E0);
      FillRect(196, 224, 48, 31, 0x07E0);
    
      //Fila 4
      FillRect(0, 192, 48, 31, 0x07E0);
      FillRect(49, 192, 48, 31, 0x07E0);
      FillRect(147, 192, 48, 31, 0x07E0);
      FillRect(196, 192, 48, 31, 0x07E0);
    
      //Fila 5
      FillRect(0, 160, 48, 31, 0x07E0);
      FillRect(49, 160, 48, 31, 0x07E0);
      FillRect(147, 160, 48, 31, 0x07E0);
      FillRect(196, 160, 48, 31, 0x07E0);
    
      //Fila 6
      FillRect(0, 128, 48, 31, 0x0FFF);
      FillRect(49, 128, 48, 31, 0x0FFF);
      FillRect(147, 128, 48, 31, 0x0FFF);
      FillRect(196, 128, 48, 31, 0x0FFF);

      Serial.println("Intensidad: 0 dB");

      delay(1000);  
    }
    else if (dB > 45 && dB <= 50){
      analogWrite(ledR, 255); 
      analogWrite(ledA, 0);
      analogWrite(ledV, 255);
      datodB = "Intensidad: 3dB";
      
      //Fila 1
      FillRect(0, 288, 48, 31, 0x07E0);
      FillRect(49, 288, 48, 31, 0x07E0);
      FillRect(147, 288, 48, 31, 0x07E0);
      FillRect(196, 288, 48, 31, 0x07E0);
    
      //Fila 2
      FillRect(0, 256, 48, 31, 0x07E0);
      FillRect(49, 256, 48, 31, 0x07E0);
      FillRect(147, 256, 48, 31, 0x07E0);
      FillRect(196, 256, 48, 31, 0x07E0);
    
      //Fila 3
      FillRect(0, 224, 48, 31, 0x07E0);
      FillRect(49, 224, 48, 31, 0x07E0);
      FillRect(147, 224, 48, 31, 0x07E0);
      FillRect(196, 224, 48, 31, 0x07E0);
    
      //Fila 4
      FillRect(0, 192, 48, 31, 0x07E0);
      FillRect(49, 192, 48, 31, 0x07E0);
      FillRect(147, 192, 48, 31, 0x07E0);
      FillRect(196, 192, 48, 31, 0x07E0);
    
      //Fila 5
      FillRect(0, 160, 48, 31, 0x07E0);
      FillRect(49, 160, 48, 31, 0x07E0);
      FillRect(147, 160, 48, 31, 0x07E0);
      FillRect(196, 160, 48, 31, 0x07E0);
    
      //Fila 6
      FillRect(0, 128, 48, 31, 0x0FFF);
      FillRect(49, 128, 48, 31, 0x0FFF);
      FillRect(147, 128, 48, 31, 0x0FFF);
      FillRect(196, 128, 48, 31, 0x0FFF);
    
      //Fila 7
      FillRect(0, 96, 48, 31, 0x0FFF);
      FillRect(49, 96, 48, 31, 0x0FFF);
      FillRect(147, 96, 48, 31, 0x0FFF);
      FillRect(196, 96, 48, 31, 0x0FFF);

      Serial.println("Intensidad: 3dB");

      delay(1000);
      
    }

    else if (dB > 50 && dB <= 55){
      analogWrite(ledR, 255); 
      analogWrite(ledA, 0);
      analogWrite(ledV, 255);
      datodB = "Intensidad: 6dB";
      
      //Fila 1
      FillRect(0, 288, 48, 31, 0x07E0);
      FillRect(49, 288, 48, 31, 0x07E0);
      FillRect(147, 288, 48, 31, 0x07E0);
      FillRect(196, 288, 48, 31, 0x07E0);
    
      //Fila 2
      FillRect(0, 256, 48, 31, 0x07E0);
      FillRect(49, 256, 48, 31, 0x07E0);
      FillRect(147, 256, 48, 31, 0x07E0);
      FillRect(196, 256, 48, 31, 0x07E0);
    
      //Fila 3
      FillRect(0, 224, 48, 31, 0x07E0);
      FillRect(49, 224, 48, 31, 0x07E0);
      FillRect(147, 224, 48, 31, 0x07E0);
      FillRect(196, 224, 48, 31, 0x07E0);
    
      //Fila 4
      FillRect(0, 192, 48, 31, 0x07E0);
      FillRect(49, 192, 48, 31, 0x07E0);
      FillRect(147, 192, 48, 31, 0x07E0);
      FillRect(196, 192, 48, 31, 0x07E0);
    
      //Fila 5
      FillRect(0, 160, 48, 31, 0x07E0);
      FillRect(49, 160, 48, 31, 0x07E0);
      FillRect(147, 160, 48, 31, 0x07E0);
      FillRect(196, 160, 48, 31, 0x07E0);
    
      //Fila 6
      FillRect(0, 128, 48, 31, 0x0FFF);
      FillRect(49, 128, 48, 31, 0x0FFF);
      FillRect(147, 128, 48, 31, 0x0FFF);
      FillRect(196, 128, 48, 31, 0x0FFF);
    
      //Fila 7
      FillRect(0, 96, 48, 31, 0x0FFF);
      FillRect(49, 96, 48, 31, 0x0FFF);
      FillRect(147, 96, 48, 31, 0x0FFF);
      FillRect(196, 96, 48, 31, 0x0FFF);
    
      //Fila 8
      FillRect(0, 64, 48, 31, 0x0FFF);
      FillRect(49, 64, 48, 31, 0x0FFF);
      FillRect(147, 64, 48, 31, 0x0FFF);
      FillRect(196, 64, 48, 31, 0x0FFF);

      Serial.println("Intensidad: 6dB");
  
      delay(1000);
      
    }

    else if (dB > 55 && dB <= 60){
      analogWrite(ledR, 255); 
      analogWrite(ledA, 0);
      analogWrite(ledV, 0);
      datodB = "Intensidad: 10dB";
      
      //Fila 1
      FillRect(0, 288, 48, 31, 0x07E0);
      FillRect(49, 288, 48, 31, 0x07E0);
      FillRect(147, 288, 48, 31, 0x07E0);
      FillRect(196, 288, 48, 31, 0x07E0);
    
      //Fila 2
      FillRect(0, 256, 48, 31, 0x07E0);
      FillRect(49, 256, 48, 31, 0x07E0);
      FillRect(147, 256, 48, 31, 0x07E0);
      FillRect(196, 256, 48, 31, 0x07E0);
    
      //Fila 3
      FillRect(0, 224, 48, 31, 0x07E0);
      FillRect(49, 224, 48, 31, 0x07E0);
      FillRect(147, 224, 48, 31, 0x07E0);
      FillRect(196, 224, 48, 31, 0x07E0);
    
      //Fila 4
      FillRect(0, 192, 48, 31, 0x07E0);
      FillRect(49, 192, 48, 31, 0x07E0);
      FillRect(147, 192, 48, 31, 0x07E0);
      FillRect(196, 192, 48, 31, 0x07E0);
    
      //Fila 5
      FillRect(0, 160, 48, 31, 0x07E0);
      FillRect(49, 160, 48, 31, 0x07E0);
      FillRect(147, 160, 48, 31, 0x07E0);
      FillRect(196, 160, 48, 31, 0x07E0);
    
      //Fila 6
      FillRect(0, 128, 48, 31, 0x0FFF);
      FillRect(49, 128, 48, 31, 0x0FFF);
      FillRect(147, 128, 48, 31, 0x0FFF);
      FillRect(196, 128, 48, 31, 0x0FFF);
    
      //Fila 7
      FillRect(0, 96, 48, 31, 0x0FFF);
      FillRect(49, 96, 48, 31, 0x0FFF);
      FillRect(147, 96, 48, 31, 0x0FFF);
      FillRect(196, 96, 48, 31, 0x0FFF);
    
      //Fila 8
      FillRect(0, 64, 48, 31, 0x0FFF);
      FillRect(49, 64, 48, 31, 0x0FFF);
      FillRect(147, 64, 48, 31, 0x0FFF);
      FillRect(196, 64, 48, 31, 0x0FFF);
      
      //Fila 9
      FillRect(0, 32, 48, 31, 0x003F);
      FillRect(49, 32, 48, 31, 0x003F);
      FillRect(147, 32, 48, 31, 0x003F);
      FillRect(196, 32, 48, 31, 0x003F);

      Serial.println("Intensidad:  10dB");

      delay(1000);
      
    }

    else if (dB >= 60){
      analogWrite(ledR, 255); 
      analogWrite(ledA, 0);
      analogWrite(ledV, 0);
      datodB = "Intensidad: 16dB";
      
      //Fila 1
      FillRect(0, 288, 48, 31, 0x07E0);
      FillRect(49, 288, 48, 31, 0x07E0);
      FillRect(147, 288, 48, 31, 0x07E0);
      FillRect(196, 288, 48, 31, 0x07E0);
    
      //Fila 2
      FillRect(0, 256, 48, 31, 0x07E0);
      FillRect(49, 256, 48, 31, 0x07E0);
      FillRect(147, 256, 48, 31, 0x07E0);
      FillRect(196, 256, 48, 31, 0x07E0);
    
      //Fila 3
      FillRect(0, 224, 48, 31, 0x07E0);
      FillRect(49, 224, 48, 31, 0x07E0);
      FillRect(147, 224, 48, 31, 0x07E0);
      FillRect(196, 224, 48, 31, 0x07E0);
    
      //Fila 4
      FillRect(0, 192, 48, 31, 0x07E0);
      FillRect(49, 192, 48, 31, 0x07E0);
      FillRect(147, 192, 48, 31, 0x07E0);
      FillRect(196, 192, 48, 31, 0x07E0);
    
      //Fila 5
      FillRect(0, 160, 48, 31, 0x07E0);
      FillRect(49, 160, 48, 31, 0x07E0);
      FillRect(147, 160, 48, 31, 0x07E0);
      FillRect(196, 160, 48, 31, 0x07E0);
    
      //Fila 6
      FillRect(0, 128, 48, 31, 0x0FFF);
      FillRect(49, 128, 48, 31, 0x0FFF);
      FillRect(147, 128, 48, 31, 0x0FFF);
      FillRect(196, 128, 48, 31, 0x0FFF);
    
      //Fila 7
      FillRect(0, 96, 48, 31, 0x0FFF);
      FillRect(49, 96, 48, 31, 0x0FFF);
      FillRect(147, 96, 48, 31, 0x0FFF);
      FillRect(196, 96, 48, 31, 0x0FFF);
    
      //Fila 8
      FillRect(0, 64, 48, 31, 0x0FFF);
      FillRect(49, 64, 48, 31, 0x0FFF);
      FillRect(147, 64, 48, 31, 0x0FFF);
      FillRect(196, 64, 48, 31, 0x0FFF);
      
      //Fila 9
      FillRect(0, 32, 48, 31, 0x003F);
      FillRect(49, 32, 48, 31, 0x003F);
      FillRect(147, 32, 48, 31, 0x003F);
      FillRect(196, 32, 48, 31, 0x003F);
    
      //Fila 10
      FillRect(0, 0, 48, 31, 0x003F);
      FillRect(49, 0, 48, 31, 0x003F);
      FillRect(147, 0, 48, 31, 0x003F);
      FillRect(196, 0, 48, 31, 0x003F);
      Serial.println("Intensidad: 16dB");

      delay(1000);
      
    }
}


//***************************************************************************************************************************************
// Función para inicializar LCD
//***************************************************************************************************************************************
void LCD_Init(void) {
  pinMode(LCD_RST, OUTPUT);
  pinMode(LCD_CS, OUTPUT);
  pinMode(LCD_RS, OUTPUT);
  pinMode(LCD_WR, OUTPUT);
  pinMode(LCD_RD, OUTPUT);
  for (uint8_t i = 0; i < 8; i++) {
    pinMode(DPINS[i], OUTPUT);
  }
  //****************************************
  // Secuencia de Inicialización
  //****************************************
  digitalWrite(LCD_CS, HIGH);
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, HIGH);
  digitalWrite(LCD_RD, HIGH);
  digitalWrite(LCD_RST, HIGH);
  delay(5);
  digitalWrite(LCD_RST, LOW);
  delay(20);
  digitalWrite(LCD_RST, HIGH);
  delay(150);
  digitalWrite(LCD_CS, LOW);
  //****************************************
  LCD_CMD(0xE9);  // SETPANELRELATED
  LCD_DATA(0x20);
  //****************************************
  LCD_CMD(0x11); // Exit Sleep SLEEP OUT (SLPOUT)
  delay(100);
  //****************************************
  LCD_CMD(0xD1);    // (SETVCOM)
  LCD_DATA(0x00);
  LCD_DATA(0x71);
  LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0xD0);   // (SETPOWER)
  LCD_DATA(0x07);
  LCD_DATA(0x01);
  LCD_DATA(0x08);
  //****************************************
  LCD_CMD(0x36); //(0x36) // (MEMORYACCESS)
  LCD_DATA(0x96);//(0xC8) (0xE8)0x40 | 0x80 | 0x20 | 0x08); // LCD_DATA(0x19);
  //****************************************
  LCD_CMD(0x3A); // Set_pixel_format (PIXELFORMAT)
  LCD_DATA(0x05); // color setings, 05h - 16bit pixel, 11h - 3bit pixel
  //****************************************
  LCD_CMD(0xC1);    // (POWERCONTROL2)
  LCD_DATA(0x10);
  LCD_DATA(0x10);
  LCD_DATA(0x02);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC0); // Set Default Gamma (POWERCONTROL1)
  LCD_DATA(0x00);
  LCD_DATA(0x35);
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x02);
  //****************************************
  LCD_CMD(0xC5); // Set Frame Rate (VCOMCONTROL1)
  LCD_DATA(0x04); // 72Hz
  //****************************************
  LCD_CMD(0xD2); // Power Settings  (SETPWRNORMAL)
  LCD_DATA(0x01);
  LCD_DATA(0x44);
  //****************************************
  LCD_CMD(0xC8); //Set Gamma  (GAMMASET)
  LCD_DATA(0x04);
  LCD_DATA(0x67);
  LCD_DATA(0x35);
  LCD_DATA(0x04);
  LCD_DATA(0x08);
  LCD_DATA(0x06);
  LCD_DATA(0x24);
  LCD_DATA(0x01);
  LCD_DATA(0x37);
  LCD_DATA(0x40);
  LCD_DATA(0x03);
  LCD_DATA(0x10);
  LCD_DATA(0x08);
  LCD_DATA(0x80);
  LCD_DATA(0x00);
  //****************************************
  LCD_CMD(0x2A); // Set_column_address 320px (CASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0x3F);
  //****************************************
  LCD_CMD(0x2B); // Set_page_address 480px (PASET)
  LCD_DATA(0x00);
  LCD_DATA(0x00);
  LCD_DATA(0x01);
  LCD_DATA(0xE0);
  //  LCD_DATA(0x8F);
  LCD_CMD(0x29); //display on
  LCD_CMD(0x2C); //display on

  LCD_CMD(ILI9341_INVOFF); //Invert Off
  delay(120);
  LCD_CMD(ILI9341_SLPOUT);    //Exit Sleep
  delay(120);
  LCD_CMD(ILI9341_DISPON);    //Display on
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar comandos a la LCD - parámetro (comando)
//***************************************************************************************************************************************
void LCD_CMD(uint8_t cmd) {
  digitalWrite(LCD_RS, LOW);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = cmd;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para enviar datos a la LCD - parámetro (dato)
//***************************************************************************************************************************************
void LCD_DATA(uint8_t data) {
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_WR, LOW);
  GPIO_PORTB_DATA_R = data;
  digitalWrite(LCD_WR, HIGH);
}
//***************************************************************************************************************************************
// Función para definir rango de direcciones de memoria con las cuales se trabajara (se define una ventana)
//***************************************************************************************************************************************
void SetWindows(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
  LCD_CMD(0x2a); // Set_column_address 4 parameters
  LCD_DATA(x1 >> 8);
  LCD_DATA(x1);
  LCD_DATA(x2 >> 8);
  LCD_DATA(x2);
  LCD_CMD(0x2b); // Set_page_address 4 parameters
  LCD_DATA(y1 >> 8);
  LCD_DATA(y1);
  LCD_DATA(y2 >> 8);
  LCD_DATA(y2);
  LCD_CMD(0x2c); // Write_memory_start
}
//***************************************************************************************************************************************
// Función para borrar la pantalla - parámetros (color)
//***************************************************************************************************************************************
void LCD_Clear(unsigned int c) {
  unsigned int x, y;
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  SetWindows(0, 0, 319, 239); // 479, 319);
  for (x = 0; x < 320; x++)
    for (y = 0; y < 240; y++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);
    }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea horizontal - parámetros ( coordenada x, cordenada y, longitud, color)
//***************************************************************************************************************************************
void H_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + x;
  SetWindows(x, y, l, y);
  j = l;// * 2;
  for (i = 0; i < l; i++) {
    LCD_DATA(c >> 8);
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una línea vertical - parámetros ( coordenada x, cordenada y, longitud, color)
//***************************************************************************************************************************************
void V_line(unsigned int x, unsigned int y, unsigned int l, unsigned int c) {
  unsigned int i, j;
  LCD_CMD(0x02c); //write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);
  l = l + y;
  SetWindows(x, y, x, l);
  j = l; //* 2;
  for (i = 1; i <= j; i++) {
    LCD_DATA(c >> 8);
    LCD_DATA(c);
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
void Rect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  H_line(x  , y  , w, c);
  H_line(x  , y + h, w, c);
  V_line(x  , y  , h, c);
  V_line(x + w, y  , h, c);
}
//***************************************************************************************************************************************
// Función para dibujar un rectángulo relleno - parámetros ( coordenada x, cordenada y, ancho, alto, color)
//***************************************************************************************************************************************
/*void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  unsigned int i;
  for (i = 0; i < h; i++) {
    H_line(x  , y  , w, c);
    H_line(x  , y+i, w, c);
  }
  }
*/

void FillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, unsigned int c) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 = x + w;
  y2 = y + h;
  SetWindows(x, y, x2 - 1, y2 - 1);
  unsigned int k = w * h * 2 - 1;
  unsigned int i, j;
  for (int i = 0; i < w; i++) {
    for (int j = 0; j < h; j++) {
      LCD_DATA(c >> 8);
      LCD_DATA(c);

      //LCD_DATA(bitmap[k]);
      k = k - 2;
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar texto - parámetros ( texto, coordenada x, cordenada y, color, background)
//***************************************************************************************************************************************

void LCD_Print(String text, int x, int y, int fontSize, int color, int background) {
  int fontXSize ;
  int fontYSize ;

  if (fontSize == 1) {
    fontXSize = fontXSizeSmal ;
    fontYSize = fontYSizeSmal ;
  }
  if (fontSize == 2) {
    fontXSize = fontXSizeBig ;
    fontYSize = fontYSizeBig ;
  }

  char charInput ;
  int cLength = text.length();
  Serial.println(cLength, DEC);
  int charDec ;
  int c ;
  int charHex ;
  char char_array[cLength + 1];
  text.toCharArray(char_array, cLength + 1) ;
  for (int i = 0; i < cLength ; i++) {
    charInput = char_array[i];
    Serial.println(char_array[i]);
    charDec = int(charInput);
    digitalWrite(LCD_CS, LOW);
    SetWindows(x + (i * fontXSize), y, x + (i * fontXSize) + fontXSize - 1, y + fontYSize );
    long charHex1 ;
    for ( int n = 0 ; n < fontYSize ; n++ ) {
      if (fontSize == 1) {
        charHex1 = pgm_read_word_near(smallFont + ((charDec - 32) * fontYSize) + n);
      }
      if (fontSize == 2) {
        charHex1 = pgm_read_word_near(bigFont + ((charDec - 32) * fontYSize) + n);
      }
      for (int t = 1; t < fontXSize + 1 ; t++) {
        if (( charHex1 & (1 << (fontXSize - t))) > 0 ) {
          c = color ;
        } else {
          c = background ;
        }
        LCD_DATA(c >> 8);
        LCD_DATA(c);
      }
    }
    digitalWrite(LCD_CS, HIGH);
  }
}
//***************************************************************************************************************************************
// Función para dibujar una imagen a partir de un arreglo de colores (Bitmap) Formato (Color 16bit R 5bits G 6bits B 5bits)
//***************************************************************************************************************************************
void LCD_Bitmap(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned char bitmap[]) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 = x + width;
  y2 = y + height;
  SetWindows(x, y, x2 - 1, y2 - 1);
  unsigned int k = 0;
  unsigned int i, j;

  for (int i = 0; i < width; i++) {
    for (int j = 0; j < height; j++) {
      LCD_DATA(bitmap[k]);
      LCD_DATA(bitmap[k + 1]);
      //LCD_DATA(bitmap[k]);
      k = k + 2;
    }
  }
  digitalWrite(LCD_CS, HIGH);
}
//***************************************************************************************************************************************
// Función para dibujar una imagen sprite - los parámetros columns = número de imagenes en el sprite, index = cual desplegar, flip = darle vuelta
//***************************************************************************************************************************************
void LCD_Sprite(int x, int y, int width, int height, unsigned char bitmap[], int columns, int index, char flip, char offset) {
  LCD_CMD(0x02c); // write_memory_start
  digitalWrite(LCD_RS, HIGH);
  digitalWrite(LCD_CS, LOW);

  unsigned int x2, y2;
  x2 =   x + width;
  y2 =    y + height;
  SetWindows(x, y, x2 - 1, y2 - 1);
  int k = 0;
  int ancho = ((width * columns));
  if (flip) {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width - 1 - offset) * 2;
      k = k + width * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k - 2;
      }
    }
  } else {
    for (int j = 0; j < height; j++) {
      k = (j * (ancho) + index * width + 1 + offset) * 2;
      for (int i = 0; i < width; i++) {
        LCD_DATA(bitmap[k]);
        LCD_DATA(bitmap[k + 1]);
        k = k + 2;
      }
    }


  }
  digitalWrite(LCD_CS, HIGH);
}
