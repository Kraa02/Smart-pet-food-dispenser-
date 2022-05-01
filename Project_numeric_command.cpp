#define BLYNK_PRINT Serial
#include <SPI.h>
#include <WiFiNINA.h>
#include <BlynkSimpleWiFiNINA.h>
#include <TimeLib.h>
#include <WidgetRTC.h>
#include <Wire.h>                   
#include <Servo.h>
char auth[] = "k_lZZKvp6drF2neXuRzUMAAqHIvC8JC5";
char ssid[] = "AMINE";
char pass[] = "12345678";

int pompe1 = 2;
int pompe2 = 4;

int capteur_niveau_haut_eau = 12;
int capteur_niveau_bas_eau = 11;

int capteur_niveau_haut_nourriture = 9;
int capteur_niveau_bas_nourriture = 8;

int capteur_presence1 = 6;
int capteur_presence2 = 10;

int capteur_portion_croquette = 7 ; 

int detection_niveau_haut_eau;
int detection_niveau_bas_eau;

int detection_niveau_bas_nourriture;
int detection_niveau_haut_nourriture;

int detection_presence_1;
int detection_presence_2;

int detection_portion_croquette;

Servo servo;

WidgetTerminal terminal(V1);

BlynkTimer timer;                             
WidgetRTC rtc;                                      
unsigned long startMillis;                            
unsigned long currentMillis;                          
const unsigned long period = 1000;                      
  
BLYNK_CONNECTED()                                             
{ rtc.begin(); }



// or Software Serial on Uno, Nano...
#include <SoftwareSerial.h>
SoftwareSerial EspSerial(2, 3); // RX, TX

int sensorPin = A0;   // Input pin for the Flame Sensor
int sensorValue = 0;  // Variable to store the value coming from the flame sensor



void notifyOnFire()
{
  int isButtonPressed1 = digitalRead(A0);
  int isButtonPressed2 = digitalRead(A1);
  if (isButtonPressed1==0) {
    Serial.println("Le chat est en train de manger");
    Blynk.notify("Le chat est en train de manger");
  }
  
  else if (isButtonPressed2==0){
    Serial.println("Le chat est en train de boire");
    Blynk.notify("Le chat est en train de boire");
  }
}


void setup()
{

 // Debug console
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  // Clear the terminal content
  terminal.clear();

  // This will print Blynk Software version to the Terminal Widget when
  // your hardware gets connected to Blynk Server
  terminal.println(F("Salut !!"));
  terminal.println(F("Veuillez saisir soit 'eau' pour vérifier le niveau d'eau"));
  terminal.println(F("soit 'nourriture' pour vérifier le niveau de nourriture"));
  //terminal.println(F("anything else and get it printed back."));
  terminal.flush();
  
  pinMode(capteur_niveau_haut_eau, INPUT_PULLUP);
  pinMode(capteur_niveau_bas_eau, INPUT_PULLUP);
  
  pinMode(pompe1, OUTPUT);
  pinMode(pompe2, OUTPUT); 

  pinMode(capteur_niveau_haut_nourriture, INPUT);
  pinMode(capteur_niveau_bas_nourriture, INPUT);

  pinMode(capteur_presence1, INPUT);
  pinMode(capteur_presence2, INPUT);

  pinMode(capteur_portion_croquette, INPUT);
  
 //serial.begin(115200);
 // Blynk.begin(auth, ssid, pass);
  servo.attach(13); // NodeMCU D8 pin
 
 /* 0 - General */
                                           /* Initiate the Serial Monitor function */
 // pinMode(3,OUTPUT);                                            /* Declare the Output Pin D4 (GPIO 2) as an output pin for LED Light*/
  
  /* 1- Blynk Server and Wifi Connection */
  
  setSyncInterval(1);                                           /* Synchronise or read time from the Blynk Server every 1 second */
  Blynk.begin(auth,ssid,pass);                                  /* Initiate the Blynk server login for the specific project*/
  while (Blynk.connect() == false ) {}                          /* If the Blynk Server not yet connected to nodeMCU, keep waiting here */
  setSyncInterval(10*60);                                       /* After successful login, change Synchornise Time reading for every 10 minute (Do not need to always check for the time)*/
  
  /* 2- Display refresh*/
  
  startMillis = millis();                                       /* Start record initial time for display refresh */
  Blynk.begin(auth, ssid, pass);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  timer.setInterval(1000L,notifyOnFire); 
 }
