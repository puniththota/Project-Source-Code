/*
 * txdemo.c
 *
 * Created: 23/04/2019 13:28:05
 * Author : p190047
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

//intializing the radio and buttons
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
   
   //intializing the buffer(data) to be sent to receiver		   
	buffer[0] = '0';
	buffer[1] = 'x';
	buffer[3] = '2';
	
	if (button_pressed()) {
		while (button_pressed()); 
		printf("Button pressed \n");
		buffer[2] = '2';
		leds_on(LED_GREEN);
		radio_send(buffer,4,0x0002);  //method to send data
		printf("Request sent from transmitter %04x to receiver %04x with data:",NODE,DESTINATION);
		for (int i=0;i<4;i++)
		{
		  printf("%02x ",buffer[i]);
		}
		leds_off(LED_GREEN);
		until=time_millis()+1000;
	}
	   
}
