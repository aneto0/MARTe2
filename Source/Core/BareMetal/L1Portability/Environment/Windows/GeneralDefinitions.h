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

#include <stdint.h>
#include <windows.h>

namespace MARTe{


/** Defines the event handle */
typedef HANDLE HEV;

/** Defines the console handle. */
typedef HANDLE ConsoleHandle;

/** Defines the console informations structure. */
typedef CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo;

/** Defines the TID type */
typedef DWORD ThreadIdentifier;
static const ThreadIdentifier InvalidThreadIdentifier = static_cast<ThreadIdentifier>(0);

/** Maximum number of global databases */
const uint32 NUMBER_OF_GLOBAL_OBJECTS = 64u;

/** Defines the default stack size for a thread. */
#define THREADS_DEFAULT_STACKSIZE 32768u

/** Threads database memory granularity */
#define THREADS_DATABASE_GRANULARITY 64

#define SCHED_GRANULARITY_US 10000u

/** Defines the maximum number of elements in the memory database */
#define MAX_NO_OF_MEMORY_MONITORS 64u

#define MAX_STATICLISTHOLDER_BYTESIZE UINT32_MAX

/** In windows the newline is specified with \r\n */
#define N_CHARS_NEWLINE 2u

typedef long oslong;
typedef size_t osulong;

#define __ERROR_FUNCTION_NAME__   __FUNCDNAME__  //windows

/**
 * Know shared library extensions
 */
//TODO AVON
static const char8 * const operatingSystemDLLExtensions[] = { ".gam", ".drv", ".dll", 0 };

struct MutexSemProperties;
#define MutexSemHandle MutexSemProperties*

struct EventSemProperties;
#define EventSemHandle EventSemProperties*

struct BasicConsoleProperties;
#define BasicConsoleHandle BasicConsoleProperties*

typedef HANDLE Handle;
static const char8 DIRECTORY_SEPARATOR = '\\';

//const uint32 SCHED_GRANULARITY_US = 10000u;
typedef uint32 InternetAddress;

}
#endif /* GENERALDEFINITIONS_ENV_H_ */
