#include <Arduino.h>
#include "MeAuriga.h"
#include "MeRGBLineFollower.h"
#include <SoftwareSerial.h>

// Bluetooth
SoftwareSerial Bluetooth(10, 9); // RX, TX

// Levý motor
const int pwmMotorPravy = 11;
const int inMotorPravy1 = 49;
const int inMotorPravy2 = 48;

// Pravý motor
const int pwmMotorLevy = 10;
const int inMotorLevy1 = 47;
const int inMotorLevy2 = 46;

int normalSpeed = 150;
int aboveNormalSpeed = 180;
int rs = 100;
int rotationSpeed = 200;
int scanningSpeed = 100;
int minRychlost = 110;
int maxRychlost = 255;
int overpassTime=80;
int turnTimeTimeMillis = 2000;

// Ultrazvukovy snimac
// pouziti: vzdalenost = sonar.distanceCm()
MeUltrasonicSensor sonar(PORT_10);

// Snimac cary
// pouziti: linState = RGBLineFollower.getPositionState();
//          lineOffset = RGBLineFollower.getPositionOffset();
MeRGBLineFollower RGBLineFollower(PORT_9);

// Servo
const byte servoPin = 68;
const byte servoMin = 13;
const byte servoMax = 137;
Servo servo;

// narazniky
const byte pravyNaraznik = 67;
const byte levyNaraznik = 62;
volatile boolean narazVpravo = false;
volatile boolean narazVlevo = false;

// promenne pro enkodery
// enkoder pro pravy motor
const byte pravyEnkoderA = 19; // INT2
const byte pravyEnkoderB = 42; // no interrupts 🙁

// enkoder pro levy motor
const byte levyEnkoderA = 18; // INT3
const byte levyEnkoderB = 43; // no interrupts 🙁

const int eckoderPulseNumber = 9;
const float motorGearRatio = 39.267;

volatile long pulseCountVlevo = 0;
volatile long pulseCountVpravo = 0;

// RGB LED ring
const byte numberOfLEDs = 12;
const byte rgbLEDringPin = 44;
#define RINGALLLEDS        0
MeRGBLed ledRing(0, numberOfLEDs );

#define amber      255,194,000
#define orange     255,165,000
#define vermillion 227,066,052
#define red        255,000,000
#define magenta    255,000,255
#define purple     128,000,128
#define indigo     075,000,130
#define blue       000,000,255
#define aquamarine 127,255,212
#define green      000,255,000
#define chartreuse 127,255,000
#define yellow     255,255,000
#define white      000,000,000
#define black      255,255,255


#define leftOnlyCross 0
#define rightOnlyCross  1
#define leftForwardCross 2
#define rightForwardCross 3
#define fullCross 4

// bzučák
const byte buzzerPin = 45;
MeBuzzer buzzer;

// Gyro
MeGyro gyro(1,0x69);

void setup() {
  // nastav piny narazniku
  pinMode(pravyNaraznik,INPUT_PULLUP);
  pinMode(levyNaraznik,INPUT_PULLUP);

  // nastavení ovladacích pinů motorů jako výstupní
  pinMode(pwmMotorPravy,OUTPUT);
  pinMode(inMotorPravy1,OUTPUT);
  pinMode(inMotorPravy2,OUTPUT);

  pinMode(pwmMotorLevy,OUTPUT);
  pinMode(inMotorLevy1,OUTPUT);
  pinMode(inMotorLevy2,OUTPUT);

  // Nastavení frekvencep pwm na 8KHz pro řízení DC motorů
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);

  // inicializace enkoderu
  pinMode(pravyEnkoderA,INPUT_PULLUP);
  pinMode(pravyEnkoderB,INPUT_PULLUP);
  pinMode(levyEnkoderA,INPUT_PULLUP);
  pinMode(levyEnkoderB,INPUT_PULLUP);

  // inicializace obsluhy preruseni od kanalů A enkoderů
  attachInterrupt(digitalPinToInterrupt(pravyEnkoderA),&pravyEncoderAInt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(levyEnkoderA),&levyEncoderAInt, CHANGE);

  // pripoj a omez servo
  servo.attach(servoPin);//,servoMin,servoMax);
  servo.write(90);

  // inicializace RGB LED ringu
  // pro ovládání slouží metoda
  // bool MeRGBLed::setColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
  ledRing.setpin( rgbLEDringPin );
  ledRing.setColor( RINGALLLEDS, 0, 0, 0);
  ledRing.show();

  // nastavení bzučáku
  buzzer.setpin(buzzerPin);
  buzzer.noTone();

  // inicializace gyra
  //gyro.begin();
  
  // inicializace sledovani cary
  RGBLineFollower.begin();
  RGBLineFollower.setKp(1);

  // inicializace sériového kanálu
  Bluetooth.begin(9600);
  Serial.begin(9600);
  Serial.println("Waiting for command...");
  Bluetooth.println("Send 1 to turn on the LED. Send 0 to turn Off");

  while (digitalRead(levyNaraznik)) {
    // nepokracuj dokud neni stiknut levy naraznik
  }
}
int cachedStatesNum = 5;
bool flag_forw = true;
bool flag_turning_left =false;
bool flag_turning_right =false;
int nothing = 0b1111;
int midTwo = 0b1001;
int rightFirst = 0b1101;
int leftFirst = 0b1011;
int all = 0b0000;
int leftActive = 0b0001;
int rightActive = 0b1000;
int linState;
int lineOffset;
int stateCache[8] = {} ;
long loopCounter = 0;

