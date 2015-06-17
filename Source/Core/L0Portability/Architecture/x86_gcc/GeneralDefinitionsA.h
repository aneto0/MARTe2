/**
 * @file GeneralDefinitionsA.h
 * @brief Header file for class GeneralDefinitionsA
 * @date 17/06/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class GeneralDefinitionsA
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GENERALDEFINITIONSA_H_
#define 		GENERALDEFINITIONSA_H_

/** 64 Bit unsigned integer. */
typedef unsigned long long uint64;
/** 64 Bit signed integer. */
typedef long long int64;

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

/** A tool to find indexes of structures fields.
 1024 has been used to avoid alignment problems. */
#define indexof(type,field) ((intptr)&(((type *)1024)->field) - 1024)
/** A tool to find the size of structures fields.
 1024 has been used to avoid alignment problems. */
#define msizeof(type,field) sizeof(((type *)1024)->field)

#define HANDLE void *

#endif /* GENERALDEFINITIONSA_H_ */

