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

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
/**
 * @brief Defines all the framework possible error types.
 */
enum ErrorType {
    /**
     * No error to report
     */
    NoError = 0,
    /**
     * Debug Information (should never be found in production code)
     */
    Debug,
    /**
     * Application standard information 
     */
    Information,
    /**
     * Application warns of suspicious conditions
     */
    Warning,
    /**
     * Application reports a fatal error
     */
    FatalError,
    /**
     * Application reports an error that allows recovery
     */
    RecoverableError,
    /**
     * Application reports an error during initialization
     */
    InitialisationError,
    /**
     * Error while calling an operating system function
     */
    OSError,
    /**
     * Unexpected parameter value that was passed to a function
     */
    ParametersError,
    /**
     * The operation was illegal in the run time context
     */
    IllegalOperation,
    /**
     * The operation failed because of a sharing problem
     */
    ErrorSharing,
    /**
     * The operation failed because of a sharing problem
     */
    ErrorAccessDenied,
    /**
     * An exception has occurred
     */
    Exception,
    /**
     * A timeout has occurred
     */
    Timeout,
    /**
     * Error during a communication
     */
    CommunicationError,
    /**
     * Error while parsing
     */
    SyntaxError,
    /**
     * Something that should be possible but still it is not supported
     */
    UnsupportedFeature
};

#endif /* ERRORS_H_ */
