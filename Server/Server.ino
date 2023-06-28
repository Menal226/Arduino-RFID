//COM4
#include <Dictionary.h>
#include <DictionaryDeclarations.h>
#include <RTClib.h>
#include <SoftwareSerial.h>

#define RX_PIN 5 //The blue cable
#define TX_PIN 3 //The white cable

Dictionary *minEntryTime = new Dictionary();
Dictionary *maxEntryTime = new Dictionary();
Dictionary *lastEntryTime = new Dictionary();

RTC_DS1307 rtc;
SoftwareSerial ardvComm (RX_PIN, TX_PIN);
char buffer[128];
int buffer_length = 0;
char allowedIDs[10][15];

void setup() {
  rtc.begin();
  ardvComm.begin(9600);
  Serial.begin(9600);
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  dict->insert("xd", "xd");
  Serial.println(dict->search("xd"));
  strcpy(allowedIDs[0], "19 225 7 183");
}

void loop() {
  if(ardvComm.available()>3){
    int p1 = ardvComm.read();
    int p2 = ardvComm.read();
    int p3 = ardvComm.read();
    int p4 = ardvComm.read();
    ardvComm.flush();
    sprintf(buffer,"%u %u %u %u", p1, p2, p3, p4);
    bool isIn = false;
    Serial.println(buffer);
    for(int i = 0; i < sizeof(allowedIDs)/sizeof(allowedIDs[0]); i++){
      if (strcmp(allowedIDs[i], buffer) == 0) isIn = true;
    }
    if(isIn){
      ardvComm.write(1);
      Serial.println("Ano");
    }else{
      Serial.println("Ne");
      ardvComm.write(2);
    }
  }
  if(Serial.available()){
    String reqFromPC = Serial.readString();


  }
}