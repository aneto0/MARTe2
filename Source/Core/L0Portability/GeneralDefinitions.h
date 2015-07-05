/**
 * @file GeneralDefinitions.h
 * @brief Header file for class GeneralDefinitions
 * @date 17/06/2015
 * @author Giuseppe Ferr�
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
#define GENERALDEFINITIONS_H_

/*lint -save -e9026, function-like macro defined */
#define QUOTE(x) QUOTE_1(x)
/*lint -restore */
/*lint -save -e9026 -e9024, function-like macro defined, '#/##' operators used in macro */
#define QUOTE_1(x) #x
/*lint -restore */
/*lint -save -e9026 -estring(1960, *16-0-6*) , function-like macro defined, unparenthesized macro parameter*/
#define INCLUDE_FILE_ARCHITECTURE(x,y) QUOTE(Architecture/x/y)
#define INCLUDE_FILE_OPERATING_SYSTEM(x,y) QUOTE(OperatingSystem/x/y)
/*lint -restore */

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
 * TODO
 */

class Flags {
public:
    Flags() {
        mask = 0u;
    }

    explicit Flags(const uint32 m) :
            mask(m) {
    }

    inline uint32 GetMask() const {
        return mask;
    }

    inline operator uint32() const {
        return mask;
    }

    inline Flags &operator=(const Flags &f) {
        if (this != &f) {
            mask = f.GetMask();
        }
        return *this;
    }

private:
    uint32 mask;
};

/**
 * @brief A structure containing the time stamp informations.
 */
struct TimeValues {

    /** nanoseconds 0-999999 */
    uint32 microseconds;

    /** seconds 0-59 */
    uint32 seconds;

    /** minutes 0-59 */
    uint32 minutes;

    /** hours 0-23 */
    uint32 hours;

    /** days 1-31 */
    uint32 days;

    /** month 0-11 */
    uint32 month;

    /** year since 1900 */
    uint32 year;
};

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
    inline Error(const ErrorType errorArg) {
        errorType = errorArg;
    }

    /**
     * @brief Copy operator.
     * @param[in] errorArg is the value to be assigned to this.
     */
    inline void operator=(const ErrorType errorArg) {
        errorType = errorArg;
    }

    /**
     * @brief Copy assignment operator
     * @param source the Error object to copy from.
     * @return an Error object with the same errorType as the source object.
     */
    Error& operator=(const Error &source) {
        if (&source != this) {
            errorType = source.errorType;
        }
        return *this;
    }

    /**
     * @brief Is equal operator.
     * @param[in] errorArg is the value for the comparison.
     * @return true if errorArg is equal to this.
     */
    inline bool operator==(const ErrorType errorArg) const {
        return errorType == errorArg;
    }

    /**
     * @brief Is different operator.
     * @param[in] errorArg is the value for the comparison.
     * @return true if errorArg is different from this.
     */
    inline bool operator!=(const ErrorType errorArg) const {
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

#endif /* GENERALDEFINITIONS_H_ */

