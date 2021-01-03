#include "timer.h"
#include "vic.h"
#include <LPC23xx.H>

#define findEffectiveAddress(base,offset) base+offset

timer timer0;

static void regWrite(int addr,int mask,int data){
	int *regAddr = (int*)addr;
	*regAddr = (*regAddr&mask)|data;
}


static int regRead(int addr){
	int *regAddr = (int*)addr;
	return *regAddr;
}

/********************************************************/
/*Function to initialize the timer                      */
/*parameters : timer instance, timer number             */
/*return     : SUCCESS on successful initialization     */
/*             FAIL if the timer number is invalid      */
/********************************************************/

int initTimer(timer* t,int n){
	switch(n){
		case 0:
			t->baseAddress = TMR0_BASE_ADDR;
			t->initialized = 1;
			break;
		case 1:
			t->baseAddress = TMR1_BASE_ADDR;
			t->initialized = 1;
			break;
		case 2:
			t->baseAddress = TMR2_BASE_ADDR;
			t->initialized = 1;
			break;
		case 3:
			t->baseAddress = TMR0_BASE_ADDR;
			t->initialized = 1;
			break;
		default:
			return INVALID_TIMER_NUM;
	}
	return SUCCESS_STAT;
}

/********************************************************/
/*Function to start the timer. If in reset, brings out  */
/*of reset                                              */
/*parameters : timer instance                           */
/*return     : SUCCESS on successful start              */
/*             TIMER_NOT_INIT_ERROR if the timer is     */
/*             not initialized before this function call*/
/********************************************************/

int timerStart(timer* t){
	if(t->initialized != 1)
		return TIMER_NOT_INIT_ERROR;
	regWrite(findEffectiveAddress(t->baseAddress,TCR_OFFSET),START_STOP_MASK&RESET_MASK,START_TIMER_SET);
	return SUCCESS_STAT;
}

/********************************************************/
/*Function to stop the timer.                           */
/*parameters : timer instance                           */
/*return     : SUCCESS on successful start              */
/*             TIMER_NOT_INIT_ERROR if the timer  is    */
/*             not initialized before this function call*/
/********************************************************/

int timerStop(timer* t){
	if(t->initialized != 1)
		return TIMER_NOT_INIT_ERROR;
	regWrite(findEffectiveAddress(t->baseAddress,TCR_OFFSET),START_STOP_MASK,STOP_TIMER_SET);
	return SUCCESS_STAT;
}

/********************************************************/
/*Function to reset the timer.                          */
/*parameters : timer instance                           */
/*return     : SUCCESS on successful reset              */
/*             TIMER_NOT_INIT_ERROR if the timer is     */
/*             not initialized before this function call*/
/********************************************************/
int timerReset(timer *t){
	if(t->initialized != 1)
		return TIMER_NOT_INIT_ERROR;
	regWrite(findEffectiveAddress(t->baseAddress,TCR_OFFSET),RESET_MASK,START_TIMER_SET|RESET_TIMER_SET);
	return SUCCESS_STAT;
}

/********************************************************/
/*Function to configure the timer in different modes    */
/*parameters : timer instance                           */
/*             mode: only Match0 or Match1 implemented  */
/*return     : SUCCESS on successful reset              */
/*             TIMER_NOT_INIT_ERROR if the timer is     */
/*             not initialized before this function call*/
/*             INVALID_MOD_ERROR if specified mode in   */
/*             invalid                                  */
/********************************************************/
int configureTimer(timer *t,int mode,int matchValue,int interrupt,int reset,int stop){
	if(t->initialized != 1)
		return TIMER_NOT_INIT_ERROR;
	switch(mode){
		case MAT0:
			regWrite(findEffectiveAddress(t->baseAddress,MR0_OFFSET),ALL_CLEAR_MASK,matchValue);
		  regWrite(findEffectiveAddress(t->baseAddress,MCR_OFFSET),ALL_CLEAR_MASK,interrupt|reset<<1|stop<<2);
			break;
		case MAT1:
			regWrite(findEffectiveAddress(t->baseAddress,MR1_OFFSET),ALL_CLEAR_MASK,matchValue);
		  regWrite(findEffectiveAddress(t->baseAddress,MCR_OFFSET),ALL_CLEAR_MASK,interrupt<<3|reset<<4|stop<<5);
			break;	
		default:
			return INVALID_MOD_ERROR;
	}
	return SUCCESS_STAT;
}

int setCallbackTimer(timer *t,void(*callback)(void)){
	t->callback = callback;
	return 0;
}


//Timer IRQ
__irq void Timer_MAT0_ISR(void) {
		extern vic myVIC;
		disableInterruptVIC(&myVIC,Timer0);
    timer0.callback();
	  T0IR = 0x1;
	  enableInterruptVIC(&myVIC,Timer0);
	  VICVectAddr = 0;
}
