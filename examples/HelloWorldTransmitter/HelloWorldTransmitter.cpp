#include <RFTransmitter.h>
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
	
	for(;;) //infite loop, instead of "void loop()" function
	{
		char const *msg = "Hello World!"; //if there are some problems, use "char *msg" instead of "char const *msg"
		transmitter.send((unsigned char *)msg, strlen(msg) + 1);

		delay(200);
  
		transmitter.resend((unsigned char *)msg, strlen(msg) + 1); //If you want to resend the same code just to be sure it was delivered to receiver
	}
	return 0;
}

void loop() {
  
}