void loop()
{

  detection_niveau_haut_eau = digitalRead(capteur_niveau_haut_eau);
  detection_niveau_bas_eau = digitalRead(capteur_niveau_bas_eau);

  detection_niveau_haut_nourriture = digitalRead(capteur_niveau_haut_nourriture);
  detection_niveau_bas_nourriture = digitalRead(capteur_niveau_bas_nourriture);

  detection_presence_1 = digitalRead(capteur_presence1);
  detection_presence_2 = digitalRead(capteur_presence2);

  detection_portion_croquette = digitalRead(capteur_portion_croquette);
  
  Blynk.run();
  timer.run();                                                                    /* allow the Blynk timer to keep counting */
  sensorValue = analogRead(sensorPin);
 // if (sensorValue < 700) {
  //  Blynk.notify("Le chat est en train de manger");
//  }


    if (detection_niveau_haut_eau == 1) {
    digitalWrite(pompe1, LOW);
    }
    if (detection_niveau_bas_eau == 1) {
    digitalWrite(pompe2, LOW);
    }

    if (detection_portion_croquette == 0) {
    servo.write(
      0);
    }
    

  /* 2- Display refresh*/

  currentMillis = millis();                                                       /* Keep counting time for display refresh */
  if(currentMillis - startMillis > period)                                        /* For every 1 second, run the set of code*/
  {
    String currentTime = String(hour()) + ":" + minute() + ":" + second();        /* Define "currentTime" by combining hour, minute and second */
    String currentDate = String(day()) + " " + month() + " " + year();            /* Define "currentDate" by combining day, month, and year */
    Serial.print("Current time: ");                                               /* Display values on Serial Monitor */
    Serial.print(currentTime);
    Serial.print(" ");
    Serial.print(currentDate);
    Serial.println();

    Blynk.virtualWrite(V9, currentTime);                                          /* Send Time parameters to Virtual Pin V1 on Blynk App */
    Blynk.virtualWrite(V10, currentDate);                                          /* Send Date parameters to Virtual Pin V2 on Blynk App */
    
      int getSecond = second();                                                     /* Assign "getHour" as the hour now */
   // if (getSecond > 30)
   // { digitalWrite(3,HIGH);}                                                      /* Turn OFF the LED if seconds count is more than 30 */
   // if (getSecond < 30)
   // { digitalWrite(3,LOW); }                                                      /* Turn ON the LED if the seconds count is less than 30 */                                                 
   // startMillis = millis();                                                       /* Reset time for the next counting cycle */
  }
}

BLYNK_WRITE(V2)
{
  servo.write(0);
}
BLYNK_WRITE(V3)
{
  servo.write(180);
}
BLYNK_WRITE(V1)
  {

 if (String("eau") == param.asStr()) {

    if (detection_niveau_haut_eau == 1) {
    terminal.println("Le niveau d'eau actuel est : 'HAUT'") ;
    }
    else if (detection_niveau_bas_eau == 0) {
    terminal.println("Le niveau d'eau actuel est : 'BAS'") ;
    }
    else 
    terminal.println("Le niveau d'eau actuel est : 'TROP BAS' Veuillez remplir le réservoir d’eau s’il vous plait ") ;
 } 
  else if (String("nourriture") == param.asStr()) {

    if (detection_niveau_haut_nourriture == 0) {
    terminal.println("Le niveau de nourriture actuel est : 'HAUT'") ;
    }
    else if (detection_niveau_bas_nourriture == 0) {
    terminal.println("Le niveau de nourriture actuel est : 'BAS'") ;
    }
    else 
    terminal.println("Le niveau de nourriture actuel est : 'TROP BAS'Veuillez remplir le réservoir de nourriture s’il vous plait ") ;
 } 
  
    else {

    // Send it back
    terminal.println("veuillez choisir l'une de ces deux options :");
    terminal.println("1/ 'eau'");
    terminal.println("2/ 'nourriture'");

  }

  // Ensure everything is sent
  terminal.flush();
  }
