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

/**
TODO
 * to allow manage bit size of error type
 */
typedef uint32 ErrorIntegerFormat ;

/**
TODO
 * to allow manage bit size of error type
 */
static const uint32 errorIntegerFormatBitSize(sizeof(ErrorIntegerFormat)*8);

/**
 * Bit for Fatal Error (was called funcvtionError) Generic unspecified error. Used when a simple GO no GO
 */
static const uint32 fatalErrorBit(0u);

/**
 * Bit for Recoverable error
 */
static const uint32 recoverableErrorBit(1u);

/**
 * Bit for Initialisation error
 */
static const uint32 initialisationErrorBit(2u);

/**
 * Bit for Operating system error
 */
static const uint32 OSErrorBit(3u);

/**
 * Parameters error
 */
static const uint32 parametersErrorBit(4u);

/**
 * Illegal operation
 */
static const uint32 illegalOperationBit(5u);

/**
 * Sharing error
 */
static const uint32 errorSharingBit(6u);

/**
 * Access denied
 */
static const uint32 errorAccessDeniedBit(7u);

/**
 * Exception
 */
static const uint32 exceptionBit(8u);

/**
 * Timeout occurred
 */
static const uint32 timeoutBit(9u);

/**
 * Error during communication
 */
static const uint32 communicationErrorBit(10u);

/**
 * Syntax error
 */
static const uint32 syntaxErrorBit(11u);

/**
 * Unsupported feature
 */
static const uint32 unsupportedFeatureBit(12u);

/**
 * Internal setup error
 */
static const uint32 internalSetupErrorBit(13u);

/**
 * Bit for Debug information
 */
static const uint32 debugBit(14u);

/**
 * Bit for General information
 */
static const uint32 informationBit(15u);

/**
 * Bit for Warning
 */
static const uint32 warningBit(16u);

/**
 * Bit to communicate that a cycling called function has finished and does not need to be called again
 */
static const uint32 completedBit(17u);

/**
 * last used Bit
 */
static const uint32 lastErrorBit(16u);


/**
 * To generate the constants representing a specific error type
 */
#define GENERATE_ERROR_CONSTANTS(errorName)   static const ErrorIntegerFormat errorName =(1 << errorName ## Bit);


/**
 * To generate the constants representing a specific error type
 */
#define GENERATE_ERROR_BITRANGE(errorName)    BitBoolean<ErrorIntegerFormat, errorName ## Bit> errorName;


/**
 * No error to report
 */
static const ErrorIntegerFormat noError = 0u;


/**
 * Fatal Error
 */
GENERATE_ERROR_CONSTANTS(fatalError)

/**
 * Recoverable error
 */
GENERATE_ERROR_CONSTANTS(recoverableError)

/**
 * Initialisation error
 */
GENERATE_ERROR_CONSTANTS(initialisationError)

/**
 * Operating system error
 */
GENERATE_ERROR_CONSTANTS(OSError)

/**
 * Parameters error
 */
GENERATE_ERROR_CONSTANTS(parametersError)

/**
 * Illegal operation
 */
GENERATE_ERROR_CONSTANTS(illegalOperation)

/**
 * Sharing error
 */
GENERATE_ERROR_CONSTANTS(errorSharing)

/**
 * Access denied
 */
GENERATE_ERROR_CONSTANTS(errorAccessDenied);

/**
 * Exception
 */
GENERATE_ERROR_CONSTANTS(exception)

/**
 * Timeout occurred
 */
GENERATE_ERROR_CONSTANTS(timeout)

/**
 * Error during communication
 */
GENERATE_ERROR_CONSTANTS(communicationError)

/**
 * Syntax error
 */
GENERATE_ERROR_CONSTANTS(syntaxError)

/**
 * Unsupported feature
 */
GENERATE_ERROR_CONSTANTS(unsupportedFeature)

/**
 * Internal setup error
 */
GENERATE_ERROR_CONSTANTS(internalSetupError);

/**
 * Debug information
 */
GENERATE_ERROR_CONSTANTS(debug)

/**
 * General information
 */
GENERATE_ERROR_CONSTANTS(information)

/**
 * ErrorManagement::warning
 */
GENERATE_ERROR_CONSTANTS(warning)

/**
 * ErrorManagement::warning
 */
GENERATE_ERROR_CONSTANTS(completed)



/**
 * TODO
 * Provides an intelligent alternative to bool as return type from functions
 * */
class DLL_API ErrorType {
public:
    /**
     * TODO
     * if allOk is false then the error.FatalError is set to false
     * all other errors are always cleared (set to 1)
     * */
    inline ErrorType(bool allOk=true);

    /**
     * TODO
     * */
    inline ErrorType(const ErrorIntegerFormat errorBitSet);

