Test Suite
Description
Several test applications link a set of software modules (test suite) in order to verify the proper working of the kernel.
Each Test Module performs a series of tests on a specified subsystem or subsystems and can report a failure/success status and/or a performance index (maybe some other information) as the test suite output. 
The test suite output is usually sent through a serial port and can be examined by using a terminal emulator program. 
Test Module
  *Tasks and Scheduler test
  *Time Delay Test
  *Software Timer Test
  *Semaphores Test
  *Mailboxes Test
  *Queues Test
  *Event Flags Test
  *Mutexes Test
  *Memory Partition Test
  *Memory Heap Test
Preconditions
  *The macro CFG_MAX_USER_TASKS should be set to an appropriate value, and must be large enough.
  *Config the Test Module macro to enable/disable special module in \test\test.h.
  *Redirect the function printf to uartx on the target board. 
How to
  Usually our test projects just do the test work. We call the test suite through calling mainTestSetup() after the call of CoOsInit() in main function. If there is a error in a test module, then the module return, jump to the next. Some error information, a performance index for example will be print on the uart0. A success status will be reported when all the test modules pass.

------------------------------------------------------------------------------- 
Tasks and Scheduler test
Description
  This module implements the test sequence for the Task and Scheduler subsystem.
Preconditions
  *TASK_TEST_EN 1
Test Cases
  *Basic Test
    1. Invalid parameters test.
    2. Basic use of all the APIs under the subsystem. 
Time Delay Test
Description
  This module implements the test sequence for the Time Delay subsystem.
Preconditions
  *CFG_TIME_DELAY_EN 1
  *CFG_UTILITY_EN 1
  *CFG_TICK_TO_TIME_EN 1
  *CFG_TIME_TO_TICK_EN 1
  *TIME_TEST_EN 1 
Test Cases
  *Basic Test
    1. Delay accuracy test of TimeDelay & TickDelay¡£
    2. Basic use of all the APIs under the subsystem¡£
    3. Invalid parameters test.
------------------------------------------------------------------------------- 
Software Timer Test
Description
  This module implements the test sequence for the Software Timer subsystem.
Preconditions
  *CFG_TMR_EN 1
  *CFG_MAX_TMR >= 1
  *TIMER_TEST_EN 1
Test Cases
  *Basic Test
    1. Invalid parameters test.
    2. Reload type timer¡¯s accuracy test.
    3. One shot type timer¡¯s accuracy test.
    4. Basic use of all the APIs under the subsystem.
------------------------------------------------------------------------------- 
Semaphores Test
Description
  This module implements the test sequence for the Semaphore subsystem.
Preconditions
  *CFG_EVENT_EN 1
  *CFG_EVENT_SORT 3
  *CFG_SEM_EN 1
  *SEM_TEST_EN
Test Cases
  *Basic &Timeout Test
    1. Invalid parameters test.
    2. Basic use of all the APIs under the subsystem.
    3. Timeout¡¯s accuracy test.
  *FIFO Enqueuing Test
  *Priority Preemptive Enqueuing Test
------------------------------------------------------------------------------- 
Mailboxes Test
Description
  This module implements the test sequence for the Mailbox subsystem.
Preconditions
  *CFG_EVENT_EN 1
  *CFG_EVENT_SORT 3
  *CFG_MAILBOX_EN 1
  *MBOX_TEST_EN 1
Test Cases
  *Basic &Timeout Test
    1. Invalid parameters test.
    2. Basic use of all the APIs under the subsystem.
    3. Timeout¡¯s accuracy test.
  *FIFO Enqueuing Test
  *Priority Preemptive Enqueuing Test
------------------------------------------------------------------------------- 
Queues Test
Description
  This module implements the test sequence for the Queue subsystem.
Preconditions
  *CFG_EVENT_EN 1
  *CFG_EVENT_SORT 3
  *CFG_QUEUE_EN 1
  *QUEUE_TEST_EN
Test Cases
  *Basic &Timeout Test
    1. Invalid parameters test.
    2. Basic use of all the APIs under the subsystem.
    3. Timeout¡¯s accuracy test.
  *FIFO Enqueuing Test
  *Priority Preemptive Enqueuing Test
------------------------------------------------------------------------------- 
Event Flags Test
Description
  This module implements the test sequence for the Event Flag subsystem.
Preconditions
  *CFG_FLAG_EN 1
  *FLAG_TEST_EN 1
Test Cases
  *Basic Test
    1. Invalid parameters test.
    2. Basic use of all the APIs under the subsystem.
 
Mutexes Test
Description
  This module implements the test sequence for the Mutex subsystem.
Preconditions
  *CFG_MUTEX_EN 1
  *CFG_MAX_MUTEX 10
  *MUTEX_TEST_EN 1
Test Cases
  *Basic Test
    1. Invalid parameters test.
    2. Basic use of all the APIs under the subsystem.
------------------------------------------------------------------------------- 
Memory Partition Test
Description
  This module implements the test sequence for the Memory Partition subsystem.
Preconditions
  *CFG_MM_EN 1
  *CFG_MAX_MM > =1
  *MM_TEST_EN 1
Test Cases
  *Basic Test
    1. Invalid parameters test.
    2. Basic use of all the APIs under the subsystem.
 -------------------------------------------------------------------------------
Memory Heap Test
Description
  This module implements the test sequence for the Memory Heap subsystem.
Preconditions
  *CFG_KHEAP_EN 1
  *KHEAP_SIZE 100
  *KHEAP_TEST_EN
Test Cases
  *Basic Test
    1. Invalid parameters test.
    2. Basic use of all the APIs under the subsystem.

