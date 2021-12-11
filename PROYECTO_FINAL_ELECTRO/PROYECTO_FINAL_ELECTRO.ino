#include <Wire.h>
#include "Adafruit_TCS34725.h"

int LEDR = 11;  
int LEDB = 10;
int LEDG = 9;

bool did = false; //eliminar despues de pruebas
bool chat = true; //va en false
bool otherChat = true;//va en false
String input = "";
String text = "Hola como estas?";
boolean primero = false;

String colorAscii;

uint16_t colorsList[20][3];    //Stores the meditions of 1 second
int idxColorList = 0;       //Current medition
int countLecture = 0;
boolean firstZero = false;
String currentChar = "";

int count = 0;
long start, finish;

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

  start = millis;
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
  finish = millis();
  //if(finish - start >= 1000){
      //Serial.println(count);
      //Serial.print("Tamaño ");
      //Serial.println(sizeof(colorsList));
  //}else{
      readInput();
      testLecture();
      //count++;
  //}

  
  //lectura();
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

void testLecture(){

    uint16_t r, g, b, c;
    boolean redOn, blueOn, greenOn;
    
    tcs.getRawData(&r, &g, &b, &c);

    redOn = (r > 120) ? true: false;
    blueOn = (b > 230) ? true: false;
    greenOn = (g > 140) ? true: false;

    Serial.print("Rojo: "); Serial.println(r, DEC);
    Serial.print("Verde: "); Serial.println(g, DEC);
    Serial.print("Azul: "); Serial.println(b, DEC);
    Serial.println(" ");
    
    if(r == 0 || b == 0 || g == 0){
        idxColorList = 0;
        if(firstZero){
            firstZero = false;
            //Serial.println("In 1 -------------------------");
            getMax();
        }
    }else{
        firstZero = true;
        //Creates a vector of (r,g,b) and stores it in colorsList
        colorsList[idxColorList][0] = r;
        colorsList[idxColorList][1] = g;
        colorsList[idxColorList][2] = b;
        idxColorList++;
    }

    if(currentChar.length() == 3){
        //Prints the char according to the colors read in 3 seconds
        //Serial.println("In 2 -------------------------");
        Serial.write(currentChar.toInt());
        Serial.println(" ");
        currentChar = "";
    }
}

void getMax(){
    uint16_t rMax, gMax, bMax;
    rMax = 0;
    gMax = 0;
    bMax = 0;
    for(int i = 0; i < 17; i++){
        int rTemp, gTemp, bTemp;
        rTemp = colorsList[i][0];
        gTemp = colorsList[i][1];
        bTemp = colorsList[i][2];
        
        rMax = (rTemp > rMax)? rTemp : rMax;
        gMax = (gTemp > gMax)? gTemp : gMax;
        bMax = (bTemp > bMax)? bTemp : bMax;
    }
    Serial.print("Rojo Max: "); Serial.println(rMax, DEC);
    Serial.print("Verde Max: "); Serial.println(gMax, DEC);
    Serial.print("Azul Max: "); Serial.println(bMax, DEC);
    Serial.println(" ");

    //printMatrix();
    currentChar += readColor(rMax, gMax, bMax);
    Serial.println(currentChar);
}

void printMatrix(){

  for(int i = 0; i < 17; i++){
      Serial.print("R: "); Serial.print(colorsList[i][0]); Serial.print(" ");
      Serial.print("G: "); Serial.print(colorsList[i][1]); Serial.print(" ");
      Serial.print("B: "); Serial.print(colorsList[i][2]); Serial.print(" ");
      Serial.println(" ");
  }

}

void lectura(){
  uint16_t r, g, b, c, lux;

  tcs.getRawData(&r, &g, &b, &c);
  
  boolean redOn, blueOn, greenOn;
  String value = "";
  //String answer = "";
  //long start, finish, reciept = 0;
  //start = millis();
  //tcs.getRawData(&r, &g, &b, &c);
  //reciept = millis();
  redOn = (r > 120) ? true: false;
  blueOn = (b > 270) ? true: false;
  greenOn = (g > 140) ? true: false; 
  if(redOn || blueOn || greenOn){
    Serial.print("Rojo: "); Serial.println(r, DEC);
    Serial.print("Verde: "); Serial.println(g, DEC);
    Serial.print("Azul: "); Serial.println(b, DEC);
    Serial.println(" ");
    //Serial.println("Recibido: " + reciept);
    value = readColor(r, g, b);
    if(countLecture == 0){
      //delay(100);
    }
    countLecture += 1;
    currentChar += value;
    //finish = millis();
    //Serial.println("Lectura " + finish - start);
    delay(1000);
    /*for (int i = 0; i < 3; i++){
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
  } */ 
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
  
  if(countLecture == 3){
    //Serial.println("Valor: " + currentChar);
    Serial.write(currentChar.toInt());
    //Serial.print(": " + currentChar);
    Serial.println(" ");
    countLecture = 0;
    currentChar = "";
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
        if(g>800){
            value = 8;
        }else if(r>=500){
            value = 9;
        }else{
            value = 6;
        }
    }else if(b>270){
        if(g > 140){
            value = 3;
        }else if(r > 120){
            value = 4;
        }else{
            value = 1;
        }
    }else if(g>170){
        if(g>600){
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
    Serial.println("Values: " + values);
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
