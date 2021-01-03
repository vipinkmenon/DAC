#ifndef TIMER_H  //To avoid circular reference
#define TIMER_H

//Register Offset Addresses
#define TCR_OFFSET  0x00000004
#define MCR_OFFSET  0x00000014
#define MR0_OFFSET  0x00000018
#define MR1_OFFSET  0x0000001C

//Mask Patterns
#define START_STOP_MASK 0xFFFFFFFE
#define RESET_MASK      0xFFFFFFFD
#define ALL_CLEAR_MASK  0x00000000
#define ALL_SET_MASK    0x11111111

//Register Configuration BIT patterns
#define START_TIMER_SET 0x00000001
#define STOP_TIMER_SET  0x00000000
#define RESET_TIMER_SET 0x00000002

//Error Codes
#define SUCCESS_STAT       0
#define INVALID_TIMER_NUM -1
#define TIMER_NOT_INIT_ERROR    -2
#define INVALID_MOD_ERROR -3

//Enumerated Data types
enum timerInterfaces{CAP0,CAP1,MAT0,MAT1,MAT2,MAT3,EMAT0,EMAT1};
enum interruptType{disableInterrupt,enableInterrupt};
enum resetType{disableReset,enableReset};
enum stopType{disableStop,enableStop};


//Timer Structure
typedef struct{
	int baseAddress;//to store base address
	int initialized;
	void (*callback)(void);
}timer;

//Timer APIs
int initTimer(timer* t,int n);
int timerStart(timer* t);
int timerStop(timer* t);
int timerReset(timer *t);
int configureTimer(timer *,int ,int ,int ,int ,int);//timer,mode,match value,interrupt,reset,stop
int setCallbackTimer(timer *,void(*callback)(void));

__irq void Timer_MAT0_ISR(void);
#endif
