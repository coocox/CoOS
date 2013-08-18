/**
 * @file sys_arch.c
 * @brief lwIP architecture and OS dependent layer.
 *
 */

#include <AppConfig.h>

#include "lwip/debug.h"
#include "lwip/def.h"
#include "lwip/sys.h"
#include "lwip/mem.h"
#include "lwip/stats.h"
#include "osevent.h"


static U8 queue_memory_pool[LWIP_MAX_QUEUE * LWIP_SIZE_QUEUE * 4];
static OS_MMID  queue_pool; 

static u8_t current_task = 0;   // number of threads that started with sys_thread_new()

/*-----------------------------------------------------------------------------------*/
//  Creates an empty mailbox.
err_t sys_mbox_new(sys_mbox_t *mbox, int size) {
    void *qstart;
    
    // Allocate memory for mailbox
    if (E_CREATE_FAIL != queue_pool) {
        qstart = CoGetMemoryBuffer(queue_pool);    
        if (NULL != qstart) {
            *mbox = CoCreateQueue((void **)qstart, LWIP_SIZE_QUEUE, EVENT_SORT_TYPE_PRIO);
            if (E_CREATE_FAIL == *mbox) {
                // Mailbox created failed, release allocated memory
                CoFreeMemoryBuffer(queue_pool, qstart);
            }
            
            return ERR_OK;
        }
    }
    
    return ERR_MEM;
}

/*-----------------------------------------------------------------------------------*/
/*
  Deallocates a mailbox. If there are messages still present in the
  mailbox when the mailbox is deallocated, it is an indication of a
  programming error in lwIP and the developer should be notified.
*/
void sys_mbox_free(sys_mbox_t *mbox) {
    void *mbuf;
    
    if (SYS_MBOX_NULL != *mbox) {    
        mbuf = (void *)((P_QCB)EventTbl[*mbox].eventPtr)->qStart;       // Get queue point
        CoDelQueue(*mbox, OPT_DEL_ANYWAY);      // Deleted the mailbox
        CoFreeMemoryBuffer(queue_pool, mbuf);   // Release memory
    }
}

void sys_mbox_post(sys_mbox_t *mbox, void *msg) {
    CoPostQueueMail(*mbox, msg);
}

err_t sys_mbox_trypost(sys_mbox_t *mbox, void *msg) {
    err_t res;
    
    if (E_OK != CoPostQueueMail(*mbox, msg)) {
        res = ERR_MEM;
#if SYS_STATS
      	lwip_stats.sys.mbox.err++;
#endif /* SYS_STATS */
    }
    else {
        res = ERR_OK;
    }
    
    return res;
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
u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, void **msg, u32_t timeout) {
    void *data;
    StatusType err;
    u32_t timeoutTick;
    u32_t startTick, endTick, ElapseTime;
    
    // Covert  milliseconds to os ticks
    timeoutTick = (timeout * CFG_SYSTICK_FREQ) / 1000;
    if (timeoutTick == 0 && timeout != 0)
        timeoutTick = 1;
    
    startTick = CoGetOSTime();
    data = CoPendQueueMail(*mbox, timeoutTick, &err);
    if (timeoutTick != 0 && E_OK != err) {
        if (msg)
            *msg = NULL;
        
        return SYS_ARCH_TIMEOUT;						
    } 
    
    endTick = CoGetOSTime();	
    if (msg)
        *msg = data;
    
    ElapseTime = ((endTick - startTick) * 1000) / CFG_SYSTICK_FREQ;
    
    return ElapseTime;		
}

u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, void **msg) {
    void *data;
    StatusType err;
    
    data = CoAcceptQueueMail(*mbox, &err);
    if (E_OK == err) {
        if (msg)
            *msg = data;
        return ERR_OK; 		
    }
    
    if (msg)
        *msg = NULL;
    
    return SYS_MBOX_EMPTY; 
}

int sys_mbox_valid(sys_mbox_t *mbox) {
    return EVENT_TYPE_QUEUE == EventTbl[*mbox].eventType;
}

void sys_mbox_set_invalid(sys_mbox_t *mbox) {
}

err_t sys_sem_new(sys_sem_t *sem, u8_t count) {
    *sem = CoCreateSem(count, OS_SEM_MAX_STATE, EVENT_SORT_TYPE_PRIO);
    return (E_CREATE_FAIL != *sem) ? ERR_OK : -1;
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
u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout) {
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
    result = CoPendSem(*sem, timeoutTick);
    
    if(timeoutTick != 0 && result != E_OK) {
        return SYS_ARCH_TIMEOUT;
    }
    endTick = CoGetOSTime();
    
    //chang timeout from Coos tick to millisecond
    elapseTime = ((endTick - startTick) *1000) / CFG_SYSTICK_FREQ;
    return elapseTime;		
}

void sys_sem_signal(sys_sem_t *sem) {
    CoPostSem(*sem);
}

void sys_sem_free(sys_sem_t *sem) {
    CoDelSem(*sem, OPT_DEL_ANYWAY);
}

int sys_sem_valid(sys_sem_t *sem) {
    return EVENT_TYPE_SEM == EventTbl[*sem].eventType;
}

void sys_sem_set_invalid(sys_sem_t *sem) {
}

/** Initialize sys arch.
 * This func is called first in lwip task.
 */
void sys_init(void) {
    current_task = 0;
    
    // Init memory pool for sys_mbox_t
    queue_pool = CoCreateMemPartition(queue_memory_pool, LWIP_SIZE_QUEUE * 4, LWIP_MAX_QUEUE);		
    if (E_CREATE_FAIL == queue_pool) {
        LWIP_DEBUGF(NETIF_DEBUG,("Memory pool for QUEUE created failed!\n\r"));
    }
}

sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread, void *arg, int stacksize, int prio) {
    OS_TID tsk;
    OS_STK *stack;
    
    if (LWIP_TASK_MAX > current_task) {
        stack = (OS_STK*)CoKmalloc(stacksize);
        if (stack) {
            tsk = CoCreateTask(thread, arg, prio, &stack[(stacksize / sizeof(OS_STK)) - 1], (stacksize / sizeof(OS_STK)));
            if (E_CREATE_FAIL != tsk) {
                current_task++;
                return tsk;
            }
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
sys_prot_t sys_arch_protect(void) {
    sys_prot_t res = 0;
    
    res = __get_PRIMASK();
    __disable_irq();
    
    return res;
}

/*
  This optional function does a "fast" set of critical region protection to the
  value specified by pval. See the documentation for sys_arch_protect() for
  more information. This function is only required if your port is supporting
  an operating system.
*/
void sys_arch_unprotect(sys_prot_t pval) {
    __set_PRIMASK(pval & 1);
}
