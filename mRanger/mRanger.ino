// NAS ROBOT JE CISLO 8
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
const byte pravyEnkoderA = 19;  // INT2
const byte pravyEnkoderB = 42;  // no interrupts :(

// enkoder pro levy motor
const byte levyEnkoderA = 18;  // INT3
const byte levyEnkoderB = 43;  // no interrupts :(

const int eckoderPulseNumber = 9;
const float motorGearRatio = 39.267;
const float prevodovyPomer = 758.1;  // IDK musim upravit //od kuby
const float prevodniKonstanta = (TWO_PI * motorGearRatio) / (prevodovyPomer * eckoderPulseNumber);
volatile float ujetoVpravo = 0;
volatile float ujetoVlevo = 0;

volatile long pulseCountVlevo = 0;
volatile long pulseCountVpravo = 0;

volatile byte levyStateA, oldLevyStateA;  //od kuby
volatile byte pravyStateA, oldPravyStateA;

// RGB LED ring
const byte numberOfLEDs = 12;
const byte rgbLEDringPin = 44;
#define RINGALLLEDS 0
MeRGBLed ledRing(0, numberOfLEDs);

#define amber 255, 194, 000
#define orange 255, 165, 000
#define vermillion 227, 066, 052
#define red 255, 000, 000
#define magenta 255, 000, 255
#define purple 128, 000, 128
#define indigo 075, 000, 130
#define blue 000, 000, 255
#define aquamarine 127, 255, 212
#define green 000, 255, 000
#define chartreuse 127, 255, 000
#define yellow 255, 255, 000
#define white 000, 000, 000
#define black 255, 255, 255

// bzučák
const byte buzzerPin = 45;
MeBuzzer buzzer;

//coffin
#define NOTE_B0 31
#define NOTE_C1 33
#define NOTE_CS1 35
#define NOTE_D1 37
#define NOTE_DS1 39
#define NOTE_E1 41
#define NOTE_F1 44
#define NOTE_FS1 46
#define NOTE_G1 49
#define NOTE_GS1 52
#define NOTE_A1 55
#define NOTE_AS1 58
#define NOTE_B1 62
#define NOTE_C2 65
#define NOTE_CS2 69
#define NOTE_D2 73
#define NOTE_DS2 78
#define NOTE_E2 82
#define NOTE_F2 87
#define NOTE_FS2 93
#define NOTE_G2 98
#define NOTE_GS2 104
#define NOTE_A2 110
#define NOTE_AS2 117
#define NOTE_B2 123
#define NOTE_C3 131
#define NOTE_CS3 139
#define NOTE_D3 147
#define NOTE_DS3 156
#define NOTE_E3 165
#define NOTE_F3 175
#define NOTE_FS3 185
#define NOTE_G3 196
#define NOTE_GS3 208
#define NOTE_A3 220
#define NOTE_AS3 233
#define NOTE_B3 247
#define NOTE_C4 262
#define NOTE_CS4 277
#define NOTE_D4 294
#define NOTE_DS4 311
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_FS4 370
#define NOTE_G4 392
#define NOTE_GS4 415
#define NOTE_A4 440
#define NOTE_AS4 466
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_CS5 554
#define NOTE_D5 587
#define NOTE_DS5 622
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_FS5 740
#define NOTE_G5 784
#define NOTE_GS5 831
#define NOTE_A5 880
#define NOTE_AS5 932
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_CS6 1109
#define NOTE_D6 1175
#define NOTE_DS6 1245
#define NOTE_E6 1319
#define NOTE_F6 1397
#define NOTE_FS6 1480
#define NOTE_G6 1568
#define NOTE_GS6 1661
#define NOTE_A6 1760
#define NOTE_AS6 1865
#define NOTE_B6 1976
#define NOTE_C7 2093
#define NOTE_CS7 2217
#define NOTE_D7 2349
#define NOTE_DS7 2489
#define NOTE_E7 2637
#define NOTE_F7 2794
#define NOTE_FS7 2960
#define NOTE_G7 3136
#define NOTE_GS7 3322
#define NOTE_A7 3520
#define NOTE_AS7 3729
#define NOTE_B7 3951
#define NOTE_C8 4186
#define NOTE_CS8 4435
#define NOTE_D8 4699
#define NOTE_DS8 4978

