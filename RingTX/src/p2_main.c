#include <lpc17xx.h>
#include "stdio.h"
#include <stdint.h>

void setLEDs(uint32_t n) {
	
	if(n & 1)
		LPC_GPIO1->FIOSET |= 1<<28;
	else 
		LPC_GPIO1->FIOCLR |= 1<<28;
	
	if(n & 2)
		LPC_GPIO1->FIOSET |= 1<<29;
	else 
		LPC_GPIO1->FIOCLR |= 1<<29;
	
	if(n & 4)
		LPC_GPIO1->FIOSET |= 1<<31;
	else 
		LPC_GPIO1->FIOCLR |= 1<<31;
	
	if(n & 8)
		LPC_GPIO2->FIOSET |= 1<<2;
	else 
		LPC_GPIO2->FIOCLR |= 1<<2;
	
	if(n & 16)
		LPC_GPIO2->FIOSET |= 1<<3;
	else 
		LPC_GPIO2->FIOCLR |= 1<<3;
	
	if(n & 32)
		LPC_GPIO2->FIOSET |= 1<<4;
	else 
		LPC_GPIO2->FIOCLR |= 1<<4;
	
	if(n & 64)
		LPC_GPIO2->FIOSET |= 1<<5;
	else 
		LPC_GPIO2->FIOCLR |= 1<<5;
	
	if(n & 128)
		LPC_GPIO2->FIOSET |= 1<<6;
	else 
		LPC_GPIO2->FIOCLR |= 1<<6;
}

void checkJoystick() {
	
	while(1) {
	
		if (!((LPC_GPIO1->FIOPIN) & (1<<23)))
			printf("\nJOYSTICK UP");
		if (!((LPC_GPIO1->FIOPIN) & (1<<24)))
			printf("\nJOYSTICK RIGHT");
		if (!((LPC_GPIO1->FIOPIN) & (1<<25)))
			printf("\nJOYSTICK DOWN");
		if (!((LPC_GPIO1->FIOPIN) & (1<<26)))
			printf("\nJOYSTICK LEFT");	
		if (!((LPC_GPIO1->FIOPIN) & (1<<20)))
			printf("\nJOYSTICK PRESSED");	
		if (!((LPC_GPIO2->FIOPIN) & (1<<10)))
			printf("\nPUSHBUTTON PRESSED");	
	}
}

int main(void) {
	SystemInit();
	
	//printf("\nHola Diabeto, Roll Back to RTOS");
	//printf("\nMy name is Mike and I use 3 teabags per tea");
	
	LPC_GPIO1->FIOCLR |= 0;
	LPC_GPIO1->FIOCLR |= 0;

	//Set LED's to '1' - Output pins
	LPC_GPIO1->FIODIR |= 1<<28;
	LPC_GPIO1->FIODIR |= 1<<29;
	LPC_GPIO1->FIODIR |= 1<<31;
	LPC_GPIO2->FIODIR |= 1<<2;
	LPC_GPIO2->FIODIR |= 1<<3;
	LPC_GPIO2->FIODIR |= 1<<4;
	LPC_GPIO2->FIODIR |= 1<<5;
	LPC_GPIO2->FIODIR |= 1<<6;
	
	//setLEDs(123);
	checkJoystick();

	while(1);
}
