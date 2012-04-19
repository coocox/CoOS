#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/stats.h"
#include "osevent.h"
#include "LPC17xx_reg.h"

/* This is the number of threads that can be started with sys_thread_new() */
//static struct timeoutlist lwip_timeout[LWIP_TASK_MAX];
//static struct timeoutlist null_timeout;

static U8 queue_memory_pool[LWIP_MAX_QUEUE * LWIP_SIZE_QUEUE*4];
static OS_MMID  queue_pool; 

struct timeoutstruct {
	struct sys_timeouts timeouts;
	OS_TID tid;
};

static struct timeoutstruct timeout_list[LWIP_TASK_MAX];
static u8_t   current_task = 0;

/*-----------------------------------------------------------------------------------*/
//  Creates an empty mailbox.
sys_mbox_t sys_mbox_new(int size)
{
	OS_EventID mbox;
	void *qstart;

  // Allocate memory for mailbox
	if(queue_pool != E_CREATE_FAIL) {
		qstart = CoGetMemoryBuffer(queue_pool);    
		if(qstart != NULL) {
			mbox = CoCreateQueue((void **)qstart, LWIP_SIZE_QUEUE, EVENT_SORT_TYPE_PRIO);
			
      if(mbox == E_CREATE_FAIL) {
        // Mailbox created failed, release allocated memory
        CoFreeMemoryBuffer(queue_pool, qstart);
      }
            
			return mbox;			
		}
	}
	
	return SYS_MBOX_NULL;
}

/*-----------------------------------------------------------------------------------*/
/*
  Deallocates a mailbox. If there are messages still present in the
  mailbox when the mailbox is deallocated, it is an indication of a
  programming error in lwIP and the developer should be notified.
*/
void sys_mbox_free(sys_mbox_t mbox)
{
	void *mbuf;

	if(mbox == SYS_MBOX_NULL) return;

	mbuf = (void *)((P_QCB)EventTbl[mbox].eventPtr)->qStart;	// Get queue point
	CoDelQueue(mbox, OPT_DEL_ANYWAY);           // Deleted the mailbox
	CoFreeMemoryBuffer(queue_pool, mbuf);       // Release memory
}

/*-----------------------------------------------------------------------------------*/
//   Posts the "msg" to the mailbox.
void sys_mbox_post(sys_mbox_t mbox, void *data)
{
	CoPostQueueMail(mbox, data);
}


/*-----------------------------------------------------------------------------------*/
//   Try to post the "msg" to the mailbox.
err_t sys_mbox_trypost(sys_mbox_t mbox, void *msg)
{
	err_t result;

	if(CoPostQueueMail(mbox, msg) != E_OK) {
		
		result = ERR_MEM;		

	#if SYS_STATS
      	lwip_stats.sys.mbox.err++;
	#endif /* SYS_STATS */
	
	} else {
		result = ERR_OK;
	}

	return result;
}

/*-----------------------------------------------------------------------------------*/
/*
  Blocks the thread until a message arrives in the mailbox, but does
  not block the thread longer than "timeout" milliseconds (similar to
  the sys_arch_sem_wait() function). The "msg" argument is a result
  parameter that is set by the function (i.e., by doing "*msg =
  ptr"). The "msg" parameter maybe NULL to indicate that the message
  should be dropped.

  The return values are the same as for the sys_arch_sem_wait() function:
  Number of milliseconds spent waiting or SYS_ARCH_TIMEOUT if there was a
  timeout.

  Note that a function with a similar name, sys_mbox_fetch(), is
  implemented by lwIP.
*/
u32_t sys_arch_mbox_fetch(sys_mbox_t mbox, void **msg, u32_t timeout)
{
	void *data;
	StatusType result;
	u32_t timeoutTick;
	u32_t startTick, endTick, ElapseTime;

	//Covert  microsecond to os ticks
	timeoutTick = (timeout * CFG_SYSTICK_FREQ) / 1000;
	if(timeoutTick==0 && timeout!=0) timeoutTick = 1;
	
	startTick = CoGetOSTime();
	data = CoPendQueueMail(mbox, timeoutTick, &result);			
	if(timeoutTick != 0 && result != E_OK) {				
		if(msg) *msg = NULL;
		return SYS_ARCH_TIMEOUT;						
	} 

	endTick = CoGetOSTime();	
	if(msg) *msg = data;
	ElapseTime = ((endTick - startTick) * 1000)/CFG_SYSTICK_FREQ;
	
	return ElapseTime;		
}

