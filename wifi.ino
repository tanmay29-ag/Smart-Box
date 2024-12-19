#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

////////////////////////////////////////////////////
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

WiFiUDP ntpUDP;  
#define offset 19800
NTPClient timeClient(ntpUDP, "pool.ntp.org");
/////////////////////////////////////////////////////

#define BLYNK_TEMPLATE_ID "TMPLpO-fr9E3"
#define BLYNK_TEMPLATE_NAME "Smart Home"
#define BLYNK_AUTH_TOKEN "AKlcmGFH9Yggs-NxzyBAgRA9wPHztYxL"

char auth[] = "AKlcmGFH9Yggs-NxzyBAgRA9wPHztYxL";
char ssid[] = "V2022";//Enter your WIFI name
char pass[] = "tanmayag";//Enter your WIFI password

//////////////////////////////////////////////////////
bool fetch_blynk_state = true;
#define RelayPin1 5  //D1
#define RelayPin2 4  //D2
#define RelayPin3 0  //D3
#define RelayPin4 2  //D4
#define V_1 V1 //D1
#define V_2 V2 //D2
#define V_3 V3 //D3
#define V_4 V4 //D4
#define V_0 V0
#define V_5 V5
#define V_6 V6
bool toggleState_1 = LOW; //for relay 1
bool toggleState_2 = LOW; //for relay 2
bool toggleState_3 = LOW; //for relay 3
bool toggleState_4 = LOW; //for relay 4
int s1 =0;int s2 =0;int s3 =0;int s4 =0;int s6 =0;
//////////////////////////////////////////////////////
//Get the button value
// #define RelayPin1 5  //D1
// #define RelayPin2 4  //D2
// #define RelayPin3 0  //D3
// #define RelayPin4 2  //D4

BlynkTimer timer;
int wifiFlag = 1;
void checkBlynkStatus() 
{ // called every 2 seconds by SimpleTimer
  bool isconnected = Blynk.connected();
  if (isconnected == false) 
  {
    wifiFlag = 1;
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("WiFi");    
    lcd.setCursor(1, 1);
    lcd.print("Not Connected");        
    Serial.println("Blynk Not Connected");    
  }
  if (isconnected == true) 
  {
    wifiFlag = 0;
////////////////////////////////////////////////
    if (!fetch_blynk_state){
    Blynk.virtualWrite(V_1, toggleState_1);
    Blynk.virtualWrite(V_2, toggleState_2);
    Blynk.virtualWrite(V_3, toggleState_3);
    Blynk.virtualWrite(V_4, toggleState_4);
    }
////////////////////////////////////////////////
    Serial.println("Blynk Connected");
    //tym();    
  }
}

/////////////////////////////////////////////
BLYNK_CONNECTED() {
  // Request the latest state from the server
  if (fetch_blynk_state){
    Blynk.syncVirtual(V_1);
    Blynk.syncVirtual(V_2);
    Blynk.syncVirtual(V_3);
    Blynk.syncVirtual(V_4);
  }    
}
/////////////////////////////////////////////

/////////////////////////////////////////////
void tym()
{ 
  timeClient.update();
  time_t epochTime = timeClient.getEpochTime();
  String formattedTime = timeClient.getFormattedTime();
  struct tm *ptm = gmtime ((time_t *)&epochTime);

  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon+1;
  int currentYear = ptm->tm_year+1900;
  String currentDate = String(monthDay)+"/"+String(currentMonth)+"/"+String(currentYear); 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("DATE:"+currentDate);
  Serial.println("DATE:"+currentDate);
  lcd.setCursor(0, 1);
  lcd.print("TIME:"+formattedTime);
  Serial.println("TIME:"+formattedTime);
  lcd.setCursor(15, 1);
  delay(3000);  
}
/////////////////////////////////////////////

////////////////////////////////////////////
BLYNK_WRITE(V_1) {
  s1 = param.asInt();
  toggleState_1 = param.asInt();
  digitalWrite(RelayPin1, !toggleState_1);
}

BLYNK_WRITE(V_2) {
  s2 = param.asInt();
  toggleState_2 = param.asInt();
  digitalWrite(RelayPin2, !toggleState_2);
}

BLYNK_WRITE(V_3) {
  s3 = param.asInt();
  toggleState_3 = param.asInt();
  digitalWrite(RelayPin3, !toggleState_3);
}

