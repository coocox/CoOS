/**
 *******************************************************************************
 * @file       serviceReq.c
 * @version    V1.13    
 * @date       2010.04.26
 * @brief      servive request management implementation code of CooCox CoOS kernel.	
 *******************************************************************************
 * @copy
 *
 * INTERNAL FILE,DON'T PUBLIC.
 * 
 * <h2><center>&copy; COPYRIGHT 2009 CooCox </center></h2>
 *******************************************************************************
 */ 



/*---------------------------- Include ---------------------------------------*/
#include <coocox.h>

#if (CFG_TASK_WAITTING_EN > 0) || (CFG_TMR_EN >0)

#if CFG_MAX_SERVICE_REQUEST > 0
/*---------------------------- Variable Define -------------------------------*/
SRQ   ServiceReq = {0,0};             /*!< ISR server request queue         */		     
#endif       
BOOL  IsrReq   = FALSE;
#if (CFG_TASK_WAITTING_EN > 0)
BOOL  TimeReq  = FALSE;                 /*!< Time delay dispose request       */
#endif

#if CFG_TMR_EN  > 0
BOOL  TimerReq = FALSE;                 /*!< Timer dispose request            */
#endif

/**
 *******************************************************************************
 * @brief      Insert into service requst queue	 
 * @param[in]  type     Service request type.	
 * @param[in]  id       Service request event id,event id/flag id.
 * @param[in]  arg      Service request argument. 
 * @param[out] None 
 * 	 
 * @retval     FALSE    Successfully insert into service request queue. 
 * @retval     TRUE     Failure to insert into service request queue.  
 *
 * @par Description		 
 * @details    This function be called to insert a requst into service request	
 *             queue.
 * @note 
 *******************************************************************************
 */
#if (CFG_MAX_SERVICE_REQUEST > 0)
BOOL InsertInSRQ(U8 type,U8 id,void* arg)
{
    P_SQC   pcell;
	U8 cnt;
	U8 heed;
    IRQ_DISABLE_SAVE();
    if (ServiceReq.cnt >= CFG_MAX_SERVICE_REQUEST)
    {
        IRQ_ENABLE_RESTORE ();

        return FALSE;                   /* Error return                       */
    }
	cnt = Inc8(&ServiceReq.cnt);
	heed = ServiceReq.head;
    IsrReq = TRUE;
    pcell = &ServiceReq.cell[((cnt+heed)%CFG_MAX_SERVICE_REQUEST)];/*the tail */
    pcell->type = type;                 /* Save service request type,         */
    pcell->id   = id;                   /* event id                           */
    pcell->arg  = arg;                  /* and parameter                      */
    IRQ_ENABLE_RESTORE ();

    return TRUE;                        /* Return OK                          */
}
#endif



/**
 *******************************************************************************
 * @brief      Respond the request in the service request queue.	 
 * @param[in]  None
 * @param[out] None 
 * @retval     None  
 *
 * @par Description		 
 * @details    This function be called to respond the request in the service  
 *             request queue.
 * @note 
 *******************************************************************************
 */
void RespondSRQ(void)
{

#if CFG_MAX_SERVICE_REQUEST > 0
    SQC cell;

#endif

#if (CFG_TASK_WAITTING_EN > 0)
    if(TimeReq == TRUE)                 /* Time delay request?                */
    {
        TimeDispose();                  /* Yes,call handler                   */
        TimeReq = FALSE;                /* Reset time delay request false     */
    }
#endif
#if CFG_TMR_EN  > 0
    if(TimerReq == TRUE)                /* Timer request?                     */
    {
        TmrDispose();                   /* Yes,call handler                   */
        TimerReq = FALSE;               /* Reset timer request false          */
    }
#endif

#if CFG_MAX_SERVICE_REQUEST > 0

    while (ServiceReq.cnt != 0)
    {
        IRQ_DISABLE_SAVE ();            /* need to protect the following      */
        cell = ServiceReq.cell[ServiceReq.head];  /* extract one cell         */
        ServiceReq.head = (ServiceReq.head + 1) % /* move head (pop)          */
                     CFG_MAX_SERVICE_REQUEST;
        ServiceReq.cnt--;
        IRQ_ENABLE_RESTORE ();          /* now use the cell copy              */

        switch(cell.type)               /* Judge service request type         */
        {
#if CFG_SEM_EN > 0
        case SEM_REQ:                   /* Semaphore post request,call handler*/
            CoPostSem(cell.id);
            break;
#endif
#if CFG_MAILBOX_EN > 0
        case MBOX_REQ:                  /* Mailbox post request,call handler  */
            CoPostMail(cell.id, cell.arg);
            break;
#endif
#if CFG_FLAG_EN > 0
        case FLAG_REQ:                  /* Flag set request,call handler      */
            CoSetFlag(cell.id);
            break;
#endif
#if CFG_QUEUE_EN > 0
        case QUEUE_REQ:                 /* Queue post request,call handler    */
            CoPostQueueMail(cell.id, cell.arg);
            break;
#endif
        default:                        /* Others,break                       */
            break;
        }
    }
#endif
    IRQ_DISABLE_SAVE ();                /* need to protect the following      */

    if (ServiceReq.cnt == 0)            /* another item in the queue already? */
    {
        IsrReq = FALSE;                 /* queue still empty here             */
    }
    IRQ_ENABLE_RESTORE ();              /* now it is done and return          */
}

#endif

