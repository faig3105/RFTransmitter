RFTransmitter
===========

![433 MHz modules](https://github.com/zeitgeist87/RFTransmitter/raw/master/images/xy-mk-5v.jpg)

Adapted for RaspberryPi/OrangePi (as transmitter) version of the Arduino library for low-cost 433 MHz transmitter modules with a focus on
reliable one-way communication and forward error correction. 
It is designed for use with Arduino receiver (with original RFReceiver library). 

The corresponding (original) [RFReceiver library](https://github.com/zeitgeist87/RFReceiver)
for the 433 MHz receiver modules can
be found [here](https://github.com/zeitgeist87/RFReceiver).

Changes that I've made: 
1) "pulseLenght" is 99 by default. It must be 99 or lower (with 100 and above RaspberryPi/OrangePi will not work).
So receiver must be configured accordingly: "RFReceiver receiver(INPUT_PIN, 99); //sets pulseLength to 99".

2) Added libraries necessary for compiling on linux.
3) Changed some variable type so they are compatible with linux copilers
4) Changed random function. It defers from the one for arduino 


Usage
-----
At the receiver side initialize it with additional parameters:
RFReceiver receiver(INPUT_PIN, 99); //sets pulseLength to 99


![433 MHz module connection](https://github.com/zeitgeist87/RFTransmitter/raw/master/images/xy-mk-5v-connect.jpg)

```cpp
#include <wiringPi.h>
#include <string.h>

#define NODE_ID          1 // change this if you have second or third transmitter
#define OUTPUT_PIN       11 //replace this with your pin on RaspberryPi/OrangePi

//Send over digital pin "OUTPUT_PIN",identify as node 1
//you don't need to set the pulseLength to 99, I've already set it in the "RFTransmitter.h" file. 
RFTransmitter transmitter(OUTPUT_PIN, NODE_ID);

int main(void) 
{
	wiringPiSetup(); //initialize "wiringPi" settings
	pinMode(OUTPUT_PIN,  OUTPUT);
	
	for(;;;) //infite loop, instead of "void loop()" function
	{
		char const *msg = "Hello World!"; //if there are some problems, use "char *msg" instead of "char const *msg"
		transmitter.send((unsigned char *)msg, strlen(msg) + 1);

		delay(5000);
  
		transmitter.resend((unsigned char *)msg, strlen(msg) + 1); //If you want to resend the same code just to be sure it was delivered to receiver
	}
	return 0;
}
```

How to compile
-----
G++ compiler’s command: 

g++ -lwiringPi -o [name of executable file as result] [name of main code files] [name of library’s “.cpp” file]


example (case when all mentioned files were placed in one folder): 

g++ -lwiringPi -o HelloWorldTransmitter HelloWorldTransmitter.cpp RFTransmitter.cpp

