#include <Arduino.h>
#include "MeAuriga.h"
#include "MeRGBLineFollower.h"

// Levý motor
const int pwmMotorPravy = 11;
const int inMotorPravy1 = 49;
const int inMotorPravy2 = 48;

// Pravý motor
const int pwmMotorLevy = 10;
const int inMotorLevy1 = 47;
const int inMotorLevy2 = 46;

int normalSpeed = 100;
int aboveNormalSpeed = 130;
int scanningSpeed = 100;
int minRychlost = 100;
int maxRychlost = 255;
int overPassTimeMillis=500;
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
  Serial.begin(9600);

  while (digitalRead(levyNaraznik)) {
    // nepokracuj dokud neni stiknut levy naraznik
  }
}

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
void loop() {
  // sejmutí dat z detektoru cary
  RGBLineFollower.loop();

  delay(10);

  linState = RGBLineFollower.getPositionState();
  lineOffset = RGBLineFollower.getPositionOffset();

  printData(linState, lineOffset);
//  onMiddleTwo();
//  onOneDeviation();
//  onNothing();
  onCross();
  





  
  
}
void onCross(){
    if (linState == all || linState == leftActive || flag_turning_left ){
//    pravyMotorVpred(aboveNormalSpeed);
//    levyMotorVzad(aboveNormalSpeed);
//      runCrossIntersectionScan();
      turnLeft();
  }
  if(linState == rightActive || flag_turning_right){
      turnRight();
    }
}

void onMiddleTwo(){
  if (linState == midTwo){
    if (flag_forw){
      pravyMotorVpred(normalSpeed);
      levyMotorVpred(normalSpeed);
    } else {
      pravyMotorVzad(normalSpeed);
      levyMotorVzad(normalSpeed);      
    }
  }
 }

void onOneDeviation(){
    if (linState == rightFirst){
      regulateLeft();
  }

  if (linState == leftFirst){
    regulateRight();
  }
}
void onNothing(){
  if (linState == nothing){
    driveForward(0);
  }
}


// osetreni preruseni od kanalu A enkoderu na pravem motoru
void pravyEncoderAInt() {
}

// osetreni preruseni od kanalu A enkoderu na levem motoru
void levyEncoderAInt() {
}



void printData(int linState, int lineOffset){

  Serial.print(0b1111 & linState == true);
  Serial.print("LinState: ");
  Serial.print(linState, BIN);
  Serial.print("\tLineOffset: ");
  Serial.println(lineOffset);
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
void regulateLeft(){
  pravyMotorVpred(aboveNormalSpeed);
   levyMotorVpred(normalSpeed);
}
void regulateRight(){
  pravyMotorVpred(normalSpeed);
   levyMotorVpred(aboveNormalSpeed);
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

int turnLeft(){
  flag_turning_left=true;
  int starttime = millis();
  while ((millis() - starttime) <=turnTimeTimeMillis){
    driveLeft(scanningSpeed);
    checkForMidTwo();
   
  }
  flag_turning_left=false;
  driveForward(0);
 }
int turnRight(){
  int starttime = millis();
  flag_turning_right=true;
  while ((millis() - starttime) <=turnTimeTimeMillis){
    driveRight(scanningSpeed);
    checkForMidTwo();
  }
  flag_turning_right=false;
  driveForward(0);
 }
void checkForMidTwo(){
  if(linState==midTwo){
    flag_turning_right=false;
    flag_turning_left=false;
    }
  }
