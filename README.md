# Arduino-RFID

This is a project I did to learn how to communicate between two arduino boards. RTClib.h, MFRC522.h and SPI.h need to be present for the program to run.

When an RFID tag is placed near the scanner the client arduino sends it's ID to the server board using the pins 3 and 5. The server checks if the ID is allowed to enter in which case 1 is sent to the client board and green LED flashes. If the ID isn't allowed 2 is sent and red LED flashes. (Allowed to enter means that the ID is on the allowedlist and it is currently the within the aproved time interval. If the connection has been broken the blue light starts flashing.

Allowed IDs are stored in the servers EEPROM into which new one can be added/from which user can remove IDs by issuing command listed bellow. There can be at most 10 IDs stored at once.

**Commands for server**
Commands must be input with the "no new line" opion selected in the serial monitor
 1. A {ID in decimal}
	- This command adds a device with the specified ID to the list of allowed devices
	- Carefull: This sets the device's entry time as allways
	- Example: S 205 206 207 208
 2. D {ID in decimal}
	- This command removes the device with the specified ID from the list of allowed devices
	- This also clears all the setup time intervals asociated with ID (including last entry time)
	- Example: R 205 206 207 208
 3. T {ID in decimal} {start of the allowed time in format hh:mm} {end of the allowed time in format hh:mm}
	- This command sets a time interval in which the ID is allowed to enter
	- Example: T 205 206 207 208 16:00 18:00
 4. S
	- This command lists out all the currently allowed IDs together with their allowed times and last entry time
 5. R
	- Resets the entire EEPROM
