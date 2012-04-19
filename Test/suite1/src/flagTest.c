/**
 *******************************************************************************
 * @file        flagTest.c
 * @version     V1.00    Initial version
 * @date        2009.06.26
 * @brief       This file provides all test functions for flag.
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
#include <CoOS.h>
#include <config.h>

#include "test.h" 

#if (CFG_FLAG_EN > 0 && FLAG_TEST_EN > 0)


/*---------------------------- Variable Define -------------------------------*/
OS_STK    Set_flag_task_stk[SLAVE_TASK_STK_SIZE];
unsigned char Flag_Id[33];
volatile unsigned char ExitFlag = 0;
volatile unsigned char prio = 0;

volatile unsigned char Cnt2 = 0;
volatile unsigned char Cnt3 = 0;

extern volatile unsigned char Cnt;
extern volatile unsigned char Flag [50];


char* flag1_getTest (void)
{
	return "flag test example 1";
}

char* flag2_getTest (void)
{
	return "flag test example 2";
}

char* flag3_getTest (void)
{
	return "flag test example 3";
}		  

char* flag4_getTest (void)
{
	return "flag test example 4";
}

char* flag5_getTest (void)
{
	return "flag test example 5";
}


/**
 * ----------------------------------------------------------------------------
 * @brief       set_flag_task	 
 * @param[in] 	None	 
 * @param[out]  None
 * @retval	    None	 
 * @par Description
 * @details	    This task use to set event flags. 
 * ----------------------------------------------------------------------------
 */
void set_flag_task (void* pdata) 
{
	unsigned char i,err;
	unsigned char cnt = 0;
	
	while (1) {
	    cnt ++;
		if (cnt == 2) {
		  ExitFlag = 1;
		  CoExitTask ();
		}
	
	    for (i=0; i<32; i++) {
		    err = CoSetFlag (Flag_Id[i]);
			if (err != E_OK) {
			    printf ("\r Set Flag[%d] Error. \n", Flag_Id[i]);
			}
		}
	}
}



/**
 * ----------------------------------------------------------------------------
 * @brief       set_flag_task_01	 
 * @param[in] 	None	 
 * @param[out]  None
 * @retval	    None	 
 * @par Description
 * @details	    This task use to set event flags. 
 * ----------------------------------------------------------------------------
 */
void set_flag_task_01 (void* pdata) 
{
	unsigned char i,err;
	unsigned char cnt = 0;
	
	while (1) {
	    cnt ++;
	    for (i=0; i<20; i++) {
		    err = CoSetFlag (Flag_Id[i]);
			if (err != E_OK) {
			    printf ("\r Set Flag[%d] Error. \n", Flag_Id[i]);
			}
			if ((cnt == 2)&&(i==19)) {
		       CoExitTask ();
		    }
			else
			   CoTickDelay (20);
		}
	}
}


/**
 * -------------------------------------------------------------------------------------------
 * @brief       create_flags	 
 * @param[in] 	num		How many flags want to create(MUST BE LESS THAN 32).
 * @param[out]  None
 * @retval	    None	 
 * @par Description
 * @details	    Initial task for testing event flag. 
 * -------------------------------------------------------------------------------------------
 */
void create_flags(unsigned char num)
{
	unsigned char i;
	
	/*!< Create event flags. 	*/
	for (i = 0;i< num; i++) {	 	
	 Flag_Id[i] = CoCreateFlag (1,0);
	 if (i >=32 ) {
	     if (Flag_Id[i] != E_CREATE_FAIL) {
	         printf ("\r Create flag error: Flag_Id[%2d] = %2d. \n",i,Flag_Id[i]);
	     }
	 }
	 else {
	     if (Flag_Id[i] == E_CREATE_FAIL) {
	         printf ("\r Create flag error: Flag_Id[%2d] = %2d. \n",i,Flag_Id[i]);
	     }
	 }
	}	
}


/**
 * -----------------------------------------------------------------------------
 * @brief       flag_task_01	 
 * @param[in]   pdata    A pointer to parameter passed to task.	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     Flag test task code. 
 * -----------------------------------------------------------------------------
 */