int recognition(int linState){
  stateCache[loopCounter%8] = linState;
  return findMostCommonByte(stateCache, 8);
}

int findMostCommonByte(int arr[], int arrLen) {
  int mostCommonByte = arr[0]; // Assume the first byte is the most common
  int highestFrequency = 1; // Assume the frequency of the most common byte is 1

  for (int i = 0; i < arrLen; i++) {
    int currentFrequency = 1; // Start with a frequency of 1 for the current byte
    for (int j = i+1; j < arrLen; j++) {
      if (arr[i] == arr[j]) {
        currentFrequency++; // Increment the frequency for each matching byte
      }
    }
    if (currentFrequency > highestFrequency) {
      highestFrequency = currentFrequency;
      mostCommonByte = arr[i];
    }
  }

  return mostCommonByte;
}

int thresh = 30;
int rotThresh =30;
String oldState="1111";
String state;

void updateState(int inthresh){
  
  RGBLineFollower.loop();
  int LL = RGBLineFollower.getADCValueRGB1();
  int L = RGBLineFollower.getADCValueRGB2();
  int R = RGBLineFollower.getADCValueRGB3();
  int RR = RGBLineFollower.getADCValueRGB4();
  LL = int(LL >inthresh);
  L = int(L >inthresh);
  R = int(R >inthresh);
  RR = int(RR >inthresh);
  state = String(LL) + String(L) + String(R) + String(RR);
  
 }
void loop() {
  // sejmutí dat z detektoru cary
  updateState(thresh);
  Serial.println(state);
  linState = RGBLineFollower.getPositionState();
  lineOffset = RGBLineFollower.getPositionOffset();
  Serial.println(lineOffset);
  int most_common_state = recognition(linState);
  printData(linState, lineOffset, most_common_state);

  if(state=="1001"){
    onMiddleTwo();
    }else if(state=="0000"){
      onCross();
    }else if(state=="1000" || state=="1100"){
      
      
    }else if(state=="0001" || state=="0011"){
      turn90Left();
    }else if(state=="1011" || state=="0111"){
      regulateLeft(rs);
    }else if(state=="1101"|| state=="1110"){
      regulateRight(rs);
    }else if(state=="1111" && oldState=="1111"){
      turn180();
    }
//    else if(state=="1111" && oldState!="1111"){
//      regulateLeft(normalSpeed);
//    }
//    regBasedOnLineOffset(lineOffset);

    oldState = state;
  switch (linState) {
//    case "1001":
//      onMiddleTwo();
//      break;
//    case "0000":
//      onCross();
//      break;
//    case "1000":
//    case "1100":
//      turn90Left();
//      break;
//    case "0001":
//    case "0011":
//      turn90Right();
//      break;
//    case "1011":
//      regulateRight(normalSpeed);
//      break;
//    case "1101":
//      regulateLeft(normalSpeed);
//      break;
//    case "1111":
//      turn180();
//      break;

  }
    delay(10);
  
  if (Bluetooth.available()){ //wait for data received
    int Data = Bluetooth.read();
    // if(Data=='1'){  
    //   Serial.println("LED On!");
    //   Bluetooth.println("LED On!");
    // }
    // else if(Data=='0'){
    //    Serial.println("LED Off!");
    //    Bluetooth.println("LED  On D13 Off ! ");
    // }
    // else{;}
  }


  loopCounter++;
}

void onCross(){
  turn90Left();
}
void regBasedOnLineOffset(int offset){
  if(offset<0){
    regulateLeft(abs(offset));
    }
   else if(offset>0){
    regulateRight(abs(offset));
    }
  }
void onMiddleTwo(){
    if (flag_forw){
      pravyMotorVpred(normalSpeed);
      levyMotorVpred(normalSpeed);
    } else {
      pravyMotorVzad(normalSpeed);
      levyMotorVzad(normalSpeed);      
    }
 }


void onNothing(){
    driveForward(0);
}
void handleOverpass(){
  int time = millis();
  while((millis()-time)<overpassTime){
    driveForward(minRychlost);
    }
  updateState(thresh);
  if(state == "1111"){
    turn90Right();
    }else{
       driveForward(normalSpeed);
    }
  }


