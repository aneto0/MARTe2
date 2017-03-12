/**
 * @file AdvancedErrorManagement.h
 * @brief Header file for class AdvancedErrorManagement
 * @date 21/10/2015
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

 * @details This header file contains the declaration of the class AdvancedErrorManagement
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ADVANCEDERRORMANAGEMENT_H_
#define ADVANCEDERRORMANAGEMENT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "CompilerTypes.h"
#include "ErrorManagement.h"
#include "StreamMemoryReference.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * Maximum size for error messages.
 */
static const uint32 MAX_ERROR_MESSAGE_SIZE = 200u;

/**
 * @brief Report an error based on code, message, and a open list of extra
 * parameters. TODO REDO DOCUMENTATION
 */
//This should not be possible. Minimum parameters are code and message
#define REPORT_ERROR_STATIC_U(code)                                           \
    char8 buffer[MAX_ERROR_MESSAGE_SIZE+1u];                                                 \
    StreamMemoryReference smr(&buffer[0],MAX_ERROR_MESSAGE_SIZE);                               \
    (void) (smr.Printf(reinterpret_cast<const char8 *>(message),par1));                       \
    buffer[smr.Size()]='\0';                                                               \
    ErrorManagement::ReportError(code,&buffer[0], NULL_PTR(const char8* ), NULL_PTR(const char8* ), NULL_PTR(const void* ), __FILE__,__LINE__,__ERROR_FUNCTION_NAME__)

#define REPORT_ERROR_STATIC_PARAMETERS(code, message,...)                                           \
    MARTe::char8 buffer[MARTe::MAX_ERROR_MESSAGE_SIZE+1u];                                                 \
    MARTe::StreamMemoryReference smr(&buffer[0],MARTe::MAX_ERROR_MESSAGE_SIZE);                               \
    (void) (smr.Printf(reinterpret_cast<const MARTe::char8 *>(message),__VA_ARGS__));                        \
    buffer[smr.Size()]='\0';                                                               \
    MARTe::ErrorManagement::ReportError(code,&buffer[0], NULL_PTR(const MARTe::char8* ), NULL_PTR(const MARTe::char8* ), NULL_PTR(const void* ), __FILE__,__LINE__,__ERROR_FUNCTION_NAME__);

#define REPORT_ERROR_STATIC_MACRO_CHOOSER(_U, _0, _1, _2, _3, _4, _5, NAME, ...) NAME

#define REPORT_ERROR_STATIC(...) REPORT_ERROR_STATIC_MACRO_CHOOSER(__VA_ARGS__, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_0, REPORT_ERROR_STATIC_U)( __VA_ARGS__)

/**
 * @brief Report an error based on code, message, and a open list of extra
 * parameters.
 */
//TODO REDO DOCUMENTATION OF THIS FILE! AND TEST!
//This should not be possible. Minimum parameters are code and message
#define REPORT_ERROR_U(code)                                           \
    char8 buffer[MAX_ERROR_MESSAGE_SIZE+1u];                                                 \
    StreamMemoryReference smr(&buffer[0],MAX_ERROR_MESSAGE_SIZE);                               \
    (void) (smr.Printf(reinterpret_cast<const char8 *>(message),par1));                       \
    buffer[smr.Size()]='\0';                                                               \
    ErrorManagement::ReportError(code,&buffer[0], GetClassProperties()->GetName(), GetName(), this, __FILE__,__LINE__,__ERROR_FUNCTION_NAME__)

#define REPORT_ERROR_0(code, message)                                           \
        MARTe::ErrorManagement::ReportError(code, message, GetClassProperties()->GetName(), GetName(), this, __FILE__,__LINE__,__ERROR_FUNCTION_NAME__)

#define REPORT_ERROR_PARAMETERS(code, message,...)                                           \
    MARTe::char8 buffer[MARTe::MAX_ERROR_MESSAGE_SIZE+1u];                                                 \
    MARTe::StreamMemoryReference smr(&buffer[0],MARTe::MAX_ERROR_MESSAGE_SIZE);                               \
    (void) (smr.Printf(reinterpret_cast<const MARTe::char8 *>(message),__VA_ARGS__));                        \
    buffer[smr.Size()]='\0';                                                               \
    MARTe::ErrorManagement::ReportError(code,&buffer[0], GetClassProperties()->GetName(), GetName(), this, __FILE__,__LINE__,__ERROR_FUNCTION_NAME__);

#define REPORT_ERROR_MACRO_CHOOSER(_U, _0, _1, _2, _3, _4, _5, NAME, ...) NAME

#define REPORT_ERROR(...) REPORT_ERROR_MACRO_CHOOSER(__VA_ARGS__, REPORT_ERROR_PARAMETERS, REPORT_ERROR_PARAMETERS, REPORT_ERROR_PARAMETERS, REPORT_ERROR_PARAMETERS, REPORT_ERROR_PARAMETERS, REPORT_ERROR_0, REPORT_ERROR_U)( __VA_ARGS__)

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ADVANCEDERRORMANAGEMENT_H_ */

