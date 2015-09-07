/**
 * @file ErrorType.h
 * @brief Header file for class Errors
 * @date 06/07/2015
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class Errors
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ERRORS_H_
#define ERRORS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

/**
 * @brief Defines all the framework possible error types.
 */
namespace ErrorManagement {

typedef int8 ErrorType;

/**
 * No error to report
 */
static const ErrorType NoError(0);
/**
 * Debug information
 */
static const ErrorType Debug(3);
/**
 * General information
 */
static const ErrorType Information(2);
/**
 * ErrorManagement::Warning
 */
static const ErrorType Warning(1);
/**
 * Fatal Error
 */
static const ErrorType FatalError(-1);
/**
 * Recoverable error
 */
static const ErrorType RecoverableError(-2);
/**
 * Initialization error
 */
static const ErrorType InitialisationError(-3);
/**
 * Operating system error
 */
static const ErrorType OSError(-4);
/**
 * Parameters error
 */
static const ErrorType ParametersError(-5);
/**
 * Illegal operation
 */
static const ErrorType IllegalOperation(-6);
/**
 * Sharing error
 */
static const ErrorType ErrorSharing(-7);
/**
 * Access denied
 */
static const ErrorType ErrorAccessDenied(-8);
/**
 * Exception
 */
static const ErrorType Exception(-9);
/**
 * Timeout occurred
 */
static const ErrorType Timeout(-10);
/**
 * Error during comunication
 */
static const ErrorType CommunicationError(-11);
/**
 * Syntax error
 */
static const ErrorType SyntaxError(-12);
/**
 * Unsupported feature
 */
static const ErrorType UnsupportedFeature(-13);
}

#endif /* ERRORS_H_ */
