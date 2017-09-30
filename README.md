RFTransmitter
===========

![433 MHz modules](https://github.com/zeitgeist87/RFTransmitter/raw/master/images/xy-mk-5v.jpg)

Adapted for RaspberryPi/OrangePi (as transmitter) version of the Arduino library for low-cost 433 MHz transmitter modules with a focus on
reliable one-way communication and forward error correction. 
It is designed for use with Arduino receiver (with original RFReceiver library). 

The corresponding (original) [RFReceiver library](https://github.com/zeitgeist87/RFReceiver)
for the 433 MHz receiver modules can
be found [here](https://github.com/zeitgeist87/RFReceiver).

Links to "wiringpi.h" libraries:
RaspberryPi: http://wiringpi.com/download-and-install/
OrangePi-Zero: https://github.com/xpertsavenue/WiringOP-Zero
OrangePi-PC: https://github.com/zhaolei/WiringOP


Changes that I've made
----- 
1) "pulseLenght" is set to 99 in object construcor. It must be 99 or lower (with 100 and above RaspberryPi/OrangePi will not work).
So receiver must be configured accordingly: "RFReceiver receiver(INPUT_PIN, 99); //sets pulseLength to 99".

2) Added libraries necessary for compiling on linux.
3) Changed some variable types, so they are compatible with linux copilers
4) Changed the random function. It defers from the one for arduino 

IMPORTANT NOTE
-----
Use almost perfect 5V power supply for Receiver module (USB port cant give you such power, use additional power supplies), 
otherwise you will experience strange behaviour while transmitting messages to it!

Howewer transmitter module can operate with 3.3V - 5V power supply.

Usage
-----

Receiver side: initialize the constructor with additional parameters before the "void setup()" function:
```cpp
//by default, in example "INPUT_PIN" is 2
RFReceiver receiver(INPUT_PIN, 99); //sets pulseLength to 99
```

Transmitter side:

```cpp
#include <wiringPi.h>
#include <string.h>

#define NODE_ID          1  // change this if you have second or third transmitter
#define OUTPUT_PIN       11 //replace this with your pin on RaspberryPi/OrangePi
#define pulseLength      99 //change it to lower if connection is not stable

//Send over digital pin "OUTPUT_PIN",identify as node 1
RFTransmitter transmitter(OUTPUT_PIN, NODE_ID, pulseLength);

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

```
g++ -lwiringPi -o [name of executable file as result] [name of main code files] [name of library’s “.cpp” file]
```

Example (case when all mentioned files were placed in one folder): 

```
g++ -lwiringPi -o HelloWorldTransmitter HelloWorldTransmitter.cpp RFTransmitter.cpp
```
After compilation just run "HelloWorldTransmitter" binary to send desired message. 
To run it if you are in the same folder, type this and press "Enter": 
```
./HelloWorldTransmitter
```


P.S.
-----
Because of the buffer overflow in the "millis()" function, after 50 days of running without restart, it would freeze and require the restart to operate normal again. 
I already have a workaround for this issue, and will try to implement it in this project as soon as possible.


