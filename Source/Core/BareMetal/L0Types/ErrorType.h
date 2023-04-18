/**
 * @file ErrorType.h
 * @brief Header file for class ErrorType
 * @date 06/07/2015
 * @author Andre Neto
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

#ifndef ERRORTYPE_H_
#define ERRORTYPE_H_

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
 * Defines the error type.
 */
typedef uint32 ErrorIntegerFormat;

/**
 * Defines the error type bit size.
 */
static const uint32 errorIntegerFormatBitSize(sizeof(ErrorIntegerFormat) * 8);

/**
 * Bit for Fatal Error unspecified error.
 */
static const uint32 fatalErrorBit(0u);
static const uint32 FatalErrorBit(fatalErrorBit);

/**
 * Bit for Recoverable error.
 */
static const uint32 recoverableErrorBit(1u);
static const uint32 RecoverableErrorBit(recoverableErrorBit);

/**
 * Bit for Initialisation error.
 */
static const uint32 initialisationErrorBit(2u);
static const uint32 InitialisationErrorBit(initialisationErrorBit);

/**
 * Bit for Operating system error.
 */
static const uint32 OSErrorBit(3u);

/**
 * Parameters error.
 */
static const uint32 parametersErrorBit(4u);
static const uint32 ParametersErrorBit(parametersErrorBit);

/**
 * Illegal operation.
 */
static const uint32 illegalOperationBit(5u);
static const uint32 IllegalOperationBit(illegalOperationBit);

/**
 * Sharing error.
 */
static const uint32 errorSharingBit(6u);
static const uint32 ErrorSharingBit(errorSharingBit);

/**
 * Access denied.
 */
static const uint32 errorAccessDeniedBit(7u);
static const uint32 ErrorAccessDeniedBit(errorAccessDeniedBit);

/**
 * Exception.
 */
static const uint32 exceptionBit(8u);
static const uint32 ExceptionBit(exceptionBit);

/**
 * Timeout occurred.
 */
static const uint32 timeoutBit(9u);
static const uint32 TimeoutBit(timeoutBit);

/**
 * Error during communication.
 */
static const uint32 communicationErrorBit(10u);
static const uint32 CommunicationErrorBit(communicationErrorBit);

/**
 * Syntax error.
 */
static const uint32 syntaxErrorBit(11u);
static const uint32 SyntaxErrorBit(syntaxErrorBit);

/**
 * Unsupported feature.
 */
static const uint32 unsupportedFeatureBit(12u);
static const uint32 UnsupportedFeatureBit(unsupportedFeatureBit);

/**
 * Internal setup error.
 */
static const uint32 internalSetupErrorBit(13u);
static const uint32 InternalSetupErrorBit(internalSetupErrorBit);

/**
 * Bit for Debug information.
 */
static const uint32 debugBit(14u);
static const uint32 DebugBit(debugBit);

/**
 * Bit for General information.
 */
static const uint32 informationBit(15u);
static const uint32 InformationBit(informationBit);

/**
 * Bit for Warning.
 */
static const uint32 warningBit(16u);
static const uint32 WarningBit(warningBit);

/**
 * Bit to communicate that a cycling called function has finished and does not need to be called again.
 */
static const uint32 completedBit(17u);
static const uint32 CompletedBit(completedBit);

/**
 * Bit to communicate that a cycling called function has not finished and needs to be called again.
 */
static const uint32 notCompletedBit(18u);
static const uint32 NotCompletedBit(notCompletedBit);

/**
 * Bit to communicate that an operation yielded a result larger than possible / expected.
 */
static const uint32 overflowBit(19u);
static const uint32 OverflowBit(overflowBit);

/**
 * Bit to communicate that an operation yielded a result smaller than possible / expected.
 */
static const uint32 underflowBit(20u);
static const uint32 UnderflowBit(underflowBit);

/**
 * Bit for OutOfRange error.
 */
static const uint32 outOfRangeBit(21u);
static const uint32 OutOfRangeBit(outOfRangeBit);

/**
 * Last used Bit.
 */
