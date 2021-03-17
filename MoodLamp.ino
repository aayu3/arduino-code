/* Creation Crate Mood Lamp
This lamp smoothly cycles through a colour spectrum.
Colour Equations (we'll be using these later):
Red = sin(x)
Green = sin(x + PI/3)
Blue = sin(x+ 2PI/3)
These equations are how the program will calculate the brightness of
the LEDs.

Step 1: Input User Defined Variables
Think of variables like containers - they're used to store information. */

//This value controls how fast the mood lamp runs. You can replace this with any whole number.
int pulseSpeed = 5;

//These are the pins we are using with the UNO R# (Arduino-compatible). You can see the numbers on the board itself.
int ldrPin = 0; //LDR in Analog Input 0 to read the surrounding light.
int redLed = 11; // red LED in Digital Pin 11.
int greenLed = 10; //green LED in Digital Pin 10.
int blueLed = 9; //blue LED in Digital Pin 9

int ambientLight; //This variable stores the value of the light in the room
int power = 150; //This variable controls the brightness of the lamp (2-255).
float RGB[3]; //This is an array. It can hold 3 values: one for each of the three LEDs
