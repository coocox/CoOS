/**
 *******************************************************************************
 * @file        queueTest.c
 * @version     V1.00    Initial version
 * @date        2009.06.26
 * @brief       This file provides all test functions for queue.
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

#if (CFG_EVENT_EN > 0 && CFG_QUEUE_EN > 0 && QUEUE_TEST_EN > 0)

/*---------------------------- Macro Define ----------------------------------*/
#define ALLOWED_DELAY_TICK (2)
#define TEST_QUEUE_BUF_SIZE ( 5 )

/*---------------------------- Variable Define -------------------------------*/
OS_EventID queue1ID;
U32 queue1Buf[TEST_QUEUE_BUF_SIZE];

static char* queue1_getTest(void)
{
	return "Queue, Basic & Timeout test";
}

static void task1(void* pdata)
{
	StatusType err;
	for(;;)
	{
		CoPendQueueMail(queue1ID,0,&err);
	}
	// Delete this task in queue1_execute
}

static void queue1_execute(void) 
{
	int i;
	StatusType err;
	void* pmail;
	U64 expectTime;

	char expectQueueBuf[TEST_QUEUE_BUF_SIZE+1];

	for(i = 0; i < TEST_QUEUE_BUF_SIZE; i++)
	{
		expectQueueBuf[i] = 'A' + i;	
	}
	expectQueueBuf[i] = '\0';

	// Test 1 operate on an invaild Queue
	err = CoPostQueueMail(queue1ID,NULL);
	testAssert(err == E_INVALID_ID,"#1");
	err = CoDelQueue(queue1ID,OPT_DEL_ANYWAY);
	testAssert(err == E_INVALID_ID,"#2");
	// Test 2 CreateQueue
	queue1ID = CoCreateQueue((void**)queue1Buf,TEST_QUEUE_BUF_SIZE,EVENT_SORT_TYPE_PRIO);
	testAssert(queue1ID != (OS_EventID)-1,"#3");

	// Test 3 Fill an empty queue
	for(i = 0; i < TEST_QUEUE_BUF_SIZE; i ++)
	{
		err = CoPostQueueMail(queue1ID,(void*)expectQueueBuf[i]);
		testAssert(err == E_OK,"#4"); 		
	}

	// Test 4 Fill an full queue
	err = CoPostQueueMail(queue1ID,(void*)('A' + TEST_QUEUE_BUF_SIZE));
	testAssert(err == E_QUEUE_FULL,"#5");

	// Test 5 PendQueueMail without timeout
	expectTime = CoGetOSTime(); 
	for(i = 0; i < TEST_QUEUE_BUF_SIZE; i++)
	{
		pmail = CoPendQueueMail(queue1ID,50,&err);
		testAssert(err == E_OK,"#6");
		testEmitToken((char)(U32)pmail);
	}
	testAssertTimeWindow(expectTime,expectTime + (TEST_QUEUE_BUF_SIZE * 50) + ALLOWED_DELAY_TICK);

	// Test 6 if the Queue is FIFO
	testAssertSequence(expectQueueBuf);

	// Test 7 Empty Queue,PendQueueMail timeout
	expectTime = CoGetOSTime() + (TEST_QUEUE_BUF_SIZE * 50);
	for(i = 0; i < TEST_QUEUE_BUF_SIZE; i++)
	{
		pmail = CoPendQueueMail(queue1ID,50,&err);
		testAssert(err == E_TIMEOUT,"#7");
	}
 	testAssertTimeWindow(expectTime,expectTime + ALLOWED_DELAY_TICK);

	// Test 8 AcceptQueueMail a empty queue
	pmail = CoAcceptQueueMail(queue1ID,&err);
 	testAssert(err == E_QUEUE_EMPTY,"#8");

	// Test 9 AcceptQueueMail a not empty queue
	err = CoPostQueueMail(queue1ID,(void*)'F');
	pmail = CoAcceptQueueMail(queue1ID,&err);
	testAssert(err == E_OK,"#8");
	testAssert((char)(U32)pmail == 'F',"#9");

	// Test 10 Delete a queue with task wait
	Task_Id[0] = CoCreateTask(task1, NULL, MAINTEST_PRIMARY_PRIORITY - 3, &Task_Stack[0][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	err = CoDelQueue(queue1ID,OPT_DEL_NO_PEND);
	testAssert(err == E_TASK_WAITING,"#10");

	err = CoDelQueue(queue1ID,OPT_DEL_ANYWAY);
	testAssert(err == E_OK,"#11");

	CoDelTask(Task_Id[0]);


}

const TestCase testQueue1 = {
	queue1_getTest,
	NULL,
	NULL,
	queue1_execute
};

static char* queue2_getTest(void)
{
	return "Queue, FIFO enqueuing test";
}

static void task2(void *pdata) 
{
	StatusType err;
	CoPendQueueMail(queue1ID,0,&err);
	if (err == E_OK)
	{
	 	testEmitToken(*(char*)pdata);
	}
	CoExitTask();
}

static void queue2_execute(void) 
{
	int i;
	StatusType err;
		
	queue1ID = CoCreateQueue((void**)queue1Buf,TEST_QUEUE_BUF_SIZE,EVENT_SORT_TYPE_FIFO);
	testAssert(queue1ID != (OS_EventID)-1,"#12");

	Task_Id[0] = CoCreateTask(task2, "A", MAINTEST_PRIMARY_PRIORITY - 3, &Task_Stack[0][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[1] = CoCreateTask(task2, "B", MAINTEST_PRIMARY_PRIORITY - 4, &Task_Stack[1][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[2] = CoCreateTask(task2, "C", MAINTEST_PRIMARY_PRIORITY - 1, &Task_Stack[2][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[3] = CoCreateTask(task2, "D", MAINTEST_PRIMARY_PRIORITY - 5, &Task_Stack[3][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[4] = CoCreateTask(task2, "E", MAINTEST_PRIMARY_PRIORITY - 2, &Task_Stack[4][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);

	for(i = 0; i < 5; i++)
	{
		err = CoPostQueueMail(queue1ID,NULL);
	}
	testAssertSequence("ABCDE");
	
	err = CoDelQueue(queue1ID,OPT_DEL_NO_PEND);
	testAssert(err == E_OK,"#13");

}

const TestCase testQueue2 = {
	queue2_getTest,
	NULL,
	NULL,
	queue2_execute
};

static char* queue3_getTest(void)
{
  return "Queue, priority preemptive enqueuing test";
}


static void queue3_execute(void) 
{
	int i;
	StatusType err;
		
	queue1ID = CoCreateQueue((void**)queue1Buf,TEST_QUEUE_BUF_SIZE,EVENT_SORT_TYPE_PRIO);
	testAssert(queue1ID != (OS_EventID)-1,"#14");
	
	Task_Id[0] = CoCreateTask(task2, "A", MAINTEST_PRIMARY_PRIORITY - 3, &Task_Stack[0][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[1] = CoCreateTask(task2, "B", MAINTEST_PRIMARY_PRIORITY - 4, &Task_Stack[1][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[2] = CoCreateTask(task2, "C", MAINTEST_PRIMARY_PRIORITY - 1, &Task_Stack[2][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[3] = CoCreateTask(task2, "D", MAINTEST_PRIMARY_PRIORITY - 5, &Task_Stack[3][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	Task_Id[4] = CoCreateTask(task2, "E", MAINTEST_PRIMARY_PRIORITY - 2, &Task_Stack[4][SLAVE_TASK_STK_SIZE-1], SLAVE_TASK_STK_SIZE);
	
	for(i = 0; i < 5; i++)
	{
		err = CoPostQueueMail(queue1ID,NULL);
	}
	testAssertSequence("DBAEC");
	
	err = CoDelQueue(queue1ID,OPT_DEL_NO_PEND);
	testAssert(err == E_OK,"#15");

}

const TestCase testQueue3 = {
	queue3_getTest,
	NULL,
	NULL,
	queue3_execute
};

/*
 * Test sequence for semaphores pattern.
 */
const TestCase* const patternQueue[] = {
	&testQueue1,
	&testQueue2,
	&testQueue3,

  NULL
};
#endif