int melody[] = {
  NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_DS5,
  NOTE_E5, NOTE_B4, NOTE_D5, NOTE_C5,
  NOTE_A4, NOTE_C4, NOTE_E4, NOTE_A4,
  NOTE_B4, NOTE_E4, NOTE_GS4, NOTE_B4,
  
  NOTE_C5, NOTE_E4, NOTE_E5, NOTE_DS5,
  NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_B4,
  NOTE_D5, NOTE_C5, NOTE_A4, NOTE_C4,
  NOTE_E4, NOTE_A4, NOTE_B4, NOTE_E4,
  
  NOTE_C5, NOTE_B4, NOTE_A4, NOTE_B4,
  NOTE_C5, NOTE_D5, NOTE_E5, NOTE_G4,
  NOTE_F5, NOTE_E5, NOTE_D5, NOTE_F4,
  NOTE_E5, NOTE_D5, NOTE_C5, NOTE_E4,
  
  NOTE_D5, NOTE_C5, NOTE_B4, NOTE_E4,
  NOTE_E5, NOTE_E4, NOTE_E5, NOTE_E4,
  NOTE_E5, NOTE_E4, NOTE_E5, NOTE_DS4,
  NOTE_E5, NOTE_D4, NOTE_E5, NOTE_DS4,
  
  NOTE_E5, NOTE_B4, NOTE_D5, NOTE_C5,
  NOTE_A4, NOTE_C4, NOTE_E4, NOTE_A4,
  NOTE_B4, NOTE_E4, NOTE_GS4, NOTE_B4,
  NOTE_C5, NOTE_E4, NOTE_E5, NOTE_DS5,

  NOTE_E5, NOTE_DS5, NOTE_E5, NOTE_B4,
  NOTE_D5, NOTE_C5, NOTE_A4, NOTE_C4,
  NOTE_E4, NOTE_A4, NOTE_B4, NOTE_E4,
  NOTE_C5, NOTE_B4, NOTE_A4, 0,
};
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  9, 9, 9, 9,
  9, 9, 9, 9,
  3, 9, 9, 9,
  3, 9, 9, 9,

  3, 9, 9, 9,
  9, 9, 9, 9,
  9, 9, 3, 9,
  9, 9, 3, 9,

  9, 9, 3, 9,
  9, 9, 3, 9,
  9, 9, 3, 9,
  9, 9, 3, 9,

  9, 9, 9, 9,
  9, 9, 9, 9,
  9, 9, 9, 9,
  9, 9, 9, 9,

  9, 9, 9, 9,
  3, 9, 9, 9,
  3, 9, 9, 9,
  3, 9, 9, 9,

  9, 9, 9, 9,
  9, 9, 3, 9,
  9, 9, 3, 8,
  8, 8, 1, 9,
};

// Gyro
MeGyro gyro(1, 0x69);

void setup() {
  // nastav piny narazniku
  pinMode(pravyNaraznik, INPUT_PULLUP);
  pinMode(levyNaraznik, INPUT_PULLUP);

  // nastavení ovladacích pinů motorů jako výstupní
  pinMode(pwmMotorPravy, OUTPUT);
  pinMode(inMotorPravy1, OUTPUT);
  pinMode(inMotorPravy2, OUTPUT);

  pinMode(pwmMotorLevy, OUTPUT);
  pinMode(inMotorLevy1, OUTPUT);
  pinMode(inMotorLevy2, OUTPUT);

  // Nastavení frekvencep pwm na 8KHz pro řízení DC motorů
  TCCR1A = _BV(WGM10);
  TCCR1B = _BV(CS11) | _BV(WGM12);

  TCCR2A = _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);

  // inicializace enkoderu
  pinMode(pravyEnkoderA, INPUT_PULLUP);
  pinMode(pravyEnkoderB, INPUT_PULLUP);
  pinMode(levyEnkoderA, INPUT_PULLUP);
  pinMode(levyEnkoderB, INPUT_PULLUP);

  // inicializace obsluhy preruseni od kanalů A enkoderů
  attachInterrupt(digitalPinToInterrupt(pravyEnkoderA), &pravyEncoderAInt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(levyEnkoderA), &levyEncoderAInt, CHANGE);
  oldPravyStateA = digitalRead(pravyEnkoderA);  //od kuby, nevím jestli je potřeba
  oldLevyStateA = digitalRead(levyEnkoderA);

  // pripoj a omez servo
  servo.attach(servoPin);  //,servoMin,servoMax);
  servo.write(90);

  // inicializace RGB LED ringu
  // pro ovládání slouží metoda
  // bool MeRGBLed::setColor(uint8_t index, uint8_t red, uint8_t green, uint8_t blue)
  ledRing.setpin(rgbLEDringPin);
  ledRing.setColor(RINGALLLEDS, 0, 0, 0);
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
    //nepokračuj dokud není stisknut levý nárazník
  }
}

int distance = 0;

