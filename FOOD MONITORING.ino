#include <LiquidCrystal.h>
#include "DHT.h"  
LiquidCrystal lcd(10, 9, 5, 4, 3, 2);
#include <SoftwareSerial.h>      
SoftwareSerial espSerial(12, 13);   
#define DHTTYPE DHT11
#define LDR_PIN A1
#define MQ3_PIN A0
DHT dht(8, DHTTYPE);     
#define DEBUG true
String mySSID = "Posh";       
String myPWD = "nnnnnnnn"; 
String myAPI = "28I9S8YRDULQ35B4";   
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1";
String myFIELD2 = "field2"; 
String myFIELD3 = "field3";
String myFIELD4 = "field4";  

float t=0;
float h=0;
int ldr_read = 0;
int gas_read = 0;


void setup()
{
  lcd.begin(16,2);
  dht.begin();
  lcd.setCursor(0,0);
  lcd.print(" IoT Based Smart");
  lcd.setCursor(0,1);
  lcd.print(" Food Monitoring");
  Serial.begin(9600);
  espSerial.begin(115200);
  
  espData("AT+RST", 1000, DEBUG);                      
  espData("AT+CWMODE=1", 1000, DEBUG);                 
  espData("AT+CWJAP=\""+ mySSID +"\",\""+ myPWD +"\"", 1000, DEBUG);   
  
  delay(1000);
  
}

  void loop()
  {

      lcd.clear();
      ldr_read =1023-analogRead(LDR_PIN);
      lcd.setCursor(0,0);
      lcd.print("LDR-");
      lcd.setCursor(4,0);
      lcd.print(ldr_read);
      lcd.setCursor(0,1);
      lcd.print("MQ3-");
      gas_read = analogRead(MQ3_PIN);
      lcd.setCursor(4,1);
      lcd.print(gas_read);
      Serial.print("Temperature = ");
      t = dht.readTemperature();
      lcd.setCursor(8,0);
      lcd.print("Tem-");
      lcd.setCursor(12,0);
      lcd.print(t);
      Serial.println(t);
      Serial.print("Humidity = ");
      h=dht.readHumidity();
      lcd.setCursor(8,1);
      lcd.print("Hum-");
      lcd.setCursor(12,1);
      lcd.print(h);
      Serial.println(h);

  
    
    String sendData = "GET /update?api_key="+ myAPI +"&"+ myFIELD +"="+String(ldr_read)+"&"+ myFIELD2 +"="+String(gas_read)+"&"+ myFIELD3 +"="+String(t)+"&"+ myFIELD4 +"="+String(h);
  
    espData("AT+CIPMUX=1", 1000, DEBUG);       
    espData("AT+CIPSTART=0,\"TCP\",\""+ myHOST +"\","+ myPORT, 1000, DEBUG);
    espData("AT+CIPSEND=0," +String(sendData.length()+4),1000,DEBUG);  
    espSerial.find(">"); 
    espSerial.println(sendData);
    Serial.print("Value to be sent: ");
    
     
    espData("AT+CIPCLOSE=0",1000,DEBUG);
    delay(9000);
  }

  String espData(String command, const int timeout, boolean debug)
{
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println("     ");
  
  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ( (time + timeout) > millis())
  {
    while (espSerial.available())
    {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug)
  {
   
  }
  return response;
}
