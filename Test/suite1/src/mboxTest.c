/**
 *******************************************************************************
 * @file        mboxTest.c
 * @version     V1.00    Initial version
 * @date        2009.06.26
 * @brief       This file provides all test functions for mbox.
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

#if (CFG_EVENT_EN > 0 && CFG_MAILBOX_EN > 0 && MBOX_TEST_EN > 0)
/*---------------------------- Macro Define ----------------------------------*/
#define ALLOWED_DELAY_TICK (2)

/*---------------------------- Variable Define -------------------------------*/
OS_EventID mbox1ID;


static char* mbox1_getTest(void)
{
	return "Mailbox, FIFO enqueuing test";
}

static void task1(void *pdata) 
{
	StatusType err;
	CoPendMail(mbox1ID,0,&err);
	if (err == E_OK)
	{
	 	testEmitToken(*(char*)pdata);
	}
	CoExitTask();
}

static void mbox1_execute(void) 
{
	int i;
	StatusType err;	
	mbox1ID = CoCreateMbox(EVENT_SORT_TYPE_FIFO);
	testAssert(mbox1ID != (OS_EventID)-1,"#1");

	Task_Id[0] = CoCreateTask(task1, "A", MAINTEST_PRIMARY_PRIORITY - 3, &Task_Stack[0][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[1] = CoCreateTask(task1, "B", MAINTEST_PRIMARY_PRIORITY - 4, &Task_Stack[1][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[2] = CoCreateTask(task1, "C", MAINTEST_PRIMARY_PRIORITY - 1, &Task_Stack[2][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[3] = CoCreateTask(task1, "D", MAINTEST_PRIMARY_PRIORITY - 5, &Task_Stack[3][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[4] = CoCreateTask(task1, "E", MAINTEST_PRIMARY_PRIORITY - 2, &Task_Stack[4][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);

	for(i = 0; i < 5; i++)
	{
		err = CoPostMail(mbox1ID,NULL);
	}
	testAssertSequence("ABCDE");
	
	err = CoDelMbox(mbox1ID,OPT_DEL_ANYWAY);
	testAssert(err == E_OK,"#2");

}

const TestCase testMbox1 = {
	mbox1_getTest,
	NULL,
	NULL,
	mbox1_execute
};

static char* mbox2_getTest(void)
{
	return "Mailbox, priority preemptive enqueuing test";
}


static void mbox2_execute(void) 
{
	int i;
	StatusType err;	
	mbox1ID = CoCreateMbox(EVENT_SORT_TYPE_PRIO);
	testAssert(mbox1ID != (OS_EventID)-1,"#1");
	
	Task_Id[0] = CoCreateTask(task1, "A", MAINTEST_PRIMARY_PRIORITY - 3, &Task_Stack[0][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[1] = CoCreateTask(task1, "B", MAINTEST_PRIMARY_PRIORITY - 4, &Task_Stack[1][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[2] = CoCreateTask(task1, "C", MAINTEST_PRIMARY_PRIORITY - 1, &Task_Stack[2][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[3] = CoCreateTask(task1, "D", MAINTEST_PRIMARY_PRIORITY - 5, &Task_Stack[3][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[4] = CoCreateTask(task1, "E", MAINTEST_PRIMARY_PRIORITY - 2, &Task_Stack[4][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	
	for(i = 0; i < 5; i++)
	{
		err = CoPostMail(mbox1ID,NULL);
	}
	testAssertSequence("DBAEC");
	
	err = CoDelMbox(mbox1ID,OPT_DEL_ANYWAY);
	testAssert(err == E_OK,"#2");

}

const TestCase testMbox2 = {
	mbox2_getTest,
	NULL,
	NULL,
	mbox2_execute
};

static char* mbox3_getTest(void)
{
	return "Mailbox, Basic & timeout test";
}

static void task2(void* pdata)
{
	CoTickDelay((U32)pdata);
	CoPostMail(mbox1ID,"F");
	CoExitTask();
}

static void task3(void* pdata)
{
	StatusType err;
	CoPendMail(mbox1ID,0,&err);
	CoExitTask();
}
static void mbox3_execute(void) 
{
	int i;
	StatusType err;
	U32* pmail;
	U64 expectTime;
	// Test 1 mbox1ID is an invaild Mbox ID 
	err = CoPostMail(mbox1ID,NULL);
	testAssert(err == E_INVALID_ID,"#3");
	
	err = CoDelMbox(mbox1ID,OPT_DEL_ANYWAY);
	testAssert(err == E_INVALID_ID,"#4");
	
		
	mbox1ID = CoCreateMbox(EVENT_SORT_TYPE_PRIO);
	testAssert(mbox1ID != (OS_EventID)-1,"#5");
	
	// Test 2 First Time Fill the mailbox
	err = CoPostMail(mbox1ID,"A");
	testAssert(err == E_OK,"#6");
	
	// Test 3 Fill a full mailbox
	err = CoPostMail(mbox1ID,"B");
	testAssert(err == E_MBOX_FULL,"#7");
	
	// Test 4 AcceptMail a full mailbox
	pmail = CoAcceptMail(mbox1ID,&err);
	testAssert(err == E_OK,"#8");
	testAssert(*(char*)pmail == 'A',"#9");
	
	// Test 5 AcceptMail an empty mailbox
	pmail = CoAcceptMail(mbox1ID,&err);
	testAssert(err == E_MBOX_EMPTY,"#10");
	
	// Test 6  PendMail with timeout
	expectTime = CoGetOSTime() + (MAX_SLAVE_TEST_TASKS * 50); 
	for(i = 0; i < MAX_SLAVE_TEST_TASKS; i++)
	{
		pmail = CoPendMail(mbox1ID,50,&err);
		testAssert(err == E_TIMEOUT,"#11");
	}
	testAssertTimeWindow(expectTime,expectTime + ALLOWED_DELAY_TICK);
	
	
	Task_Id[0] = CoCreateTask(task2, (void*)50, MAINTEST_PRIMARY_PRIORITY - 3, &Task_Stack[0][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	expectTime = CoGetOSTime();
	pmail = CoPendMail(mbox1ID,100,&err);
	testAssert(*(char*)pmail == 'F',"#12");
	testAssertTimeWindow(expectTime,expectTime + 100);
	
	// Test 7 if mail can be any U32
	err = CoPostMail(mbox1ID,(void*)0);
	testAssert(err == E_OK,"#13");
	pmail = CoPendMail(mbox1ID,100,&err);
	testAssert(err == E_OK,"#14");	// if can't err == E_TIMEOUT
	
	err = CoPostMail(mbox1ID,(void*)-1);
	testAssert(err == E_OK,"#15");
	pmail = CoPendMail(mbox1ID,100,&err);
	testAssert(err == E_OK,"#16");	// if can't err == E_TIMEOUT
	
	// Test 8 Del a Mbox when there is tasks pend
	pmail = CoAcceptMail(mbox1ID,&err);	// clear the mailbox
	testAssert(err == E_MBOX_EMPTY,"#17");
	Task_Id[0] = CoCreateTask(task3, (void*)50, MAINTEST_PRIMARY_PRIORITY - 3, &Task_Stack[0][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	
	// Task_Id[0] is waiting mbox1ID
	err = CoDelMbox(mbox1ID,OPT_DEL_NO_PEND);
	testAssert(err == E_TASK_WAITING,"#18");
	// delete task Task_Id[0]
	err = CoDelTask(Task_Id[0]);
	testAssert(err == E_OK,"#19");
	
	// now there is no task waiting mbox1ID
	err = CoDelMbox(mbox1ID,OPT_DEL_NO_PEND);
	testAssert(err == E_OK,"#20");
	
	// Test 9  We use a Sem type as a mailbox
	mbox1ID = CoCreateSem(0,5,EVENT_SORT_TYPE_FIFO);
	err = CoPostMail(mbox1ID,NULL);
	testAssert(err == E_INVALID_ID,"#21");
	
	err = CoDelMbox(mbox1ID,OPT_DEL_NO_PEND);
	testAssert(err == E_INVALID_ID,"#22");
	
	err = CoDelSem(mbox1ID,OPT_DEL_NO_PEND);
	testAssert(err == E_OK,"#23");

}

const TestCase testMbox3 = {
	mbox3_getTest,
	NULL,
	NULL,
	mbox3_execute
};

/*
 * Test sequence for semaphores pattern.
 */
const TestCase* const patternMbox[] = {
	&testMbox1,
	&testMbox2,
	&testMbox3,
	NULL
};
#endif


