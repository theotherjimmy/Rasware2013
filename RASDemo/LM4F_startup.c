/*
* Copyright (c) 2012, Mauro Scomparin
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * Neither the name of Mauro Scomparin nor the
*       names of its contributors may be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY Mauro Scomparin ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL Mauro Scomparin BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* File:     LM4F_startup.c.
* Author:   Mauro Scomparin <http://scompoprojects.worpress.com>.
* Version:    1.0.0.
* Description:  LM4F120H5QR startup code.
*/

#include <StellarisWare/inc/hw_nvic.h>
#include <StellarisWare/inc/hw_types.h>


//-----------------------------------------------------------------------------
//               Functions declarations
//-----------------------------------------------------------------------------

// Main should be defined on your main file so it's extern.
extern int main(void);
// rst_handler contains the code to run on reset.
void rst_handler(void);
// nmi_handler it's the code for an non maskerable interrupt.
void nmi_handler(void);
// this is just the default handler.
void empty_def_handler(void);
// this is the code for an hard fault.
void hardfault_handler(unsigned int * hardfault_args) ;
extern void SysTickHandler(void);
extern void PortAHandler(void);
extern void PortBHandler(void);
extern void PortCHandler(void);
extern void PortDHandler(void);
extern void PortEHandler(void);
extern void PortFHandler(void);
extern void I2C0Handler(void);
extern void I2C1Handler(void);
extern void I2C2Handler(void);
extern void I2C3Handler(void);
extern void I2C4Handler(void);
extern void I2C5Handler(void);
extern void ADC0SS0Handler(void);
extern void ADC0SS1Handler(void);
extern void ADC1SS0Handler(void);
extern void ADC1SS1Handler(void);
extern void Timer5Handler(void);
extern void WTimer0AHandler(void);
extern void WTimer0BHandler(void);
extern void WTimer1AHandler(void);
extern void WTimer1BHandler(void);
extern void WTimer2AHandler(void);
extern void WTimer2BHandler(void);
extern void WTimer3AHandler(void);
extern void WTimer3BHandler(void);
extern void WTimer4AHandler(void);
extern void WTimer4BHandler(void);
extern void WTimer5AHandler(void);
extern void WTimer5BHandler(void);
extern void SysTickHandler(void);

//-----------------------------------------------------------------------------
//                 Variables declarations
//-----------------------------------------------------------------------------

// defined by the linker it's the stack top variable (End of ram)
extern unsigned long _estack;
// defined by the liker, this are just start and end marker for each section.
// .text (code)
extern unsigned long _stext;
extern unsigned long _etext;
// .data (data to be copied on ram)
extern unsigned long _sdata;
extern unsigned long _edata;
// .bss (uninitialized data to set to 0);
extern unsigned long _sbss;
extern unsigned long _ebss;

int __errno;