int vlevo = 0;
int vpravo = 0;
int finale_status = 0;
int rovne = 0;
int rovne2 = 0;
unsigned long timerSonar = 0;
unsigned long timerVlevo = 0;
unsigned long timerVpravo = 0;
unsigned long timerCil = 0;
unsigned long timerRovne = 0;
unsigned long timerIndex = 0;

unsigned long timerXLLL = 0;

int x = 0;
int y = 0;
int otocka = 0;

int oldLevaVzdalenost = 0;
int oldPravaVzdalenost = 0;
int RRR = 0;
int LLL = 0;
int CCC = 0;
int FFF = 0;

int XLLL = 0;

int LED = 0;

//String way = String("FRLLRRFLLRC");
//int race=1;
String way = String("");
int race = 0;
unsigned int raceIndex = 0;
int changeIndex = 0;
char dir = 'F';  //inicializace musí být vždy na něco, v tomto úžasném jazyce nejde prealocovat prázdný char :) :)

int first_crossing_state = 0;

int sensor_threshold = 25;         //25 ideál pro roboty 11 a 8 //28 se šmouhou 45 u robota číslo 9 //18 až 25 se šmouhou 45 u robota číslo 8
int smouha = 40;       //45
int left_threshold = 17;     //22 //24 //levá čára je 9 až 19, takže přes 20 nedávat (robot 2) //17!! //14 ve tmě
int smouhaLeft = 38;   //25 //20 //30
int prahRight = 28;    //22 //24
int smouhaRight = 10;  //25 //20

int raceSpeed = 200;            //190
int dorovnavaciKonstanta = 55;  //40
////////////////////





int * updateRGBFollower(){
  RGBLineFollower.loop();
  int LL = RGBLineFollower.getADCValueRGB1();
  int L = RGBLineFollower.getADCValueRGB2();
  int R = RGBLineFollower.getADCValueRGB3();
  int RR = RGBLineFollower.getADCValueRGB4();
  static int values[] = {LL, L, R, RR};
  return values;
}

void runLED(uint8_t R, uint8_t G, uint8_t B){
  ledRing.setColor(RINGALLLEDS, R, G, B);
  ledRing.show();
  LED = 0;
}