/*-----------------------------------------------------------------------------------*/
/*
  Similar to sys_arch_mbox_fetch, but if message is not ready immediately, we'll
  return with SYS_MBOX_EMPTY.  On success, 0 is returned.
*/
u32_t sys_arch_mbox_tryfetch(sys_mbox_t mbox, void **msg)
{
	void *data;
	StatusType result;
	data = CoAcceptQueueMail(mbox, &result);
	if(result == E_OK) {
		if(msg) *msg = data;
		return ERR_OK; 		
	}

	if(msg) *msg = NULL;
	return SYS_MBOX_EMPTY; 
}

/*-----------------------------------------------------------------------------------*/
//  Creates and returns a new semaphore. The "count" argument specifies
//  the initial state of the semaphore.
sys_sem_t sys_sem_new(u8_t count)
{
	OS_EventID sem;  

	sem = CoCreateSem(count, OS_SEM_MAX_STATE, EVENT_SORT_TYPE_PRIO);

	return sem;
}

/*-----------------------------------------------------------------------------------*/
/*
  Blocks the thread while waiting for the semaphore to be
  signaled. If the "timeout" argument is non-zero, the thread should
  only be blocked for the specified time (measured in
  milliseconds).

  If the timeout argument is non-zero, the return value is the number of
  milliseconds spent waiting for the semaphore to be signaled. If the
  semaphore wasn't signaled within the specified time, the return value is
  SYS_ARCH_TIMEOUT. If the thread didn't have to wait for the semaphore
  (i.e., it was already signaled), the function may return zero.

  Notice that lwIP implements a function with a similar name,
  sys_sem_wait(), that uses the sys_arch_sem_wait() function.
*/
u32_t sys_arch_sem_wait(sys_sem_t sem, u32_t timeout)
{
	StatusType result;

	u32_t timeoutTick;
	u32_t startTick, endTick, elapseTime;
	
  //in lwip ,timeout is  millisecond 
  //in Coos ,timeout is timer  tick! 
  //chang timeout from millisecond to Coos tick
  timeoutTick = (timeout * CFG_SYSTICK_FREQ) / 1000;
	if(timeoutTick==0 && timeout != 0) {
		timeoutTick = 1;
	}	

  if(timeoutTick > 0xFFFF) {
    timeoutTick = 0xFFFF;
  }

	startTick = CoGetOSTime();
	result = CoPendSem(sem, timeoutTick);

	if(timeoutTick != 0 && result != E_OK) {
		return SYS_ARCH_TIMEOUT;
	}
	endTick = CoGetOSTime();

  //chang timeout from Coos tick to millisecond
	elapseTime = ((endTick - startTick) *1000) / CFG_SYSTICK_FREQ;
	return elapseTime;		
}

/*-----------------------------------------------------------------------------------*/
// Signals a semaphore
void sys_sem_signal(sys_sem_t sem)
{
	CoPostSem(sem);
}

/*-----------------------------------------------------------------------------------*/
// Deallocates a semaphore
void sys_sem_free(sys_sem_t sem)
{
	CoDelSem(sem, OPT_DEL_ANYWAY);
}

