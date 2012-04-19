/**
 *******************************************************************************
 * @file        timeTest.c
 * @version     V1.00    Initial version
 * @date        2009.06.26
 * @brief       This file provides all test functions for Time.
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

#if (CFG_TIME_DELAY_EN > 0 && TIME_TEST_EN > 0)

/*---------------------------- Macro Define ----------------------------------*/
#define ALLOWED_DELAY_TICK (2)

/*---------------------------- Variable Define -------------------------------*/
OS_EventID timeMbox;

static char* time1_getTest(void)
{
	return "Time, Basic test";
}

static void task1(void* pdata)
{
	U64 beginTime,endTime;
	U32 delayTicks;
	delayTicks = (U32)pdata;
	beginTime = CoGetOSTime();
	CoTickDelay(delayTicks);
	endTime = CoGetOSTime();
	delayTicks = (U32)(endTime - beginTime);
	CoPostMail(timeMbox,(void*)delayTicks);
	CoExitTask();
}

static void time1_execute(void) 
{
	U64 prevOsTime,curOsTime;
	U32 expectTicks,actualTicks;
	U32 expectSec,expectMs;
	StatusType err;

	expectTicks = 0xFF; 
	prevOsTime = CoGetOSTime();
	CoTickDelay(expectTicks);
	curOsTime = CoGetOSTime();
	actualTicks = (U32)(curOsTime - prevOsTime);
	// Test conditions: other task's priority should not be the same to the mainTimeTask 
	// Test 1  TickDelay's precision
	testAssert(actualTicks == expectTicks,"#1");

	expectSec = 2;
	expectMs = 80;
	err = CoTimeToTick(0,0,expectSec,expectMs,&expectTicks);
	testAssert(err == E_OK,"#0");
	prevOsTime = CoGetOSTime();
	//TickDelay(expectTicks);
	CoTimeDelay(0,0,expectSec,expectMs);
	curOsTime = CoGetOSTime();
	actualTicks = (U32)(curOsTime - prevOsTime);
	// Test conditions: other task's priority should not be the same to the mainTimeTask 
	// Test 2  TimeDelay's precision 
	testAssert(actualTicks == expectTicks,"#2");

	// Test 3  ResetTaskDelay for a running task
	err = CoResetTaskDelayTick(mainTestTaskID,0);
	testAssert(err == E_NOT_IN_DELAY_LIST,"#3");

	timeMbox = CoCreateMbox(EVENT_SORT_TYPE_PRIO);
	testAssert(err == E_NOT_IN_DELAY_LIST,"#4");

	Task_Id[0] = CoCreateTask(task1,(void*)0xFFFF,MAINTEST_PRIMARY_PRIORITY-1,&Task_Stack[0][SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE);
	
	testAssert(Task_Id[0] != (OS_TID)-1,"#5");

	// Test 4 ResetTaskDelay for a delay task 
	err = CoResetTaskDelayTick(Task_Id[0],0);
	testAssert(err == E_OK,"#6");

	// Get the actual delay ticks
	actualTicks = (U32)CoPendMail(timeMbox,50,&err);
	testAssert(err == E_OK,"#7");
	testAssert(actualTicks < ALLOWED_DELAY_TICK,"#8");
	err = CoDelMbox(timeMbox,OPT_DEL_NO_PEND);
	testAssert(err == E_OK,"#8");

}

const TestCase testTime1 = {
	time1_getTest,
	NULL,
	NULL,
	time1_execute
};

/*
 * Test sequence for semaphores pattern.
 */
const TestCase* const patternTime[] = {
	&testTime1,
	NULL
};
#endif
