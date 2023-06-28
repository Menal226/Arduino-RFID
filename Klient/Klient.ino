//COM3

#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

#define SDA_PIN 10
#define RST_PIN 9
#define RX_PIN 5 //The blue cable
#define TX_PIN 3 //The white cable
#define GREEN_PIN 7
#define RED_PIN 6

MFRC522 rfid(SDA_PIN, RST_PIN);
SoftwareSerial ardvComm (RX_PIN, TX_PIN);
char buffer[128];

void setup() {
  Serial.begin(9600);
  ardvComm.begin(9600);
  SPI.begin();
  rfid.PCD_Init();
  pinMode(GREEN_PIN,OUTPUT);
  pinMode(RED_PIN,OUTPUT);
}

void loop() {
  //If theres a tag nearby
  if (rfid.PICC_IsNewCardPresent()) {
    //If i manage to read the tag
    if (rfid.PICC_ReadCardSerial()) {
      Serial.println(rfid.uid.uidByte[0]);
      Serial.println(rfid.uid.uidByte[1]);
      Serial.println(rfid.uid.uidByte[2]);
      Serial.println(rfid.uid.uidByte[3]);
      
      ardvComm.write(rfid.uid.uidByte[0]);
      ardvComm.write(rfid.uid.uidByte[1]);
      ardvComm.write(rfid.uid.uidByte[2]);
      ardvComm.write(rfid.uid.uidByte[3]);
      //End comm with the tag in order to prevent multiple readings of the same tag
      rfid.PICC_HaltA();
      rfid.PCD_StopCrypto1();
    } 
  } 
  if(ardvComm.available()){
    int answer = ardvComm.read();
    if(answer == 1){
      digitalWrite(GREEN_PIN,HIGH);
      delay(1000);
      digitalWrite(GREEN_PIN, LOW);
    }else {
      digitalWrite(RED_PIN,HIGH);
      delay(1000);
      digitalWrite(RED_PIN, LOW);
    }
    ardvComm.flush();
  }
}
void vypisHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}