/*-----------------------------------------------------------------------------------*/
// Initialize sys arch
// This func is called first in lwip task!
void sys_init(void)
{
	U8 i;

  current_task = 0;

	for(i=0; i<LWIP_TASK_MAX; i++) {
		timeout_list[i].tid = E_CREATE_FAIL;	
	}

	// Init memory pool for sys_mbox_t
  queue_pool = CoCreateMemPartition(queue_memory_pool, LWIP_SIZE_QUEUE*4, LWIP_MAX_QUEUE);		
  if(E_CREATE_FAIL == queue_pool) {
    LWIP_DEBUGF(NETIF_DEBUG,("Memory pool for QUEUE created failed!\n\r"));
  }
}

/*-----------------------------------------------------------------------------------*/
/*
  Returns a pointer to the per-thread sys_timeouts structure. In lwIP,
  each thread has a list of timeouts which is represented as a linked
  list of sys_timeout structures. The sys_timeouts structure holds a
  pointer to a linked list of timeouts. This function is called by
  the lwIP timeout scheduler and must not return a NULL value.

  In a single threaded sys_arch implementation, this function will
  simply return a pointer to a global sys_timeouts variable stored in
  the sys_arch module.
*/
struct sys_timeouts *sys_arch_timeouts(void)
{
	U8 i;
	OS_TID tsk;

	tsk = CoGetCurTaskID();

  for(i=0; i<LWIP_TASK_MAX; i++) {
		if(timeout_list[i].tid == tsk) {
			return &timeout_list[i].timeouts;			
		}		
	}

  // Task created by CoCreateTask
  if(current_task < LWIP_TASK_MAX) {
    timeout_list[current_task].tid = tsk;   
    current_task++;
    return &timeout_list[current_task-1].timeouts;
  }

	return 	(struct sys_timeouts *)0;
}

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
// TODO
/*-----------------------------------------------------------------------------------*/
/*
  Starts a new thread with priority "prio" that will begin its execution in the
  function "thread()". The "arg" argument will be passed as an argument to the
  thread() function. The id of the new thread is returned. Both the id and
  the priority are system dependent.
*/
sys_thread_t sys_thread_new(char *name, void (* thread)(void *arg), void *arg, int stacksize, int prio)
{
	OS_TID tsk;

  if(current_task < LWIP_TASK_MAX) {
    tsk = CoCreateTask(thread, NULL, prio, arg, stacksize);			
    if(tsk != E_CREATE_FAIL) {
      timeout_list[current_task].tid = tsk;   
      current_task++;        
      return tsk;
    }
  } 
  
  LWIP_DEBUGF(NETIF_DEBUG,("Lwip task created failed, maybe out of range !\n\r"));
  return E_CREATE_FAIL;  
}

/*
  This optional function does a "fast" critical region protection and returns
  the previous protection level. This function is only called during very short
  critical regions. An embedded system which supports ISR-based drivers might
  want to implement this function by disabling interrupts. Task-based systems
  might want to implement this by using a mutex or disabling tasking. This
  function should support recursive calls from the same task or interrupt. In
  other words, sys_arch_protect() could be called while already protected. In
  that case the return value indicates that it is already protected.

  sys_arch_protect() is only required if your port is supporting an operating
  system.
*/
sys_prot_t sys_arch_protect(void)
{

	sys_prot_t result = 0;

	//__asm volatile ("MRS %0, primask" : "=r" (result) );
	//__asm volatile ("cpsid i");

	return result;
}

/*
  This optional function does a "fast" set of critical region protection to the
  value specified by pval. See the documentation for sys_arch_protect() for
  more information. This function is only required if your port is supporting
  an operating system.
*/
void sys_arch_unprotect(sys_prot_t pval)
{
	//__asm volatile ("MSR primask, %0" : : "r" (pval) );
}


/*
 * Prints an assertion messages and aborts execution.
 */
void sys_assert( const char *msg )
{	
	( void ) msg;

    for(;;)
    ;
}

void DebugPrintf(const char *pcString, ...)
{

}

