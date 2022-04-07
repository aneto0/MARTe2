/**
 * @file CompilerTypesXil.h
 * @brief Header file for CompilerTypesXil
 * @date 25/08/2021
 * @author Giuseppe Avon
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

 * @details This header file contains the declaration of the CompilerTypes
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef COMPILERTYPESA_H_
#define COMPILERTYPESA_H_

#define GCC_VERSION (__GNUC__ * 10000 \
                     + __GNUC_MINOR__ * 100 \
                     + __GNUC_PATCHLEVEL__)

#include "xil_types.h"

namespace MARTe {

//Warning NULL, TRUE and FALSE are already defined in xil_types.h

/** 64 Bit unsigned integer. */
typedef u64 uint64;
/** 64 Bit signed integer. */
typedef s64 int64;

/** 32 Bit unsigned integer. */
typedef u32 uint32;
/** 32 Bit signed integer. */
typedef s32 int32;

/** 16 Bit unsigned integer. */
typedef u16 uint16;
/** 16 Bit signed integer. */
typedef s16 int16;
/** 8 Bit unsigned integer. */
typedef u8 uint8;

/** 8 Bit signed integer. */
typedef s8 int8;

/** IEEE 754 single precision float */
typedef float float32;
/** IEEE 754 double precision float */
typedef double float64;

/** 8 Bit character */
//Warning - char8 is already defined in xil_types.h
//TODO: check why the already defined char8 type is not propagated across MARTe2
typedef char char8;

/** Sufficiently large to hold a pointer address in the target architecture*/
typedef uintptr_t      uintp;

/** A tool to find indexes of structures fields.
 1024 has been used to avoid alignment problems. */
#define indexof(type,field) ((intptr)&(((type *)1024)->field) - 1024)
/** A tool to find the size of structures fields.
 1024 has been used to avoid alignment problems. */
#define msizeof(type,field) sizeof(((type *)1024)->field)

#define HANDLE void *

#define dll_import
#define dll_export

}

#endif /* COMPILERTYPESA */