void loop() {
  LED++;
  if (LED > 2000) {
    runLED(white);
  }

  RGBLineFollower.loop();
  int LL = RGBLineFollower.getADCValueRGB1();
  int L = RGBLineFollower.getADCValueRGB2();
  int R = RGBLineFollower.getADCValueRGB3();
  int RR = RGBLineFollower.getADCValueRGB4();

  if (race == 0) {
    if (first_crossing_state == 0) {
      levyMotorVpred(110);   //80 víc ne
      pravyMotorVpred(112);  //80
      delay(1000);
      first_crossing_state = 1;
    }

    if (LL < sensor_threshold && L < sensor_threshold && R < sensor_threshold && RR < sensor_threshold && finale_status == 0 && CCC == 0) {  //fake
      timerCil = millis();
      finale_status = 1;
    }
    if (LL < sensor_threshold && L < sensor_threshold && R < sensor_threshold && RR < sensor_threshold && millis() - timerCil > smouha) {
      zjistiCil();
    }

    if (LL < left_threshold && vlevo == 0 && XLLL == 0) {  //jeden fake alarm
      timerVlevo = millis();
      vlevo = 1;
    }
    if (LL < left_threshold && millis() - timerVlevo > smouhaLeft && vlevo == 1) {  //když můžeš zatoč vlevo
      x = 1;                                                                  //vypni zatáčení doprava

      zjistiCil();
      timerVlevo = 0;
      vlevo = 0;
      LLL = 1;
      RRR = 0;
      CCC = 0;
      FFF = 0;
    } else if (LL > sensor_threshold && L < sensor_threshold && R < sensor_threshold) {  // když to nejde doleva tak rovně - oba stredy na care x00x
      if (LLL == 1) {
        runLED(green);
        changeDir('L');
      }
      LLL = 0;
      if (RRR == 1) {
        int newLevaVzdalenost = pulseCountVlevo * prevodniKonstanta;
        //Serial.println(newLevaVzdalenost - oldLevaVzdalenost);
        if (newLevaVzdalenost - oldLevaVzdalenost > 70) {  //80 míň ne //75 s upraveným točením
          runLED(white);
          changeDir('T');
        } else if (newLevaVzdalenost - oldLevaVzdalenost > 20) {  //15
          runLED(red);

          changeDir('R');
        } else {
          //fejk
        }
      }
      RRR = 0;

      if (RR < sensor_threshold && rovne == 0) {
        timerRovne = millis();
        rovne = 1;
        FFF = 1;
      }
      if (RR < sensor_threshold && millis() - timerRovne > smouha) {  //ošetřeno aby se nespouštělo při zatáčce vpravo (maybe)
        FFF = 1;
      }
      if (FFF == 1 && millis() - timerRovne > smouha * 5) {
        rovne = 0;
        FFF = 0;

        runLED(purple);
        changeDir('F');
      }

      levyMotorVpred(110);   //80 víc ne
      pravyMotorVpred(110);  //80
      x = 0;
      otocka = 0;
    } else if (LL > sensor_threshold && L > sensor_threshold && R > sensor_threshold && RR > sensor_threshold && x == 0 && vpravo == 0 && otocka == 0) {  //fake
      timerVpravo = millis();
      vpravo = 1;
    } else if (LL > sensor_threshold && L > sensor_threshold && R > sensor_threshold && RR > sensor_threshold && x == 0 && millis() - timerVpravo > smouha) {  //mimo čáru toč vpravo
      if (RRR == 0) {
        oldLevaVzdalenost = pulseCountVlevo * prevodniKonstanta;
      }
      stop();
      R90();
      timerVpravo = 0;
      vpravo = 0;
      LLL = 0;
      RRR = 1;
      CCC = 0;
      FFF = 0;
      rovne = 0;
    } else if (L < sensor_threshold && R > sensor_threshold) {  //pritoc doleva //x01x
      levyMotorVpred(80);
      pravyMotorVpred(150);  //130
      x = 0;
    } else if (L > sensor_threshold && R < sensor_threshold) {  //pritoc doprava //x10x
      levyMotorVpred(150);              //130
      pravyMotorVpred(80);
      x = 0;
    } else if (LL < sensor_threshold && L > sensor_threshold && R > sensor_threshold && RR > sensor_threshold && x == 0) {  //na care je pouze LL -> hodně přitoč doleva
      levyMotorVzad(80);
      pravyMotorVpred(160);
    } else if (LL > sensor_threshold && L > sensor_threshold && R > sensor_threshold && RR < sensor_threshold && x == 0) {  //na care je pouze RR -> hodně přitoč doprava
      levyMotorVpred(160);
      pravyMotorVzad(80);
    }
  }
  //=============================RACE=================================================================
  else if (race == 1) {
    if (digitalRead(levyNaraznik) == false) {  //levý nárazník v průběhu závodu ho stopne
      stop();
      reinicializace();
      while (digitalRead(pravyNaraznik)) {
        //nepokračuj dokud není stisknut pravý nárazník
      }
      levyMotorVpred(100);
      pravyMotorVpred(100);
      runLED(orange);
      LED = 0;
      return;
    }

    if (first_crossing_state == 0) {
      LLL = 0;
      first_crossing_state = 1;
      dir = way.charAt(raceIndex);
      levyMotorVpred(110);   //80 víc ne
      pravyMotorVpred(112);  //80
      delay(1000);
    }
    dir = way.charAt(raceIndex);
    Serial.print(raceIndex);
    Serial.print(':');
    Serial.println(dir);

    if (dir == 'L') {  //porovnávání stringů ani charů vůbec nefunguje naprosto napíču nevim co s tím
      Serial.print('L');
      Serial.print('L');
      Serial.print('-');
      Serial.print(LL);
      Serial.print('-');
      if (LL < left_threshold && vlevo == 0 && XLLL == 0) {  //jeden fake alarm
        timerVlevo = millis();
        vlevo = 1;
      }
      if (LL < left_threshold && millis() - timerVlevo > smouhaLeft / 3 && vlevo == 1) {  //když můžeš zatoč vlevo
        stop();
        delay(100);
        zjistiCilR();  //obsahuje L90
        timerVlevo = 0;
        vlevo = 0;
        LLL = 1;
      }
    }
    if (dir == 'F') {
      if ((RR < prahRight || LL < left_threshold) && rovne == 0) {  //fake
        timerRovne = millis();
        rovne = 1;
      }
      if ((RR < prahRight && millis() - timerRovne > smouhaRight / 3) || (LL < left_threshold && millis() - timerRovne > smouhaLeft / 3)) {  //ví, že má jet rovně, takže musí zjistit, že projel přípojkou, jedno zda levou či pravou
        rovne2 = 1;
      }
      if (rovne2 == 1 && millis() - timerRovne > smouha * 5) {  //ošetření aby se tenhle paznecht nepouštěl dřív než má
        FFF = 1;
        rovne = 0;
        rovne2 = 0;
      }
    }
    if (dir == 'R') {
      Serial.print('R');
      Serial.print('R');
      Serial.print('-');
      Serial.print(RR);
      Serial.print('-');
      if (RR < prahRight && vpravo == 0) {  //fake
        timerVpravo = millis();
        vpravo = 1;
      }
      if (RR < prahRight && millis() - timerVpravo > smouhaRight / 3) {  //když můžeš vpravo a máš jet vpravo jeď vpravo
        stop();
        delay(100);
        R_90R(LL, L, R, RR);
        timerVpravo = 0;
        vpravo = 0;
        RRR = 1;
      }
    }

    if ((dir == 'C') || (raceIndex >= way.length())) {
      if (LL < sensor_threshold && L < sensor_threshold && R < sensor_threshold && RR < sensor_threshold && finale_status == 0) {  //fake
        timerCil = millis();
        finale_status = 1;
      }
      if (LL < sensor_threshold && L < sensor_threshold && R < sensor_threshold && RR < sensor_threshold && millis() - timerCil > smouha) {
        zjistiCil();
        finale_status = 0;
      }
    }

    if (L < sensor_threshold && R < sensor_threshold) {  //sleduj čáru
      if (LLL == 1) {
        Serial.print(raceIndex);
        Serial.print(':');
        Serial.println('L');
        runLED(green);
        raceIndex++;
      }
      if (RRR == 1) {
        Serial.print(raceIndex);
        Serial.print(':');
        Serial.println('R');
        runLED(red);
        raceIndex++;
      }
      if (FFF == 1) {
        Serial.print(raceIndex);
        Serial.print(':');
        Serial.println('F');
        runLED(indigo);
        raceIndex++;
      }
      LLL = 0;
      RRR = 0;
      FFF = 0;

      levyMotorVpred(raceSpeed);   //190
      pravyMotorVpred(raceSpeed);  //190
    }
    if (L < sensor_threshold && R > sensor_threshold) {  //pritoc doleva //x01x
      levyMotorVpred(raceSpeed - dorovnavaciKonstanta);
      pravyMotorVpred(raceSpeed + dorovnavaciKonstanta);
    }
    if (L > sensor_threshold && R < sensor_threshold) {  //pritoc doprava //x10x
      levyMotorVpred(raceSpeed + dorovnavaciKonstanta);
      pravyMotorVpred(raceSpeed - dorovnavaciKonstanta);
    }
    if (LL < left_threshold && L > sensor_threshold && R > sensor_threshold && RR > prahRight) {  //na care je pouze LL -> hodně přitoč doleva
      levyMotorVzad(255);
      pravyMotorVpred(raceSpeed + dorovnavaciKonstanta);
    }
    if (LL > left_threshold && L > sensor_threshold && R > sensor_threshold && RR < prahRight) {  //na care je pouze RR -> hodně přitoč doprava
      levyMotorVpred(255);
      pravyMotorVzad(raceSpeed - 2 * dorovnavaciKonstanta);
    }
  }

  //============================================================
  //čištění fejkStacku nebo jak to nazvat xd - na konci loopu
  if (millis() - timerVlevo > smouha + 2) {
    timerVlevo = 0;
    vlevo = 0;
  }
  if (millis() - timerVpravo > smouha + 2) {
    timerVpravo = 0;
    vpravo = 0;
  }
  if (millis() - timerCil > smouha + 2) {
    timerCil = 0;
    finale_status = 0;
  }

  if (millis() - timerRovne > smouha * 5 + 2) {
    timerRovne = 0;
    rovne = 0;
  }
  if (millis() - timerXLLL > 500) {
    XLLL = 0;
  }
}

