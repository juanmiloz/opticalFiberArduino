#include <Wire.h>
#include "Adafruit_TCS34725.h"

int LEDR = 11;  
int LEDB = 10;
int LEDG = 9;

String CHAT_CHAR = "%";
String ECO_CHAR = "!";    //Not defined in the statement
String INFO_CHAR = "&";

bool chat = false;      //Current chat is on, starts in false
bool otherChat = false; //Other chat is on, starts in false
bool eco = false;       //Eco is on

String input = "";
String text = "";
//String text = "Escribi un cuento de cien palabras perfecto. La gente lo leia con avidez, y lo enviaban entusiasmados a sus amigos. Me llamaron para hablar sobre el cuento en la tele, y desde Hollywood querian adaptarlo. Entonces alguien descubrio que habia escrito porque, en vez de por que, asi que ahora sobraba una palabra. Pero quitar cualquiera de ellas desmontaba el delicado mecanismo de relojeria que habia conseguido construir. Finalmente elimine";

String colorAscii;

uint16_t colorsList[20][3];    //Stores the meditions of 1 second
int idxColorList = 0;       //Current medition
int countLecture = 0;       //DELETE
boolean firstZero = false;  //First read is zero
String currentChar = "";    //Current char in reading process (ASCII Code)

String ecoReceipt = "";            //Eco reciept in the 3rd test
String ecoSended = "";             //Eco sended to other arduino in the 3rd test

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
  if(isChatActive()){
      testReadInput();
      testLecture();
  }else if(eco){
      testLecture();
      if(ecoReceipt.length() == 3){
        translateInput(ecoReceipt);
        if(ecoReceipt.toInt() ==  ECO_CHAR.toInt()){
          eco = false;
        }
        ecoReceipt = "";
      }
  }else{
    testReadInput();
    testLecture();
  }
  
  //setupMode();
  //readInput();
  //testLecture();
}

void readInput(){
  if(chat && otherChat && Serial.available()){
    input = Serial.readStringUntil('\n');
    if(input=="&"){
      input="";
      translateAscii();
    }
  }else{
    input= Serial.readStringUntil('\n');
    if(input == "%"){
      
      translateInput(input);
    }
  }
}

void testReadInput(){
  if(Serial.available() > 0){
    input = Serial.readStringUntil('\n');
    /*
    Serial.println(input.equals(INFO_CHAR));
    Serial.println(input.equals(ECO_CHAR));
    Serial.println(input.equals(CHAT_CHAR));*/
    if(isChatActive()){
      translateInput(input);
      
    }else if(input.equals(INFO_CHAR)){
      input = "";
      translateAscii();
      
    }else if(input.equals(ECO_CHAR)){
      eco = true;
      translateEco();
      
    }else if(input.equals(CHAT_CHAR)){
      translateInput(input);
      Serial.println(chat);
      Serial.println(otherChat);
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

    //Serial.print("Rojo: "); Serial.println(r, DEC);
    //Serial.print("Verde: "); Serial.println(g, DEC);
    //Serial.print("Azul: "); Serial.println(b, DEC);
    //Serial.println(" ");
    
    if(r == 0 || b == 0 || g == 0){
        idxColorList = 0;
        if(firstZero){
            firstZero = false;
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
        if(eco){
          ecoReceipt = currentChar;
        }
        //Prints the char according to the colors read in 3 seconds
        Serial.write(currentChar.toInt());
        currentChar = "";
        //Serial.print("Chat: "); Serial.println(chat);
        //Serial.print("Other chat: "); Serial.println(otherChat);
    }
    if(currentChar == '%'){
      otherChat = !otherChat;
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
    //Serial.println(currentChar);
}

void printMatrix(){

  for(int i = 0; i < 17; i++){
      Serial.print("R: "); Serial.print(colorsList[i][0]); Serial.print(" ");
      Serial.print("G: "); Serial.print(colorsList[i][1]); Serial.print(" ");
      Serial.print("B: "); Serial.print(colorsList[i][2]); Serial.print(" ");
      Serial.println(" ");
  }

}

bool isEcoCorrect(){
  bool isCorrect = false;
  if(ecoReceipt != "" && ecoReceipt != ""){
    isCorrect = ecoReceipt == ecoSended;
  }
  return isCorrect;
}

bool isChatActive(){
  return chat && otherChat;
}

String readColor(uint16_t r, uint16_t g, uint16_t b){
  
  int value = 0;
  if(b>1000){
      if(g>800){
          value = 8;
      }else if(r>=500){
          value = 9;
      }else{
          value = 6;
      }
  }else if(b>320){
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
  for(int i = 0; i < text.length(); i++){
    String values = String(int(text.charAt(i)));   
    Serial.println("Values: " + values);
    if(values.length() == 2){
        colorN('0');
        off();
        delay(1000);
    }
    
    for(int j = 0; j < values.length(); j++){
      char n = values.charAt(j); 
      colorN(n);
      off();
      delay(1000);
    }
  }
  /*char ascii[text.length()];
  text.toCharArray(ascii,text.length()+1);
  for(int i = 0; i < sizeof(ascii); i++){
    String values = String(int(ascii[i]));   
    Serial.println("Values: " + values);
    if(values.length() == 2){
        colorN('0');
        off();
        delay(1000);
    }
    
    for(int j = 0; j < values.length(); j++){
      char n = values.charAt(j); 
      colorN(n);
      off();
      delay(1000);
    }
  }*/
  //off();
}

void translateEco(){
  text = text + ECO_CHAR;
  for(int i = 0; i < text.length() && isEcoCorrect(); i++){
    String values = String(int(text.charAt(i)));   
    ecoSended = values;
    ecoReceipt = "";
    if(values.length() == 2){
        colorN('0');
        off();
        delay(1000);
    }
    
    for(int j = 0; j < values.length(); j++){
      char n = values.charAt(j); 
      colorN(n);
      off();
      delay(1000);
    }

    //wait until confirm the value sended is the same receipt
    waitEco();
    if(!isEcoCorrect()){
      Serial.println("Error de Eco: El caracter enviado es distinto al recibido");
    }
  }
  eco = false;
  text.remove(text.length() - 1);
  //In case the remove does not works use: translateInput(ECO_CHAR);
  //and remove the first line of the method.
}

void translateInput(String input){
  for(int i = 0; i < input.length(); i++){
    String values = String(int(input.charAt(i)));   
    Serial.println("Values: " + values);
    if(values.length() == 2){
        colorN('0');
        off();
        delay(1000);
    }
    
    for(int j = 0; j < values.length(); j++){
      char n = values.charAt(j); 
      colorN(n);
      off();
      delay(1000);
    }

    Serial.println(CHAT_CHAR);
    if(CHAT_CHAR.toInt() == values.toInt()){
      Serial.println("Aquí");
      chat = !chat;
    }
  }
}

void waitEco(){
  while(ecoReceipt.length() != 3){
    testLecture();
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
  } //Falta la posibilidad de !chat y otherChat
}

void off(){
  digitalWrite(LEDR, LOW); 
  digitalWrite(LEDG, LOW);
  digitalWrite(LEDB, LOW);
}
