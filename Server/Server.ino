
//COM4
#include <EEPROM.h>
#include <Dictionary.h>
#include <DictionaryDeclarations.h>
#include <RTClib.h>
#include <SoftwareSerial.h>

#define RX_PIN 5  //The blue cable
#define TX_PIN 3  //The white cable
#define ID0_OFFSET 0
#define ID1_OFFSET 1
#define ID2_OFFSET 2
#define ID3_OFFSET 3
#define MINHOUR_OFFSET 4
#define MINMIN_OFFSET 5
#define MAXHOUR_OFFSET 6
#define MAXMIN_OFFSET 7
#define LASTHOUR_OFFSET 8
#define LASTMIN_OFFSET 9

/*
EEPROM Memory structure
Max 10 ID at a time
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

[100] = Is place 1 full? 1:0
[101] = Is place 2 full? 1:0
...
*/

//Creates an instance of the RTC
RTC_DS1307 rtc;
//Creates communication between the arduinos
SoftwareSerial ardvComm(RX_PIN, TX_PIN);
char buffer[128];

void setup() {
  //Initializes the RTC
  rtc.begin();
  ardvComm.begin(9600);
  Serial.begin(9600);
  //Sets RTC time based on the PC time
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  //for(int i = 0; i<111; i++){
  //  EEPROM[i] = 0;
  //}
}

void loop() {
  //If I get tag ID from the other arduino
  if (ardvComm.available() > 3) {
    int memIdEEPROM = 0;
    int p1 = ardvComm.read();
    int p2 = ardvComm.read();
    int p3 = ardvComm.read();
    int p4 = ardvComm.read();
    ardvComm.flush();
    bool isAllowed = false;
    //Chech if the ID is allowed
    for (int i = 0; i <= 90; i = i + 10) {
      if (EEPROM[i] == p1 && EEPROM[i + 1] == p2 && EEPROM[i + 2] == p3 && EEPROM[i + 3] == p4) {
        isAllowed = true;
        memIdEEPROM = i;
      }
    }
    //Checks if the user is allowed to enter
    if (isAllowed) {
      //Now we need to check if the ID can enter at this time
      //Creates a DateTime object and set to it the current time
      DateTime now = rtc.now();
      //If the time is obviously in the limit
      if (EEPROM[memIdEEPROM + MINHOUR_OFFSET] < now.hour() && EEPROM[memIdEEPROM + MAXHOUR_OFFSET] > now.hour()) {
        answYes(memIdEEPROM, now);
      }
      //If the time is on the lower border but still in
      else if (EEPROM[memIdEEPROM + MINHOUR_OFFSET] == now.hour() && now.minute() >= EEPROM[memIdEEPROM + MINMIN_OFFSET]) {
        answYes(memIdEEPROM, now);
      }
      //If the time is on the uppper border but still in
      else if (EEPROM[memIdEEPROM + MAXHOUR_OFFSET] == now.hour() && now.minute() < EEPROM[memIdEEPROM + MAXMIN_OFFSET]) {
        answYes(memIdEEPROM, now);
      }
      //If the time isnt in the limit
      else {
        answNo();
      }
      //If the ID isnt on the allowed list
    } else {
      answNo();
    }
  }
  //If there's a command from PC
  if (Serial.available()) {
    //THIS IS SLOW AS FUCK, BUT I CANT BE FUCKED TO WRITE MY OWN FUNCTION, SO THIS WILL DO :)
    String reqFromPC = Serial.readString();
    //If i get the ADD command from PC
    if (reqFromPC[0] == 'A') {
      //Finds the 1st free space in EEPROM
      int avlID = -1;
      for
        for (int i = 100; i <= 110; i++) {
          if (EEPROM[i] == 0) {
            avlID = i;
            break;
          }
        }
      //If theres a free space in EEPROM
      if (avlID != -1) {
        //Sets the ID place as full
        EEPROM[avlID] = 1;
        avlID = (avlID - 100) * 10;
        //Get the ID of wanted device and and set its max time as always
        int d1, d2, d3, d4;
        sscanf(reqFromPC.c_str(), "A %i %i %i %i", &d1, &d2, &d3, &d4);
        EEPROM[avlID] = d1;
        EEPROM[avlID + ID1_OFFSET] = d2;
        EEPROM[avlID + ID2_OFFSET] = d3;
        EEPROM[avlID + ID3_OFFSET] = d4;
        EEPROM[avlID + MAXHOUR_OFFSET] = 23;
        EEPROM[avlID + MAXMIN_OFFSET] = 59;
        Serial.println("This ID has been added");
      } else {
        Serial.println("The maximum amount od IDs has already been registered.\nPlease remove one using the R commmand.");
      }
    }
    //If i get the REMOVE command from PC
    else if (reqFromPC[0] == 'R') {
      int d1, d2, d3, d4;
      sscanf(reqFromPC.c_str(), "R %i %i %i %i", &d1, &d2, &d3, &d4);
    }
    //If i get the TIME command from PC
    else if (reqFromPC[0] == 'T') {

    }
    //If i get the SHOW command from PC
    else if (reqFromPC[0] == 'S') {
      int ph = 1;
      for (int i = 100; i < 111; i++) {
        //If theres an entry to the eeprom on an address
        if (EEPROM[i] == 1) {
          int nr = (i - 100) * 10;
          //I hate this with a passion, but i cant think of a better way of fixing this RN so itll do
          //When I place it straight into the sprintf it just doesnt work. Im guessing that the answer is some pointer BS, but I cant be fucked
          int kys0 = EEPROM[nr];
          int kys1 = EEPROM[nr + ID1_OFFSET];
          int kys2 = EEPROM[nr + ID2_OFFSET];
          int kys3 = EEPROM[nr + ID3_OFFSET];
          int kys4 = EEPROM[nr + MINHOUR_OFFSET];
          int kys5 = EEPROM[nr + MINMIN_OFFSET];
          int kys6 = EEPROM[nr + MAXHOUR_OFFSET];
          int kys7 = EEPROM[nr + MAXMIN_OFFSET];
          int kys8 = EEPROM[nr + LASTHOUR_OFFSET];
          int kys9 = EEPROM[nr + LASTMIN_OFFSET];
          sprintf(buffer, "%i. ID: %i %i %i %i - minTime: %02i:%02i, maxTime: %02i:%02i, lastTime: %02i:%02i", ph, kys0, kys1, kys2, kys3, kys4, kys5, kys6, kys7, kys8, kys9);
          Serial.println(buffer);
          ph++;
        }
      }
      if (ph == 1) {
        Serial.println("The allowed ID list is empty");
      }
    }
    //If I get unknow command from PC
    else {
      Serial.println("Unknown command");
    }
  }
}

//Send the answer corresponding to NO to the other arduino
void answNo() {
  Serial.println("Denied");
  ardvComm.write(2);
}

//Send the answer corresponding to YES to the other arduino and logs the last time of entry
void answYes(int memIdEEPROM, DateTime now) {
  EEPROM[memIdEEPROM + LASTHOUR_OFFSET] = now.hour();
  EEPROM[memIdEEPROM + LASTMIN_OFFSET] = now.minute();
  Serial.println("Allowed");
  ardvComm.write(1);
}