//////////

void stop() {
  levyMotorStop();
  pravyMotorStop();
}

void L90(int LL, int L, int R, int RR) {
  levyMotorVzad(150);    //150 //140
  pravyMotorVpred(150);  //150
  unsigned long pockej = millis();
  while (millis() - pockej < 250) {  //200
    //misto delaye
  }
  while (!(LL > left_threshold && L < sensor_threshold && R < sensor_threshold && RR > prahRight)) {
    RGBLineFollower.loop();
    LL = RGBLineFollower.getADCValueRGB1();
    L = RGBLineFollower.getADCValueRGB2();
    R = RGBLineFollower.getADCValueRGB3();
    RR = RGBLineFollower.getADCValueRGB4();
  }
  if (LL > left_threshold && L < sensor_threshold && R < sensor_threshold && RR > prahRight) {
    RGBLineFollower.loop();
    LL = RGBLineFollower.getADCValueRGB1();
    L = RGBLineFollower.getADCValueRGB2();
    R = RGBLineFollower.getADCValueRGB3();
    RR = RGBLineFollower.getADCValueRGB4();
    while (!(LL > left_threshold && L < sensor_threshold && R < sensor_threshold && RR > prahRight)) {
      RGBLineFollower.loop();
      LL = RGBLineFollower.getADCValueRGB1();
      L = RGBLineFollower.getADCValueRGB2();
      R = RGBLineFollower.getADCValueRGB3();
      RR = RGBLineFollower.getADCValueRGB4();
    }
  }

  XLLL = 1;
  timerXLLL = millis();
}

