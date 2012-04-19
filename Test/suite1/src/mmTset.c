/**
 *******************************************************************************
 * @file        mmTest.c
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

#if (CFG_MM_EN > 0 && MM_TEST_EN > 0)

/*---------------------------- Macro Define ----------------------------------*/
#define MEM_TEST_BUF_SIZE	0x40
#define MEM_TEST_BLOCK_SIZE 0x10
/*---------------------------- Variable Define -------------------------------*/
static U8 memTestBuf[MEM_TEST_BUF_SIZE];

static OS_MMID mmID;

static char* mm1_getTest(void)
{
	return "MemPartition, Basic test";
}

static void mm1_execute(void) 
{
	int i,j;
	StatusType err;
	void* pMem[MEM_TEST_BUF_SIZE/MEM_TEST_BLOCK_SIZE + 1];

	mmID = E_CREATE_FAIL;
	// Test 1 invalid parameter Test
  	mmID = CoCreateMemPartition(NULL,0x4,10);		// Invalid memBuf
	testAssert(mmID == E_CREATE_FAIL,"#1");
												
	mmID = CoCreateMemPartition(memTestBuf,0x3,10);	// Invalid blockSize
	testAssert(mmID == E_CREATE_FAIL,"#2");

	mmID = CoCreateMemPartition(memTestBuf,0,10);	// Invalid blockSize
	testAssert(mmID == E_CREATE_FAIL,"#3");

	err = CoDelMemoryPartition(mmID);				// Invalid MMID
	testAssert(err == E_INVALID_ID,"#4");

	pMem[0] = CoGetMemoryBuffer(mmID);				// Invalid MMID
	testAssert(pMem[0] == NULL,"#5");

	mmID = E_CREATE_FAIL;
	// Test 2 CoCreateMemPartition with valid parameter
	mmID = CoCreateMemPartition(memTestBuf,MEM_TEST_BLOCK_SIZE,MEM_TEST_BUF_SIZE/MEM_TEST_BLOCK_SIZE);
	testAssert((mmID != E_CREATE_FAIL),"#6");
	
	// Test 3 Get a block from the pool
	for (i = 0; i < MEM_TEST_BUF_SIZE/MEM_TEST_BLOCK_SIZE; i++)
	{
		pMem[i] = CoGetMemoryBuffer(mmID);
		testAssert(pMem[i] != NULL,"#7");
		// Use the block
		for(j = 0; j < MEM_TEST_BLOCK_SIZE;j++)
		{
		 	*((U8*)pMem[i] + j) = (U8)j;
		}
	}
	// Get a block again from the Empty pool
	pMem[i] = CoGetMemoryBuffer(mmID);
	testAssert(pMem[i] == NULL,"#8");

	// Test 4 Free a block then get a block
	err = CoFreeMemoryBuffer(mmID,(void*)((U32)pMem[0] + 1));
	testAssert(err == E_INVALID_PARAMETER,"#9.0");
    
	err = CoFreeMemoryBuffer(mmID,pMem[0]);
	testAssert(err == E_OK,"#9");

	// Free the block again
	//err = CoFreeMemoryBuffer(mmID,pMem[0]);
	//testAssert(err == E_INVALID_PARAMETER,"#10");

	pMem[0] = CoGetMemoryBuffer(mmID);
	testAssert(err == E_OK,"#11");
	// Use the block
	for(j = 0; j < MEM_TEST_BLOCK_SIZE;j++)
	{
	 	*((U8*)pMem[0] + j) = (U8)j;
	}

	// Test 5 Del MemoryPartition when some block is not free
	//err = CoDelMemoryPartition(mmID);
	//testAssert(err != E_OK,"#12");

	// Test 6 Del memoryPartition normal
	// free all
	for (i = 0; i < MEM_TEST_BUF_SIZE/MEM_TEST_BLOCK_SIZE; i++)
	{
		err = CoFreeMemoryBuffer(mmID,pMem[i]);
		testAssert(err == E_OK,"#13");
	}
	// Del memoryPartition with all blocks free
	err = CoDelMemoryPartition(mmID);
	testAssert(err == E_OK,"#14");

	// Del memoryPartition again
	err = CoDelMemoryPartition(mmID);
	testAssert(err == E_INVALID_ID,"#15");


}


const TestCase testMm1 = {
	mm1_getTest,
	NULL,
	NULL,
	mm1_execute
};

/*
 * Test sequence for semaphores pattern.
 */
const TestCase* const patternMm[] = {
	&testMm1,
	NULL
};
#endif