static const uint32 lastErrorBit(22u);
static const uint32 LastErrorBit(lastErrorBit);

/**
 * No error to report.
 */
static const ErrorIntegerFormat NoError = 0u;

/**
 * To generate the constants representing a specific error type.
 */
#define GENERATE_ERROR_CONSTANTS(errorName)   static const ErrorIntegerFormat errorName =(1 << errorName ## Bit);

/**
 * To generate the constants representing a specific error type.
 */
#define GENERATE_ERROR_BITRANGE(errorName)    BitBoolean<ErrorIntegerFormat, errorName ## Bit> errorName;

/**
 * Fatal Error.
 */
GENERATE_ERROR_CONSTANTS(FatalError)

/**
 * Recoverable error.
 */
GENERATE_ERROR_CONSTANTS(RecoverableError)

/**
 * Initialisation error.
 */
GENERATE_ERROR_CONSTANTS(InitialisationError)

/**
 * Operating system error.
 */
GENERATE_ERROR_CONSTANTS(OSError)

/**
 * Parameters error.
 */
GENERATE_ERROR_CONSTANTS(ParametersError)

/**
 * Illegal operation.
 */
GENERATE_ERROR_CONSTANTS(IllegalOperation)

/**
 * Sharing error.
 */
GENERATE_ERROR_CONSTANTS(ErrorSharing)

/**
 * Access denied.
 */
GENERATE_ERROR_CONSTANTS(ErrorAccessDenied);

/**
 * Exception.
 */
GENERATE_ERROR_CONSTANTS(Exception)

/**
 * Timeout occurred.
 */
GENERATE_ERROR_CONSTANTS(Timeout)

/**
 * Error during communication.
 */
GENERATE_ERROR_CONSTANTS(CommunicationError)

/**
 * Syntax error.
 */
GENERATE_ERROR_CONSTANTS(SyntaxError)

/**
 * Unsupported feature.
 */
GENERATE_ERROR_CONSTANTS(UnsupportedFeature)

/**
 * Internal setup error.
 */
GENERATE_ERROR_CONSTANTS(InternalSetupError);

/**
 * Debug information.
 */
GENERATE_ERROR_CONSTANTS(Debug)

/**
 * General information.
 */
GENERATE_ERROR_CONSTANTS(Information)

/**
 * ErrorManagement::Warning.
 */
GENERATE_ERROR_CONSTANTS(Warning)

/**
 * ErrorManagement::Completed.
 */
GENERATE_ERROR_CONSTANTS(Completed)

/**
 * ErrorManagement::NotCompleted.
 */
GENERATE_ERROR_CONSTANTS(NotCompleted)

/**
 * ErrorManagement::Overflow.
 */
GENERATE_ERROR_CONSTANTS(Overflow)

/**
 * ErrorManagement::Underflow.
 */
GENERATE_ERROR_CONSTANTS(Underflow)

/**
 * ErrorManagement::OutOfRange.
 */
GENERATE_ERROR_CONSTANTS(OutOfRange)

/**
 * @brief Provides an alternative to bool as return type from functions, allowing to add extra information.
 */
class DLL_API ErrorType {
public:
    /**
     * @brief Constructor.
     * @param[in] allOk is false then the error.FatalError is set to false, otherwise all other errors cleared.
     *   @post ErrorsCleared() == allOk
     */
    inline ErrorType(bool allOk = true);

    /**
     * @brief Constructor from a bit set.
     * @param[in] errorBitSet initialises the ErrorType against this bit set.
     */
    inline ErrorType(const ErrorIntegerFormat errorBitSet);

    /**
     * @brief Checks if any error has been flagged.
     * @return true is no error is flagged (warnings may have been set).
     */
    inline bool ErrorsCleared() const;

    /**
     * @brief Checks if any error has been flagged.
     * @return true is no error is flagged (warnings may have been set).
     */
    inline operator bool() const;

    /**
     * @brief Allows using as a mask.
     * @return the ErrorType in the ErrorIntegerFormat format.
     */
    inline operator ErrorIntegerFormat() const;

