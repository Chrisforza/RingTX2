#include <lpc17xx.h>
#include "stdio.h"
#include <stdint.h>
#include "GLCD.h"
#include <cmsis_os2.h>
#include <stdbool.h>

unsigned char *stringPointer;
unsigned char pressed[13] = {'P','R','E','S','S','E','D',' ',' ',' ',' ',' '};
unsigned char notPressed[13] = {'N','O','T',' ','P','R','E','S','S','E','D',' ',};
unsigned char up[13] = {'U','P',' ',' ',' ',' ',' ',' ',' ',' ',' ',' '};
unsigned char down[13] = {'D','O','W','N',' ',' ',' ',' ',' ',' ',' ',' '};
unsigned char left[13] = {'L','E','F','T',' ',' ',' ',' ',' ',' ',' ',' '};
unsigned char right[13] = {'R','I','G','H','T',' ',' ',' ',' ',' ',' ',' '};
unsigned char noDir[13] = {'N','O',' ','D','I','R',' ',' ',' ',' ',' ',' '};
bool ledLastOn = false;

void setLEDs(uint32_t n) {
	
	if(n & 1)
		LPC_GPIO1->FIOSET |= 1U<<28;
	else 
		LPC_GPIO1->FIOCLR |= 1U<<28;
	
	if(n & 2)
		LPC_GPIO1->FIOSET |= 1U<<29;
	else 
		LPC_GPIO1->FIOCLR |= 1U<<29;
	
	if(n & 4)
		LPC_GPIO1->FIOSET |= 1U<<31;
	else 
		LPC_GPIO1->FIOCLR |= 1U<<31;
	
	if(n & 8)
		LPC_GPIO2->FIOSET |= 1U<<2;
	else 
		LPC_GPIO2->FIOCLR |= 1U<<2;
	
	if(n & 16)
		LPC_GPIO2->FIOSET |= 1U<<3;
	else 
		LPC_GPIO2->FIOCLR |= 1U<<3;
	
	if(n & 32)
		LPC_GPIO2->FIOSET |= 1U<<4;
	else 
		LPC_GPIO2->FIOCLR |= 1U<<4;
	
	if(n & 64)
		LPC_GPIO2->FIOSET |= 1U<<5;
	else 
		LPC_GPIO2->FIOCLR |= 1U<<5;
	
	if(n & 128)
		LPC_GPIO2->FIOSET |= 1U<<6;
	else 
		LPC_GPIO2->FIOCLR |= 1U<<6;
}

bool checkPushbutton(void) {
	
	while(1){
		
		if (!((LPC_GPIO2->FIOPIN) & (1U<<10))) {
			printf("\nPUSHBUTTON PRESSED");	
			return true;
		}
		else 
			return false;
	}
}
void checkJoystickPushed(void){
	
		if (!((LPC_GPIO1->FIOPIN) & (1U<<20))){
			stringPointer = pressed;
			GLCD_DisplayString(50, 0, 1, stringPointer);
		}
		else {
			stringPointer = notPressed;
			GLCD_DisplayString(50, 0, 1, stringPointer);
		}
}

void checkJoystickDir(void) {
	
		if (!((LPC_GPIO1->FIOPIN) & (1U<<23))){
			stringPointer = up;
			GLCD_DisplayString(0, 0, 1, stringPointer);
		}
		else if (!((LPC_GPIO1->FIOPIN) & (1U<<24))){
			stringPointer = right;
			GLCD_DisplayString(0, 0, 1, stringPointer);
		}
		else if (!((LPC_GPIO1->FIOPIN) & (1U<<25))){
			stringPointer = down;
			GLCD_DisplayString(0, 0, 1, stringPointer);
		}
		else if (!((LPC_GPIO1->FIOPIN) & (1U<<26))){
			stringPointer = left;
			GLCD_DisplayString(0, 0, 1, stringPointer);
		}
		else{
			stringPointer = noDir;
			GLCD_DisplayString(0, 0, 1, stringPointer);
		}
}

void checkPot(void) {
	
	uint32_t polled = 0;
		
	LPC_ADC->ADCR |= 1U<<24;
	
	while(!(LPC_ADC->ADGDR & (1U<<31)));
		
		polled =  LPC_ADC->ADGDR;
		
		//Sets all bits after 15 to 0
		polled &= 32767;
		//Left shift by 4
		polled = polled >> 3U;
		//Print polled value
		printf("\nPolled: %d", polled);
	
}

//Thread definitions
void readPotentiometer(void *args) {
	while(1)
	{
		checkPot();
		osThreadYield();
	}
}

void readJoystick(void *args) {
	while(1)
	{
		while(1){
			checkJoystickDir();
			checkJoystickPushed();
		}
		
		osThreadYield();
	}
}

void toggleLED(void *args) {
	while(1)
	{
		if (checkPushbutton()) {
			if(ledLastOn){
				while(checkPushbutton()); //wait for button to be released before moving on, to avoid flickering
				LPC_GPIO1->FIOCLR |= 1U<<28; //if LED's last state was on, and the push button has been pushed, turn LED off
				ledLastOn = false;
			}else {
				while(checkPushbutton()); //wait for button to be released before moving on, to avoid flickering
				LPC_GPIO1->FIOSET |= 1U<<28; //if LED's last state was off, and the push button has been pushed, turn LED on
				ledLastOn = true;
			}	
		}
		osThreadYield();
	}
}

int main(void) {
	
	//Initialize variables for LCD display
	
	SystemInit();
	GLCD_Init();
	osKernelInitialize();
	
	printf("\nMy name is Mike and I use 3 teabags per tea...also starting kernel");
	GLCD_Clear(0x001F); //clear screen to initialize as blank screen
	
	//Create Threads
	
	osThreadNew(readPotentiometer, NULL, NULL);
	osThreadNew(readJoystick, NULL, NULL);
	osThreadNew(toggleLED, NULL, NULL);
	
	LPC_GPIO1->FIOCLR |= 0;
	LPC_GPIO1->FIOCLR |= 0;

	//Set LED's to '1' - Output pins
	LPC_GPIO1->FIODIR |= 1U<<28;
	LPC_GPIO1->FIODIR |= 1U<<29;
	LPC_GPIO1->FIODIR |= 1U<<31;
	LPC_GPIO2->FIODIR |= 1U<<2;
	LPC_GPIO2->FIODIR |= 1U<<3;
	LPC_GPIO2->FIODIR |= 1U<<4;
	LPC_GPIO2->FIODIR |= 1U<<5;
	LPC_GPIO2->FIODIR |= 1U<<6;
	
	//Clear PINSEL registers (ADC)
	LPC_PINCON->PINSEL1 &= 0U<<18;
	LPC_PINCON->PINSEL1 &= 0U<<19;
	//Set PINSEL bit 18 (ADC)
	LPC_PINCON->PINSEL1 |= 1U<<18;
	//Power on ADC by setting bit 12 of PCONP
	LPC_SC->PCONP |= 1U<<12;
	//Set clock frequency
	LPC_ADC->ADCR = (1<<2)|(4<<8)|(1<<21);
	
	//Display "Hello World!" on blue background
	GLCD_SetBackColor(0x001F);
	
	osKernelStart();

	while(1) {	
	}
}