    /**
     * TODO
     * is no error is flagged
     * warnings maybe set
     * */
    inline bool ErrorsCleared() const ;

    /**
     * TODO
     * allows mixing with other booleans
     * */
    inline operator bool() const;

    /**
     * TODO
     * allows using as a mask;
     * */
    inline operator ErrorIntegerFormat() const;

    /**
     * TODO
     * */
    inline bool operator ==(const ErrorIntegerFormat errorBitSet)const;

    /**
     * TODO
     * */
    inline bool operator !=(const ErrorIntegerFormat errorBitSet) const;

    /**
     * TODO
     * */
    inline ErrorIntegerFormat operator =(const ErrorIntegerFormat errorBitSet);

    /**
     * TODO
     * */
    inline void SetError(const ErrorIntegerFormat errorBitSet);

    /**
     * TODO
     * */
    inline void ClearError(const ErrorIntegerFormat errorBitSet);

    /**
     * TODO
     * True if all the bits of errorBitSet are contained in this
     * */
    inline bool Contains(const ErrorIntegerFormat errorBitSet) const;


    /*lint -e{9018} Use of union allows to use this memory to describe both objects and basic types.*/
    union {

        /**
         * The whole set of bits should fits within chosen bit representation (ErrorIntegerFormat)
         */
        ErrorIntegerFormat format_as_integer;

        /**
         * Fatal Error
         */
        GENERATE_ERROR_BITRANGE(fatalError)

        /**
         * Recoverable error
         */
        GENERATE_ERROR_BITRANGE(recoverableError)

        /**
         * Initialization error
         */
        GENERATE_ERROR_BITRANGE(initialisationError)

        /**
         * Operating system error
         */
        GENERATE_ERROR_BITRANGE(OSError)

        /**
         * Parameters error
         */
        GENERATE_ERROR_BITRANGE(parametersError)

        /**
         * Illegal operation
         */
        GENERATE_ERROR_BITRANGE(illegalOperation)

        /**
         * Sharing error
         */
        GENERATE_ERROR_BITRANGE(errorSharing)

        /**
         * Access denied
         */
        GENERATE_ERROR_BITRANGE(errorAccessDenied);

        /**
         * Exception
         */
        GENERATE_ERROR_BITRANGE(exception)

        /**
         * Timeout occurred
         */
        GENERATE_ERROR_BITRANGE(timeout)

        /**
         * Error during communication
         */
        GENERATE_ERROR_BITRANGE(communicationError)

        /**
         * Syntax error
         */
        GENERATE_ERROR_BITRANGE(syntaxError)

        /**
         * Unsupported feature
         */
        GENERATE_ERROR_BITRANGE(unsupportedFeature)

        /**
         * Internal setup error
         */
        GENERATE_ERROR_BITRANGE(internalSetupError);

        /**
         * Debug information
         */
        GENERATE_ERROR_BITRANGE(debug)

        /**
         * General information
         */
        GENERATE_ERROR_BITRANGE(information)

        /**
         * ErrorManagement::warning
         */
        GENERATE_ERROR_BITRANGE(warning)

        /**
         * ErrorManagement::warning
         */
        GENERATE_ERROR_BITRANGE(completed)

        /**
         * unmapped
         * */
        BitRange<ErrorIntegerFormat, lastErrorBit+1, errorIntegerFormatBitSize - lastErrorBit -1 > unmapped;

    };

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

inline ErrorType::ErrorType(const ErrorIntegerFormat errorBitSet) {
    format_as_integer = errorBitSet;
}

inline ErrorType::ErrorType(bool allOk) {
    format_as_integer = noError;
    fatalError = !allOk;
}


inline bool ErrorType::ErrorsCleared() const {
    return (format_as_integer == 0);
}

inline ErrorType::operator bool() const {
    return (format_as_integer == 0);
}

inline ErrorType::operator ErrorIntegerFormat() const{
    return format_as_integer ;
}

inline bool ErrorType::operator ==(const ErrorIntegerFormat errorBitSet) const {
    return format_as_integer == errorBitSet;
}

inline bool ErrorType::operator !=(const ErrorIntegerFormat errorBitSet) const {
    return format_as_integer != errorBitSet;
}

inline ErrorIntegerFormat ErrorType::operator =(const ErrorIntegerFormat errorBitSet){
    format_as_integer = errorBitSet;
    return errorBitSet;
}

inline void ErrorType::SetError(const ErrorIntegerFormat errorBitSet) {
    format_as_integer |= errorBitSet;
}

inline void ErrorType::ClearError(const ErrorIntegerFormat errorBitSet) {
    format_as_integer &= ~errorBitSet;
}

inline bool ErrorType::Contains(const ErrorIntegerFormat errorBitSet)const{
    return   ((format_as_integer & errorBitSet) == errorBitSet);
}


}

}

#endif /* ERRORS_H_ */
