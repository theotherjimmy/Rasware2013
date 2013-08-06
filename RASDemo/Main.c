#include <inc/hw_types.h>		// tBoolean
#include "RASDemo.h"
//#include <utils/uartstdio.h>	// input/output over UART
#include <../RASLib/inc/uart.h> 
#include <../RASLib/inc/init.h> 

#include <../RASLib/inc/time.h>

    
int main(void)
{	
	char ch;	  	 

	InitializeMCU();
	initUART();

	while(1) {
		printf("\nRAS Demo for Robotathon 2013\n");
		printf("  0=UART Demo\n  1=Motor Demo\n");
		printf("  2=Servo Demo\n  3=Line Sensor\n");
		printf("  4=IR Sensor Demo\n  5=Encoders Demo\n");
		
		printf(">> ");
		ch = getchar();
		putchar(ch);
		printf("\n");

		if (ch == '0') {
			printf("\nUART Demo\n");
			uartDemo();	 
		}
		else if (ch == '1') {
			printf("\nMotor Demo\n");
			initMotors();
			motorDemo(); 
		}
		else if (ch == '2') {
			printf("\nServo Demo\n");
			initServo();
			servoDemo();   
		}
		else if (ch == '3') {			   
			printf("\nLine Sensor Demo\n");
			initLineSensor();		  
			lineSensorDemo();
		}
		else if (ch == '4') {	   
			printf("\nIR Sensor Demo\n");
			initIRSensor();
			IRSensorDemo();	 
		}
		else if (ch == '5') {
			printf("\nEncoders Demo\n");
			initEncoders();
			encoderDemo();
		}
	}
}
