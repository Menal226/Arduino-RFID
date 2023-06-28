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
  minEntryTime->insert("3 36 233 14", "16:00");
  maxEntryTime->insert("3 36 233 14", "18:00");
  strcpy(allowedIDs[0], "3 36 233 14");
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
    bool isAllowed = false;
    Serial.println(buffer);
    //Chech if the ID is allowed
    for(int i = 0; i < sizeof(allowedIDs)/sizeof(allowedIDs[0]); i++){
      if (strcmp(allowedIDs[i], buffer) == 0) isAllowed = true;
    }
    //Checks if the user is allowed to enter
    if(isAllowed){
      //Now we need to check if the ID can enter at this time
      //Creates a DateTime object and set to it the current time
      DateTime now = rtc.now();
      unsigned int maxHour;
      unsigned int maxMin;
      unsigned int minHour;
      unsigned int minMin;
      //Gets the minimal and maximal entry time of the specific ID and saves it into the specified vars
      sscanf(minEntryTime->search(buffer).c_str(),"%u:%u",&minHour, &minMin);
      sscanf(maxEntryTime->search(buffer).c_str(),"%u:%u",&maxHour, &maxMin);
      //If the time is obviously in the limit
      if(minHour < now.hour() && maxHour > now.hour()){
        answYes();
      }
      //If the time is on the lower border but still in
      else if(minHour == now.hour() && now.minute() >= minMin){
        answYes();
      }
      //If the time is on the uppper border but still in
      else if(maxHour == now.hour() && now.minute() < maxMin){
        answYes();
      }
      //If the time isnt in the limit
      else{
        answNo();
      }
    //If the ID isnt on the allowed list
    }else{
      answNo();
    }
  }
  //If there's a command from PC
  if(Serial.available()){
    String reqFromPC = Serial.readString();


  }
}
void loadFromEEPROM(){
  
}

//Send the answer corresponding to NO to the other arduino
void answNo(){
  Serial.println("Ne");
  ardvComm.write(2);
}

//Send the answer corresponding to YES to the other arduino
void answYes(){
  Serial.println("Ano");
  ardvComm.write(1);
}