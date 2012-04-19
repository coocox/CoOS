/**
 *******************************************************************************
 * @file        kHeapTest.c
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

#if (CFG_KHEAP_EN > 0 && KHEAP_TEST_EN > 0)

/*---------------------------- Macro Define ----------------------------------*/
#define TEST_HEAP_SIZE (KHEAP_SIZE*4)
#define SYS_HEAP_RESVD_SIZE	 20

/*---------------------------- Variable Define -------------------------------*/




static char* kHeap1_getTest(void)
{
	return "Kernel Heap malloc & free, Basic test";
}

void memSet(U32* src, U32 size)
{
	int i;
 	for(i = 0;i < size; i++)
	{
	 	*((U8*)((U32)src + i)) = i;
	}

}
static void kHeap1_execute(void) 
{
	int i;
	void* pMem[5];

	// Test 0 
	pMem[0] = CoKmalloc(0);
	testAssert((pMem[0] == NULL),"#0.0");

	// Test 1 malloc all the heap
	pMem[0] = CoKmalloc(TEST_HEAP_SIZE-SYS_HEAP_RESVD_SIZE);
	testAssert((pMem[0] != NULL),"#0");
	
	memSet(pMem[0],TEST_HEAP_SIZE-SYS_HEAP_RESVD_SIZE);

	// Test 2 free the Mem alloced
	CoKfree(pMem[0]);

	// Can if malloc all the heap successful
	pMem[0] = CoKmalloc(TEST_HEAP_SIZE-SYS_HEAP_RESVD_SIZE);
	testAssert((pMem[0] != NULL),"#1");
	memSet(pMem[0],TEST_HEAP_SIZE-SYS_HEAP_RESVD_SIZE);
	CoKfree(pMem[0]);


	// Test 3 Free in positive sequence
	for(i = 0; i < 5; i++)
	{
		pMem[i] = CoKmalloc((10*i) + 1);
		testAssert((pMem[i] != NULL),"#2");
		//memSet(pMem[i],10*i);
	}


	for(i = 0; i < 5; i++) 
	{
	 	CoKfree(pMem[i]);	
	}
	// Can if malloc all the heap successful(if there is mem fragmentation)
	pMem[0] = CoKmalloc(TEST_HEAP_SIZE-SYS_HEAP_RESVD_SIZE);
	testAssert((pMem[0] != NULL),"#3");	
	memSet(pMem[0],TEST_HEAP_SIZE-SYS_HEAP_RESVD_SIZE);
	CoKfree(pMem[0]);

	// Test 4 Free in inverted sequence
   	for(i = 0; i < 5; i++)
	{
		pMem[i] = CoKmalloc(10*i + 1);
		testAssert((pMem[i] != NULL),"#4");
		memSet(pMem[i],10*i);
	}
	for(i = 0; i < 5; i++) 
	{
	 	CoKfree(pMem[4-i]);	
	}
	// Can if malloc all the heap successful(if there is mem fragmentation)
	pMem[0] = CoKmalloc(TEST_HEAP_SIZE-SYS_HEAP_RESVD_SIZE);
	testAssert((pMem[0] != NULL),"#5");	
	memSet(pMem[0],TEST_HEAP_SIZE-SYS_HEAP_RESVD_SIZE);
	CoKfree(pMem[0]);

	// Test 5 Free in custom sequence
	for(i = 0; i < 5; i++)
	{
		pMem[i] = CoKmalloc(10*i + 1);
		testAssert((pMem[i] != NULL),"#6");
		memSet(pMem[i],10*i);
	}
	// Free the most left block
	CoKfree(pMem[0]);

	// Free the most right block
	CoKfree(pMem[4]);

	// Free the center block
	CoKfree(pMem[2]);

	CoKfree(pMem[3]);
	CoKfree(pMem[1]);

	// Can if malloc all the heap successful(if there is mem fragmentation)
	pMem[0] = CoKmalloc(TEST_HEAP_SIZE-SYS_HEAP_RESVD_SIZE);
	testAssert((pMem[0] != NULL),"#7");	
	memSet(pMem[0],TEST_HEAP_SIZE-SYS_HEAP_RESVD_SIZE);
	CoKfree(pMem[0]);






}


const TestCase testKHeap1 = {
	kHeap1_getTest,
	NULL,
	NULL,
	kHeap1_execute
};

/*
 * Test sequence for semaphores pattern.
 */
const TestCase* const patternKHeap[] = {
	&testKHeap1,
	NULL
};
#endif

