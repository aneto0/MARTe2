/**
 * @file GeneralDefinitions.h
 * @brief Header file for class GeneralDefinitions
 * @date 17/06/2015
 * @author Giuseppe Ferr�
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


#include <pthread.h>
#ifndef NULL
#define NULL 0
#endif


namespace MARTe{

typedef pthread_t ThreadIdentifier;
static const ThreadIdentifier InvalidThreadIdentifier = static_cast<ThreadIdentifier>(0);

/** Defines the console handle. */
typedef struct termio ConsoleHandle;

/** Threads database memory granularity */
const uint32 THREADS_DATABASE_GRANULARITY = 64u;

/** Defines the default stack size for a thread. */
const uint32 THREADS_DEFAULT_STACKSIZE = 262144u;

/** Defines the maximum number of elements in the memory database */
const uint32 MAX_NO_OF_MEMORY_MONITORS = 64u;

/** In windows the newline is specified with \r\n */
const uint32 N_CHARS_NEWLINE = 1u;

/** Maximum number of global databases */
const uint32 NUMBER_OF_GLOBAL_OBJECTS = 64u;

/** This is just to avoid an exception to Rule 3-9-2 of MISRA, given that many Linux operating system calls require a long as an input
 * and the framework does not define such type*/
#ifdef __LP64__
typedef long oslong;
typedef unsigned long osulong;
#else
typedef long oslong;
typedef size_t osulong;
#endif

#define __ERROR_FUNCTION_NAME__   __PRETTY_FUNCTION__

/**
 * Know shared library extensions
 */
static const char8 * const operatingSystemDLLExtensions[] = { ".gam", ".drv", ".so", static_cast<char8 *>(NULL) };

struct MutexSemProperties;
#define MutexSemHandle MutexSemProperties*

struct EventSemProperties;
#define EventSemHandle EventSemProperties*

struct BasicConsoleProperties;
#define BasicConsoleHandle BasicConsoleProperties*

typedef int32 Handle;
static const char8 DIRECTORY_SEPARATOR = '/';
const uint32 SCHED_GRANULARITY_US = 10000u;

}


#endif /* GENERALDEFINITIONSOS_H_ */

