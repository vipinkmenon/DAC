#include "dac.h"


static void regWrite(int addr,int data){
	int *regAddr = (int*)addr;
	*regAddr = data;
}

/********************************************************/
/*Function to initialize the DAC                        */
/*parameters : DAC instance,reference voltage           */
/*return     : SUCCESS on successful initialization     */
/*             INVALID_VREF_ERROR if specified vref is  */
/*             not in the range of max and min vref     */
/********************************************************/
int initDAC(dac* d,float vref){
	d->baseAddress = DAC_BASE_ADDR;
	if(vref < MIN_VREF || vref > MAX_VREF)
		return INVALID_VREF_ERROR;
	d->VREF = vref;
	PINSEL1 = (PINSEL1&DAC_PINSEL_MASK)|DAC_PINSEL_BITS;
	d->initialized = 1;
	return SUCCESS_STAT;
}


/********************************************************/
/*Function to convert data to DAC format and send to pin*/
/*parameters : DAC instance,data bias                   */
/*return     : SUCCESS on successful initialization     */
/*             DAC_NOT_INITIALIZED_ERROR if DAC was not */
/*             initialized before this API call         */
/********************************************************/
int convertDAC(dac* d,int data,int b){
	if(d->initialized != 1)
		return DAC_NOT_INITIALIZED_ERROR;
	regWrite(d->baseAddress+DACR_OFFSET,((int)(1023*data/(d->VREF))<<6)|b<<16);
	return SUCCESS_STAT;
}

