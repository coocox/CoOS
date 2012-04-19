/**
 *******************************************************************************
 * @file        mutexTest.c
 * @version     V1.00    Initial version
 * @date        2009.06.26
 * @brief       This file provides all test functions for mutex.
 *******************************************************************************
 * @copy
 *
 * INTERNAL TEST FILE,DON'T PUBLIC.
 * 
 * <h2><center>&copy; COPYRIGHT 2009 CooCox </center></h2>
 *******************************************************************************
 */ 

/*---------------------------- Inlcude ---------------------------------------*/
#include <stdio.h>
#include <config.h>
#include <CoOS.h>
#include "test.h"

#if (CFG_MUTEX_EN > 0 && MUTEX_TEST_EN > 0)

/*---------------------------- Function declare ------------------------------*/
static char* mutex1_getTest (void);
static char* mutex2_getTest (void);
static char* mutex3_getTest (void);

static void  mutex1_execute (void);
static void  mutex2_execute (void);
static void  mutex3_execute (void);


#define     MUTEX_NUM   3

/*---------------------------- Variable Define -------------------------------*/
OS_MutexID  Mutex_Id [MUTEX_NUM];       /*!< Define array save mutex id	*/

volatile unsigned char Cnt       = 0;
volatile unsigned char Del_Flag  = 0;
volatile unsigned char Exit_Flag = 0;

volatile unsigned char Flag [50] = {0};

#if MUTEX_NUM   == 2
  unsigned char Sequence [CFG_MAX_USER_TASKS] = {4,6,8,10,12,3,5,7,9,11};
  unsigned char Sequence2[CFG_MAX_USER_TASKS] = {4,6,8,10,12,3,5,7,9,11}; 
  unsigned char Sequence3[CFG_MAX_USER_TASKS] = {0};
#elif MUTEX_NUM == 3
  unsigned char Sequence [MAX_SLAVE_TEST_TASKS] = {3,6,9,12,5,8,11,4,7,10};
  unsigned char Sequence2[CFG_MAX_USER_TASKS] = {3,6,9,12,5,8,11,4,7,10};
  //unsigned char Sequence3[CFG_MAX_USER_TASKS] = {3,5,8,11,4,6, 9,7,0,0}; // 98
  unsigned char Sequence3[CFG_MAX_USER_TASKS] = {3,5,8,11,6, 9,4,7,0,0}; // 108
#elif MUTEX_NUM == 5
  unsigned char Sequence [CFG_MAX_USER_TASKS] = {7,12,6,11,5,10,4,9,3,8};
  unsigned char Sequence2[CFG_MAX_USER_TASKS] = {7,12,6,11,5,10,4,9,3,8};
  unsigned char Sequence3[CFG_MAX_USER_TASKS] = {0};
#elif MUTEX_NUM == 10
  unsigned char Sequence[CFG_MAX_USER_TASKS]  = {12,11,10,9,8,7,6,5,4,3};
  unsigned char Sequence2[CFG_MAX_USER_TASKS] = {12,11,10,9,8,7,6,5,4,3};
  unsigned char Sequence3[CFG_MAX_USER_TASKS] = {0};
#endif


const TestCase mutexTest1 = {
	mutex1_getTest,
	NULL,
	NULL,
	mutex1_execute
};

const TestCase mutexTest2 = {
	mutex2_getTest,
	NULL,
	NULL,
	mutex2_execute
};

const TestCase mutexTest3 = {
	mutex3_getTest,
	NULL,
	NULL,
	mutex3_execute
};

/*
 * Test sequence for mutex pattern.
 */
const TestCase* const patternMutex[] = {
	&mutexTest1,
	&mutexTest2,
	&mutexTest3,
	NULL
};



static char* mutex1_getTest (void)
{
	return "mutex test example 1";
}

static char* mutex2_getTest (void)
{
	return "mutex test example 2";
}

static char* mutex3_getTest (void)
{
	return "mutex test example 3";
}



/**
 * -----------------------------------------------------------------------------
 * @brief       mutexTask01	 
 * @param[in]   pdata    A pointer to parameter passed to task.	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     Mutex test task code. 
 * -----------------------------------------------------------------------------
 */
