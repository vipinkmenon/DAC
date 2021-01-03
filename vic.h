#ifndef VIC_H  //To avoid circular reference
#define VIC_H


#include <LPC23xx.H>

#define MIN_PRIORITY 0
#define MAX_PRIORITY 15

//Register Offset Addresses
#define VICIntEnableOffset 0x010
#define VICSoftIntOffset   0x018
#define VICPriorityOffset  0x200
#define VICVectAddrOffset  0x100

//Mask Patterns
#define AllSetMask 0xFFFFFFFF
#define AllClearMask 0x00000000

//Register Configuration BIT patterns
#define disableAllInterruptBits  0x00000000
#define enableAllInterruptBits   0xFFFFFFFF

//Error Codes
#define SUCCESS_STAT            0
#define VIC_NOT_INIT_ERROR     -1
#define INVALID_PRIORITY_ERROR -2

//Enumerates Data types for interrupt sources
enum interruptSouce{WDT,RS,ARMC0,ARMC1,Timer0,Timer1,UART0,UART1,PWM,I2C0,SSP0,SSP1,PLL,RTC,EINT0,\
                   EINT1,EINT2,EINT3,AD,I2C1,BOD,Ether,USB,CAN,SD,GPDMA,TIMER2,TIMER3,UART2,UART3,I2C2,I2S};

//VIC Structure
typedef struct{
	int baseAddress;
	int initialized;
}vic;

//APIs
int initVIC(vic*);
int enableInterruptVIC(vic*, int);
int disableInterruptVIC(vic* v, int intSource);
int setPriorityVIC(vic*, int, int);
int setISRVIC(vic*, int, unsigned long);


#endif
