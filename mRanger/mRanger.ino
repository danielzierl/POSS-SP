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

int rychlostJizdy = 200;
int minRychlost = 100;
int maxRychlost = 255;

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
const byte pravyEnkoderB = 42; // no interrupts :(

// enkoder pro levy motor
const byte levyEnkoderA = 18; // INT3
const byte levyEnkoderB = 43; // no interrupts :(

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
  delay(500);
}

bool flag_rovne = true;
void loop() {
  // sejmutí dat z detektoru cary
  RGBLineFollower.loop();

  delay(10);

  int linState = RGBLineFollower.getPositionState();
  int lineOffset = RGBLineFollower.getPositionOffset();

  int nothing = 0b1111;
  int midTwo = 0b1001;
  int rightFirst = 0b1101;
  int leftFirst = 0b1011;
  int all = 0b0000;

  printData(linState, lineOffset);
  if (linState == midTwo){
    if (flag_rovne){
      pravyMotorVpred(110);
      levyMotorVpred(110);
    } else {
      pravyMotorVzad(110);
      levyMotorVzad(110);      
    }
  } else{
    if ((linState == 0b0001) || (linState == 0b0000)){
      turn90Left();
      delay(1000);
    }
  }

  if (linState == rightFirst){
    pravyMotorVpred(130);
    levyMotorVpred(100);
  }

  if (linState == leftFirst){
    pravyMotorVpred(100);
    levyMotorVpred(130);
  }


}
// osetreni preruseni od kanalu A enkoderu na pravem motoru
void pravyEncoderAInt() {
}

// osetreni preruseni od kanalu A enkoderu na levem motoru
void levyEncoderAInt() {
}



void printData(int linState, int lineOffset){
  Serial.print("LinState: ");
  Serial.print(linState, BIN);
  Serial.print("\tLineOffset: ");
  Serial.println(lineOffset);
}

const long TURN_90_TIME = 250;
void turn90Left(){
  long timeStart = millis();

  while ((millis() - timeStart) < 250){
    pravyMotorVpred(150);
    levyMotorVzad(150);         
  }

  while (RGBLineFollower.getPositionState() != 0b1001){
    pravyMotorVpred(100);
    levyMotorVzad(100);  
    if (RGBLineFollower.getPositionState()== 0b1001){
      pravyMotorVpred(0);
    levyMotorVzad(0);   
      break;
    }  
  }
}


void turn90Right(){
  long timeStart = millis();


  while ((millis() - timeStart) < TURN_90_TIME){
    levyMotorVpred(maxRychlost);
    pravyMotorVzad(maxRychlost);         
  }

  while (RGBLineFollower.getPositionState() != 0b1001){
    levyMotorVpred(minRychlost);
    pravyMotorVzad(minRychlost);    
    if (RGBLineFollower.getPositionState()== 0b1001){
      levyMotorVpred(0);
      pravyMotorVzad(0);    
      break;
    }  
  }
}

void turn180Left(){
  long timeStart = millis();

  while ((millis() - timeStart) < 2*TURN_90_TIME){
    pravyMotorVpred(maxRychlost);
    levyMotorVzad(maxRychlost);         
  }

  //while (RGBLineFollower.getPositionState() != 0b1001){
  //pravyMotorVpred(minRychlost);
  //levyMotorVzad(minRychlost);      
  //}
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

bool checkBumper(){
  return (digitalRead(levyNaraznik) || digitalRead(pravyNaraznik));
}
