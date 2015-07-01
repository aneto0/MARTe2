/**
 * @file GeneralDefinitions.h
 * @brief Header file for class GeneralDefinitions
 * @date 17/06/2015
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

 * @details This header file contains the declaration of the class GeneralDefinitions
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GENERALDEFINITIONS_H_
#define 		GENERALDEFINITIONS_H_

#define QUOTE(x) QUOTE_1(x)
#define QUOTE_1(x) #x
#define INCLUDE_FILE_ARCHITECTURE(x,y) QUOTE(Architecture/x/y)
#define INCLUDE_FILE_OPERATING_SYSTEM(x,y) QUOTE(OperatingSystem/x/y)

#include INCLUDE_FILE_ARCHITECTURE(ARCHITECTURE,GeneralDefinitionsA.h)
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,GeneralDefinitionsOS.h)

/**
 * Uncomment this section to use memory statistics functions
#ifndef MEMORY_STATISTICS
#define MEMORY_STATISTICS
#endif
*/

/** List of colors */
typedef enum {
    Black = 0,
    Red = 1,
    Green = 2,
    Yellow = 3,
    Blue = 4,
    Purple = 5,
    Cyan = 6,
    White = 7,
    Grey = 8,
    DarkRed = 9,
    DarkGreen = 10,
    DarkYellow = 11,
    DarkBlue = 12,
    DarkPurple = 13,
    DarkCyan = 14,
    DarkGrey = 15

}Colours;

/** values to be used in ErrorManagementFunction */
typedef enum {
    /** Debug Information (should never be found in production code) */
    Debug = 2,
    /** Application Information to be used to understand the working of the code */
    Information = 1,
    /** Application warns of suspicious conditions */
    Warning = 0,
    /** Application reports a fatal error */
    FatalError = -1,
    /** Application reports an error that allows recovery */
    RecoverableError = -2,
    /** Application reports an error during initialization */
    InitialisationError = -3,
    /** Error while calling an operating system function */
    OSError = -4,
    /** Unexpected parameter value that was passed to a function */
    ParametersError = -5,
    /** The operation was illegal in the run time context */
    IllegalOperation = -6,
    /** The operation failed because of a sharing problem */
    ErrorSharing = -7,
    /** The operation failed because of a sharing problem */
    ErrorAccessDenied = -8,
    /** an exception has occurred */
    Exception = -9,
    /** a Timeout has occurred */
    Timeout = -10,
    /** error during a communication */
    CommunicationError = -11,
    /** error while parsing */
    SyntaxError = -12,
    /**something that should be possible but still it is not supported */
    UnsupportedError = -13
} ErrorType;

/**
 * @brief A class that could be used to return the error type in the functions.
 * @details In many functions the user can pass by reference this type of object to know
 * which type of error causes the eventual function's failure. By Default in many functions
 * it is defined a global variable Global::errorType then the user can in any moment
 * access to this variable knowing the type of the last error triggered.
 */
class Error {

public:
    /**
     * @brief Default constructor.
     */
    inline Error() {
        errorType = Debug;
    }

    /**
     * @brief Copy constructor.
     * @param[in] errorArg is the initial value to be assigned to this.
     */
    inline Error(ErrorType errorArg) {
        errorType = errorArg;
    }

    /**
     * @brief Copy operator.
     * @param[in] errorArg is the value to be assigned to this.
     */
    inline void operator=(ErrorType errorArg) {
        errorType = errorArg;
    }

    /**
     * @brief Is equal operator.
     * @param[in] errorArg is the value for the comparison.
     * @return true if errorArg is equal to this.
     */
    inline bool operator==(ErrorType errorArg) const {
        return errorType == errorArg;
    }

    /**
     * @brief Is different operator.
     * @param[in] errorArg is the value for the comparison.
     * @return true if errorArg is different from this.
     */
    inline bool operator!=(ErrorType errorArg) const {
        return errorType != errorArg;
    }

    /**
     * @brief Returns the error type.
     * @return errorType attribute.
     */
    inline ErrorType GetLastError() const {
        return errorType;
    }
private:

    /** The error type. */
    ErrorType errorType;
};

/**
 * @brief Here are defined the global static variables.
 */
namespace Global {
/**
 * Global error static variable.
 */
static Error errorType;
}

/** Large enough to store a pointer*/
#ifdef __LP64__
typedef unsigned long intptr;
#elif defined __ILP64__
typedef unsigned long intptr;
#elif defined __LLP64__
typedef unsigned long long intptr;
#else
typedef unsigned long intptr;
#endif

#ifndef True
/** Portable definition of true. */
#define True   (1==1)
/** Portable definition of false. */
#define False  (1==0)
#endif

/** Builds a 64 bit field with two 32 bit values. */
#define load64(x,a,b)  ((uint32 *)&x)[0] = b; ((uint32 *)&x)[1] = a;

#endif /* GENERALDEFINITIONS_H_ */

