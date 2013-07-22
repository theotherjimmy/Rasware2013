#include <inc/hw_types.h>		// tBoolean
#include <inc/hw_memmap.h>
#include <utils/uartstdio.h>	// input/output over UART
#include <driverlib/gpio.h>
#include <driverlib/sysctl.h>

#include <RASLib/inc/motor.h>

#include "RASDemo.h"

tMotor *motors[2];

void initMotors(void) {
    motors[0] = InitializeMotor(PIN_A4, PIN_A5, false);
    motors[1] = InitializeMotor(PIN_A6, PIN_A7, false);
}

void motorDemo(void) {
	// 2012 Motor Demo
	UARTprintf("Press:\n  w-forward\n  s-backward\n  a-left\n  ");
	UARTprintf("d-right\n  space-stop\n  enter-quit\n");
	
	{
		// max speed is 1.0 but 0.75 lets us check for working pwm
		float left = 0, right = 0, speed = 0.75;
		char newline = 13;
		char ch = getc();
		while(ch != newline) {
			ch = getc();
			putc(ch);
			if (ch == 'w') {
				left = speed;
				right = -speed;
			} else if (ch == 's') {
				left = -speed;
				right = speed;
			} else if (ch == 'a') {
				left = -speed;
				right = -speed;
			} else if (ch == 'd') {
				left = speed;
				right = speed;
			} else if (ch == ' ') {
				left = 0;
				right = 0;
			}
			SetMotor(motors[0], left);
			SetMotor(motors[1], right);
			UARTprintf("Set Motor to 0x%x%x 0x%x%x\r", left, right);
		}
	}
	
	SetMotor(motors[0], 0.0f);
	SetMotor(motors[1], 0.0f);
	UARTprintf("\n");
}
