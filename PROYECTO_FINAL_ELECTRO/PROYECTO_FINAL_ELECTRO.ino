#include <Wire.h>
#include "Adafruit_TCS34725.h"

int LEDR = 11;  
int LEDB = 10;
int LEDG = 9;

bool did = false; //eliminar despues de pruebas
bool chat = true; //va en false
bool otherChat = true;//va en false
String input = "";
String text = "H";
boolean primero =false;

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
  uint16_t r, g, b, c, lux;

  //tcs.getRawData(&r, &g, &b, &c);

  //Serial.print("Rojo: "); Serial.println(r, DEC);
  //Serial.print("Verde: "); Serial.println(g, DEC);
  //Serial.print("Azul: "); Serial.println(b, DEC);
  //Serial.println(" ");
  
  boolean redOn, blueOn, greenOn;
  String value = "";
  String answer = "";

  tcs.getRawData(&r, &g, &b, &c);
  redOn = (r > 130) ? true: false; //arreglar 1000 por cifra que indique cuándo está prendido el led
  blueOn = (b > 300) ? true: false;
  greenOn = (g > 140) ? true: false; 
  if(redOn || blueOn || greenOn){
    for (int i = 0; i < 3; i++){
    if(i!=0){
      tcs.getRawData(&r, &g, &b, &c);
      redOn = (r > 130) ? true: false; //arreglar 1000 por cifra que indique cuándo está prendido el led
      blueOn = (b > 300) ? true: false;
      greenOn = (g > 140) ? true: false; 
    }
    if(redOn || blueOn || greenOn){
      if(!primero){
        primero = true;
        
      }
      value = readColor(r,g,b);
      answer += value;
      delay(1000);
    }
  }  
  }
  /*
  if(redOn || blueOn || greenOn){
    if(!primero){
      primero = true;
      delay(400)
     }
    value0 = readColor(r,g,b);
    delay(1000);
  }
  
  tcs.getRawData(&r, &g, &b, &c);
  redOn = (r > 130) ? true: false;
  blueOn = (b > 300) ? true: false;
  greenOn = (g > 140) ? true: false;
  if(redOn || blueOn || greenOn){
    //Serial.println("Entro 2");
    value1 = readColor(r,g,b);
    delay(1000);
  }
  tcs.getRawData(&r, &g, &b, &c);
  if(redOn || blueOn || greenOn){
    //Serial.println("Entro 3");
    value2 = readColor(r,g,b);
    delay(1000);
  }*/
  
  if(answer != ""){
    //Serial.println("Valor: " + answer);
    Serial.write(answer.toInt());
    Serial.println(" ");
  }


  //analogWrite(LEDR, 0);
  //analogWrite(LEDG, 255);
  //analogWrite(LEDB, 255);  
}

String readColor(uint16_t r, uint16_t g, uint16_t b){
  
  //boolean redOn = (r > 1000) ? true: false;
  //boolean blueOn = (b > 1000) ? true: false;
  //boolean greenOn = (g > 1000) ? true: false;
  
  int value = 0;
  /*
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
  //}*/
  if(b>1000){
    if(g>1000){
        value = 8;
    }else if(r>600){
        value = 9;
    }else{
        value = 6;
    }
}else if(b>300){
    if(b<330){
        value = 1;
    }else if(b<350){
        value = 4;
    }else{
        value = 3;
    }
}else if(g>170){
    if(g>700){
        value = 7;
    }else{
        value = 2;
    }
}else if(r>100){
    if(r>300){
        value = 5;
    }else{
        value = 0;
    }
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
  } //Falta la posibilidad de !chat y otherChat
}

void off(){
  digitalWrite(LEDR, LOW); 
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
}
