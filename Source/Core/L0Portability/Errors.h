/**
 * @file Errors.h
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
#include "FlagsType.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
/**
 * @brief Defines all the framework possible error types.
 */
struct Errors {
    /**
     * Debug Information (should never be found in production code)
     */
    static const FlagsType Debug;
    /**
     * Application standard information 
     */
    static const FlagsType Information;
    /**
     * Application warns of suspicious conditions
     */
    static const FlagsType Warning;
    /**
     * Application reports a fatal error
     */
    static const FlagsType FatalError;
    /**
     * Application reports an error that allows recovery
     */
    static const FlagsType RecoverableError;
    /**
     * Application reports an error during initialization
     */
    static const FlagsType InitialisationError;
    /**
     * Error while calling an operating system function
     */
    static const FlagsType OSError;
    /**
     * Unexpected parameter value that was passed to a function
     */
    static const FlagsType ParametersError;
    /**
     * The operation was illegal in the run time context
     */
    static const FlagsType IllegalOperation;
    /**
     * The operation failed because of a sharing problem
     */
    static const FlagsType ErrorSharing;
    /**
     * The operation failed because of a sharing problem
     */
    static const FlagsType ErrorAccessDenied;
    /**
     * An exception has occurred
     */
    static const FlagsType Exception;
    /**
     * A timeout has occurred
     */
    static const FlagsType Timeout;
    /**
     * Error during a communication
     */
    static const FlagsType CommunicationError;
    /**
     * Error while parsing
     */
    static const FlagsType SyntaxError;
    /**
     * Something that should be possible but still it is not supported
     */
    static const FlagsType UnsupportedError;
};

#endif /* ERRORS_H_ */

