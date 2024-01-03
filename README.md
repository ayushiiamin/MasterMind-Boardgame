# MasterMind-Boardgame

## Hardware Specification and Wiring
- Hardware Environment: The Raspberry Pi2 is used. The keyboard and mouse is used for input and the monitor is used for output.
- Wiring: Two LEDs are used as output, a green LED and a red LED. The green LED is connected to the RPi2 using GPIO pin 13 and the red LED is connected to the RPi2 using GPIO pin 5. A button is used as an input device and is connected to the RPi2 using GPIO pin 19.

## Specification of the Code
Pin 13 (Green LED) and pin 5 (Red LED) are set to output mode and pin 5 (button) is set to input mode. The secret is generated randomly using the rand() function and is stored in an integer array (int*) secret of length N. The green LED blinks once to indicate the beginning of the game.



## Team Members
* Ayushi Amin (GitHub: @ayushiiamin)
* Harini Balamurugan (GitHub: @HariniBalamurugan)
