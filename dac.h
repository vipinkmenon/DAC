#ifndef DAC_H  //To avoid circular reference
#define DAC_H


#include <LPC23xx.H>

#define MIN_VREF 0
#define MAX_VREF 3.3

//Register Offset Addresses
#define DACR_OFFSET 0x0

//Masks
#define DAC_PINSEL_MASK 0xDFFFFF


#define DAC_PINSEL_BITS 0x200000

//Error codes
#define SUCCESS_STAT        0
#define DAC_NOT_INITIALIZED_ERROR -1
#define INVALID_VREF_ERROR -2

//DAC Structure
typedef struct{
	int baseAddress;
	int initialized;
	float VREF;
}dac;

//DAC APIs
int initDAC(dac* ,float );
int convertDAC(dac*,int,int);//dac pointer, data, bias


#endif
