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
   	
	   //buffer(data) to be sent initialized	   
	buffer[0] = '0';
	buffer[1] = 'abc';


	if (button_pressed()) {
		while (button_pressed());
		printf("Button pressed\n");
		buffer[2] = '123';
		leds_on(LED_GREEN);
		printf("Request built with data:");
		for (int i=0;i<3;i++)
		{
		printf("%02x ",buffer[i]);
		}
		radio_send(buffer,3,0x0002);  //sending the data to receiver using send method
		printf("Request sent to receiver %04x \n",DESTINATION);
		leds_off(LED_GREEN);
		until=time_millis()+1000;
	}
	   
}