// osetreni preruseni od kanalu A enkoderu na pravem motoru
void pravyEncoderAInt() {
}

// osetreni preruseni od kanalu A enkoderu na levem motoru
void levyEncoderAInt() {
}



void printData(int linState, int lineOffset, int most_common_state){

  Serial.print(0b1111 & linState == true);
  Serial.print("LinState: ");
  Serial.print(linState, BIN);
  Serial.print("\tMost common state: ");
  Serial.print(most_common_state, BIN);

  Serial.print("\tLineOffset: ");
  Serial.println(lineOffset);
}

const long TURN_90_TIME = 250;

void turn90Left(){
  long timeStart = millis();
  while ((millis() - timeStart) < normalSpeed*0.8){
      pravyMotorVpred(minRychlost);
      levyMotorVpred(minRychlost);         
    }

  timeStart = millis();
  while ((millis() - timeStart) < normalSpeed*2.2){
    pravyMotorVpred(rotationSpeed);
    levyMotorVzad(rotationSpeed);         
  }

  timeStart = millis();
  updateState(rotThresh);
  while (state != "1001" && state != "1101" && state != "1011"){
    
    pravyMotorVpred(minRychlost);
    levyMotorVzad(minRychlost); 
    updateState(rotThresh); 
    if((millis() - timeStart) > 2000){
      break;
    }
  }
}
void turn180(){
    int timeStart = millis();
  while ((millis() - timeStart) < normalSpeed*2.4){
    levyMotorVpred(minRychlost);
    pravyMotorVzad(minRychlost);         
  }
  timeStart = millis();
  while (state != "1001" && state != "1101" && state != "1011"){
    levyMotorVpred(minRychlost);
    pravyMotorVzad(minRychlost);  
    updateState(rotThresh);
    if((millis() - timeStart) > 2000){
      break;
    }
  }
  }
void turn90Right(){
  long timeStart = millis();
  while ((millis() - timeStart) < normalSpeed*0.8){
      pravyMotorVpred(minRychlost);
      levyMotorVpred(minRychlost);         
    }

  timeStart = millis();
  while ((millis() - timeStart) < normalSpeed*2.2){
    levyMotorVpred(rotationSpeed);
    pravyMotorVzad(rotationSpeed);         
  }

  timeStart = millis();
  updateState(rotThresh);
  while (state != "1001" && state != "1101" && state != "1011"){
    levyMotorVpred(minRychlost);
    pravyMotorVzad(minRychlost);  
    updateState(rotThresh);
    if((millis() - timeStart) > 2000){
      break;
    }
  }
}



bool checkForMidTwo(){
  if(linState==midTwo){
    flag_turning_right=false;
    flag_turning_left=false;
    }
  }


void driveForward(int speed){
  pravyMotorVpred(speed);
  levyMotorVpred(speed);
}
void driveBackward(int speed){
  pravyMotorVzad(speed);
  levyMotorVzad(speed);
}
void driveLeft(int speed){
  pravyMotorVpred(speed);
  levyMotorVzad(speed);
  }
void driveRight(int speed){
  levyMotorVpred(speed);
  pravyMotorVzad(speed);
  }
void regulateLeft(int speed){
  pravyMotorVpred(speed*1.5);
   levyMotorVpred(speed);
}
void regulateRight(int speed){
  pravyMotorVpred(speed);
   levyMotorVpred(speed*1.5);
}

void levyMotorVpred(int rychlost) {
  digitalWrite(inMotorLevy1, HIGH);
  digitalWrite(inMotorLevy2, LOW);
  analogWrite(pwmMotorLevy, rychlost);
}

void levyMotorVzad(int rychlost) {
  digitalWrite(inMotorLevy1, LOW);
  digitalWrite(inMotorLevy2, HIGH);
  analogWrite(pwmMotorLevy, rychlost);
}

void levyMotorStop() {
  analogWrite(pwmMotorLevy, 0);
}

void pravyMotorVpred(int rychlost) {
  digitalWrite(inMotorPravy1, LOW);
  digitalWrite(inMotorPravy2, HIGH);
  analogWrite(pwmMotorPravy, rychlost);
}

void pravyMotorVzad(int rychlost) {
  digitalWrite(inMotorPravy1, HIGH);
  digitalWrite(inMotorPravy2, LOW);
  analogWrite(pwmMotorPravy, rychlost);
}

void pravyMotorStop() {
  analogWrite(pwmMotorPravy, 0);
}
//int runCrossIntersectionScan(){
//    int starttime = millis();
//    while ((millis() - starttime) <=overPassTimeMillis){
//      driveForward(scanningSpeed);
//    }
//    starttime = millis();
//    while ((millis() - starttime) <=overPassTimeMillis){
//      driveBackward(scanningSpeed);
//    }
//}
