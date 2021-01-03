#include "vic.h"

#define findEffectiveAddress(base,offset) base+offset

vic myVIC;

static void regWrite(int addr,int mask,int data){
	int *regAddr = (int*)addr;
	*regAddr = (*regAddr&mask)|data;
}


static int regRead(int addr){
	int *regAddr = (int*)addr;
	return *regAddr;
}

/********************************************************/
/*Function to initialize the VIC                        */
/*parameters : VIC instance                             */
/*return     : SUCCESS on successful initialization     */
/********************************************************/
int initVIC(vic* v){
	v->baseAddress = VIC_BASE_ADDR;
	v->initialized = 1;
	regWrite(findEffectiveAddress(v->baseAddress,VICIntEnableOffset),AllSetMask,disableAllInterruptBits);//disbale all the interrupts
	regWrite(findEffectiveAddress(v->baseAddress,VICSoftIntOffset),AllSetMask,disableAllInterruptBits);//disable all software interrupts
	return SUCCESS_STAT;
}

/********************************************************/
/*Function to enable an interrupt                       */
/*parameters : VIC instance, interrupt channel number   */
/*return     : SUCCESS on successful enabling           */
/*             VIC_NOT_INIT_ERROR if VIC is not initiali*/
/*             zed before this API call                 */
/********************************************************/
int enableInterruptVIC(vic* v, int intSource){
	if(v->initialized != 1)
		return VIC_NOT_INIT_ERROR;
	regWrite(findEffectiveAddress(v->baseAddress,VICIntEnableOffset),AllSetMask,1<<intSource);
	return SUCCESS_STAT;
}

/********************************************************/
/*Function to disable an interrupt                      */
/*parameters : VIC instance, interrupt channel number   */
/*return     : SUCCESS on successful disabling          */
/*             VIC_NOT_INIT_ERROR if VIC is not initiali*/
/*             zed before this API call                 */
/********************************************************/
int disableInterruptVIC(vic* v, int intSource){
	if(v->initialized != 1)
		return VIC_NOT_INIT_ERROR;
	regWrite(findEffectiveAddress(v->baseAddress,VICIntEnableOffset),~intSource,AllClearMask);
	return SUCCESS_STAT;
}

/********************************************************/
/*Function to set priority of an interrupt              */
/*parameters : VIC instance, interrupt channel number   */
/*             priority level                           */
/*return     : SUCCESS on successful disabling          */
/*             VIC_NOT_INIT_ERROR if VIC is not initiali*/
/*             zed before this API call                 */
/*             INVALID_PRIORITY_ERROR if specified prio */
/*             rity is not with in the supported levels */
/********************************************************/
int setPriorityVIC(vic* v, int intSource, int priority){
	if(v->initialized != 1)
		return VIC_NOT_INIT_ERROR;
	if(priority < MIN_PRIORITY || priority > MAX_PRIORITY)
		return INVALID_PRIORITY_ERROR;
	regWrite(findEffectiveAddress(v->baseAddress,VICPriorityOffset+intSource*4),AllClearMask,priority);
	return SUCCESS_STAT;
}

/********************************************************/
/*Function to link an ISR with an interrupt             */
/*parameters : VIC instance, interrupt channel number   */
/*             starting address of ISR                  */
/*return     : SUCCESS on successful linking            */
/*             VIC_NOT_INIT_ERROR if VIC is not initiali*/
/*             zed before this API call                 */
/********************************************************/
int setISRVIC(vic* v, int intSource, unsigned long vector){
	if(v->initialized != 1)
		return VIC_NOT_INIT_ERROR;
	regWrite(findEffectiveAddress(v->baseAddress,VICVectAddrOffset+intSource*4),AllClearMask,vector);
	return SUCCESS_STAT;
}
