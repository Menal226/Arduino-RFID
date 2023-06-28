//COM4
#include <Dictionary.h>
#include <DictionaryDeclarations.h>
#include <RTClib.h>
#include <SoftwareSerial.h>

#define RX_PIN 5 //The blue cable
#define TX_PIN 3 //The white cable

//Creates dicts into which the times will be stored
Dictionary *minEntryTime = new Dictionary();
Dictionary *maxEntryTime = new Dictionary();
Dictionary *lastEntryTime = new Dictionary();

//Creates an instance of the RTC
RTC_DS1307 rtc;
//Creates communication between the arduinos
SoftwareSerial ardvComm (RX_PIN, TX_PIN);
char buffer[128];
//Array of allowed IDs
char allowedIDs[10][15];

void setup() {
  //Initializes the RTC
  rtc.begin();
  ardvComm.begin(9600);
  Serial.begin(9600);
  //Sets RTC time based on the PC time
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //dict->insert("xd", "xd");
  //Serial.println(dict->search("xd"));
  strcpy(allowedIDs[0], "19 225 7 183");
}

void loop() {
  //If I get tag ID from the other arduino
  if(ardvComm.available()>3){
    int p1 = ardvComm.read();
    int p2 = ardvComm.read();
    int p3 = ardvComm.read();
    int p4 = ardvComm.read();
    ardvComm.flush();
    //Format ID form later work
    sprintf(buffer,"%u %u %u %u", p1, p2, p3, p4);
    bool isIn = false;
    Serial.println(buffer);
    //Chech if the ID is allowed
    for(int i = 0; i < sizeof(allowedIDs)/sizeof(allowedIDs[0]); i++){
      if (strcmp(allowedIDs[i], buffer) == 0) isIn = true;
    }
    //Send if the ID is allowed or not to the other arduino
    if(isIn){
      ardvComm.write(1);
      Serial.println("Ano");
    }else{
      Serial.println("Ne");
      ardvComm.write(2);
    }
  }
  //If there's a command from PC
  if(Serial.available()){
    String reqFromPC = Serial.readString();


  }
}