static void mutexTask01 (void* pdata)
{
	unsigned int  task_para;
	unsigned char err;
	unsigned char mutex_id;
	
	task_para  = (unsigned int)pdata;
	mutex_id = task_para%MUTEX_NUM;
	
	for (;;)
	{
	
	 err = CoEnterMutexSection (Mutex_Id[mutex_id]);
	 if (err != E_OK) {
	    printf ("\r Enter mutex error: \n");
		printf ("\r Task id = %2d, mutex id = %2d \n",task_para,mutex_id);
	 }
	
	 if (task_para < MUTEX_NUM) {
	     CoTickDelay (20);
	 }
	 
	 Flag [Cnt] = task_para +3;
	 Cnt++ ;
	
	 err = CoLeaveMutexSection (Mutex_Id[mutex_id]);
	 if (err != E_OK) {
	    printf ("\r Leave Mutex error: \n");
		printf ("\r Task id = %2d, mutex id = %2d \n",task_para,mutex_id);
	 }
	
	 if (Cnt == MAX_SLAVE_TEST_TASKS)
	 {
	    Exit_Flag  = 1;
	 }
	
	 CoExitTask ();
	
	}
}


/**
 * -----------------------------------------------------------------------------
 * @brief       mutexTask02	 
 * @param[in]   pdata    A pointer to parameter passed to task.	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     Mutex test task code. 
 * -----------------------------------------------------------------------------
 */
static void mutexTask02 (void* pdata)
{
	unsigned int  task_para;
	unsigned char err;
	unsigned char mutex_id;
	
	task_para  = (unsigned int)pdata;
	mutex_id = task_para%MUTEX_NUM;
	
	for (;;)
	{
	
	 err = CoEnterMutexSection (Mutex_Id[mutex_id]);
	 if (err != E_OK) {
	    printf ("\r Enter mutex error: \n");
		printf ("\r Task id = %2d, mutex id = %2d \n",task_para,mutex_id);
	 }
	
	 if (task_para < MUTEX_NUM) {
	     CoTickDelay (25);
	 }
	 
	 Flag [Cnt] = task_para +3;
	 Cnt++ ;
	
	 if (Cnt == MAX_SLAVE_TEST_TASKS){
	    Exit_Flag  = 1;
	 }
	
	 if (task_para < MUTEX_NUM)	{
	     Del_Flag = 1;
		 CoExitTask ();
	 }
	
	 err = CoLeaveMutexSection (Mutex_Id[mutex_id]);
	 if (err != E_OK) {
	    printf ("\r Leave Mutex error: \n");
		printf ("\r Task id = %2d, mutex id = %2d \n",task_para,mutex_id);
	 }
	
	 if (Del_Flag == 1) {
	 	CoExitTask ();
	 }
	}
}


/**
 * -----------------------------------------------------------------------------
 * @brief       mutexTask03	 
 * @param[in]   pdata    A pointer to parameter passed to task.	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     Mutex test task code. 
 * -----------------------------------------------------------------------------
 */
static void mutexTask03 (void* pdata)
{
	unsigned int  task_para;
	unsigned char err;
	unsigned char mutex_id;
	task_para = (unsigned int)pdata;
	mutex_id = task_para % MUTEX_NUM;
	
	for (;;)
	{
	 err = CoEnterMutexSection (Mutex_Id[mutex_id]);
	 if (err != E_OK) {
	    printf ("\r Enter mutex error: \n");
		printf ("\r Task id = %2d, mutex id = %2d \n",task_para,mutex_id);
	 }
	 
	 if (task_para < MUTEX_NUM) {
	     CoTickDelay (30);
	
		 Flag [Cnt] = task_para+3;
	     Cnt++;
		  
		 err = CoDelTask (Task_Id[MAX_SLAVE_TEST_TASKS-task_para-1]);
		 if (task_para == 1) {
	         if (err == E_OK) {
	             printf ("\r Error: delete task [%d] fail.\n",MAX_SLAVE_TEST_TASKS-task_para-1);
	         }		   
		 }
		 else {
	         if (err != E_OK) {
	             printf ("\r Error: delete task [%d] fail.\n",MAX_SLAVE_TEST_TASKS-task_para-1);
	         }
		 }
	 }
	 else {
	 	 Flag [Cnt] = task_para+3;
	     Cnt++;
	 }
	
	 err = CoLeaveMutexSection (Mutex_Id[mutex_id]);
	 if (err != E_OK) {
	    printf ("\r Leave Mutex error: \n");
		printf ("\r Task id = %2d, mutex id = %2d \n",task_para,mutex_id);
	 }
	
	 if (task_para == 4)
	    Exit_Flag = 1;
	 
	 CoExitTask ();
	 
	}
}



