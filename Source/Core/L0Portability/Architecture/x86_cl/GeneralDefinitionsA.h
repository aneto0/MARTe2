/*
 * Copyright 2015 F4E | European Joint Undertaking for 
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
 will be approved by the European Commission - subsequent  
 versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
 Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
 writing, software distributed under the Licence is 
 distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
 express or implied. 
 * See the Licence  
 permissions and limitations under the Licence. 
 *
 * $Id: Endianity.h 3 2012-01-15 16:26:07Z aneto $
 *
 **/
/** 
 * @file
 * @brief Definition of the basic properties and types.
 */

#ifndef GENERAL_DEFINITIONS_P_H
#define GENERAL_DEFINITIONS_P_H

#include <windows.h>

/** 64 Bit unsigned integer. */
typedef unsigned _int64 uint64;
/** 64 Bit signed integer. */
typedef _int64 int64;

/** 32 Bit unsigned integer. */
typedef unsigned int uint32;
/** 32 Bit signed integer. */
typedef signed int int32;

/** 16 Bit unsigned integer. */
typedef unsigned short uint16;
/** 16 Bit signed integer. */
typedef signed short int16;
/** 8 Bit unsigned integer. */
typedef unsigned char uint8;
/** 8 Bit signed integer. */
typedef signed char int8;

/** A tool to find indexes of structures fields. */
#define indexof(type,field) ((intptr)&(((type *)0)->field))
/** A tool to find the size of structures fields. */
#define msizeof(type,field) sizeof(((type *)0)->field)

#define HANDLE void *

#endif

