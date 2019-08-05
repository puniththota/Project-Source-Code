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
#include <crypto.h>
// Transmitter is node 1
// Receiver is node 2

#define NODE 0x0002

// Radio receive modes
#define NORMAL 0
#define RAW 1

//long t;
static unsigned char buffer[128];
static unsigned char mode=NORMAL;
static unsigned char plain[128];
static unsigned char cipher[128];
//crypto key initialized 
unsigned char key[]={0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x04,0x01};

//radio and buttons initialized 
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
	crypto_init(key);
}

static long counter=0;
static int first=true;
static int flag=false;
static unsigned long until = 0;
int rcode=0;
int tcode=0;


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
	     until=time_millis()+1000L;
		 rcode = rcode+1;
		 flag = true;
		 leds_on(LED_GREEN);
		 plain[0]= rcode;
		 printf("hello message sent: %02x \n",rcode);
		 crypto_encrypt(plain,cipher);
	     radio_send(cipher,16,0x0001);
		
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

  
   if (radio_rxed()) {
		if (mode==NORMAL) {
		    if(until>=time_millis()){
			int rxLen=radio_recv(&buffer[0], 128, &dst, &src, &valid, &energy, &lqi);
			printf("Message received from transmitter %02x to receiver %02x with data :",
			src,dst);
			for(int i=0;i<rxLen;i++)
			{
				printf("%02x",buffer[i]);
			}
			crypto_decrypt(buffer,plain);
			printf("Message decrypted:");
			for(int i=0;i<4;i++){
				printf("%02x ",plain[i]);
			}
			printf("Response received within the time frame\n");
			if(plain[0]>rcode){
				if(dst==2 && src==1){
					printf("Valid code received and source,destination valid \n");
					printf("Lock opened\n");
					leds_on(LED_ORANGE);
					printf("Lock closed");
					rcode=plain[0];
				}
				else
				{
					printf("Source or destination is invalid \n");
				}
			}
			}
			else{
				printf("Response not received within time frame\n");
				printf("Action not performed \n");
			}
		}
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
		   printf("Echoed.\n\r");
		}
   }
	   
}