// NVIC ISR table
// the funny looking void(* myvectors[])(void) basically it's a way to make cc accept an array of function pointers.
__attribute__ ((section(".isr_vector")))
void(* myvectors[])(void) = {
  // This are the fixed priority interrupts and the stack pointer loaded at startup at R13 (SP).
  //                        VECTOR N (Check Datasheet)
  // here the compiler it's boring.. have to figure that out
  (void (*)(void)) &_estack, 
                // stack pointer should be 
              // placed here at startup.      0
    rst_handler,      // code entry point         1
    nmi_handler,      // NMI handler.           2
  (void (*)(void)) hardfault_handler,    // hard fault handler.        3
    // Configurable priority interruts handler start here.
    empty_def_handler,    // Memory Management Fault      4
    empty_def_handler,    // Bus Fault            5
    empty_def_handler,    // Usage Fault            6
    0,            // Reserved             7
    0,            // Reserved             8
    0,            // Reserved             9
    0,            // Reserved             10
    empty_def_handler,    // SV call              11
    empty_def_handler,    // Debug monitor          12
    0,            // Reserved             13
    empty_def_handler,    // PendSV             14
  SysTickHandler,
    // Peripherial interrupts start here.
  PortAHandler,    // GPIO Port A            16
    PortBHandler,    // GPIO Port B            17
    PortCHandler,    // GPIO Port C            18
    PortDHandler,    // GPIO Port D            19
    PortEHandler,    // GPIO Port E            20
    empty_def_handler,    // UART 0             21
    empty_def_handler,    // UART 1             22
    empty_def_handler,    // SSI 0              23
    I2C0Handler,    // I2C 0              24
    0,            // Reserved             25
    0,            // Reserved             26
    0,            // Reserved             27
    0,            // Reserved             28
    0,            // Reserved             29
    ADC0SS0Handler,    // ADC 0 Seq 0            30
    ADC0SS1Handler,    // ADC 0 Seq 1            31
    empty_def_handler,    // ADC 0 Seq 2            32
    empty_def_handler,    // ADC 0 Seq 3            33
    empty_def_handler,    // WDT 0 and 1            34
    empty_def_handler,    // 16/32 bit timer 0 A        35
    empty_def_handler,    // 16/32 bit timer 0 B        36
    empty_def_handler,    // 16/32 bit timer 1 A        37
    empty_def_handler,    // 16/32 bit timer 1 B        38
    empty_def_handler,    // 16/32 bit timer 2 A        39
    empty_def_handler,    // 16/32 bit timer 2 B        40
    empty_def_handler,    // Analog comparator 0        41
    empty_def_handler,    // Analog comparator 1        42
    0,            // Reserved             43
    empty_def_handler,    // System control         44
    empty_def_handler,    // Flash + EEPROM control     45
    PortFHandler,    // GPIO Port F            46
    0,            // Reserved             47
    0,            // Reserved             48
    empty_def_handler,    // UART 2             49
    empty_def_handler,    // SSI 1              50
    empty_def_handler,    // 16/32 bit timer 3 A        51
    empty_def_handler,    // 16/32 bit timer 3 B        52
    I2C1Handler,    // I2C 1              53
    0,            // Reserved             54
    empty_def_handler,    // CAN 0              55
    0,            // Reserved             56
    0,            // Reserved             57
    0,            // Reserved             58
    empty_def_handler,    // Hibernation module       59
    empty_def_handler,    // USB                60
    0,            // Reserved             61
    empty_def_handler,    // UDMA SW              62
    empty_def_handler,    // UDMA Error           63
  ADC1SS0Handler,    // ADC 1 Seq 0            64
  ADC1SS1Handler,    // ADC 1 Seq 1            65
    empty_def_handler,    // ADC 1 Seq 2            66
    empty_def_handler,    // ADC 1 Seq 3            67
    0,            // Reserved             68
    0,            // Reserved             69
    0,            // Reserved             70
    0,            // Reserved             71
    0,            // Reserved             72
    empty_def_handler,    // SSI 2              73
    empty_def_handler,    // SSI 2              74
    empty_def_handler,    // UART 3             75
    empty_def_handler,    // UART 4             76
    empty_def_handler,    // UART 5             77
    empty_def_handler,    // UART 6             78
    empty_def_handler,    // UART 7             79
    0,            // Reserved             80
    0,            // Reserved             81
    0,            // Reserved             82
    0,            // Reserved             83
    I2C2Handler,    // I2C 2              84
    I2C3Handler,    // I2C 3              85
    empty_def_handler,    // 16/32 bit timer 4 A        86
    empty_def_handler,    // 16/32 bit timer 4 B        87
    0,            // Reserved             88
    0,            // Reserved             89
    0,            // Reserved             90
    0,            // Reserved             91
    0,            // Reserved             92
    0,            // Reserved             93
    0,            // Reserved             94
    0,            // Reserved             95
    0,            // Reserved             96
    0,            // Reserved             97
    0,            // Reserved             98
    0,            // Reserved             99
    0,            // Reserved             100
    0,            // Reserved             101
    0,            // Reserved             102
    0,            // Reserved             103
    0,            // Reserved             104
    0,            // Reserved             105
    0,            // Reserved             106
    0,            // Reserved             107
    Timer5Handler,    // 16/32 bit timer 5 A        108
    empty_def_handler,    // 16/32 bit timer 5 B        109
    WTimer0AHandler,    // 32/64 bit timer 0 A        110
    WTimer0BHandler,    // 32/64 bit timer 0 B        111
    WTimer1AHandler,    // 32/64 bit timer 1 A        112
    WTimer1BHandler,    // 32/64 bit timer 1 B        113
    WTimer2AHandler,    // 32/64 bit timer 2 A        114
    WTimer2BHandler,    // 32/64 bit timer 2 B        115
    WTimer3AHandler,    // 32/64 bit timer 3 A        116
    WTimer3BHandler,    // 32/64 bit timer 3 B        117
    WTimer4AHandler,    // 32/64 bit timer 4 A        118
    WTimer4BHandler,    // 32/64 bit timer 4 B        119
    WTimer5AHandler,    // 32/64 bit timer 5 A        120
    WTimer5BHandler,    // 32/64 bit timer 5 B        121
    empty_def_handler,    // FPU         122
    0,            // PECI             123
    0,            // LPC             124
    I2C4Handler,            // I2C4             125
    I2C5Handler,            // I2C5             126
    0,            // Reserved             127
    0,            // Reserved             128
    0,            // Reserved             129
    0,            // Reserved             130
    0,            // Reserved             131
    0,            // Reserved             132
    0,            // Reserved             133
    0,            // Reserved             134
    0,            // Reserved             135
    0,            // Reserved             136
    0,            // Reserved             137
    0,            // Reserved             138
    0,            // Reserved             139
    0,            // Reserved             140
    0,            // Reserved             141
    0,            // Reserved             142
    0,            // Reserved             143
    0,            // Reserved             144
    0,            // Reserved             145
    0,            // Reserved             146
    0,            // Reserved             147
    0,            // Reserved             148
    0,            // Reserved             149
    0,            // Reserved             150
    0,            // Reserved             151
    0,            // Reserved             152
    0,            // Reserved             153
    0           // Reserved             154
};

