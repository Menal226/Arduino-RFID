
//COM4
#include <EEPROM.h>
#include <Dictionary.h>
#include <DictionaryDeclarations.h>
#include <RTClib.h>
#include <SoftwareSerial.h>

#define RX_PIN 5 //The blue cable
#define TX_PIN 3 //The white cable

/*
EEPROM Memory structure
Max 10 ID najednou
[x] = ID[0]
[x+1] = ID[1]
[x+2] = ID[2]
[x+3] = ID[3]
[x+4] = minHour
[x+5] = minMin
[x+6] = maxHour
[x+7] = maxMin
[x+8] = lastHour
[x+9] = lastMin
...

[100] = Je 1 místo plné? 1:0
[101] = Je 2 místo plné? 1:0
...
*/

//Creates an instance of the RTC
RTC_DS1307 rtc;
//Creates communication between the arduinos
SoftwareSerial ardvComm (RX_PIN, TX_PIN);
char buffer[128];

void setup() {
  //Initializes the RTC
  rtc.begin();
  ardvComm.begin(9600);
  Serial.begin(9600);
  //Sets RTC time based on the PC time
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  for(int i = 0; i<111; i++){
    EEPROM[i] = 0;
  }
}

void loop() {
  //If I get tag ID from the other arduino
  if(ardvComm.available()>3){
    int memIdEEPROM = 0;
    int p1 = ardvComm.read();
    int p2 = ardvComm.read();
    int p3 = ardvComm.read();
    int p4 = ardvComm.read();
    ardvComm.flush();
    bool isAllowed = false;
    //Chech if the ID is allowed
    for(int i = 0; i <= 90; i = i + 10){
      if(EEPROM[i] == p1 && EEPROM[i+1] == p2 && EEPROM[i+2] == p3 && EEPROM[i+3] == p4){
        isAllowed = true;
        memIdEEPROM = i;
      }     
    }
    //Checks if the user is allowed to enter
    if(isAllowed){
      //Now we need to check if the ID can enter at this time
      //Creates a DateTime object and set to it the current time
      DateTime now = rtc.now();
      //If the time is obviously in the limit
      if(EEPROM[memIdEEPROM+4] < now.hour() && EEPROM[memIdEEPROM+6] > now.hour()){
        answYes();
      }
      //If the time is on the lower border but still in
      else if(EEPROM[memIdEEPROM+4] == now.hour() && now.minute() >= EEPROM[memIdEEPROM+5]){
        answYes();
      }
      //If the time is on the uppper border but still in
      else if(EEPROM[memIdEEPROM+6] == now.hour() && now.minute() < EEPROM[memIdEEPROM+7]){
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
    //THIS IS SLOW AS FUCK, BUT I CANT BE FUCKED TO WRITE MY OWN FUNCTION, SO THIS WILL DO :)
    String reqFromPC = Serial.readString();
    //If i get the ADD command from PC
    if(reqFromPC[0] == 'A'){
      int avlID = -1;
      //Finds the 1st free space in EEPROM
      for(int i = 100; i <= 110; i++){
        if(EEPROM[i] == 0) {
          avlID = i;
          break;
        }
      }
      //If theres a free space in EEPROM
      if(avlID != -1){
        //Get the ID of wanted device and and set its max time as always
        char ph;
        int d1,d2,d3,d4;
        Serial.println(reqFromPC.c_str());
        sscanf(reqFromPC.c_str(),"A %i %i %i %i", &d1, &d2, &d3, &d4);
        EEPROM[avlID-100] = d1;
        EEPROM[avlID-99] = d2;
        EEPROM[avlID-98] = d3;
        EEPROM[avlID-97] = d4;
        EEPROM[avlID-94] = 23;
        EEPROM[avlID-93] = 59;
      }else{
        Serial.println("The maximum amount od IDs has already been registered.\nPlease remove one using the R commmand.");
      }
    }
    //If i get the REMOVE command from PC
    else if(reqFromPC[0] == 'R'){

    }
    //If i get the TIME command from PC
    else if(reqFromPC[0] == 'T'){

    }
    //If i get the SHOW command from PC
    else if(reqFromPC[0] == 'S'){

    }
    //If I get unknow command from PC
    else{
      Serial.println("Unknown command");
    }

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