void R90() {
  levyMotorVpred(140);  //140
  pravyMotorVzad(147);  //140
}

void R_90(int LL, int L, int R, int RR) {
  stop();
  unsigned long pockej = millis();
  while (millis() - pockej < 100) {
    //misto delaye
  }
  levyMotorVpred(80);
  pravyMotorVpred(80);
  pockej = millis();
  while (millis() - pockej < 350 - smouha) {  //350 //550
    //misto delaye
  }
  stop();
  while (millis() - pockej < 100) {
    //misto delaye
  }

  levyMotorVpred(140);  //140
  pravyMotorVzad(147);  //140

  pockej = millis();                 //vyhodí ho z dráhy
  while (millis() - pockej < 250) {  //200
    //misto delaye
  }

  while (!(LL > left_threshold && L < sensor_threshold && R < sensor_threshold && RR > prahRight)) {  //toč dokud nenarazíš na čáru
    RGBLineFollower.loop();
    LL = RGBLineFollower.getADCValueRGB1();
    L = RGBLineFollower.getADCValueRGB2();
    R = RGBLineFollower.getADCValueRGB3();
    RR = RGBLineFollower.getADCValueRGB4();
  }
  if (LL > left_threshold && L < sensor_threshold && R < sensor_threshold && RR > prahRight) {
    RGBLineFollower.loop();
    LL = RGBLineFollower.getADCValueRGB1();
    L = RGBLineFollower.getADCValueRGB2();
    R = RGBLineFollower.getADCValueRGB3();
    RR = RGBLineFollower.getADCValueRGB4();
    while (!(LL > left_threshold && L < sensor_threshold && R < sensor_threshold && RR > prahRight)) {
      RGBLineFollower.loop();
      LL = RGBLineFollower.getADCValueRGB1();
      L = RGBLineFollower.getADCValueRGB2();
      R = RGBLineFollower.getADCValueRGB3();
      RR = RGBLineFollower.getADCValueRGB4();
    }
  }
  levyMotorVpred(80);
  pravyMotorVpred(80);
}

void R180() {
  stop();
  delay(500);
  levyMotorVzad(80);
  pravyMotorVzad(80);
  delay(600);
  levyMotorVpred(127);
  pravyMotorVzad(127);
  delay(1400);  //1400
  stop();
}

void zjistiCil() {
  stop();
  unsigned long pockej = millis();
  while (millis() - pockej < 100) {
    //misto delaye
  }
  levyMotorVpred(80);
  pravyMotorVpred(80);
  pockej = millis();
  while (millis() - pockej < 450 - smouha) {  //350 //550
    //misto delaye
  }
  stop();
  while (millis() - pockej < 100) {
    //misto delaye
  }
  RGBLineFollower.loop();
  int LL = RGBLineFollower.getADCValueRGB1();
  int L = RGBLineFollower.getADCValueRGB2();
  int R = RGBLineFollower.getADCValueRGB3();
  int RR = RGBLineFollower.getADCValueRGB4();
  if (LL < left_threshold && L < sensor_threshold && R < sensor_threshold && RR < prahRight) {
    runLED(yellow);
    findShortest();
    if (race == 1) {
      runLED(yellow);
      coffin();
      reinicializace();
      while (digitalRead(pravyNaraznik)) {
        //nepokračuj dokud není stisknut pravý nárazník
      }
      levyMotorVpred(100);
      pravyMotorVpred(100);
      runLED(aquamarine);
      return;
    }
    buzzer.tone(165, 500);
    reinicializace();
    while (digitalRead(pravyNaraznik)) {
      //nepokračuj dokud není stisknut pravý nárazník
    }
    levyMotorVpred(100);
    pravyMotorVpred(100);
    runLED(aquamarine);
    return;  //vrátí se do loopu
  } else {
    RGBLineFollower.loop();
    int LL = RGBLineFollower.getADCValueRGB1();
    int L = RGBLineFollower.getADCValueRGB2();
    int R = RGBLineFollower.getADCValueRGB3();
    int RR = RGBLineFollower.getADCValueRGB4();
    if (LL < left_threshold && L < sensor_threshold && R < sensor_threshold && RR < prahRight) {
      runLED(yellow);
      findShortest();
      if (race == 1) {
        runLED(yellow);
        coffin();
        reinicializace();
        while (digitalRead(pravyNaraznik)) {
          //nepokračuj dokud není stisknut pravý nárazník
        }
        levyMotorVpred(100);
        pravyMotorVpred(100);
        runLED(aquamarine);
        return;
      }
      buzzer.tone(165, 500);
      reinicializace();
      while (digitalRead(pravyNaraznik)) {
        //nepokračuj dokud není stisknut pravý nárazník
      }
      levyMotorVpred(100);
      pravyMotorVpred(100);
      runLED(aquamarine);
      return;  //vrátí se do loopu
    } else if (XLLL == 0) {
      L90(LL, L, R, RR);  //jen pokus, že by se nevracel ale rovnou za křižovatkou zatočil - lepší než předtím
      //sem NEdávej LLL=1 ty idiote
    }
  }
}

