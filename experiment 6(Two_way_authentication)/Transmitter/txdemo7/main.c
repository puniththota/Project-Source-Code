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
//crypto key used for encryption initialized
unsigned char key[]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x04,0x01};
static unsigned char cipher[128];
static unsigned char plain[128];
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

//hello message received with the task required to be performed(incrementing the counter value to one and sending back)
	if (radio_rxed()) {
		unsigned char energy;
		unsigned char valid;
		int rxLen = radio_recv(&buffer[0],128,&dst,&src,&valid,&energy,&lqi);  
		printf("Hello message received\n");
		crypto_decrypt(buffer,plain);
		printf("%02x ",plain[0]);
		printf("\n\r");
		plain[0]=plain[0]+1;
		printf("%02x ",plain[0]);
		crypto_encrypt(plain,cipher);
		printf("Sending the required response \n");
		radio_send(cipher,16,0x0002);
	} 
	}
	
