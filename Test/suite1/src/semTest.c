/**
 *******************************************************************************
 * @file        semTest.c
 * @version     V1.00    Initial version
 * @date        2009.06.26
 * @brief       This file provides all test functions for sem.
 *******************************************************************************
 * @copy
 *
 * INTERNAL TEST FILE,DON'T PUBLIC.
 * 
 * <h2><center>&copy; COPYRIGHT 2009 CooCox </center></h2>
 *******************************************************************************
 */ 

/*---------------------------- Inlcude ---------------------------------------*/
#include <config.h>
#include <CoOS.h>
#include "test.h"

#if (CFG_EVENT_EN > 0 && CFG_SEM_EN > 0 && SEM_TEST_EN > 0)

/*---------------------------- Macro Define ----------------------------------*/
#define ALLOWED_DELAY_TICK (2)

/*---------------------------- Variable Define -------------------------------*/
OS_EventID sem1ID;

static char* sem1_getTest(void) 
{
	return "Semaphores, FIFO enqueuing test";
}

static void task1(void *pdata) 
{
	StatusType err;
	//StatusType PendSem(OS_EventID id,U32 timeout)
	err = CoPendSem(sem1ID,0);
	if (err == E_OK)
	{
	 	testEmitToken(*(char*)pdata);
	}
	CoExitTask();
}

static void sem1_execute(void) 
{
	int i;
	StatusType err;	
	sem1ID = CoCreateSem(0,5,EVENT_SORT_TYPE_FIFO);
	testAssert(sem1ID != (OS_EventID)-1,"#1");
	
	Task_Id[0] = CoCreateTask(task1, "A", MAINTEST_PRIMARY_PRIORITY - 3, &Task_Stack[0][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[1] = CoCreateTask(task1, "B", MAINTEST_PRIMARY_PRIORITY - 4, &Task_Stack[1][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[2] = CoCreateTask(task1, "C", MAINTEST_PRIMARY_PRIORITY - 1, &Task_Stack[2][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[3] = CoCreateTask(task1, "D", MAINTEST_PRIMARY_PRIORITY - 5, &Task_Stack[3][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[4] = CoCreateTask(task1, "E", MAINTEST_PRIMARY_PRIORITY - 2, &Task_Stack[4][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	
	for(i = 0; i < 5; i++)
	{
		err = CoPostSem(sem1ID);
	}
	testAssertSequence("ABCDE");
	
	err = CoDelSem(sem1ID,OPT_DEL_ANYWAY);
	testAssert(err == E_OK,"#2");

}

static char* sem2_getTest(void) 
{
	return "Semaphores, priority preemptive enqueuing test";
}

static void sem2_execute(void) 
{
	int i;
	StatusType err;	
	for(i = 0; i < MAX_SLAVE_TEST_TASKS; i++)
	{
	 	Task_Id[i] = 0;
	}
	sem1ID = CoCreateSem(0,5,EVENT_SORT_TYPE_PRIO);
	testAssert(sem1ID != (OS_EventID)-1,"#3");;

	Task_Id[0] = CoCreateTask(task1, "A", MAINTEST_PRIMARY_PRIORITY - 3, &Task_Stack[0][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[1] = CoCreateTask(task1, "B", MAINTEST_PRIMARY_PRIORITY - 4, &Task_Stack[1][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[2] = CoCreateTask(task1, "C", MAINTEST_PRIMARY_PRIORITY - 1, &Task_Stack[2][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[3] = CoCreateTask(task1, "D", MAINTEST_PRIMARY_PRIORITY - 5, &Task_Stack[3][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[4] = CoCreateTask(task1, "E", MAINTEST_PRIMARY_PRIORITY - 2, &Task_Stack[4][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);

	for(i = 0; i < 5; i++)
	{
		err = CoPostSem(sem1ID);
	}
	testAssertSequence("DBAEC");
	err = CoDelSem(sem1ID,OPT_DEL_ANYWAY);
	testAssert(err == E_OK,"#4");

}
const TestCase testSem1 = {
	sem1_getTest,
	NULL,
	NULL,
	sem1_execute
};


const TestCase testSem2 = {
	sem2_getTest,
	NULL,
	NULL,
	sem2_execute  
};


static void task2(void* pdata)
{
 	CoTickDelay(50);
	CoPostSem(sem1ID);
	CoExitTask();
}
static char* sem3_getTest(void) 
{
	return "Semaphores, timeout test";
}

static void sem3_execute(void)
{
	StatusType err;
	U64 expectTime;
	int i;

	sem1ID = CoCreateSem(0,5,EVENT_SORT_TYPE_FIFO);
	// Test 1 AcceptSem
	expectTime = CoGetOSTime(); 
	err = CoAcceptSem(sem1ID);
	testAssert(err == E_SEM_EMPTY,"#5");
	testAssertTimeWindow(expectTime,expectTime + ALLOWED_DELAY_TICK);
	// Test 2 PendSem with timeout
	
	//printf("#5");
	expectTime = CoGetOSTime() + (MAX_SLAVE_TEST_TASKS * 50); 
	for(i = 0; i < MAX_SLAVE_TEST_TASKS; i++)
	{
		err = CoPendSem(sem1ID,50);
		testAssert(err == E_TIMEOUT,"#6");
	}
	testAssertTimeWindow(expectTime,expectTime + ALLOWED_DELAY_TICK);
	//printf("#6");
	// Test 3 PendSem without timeout
	Task_Id[0] = CoCreateTask(task2, "A", MAINTEST_PRIMARY_PRIORITY - 3, &Task_Stack[0][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	err = CoPendSem(sem1ID,100);
	testAssert(err == E_OK,"#8");
	// PostSem only inc 1 && PendSem dec 1
	//printf("#7");
	err = CoPendSem(sem1ID,100);
	testAssert(err == E_TIMEOUT,"#9");
	err = CoDelSem(sem1ID,OPT_DEL_NO_PEND);
	testAssert(err == E_OK,"#7");


}

const TestCase testSem3 = {
	sem3_getTest,
	NULL,
	NULL,
	sem3_execute
};

/*
 * Test sequence for semaphores pattern.
 */
const TestCase* const patternSem[] = {
	&testSem1,
	&testSem2,
	&testSem3,
	NULL
};
#endif

