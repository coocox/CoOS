/**
 *******************************************************************************
 * @file        timerTest.c
 * @version     V1.00    Initial version
 * @date        2009.06.26
 * @brief       This file provides all test functions for timer.
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


#if (CFG_TMR_EN >0 && TIMER_TEST_EN > 0)

/*---------------------------- Macro Define ----------------------------------*/
#define TIMERTEST_TIME (8)

/*---------------------------- Variable Define -------------------------------*/
OS_TCID timer1ID;
OS_EventID newOsTimeMbox;
U32 execTimes;

static char* timer1_getTest(void)
{
	return "Timer, Basic test";
}


static void tmrDoNothing()
{

}

static void tmrOneShotPrecisionTest()
{
	static U64 osTime;
	osTime = CoGetOSTime();
 	isr_PostMail(newOsTimeMbox,(void*)&osTime);
}

static void tmrPeriodicPrecisionTest()
{
	static U64 osTime;
	
	if (execTimes < TIMERTEST_TIME)
	{
	 	osTime = CoGetOSTime();
 		isr_PostMail(newOsTimeMbox,(void*)&osTime);
		execTimes++;
	}


}

static void timer1_execute(void) 
{
	int i;
	U32 curCnt,expectCnt;
	U32 timeCntAcc;
	U64 prevOsTime,curOsTime;
	StatusType err;

	// Test 1: operate on an invaild timer	
	curCnt = CoGetCurTmrCnt(timer1ID,&err);
	testAssert(err == E_INVALID_ID,"#1");
	err = CoSetTmrCnt(timer1ID,0xFF,0xFF);
	testAssert(err == E_INVALID_ID,"#2");

	err = CoStartTmr(timer1ID);
	testAssert(err == E_INVALID_ID,"#3");
	err = CoStopTmr(timer1ID);
	testAssert(err == E_INVALID_ID,"#4");

	err = CoDelTmr(timer1ID);
	testAssert(err == E_INVALID_ID,"#5");

	// Test 2  GetCurTimerCnt & SetTimerCnt
	timer1ID = CoCreateTmr(TMR_TYPE_PERIODIC,0xFF,0,tmrDoNothing);
	testAssert(timer1ID != (OS_TCID)-1,"#6");
	curCnt = CoGetCurTmrCnt(timer1ID,&err);
	testAssert(err == E_OK,"#7");
	testAssert(curCnt == 0xFF,"#8");

	expectCnt = (U32)CoGetOSTime();
	err = CoSetTmrCnt(timer1ID,expectCnt,0);
	testAssert(err == E_OK,"#9");
	curCnt = CoGetCurTmrCnt(timer1ID,&err);
	testAssert(curCnt == expectCnt,"#10");
	err = CoDelTmr(timer1ID);
	testAssert(err == E_OK,"#11");
	curCnt = CoGetCurTmrCnt(timer1ID,&err);
	// DelTimer do not work
	testAssert(err == E_INVALID_ID,"#12");

	// Test 3 TMR_TYPE_ONE_SHOT Precision
	timer1ID = CoCreateTmr(TMR_TYPE_ONE_SHOT,0xFF,0,tmrOneShotPrecisionTest);
	testAssert(timer1ID != (OS_TCID)-1,"#13");

	newOsTimeMbox = CoCreateMbox(EVENT_SORT_TYPE_PRIO);
	testAssert(newOsTimeMbox != (OS_EventID)-1,"#14");
	timeCntAcc = 0;
	prevOsTime = CoGetOSTime();

	CoStartTmr(timer1ID);
	timeCntAcc += *(U64*)CoPendMail(newOsTimeMbox,0,&err) - prevOsTime;

	testAssert(timeCntAcc == 0xFF,"#15");
	CoDelTmr(timer1ID);	

	// Test 4	 TMR_TYPE_PERIODIC Precision
	timer1ID = CoCreateTmr(TMR_TYPE_PERIODIC,0x20,0x20,tmrPeriodicPrecisionTest);
	testAssert(timer1ID != (OS_TCID)-1,"#13");
	timeCntAcc = 0;
	prevOsTime = CoGetOSTime();

	execTimes = 0;
	CoStartTmr(timer1ID);
	for(i = 0; i < TIMERTEST_TIME; i++)
	{
		curOsTime = *(U64*)CoPendMail(newOsTimeMbox,0,&err);
		timeCntAcc += curOsTime - prevOsTime;
		prevOsTime = curOsTime;
	}

	testAssert(timeCntAcc == 0x20*TIMERTEST_TIME,"#14")
	CoDelTmr(timer1ID);

	CoDelMbox(newOsTimeMbox,OPT_DEL_ANYWAY);
}

const TestCase testTimer1 = {
	timer1_getTest,
	NULL,
	NULL,
	timer1_execute
};

/*
 * Test sequence for semaphores pattern.
 */
const TestCase* const patternTimer[] = {
	&testTimer1,
	
	NULL
};
#endif

