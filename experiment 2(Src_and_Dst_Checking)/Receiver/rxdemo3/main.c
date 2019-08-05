/*
 * rxdemo.c
 *
 * Created: 23/04/2019 13:28:30
 * Author : punith
 */ 


#include <stdio.h>
#include <serial.h>
#include <leds.h>
#include <button.h>
#include <radio.h>

// Transmitter is node 1
// Receiver is node 2

#define NODE 0x0002

// Radio receive modes
#define NORMAL 0
#define RAW 1

static unsigned char buffer[128];
static unsigned char mode=NORMAL;

radio and buttons intitialized
void init() 
{
	leds_init();
	leds_on(LED_RED);
	serial_init(9600);
	serial_puts("Demo program running.\n\r");
	leds_off(LED_RED);
	leds_on(LED_ORANGE);
	button_init();
	radio_init(NODE);
	radio_start();
}

static long counter=0;
static int first=true;
static int flag=false;
static unsigned long until = 0;


void loop() {
   	
   unsigned short dst=0;
   unsigned short src=0;
   unsigned char valid=0;
   unsigned char energy=0;
   unsigned char lqi=0;
	   
   if (first) {
	   leds_off(LED_ORANGE);
	   leds_on(LED_GREEN);
	   first = false;
   }
	   
   if ((++counter)>100000L) {
	   counter = 0;
	   if (flag) {
		   flag = false;
		   leds_off(LED_GREEN);
		   printf("off\n\r");
	   }
	   else {
		   flag = true;
		   leds_on(LED_GREEN);
		   printf("on...");
	   }
   }
	   
   if (button_pressed()) {
	   if (mode==NORMAL) {
		   leds_on(LED_ORANGE);
		   printf("Receive in raw mode.\n\r");		   
		   mode = RAW;
	   }
	   else {
		   leds_off(LED_ORANGE);
		   printf("Receive in normal mode.\n\r");		   
		   mode = NORMAL;		   
	   }
	   while (button_pressed()); 
   }

//if data is received this block is executed
   if (radio_rxed()) {
	   //normal mode of transmission
		if (mode==NORMAL) {
			int rxLen=radio_recv(&buffer[0], 128, &dst, &src, &valid, &energy, &lqi);   //reading the incoming data packet
			printf("Message received from source %04x to destination %04x with data:", 
					src, dst);
           for (int i=0;i<rxLen;i++)
           {
		   printf("%02x ",buffer[i]);
           }
			if(buffer[0]=='0'&&buffer[1]=='x'&&buffer[2]=='2'&&buffer[3]=='2'){
		    if(dst==2 && src==1){
			printf("Valid code received");
			printf("Source and destination valid \n");
			printf("Lock opened\n");
			leds_on(LED_ORANGE);
			printf("Lock closed\n");
			}
			else
			{
			printf("Source or destination is invalid \n");
			}
			}
			else{
			printf("Invalid code received \n");
			printf("Lock not opened \n");
			}
		}
		//raw mode of transmission
		else {
			int rxLen=radio_raw_recv(&buffer[0], 128, &valid, &energy);
			printf("Radio rx. valid=%d energy=%d len=%d. Data=", valid, energy, rxLen);
			for (int i=0; i<rxLen; i++) 
				printf("%02x ", buffer[i]);
			printf("=\"");
			for (int i=0; i<rxLen; i++) 
				if ((buffer[i]>=' ')&&(buffer[i]<='~'))
					printf("%c", buffer[i]);
				else
					printf(".");
		   printf("\"\n\r");
		   radio_raw_send(&buffer[0],rxLen-3);
		   printf("Echoed.\n\r");
		}
   }
	   
}

