/**
 *******************************************************************************
 * @file       hook.c
 * @version   V1.1.4    
 * @date      2011.04.20
 * @brief      hook management implementation code of CooCox CoOS kernel.	
 *******************************************************************************
 * @copy
 *
 * INTERNAL FILE,DON'T PUBLIC.
 * 
 * <h2><center>&copy; COPYRIGHT 2009 CooCox </center></h2>
 *******************************************************************************
 */ 


/*---------------------------- Inlcude --------------------------------------*/
#include <coocox.h>

/**
 *******************************************************************************
 * @brief      IDLE task of OS	 
 * @param[in]  pdata	The parameter passed to IDLE task.		 
 * @param[out] None 
 * @retval     None	 
 *
 * @par Description
 * @details    This function is system IDLE task code.	 
 *******************************************************************************
 */
void CoIdleTask(void* pdata)
{
    /* Add your codes here */
    for(; ;) 
    {
#if CFG_STAT_TASK > 0
     	CoSchedLock();
    	CoIdleCtrl++;
        CoSchedUnlock();
#endif

        /* Add your codes here */
    }
}


/**
 *******************************************************************************
 * @brief      Hook for stack overflow	 
 * @param[in]  taskID	Piont to the task which lead to stack overflow.		 
 * @param[out] None 
 * @retval     None	 
 *
 * @par Description
 * @details    This function is a hook for stack overflow.	 
 *******************************************************************************
 */
void CoStkOverflowHook(OS_TID taskID)
{
    /* Process stack overflow  here */
    for(; ;) 
    {
      
    }
}



#if CFG_STAT_TASK > 0

/*
 * Start Statistic Task.
 * Wait almost a second (or more) to start all the other task.
 */
void  CoStatInit (void)
{

    CoTimeDelay(0,0,0,100);                    	  // Mi sincronizzo con il timer e con la statistic task

	CoSchedLock();
    CoIdleCtrl    = 0L;                           /* Clear idle counter                                 */
    CoSchedUnlock();
    CoTimeDelay(0,0,1,0);                         /* Determine MAX. idle counter value for 1 second     */
    CoSchedLock();
    CoIdleCtrMax = CoIdleCtrl;                    /* Store maximum idle counter count in 1 second       */
    CoStatRdy    = 1;
    CoSchedUnlock();


}


/*
 * Statistic task
 */
void CoStatTask(void* pdata)
{
    U32	    max,run;
    S8      usage;


	while (CoStatRdy == 0)
	{
	    CoTimeDelay(0,0,3,0);			// Wait until statistic task is ready
	}


	max = CoIdleCtrMax / 100L;

	for(; ;)
    {
    	CoSchedLock();
        CoIdleCtrRun   = CoIdleCtrl;        /* Obtain the of the idle counter for the past second */
        CoIdleCtrl    = 0L;                 /* Reset the idle counter for the next second         */
        CoSchedUnlock();
        run = CoIdleCtrRun;
        if (max > 0L)
        {
              usage = (S8)(100L - run / max);
              if (usage >= 0)
              {
            	  CoCPUUsage = usage;
            	  if(CoCPUUsage > CoCPUUsagePeak)
            	  {
            		  CoCPUUsagePeak = CoCPUUsage;
             	  }
              }
              else CoCPUUsage = 0;
         }
        else CoCPUUsage = 0;
	    CoTimeDelay(0,0,1,0);
    }
}
#endif

#if CFG_STAT_TASK == 0
void  CoStatInit (void)
{


}

#endif