void changeDir(char change) {
  way.concat(change);
}
void findShortest() {
  Serial.println(way);
  int zmena = 1;
  while (zmena == 1) {
    String oldWay = way;
    way.replace("LTL", "F");
    way.replace("LTF", "R");
    way.replace("RTL", "T");
    way.replace("FTL", "R");
    way.replace("FTF", "T");
    way.replace("LTR", "T");
    if (way == oldWay) { zmena = 0; }
  }
  way.concat('C');
  for (int i = 0; i < way.length(); i++) {
    if (way.charAt(i) == 'L') {
      ledRing.setColor(RINGALLLEDS, red);
      ledRing.show();
    }
    if (way.charAt(i) == 'R') {
      ledRing.setColor(RINGALLLEDS, green);
      ledRing.show();
    }
    if (way.charAt(i) == 'F') {
      ledRing.setColor(RINGALLLEDS, purple);
      ledRing.show();
    }
    if (way.charAt(i) == 'T') {
      ledRing.setColor(RINGALLLEDS, blue);
      ledRing.show();
    }
    if (way.charAt(i) == 'C') {
      ledRing.setColor(RINGALLLEDS, yellow);
      ledRing.show();
    }
    delay(300);
  }
  Serial.println(way);
}

void reinicializace() {
  race = 1;
  raceIndex = 0;

  vlevo = 0;
  vpravo = 0;
  finale_status = 0;
  rovne = 0;
  rovne2 = 0;
  timerSonar = 0;
  timerVlevo = 0;
  timerVpravo = 0;
  timerCil = 0;
  timerRovne = 0;
  x = 0;
  otocka = 0;
  oldLevaVzdalenost = 0;
  oldPravaVzdalenost = 0;
  RRR = 0;
  LLL = 0;
  CCC = 0;
  FFF = 0;
  LED = 0;

  first_crossing_state = 0;
}

/////////////======================================

// osetreni preruseni od kanalu A enkoderu na pravem motoru
void pravyEncoderAInt() {
  pulseCountVpravo++;  // vzestupna -> pricti puls
}

