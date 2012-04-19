/**
 *******************************************************************************
 * @file        test.h
 * @version     V1.00    Initial version
 * @date        2009.06.29
 * @brief       This header file is part of CooCox OS test.
 *******************************************************************************
 * @copy
 *
 * INTERNAL TEST FILE,DON'T PUBLIC.
 * 
 * <h2><center>&copy; COPYRIGHT 2009 CooCox </center></h2>
 *******************************************************************************
 */
 
 
#ifndef _TEST_H
#define _TEST_H

#include <config.h>

/*---------------------------- Define  ---------------------------------------*/

/*<--------------------------- Test Moudle Config ----------------------------*/
#define TASK_TEST_EN			(1)
#define TIME_TEST_EN			(1) 
#define TIMER_TEST_EN			(1)
#define SEM_TEST_EN				(1)
#define MBOX_TEST_EN			(1)
#define QUEUE_TEST_EN			(1)
#define FLAG_TEST_EN			(1)
#define MUTEX_TEST_EN			(1)
#define MM_TEST_EN				(1)
#define KHEAP_TEST_EN			(1)


#define MAINTEST_PRIMARY_PRIORITY (32)  
#define MAX_TOKENS                (16)  

#define MAX_SLAVE_TEST_TASKS      (10)  /*!< Max tasks	 */
#define SLAVE_TASK_STK_SIZE       (70)  /*!< Stack size	 */

extern OS_TID mainTestTaskID;
extern OS_STK  Task_Stack [MAX_SLAVE_TEST_TASKS][SLAVE_TASK_STK_SIZE];
extern OS_TID  Task_Id [CFG_MAX_USER_TASKS];
/**
 * @struct	 TestCase  test.h  	
 * @brief	 
 * @details	 
 */
typedef struct {
  char *(*getTest)(void);
  void (*setup)(void);
  void (*teardown)(void);
  void (*execute)(void);
}TestCase;

/*---------------------------- Function declare ------------------------------*/
extern void testEmitToken(char token);
extern BOOL _testFail(char * msg);
extern BOOL _testAssert(BOOL condition, char * msg) ;
extern BOOL _testAssertSequence(char *expected);
extern BOOL _testAssertTimeWindow(U64 start, U64 end);

extern void mainTestSetup(void);

#define testFail(msg) {                                       \
  _testFail(msg);                                             \
  return;                                                     \
}

#define testAssert(condition, msg) {                          \
  if (!_testAssert(condition, msg))                           \
    return;                                                   \
}

#define testAssertSequence(expected) {                        \
  if (!_testAssertSequence(expected))                         \
    return;                                                   \
}

#define testAssertTimeWindow(start, end) {                    \
  if (!_testAssertTimeWindow(start, end))                     \
    return;                                                   \
}


/*---------------------------- TestCase declare ------------------------------*/

extern const TestCase* const patternTime[];
extern const TestCase* const patternTask[];
extern const TestCase* const patternTime[];
extern const TestCase* const patternTimer[];
extern const TestCase* const patternSem[];
extern const TestCase* const patternMbox[];
extern const TestCase* const patternQueue[];
extern const TestCase* const patternMutex[];
extern const TestCase* const patternFlag[];
extern const TestCase* const patternMm[];
extern const TestCase* const patternKHeap[];

#endif