//-----------------------------------------------------------------------------
//              Function implementations.
//-----------------------------------------------------------------------------

/* 
* System on reset code. NVIC 1
* Here I prepare the memory for the c compiler.
* The stack pointer should be set at the beginning with the NVIC table already.
* Copy the .data segment from flash into ram.
* 0 to the .bss segment 
*/

void rst_handler(void){ 
  // Copy the .data section pointers to ram from flash.
  // Look at LD manual (Optional Section Attributes).

  // source and destination pointers
  unsigned long *src;
  unsigned long *dest;

  //this should be good!
  src = &_etext;
  
  dest = &_sdata;

  //this too
    while(dest < &_edata)
    {
        *dest++ = *src++;
    }

    // now set the .bss segment to 0!
    dest = &_sbss;
  while(dest < &_ebss){
    *dest++ = 0;
  }

    //  
    // Enable the floating-point unit.  This must be done here to handle the
    // case where main() uses floating-point and the function prologue saves
    // floating-point registers (which will fault if floating-point is not
    // enabled).  Any configuration of the floating-point unit using DriverLib
    // APIs must be done here prior to the floating-point unit being enabled.
    //  
    // Note that this does not use DriverLib since it might not be included in
    // this project.
    //  
    HWREG(NVIC_CPAC) = ((HWREG(NVIC_CPAC) &
                         ~(NVIC_CPAC_CP10_M | NVIC_CPAC_CP11_M)) |
                        NVIC_CPAC_CP10_FULL | NVIC_CPAC_CP11_FULL);


  // after setting copying .data to ram and "zero-ing" .bss we are good
  // to start the main() method!
  // There you go!
  main();
}

// NMI Exception handler code NVIC 2
void nmi_handler(void){
  // Just loop forever, so if you want to debug the processor it's running.
    while(1){
    }
}

// Hard fault handler code NVIC 3
__attribute__((naked))
 void hardfault_handler( unsigned int * hardfault_args ){
  __asm(" .syntax unified\n"
    "movs r0, #4\n"
    "mov r1, lr\n"
    "tst r0, r1\n"
    "beq _MSP\n"
    "MRS r0, PSP\n"
    "B Hardfault_HandlerC\n"
  "_MSP:\n"
    "MRS r0, msp\n"
    "B Hardfault_HandlerC\n");
}

void Hardfault_HandlerC(unsigned long *hardfault_args){
  volatile unsigned long stacked_r0 = hardfault_args[0];
  volatile unsigned long stacked_r1 = hardfault_args[1];
  volatile unsigned long stacked_r2 = hardfault_args[2];
  volatile unsigned long stacked_r3 = hardfault_args[3];
  volatile unsigned long stacked_r12 = hardfault_args[4];
  volatile unsigned long stacked_lr = hardfault_args[5];
  volatile unsigned long stacked_pc = hardfault_args[6];
  volatile unsigned long stacked_psr = hardfault_args[7];
  volatile unsigned long _CFSR = *((volatile unsigned long*) 0xE000ED28);
  volatile unsigned long _HFSR = *((volatile unsigned long*) 0xE000ED2C);
  volatile unsigned long _DFSR = *((volatile unsigned long*) 0xE000ED30);
  volatile unsigned long _AFSR = *((volatile unsigned long*) 0xE000ED3C);
  volatile unsigned long _BFAR = *((volatile unsigned long*) 0xE000ED38);
  volatile unsigned long _MMAR = *((volatile unsigned long*) 0xE000ED34);
  __asm("BKPT #0\n");
}

void empty_def_handler(void){
    while(1){
    }
}