// osetreni preruseni od kanalu A enkoderu na levem motoru
void levyEncoderAInt() {
  pulseCountVlevo++;  // vzestupna -> pricti puls
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

void coffin() {
  for (int thisNote = 0; thisNote < 64; thisNote++) {

    int noteDuration = 400 / noteDurations[thisNote];
    buzzer.tone(melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    buzzer.noTone();
  }
}

//======race metody
void L90R(int LL, int L, int R, int RR) {
  levyMotorVzad(140);    //150
  pravyMotorVpred(140);  //150
  unsigned long pockej = millis();
  while (millis() - pockej < 250) {  //200
    //misto delaye
  }
  while (!(LL > left_threshold && L < sensor_threshold && R < sensor_threshold && RR > prahRight)) {
    RGBLineFollower.loop();
    LL = RGBLineFollower.getADCValueRGB1();
    L = RGBLineFollower.getADCValueRGB2();
    R = RGBLineFollower.getADCValueRGB3();
    RR = RGBLineFollower.getADCValueRGB4();
  }
  if (LL > left_threshold && L < sensor_threshold && R < sensor_threshold && RR > prahRight) {
    RGBLineFollower.loop();
    LL = RGBLineFollower.getADCValueRGB1();
    L = RGBLineFollower.getADCValueRGB2();
    R = RGBLineFollower.getADCValueRGB3();
    RR = RGBLineFollower.getADCValueRGB4();
    while (!(LL > left_threshold && L < sensor_threshold && R < sensor_threshold && RR > prahRight)) {
      RGBLineFollower.loop();
      LL = RGBLineFollower.getADCValueRGB1();
      L = RGBLineFollower.getADCValueRGB2();
      R = RGBLineFollower.getADCValueRGB3();
      RR = RGBLineFollower.getADCValueRGB4();
    }
  }

  XLLL = 1;
  timerXLLL = millis();
}

void R90R() {
  levyMotorVpred(140);  //140
  pravyMotorVzad(147);  //140
}

void R_90R(int LL, int L, int R, int RR) {
  stop();
  unsigned long pockej = millis();
  while (millis() - pockej < 100) {
    //misto delaye
  }
  levyMotorVpred(80);
  pravyMotorVpred(80);
  pockej = millis();
  while (millis() - pockej < 150 - smouha) {  //350 //550
    //misto delaye
  }
  stop();
  while (millis() - pockej < 100) {
    //misto delaye
  }

  levyMotorVpred(140);  //140
  pravyMotorVzad(147);  //140

  pockej = millis();                 //vyhodí ho z dráhy
  while (millis() - pockej < 150) {  //200
    //misto delaye
  }

  while (!(LL > left_threshold && L < sensor_threshold && R < sensor_threshold && RR > prahRight)) {  //toč dokud nenarazíš na čáru
    RGBLineFollower.loop();
    LL = RGBLineFollower.getADCValueRGB1();
    L = RGBLineFollower.getADCValueRGB2();
    R = RGBLineFollower.getADCValueRGB3();
    RR = RGBLineFollower.getADCValueRGB4();
  }
  if (LL > left_threshold && L < sensor_threshold && R < sensor_threshold && RR > prahRight) {
    RGBLineFollower.loop();
    LL = RGBLineFollower.getADCValueRGB1();
    L = RGBLineFollower.getADCValueRGB2();
    R = RGBLineFollower.getADCValueRGB3();
    RR = RGBLineFollower.getADCValueRGB4();
    while (!(LL > left_threshold && L < sensor_threshold && R < sensor_threshold && RR > prahRight)) {
      RGBLineFollower.loop();
      LL = RGBLineFollower.getADCValueRGB1();
      L = RGBLineFollower.getADCValueRGB2();
      R = RGBLineFollower.getADCValueRGB3();
      RR = RGBLineFollower.getADCValueRGB4();
    }
  }
  levyMotorVpred(80);
  pravyMotorVpred(80);
}


void zjistiCilR() {
  stop();
  unsigned long pockej = millis();
  while (millis() - pockej < 100) {
    //misto delaye
  }
  levyMotorVpred(80);
  pravyMotorVpred(80);
  pockej = millis();
  while (millis() - pockej < 250 - smouha) {  //150
    //misto delaye
  }
  stop();
  while (millis() - pockej < 100) {
    //misto delaye
  }
  RGBLineFollower.loop();
  int LL = RGBLineFollower.getADCValueRGB1();
  int L = RGBLineFollower.getADCValueRGB2();
  int R = RGBLineFollower.getADCValueRGB3();
  int RR = RGBLineFollower.getADCValueRGB4();
  if (LL < left_threshold && L < sensor_threshold && R < sensor_threshold && RR < prahRight) {
    runLED(yellow);
    findShortest();
    if (race == 1) {
      runLED(yellow);
      coffin();
      reinicializace();
      while (digitalRead(pravyNaraznik)) {
        //nepokračuj dokud není stisknut pravý nárazník
      }
      levyMotorVpred(100);
      pravyMotorVpred(100);
      runLED(aquamarine);
      return;
    }
    coffin();
    reinicializace();
    while (digitalRead(pravyNaraznik)) {
      //nepokračuj dokud není stisknut pravý nárazník
    }
    levyMotorVpred(100);
    pravyMotorVpred(100);
    runLED(aquamarine);
    return;  //vrátí se do loopu
  } else {
    RGBLineFollower.loop();
    int LL = RGBLineFollower.getADCValueRGB1();
    int L = RGBLineFollower.getADCValueRGB2();
    int R = RGBLineFollower.getADCValueRGB3();
    int RR = RGBLineFollower.getADCValueRGB4();
    if (LL < left_threshold && L < sensor_threshold && R < sensor_threshold && RR < prahRight) {
      runLED(yellow);
      findShortest();
      if (race == 1) {
        runLED(yellow);
        coffin();
        reinicializace();
        while (digitalRead(pravyNaraznik)) {
          //nepokračuj dokud není stisknut pravý nárazník
        }
        levyMotorVpred(100);
        pravyMotorVpred(100);
        runLED(aquamarine);
        return;
      }
      coffin();
      reinicializace();
      while (digitalRead(pravyNaraznik)) {
        //nepokračuj dokud není stisknut pravý nárazník
      }
      levyMotorVpred(100);
      pravyMotorVpred(100);
      runLED(aquamarine);
      return;  //vrátí se do loopu
    } else if (XLLL == 0) {
      L90R(LL, L, R, RR);  //jen pokus, že by se nevracel ale rovnou za křižovatkou zatočil - lepší než předtím
      //sem NEdávej LLL=1 ty idiote
    }
  }
}
