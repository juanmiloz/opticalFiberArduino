#include <Wire.h>
#include "Adafruit_TCS34725.h"

int LEDR = 11;  
int LEDB = 10;
int LEDG = 9;

bool did = false;
bool chat = true; //va en false
bool otherChat = true;//va en false
String input = "";
String text = "H";

String colorAscii;

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
  readInput();
  
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

  Serial.print("Rojo: "); Serial.println(r, DEC);
  Serial.print("Verde: "); Serial.println(g, DEC);
  Serial.print("Azul: "); Serial.println(b, DEC);
  Serial.println(" ");
  
  boolean redOn = (r > 1000) ? true: false;
  boolean blueOn = (b > 1000) ? true: false;
  boolean greenOn = (g > 1000) ? true: false;
  String value0, value1, value2 = "";
  if(redOn || blueOn || greenOn){
    Serial.println("Entro 1");
    value0 = readColor(r,g,b);
  }
  //delay(1000);
  tcs.getRawData(&r, &g, &b, &c);
  redOn = (r > 1000) ? true: false;
  blueOn = (b > 1000) ? true: false;
  greenOn = (g > 1000) ? true: false;
  if(redOn || blueOn || greenOn){
    Serial.println("Entro 2");
    value1 = readColor(r,g,b);
  }
  //delay(1000);
  tcs.getRawData(&r, &g, &b, &c);
  if(redOn || blueOn || greenOn){
    Serial.println("Entro 3");
    value2 = readColor(r,g,b);
  }
  
  if(value0 != "" || value1 != "" || value2 != ""){
    String answer = value0 + value1 + value2;
    Serial.println(answer);
  }
  //delay(1000);


  //analogWrite(LEDR, 0);
  //analogWrite(LEDG, 255);
  //analogWrite(LEDB, 255);  
}

String readColor(uint16_t r, uint16_t g, uint16_t b){
  
  boolean redOn = (r > 1000) ? true: false;
  boolean blueOn = (b > 1000) ? true: false;
  boolean greenOn = (g > 1000) ? true: false;

  int value = 0;
  //if(redOn || blueOn || greenOn){
  if(redOn && !blueOn & !greenOn){
      if(r > 8000){
          value = 5; 
      } else{
          value = 0;
      }
  }else if(!redOn && blueOn & !greenOn){
  
    if(b > 8000){
        value = 6;
    } else{
        value = 1;
    }
  }else if(!redOn && !blueOn & greenOn){
  
    if(g > 8000){
        value = 7;
    } else{
        value = 2;
    }
  }else if(!redOn && blueOn & greenOn){
  
    if(b > 8000){
        value = 8;
    } else{
        value = 3;
    }
  }else if(redOn && blueOn & !greenOn){
  
    if(b > 8000){
        value = 9;
    } else{
        value = 4;
    }
  //}
}

  return String(value);
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
  off();
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

void off(){
  digitalWrite(LEDR, LOW); 
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
}
