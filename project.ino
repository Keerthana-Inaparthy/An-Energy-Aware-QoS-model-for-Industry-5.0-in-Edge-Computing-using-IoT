#include <LiquidCrystal.h>
#include <stdio.h>
LiquidCrystal lcd(13, 12, 14, 27, 26, 25);

#include <WiFi.h>
#include <HTTPClient.h>

HTTPClient http;

const char *ssid = "iotserver";
const char *password = "iotserver123";

String smoke_string="";

int httpResponseCode;
String servername = "http://projectsfactoryserver.in/storedata.php?name=";
String accountname = "iot869";
String field1 = "&s1=";
String field2 = "&s2=";
String field3 = "&s3=";
String field4 = "&s4=";
String field5 = "&s5=";
String payload="";

#include "DHT.h"
#define DHTPIN 22
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

int buzzer  = 21;
int smoke   = 23;

float tempc=0,humc=0;

unsigned char rcv,count,gchr,gchr1,robos='s';
 
int sti=0;
String inputString = "";         
boolean stringComplete = false; 

int i=0,k=0,lop=0;
int  gps_status=0;
float latitude=0; 
float logitude=0;                       
String Speed="";
String gpsString="";
char *test="$GPRMC";

unsigned char gv=0,msg1[10],msg2[11];
 float lati=0,longi=0;
unsigned int lati1=0,longi1=0;
unsigned char flat[5],flong[5];
char finallat[10],finallong[10];
String finallat1="";
String finallong1="";

 int ii=0,rchkr=0;
 
void beep()
{
  digitalWrite(buzzer, LOW);delay(2000);digitalWrite(buzzer, HIGH);
}
void okcheck()
{
  unsigned char rcr;
  do{
      rcr = Serial.read();
    }while(rcr != 'K');
}

int cntlmk=0;
int stn=0;
int hbv=0;

String rain_string="";
String mos_string="";
//http://projectsfactoryserver.in/storedata.php?name=server65&lat=%s&lan=%s&s1=%s

void iot_send()
{
      lcd.setCursor(15,0);lcd.print("U");
      http.begin(servername + accountname + field1 + String(tempc) + field2 + String(humc) + field3 + smoke_string);
      httpResponseCode = http.GET();
      if(httpResponseCode>0) 
        {
         payload="";
         payload = http.getString();
        }
      else 
        {
         ;
        }
      delay(3000); 
      lcd.setCursor(15,0);lcd.print(" ");
}         
         
void setup() 
{
  Serial.begin(9600);
  pinMode(smoke, INPUT);
  pinMode(buzzer, OUTPUT);

  digitalWrite(buzzer, HIGH);
  
  lcd.begin(16, 2);  
  lcd.print("   Welcome  ");
  delay(2500);
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) 
       {
           delay(500);
       }
  delay(3000);

  dht.begin(); 
  lcd.clear(); 
  lcd.print("T:");
  lcd.setCursor(8,0);
  lcd.print("H:");
   
  lcd.setCursor(0,1);
  lcd.print("S:");
}

void loop() 
{      
  humc  = dht.readHumidity();
  tempc = dht.readTemperature();
  lcd.setCursor(2,0);convertl(tempc);
  lcd.setCursor(10,0);convertl(humc);

  if(tempc > 40 || humc > 70)
    {
      beep();
      iot_send();
    }  

 smoke_string="";
 if(digitalRead(smoke) == LOW)
   {
      lcd.setCursor(2,1);lcd.print("ON  ");
      smoke_string = "ON";
      beep(); 
      iot_send();
   }   
 if(digitalRead(smoke) == HIGH)
   {
      lcd.setCursor(2,1);lcd.print("OFF ");
      smoke_string = "OFF";
   }
 delay(1000);
 cntlmk++;
 if(cntlmk >= 60)
   {cntlmk = 0;
      iot_send();
   }
}

void serialEvent() 
{
  while (Serial.available()) 
        {
         
         char inChar = (char)Serial.read();
           if(inChar == '*')
             {
              gchr = Serial.read();
             }
           if(inChar == '#')
             {
              gchr1 = Serial.read();
             }  
        }
}

 void convlat(unsigned int value)  
  {
      unsigned int a,b,c,d,e,f,g,h;
      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;              
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
   
      flat[0] = c;
      flat[1] = e;
      flat[2] = g;
      flat[3] = h;

  }

 void convlong(unsigned int value)  
  {
      unsigned int a,b,c,d,e,f,g,h;
      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;

      a=a|0x30;              
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
    
         
      flong[0] = c;
      flong[1] = e;
      flong[2] = g;
      flong[3] = h;
}

void converts(unsigned int value)
{
    unsigned int a,b,c,d,e,f,g,h;

    a=value/10000;
    b=value%10000;
    c=b/1000;
    d=b%1000;
    e=d/100;
    f=d%100;
    g=f/10;
    h=f%10;

    a=a|0x30;               
    c=c|0x30;
    e=e|0x30; 
    g=g|0x30;              
    h=h|0x30;
      
    Serial.write(a);
    Serial.write(c);
    Serial.write(e); 
    Serial.write(g);
    Serial.write(h);
}

void convertl(unsigned int value)
{
  unsigned int a,b,c,d,e,f,g,h;

      a=value/10000;
      b=value%10000;
      c=b/1000;
      d=b%1000;
      e=d/100;
      f=d%100;
      g=f/10;
      h=f%10;


      a=a|0x30;               
      c=c|0x30;
      e=e|0x30; 
      g=g|0x30;              
      h=h|0x30;
 
   lcd.write(e); 
   lcd.write(g);
   lcd.write(h);
}
