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
#include "CCString.h"

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
 * MACRO to generate different error tables
 * when NotAnErrorCode bit is set the whole bitset is not an error but needs to be interpreted differently
 * the interpretation depends from the context (see genericIterator for instance)
 * FatalError is used as a generic error
 * RecoverableError notifies that also a non fatal error has occurred (used to qualify error messages)
 * Warning notifies that also a warning flag has been set (used to qualify error messages)
 * Information notifies that also some info flag has been set (used to qualify error messages)
 * Completed notifies that also a positive outcome has occurred (all was done)
 * NotCompleted notifies that also not all actions in a list have been done
 * Any other flag represent a specific fatal error
 */
#define ERROR_CONSTANT_MACRO(macrofun) 						      \
		macrofun(FatalError,			fatalError,				0)\
		macrofun(RecoverableError,		recoverableError,		1)\
		macrofun(InitialisationError,	initialisationError,	2)\
		macrofun(OSError,				OSError,				3)\
		macrofun(ParametersError,		parametersError,		4)\
		macrofun(IllegalOperation,		illegalOperation,		5)\
		macrofun(ErrorSharing,			errorSharing,			6)\
		macrofun(ErrorAccessDenied,		errorAccessDenied,		7)\
		macrofun(Exception,				exception,				8)\
		macrofun(Timeout,				timeout,				9)\
		macrofun(CommunicationError,	communicationError,	   10)\
		macrofun(SyntaxError,			syntaxError,		   11)\
		macrofun(UnsupportedFeature,	unsupportedFeature,	   12)\
		macrofun(InternalSetupError,	internalSetupError,	   13)\
		macrofun(Debug,					debug,	               14)\
		macrofun(Information,			information,		   15)\
		macrofun(Warning,				warning,			   16)\
		macrofun(Completed,             completed,			   17)\
		macrofun(NotCompleted,          notCompleted,          18)\
		macrofun(InvalidOperation,      invalidOperation,      19)\
		macrofun(OutOfRange,            outOfRange,            20)\
		macrofun(OutOfMemory,           outOfMemory,           21)\
		macrofun(NotAnErrorCode,        notAnErrorCode,        31)


/**
 * to be kept up to date with the highest value of the error bits
 */
#define lastErrorBit 20

/**
 *  macro function to be used with ERROR_CONSTANT_MACRO to generate macro constants
*/
#define GENERATE_ERROR_CONSTANTS(errorName,dummy,bit)      \
	static const ErrorIntegerFormat errorName =(1 << bit);

/**
 * the macro to signal no error!
 */
static const ErrorIntegerFormat NoError = 0;

/**
 * generates all macro constants
 */
ERROR_CONSTANT_MACRO(GENERATE_ERROR_CONSTANTS)

/**
 * To generate one error bit within ErrorType
 */
#define GENERATE_ERROR_BITRANGE(dummy,errorName,bit)    BitBoolean<ErrorIntegerFormat, bit> errorName;

/**
 * To generate a field in the table of error lookup
 * depends from the ERROR_CONSTANT_MACRO(GENERATE_ERROR_CONSTANTS)
 */
#define GENERATE_ERROR_LOOKUP(ErrorName,errorName,bit)    \
        { #ErrorName, ErrorName},

/**
 * Allow converting errors to strings
 */
struct ErrorTypeLookup{
	/**
	 * The name of the error field
	 */
    CCString 			name;

    /**
     * The corresponding bit
     */
    ErrorIntegerFormat 	errorBitSet;
};

extern DLL_API ErrorTypeLookup errorTypeLookup[];


/**
 * @brief Provides an alternative to bool as return type from functions, allowing to add extra information.
 */
class DLL_API ErrorType {
public:
    /**
     * @brief Constructor.
     * @param[in] allOk is false then the error.FatalError is set , otherwise all other errors cleared.
     *   @post format_as_integer = 0 or FatalError
     */
    inline ErrorType(bool allOk = true);

    /**
     * @brief Constructor from a bit set.
     * @param[in] errorBitSet initialises the ErrorType against this bit set.
     *    @post format_as_integer = errorBitSet.FatalError
     */
    inline ErrorType(const ErrorIntegerFormat errorBitSet);

    /**
     * @brief Checks if any error has been flagged.
     * @return true is no error is flagged or if notAnError is set!
     */
    inline bool ErrorsCleared() const;

    /**
     * @brief Checks if this is an error code.
     * @return true is this is an error code even if containing no error
     */
    inline bool IsErrorCode() const;

    /**
     * @brief Checks if any error has been flagged.
     * @return true is no error is flagged or if notAnError is set.
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
     * @param[in] errorBitSet initialises the ErrorType from a simple boolean error by setting FatalError
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

        // generates all bits
        ERROR_CONSTANT_MACRO(GENERATE_ERROR_BITRANGE)

        /**
         * unmapped bits
         */
//        BitRange<ErrorIntegerFormat, lastErrorBit+1, errorIntegerFormatBitSize - lastErrorBit -1 > unmapped;

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
    if (allOk){
    	format_as_integer = NoError;
    } else {
    	format_as_integer = FatalError;
    }
}

inline bool ErrorType::ErrorsCleared() const {
    return (format_as_integer == 0)  || (format_as_integer & NotAnErrorCode);
}

inline bool ErrorType::IsErrorCode() const {
	return !(format_as_integer & NotAnErrorCode);
}

inline ErrorType::operator bool() const {
    return ErrorsCleared();
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
    if (error){
    	format_as_integer = FatalError;
    } else {
    	format_as_integer = NoError;
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
