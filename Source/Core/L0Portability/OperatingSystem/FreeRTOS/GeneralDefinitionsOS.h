/**
 * @file GeneralDefinitionsOS.h
 * @brief Header file for class GeneralDefinitionsOS
 * @date 31/07/2015
 * @author André Neto
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class GeneralDefinitionsOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GENERALDEFINITIONSOS_H_
#define GENERALDEFINITIONSOS_H_

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"


#ifndef NULL
#define NULL 0
#endif

#ifndef USE_THREADS_DATABASE
#define USE_THREADS_DATABASE 0
#endif

typedef SemaphoreHandle_t mutexHandle;

/**
 * Thread identifier in FreeRTOS
 */
typedef TaskHandle_t ThreadIdentifier;
static const ThreadIdentifier InvalidThreadIdentifier = static_cast<ThreadIdentifier>(0);

/** Threads database memory granularity */
#ifndef THREADS_DATABASE_GRANULARITY
#define THREADS_DATABASE_GRANULARITY 8u
#endif

/** Defines the default stack size for a thread. */
#define THREADS_DEFAULT_STACKSIZE configMINIMAL_STACK_SIZE*2

/** Defines the maximum number of elements in the memory database */
#define MAX_NO_OF_MEMORY_MONITORS 8

/** In windows the newline is specified with \r\n */
#define N_CHARS_NEWLINE 1u

inline void* operator new(uint32 x) {
    return pvPortMalloc(x);
}

inline void operator delete(void *ptr) {
    vPortFree(ptr);
}
#endif /* GENERALDEFINITIONSOS_H_ */

