/**
 *******************************************************************************
 * @file        taskTest.c
 * @version     V1.00    Initial version
 * @date        2009.06.26
 * @brief       This file provides all test functions for task.
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

#if (TASK_TEST_EN > 0)

/*---------------------------- Variable Define -------------------------------*/
U8 dbgPassCnt;

static char* task1_getTest(void)
{
	return "Task, Basic test";
}

void exitTaskTestTask(void* pdata)
{
	testEmitToken((char)(U32)pdata);
	dbgPassCnt++;
	CoExitTask();
	dbgPassCnt--;
}

void prioTestTask(void* pdata)
{
	for(;;)
	{
		dbgPassCnt++;
		CoSetPriority(mainTestTaskID,MAINTEST_PRIMARY_PRIORITY);
	}
}

void awakeTestTask(void* pdata)
{
	for(;;)
	{
		dbgPassCnt++;	
		CoAwakeTask(mainTestTaskID);
	}

}
static void task1_execute(void) 
{
	int i;
	StatusType err;
	char expectSequence[MAX_SLAVE_TEST_TASKS+1];
	for(i = 0; i < MAX_SLAVE_TEST_TASKS; i++)
	{
	 	expectSequence[i] = 'A' + i;
	}
	expectSequence[i] = '\0';
	
	dbgPassCnt = 0;
	// Test 1 CreateTask  && argv && ExitTask && Preemptive Priority
	for(i = 0; i < MAX_SLAVE_TEST_TASKS; i++)
	{
		Task_Id[i] = CoCreateTask(exitTaskTestTask,(void*)expectSequence[i],MAINTEST_PRIMARY_PRIORITY-1,&Task_Stack[i][SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE);
		testAssert(Task_Id[i] != (OS_TID)-1,"#1");
	}
	testAssert(dbgPassCnt == MAX_SLAVE_TEST_TASKS,"#2");
	testAssertSequence(expectSequence);
	
	// Test 2  SetPriority
	for(i = 0; i < MAX_SLAVE_TEST_TASKS; i++)
	{
		Task_Id[i] = 0;
	}
	dbgPassCnt = 0;
	for(i = 0; i < MAX_SLAVE_TEST_TASKS; i++)
	{
		Task_Id[i] = CoCreateTask(prioTestTask,NULL,MAINTEST_PRIMARY_PRIORITY+(MAX_SLAVE_TEST_TASKS-i),&Task_Stack[i][SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE);
		testAssert(Task_Id[i] != (OS_TID)-1,"#3");
		err = CoSetPriority(mainTestTaskID,MAINTEST_PRIMARY_PRIORITY + 2 + (MAX_SLAVE_TEST_TASKS-i));
		testAssert(err == E_OK,"#4");	
	}
	testAssert(dbgPassCnt == MAX_SLAVE_TEST_TASKS,"#5");
	
	for(i = 0; i < MAX_SLAVE_TEST_TASKS; i++)
	{
		err = CoDelTask(Task_Id[i]);
		testAssert(err == E_OK,"#6");
	}
	// Delete invaild task
	for(i = 0; i < MAX_SLAVE_TEST_TASKS; i++)
	{
		err = CoDelTask(Task_Id[i]);
		testAssert(err == E_INVALID_ID,"#7");
	}
	
	// Test 3 SuspendTask && AwakeTask
	dbgPassCnt = 0;
	
	// Awake a running task
	err = CoAwakeTask(mainTestTaskID);
	testAssert(err == E_TASK_NOT_WAITING,"#8");
	
	for(i = 0; i < MAX_SLAVE_TEST_TASKS; i++)
	{
		Task_Id[i] = CoCreateTask(awakeTestTask,NULL,MAINTEST_PRIMARY_PRIORITY+(MAX_SLAVE_TEST_TASKS-i),&Task_Stack[i][SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE);
		testAssert(Task_Id[i] != (OS_TID)-1,"#9");
		// Suspend a ready task
		err = CoSuspendTask(Task_Id[i]);
		testAssert(err == E_OK,"#10");
	
		// Awake a wait task
		err = CoAwakeTask(Task_Id[i]);
		testAssert(err == E_OK,"#11");
	
		// Awake a ready task
		err = CoAwakeTask(Task_Id[i]);
		testAssert(err == E_TASK_NOT_WAITING,"#12");
	
		// Suspend the running task
		err = CoSuspendTask(mainTestTaskID);		
		if (err != E_OK)
		testAssert(err == E_OK,"#13");
	}
	testAssert(dbgPassCnt == MAX_SLAVE_TEST_TASKS,"#14");
	
	for(i = 0; i < MAX_SLAVE_TEST_TASKS; i++)
	{
		err = CoDelTask(Task_Id[i]);
		testAssert(err == E_OK,"#15");
	}

}

const TestCase testTask1 = {
	task1_getTest,
	NULL,
	NULL,
	task1_execute
};

/*
 * Test sequence for semaphores pattern.
 */
const TestCase* const patternTask[] = {
	&testTask1,
	NULL
};
#endif

