// OKKOOD!!!!!!!!!!!!!

//zkusit zvetsit pole pro ukladani cesty!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//  PRO ELISKU

// NOTY
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
 
#define melodyPin 45

// MELODIE
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

// DELKY NOT
int tempo[] = {
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









#include <Arduino.h>
#include "PinChangeInterrupt.h"
#include "MeAuriga.h"
#include "MeRGBLineFollower.h"

int d1;     //DIODY
int d2;
int d3;
int d4;

//VZDALENOST

// promenne pro enkodery
// enkoder pro pravy motor
const byte pravyEnkoderA = 19; // INT2
const byte pravyEnkoderB = 42; // no interrupts :(

// enkoder pro levy motor
const byte levyEnkoderA = 18; // INT3
const byte levyEnkoderB = 43; // no interrupts :(

const int eckoderPulseNumber = 9; // pocet pulsu encoderu na celou otacku
const float motorGearRatio = 39.267; // polomer kola.. spis obvod celeho pasu
const float prevodovyPomer = 758.1; // IDK musim upravit
const float prevodniKonstanta = (TWO_PI*motorGearRatio)/(prevodovyPomer*eckoderPulseNumber);
volatile float ujetoVpravo = 0;
volatile float ujetoVlevo = 0;

volatile long pulseCountVlevo = 0;
volatile long pulseCountVpravo = 0;

volatile byte levyStateA,oldLevyStateA;
volatile byte pravyStateA,oldPravyStateA;








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
int vzdalenost;

// Snimac cary
// pouziti: lineOffset = getPositionOffset();
MeRGBLineFollower RGBLineFollower(PORT_9);
int lineOffset;

// Servo
/*const byte servoPin = 68;
const byte servoMin = 13;
const byte servoMax = 137;
Servo servo;*/

// narazniky
const byte pravyNaraznik = 67;
const byte levyNaraznik = 62;
volatile boolean narazVpravo = false;
volatile boolean narazVlevo = false;

/*// promenne pro enkodery
// enkoder pro pravy motor
const byte pravyEnkoderA = 19; // INT2
const byte pravyEnkoderB = 42; // no interrupts :(

// enkoder pro levy motor
const byte levyEnkoderA = 25; // INT3
const byte levyEnkoderB = 43; // no interrupts :(

const int eckoderPulseNumber = 9;
const float motorGearRatio = 39.267;

volatile long pulseCountVlevo = 0;
volatile long pulseCountVpravo = 0;*/

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
 //VZDALENOST
// inicializace obsluhy preruseni od kanalů A enkoderů
  attachInterrupt(digitalPinToInterrupt(pravyEnkoderA),&pravyEncoderAInt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(levyEnkoderA),&levyEncoderAInt, CHANGE);
  oldPravyStateA = digitalRead(pravyEnkoderA);
  oldLevyStateA = digitalRead(levyEnkoderA);


  
  // nastav piny narazniku
  pinMode(pravyNaraznik,INPUT_PULLUP);
  pinMode(levyNaraznik,INPUT_PULLUP);
  // inicializace obsluhy preruseni od narazniku
  //attachPCINT(digitalPinToPCINT(pravyNaraznik), &narazVpravoInt, FALLING);
  //attachPCINT(digitalPinToPCINT(levyNaraznik), &narazVlevoInt, FALLING);

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
  attachInterrupt(digitalPinToInterrupt(pravyEnkoderA),&pravyEncoderAInt, RISING);
  attachInterrupt(digitalPinToInterrupt(levyEnkoderA),&levyEncoderAInt, RISING);

  // pripoj a omez servo
  //servo.attach(servoPin);//,servoMin,servoMax);
 // servo.write(90);

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
  RGBLineFollower.setKp(0.01);

  // inicializace sériového kanálu
  Serial.begin(9600);


//  servo.write(90);
  while (digitalRead(levyNaraznik)) {
    // nepokracuj dokud neni stiknut levy naraznik
  }
  //CEKANI NA STARTOVNI CARU
  levyMotorVpred(80);
  pravyMotorVpred(80);
  while(true){
      RGBLineFollower.loop();

      d1 = RGBLineFollower.getADCValueRGB1();
      d2 = RGBLineFollower.getADCValueRGB2();
      d3 = RGBLineFollower.getADCValueRGB3();
      d4 = RGBLineFollower.getADCValueRGB4();
      if (d1<14 && d4<14 && d3<17 && d2<17){
        buzzer.tone(500,650);
        pulseCountVpravo=0;
        pulseCountVlevo=0;
        break;
      }
  }
}











