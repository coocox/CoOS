/**
 *******************************************************************************
 * @file        test.c
 * @version     V1.00    Initial version
 * @date        2009.06.26
 * @brief       This file provides test frame for CooCox test.
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

#include <stdio.h>
#include "test.h"

/*---------------------------- Define  ---------------------------------------*/
#define MAINTEST_STK_SIZE (200)
#define PRINT_STK_SIZE    ( 70)


/*
 * Array of all the test patterns.
 */
static const TestCase* const *patterns[] = {


#if (TASK_TEST_EN > 0)
	patternTask,
#endif

#if (CFG_TIME_DELAY_EN > 0 && TIME_TEST_EN > 0)
	patternTime,
#endif

#if (CFG_TMR_EN >0 && TIMER_TEST_EN > 0)
	patternTimer,
#endif

#if (CFG_EVENT_EN > 0 && CFG_SEM_EN > 0 && SEM_TEST_EN > 0)
	patternSem,
#endif

#if (CFG_EVENT_EN > 0 && CFG_MAILBOX_EN > 0 && MBOX_TEST_EN > 0)
	patternMbox,
#endif

#if (CFG_EVENT_EN > 0 && CFG_QUEUE_EN > 0 && QUEUE_TEST_EN > 0)
	patternQueue,
#endif

#if (CFG_FLAG_EN > 0 && FLAG_TEST_EN > 0)
	patternFlag,
#endif

#if (CFG_MUTEX_EN > 0 && MUTEX_TEST_EN > 0)
	patternMutex,
#endif


#if (CFG_MM_EN > 0 && MM_TEST_EN > 0)
	patternMm,
#endif


#if (CFG_KHEAP_EN > 0 && KHEAP_TEST_EN > 0)
	patternKHeap,
#endif
	
	NULL
};

/*---------------------------- Variable Define -------------------------------*/
OS_STK      Task_Stack [MAX_SLAVE_TEST_TASKS][SLAVE_TASK_STK_SIZE];
OS_TID      Task_Id [CFG_MAX_USER_TASKS];

OS_STK		mainTestTaskBuf[MAINTEST_STK_SIZE];
OS_TID		mainTestTaskID;

static BOOL localFail, globalFail;
static char *failMsg;
static char tokensBuffer[MAX_TOKENS];
static char *tokp;


OS_TID printTaskID;
OS_STK printTestTaskBuf[PRINT_STK_SIZE];

OS_EventID printMboxID;


/*---------------------------- Function declare ------------------------------*/
void mainTestTask(void* pdata);
void printTask(void* pdata);



/*
 * Console output.
 */
void test_print(char* str)
{
	CoPostMail(printMboxID,str);
}


/*
 * Tokens.
 */
static void clearTokens(void) 
{
	int i;
	for(i = 0; i < MAX_TOKENS; i++)
	{
	 	tokensBuffer[i] = 0;
	}
	tokp = tokensBuffer;
}


static void printTokens(void) 
{
	test_print(tokensBuffer);
}


void testEmitToken(char token) 
{
	*tokp++ = token; 	
}


BOOL timeIsWithin(U64 start, U64 end) 
{
	U64 time = CoGetOSTime();
	return end > start ? (time >= start) && (time < end) :
	                   (time >= start) || (time < end);
}

/*
 * Assertions.
 */
BOOL _testFail(char * msg) 
{
	localFail = TRUE;
	globalFail = TRUE;
	failMsg = msg;
	return FALSE;
}


BOOL _testAssert(BOOL condition, char * msg) 
{
	if (!condition)
	{
	return _testFail(msg);
	}
	return TRUE;
}


BOOL _testAssertSequence(char *expected) 
{
	char *cp = tokensBuffer;
	while (cp < tokp) 
	{
	if (*cp++ != *expected++)
	{
			return _testFail(NULL);
		}
	}
	if (*expected)
	{
		return _testFail(NULL);
	}
	clearTokens();
	return TRUE;
}


BOOL _testAssertTimeWindow(U64 start, U64 end) 
{
	return _testAssert(timeIsWithin(start, end), "time window error");
}



void mainTestSetup(void)
{
	printMboxID = CoCreateMbox(EVENT_SORT_TYPE_PRIO);
	if (printMboxID == (OS_EventID)-1)
	{
	 	printf(" ***Error:In Test.c/mainTestSetup() CreateMbox printMboxID fail.  \r\n");
		while(1);
		//breakhere();
	}
	mainTestTaskID = CoCreateTask(mainTestTask,NULL,MAINTEST_PRIMARY_PRIORITY,&mainTestTaskBuf[MAINTEST_STK_SIZE-1],MAINTEST_STK_SIZE);

	if(mainTestTaskID == (OS_TID)-1)
	{
	 	printf(" ***Error:In Test.c/mainTestSetup() CreateTask mainTestTaskID fail. \r\n");
		while(1);
		//breakhere();
	}
	
	printTaskID = CoCreateTask(printTask,NULL,MAINTEST_PRIMARY_PRIORITY-1,&printTestTaskBuf[PRINT_STK_SIZE-1],PRINT_STK_SIZE);
	if(printTaskID == (OS_TID)-1)
	{
	 	printf(" ***Error:In Test.c/mainTestSetup() CreateTask printTaskID fail. \r\n");
		while(1);
		//breakhere();
	}
}


void printTask(void* pdata)
{
	StatusType err;
	char* strOut;
	for(;;)
	{
		strOut = (char*)CoPendMail(printMboxID,0,&err);
		if (err == E_OK)
		{
		 	printf(" %s",strOut);
		}
	}
}


static void executeTest(const TestCase* tcp)
{
	/* Initialization */
	clearTokens();
	localFail = FALSE;
	
	if (tcp->setup != NULL)
	tcp->setup();
	tcp->execute();
	if (tcp->teardown != NULL)
	tcp->teardown();
}


void mainTestTask(void* pdata)
{
	int i, j;
	static char strTmp[20];
	
	test_print("\r\n");
	test_print(" ***************************** \r\n");
	test_print(" *** CooCox-OS test suite  *** \r\n");
	test_print(" ***************************** \r\n");
	test_print("\r\n");
	
	globalFail = FALSE;
	i = 0;
	while (patterns[i]) {
	j = 0;
	while (patterns[i][j]) {
	  test_print("--------------------------------------------------------------------------- \r\n");
	  test_print("--- Test Case ");
			sprintf(strTmp,"%d.%d",i + 1, j + 1);
	  test_print(strTmp);
	  test_print(" (");
	  test_print(patterns[i][j]->getTest());
	  test_print(")\r\n");
	  executeTest(patterns[i][j]);
	  if (localFail) {
	    test_print("--- Result: FAIL (");
	    if (failMsg)
	      test_print(failMsg);
	    else {
	      test_print("sequence error: ");
	      printTokens();
	    }
	    test_print(")\r\n");
	  }
	  else
	    test_print("--- Result: SUCCESS \r\n");
	  j++;
	}
	i++;
	}
	test_print("--------------------------------------------------------------------------- \r\n");
	test_print("\r\n");
	test_print(" Final result:");
	if (globalFail)
	test_print(" FAIL \r\n");
	else
	test_print(" SUCCESS \r\n");
	
	CoExitTask();

}