void flag_task_01 (void* pdata)
{
	unsigned char task_para;
	task_para = (unsigned int)pdata;   
	
	for (;;) {
	 while ( CoWaitForSingleFlag (Flag_Id[task_para],0) != E_OK) ;
	
	 //printf ("%2d",task_para);
	 if (Cnt3 <30) {
	    Flag[Cnt3++] = task_para;
	 } 
	 
	 if (CoDelFlag (Flag_Id[task_para],OPT_DEL_ANYWAY) != E_OK) {
		 printf ("\r Delete flag error: Flag_Id[%2d] \n",task_para);
	 }
	
	 if (task_para == MAX_SLAVE_TEST_TASKS-1 ) {
	  	//printf ("\n\r");
		Cnt++ ;
		Cnt2++;
		if(Cnt2 == 2) {
		   ExitFlag = 1;
		}
		if (Cnt2 == 3)
		   CoExitTask ();
	
	
		if ((Cnt == MAX_SLAVE_TEST_TASKS)) {
		  	create_flags(MAX_SLAVE_TEST_TASKS);
		    Cnt = 0;
		    if ( CoSetFlag (Flag_Id[Cnt]) != E_OK)
		       printf ("\r Error: Set Flag_Id[%d] \n",Cnt);
		}
		else {
		 CoExitTask ();
		}
	 }
	 else {
	   Cnt ++;
	   if ( CoSetFlag (Flag_Id[task_para +1]) != E_OK)
		   printf ("\r Error: Set Flag_Id[%d] \n",task_para);
	   if (ExitFlag == 1) {
	 	  CoExitTask ();
	 }   
	 }
	}
}


/**
 * -----------------------------------------------------------------------------
 * @brief       flag_task_02	 
 * @param[in]   pdata    A pointer to parameter passed to task.	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     Flag test task code. 
 * -----------------------------------------------------------------------------
 */
void flag_task_02 (void* pdata)
{
	unsigned char task_para,err;
	unsigned int flag_wait = 0;
	unsigned int flag_get  = 0;
	task_para = (unsigned int)pdata;  
	
	for (;;) {
	   flag_wait = 0xf<<(task_para*4);
	   flag_get = CoWaitForMultipleFlags (flag_wait,OPT_WAIT_ALL,0,&err);
	   
	   testAssert((flag_wait == flag_get)," Flag #2:Wait flag error. ");
	
	   if (err != E_OK)
	       printf ("\r Error: WaitForMultipleFlags() Task[%2d],code:%d \n",task_para,err);
	   else {
	       //printf ("%2d",task_para); 
		   if (Cnt <8) {
		       Flag[Cnt++] = task_para;
		   }
	   }
	} 
}

/**
 * -----------------------------------------------------------------------------
 * @brief       flag_task_03	 
 * @param[in]   pdata    A pointer to parameter passed to task.	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     Flag test task code. 
 * -----------------------------------------------------------------------------
 */
