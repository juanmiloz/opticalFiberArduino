#include <Wire.h>
#include "Adafruit_TCS34725.h"

int LEDR = 11;  
int LEDB = 10;
int LEDG = 9;

bool did = false;
bool chat = true; //va en false
bool otherChat = true;//va en false
String input = "";
String text = "Alisson";


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
void setup(){
  Serial.begin(9600);
  if (!tcs.begin()) 
  {
    Serial.println("Error al iniciar TCS34725");
    while (1) delay(1000);
  }
  
  pinMode(LEDR,OUTPUT); // los pins digitales serán de salida
  pinMode(LEDG,OUTPUT);
  pinMode(LEDB,OUTPUT);
}


void loop()
{
  /*if(Serial.available()){
    input = Serial.readStringUntil('\n');
    if(input=="%" && !chat){
      chat = true;
      input = "";
    }else if(input=="%" && chat){
      chat = false;
      input = "";
    }
  }
  */
  //setupMode();
  //readInput();
  
  lectura();
}

void readInput(){
  if(chat && otherChat && Serial.available()){
    input = Serial.readStringUntil('\n');
    if(input=="&"){
      input="";
      translateAscii();
    }
  }
}

void lectura(){
  uint16_t r, g, b, c, colorTemp, lux;

  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  Serial.print("Temperatura color: "); Serial.print(colorTemp, DEC); Serial.println(" K");
  Serial.print("Lux : "); Serial.println(lux, DEC);
  Serial.print("Rojo: "); Serial.println(r, DEC);
  Serial.print("Verde: "); Serial.println(g, DEC);
  Serial.print("Azul: "); Serial.println(b, DEC);
  Serial.print("Clear: "); Serial.println(c, DEC);
  Serial.println(" ");
  delay(1000);

  analogWrite(LEDR, 0);
  analogWrite(LEDG, 255);
  analogWrite(LEDB, 255);  
}

void translateAscii(){
  char ascii[text.length()];
  text.toCharArray(ascii,text.length()+1);
  for(int i = 0; i < sizeof(ascii); i++){
    String values = String(int(ascii[i]));   
    
    if(values.length() == 2){
        colorN('0');
    }
    
    for(int j = 0; j < values.length(); j++){
      char n = values.charAt(j);
       
      colorN(n);
    }
  }
}

void colorN(char n){
  switch(n){
    //ROJO
    case '0':
      analogWrite(LEDR, 63);  
      analogWrite(LEDG, 0);
      analogWrite(LEDB, 0);
    break;

    case '1':
      analogWrite(LEDR, 0);  
      analogWrite(LEDG, 0);
      analogWrite(LEDB, 63);

    break;

    case '2':
      analogWrite(LEDR, 0);  
      analogWrite(LEDG, 63);
      analogWrite(LEDB, 0);
    break;

    case '3':
      analogWrite(LEDR, 0);  
      analogWrite(LEDG, 63);
      analogWrite(LEDB, 63);
    break;

    case '4':
      analogWrite(LEDR, 63);  
      analogWrite(LEDG, 0);
      analogWrite(LEDB, 63);
    break;

    case '5':
      analogWrite(LEDR, 255);  
      analogWrite(LEDG, 0);
      analogWrite(LEDB, 0);
    break;

    case '6':
      analogWrite(LEDR, 0);  
      analogWrite(LEDG, 0);
      analogWrite(LEDB, 255);
    break;

    case '7':
      analogWrite(LEDR, 0);  
      analogWrite(LEDG, 255);
      analogWrite(LEDB, 0);
    break;

    case '8':
      analogWrite(LEDR, 0);  
      analogWrite(LEDG, 255);
      analogWrite(LEDB, 255);
    break;

    case '9':
      analogWrite(LEDR, 255);  
      analogWrite(LEDG, 0);
      analogWrite(LEDB, 255);
    break;
  }
  delay(1000);
}

void setupMode(){
  if(chat && !otherChat){
    digitalWrite(LEDR, HIGH); 
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, LOW);
  }
  if(chat && otherChat){
    digitalWrite(LEDR, LOW); 
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
  }
  if(!chat && !otherChat){
    digitalWrite(LEDR, LOW); 
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
  }
}