//  LOOP START !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!



int x = 6;        //POLOHA V SOURADNICICH
int cesta = 0; 
int smer = 1;

int pauza=0;
int pauza2=0;
unsigned long p = 0;
unsigned long p2 = 0;
unsigned long tedl = 0;
unsigned long tedp = 0;


int doprava=0;      //PREPINANI U DOROVNAVANI PRI ZATACENI
int doleva=0;


//smery
unsigned long l = 0;
int levo=0;
unsigned long o = 0;
unsigned long tedc = 0;
int otocka=0;

int vypocet=0;
int otoc=0;
int cil=85;


int pole[500];            //VYPOCET NEJKRATSI CESTY
int nepreskocene = 0;
int pred = 0;
int po = 0;
int vysledek[100];


float levyPas=0;      //INFO Z ENKODERU
float pravyPas=0;


void loop() {
  // vyhodnoceni senzoru
  RGBLineFollower.loop();

  d1 = RGBLineFollower.getADCValueRGB1();
  d2 = RGBLineFollower.getADCValueRGB2();
  d3 = RGBLineFollower.getADCValueRGB3();
  d4 = RGBLineFollower.getADCValueRGB4();

  levyPas=pulseCountVlevo*prevodniKonstanta;
  pravyPas=pulseCountVpravo*prevodniKonstanta;

  vzdalenost = sonar.distanceCm();


                   
//PRVNI PRUJEZD PRO MAPOVANI
if(cesta==0){

             
         //JIZDA


         //CARA ROVNE - NA 2 A 3 DIODE
          if (d1>14 && d4>14 && d3<17 && d2<17 && otoc==0){                //cara rovne (na 2 a 3)
 
              //JED ROVNE
              levyMotorVpred(130);
              pravyMotorVpred(130);

              //POKUD JSI ZATACEL DOPRAVA NEBO SE OTACEL, PRAVE JSI SE CHYTIL NA CARU
              if(doprava==1){

                  //ZATACEL JSI MALO -> DOPRAVA
                  if(levyPas<32 && levyPas>10){
                     doprava=0;

                     mapovani();
                     smer--;
                     if(smer<1){
                         smer=4;
                     }
                     rozsvit(smer);
                    pulseCountVpravo=0;
                    pulseCountVlevo=0;
                    levyPas=0;
                    pravyPas=0;
                   
                 }

                 //ZATACEL JSI HODNE -> OTACEL JSI SE
                 else if(levyPas>32){

                   pulseCountVpravo=0;
                   pulseCountVlevo=0;
                   levyPas = 0;
                   pravyPas = 0;
                   
                   doprava=0;

                          mapovani();
                          smer++;
                          if(smer>4){
                            smer=1;
                          }
                          smer++;
                          if(smer>4){
                            smer=1;
                          }
                          rozsvit(smer);
                          pulseCountVpravo=0;
                          pulseCountVlevo=0;
                          levyPas=0;
                          pravyPas=0;      
                 }

                 //BYL TO JEN OMYL
                 else{
                  doprava=0;  
                 }
             }

             //ZATACEL JSI DOLEVA A CHYTIL JSI ZAS CARU
             else if(doleva==1 && pravyPas>15 && levo==1){
               doleva=0;
               mapovani();
               smer++;
               if(smer>4){
                   smer=1;
               }
               rozsvit(smer);
               pulseCountVpravo=0;
               pulseCountVlevo=0;
               levyPas=0;
               pravyPas=0;               
             }
          }

          //JE TU KRIZOVATKA, JED VLEVO - CARA JE NA 1, 2 A 3 DIODE
          if(d1<14 && d4>14 && d3<17 && d2<17 && otoc==0 && doprava==0){            //zatoc vlevo - cara na 1, 2 a 3  MOZNA BUDE BLBNOUT POCITANI VZDALENOSTI KVULI OTOCCE
             if(tedl==0){
              tedl=millis();
             }
             if(millis()-tedl>20 || doleva==1){
               tedl=0;
               Levo90();

               if(doleva==0){
                 pauza2=1;
                 p2=millis();
                 doleva=1;
                 levo=1;
                 l=millis();
                  pulseCountVpravo=0;
                  pulseCountVlevo=0;
                  levyPas=0;
                  pravyPas=0;
               }
             }
          }

          //KRIZOVATKA, JED VLEVO - CARA JE NA 1, 2, 3 I 4 DIODE
          else if(d1<14 && d4<14 && d3<17 && d2<17 && otoc==0 && doprava==0){            //zatoc vlevo - cara na 1, 2 a 3 a 4
      
             if(tedl==0){
              tedl=millis();
             }
             if(millis()-tedl>20 || doleva==1){
               tedl=0;
               Levo90();
       
               if(doleva==0){
                 doleva=1;
                 pauza2=1;
                 p2=millis();
                 levo=1;
                 l=millis();
                 pulseCountVpravo=0;
                 pulseCountVlevo=0;
                 levyPas=0;
                 pravyPas=0;
               }
               
             }
          }

         //DOROVNAVEJ, KDYZ JE CARA JEN NA 1 A NEPROBIHA TOCENI VPRAVO
         if(d1<14 && d4>14 && d3>17 && d2>17 && otoc==0 && doprava==0){            //zatoc vlevo - cara na 1 MOHLO BY BYT PROBLEMATICKE PRI ZATACENI VPRAVO, KDYZ BY NEFUNGOVALO, DOMYSLET!!!!!!!!!!!!!
             pravyMotorVpred(120);
             levyMotorVzad(90);
             
          }
          //DOROVNAVEJ, KDYZ JE CARA JEN NA 4 A PROBIHA TOCENI VPRAVO
          if(d1>14 && d4<14 && d3>17 && d2>17 && otoc==0 && doprava==1){            //zatoc vpravo - cara na 4 MOHLO BY BYT PROBLEMATICKE PRI ZATACENI VPRAVO, KDYZ BY NEFUNGOVALO, DOMYSLET!!!!!!!!!!!!!
              levyMotorVpred(120);
              pravyMotorVzad(70);
          }
                   

       else if(d1>14 && d4>14 && d3>17 && d2>17 && doleva==1){
          Levo90();
       }
       
       //KDYZ VYJEDES Z CARY, TOC SE DOPRAVA, DOKUD ZASE CARU NENAJDES - ZATACKA VPRAVO NEBO OTOCENI, poznava se nahore, kdyz se zase chyti na caru
       else if(d1>14 && d4>14 && d3>17 && d2>17){ 
          if(tedp==0){
              tedp=millis();
            }
          if(millis()-tedp>50){
              tedp=0;
              Pravo90();
              if(doprava==0){
                doprava=1;
               pulseCountVpravo=0;
               pulseCountVlevo=0;
               levyPas=0;
               pravyPas=0;
              }
          }

        }

        
        //DOROVNAVANI PRI JIZDE PO CARE - JEN CUKA, ABY JEL VICEMENE ROVNE
        
          if(d1>14 && d4>14 && d3<17 && d2>17 && otoc==0 && doleva==0){            //dorovnavani 
             levyMotorVpred(180);
             pravyMotorVzad(90);
          }
          if(d1>14 && d4>14 && d3>17 && d2<17 && otoc==0 && doprava==0){            //dorovnavani
             pravyMotorVpred(180);
             levyMotorVzad(90);
          }
        
        
        
        
      //UJEL VICE CTVERCU ROVNE  
       if( pravyPas > 32 && levyPas > 32 && doleva==0 && doprava==0){ 
          pulseCountVpravo=0;
          pulseCountVlevo=0;
          levyPas=0;
          pravyPas=0;
          mapovani();
          ledRing.setColor(3, magenta);
          ledRing.show();
          delay(200);
          rozsvit(smer);
        }

        //NULOVANI CASOVACU
         if(millis()-p > 2000){       //aby neustale nemenil smer pri otaceni
          pauza = 0;
         }
         if(millis()-p2 > 1000){       //aby neustale nemenil smer pri otaceni
          pauza2 = 0;
         }
         if(millis()-l > 1500){       //aby neustale nemenil smer pri mapovani
          levo= 0;
          doleva=0;
         }
         if(millis()-o > 5000){       //aby neustale nemenil smer pri mapovani
          otocka= 0;
         }


      //JEN EFEKT - ROZSVECENI LEDEK PODLE SMERU
         if(doprava==1){
            ledRing.setColor(9, red);
            ledRing.show();
         }
         else if(doprava==0){
            ledRing.setColor(9, white);
            ledRing.show();
         }
         if(doleva==0){
            ledRing.setColor(9, white);
            ledRing.show();
         }
         else if(doleva==1){
            ledRing.setColor(9, blue);
            ledRing.show();
         }

        
        
        //OTOCENI PRED PREKAZKOU
        
          if(vzdalenost<3){                                    //ultrazvuk - prekazka, otoc se - VYZKOUSET!!!
              pravyMotorVpred(0);
              levyMotorVpred(0);
              Pravo90();
              doprava=1;
              delay(300);
              if(pauza==0){
                pauza = 1;
                p=millis();
              }
          }


      //CIL
      
       if(d1<14 && d4<14 && d3<17 && d2<17 && x==cil && smer==1){
         if(tedc==0){
              tedp=millis();
            }
          if(millis()-tedc>1000){
              tedc=0;
               cesta=1;
               mapovani();
               delay(500);
               levyMotorVpred(0);
               pravyMotorVpred(0);
               ledRing.setColor(1, black);
               ledRing.show();
               buzzer.tone(500,100);
               ledRing.setColor(2, yellow);
               ledRing.show();
               buzzer.tone(500,100);
               ledRing.setColor(3, amber);
               ledRing.show();
               buzzer.tone(600,100);
               ledRing.setColor(4, orange);
               ledRing.show();
               buzzer.tone(600,100);
               ledRing.setColor(5, vermillion);
               ledRing.show();
               buzzer.tone(500,100);
               ledRing.setColor(6, red);
               ledRing.show();
               buzzer.tone(500,100);
               ledRing.setColor(7, magenta);
               ledRing.show();
               buzzer.tone(600,100);
               ledRing.setColor(8, purple);
               ledRing.show();
               buzzer.tone(600,100);
               ledRing.setColor(9, indigo);
               ledRing.show();
               buzzer.tone(500,100);
               ledRing.setColor(10, blue);
               ledRing.show();
               buzzer.tone(500,100);
               ledRing.setColor(11, aquamarine);
               ledRing.show();
               buzzer.tone(600,100);
               ledRing.setColor(12, green);
               ledRing.show();
               buzzer.tone(600,1000);
          }
      }
      //DOCASNY POKUSNY CIL - ZRUSIT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    /* if(x==15){
         cesta=1;
         levyMotorVpred(0);
         pravyMotorVpred(0);
         int size = sizeof(melody) / sizeof(int);
             for (int thisNote = 0; thisNote < size; thisNote++) {
                int noteDuration = 1000 / tempo[thisNote];
                buzz(melodyPin, melody[thisNote], noteDuration);
                
                int pauseBetweenNotes = noteDuration * 1.30;
                delay(pauseBetweenNotes);
                
                buzz(melodyPin, 0, noteDuration);
            }
      }*/

}
else{
  if(vypocet==0){
    pole[0]=6;
    for(int i=0;i<100;i++) {
      Serial.println(pole[i]);
    }
    spoctiNejkratsi();
    int smery[nepreskocene-1];
    prevedNaSmery(smery);
    vypocet=1;
    buzzer.tone(500,300);
    delay(300);
    buzzer.tone(500,300);
    delay(300);
    buzzer.tone(500,300);
   while(digitalRead(pravyNaraznik)){
    
   }
   pulseCountVpravo=0;
   pulseCountVlevo=0;
   levyPas=0;
   pravyPas=0;
   ledRing.setColor(0, white);
   ledRing.show();
   pravyMotorVpred(100);
   levyMotorVpred(100);
   while(true){
      RGBLineFollower.loop();

      d1 = RGBLineFollower.getADCValueRGB1();
      d2 = RGBLineFollower.getADCValueRGB2();
      d3 = RGBLineFollower.getADCValueRGB3();
      d4 = RGBLineFollower.getADCValueRGB4();
      if (d1<14 && d4<14 && d3<17 && d2<17){
        buzzer.tone(500,400);
        pulseCountVpravo=0;
        pulseCountVlevo=0;
        break;
      }
  }
    jed(smery);
}

}
}


