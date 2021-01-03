#include <LPC23xx.H>
#include <math.h>
#include "timer.h"
#include "vic.h"
#include "dac.h"

#define VREF 3.3
#define VMAX 1.65
#define SINE_PERIOD 1
#define SAMPLING_PERIOD 0.001
#define TIMER_CLOCK_PER 0.000001
#define TIMER_MATCH_VAL SAMPLING_PERIOD/TIMER_CLOCK_PER

#ifndef M_PI
#define M_PI 3.1415926535897932
#endif

extern vic myVIC;
extern timer timer0;
dac dac0;


void callBack(void);

float Vout;

int main(){
	int status;
  //DAC initialization
	status = initDAC(&dac0,VREF);
	if(status != 0)
		return -1;
	//Timer Initialization
	status = initTimer(&timer0,0);
	if(status != 0)
		return -1;
	//Timer Configuration
	status = configureTimer(&timer0,MAT0,TIMER_MATCH_VAL,enableInterrupt,enableReset,disableStop);
	if(status != 0)
		return -1;
	//VIC Initialization
	status = initVIC(&myVIC);
	if(status != 0)
		return -1;
	//VIC Configuration
	status = enableInterruptVIC(&myVIC,Timer0);
	if(status != 0)
		return -1;
	status = setPriorityVIC(&myVIC,Timer0,0x1);
	if(status != 0)
		return -1;
	status = setISRVIC(&myVIC,Timer0,(unsigned long)Timer_MAT0_ISR);
	if(status != 0)
		return -1;
	
	setCallbackTimer(&timer0,callBack);
	
	
	//Start the timer
	timerReset(&timer0);
	timerStart(&timer0);
	//Stay in this loop indefinitely
	while(1){
	}
}


//Timer IRQ
void callBack(void) {
        static float t=0;
		Vout = VMAX+VMAX*sin(2*M_PI*t/SINE_PERIOD);
		convertDAC(&dac0,Vout,0);
		t += SAMPLING_PERIOD;
}