void flag_task_03 (void* pdata)
{
	unsigned char task_para;
	unsigned char cnt = 0;
	StatusType    err = 0xFF;
	unsigned char num = 0;
	
	task_para = (unsigned int)pdata; 
	
	if (task_para < 9) {
	   err = CoCreateTaskEx (flag_task_03,(void*)(task_para+1),10,&Task_Stack[task_para+1][SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE,2,0);
	   if (err == E_CREATE_FAIL) {
		   printf ("\r Create the %d task fail. \n",task_para+2);
	   }
	}
	
	if (task_para == 0) {
	   err = CoCreateTask (set_flag_task_01,(void *)0,0,&Set_flag_task_stk[SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE);
	   if (err == E_CREATE_FAIL) {
		   printf ("\r Create \"set_flag_task_01\" task fail. \n");
	   }
	
	}
	
	
	for (;;)
	{
	    while ( (CoAcceptSingleFlag (Flag_Id[task_para]) != E_OK) && (CoAcceptSingleFlag (Flag_Id[task_para+10]) !=E_OK) );
	
		num++ ;
		if (num == 2) {
		   	//printf ("%3d",task_para+10);
			if (Cnt3 <40) {
			    Flag [Cnt3++] = (task_para+10);
			}
		    err = CoClearFlag (Flag_Id[task_para+10]);
		    if (err != E_OK) {
			   printf ("\r Clear flag %2d error \n",task_para+10);
			}
		   num = 0;
		}
		else {
		  	//printf ("%3d",task_para);
			if (Cnt3 <40) {
			    Flag [Cnt3++] = task_para;
			}
		    err = CoClearFlag (Flag_Id[task_para]);
		    if (err != E_OK) {
			   printf ("\r Clear flag %2d error \n",task_para);
			}
		}
	
		err = 0xFF;
	
		cnt ++;
	    if (cnt == 4) {
		   ExitFlag = 1;
		   CoExitTask ();
		}	
	} 
}

/**
 * -----------------------------------------------------------------------------
 * @brief       flag1_execute	 
 * @param[in]   None	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     The code of flag test example 1. 
 * -----------------------------------------------------------------------------
 */
 void flag1_execute (void)
{
	unsigned char i,err;
	Cnt  = 0; 
	Cnt2 = 0;
	
	CoTickDelay (1);
	
	/*!< Create event flags. 	*/
	for (i=0; i<33; i++) {	 	
	 Flag_Id[i] = CoCreateFlag (1,0);
	 if (i == 32) {
	     testAssert((Flag_Id[i] == E_CREATE_FAIL)," Flag #1:Create flag error. ");
	 }
	 else {  
		testAssert((Flag_Id[i] != E_CREATE_FAIL)," Flag #1:Create flag error. ");
	  }
	}
	
	/*!< Create tasks			*/
	for (i=0; i<MAX_SLAVE_TEST_TASKS; i++) {
	  Task_Id[i] = CoCreateTaskEx (flag_task_01,(void*)i,MAINTEST_PRIMARY_PRIORITY-1,&Task_Stack[i][SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE,2,0);
	  
	  testAssert((Task_Id[i] != E_CREATE_FAIL)," Flag #1:Create task error. ");
	}
	
	CoSetFlag (Flag_Id[0]);
	
	while (ExitFlag == 0);
	ExitFlag = 0;
	
	
	for (i=0; i<33; i++) {
	
	 err = CoDelFlag (Flag_Id[i],OPT_DEL_ANYWAY);
	 if ((i< MAX_SLAVE_TEST_TASKS)||(i ==32))  {
		 testAssert((err != E_OK)," Flag #1 ");
	 }
	 else {
		  testAssert((err == E_OK)," Flag #1:Delete flag error. ");
	 }
	 Flag_Id[i] = 0xff;
	}
	
	for (i=0; i<30; i++){
	  testAssert((Flag[i] == i%10)," Flag #1: Test logic error. ");
	  Flag[i] = 0;
	}
}


/**
 * -----------------------------------------------------------------------------
 * @brief       flag2_execute	 
 * @param[in]   None	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     The code of flag test example 2. 
 * -----------------------------------------------------------------------------
 */
void flag2_execute (void)
{
	unsigned char i,err;
	Cnt  = 0; 
	Cnt2 = 0;
	
	CoTickDelay (1);
	
	/*!< Create event flags. 	*/
	for (i=0; i<33; i++) {	 	
	 Flag_Id[i] = CoCreateFlag (1,0);
	 if (i == 32){ 
		testAssert((Flag_Id[i] == E_CREATE_FAIL)," Flag #2: Create flag error. ");
	 }
	 else {  
	    testAssert((Flag_Id[i] != E_CREATE_FAIL)," Flag #2: Create flag error. ");
	}
	}
	
	/*!< Create tasks			*/
	for (i=0; i<8; i++) {
	  Task_Id[i] = CoCreateTask (flag_task_02,(void*)i,MAINTEST_PRIMARY_PRIORITY-10+i,&Task_Stack[i][SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE);
	
	  testAssert((Task_Id[i] != E_CREATE_FAIL)," Flag #2: Create task error. ");
	}
	
	err = CoCreateTask (set_flag_task,(void *)0,MAINTEST_PRIMARY_PRIORITY-1,&Set_flag_task_stk[SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE); 
	testAssert((err != E_CREATE_FAIL)," Flag #2:Create task fail. ");
	
	
	while (ExitFlag == 0);
	ExitFlag = 0;
	
	
	for (i=0; i<8; i++) {
	 err = CoDelTask (Task_Id[i]);
	 testAssert((err != E_INVALID_ID)," Flag #2:Delete task fail. ");
	 Task_Id[i] = 0xFF;
	}
	
	for (i=0; i<33; i++) {
	 err = CoDelFlag (Flag_Id[i],OPT_DEL_ANYWAY);
	 if (i>=32)  {
		 testAssert((err != E_OK)," Flag #2:Delete flag fail. ");
	 }
	 else {
		 testAssert((err == E_OK)," Flag #2:Delete flag fail. ");
	 }
	 Flag_Id[i] = 0xff;
	}
	
	for (i=0;i<8;i++) {
	 testAssert((Flag[i] == i)," Flag #2: Test Error ");
	 Flag [i] = 0;
	}

}


/**
 * -----------------------------------------------------------------------------
 * @brief       flag3_execute	 
 * @param[in]   None	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     The code of flag test example 3. 
 * -----------------------------------------------------------------------------
 */
void flag3_execute (void)
{
	unsigned char i,err;
	Cnt = 0;
	Cnt3=0;
	
	CoTickDelay (1);
	
	for (i=0;i<20;i++) {
	  Flag_Id[i] = CoCreateFlag (0,0);
	  testAssert((Flag_Id[i] != E_CREATE_FAIL)," Flag #3:Create flag error. ");
	}
	
	i = 0;
	err = CoCreateTaskEx (flag_task_03,(void*)i,10,&Task_Stack[i][SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE,2,0);
	testAssert((err != E_CREATE_FAIL)," Flag #3:Create flag_task_03 error. ");
	
	while (ExitFlag == 0);
	ExitFlag = 0;
	
	for (i=0; i<20; i++) {
	 err = CoDelFlag (Flag_Id[i],OPT_DEL_ANYWAY);
	 testAssert((err == E_OK)," Flag #3 ");
	
	 Flag_Id[i] = 0xff;
	}
	
	for (i=0; i<40; i++) {
	testAssert((Flag[i] == i%20)," Flag #3:Test logic error. ");
	Flag[i] = 0;
	}

}


/**
 * -----------------------------------------------------------------------------
 * @brief       flag_task_04	 
 * @param[in]   pdata    A pointer to parameter passed to task.	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     Flag test task code. 
 * -----------------------------------------------------------------------------
 */
void flag_task_04 (void* pdata)
{
    unsigned int task_para = (unsigned int)pdata;

    for (;;)
	{
	    CoWaitForSingleFlag (Flag_Id[0],0);

		Flag[task_para] = 1;
		CoExitTask();
	}

}


/**
 * -----------------------------------------------------------------------------
 * @brief       flag4_execute	 
 * @param[in]   None	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     The code of flag test example 4. 
 * -----------------------------------------------------------------------------
 */
void flag4_execute (void)
{
	unsigned char i,err;

	for (i=0;i<4;i++) {
	   Flag[i] = 0;
	}

    Flag_Id[0] = CoCreateFlag (0,0);
	testAssert((Flag_Id[0] != E_CREATE_FAIL)," Flag #4:Create flag error. ");
	
	for (i=0;i<4;i++) {
	  err = CoCreateTask (flag_task_04,(void*)i,10,&Task_Stack[i][SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE);
      testAssert((err != E_CREATE_FAIL)," Flag #4:Create flag_task_04 error. ");
	}


	CoSetFlag (Flag_Id[0]);

	for (i=0;i<4;i++){
       testAssert((Flag[i] == 1)," Flag #4:Test logic error. ");
	   Flag[i] = 0;	   
	}
	
	err = CoDelFlag (Flag_Id[0],OPT_DEL_NO_PEND); 
    testAssert((err == E_OK)," Flag #4:Delete flag error. ");

}



/**
 * -----------------------------------------------------------------------------
 * @brief       flag_task_05	 
 * @param[in]   pdata    A pointer to parameter passed to task.	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     Flag test task code. 
 * -----------------------------------------------------------------------------
 */
void flag_task_05 (void* pdata)
{
    unsigned int task_para = (unsigned int)pdata;

    for (;;)
	{
	    CoWaitForSingleFlag (Flag_Id[0],0);

		Flag[task_para] = 1;
	}

}


/**
 * -----------------------------------------------------------------------------
 * @brief       flag5_execute	 
 * @param[in]   None	
 * @param[out]  None
 * @retval      None	 
 * @par Description
 * @details     The code of flag test example 4. 
 * -----------------------------------------------------------------------------
 */
void flag5_execute (void)
{
	unsigned char i,err;
	unsigned char task_id[4] = {0};

	for (i=0;i<4;i++) {
	   Flag[i] = 0;
	}

    Flag_Id[0] = CoCreateFlag (1,0);
	testAssert((Flag_Id[0] != E_CREATE_FAIL)," Flag #5:Create flag error. ");
	
	for (i=0;i<4;i++) {
	  task_id[i] = CoCreateTask (flag_task_05,(void*)i,i,&Task_Stack[i][SLAVE_TASK_STK_SIZE-1],SLAVE_TASK_STK_SIZE);
      testAssert((task_id[i] != E_CREATE_FAIL)," Flag #5:Create flag_task_05 error. ");
	}

	for (i=0;i<4;i++) {
	    CoSetFlag (Flag_Id[0]);
    }

	for (i=0;i<4;i++){
       testAssert((Flag[i] == 1)," Flag #5:Test logic error. ");
	   Flag[i] = 0;	   
	}

    err = CoDelFlag (Flag_Id[0],OPT_DEL_NO_PEND); 
    testAssert((err == E_TASK_WAITING)," Flag #5:Delete flag error. ");

	for (i=0;i<4;i++){
	   err = CoDelTask (task_id[i]);
	   testAssert((err == E_OK)," Flag #5:Delete flag_task_05 error. ");
	}

}



const TestCase flagTest1 = {
	flag1_getTest,
	NULL,
	NULL,
	flag1_execute
};

const TestCase flagTest2 = {
	flag2_getTest,
	NULL,
	NULL,
	flag2_execute
};

const TestCase flagTest3 = {
	flag3_getTest,
	NULL,
	NULL,
	flag3_execute
};

const TestCase flagTest4 = {
	flag4_getTest,
	NULL,
	NULL,
	flag4_execute
};

const TestCase flagTest5 = {
	flag5_getTest,
	NULL,
	NULL,
	flag5_execute
};
/*
 * Test sequence for flag pattern.
 */
const TestCase* const patternFlag[] = {
	&flagTest1,
	&flagTest2,
	&flagTest3,
	&flagTest4,
	&flagTest5,
	NULL
};


#endif

