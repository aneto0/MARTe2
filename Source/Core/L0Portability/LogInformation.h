/**
 * @file LogInformation.h
 * @brief Header file for class LogInformation
 * @date 24/08/2015
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

 * @details This header file contains the declaration of the class LogInformation
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LOGINFORMATION_H_
#define LOGINFORMATION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace Logger {

/**
 @brief Information structure associated to the error.
 */
struct LogInformation {

    /**
     * Definition of the header.
     */
    struct {

        /**
         * The error code.
         */
        ErrorType errorType :8;

        /**
         * The error line number.
         */
        uint16 lineNumber;

        /**
         * Specified is the error is triggered within an object.
         */
        bool isObject :1;

    } header;

    /**
     *  High resolution timer ticks.
     */
    int64 hrtTime;

    /**
     * The error file name.
     */
    const char8 * fileName;

    /**
     * The error function name
     */
    const char8 * functionName;

    /**
     * thread ID of the threads who generate the error.
     */
    ThreadIdentifier threadId;

    /**
     * The Address of the object that produced the error.
     * Object may be temporary in memory because the
     * objectPointer will only be printed, not used
     */
    void * objectPointer;

    /**
     * A pointer to a const char * which is persistent
     * so a real constant, not a char * relabeled as const char *
     * scope. It should be global to the application and persistent
     */
    const char8 * className;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LOGINFORMATION_H_ */

