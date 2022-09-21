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
#include "ClassProperties.h"
#include "CompilerTypes.h"
#include "ErrorManagement.h"
#include "StreamMemoryReference.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/*lint -estring(1795, "*REPORT_ERROR_MACRO_CHOOSER*") -estring(1795, "*REPORT_ERROR_STATIC_MACRO_CHOOSER*") -estring(1795, "*REPORT_ERROR_PROXY_MACRO_CHOOSER*") False positives. The macros are used as described below*/
namespace MARTe {
/**
 * Maximum size for error messages.
 */
static const uint32 MAX_ERROR_MESSAGE_SIZE = 200u;

/**
 * @brief The REPORT_ERROR_STATIC_U is not to be used. It serves as the one parameter choice for the REPORT_ERROR_STATIC_MACRO_CHOOSER macro.
 */
#define REPORT_ERROR_STATIC_U(code)                                         \
/*lint -save -e717 Let lint know that we know that we are doing while(0)*/  \
do {                                                                        \
    char8 buffer[MAX_ERROR_MESSAGE_SIZE+1u];                                \
    StreamMemoryReference smr(&buffer[0],MAX_ERROR_MESSAGE_SIZE);           \
    (void) (smr.Printf(reinterpret_cast<const char8 *>(message),par1));     \
    buffer[smr.Size()]='\0';                                                \
    MARTe::ErrorManagement::ReportError(code,&buffer[0], NULL_PTR(const char8* ), NULL_PTR(const char8* ), NULL_PTR(const void* ), __FILE__,__LINE__,__ERROR_FUNCTION_NAME__); \
} while(false) /*lint -restore */ //Protect scope with the {} and force to end with ;
/**
 * @brief The REPORT_ERROR_STATIC_MACRO_CHOOSER will call this function for any call to REPORT_ERROR_STATIC that has more than two parameters (the first two being the log code and the message)
 */
#define REPORT_ERROR_STATIC_PARAMETERS(code, message,...)                              \
/*lint -save -e717 Let lint know that we know that we are doing while(0)*/             \
do {                                                                                   \
    MARTe::char8 buffer[MARTe::MAX_ERROR_MESSAGE_SIZE+1u];                             \
    MARTe::StreamMemoryReference smr(&buffer[0],MARTe::MAX_ERROR_MESSAGE_SIZE);        \
    (void) (smr.Printf(reinterpret_cast<const MARTe::char8 *>(message),__VA_ARGS__));  \
    buffer[smr.Size()]='\0';                                                           \
    MARTe::ErrorManagement::ReportError(code,&buffer[0], NULL_PTR(const MARTe::char8* ), NULL_PTR(const MARTe::char8* ), NULL_PTR(const void* ), __FILE__,__LINE__,__ERROR_FUNCTION_NAME__); \
} while(false) /*lint -restore */ //Protect scope with the {} and force to end with ;

/**
 * @brief A macro trick to "overload" a macro based on the number of parameters. Usually a different macro is associated to a different number of parameters, but in this case the REPORT_ERROR_STATIC_PARAMETERS is always called as the code algorithm does not change with the number of parameters.
 */
#define REPORT_ERROR_STATIC_MACRO_CHOOSER(_U, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME

/**
 * @brief Macro to be called to print logging messages on classes that do not inherit from Object (and that thus do not object name, class name nor object pointer).
 * <pre>
 * param1: code the ErrorType code.
 * param2: message the logging message to be printed.
 * param3: ... the parameters to be sent to a Printf (if any).
 * </pre>
 */
#define REPORT_ERROR_STATIC(...) REPORT_ERROR_STATIC_MACRO_CHOOSER(__VA_ARGS__, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_PARAMETERS, REPORT_ERROR_STATIC_0, REPORT_ERROR_STATIC_U)( __VA_ARGS__)

/**
 * @brief The REPORT_ERROR_U is not to be used. It serves as the one parameter choice for the REPORT_ERROR_MACRO_CHOOSER macro.
 */
#define REPORT_ERROR_U(code)                                                \
/*lint -save -e717 Let lint know that we know that we are doing while(0)*/  \
do {                                                                        \
    char8 buffer[MAX_ERROR_MESSAGE_SIZE+1u];                                \
    StreamMemoryReference smr(&buffer[0],MAX_ERROR_MESSAGE_SIZE);           \
    (void) (smr.Printf(reinterpret_cast<const char8 *>(message),par1));     \
    buffer[smr.Size()]='\0';                                                \
    if (GetClassProperties() != NULL_PTR(const MARTe::ClassProperties *))   \
        MARTe::ErrorManagement::ReportError(code,&buffer[0], GetClassProperties()->GetName(), GetName(), this, __FILE__,__LINE__,__ERROR_FUNCTION_NAME__); \
} while(false) /*lint -restore */ //Protect scope with the {} and force to end with ;
/**
 * @brief The REPORT_ERROR_MACRO_CHOOSER will call this function for any call to REPORT_ERROR that has two and only two parameters (the log code and the message)
 */
#define REPORT_ERROR_0(code, message)                                       \
/*lint -save -e717 Let lint know that we know that we are doing while(0)*/  \
do {                                                                        \
    const MARTe::char8 *pClassName = "Unknown";                             \
    const MARTe::ClassProperties *cProperties = GetClassProperties();       \
    if (cProperties != NULL_PTR(const MARTe::ClassProperties *)) {          \
        pClassName = cProperties->GetName();                                \
    }                                                                       \
    MARTe::ErrorManagement::ReportError(code, message, pClassName, GetName(), this, __FILE__,__LINE__,__ERROR_FUNCTION_NAME__); \
} while(false) /*lint -restore */ //Protect scope with the {} and force to end with ;
/**
 * @brief The REPORT_ERROR_MACRO_CHOOSER will call this function for any call to REPORT_ERROR that has more than two parameters (the first two being the log code and the message)
 */
#define REPORT_ERROR_PARAMETERS(code, message,...)                                     \
/*lint -save -e717 Let lint know that we know that we are doing while(0)*/             \
do {                                                                                   \
    MARTe::char8 buffer[MARTe::MAX_ERROR_MESSAGE_SIZE+1u];                             \
    MARTe::StreamMemoryReference smr(&buffer[0],MARTe::MAX_ERROR_MESSAGE_SIZE);        \
    (void) (smr.Printf(reinterpret_cast<const MARTe::char8 *>(message),__VA_ARGS__));  \
    buffer[smr.Size()]='\0';                                                           \
    const MARTe::char8 *pClassName = "Unknown";                                        \
    const MARTe::ClassProperties *cProperties = GetClassProperties();                  \
    if (cProperties != NULL_PTR(const MARTe::ClassProperties *)) {                     \
        pClassName = cProperties->GetName();                                           \
    }                                                                                  \
    MARTe::ErrorManagement::ReportError(code, &buffer[0], pClassName, GetName(), this, __FILE__,__LINE__,__ERROR_FUNCTION_NAME__); \
} while(false) /*lint -restore */ //Protect scope with the {} and force to end with ;

/**
 * @brief A macro trick to "overload" a macro based on the number of parameters. Usually a different macro is associated to a different number of parameters, but in this case the REPORT_ERROR_MACRO_CHOOSER is always called as the code algorithm does not change with the number of parameters.
 */
#define REPORT_ERROR_MACRO_CHOOSER(_U, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME

/**
 * @brief Macro to be called to print logging messages on classes that inherit from Object (this will automatically add the object name, class name and object pointer to the log message).
 * <pre>
 * param1: code the ErrorType code.
 * param2: message the logging message to be printed.
 * param3: ... the parameters to be sent to a Printf (if any).
 * </pre>
 */
#define REPORT_ERROR(...) REPORT_ERROR_MACRO_CHOOSER(__VA_ARGS__, REPORT_ERROR_PARAMETERS, REPORT_ERROR_PARAMETERS, REPORT_ERROR_PARAMETERS, REPORT_ERROR_PARAMETERS, REPORT_ERROR_PARAMETERS, REPORT_ERROR_PARAMETERS, REPORT_ERROR_PARAMETERS, REPORT_ERROR_PARAMETERS, REPORT_ERROR_PARAMETERS, REPORT_ERROR_PARAMETERS, REPORT_ERROR_0, REPORT_ERROR_U)( __VA_ARGS__)

/**
 * @brief The REPORT_ERROR_PROXY_U is not to be used. It serves as the one parameter choice for the REPORT_ERROR_PROXY_MACRO_CHOOSER macro.
 */
#define REPORT_ERROR_PROXY_U(code)                                          \
/*lint -save -e717 Let lint know that we know that we are doing while(0)*/  \
do {                                                                        \
    char8 buffer[MAX_ERROR_MESSAGE_SIZE+1u];                                \
    StreamMemoryReference smr(&buffer[0],MAX_ERROR_MESSAGE_SIZE);           \
    (void) (smr.Printf(reinterpret_cast<const char8 *>(message),par1));     \
    buffer[smr.Size()]='\0';                                                \
    if (GetClassProperties() != NULL_PTR(const MARTe::ClassProperties *))   \
        MARTe::ErrorManagement::ReportError(code,&buffer[0], GetClassProperties()->GetName(), GetName(), this, __FILE__,__LINE__,__ERROR_FUNCTION_NAME__); \
} while(false) /*lint -restore */ //Protect scope with the {} and force to end with ;
/**
 * @brief The REPORT_ERROR_PROXY_MACRO_CHOOSER will call this function for any call to REPORT_ERROR_PROXY that has two and only two parameters (the log code and the message)
 */
#define REPORT_ERROR_PROXY_0(code, message)                                 \
/*lint -save -e717 Let lint know that we know that we are doing while(0)*/  \
do {                                                                        \
    const MARTe::char8 *pClassName = "Unknown";                             \
    const MARTe::ClassProperties *cProperties = GetClassProperties();       \
    if (cProperties != NULL_PTR(const MARTe::ClassProperties *)) {          \
        pClassName = cProperties->GetName();                                \
    }                                                                       \
    MARTe::ErrorManagement::ReportError(code, message, pClassName, GetName(), this, __FILE__,__LINE__,__ERROR_FUNCTION_NAME__); \
} while(false) /*lint -restore */ //Protect scope with the {} and force to end with ;
/**
 * @brief The REPORT_ERROR_PROXY_MACRO_CHOOSER will call this function for any call to REPORT_ERROR_PROXY that has more than two parameters (the first two being the log code and the message)
 */
#define REPORT_ERROR_PROXY_PARAMETERS(code, ref, message, ...)                          \
/*lint -save -e717 Let lint know that we know that we are doing while(0)*/             \
do {                                                                                   \
    MARTe::char8 buffer[MARTe::MAX_ERROR_MESSAGE_SIZE+1u];                             \
    MARTe::StreamMemoryReference smr(&buffer[0],MARTe::MAX_ERROR_MESSAGE_SIZE);        \
    (void) (smr.Printf(reinterpret_cast<const MARTe::char8 *>(message),__VA_ARGS__));  \
    buffer[smr.Size()]='\0';                                                           \
    const MARTe::char8 *pClassName = "Unknown";                                        \
    const MARTe::ClassProperties *cProperties = ref->GetClassProperties();             \
    if (cProperties != NULL_PTR(const MARTe::ClassProperties *)) {                     \
        pClassName = cProperties->GetName();                                           \
    }                                                                                  \
    MARTe::ErrorManagement::ReportError(code, &buffer[0], pClassName, ref->GetName(), this, __FILE__, __LINE__,__ERROR_FUNCTION_NAME__); \
} while(false) /*lint -restore */ //Protect scope with the {} and force to end with ;

/**
 * @brief A macro trick to "overload" a macro based on the number of parameters. Usually a different macro is associated to a different number of parameters, but in this case the REPORT_ERROR_PROXY_MACRO_CHOOSER is always called as the code algorithm does not change with the number of parameters.
 */
#define REPORT_ERROR_PROXY_MACRO_CHOOSER(_U, _0, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, NAME, ...) NAME

/**
 * @brief Macro to be called to print logging messages on behalf (proxied) classes that inherit from Object (this will automatically add the object name, class name and object pointer to the log message).
 * <pre>
 * param1: code the ErrorType code.
 * param2: message the logging message to be printed.
 * param3: ... the parameters to be sent to a Printf (if any).
 * </pre>
 */
#define REPORT_ERROR_PROXY(...) REPORT_ERROR_PROXY_MACRO_CHOOSER(__VA_ARGS__, REPORT_ERROR_PROXY_PARAMETERS, REPORT_ERROR_PROXY_PARAMETERS, REPORT_ERROR_PROXY_PARAMETERS, REPORT_ERROR_PROXY_PARAMETERS, REPORT_ERROR_PROXY_PARAMETERS, REPORT_ERROR_PROXY_PARAMETERS, REPORT_ERROR_PROXY_PARAMETERS, REPORT_ERROR_PROXY_PARAMETERS, REPORT_ERROR_PROXY_PARAMETERS, REPORT_ERROR_PROXY_PARAMETERS, REPORT_ERROR_PROXY_0, REPORT_ERROR_PROXY_U)( __VA_ARGS__)
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ADVANCEDERRORMANAGEMENT_H_ */
