/**
 *******************************************************************************
 * @file       core.h
 * @version    V1.13    
 * @date       2010.04.26
 * @brief      Header file	related to kernel	
 *******************************************************************************
 * @copy
 *
 * INTERNAL FILE,DON'T PUBLIC.
 * 
 * <h2><center>&copy; COPYRIGHT 2009 CooCox </center></h2>
 *******************************************************************************
 */ 


#ifndef _CORE_H
#define _CORE_H

#include <CoOS.h>


#define  OsSchedLock()  OSSchedLock++;      /*!< Lock schedule                */
extern   void OsSchedUnlock(void);

#endif

