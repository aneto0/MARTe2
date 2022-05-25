/**
 * @file GeneralDefinition.h
 * @brief Header file for class GeneralDefinitions
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

 * @details This header file contains the declaration of the class GeneralDefinitions
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GENERALDEFINITIONS_ENV_H_
#define GENERALDEFINITIONS_ENV_H_

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "FreeRTOS.h"
#include "task.h"

#ifndef NULL
#define NULL 0
#endif

#ifndef USE_THREADS_DATABASE
#define USE_THREADS_DATABASE 0
#endif

namespace MARTe{

/**
 * Thread identifier in FreeRTOS
 */
typedef TaskHandle_t ThreadIdentifier;
static const ThreadIdentifier InvalidThreadIdentifier = static_cast<ThreadIdentifier>(0);



/** This is just to avoid an exception to Rule 3-9-2 of MISRA, given that many Linux operating system calls require a long as an input
 * and the framework does not define such type*/
typedef size_t oslong;
typedef size_t osulong;

/** Maximum number of classes that can be registered */
const uint32 NUMBER_OF_GLOBAL_OBJECTS = 256u;

static const char8 * const operatingSystemDLLExtensions[] = { static_cast<char8 *>(NULL) };

struct MutexSemProperties;
#define MutexSemHandle MutexSemProperties*

struct EventSemProperties;
#define EventSemHandle EventSemProperties*

struct BasicConsoleProperties;
#define BasicConsoleHandle BasicConsoleProperties*

typedef int32 Handle;

}

/** Threads database memory granularity */
#ifndef THREADS_DATABASE_GRANULARITY
#define THREADS_DATABASE_GRANULARITY 8u
#endif

/** Defines the default stack size for a thread. */
#define THREADS_DEFAULT_STACKSIZE configMINIMAL_STACK_SIZE * 2

/** Defines the maximum number of elements in the memory database */
#define MAX_NO_OF_MEMORY_MONITORS 8

/** In windows the newline is specified with \r\n */
#define N_CHARS_NEWLINE 2u

/*
inline void* operator new(size_t x) {
    return pvPortMalloc(x);
}

inline void operator delete(void *ptr) {
    vPortFree(ptr);
}
*/
static const MARTe::char8 DIRECTORY_SEPARATOR = '/';

const MARTe::uint32 SCHED_GRANULARITY_US = portTICK_PERIOD_MS * 1000u;

const MARTe::uint32 IOBUFFER_GRANULARITY = 16u;

#endif /* GENERALDEFINITIONS_ENV_H_ */

