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
static const uint32 ErrorIntegerFormatBitSize(sizeof(ErrorIntegerFormat)*8);

/**
 * Bit for Fatal Error
 */
static const uint32 FatalErrorBit(0u);

/**
 * Bit for Recoverable error
 */
static const uint32 RecoverableErrorBit(1u);

/**
 * Bit for Initialisation error
 */
static const uint32 InitialisationErrorBit(2u);

/**
 * Bit for Operating system error
 */
static const uint32 OSErrorBit(3u);

/**
 * Parameters error
 */
static const uint32 ParametersErrorBit(4u);

/**
 * Illegal operation
 */
static const uint32 IllegalOperationBit(5u);

/**
 * Sharing error
 */
static const uint32 ErrorSharingBit(6u);

/**
 * Access denied
 */
static const uint32 ErrorAccessDeniedBit(7u);

/**
 * Exception
 */
static const uint32 ExceptionBit(8u);

/**
 * Timeout occurred
 */
static const uint32 TimeoutBit(9u);

/**
 * Error during communication
 */
static const uint32 CommunicationErrorBit(10u);

/**
 * Syntax error
 */
static const uint32 SyntaxErrorBit(11u);

/**
 * Unsupported feature
 */
static const uint32 UnsupportedFeatureBit(12u);

/**
 * Internal setup error
 */
static const uint32 InternalSetupErrorBit(13u);

/**
 * Bit for Debug information
 */
static const uint32 DebugBit(14u);

/**
 * Bit for General information
 */
static const uint32 InformationBit(15u);

/**
 * Bit for Warning
 */
static const uint32 WarningBit(16u);

/**
 * last used Bit
 */
static const uint32 LastErrorBit(16u);


/**
 * To generate the constants representing a specific error type
 */
#define GENERATE_ERROR_CONSTANTS(errorName)   static const ErrorIntegerFormat errorName =(1 >> errorName ## Bit);


/**
 * To generate the constants representing a specific error type
 */
#define GENERATE_ERROR_BITRANGE(errorName,errorNameLC)    BitBoolean<ErrorIntegerFormat, errorName ## Bit> errorNameLC;


/**
 * No error to report
 */
static const ErrorIntegerFormat NoError = 0u;


/**
 * Fatal Error
 */
GENERATE_ERROR_CONSTANTS(FatalError)

/**
 * Recoverable error
 */
GENERATE_ERROR_CONSTANTS(RecoverableError)

/**
 * Initialisation error
 */
GENERATE_ERROR_CONSTANTS(InitialisationError)

/**
 * Operating system error
 */
GENERATE_ERROR_CONSTANTS(OSError)

/**
 * Parameters error
 */
GENERATE_ERROR_CONSTANTS(ParametersError)

/**
 * Illegal operation
 */
GENERATE_ERROR_CONSTANTS(IllegalOperation)

/**
 * Sharing error
 */
GENERATE_ERROR_CONSTANTS(ErrorSharing)

/**
 * Access denied
 */
GENERATE_ERROR_CONSTANTS(ErrorAccessDenied);

/**
 * Exception
 */
GENERATE_ERROR_CONSTANTS(Exception)

/**
 * Timeout occurred
 */
GENERATE_ERROR_CONSTANTS(Timeout)

/**
 * Error during communication
 */
GENERATE_ERROR_CONSTANTS(CommunicationError)

/**
 * Syntax error
 */
GENERATE_ERROR_CONSTANTS(SyntaxError)

/**
 * Unsupported feature
 */
GENERATE_ERROR_CONSTANTS(UnsupportedFeature)

/**
 * Internal setup error
 */
GENERATE_ERROR_CONSTANTS(InternalSetupError);

/**
 * Debug information
 */
GENERATE_ERROR_CONSTANTS(Debug)

/**
 * General information
 */
GENERATE_ERROR_CONSTANTS(Information)

/**
 * ErrorManagement::Warning
 */
GENERATE_ERROR_CONSTANTS(Warning)



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

    /*lint -e{9018} Use of union allows to use this memory to describe both objects and basic types.*/
    union {

        /**
         * The whole set of bits should fits within chosen bit representation (ErrorIntegerFormat)
         */
        ErrorIntegerFormat format_as_integer;

        /**
         * Fatal Error
         */
        GENERATE_ERROR_BITRANGE(FatalError,fatalError)

        /**
         * Recoverable error
         */
        GENERATE_ERROR_BITRANGE(RecoverableError,recoverableError)

        /**
         * Initialization error
         */
        GENERATE_ERROR_BITRANGE(InitialisationError,initialisationError)

        /**
         * Operating system error
         */
        GENERATE_ERROR_BITRANGE(OSError,OSError)

        /**
         * Parameters error
         */
        GENERATE_ERROR_BITRANGE(ParametersError,parametersError)

        /**
         * Illegal operation
         */
        GENERATE_ERROR_BITRANGE(IllegalOperation,illegalOperation)

        /**
         * Sharing error
         */
        GENERATE_ERROR_BITRANGE(ErrorSharing,errorSharing)

        /**
         * Access denied
         */
        GENERATE_ERROR_BITRANGE(ErrorAccessDenied,errorAccessDenied);

        /**
         * Exception
         */
        GENERATE_ERROR_BITRANGE(Exception,exception)

        /**
         * Timeout occurred
         */
        GENERATE_ERROR_BITRANGE(Timeout,timeout)

        /**
         * Error during communication
         */
        GENERATE_ERROR_BITRANGE(CommunicationError,communicationError)

        /**
         * Syntax error
         */
        GENERATE_ERROR_BITRANGE(SyntaxError,syntaxError)

        /**
         * Unsupported feature
         */
        GENERATE_ERROR_BITRANGE(UnsupportedFeature,unsupportedFeature)

        /**
         * Internal setup error
         */
        GENERATE_ERROR_BITRANGE(InternalSetupError,internalSetupError);

        /**
         * Debug information
         */
        GENERATE_ERROR_BITRANGE(Debug,debug)

        /**
         * General information
         */
        GENERATE_ERROR_BITRANGE(Information,information)

        /**
         * ErrorManagement::Warning
         */
        GENERATE_ERROR_BITRANGE(Warning,warning)

        /**
         * unmapped
         * */
        BitRange<ErrorIntegerFormat, LastErrorBit+1, ErrorIntegerFormatBitSize - LastErrorBit -1 > unmapped;

    };

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

inline ErrorType::ErrorType(const ErrorIntegerFormat errorBitSet) {
    format_as_integer = errorBitSet;
}

inline ErrorType::ErrorType(bool allOk) {
    format_as_integer = NoError;
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

}

}

#endif /* ERRORS_H_ */