    /**
     * @brief Checks if the current error bits match the provided \a errorBitSet.
     * @param[in] errorBitSet Error bits to verify.
     * @return true if the current error bits match the provided \a errorBitSet.
     */
    inline bool operator ==(const ErrorIntegerFormat errorBitSet) const;

    /**
     * @brief Checks if the current error bits do not match the provided \a errorBitSet.
     * @param[in] errorBitSet Error bits to verify.
     * @return true if the current error bits do not match the provided \a errorBitSet.
     */
    inline bool operator !=(const ErrorIntegerFormat errorBitSet) const;

    /**
     * @brief Constructor from a bit set.
     * @param[in] errorBitSet initialises the ErrorType against this bit set.
     */
    inline ErrorIntegerFormat operator =(const ErrorIntegerFormat errorBitSet);

    /**
     * @brief Constructor from a bit set.
     * @param[in] error initialises the ErrorType against this bit set.
     */
    inline ErrorIntegerFormat operator =(const bool error);

    /**
     * @brief Sets the error bits against a bit set.
     * @param[in] errorBitSet initialises the ErrorType against this bit set.
     */
    inline void SetError(const ErrorIntegerFormat errorBitSet);

    /**
     * @brief Clears the errors associated to the \a errorBitSet .
     * @param[in] errorBitSet bit set with the errors that are to be cleared.
     */
    inline void ClearError(const ErrorIntegerFormat errorBitSet);

    /**
     * @brief Checks if the current error bits contains the provided \a errorBitSet.
     * @param[in] errorBitSet Error bits to verify.
     * @return true if the current error bits contains the provided \a errorBitSet.
     *  If one of the bits in the \a errorBitSet is not set the function will return false.
     */
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
        GENERATE_ERROR_BITRANGE(errorAccessDenied)

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
        GENERATE_ERROR_BITRANGE(internalSetupError)

        /**
         * Debug information
         */
        GENERATE_ERROR_BITRANGE(debug)

        /**
         * General information
         */
        GENERATE_ERROR_BITRANGE(information)

        /**
         * Warning.
         */
        GENERATE_ERROR_BITRANGE(warning)

        /**
         * Operation completed.
         */
        GENERATE_ERROR_BITRANGE(completed)

        /**
         * Operation not completed.
         */
        GENERATE_ERROR_BITRANGE(notCompleted)

        /**
         * Overflow.
         */
        GENERATE_ERROR_BITRANGE(overflow)

        /**
         * Underflow.
         */
        GENERATE_ERROR_BITRANGE(underflow)

        /**
         * OutOfRange.
         */
        GENERATE_ERROR_BITRANGE(outOfRange)

        /**
         * unmapped bits
         */
        BitRange<ErrorIntegerFormat, lastErrorBit+1, errorIntegerFormatBitSize - lastErrorBit -1 > unmapped;

    };

};
}
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace ErrorManagement {

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

inline ErrorType::operator ErrorIntegerFormat() const {
return format_as_integer;
}

inline bool ErrorType::operator ==(const ErrorIntegerFormat errorBitSet) const {
return format_as_integer == errorBitSet;
}

inline bool ErrorType::operator !=(const ErrorIntegerFormat errorBitSet) const {
return format_as_integer != errorBitSet;
}

inline ErrorIntegerFormat ErrorType::operator =(const ErrorIntegerFormat errorBitSet) {
format_as_integer = errorBitSet;
return errorBitSet;
}

inline ErrorIntegerFormat ErrorType::operator =(const bool error) {
    if(error) {
        format_as_integer = 1;
    }
    else {
        format_as_integer = 0;
    }
    return format_as_integer;
}

inline void ErrorType::SetError(const ErrorIntegerFormat errorBitSet) {
format_as_integer |= errorBitSet;
}

inline void ErrorType::ClearError(const ErrorIntegerFormat errorBitSet) {
format_as_integer &= ~errorBitSet;
}

inline bool ErrorType::Contains(const ErrorIntegerFormat errorBitSet) const {
return ((format_as_integer & errorBitSet) == errorBitSet);
}

}

}

#endif /* ERRORTYPE */
