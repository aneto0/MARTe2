/**
 * @file GeneralDefinitionsOS.h
 * @brief Header file for class GeneralDefinitionsOS
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

 * @details This header file contains the declaration of the class GeneralDefinitionsOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GENERALDEFINITIONSOS_H_
#define GENERALDEFINITIONSOS_H_


#include <pthread.h>
#ifndef LINT
#include <termio.h>
#else
typedef unsigned long int pthread_t;
#endif

#ifndef NULL
#define NULL 0
#endif


typedef pthread_t ThreadIdentifier;
static const ThreadIdentifier InvalidThreadIdentifier = static_cast<ThreadIdentifier>(0);

/** Defines the console handle. */
typedef struct termio ConsoleHandle;

/** Threads database memory granularity */
#define THREADS_DATABASE_GRANULARITY 64u

/** Defines the default stack size for a thread. */
#define THREADS_DEFAULT_STACKSIZE 32768

/** Defines the maximum number of elements in the memory database */
#define MAX_NO_OF_MEMORY_MONITORS 64

/** In windows the newline is specified with \r\n */
#define N_CHARS_NEWLINE 1u

/** This is just to avoid an exception to Rule 3-9-2 of MISRA, given that many Linux operating system calls require a long as an input
 * and the framework does not define such type*/
typedef long oslong;
typedef unsigned long osulong;


#define __DECORATED_FUNCTION_NAME__   __PRETTY_FUNCTION__

/**
 * Know shared library extensions
 */
static const char8 * const operatingSystemDLLExtensions[] = { ".gam", ".drv", ".so", 0 };

struct MutexSemOSProperties;
#define MutexSemHandle MutexSemOSProperties*

struct EventSemOSProperties;
#define EventSemHandle EventSemOSProperties*


#endif /* GENERALDEFINITIONSOS_H_ */

