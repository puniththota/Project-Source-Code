/*
 * txdemo.c
 *
 * Created: 23/04/2019 13:28:05
 * Author : punith
 */ 

#include <stdio.h>
#include <serial.h>
#include <leds.h>
#include <button.h>
#include <radio.h>
#include <time.h>
#include <crypto.h>

// Transmitter is node 1
// Receiver is node 2

#define NODE 0x0001
#define DESTINATION 0x0002

static unsigned char buffer[128];
//initalizing the aes key used for encryption
unsigned char key[]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x04,0x01};
static unsigned char cipher[128];
int i;

//radio and buttons initialized
void init() 
{
	leds_init();
	leds_on(LED_RED);
	serial_init(9600);
	printf("Demo TX program running on node %04x\n\r", NODE);
	leds_on(LED_ORANGE);
	button_init();
	radio_init(NODE);
	radio_start();
	crypto_init(key);
}

static unsigned int counter=0;
static unsigned long until=0;

void loop() {
    unsigned short dst=0;
	unsigned short src=0;
	unsigned char valid=0;
	unsigned char energy=0;
	unsigned char lqi=0;
   		   
	buffer[0] = '0';
	buffer[1] = 'abc';
	if (button_pressed()) {
		while (button_pressed());
		buffer[2] = '123';
		counter++;
		leds_on(LED_GREEN);
		buffer[3]=counter;
		printf("Next code generated %d ",counter);
		printf("Request built at %lu: %02x %02x %2x %2x\n\r", time_millis(), buffer[0], buffer[1], buffer[2],buffer[3]);
		crypto_encrypt(buffer,cipher);  //encrypting the data packet using crypto_encrypt method
		printf("Request encrypted :\n");
		for (int i=0;i<16;i++)
		{
		printf("%02x ",cipher[i]);
		}
		radio_send(cipher,16,0x0002);
		leds_off(LED_GREEN);
		printf("Request sent to receiver :%04x \n",DESTINATION);

		until=time_millis()+1000;
	}
	   
}