BLYNK_WRITE(V_4) {
  s4 = param.asInt();
  toggleState_4 = param.asInt();
  digitalWrite(RelayPin4, !toggleState_4);
}

BLYNK_WRITE(V_6) {
  s6 = param.asInt();
}
////////////////////////////////////////////
  int x = 0;
  bool text = false;
  String textIn = "";
  int lineno =1;
  int cur =0;  
  BLYNK_WRITE(V_5)
  {
    textIn = param.asStr();
  }
/////////////////////////////////////////////

void all_SwitchOff(){
  toggleState_1 = 0; digitalWrite(RelayPin1, HIGH); 
  Blynk.virtualWrite(V_1, toggleState_1); delay(100);s1=0;

  toggleState_2 = 0; digitalWrite(RelayPin2, HIGH); 
  Blynk.virtualWrite(V_2, toggleState_2); delay(100);s2=0;

  toggleState_3 = 0; digitalWrite(RelayPin3, HIGH); 
  Blynk.virtualWrite(V_3, toggleState_3); delay(100);s3=0;

  toggleState_4 = 0; digitalWrite(RelayPin4, HIGH); 
  Blynk.virtualWrite(V_4, toggleState_4); delay(100);s4=0;
}
////////////////////////////////////////////

BLYNK_WRITE(V0) {
   all_SwitchOff();
}

// void all_SwitchOff(){
//  digitalWrite(RelayPin1, HIGH); 
//  digitalWrite(RelayPin2, HIGH); 
//  digitalWrite(RelayPin3, HIGH); 
//  digitalWrite(RelayPin4, HIGH);  
// }

void disply()
{  
  if(wifiFlag !=1 ){
    if(textIn && s6 == 1) // working for text Input Widget
    {
      text = true;
      x = 0;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(textIn);
      //Serial.print(textIn);
      text = false; 
      delay(3000);     
    } 
    tym(); 

   if(s1 == 1){
    lcd.clear();
    lcd.setCursor(5, 0);
    lcd.print("BULB");    
    lcd.setCursor(1, 1);
    lcd.print("--| IS ON |--");
    delay(3000);    
   } 
   if(s2 == 1){
    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("TUBE LIGHT");    
    lcd.setCursor(1, 1);
    lcd.print("--| IS ON |--");
    delay(3000);    
   } 
   if(s3 == 1){
    lcd.clear();
    lcd.setCursor(6, 0);
    lcd.print("FAN");    
    lcd.setCursor(1, 1);
    lcd.print("--| IS ON |--");
    delay(3000);    
   } 
   if(s4 == 1){
    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("EXHAUST");    
    lcd.setCursor(1, 1);
    lcd.print("--| IS ON |--");
    delay(3000);    
   }
  }
}

void setup() {
  //Set the LED pin as an output pin
  Serial.begin(9600);
  //Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  WiFi.begin(ssid, pass);
  timer.setInterval(2000L, checkBlynkStatus);
  // check if Blynk server is connected every 10 seconds  
  Blynk.config(auth);
  //delay(1000);

/////////////////////////////////////////////
  pinMode(RelayPin1, OUTPUT);
  pinMode(RelayPin2, OUTPUT);
  pinMode(RelayPin3, OUTPUT);
  pinMode(RelayPin4, OUTPUT);
  digitalWrite(RelayPin1, !toggleState_1);
  digitalWrite(RelayPin2, !toggleState_2);
  digitalWrite(RelayPin3, !toggleState_3);
  digitalWrite(RelayPin4, !toggleState_4);
  if (!fetch_blynk_state){
    Blynk.virtualWrite(V_1, toggleState_1);
    Blynk.virtualWrite(V_2, toggleState_2);
    Blynk.virtualWrite(V_3, toggleState_3);
    Blynk.virtualWrite(V_4, toggleState_4);
  }
/////////////////////////////////////////////

/////////////////////////////////////////////
  Wire.begin(14,12); //sda(D5),scl(D6)
  timeClient.setTimeOffset(offset);
  timeClient.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();  
/////////////////////////////////////////////
}

void loop() {
  //Run the Blynk library
  Blynk.run();
  timer.run();
  
  disply();
  //lcd.clear();
  //lcd.print("HI");

}