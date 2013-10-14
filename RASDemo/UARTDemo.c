#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
//#include <utils/uartstdio.h>	// input/output over UART
#include <driverlib/uart.h>		// input/output over UART
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>
#include "../RASLib/inc/uart.h"

#include "RASDemo.h"

void initUART(void) {
        InitializeUART();
}

void uartDemo(void) {
	printf("tell me something!\n-> ");
	
	{
		char charArray[100];
		fgets(charArray, 100, stdin);
		printf("you said, \"%s\"\n", charArray);
		printf("thanks for the input!\n");
	}
	
	{
		char newline = 13;
		char ch = getchar();
		while(ch != newline) {
			ch = getchar();
			putchar(ch);
		}
	}
}

int keyWasPressed(void) {
	if (UARTCharsAvail(UART0_BASE))	
		return 1;
	else
		return 0;
}
