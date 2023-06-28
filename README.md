# Arduino-RFID

This is a project I did to learn how to communicate between two arduino boards.

When an RFID tag is placed near the scanner the client arduino sends it's ID to the server board. Server checks if the ID is allowed to enter in which case 1 is sent to the client board and green LED flashes. If the ID isn't allowed 2 is sent and red LED flashes.

Stuff to implement:
 1. Timed entrances = ID is only allowed in predefined timelimits
 2. Storing data in EEPROM = I want to store and recover the ID and times to and from EEPROM 
 3. Adding/setting/editting/deleting IDs using UART = Have the Server accept request over UART