//LOOP KONEC!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!







        //ROZSVECENI DIODY PODLE SMERU
     void rozsvit(int smer){
         switch(smer){
          case 1:
            ledRing.setColor(3, yellow);
            ledRing.show();
            break;
          case 2:
              ledRing.setColor(3, red);
              ledRing.show();
              break;
          case 3:
              ledRing.setColor(3, green);
              ledRing.show();
              break;
          case 4:
              ledRing.setColor(3, blue);
              ledRing.show();
              break;
         }
     }

// osetreni preruseni od praveho narazniku
void narazVpravoInt() {
  narazVpravo = true;
}

// osetreni preruseni od leveho narazniku
void narazVlevoInt() {
  narazVlevo = true;
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


//IMPULS PRO ZACATEK TOCENI VPRAVO
void Pravo90() {
    levyMotorVpred(140);
    pravyMotorVzad(140);
}
//IMPULS PRO ZACATEK TOCENI VLEVO
unsigned long cekej=0;
void Levo90() {
    pravyMotorVpred(160);
    levyMotorVzad(110);
    if(doleva==0){
      delay(200);
    }

           
}



//IMPULS PRO ZACATEK TOCENI VPRAVO rychly
void Pravo290() {
    levyMotorVpred(140);
    pravyMotorVzad(140);
    if(doprava==0){
      buzzer.tone(500,400);
    }
}
//IMPULS PRO ZACATEK TOCENI VLEVO rychly
void Levo290() {
    pravyMotorVpred(140);
    levyMotorVzad(140);
    if(doleva==0){
      buzzer.tone(700,200);
    }

           
}


//VYTVARENI MAPY - UKLADANI DANYCH INDEXU, MEZI KTERYMI VEDE CESTA, DO POLI
int uzel =1;
void mapovani(){
 if(smer==1){
    pole[uzel]=x+9;
    uzel++;
    x=x+9;
  }
  else if(smer==2){
    pole[uzel]=x-1;
    uzel++;
    x=x-1;
  }
  else if(smer==3){
    pole[uzel]=x-9;
    uzel++;
    x=x-9;
  }
  else if(smer==4){
    pole[uzel]=x+1;
    uzel++;
    x=x+1;
  }
}



int kamMamJet=0;
int smerakt=0;
int smerbud=0;
int pokusnysmer=0;
int zatocvpravo=0;
int zatocvlevo=0;
int RYCHLOST=0;
int SPODNI = 110;
int HORNI = 200;

void jed(int smery[]){
  while(true){
    
    // vyhodnoceni sledovani cary
    RGBLineFollower.loop();
  
    d1 = RGBLineFollower.getADCValueRGB1();
    d2 = RGBLineFollower.getADCValueRGB2();
    d3 = RGBLineFollower.getADCValueRGB3();
    d4 = RGBLineFollower.getADCValueRGB4();

  //CTENI Z ENKODERU
  levyPas=pulseCountVlevo*prevodniKonstanta;
  pravyPas=pulseCountVpravo*prevodniKonstanta;

//V CILI ZASTAV
if(kamMamJet==nepreskocene-1){
  while(true){
    levyMotorVpred(0);
    pravyMotorVpred(0);
         int size = sizeof(melody) / sizeof(int);
             for (int thisNote = 0; thisNote < size; thisNote++) {
                int noteDuration = 1000 / tempo[thisNote];
                buzz(melodyPin, melody[thisNote], noteDuration);
                
                int pauseBetweenNotes = noteDuration * 1.30;
                delay(pauseBetweenNotes);
                
                buzz(melodyPin, 0, noteDuration);

                if(digitalRead(pravyNaraznik)==false){
                      pulseCountVpravo=0;
                      pulseCountVlevo=0;
                      levyPas=0;
                      pravyPas=0;
                      break;
                }
            }
            
  }
}


    //ZJISTENI NASLEDUJICIHO SMERU
    
    smerakt=smery[kamMamJet];
    smerbud=smery[kamMamJet+1];
    pokusnysmer=smerakt+1;
    if(pokusnysmer>4){
      pokusnysmer=1;
    }
    if(pokusnysmer==smerbud){
      zatocvlevo=1;
    }
    else{
      pokusnysmer=smerakt-1;
      if(pokusnysmer<1){
        pokusnysmer=4;
      }
      if(pokusnysmer==smerbud){
        zatocvpravo=1;
      }
    }

    //RYCHLOST PODLE ROVINEK
    if(smerakt==smerbud){
      RYCHLOST = HORNI;
    }
    else{
      RYCHLOST = SPODNI;
    }

// JED ROVNE
          if (d1>14 && d4>14 && d3<17 && d2<17 && otoc==0){                //cara rovne (na 2 a 3)
                    //JED ROVNE
                    levyMotorVpred(RYCHLOST);
                    pravyMotorVpred(RYCHLOST);

                    //URCUJ SI, KDE JSI PODLE ZATACEK
                    if(doleva==1 && pravyPas>20){       //ZATACKA VLEVO SKONCILA
                      doleva = 0;
                      kamMamJet++;
                      zatocvlevo=0; 
                      pulseCountVpravo=0;
                      pulseCountVlevo=0;
                      levyPas=0;
                      pravyPas=0;
                      ledRing.setColor(3, white);
                ledRing.show();
                    }
                    else if(levyPas<32 && levyPas>5 && doprava==1){      //ZATACKA VPRAVO SKONCILA
                      doprava=0;
                      kamMamJet++;
                      zatocvpravo=0;
                      pulseCountVpravo=0;
                      pulseCountVlevo=0;
                      levyPas=0;
                      pravyPas=0;
                      ledRing.setColor(3, white);
                ledRing.show();
                    }
          }
          
          //PRED CILEM
          if(kamMamJet==nepreskocene-2 && d1<14 && d4<14 && d3<17 && d2<17){
            RYCHLOST==HORNI;
            kamMamJet++;
          }

          //KRIZOVATKA, JED VLEVO - CARA JE NA 1, 2, 3 I 4 DIODE
          else if(d1<14 && d3<17 && d2<17 && zatocvlevo==1){            //zatoc vlevo - cara na 1, 2 a 3 a 4
            if(tedl==0){
              tedl=millis();
            }
            if(millis()-tedl>15){
               tedp=0;
               Levo290();
               doleva=1;
               pulseCountVpravo=0;
                pulseCountVlevo=0;
                levyPas=0;
                pravyPas=0;
                ledRing.setColor(3, blue);
                ledRing.show();
            }           
          }


        //ZATOC DOPRAVA
       else if(d4<15 && zatocvpravo==1){ 
            if(tedp==0){
              tedp=millis();
            }
            if(millis()-tedp>10){
               tedp=0;
               doprava=1;
               Pravo290();
               pulseCountVpravo=0;
                pulseCountVlevo=0;
                levyPas=0;
                pravyPas=0;
                ledRing.setColor(3, red);
                ledRing.show();
            }
        }

        
        //DOROVNAVANI PRI JIZDE PO CARE - JEN CUKA, ABY JEL VICEMENE ROVNE
        
          if(d1>14 && d4>14 && d3<17 && d2>17 && otoc==0 && doleva==0 && doprava==0){            //dorovnavani 
             levyMotorVpred(250);
             pravyMotorVzad(0);
          }
          if(d1>14 && d4>14 && d3>17 && d2<17 && otoc==0 && doprava==0 && doprava==0){            //dorovnavani
             pravyMotorVpred(250);
             levyMotorVzad(0);
          }
        
        
        
        
      //UJEL VICE CTVERCU ROVNE  
        else if( pravyPas > 28 && levyPas > 28 && doleva==0 && doprava==0 && RYCHLOST==HORNI){ 
          pulseCountVpravo=0;
          pulseCountVlevo=0;
          levyPas=0;
          pravyPas=0;
          mapovani();
          ledRing.setColor(3, magenta);
          ledRing.show();
          pauza=1;
          p=millis();
          kamMamJet++;
          
        }

        //NULOVANI CASOVACU
         if(millis()-p > 200){       //aby neustale nemenil smer pri otaceni
          pauza = 0;
          ledRing.setColor(3, white);
          ledRing.show();
         }
         if(millis()-p2 > 1000){       //aby neustale nemenil smer pri otaceni
          pauza2 = 0;
         }
         if(millis()-l > 2000){       //aby neustale nemenil smer pri mapovani
          levo= 0;
         }
         if(millis()-o > 5000){       //aby neustale nemenil smer pri mapovani
          otocka= 0;
         }
         if(millis()-tedp > 1500){       //aby neustale nemenil smer pri mapovani
          otocka= 0;
         }
         if(millis()-tedl > 1500){       //aby neustale nemenil smer pri mapovani
          otocka= 0;
         }

         //OPAKOVANY RYCHLY PRUJEZD PO ZMAPOVANI, KDYZ SE NECO POKAZI
         if(digitalRead(levyNaraznik)==false){
          pravyMotorVpred(0);
          levyMotorVpred(0);
          pulseCountVpravo=0;
          pulseCountVlevo=0;
          levyPas=0;
          pravyPas=0;
          d1=0;d2=0;d3=0;d4=0;
          while(digitalRead(pravyNaraznik)){
            
          }
          kamMamJet=0;
         }
  }
}













//VZDALENOST
void pravaVzdalenost(){
  ujetoVpravo = pulseCountVpravo*prevodniKonstanta;
}
void levaVzdalenost(){
  ujetoVlevo = pulseCountVlevo*prevodniKonstanta;
}
// osetreni preruseni od kanalu A enkoderu na pravem motoru
void pravyEncoderAInt() {

    pulseCountVpravo++; // vzestupna -> pricti puls

}

// osetreni preruseni od kanalu A enkoderu na levem motoru
void levyEncoderAInt() {

    pulseCountVlevo++; // vzestupna -> pricti puls

}




//HLEDANI NEJKRATSI CESTY

void spoctiNejkratsi(){
    int aktualni = 0;
    int index = 0;

    //ZKUSIT BEZ, NEVIM, JAK ARDUINO DEKLARUJE POLE
    for(int i = 0; i<100; i++) {
      vysledek[i]=0;
    }
    
    for(int i = 0; i<500; i++) {
      if(pole[i]==0) {              //NEBER V UVAHU NULOVE PRVKY
        continue;
      }
      aktualni = pole[i];
      
      for(int j = 0; j<500; j++) {  //NAJDI VSECHNY SHODNE
        if(pole[j]==0) {
          continue;
        }
        if(pole[j]==aktualni) {     //MEZI SHODNYMI VYNULUJ
          index = j;
          while(index>i) {
            pole[index]=0;
            index--;
          }
        }
      }
      vysledek[nepreskocene]=pole[i]; //UKLADEJ BEZ SLEPYCH CEST
      nepreskocene++;
      
    }
}


//PREVEDENI NEJKRATSI CESTY NA POSLOUPNOST SMERU

void prevedNaSmery(int smery[]){ 
    for(int i=nepreskocene;i>1;i--) {
      pred = vysledek[i-1];
      po = vysledek[i-2];
      
      if(pred-po==9) {
        smery[i-2]=1;
      }
      else if(pred-po==-1) {
        smery[i-2]=2;
      }
      else if(pred-po==-9) {
        smery[i-2]=3;
      }
      else if(pred-po==1) {
        smery[i-2]=4;
      }
    }
    for(int i=0;i<nepreskocene-1;i++) {
      Serial.println(smery[i]);
    }
}






//PRO ELISKU
int ledka = 1;
int ledka2 = 0;
void buzz(int targetPin, long frequency, long length) {
  
  digitalWrite(6, HIGH);
  
  long delayValue = 1000000 / frequency / 2;
  long numCycles = frequency * length / 1000;

  if(ledka2==0){
    ledka2=1;
  }
  else{
    ledka2=0;
    ledka++;
    if(ledka>12){
      ledka=1;
      ledRing.setColor(0,white);
    }
  }
  long barva = random(8);

  int barvicka1[]={255,255,255,000,255,000,255,255,127};
  int barvicka2[]={255,255,000,255,000,000,194,165,255};
  int barvicka3[]={255,000,000,000,255,255,000,000,212};
  
  for (long i = 0; i < numCycles; i++) {
    digitalWrite(targetPin, HIGH);
    ledRing.setColor(ledka, barvicka1[barva], barvicka2[barva], barvicka3[barva]);
    ledRing.show();
    delayMicroseconds(delayValue);
    digitalWrite(targetPin, LOW);
    delayMicroseconds(delayValue);
  }
  
  digitalWrite(6, LOW);
}
