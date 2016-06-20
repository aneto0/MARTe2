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
#include "CompilerTypes.h"
#include "BitBoolean.h"
#include "BitRange.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

namespace ErrorManagement {

class DLL_API ErrorType {
public:
    /**
     * TODO
     * if allOk is false then the error.functionReturn is set to false
     * all other errors are always cleared (set to 1)
     * */
    inline ErrorType(bool allOk=true);

    inline ErrorType(const uint32 errorCode);

    /**
     * TODO
     * is no error is flagged
     * warnings maybe set
     * */
    inline bool NoError() const ;

    /**
     * TODO
     * allows mixing with other booleans
     * */
    inline operator bool();

    inline bool operator ==(const ErrorType &in)const;

    inline bool operator !=(const ErrorType &in) const;

    inline void SetError(const uint32 mask);

    inline void ClearError(const uint32 mask);

    /*lint -e{9018} Use of union allows to use this memory to describe both objects and basic types.*/
    union {

        /**
         * The whole set of bits fits in a 32 bit unsigned
         */
        uint32 format_as_uint32;

        /**
         * Function fatal error
         */
        BitBoolean<uint32, 0u> functionError;

        /**
         * Recoverable error
         */
        BitBoolean<uint32, 1u> recoverableError;

        /**
         * Initialization error
         */
        BitBoolean<uint32, 2u> initialisationError;

        /**
         * TODO
         * Error due to an operating system error
         */
        BitBoolean<uint32, 3u> OSError;

        /**
         * TODO
         * Parameters error
         * Specific to RPC applications: call parameters incompatibilities for the desired function (notPrototypeMismatch)
         */
        BitBoolean<uint32, 4u> parametersError;

        /**
         * Illegal operation
         */

        BitBoolean<uint32, 5u> illegalOperation;

        /**
         * Sharing error
         */
        BitBoolean<uint32, 6u> errorSharing;

        /**
         * Access denied
         */
        BitBoolean<uint32, 7u> accessDenied;

        /**
         * Exception
         */
        BitBoolean<uint32, 8u> exception;

        /**
         * Timeout occurred
         */
        BitBoolean<uint32, 9u> timeout;

        /**
         * Error during communication
         */
        BitBoolean<uint32, 10u> communicationError;

        /**
         * Syntax error
         */
        BitBoolean<uint32, 11u> syntaxError;

        /**
         * Specific to RPC applications: the desired function was not found (notMethodNotFound)
         * Unsupported feature
         */
        BitBoolean<uint32, 12u> unsupportedFeature;

        /**
         * unexpected inconsistencies in the internal setup of a system/object
         */
        BitBoolean<uint32, 13u> internalSetupError;

        /**
         * Debug alert
         */
        BitBoolean<uint32, 14u> debug;

        /**
         * Generic information
         */
        BitBoolean<uint32, 15u> information;

        /**
         * Warning
         */
        BitBoolean<uint32, 16u> warning;

        /**
         * unmapped
         * */
        BitRange<uint32, 14u, 17u> unmapped;

    };

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

inline ErrorType::ErrorType(bool allOk) {
    format_as_uint32 = 0u;
    functionError = !allOk;
}

inline ErrorType::ErrorType(const uint32 errorCode) {
    format_as_uint32 = errorCode;
}

inline bool ErrorType::NoError() const {
    return (format_as_uint32 == 0);
}

inline ErrorType::operator bool() {
    return NoError();
}

inline bool ErrorType::operator ==(const ErrorType &in) const {
    return format_as_uint32 == in.format_as_uint32;
}

inline bool ErrorType::operator !=(const ErrorType &in) const {
    return format_as_uint32 != in.format_as_uint32;
}

inline void ErrorType::SetError(const uint32 mask) {
    format_as_uint32 |= mask;

}

inline void ErrorType::ClearError(const uint32 mask) {
    format_as_uint32 &= ~mask;
}

/**
 * No error to report
 */
static const ErrorType NoError(0u);
/**
 * Debug information
 */
static const ErrorType Debug(1u << 12);
/**
 * General information
 */
static const ErrorType Information(1u << 13);
/**
 * ErrorManagement::Warning
 */
static const ErrorType Warning(1u << 14);

/**
 * Fatal Error
 */
static const ErrorType FatalError(0x1u);
/**
 * Recoverable error
 */
static const ErrorType RecoverableError(0x2u);
/**
 * Initialization error
 */
static const ErrorType InitialisationError(0x4u);
/**
 * Operating system error
 */
static const ErrorType OSError(0x8u);
/**
 * Parameters error
 */
static const ErrorType ParametersError(1u << 4);
/**
 * Illegal operation
 */
static const ErrorType IllegalOperation(1u << 5);
/**
 * Sharing error
 */
static const ErrorType ErrorSharing(1u << 6);
/**
 * Access denied
 */
static const ErrorType ErrorAccessDenied(1u << 7);
/**
 * Exception
 */
static const ErrorType Exception(1u << 8);
/**
 * Timeout occurred
 */
static const ErrorType Timeout(1u << 9);
/**
 * Error during comunication
 */
static const ErrorType CommunicationError(1u << 10);
/**
 * Syntax error
 */
static const ErrorType SyntaxError(1u << 11);
/**
 * Unsupported feature
 */
static const ErrorType UnsupportedFeature(1u << 12);

/**
 * Internal setup error
 */
static const ErrorType InternalSetupError(1u << 13);

}
}

#endif /* ERRORS_H_ */
