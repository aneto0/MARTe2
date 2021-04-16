/**
 * @file CompilerTypes.h
 * @brief Header file for class CompilerTypes
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

 * @details This header file contains the declaration of the CompilerTypes
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef COMPILERTYPES_H_
#define COMPILERTYPES_H_

/**
 * @brief Quotes an expression after expanding it.
 */
/*lint -save -e9026, function-like macro defined */
#define QUOTE(x) QUOTE_1(x)
/*lint -restore */

/**
 * @brief Quotes an expression without expanding it.
 */
/*lint -save -e9026 -e9024, function-like macro defined, '#/##' operators used in macro */
#define QUOTE_1(x) #x
/*lint -restore */

#ifndef MARTe2_PORTABLE_ENV_DIR
#define MARTe2_PORTABLE_ENV_PARENT_DIR .
#else
#define MARTe2_PORTABLE_ENV_PARENT_DIR MARTe2_PORTABLE_ENV_DIR
#endif

#ifndef MARTe2_PORTABLE_ARCH_DIR
#define MARTe2_PORTABLE_ARCH_PARENT_DIR .
#else
#define MARTe2_PORTABLE_ARCH_PARENT_DIR MARTe2_PORTABLE_ARCH_DIR
#endif


/**
 * @brief Builds an include filename based on actual architecture.
 */
/*p=parent, a=architecture, f=file, t=tier, l=level*/
/*lint -save -e9026 -estring(1960, *16-0-6*) , function-like macro defined, unparenthesized macro parameter*/
#ifndef MARTe2_PORTABLE_ARCH_DIR
#define INCLUDE_FILE_ARCHITECTURE_(t,l,p,a,f) QUOTE(Architecture/a/f)
#else
#define INCLUDE_FILE_ARCHITECTURE_(t,l,p,a,f) QUOTE(p/t/l/Architecture/a/f)
#endif
#define INCLUDE_FILE_ARCHITECTURE(t,l,a,f) INCLUDE_FILE_ARCHITECTURE_(t,l,MARTe2_PORTABLE_ARCH_PARENT_DIR,a,f)
/*lint -restore */

/**
 * @brief Builds an include filename based on actual environment.
 */
/*p=parent, e=environment, f=file, t=tier, l=level*/
/*lint -save -e9026 -estring(1960, *16-0-6*) , function-like macro defined, unparenthesized macro parameter*/
#ifndef MARTe2_PORTABLE_ENV_DIR
#define INCLUDE_FILE_ENVIRONMENT_(t,l,p,e,f) QUOTE(Environment/e/f)
#else
#define INCLUDE_FILE_ENVIRONMENT_(t,l,p,e,f) QUOTE(p/t/l/Environment/e/f)
#endif
#define INCLUDE_FILE_ENVIRONMENT(t,l,e,f) INCLUDE_FILE_ENVIRONMENT_(t,l,MARTe2_PORTABLE_ENV_PARENT_DIR,e,f)
/*lint -restore */
#include INCLUDE_FILE_ARCHITECTURE(BareMetal,L0Types,ARCHITECTURE,CompilerTypes.h)

/**
 * @brief Casts a 0 value to the target pointer type.
 * @details The aim is to reduce the clutter in the code. This avoids
 * replacing ptr = static<MyObject *>(NULL) with ptr = NULL_PTR(MyObject *)
 */
/*lint -save -e9026, function-like macro defined. The aim is to reduce the clutter in the code
 * This avoids replacing ptr = static<MyObject *>(NULL) with ptr = NULL_PTR(MyObject *)*/
#define NULL_PTR(ptr) static_cast<ptr>(0)
/*lint -restore */

/**
 * @brief Makes NULL a synonym of NULL_PTR(void *).
 */
#ifndef NULL
#define NULL NULL_PTR(void *)
#endif

namespace MARTe {

/**
 * @brief Definition of a type large enough to store a pointer.
 */
#ifdef __LP64__
    typedef unsigned long intptr;
#elif defined __ILP64__
    typedef unsigned long intptr;
#elif defined __LLP64__
    typedef unsigned long long intptr;
#else
    typedef unsigned long intptr;
#endif

/**
 * @brief Defines the symbols' direction in DLL as import
 */
#if !defined (dll_import)
#define dll_import
#endif

/**
 * @brief Defines the symbols' direction in DLL as export
 */
#if !defined (dll_import)
#define dll_export
#endif

/**
* @brief Polymorphic macro for defining the actual direction of the
* symbols when using Windows DLLs (import or export)
*/
#if !defined (DLL_API)
#define DLL_API dll_import
#else
#undef DLL_API
#define DLL_API dll_export
#endif

}

#endif /* COMPILERTYPES */