/**
 * -----------------------------------------------------------------------------
 * @brief       mutex1_execute	 
 * @param[in]   None	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     The code of mutex test example 1. 
 * -----------------------------------------------------------------------------
 */
static void mutex1_execute (void)
{
  unsigned char i;
  Cnt = 0;
  Del_Flag  = 0;
  Exit_Flag = 0;

  CoTickDelay (1);

  for (i=0; i<MUTEX_NUM; i++) {
      Mutex_Id[i] = CoCreateMutex ();
	  if (Mutex_Id[i] == E_CREATE_FAIL) {
	       printf ("\r Create the %d mutex fail. \n",i+1);
	  }
  }

  for (i=0; i< MAX_SLAVE_TEST_TASKS; i++) {
      Task_Id [i] = CoCreateTask (mutexTask01,(void*)i,MAINTEST_PRIMARY_PRIORITY-(i+1),&Task_Stack[i][SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE);
	  testAssert((Task_Id[i] != E_CREATE_FAIL)," Create task fail #3 ");
  }

    while (Exit_Flag == 0) ;
	Del_Flag  = 0;
	Exit_Flag = 0;

	for (i=0; i<MAX_SLAVE_TEST_TASKS; i++) {
	    testAssert((Flag[i] == Sequence[i])," Mutex #1 ");
	} 
}


/**
 * -----------------------------------------------------------------------------
 * @brief       mutex2_execute	 
 * @param[in]   None	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     The code of mutex test example 3. 
 * -----------------------------------------------------------------------------
 */
static void mutex2_execute (void)
{
	unsigned char i;
	Cnt = 0;
	Exit_Flag = 0;
	
	CoTickDelay (1);
	
	for (i=0;i<MAX_SLAVE_TEST_TASKS;i++ ){
	 Flag [i] = 0;
	}
	
	for (i=0; i< MAX_SLAVE_TEST_TASKS; i++) {
	  Task_Id [i] = CoCreateTask (mutexTask02,(void*)i,MAINTEST_PRIMARY_PRIORITY-(i+1),&Task_Stack[i][SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE);
	  if (Task_Id[i] == E_CREATE_FAIL) {
	      printf ("\r Create the %d mutex task fail. \n",i+1);
	  }
	}
	
	while (Exit_Flag == 0) ;
	Exit_Flag = 0;
	
	for (i=0; i<MAX_SLAVE_TEST_TASKS; i++) {
	 testAssert((Flag[i] == Sequence2[i])," Mutex #2 ");
	}
}



/**
 * -----------------------------------------------------------------------------
 * @brief       mutex3_execute	 
 * @param[in]   None	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     The code of mutex test example 2. 
 * -----------------------------------------------------------------------------
 */
static void mutex3_execute (void)
{

	unsigned char i;
	Cnt = 0;
	Exit_Flag = 0;
	
	CoTickDelay (1);
	
	
	for (i=0;i<MAX_SLAVE_TEST_TASKS;i++ ){
	 Flag [i] = 0;
	}
	
	for (i=0; i< MAX_SLAVE_TEST_TASKS; i++) {
	  Task_Id [i] = CoCreateTask (mutexTask03,(void*)i,MAINTEST_PRIMARY_PRIORITY-(i+1),&Task_Stack[i][SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE);
	  testAssert((Task_Id[i] != E_CREATE_FAIL)," Mutex #3: Create mutex fail ");
	}
	
	while (Exit_Flag == 0) ;
	Exit_Flag = 0;
	
	for (i=0; i<MAX_SLAVE_TEST_TASKS; i++) {
	  testAssert((Flag[i] == Sequence3[i])," Mutex #3: Test fail. ");
	} 
}
#endif


