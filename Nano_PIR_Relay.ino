#include <WiFiNINA.h>
#include "secrets.h"
#include "ThingSpeak.h"

#define PIR 5
#define M 3
#define Buzz 4



char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
WiFiClient  client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

void setup() {
  pinMode(PIR,INPUT);  
  pinMode(M,OUTPUT); 
  pinMode(Buzz,OUTPUT); 
  digitalWrite(M,HIGH);
  
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    while (true);
  }
  String fv = WiFi.firmwareVersion();
 if (fv != "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }
  ThingSpeak.begin(client); 
  Upload_data(0);
 
}

void loop() {
  int pd=digitalRead(PIR);
  int st1=ThingSpeak.readFloatField(myChannelNumber,1,myWriteAPIKey);
  
  if (st1==1 && pd==0){
   Upload_data(0);} 
   
  if (st1==0 && pd==1){
   digitalWrite(Buzz,HIGH);
   delay(100);
   digitalWrite(Buzz,LOW);
   delay(100);
   digitalWrite(Buzz,HIGH);
   delay(100);
   digitalWrite(Buzz,LOW);
   Upload_data(1);}  
   
  int gate=ThingSpeak.readFloatField(myChannelNumber,2,myWriteAPIKey);
  if (gate==1)
     {
      digitalWrite(M,LOW);
      delay(20000);
      digitalWrite(M,HIGH);}  
    
}

void Upload_data(int L){
   if (WiFi.status() != WL_CONNECTED) {
    //Serial.print("Attempting to connect to SSID: ");
    //Serial.println(SECRET_SSID);
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(ssid, pass);
      //Serial.print(".");
      delay(5000);
    }
    //Serial.println("\nConnected.");
  }  
  ThingSpeak.setField(1, L);
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if (x == 200) {
    //Serial.println("Channel update successful.");
  }   
}
