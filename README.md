# MasterMind-Boardgame

Our program successfully implements the MasterMind game. It meets all the requirements and functionalities. It has been written fully using C.

## Hardware Specification and Wiring
- Hardware Environment: The Raspberry Pi2 is used. The keyboard and mouse is used for input and the monitor is used for output.
- Wiring: Two LEDs are used as output, a green LED and a red LED. The green LED is connected to the RPi2 using GPIO pin 13 and the red LED is connected to the RPi2 using GPIO pin 5. A button is used as an input device and is connected to the RPi2 using GPIO pin 19.

## Functions that directly access the hardware (All the functions use C)
- void blinkLED(int pin, int noOfTimes)
- void setOutputMode(int pin)
- void setInputMode(int pin)
- int getButtonValue(int pin)
- int buttonClick()
- void main(int argc, char* argv[])

## Sample Execution in debug mode
![image](https://github.com/ayushiiamin/MasterMind-Boardgame/assets/77382840/766062db-b860-4289-b954-dfcd08566a42)

## Contributers
* Ayushi Amin (GitHub: @ayushiiamin)
* Harini Balamurugan (GitHub: @HariniBalamurugan)
