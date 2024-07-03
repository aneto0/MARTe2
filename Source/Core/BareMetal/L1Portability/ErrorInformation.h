/**
 * @file ErrorInformation.h
 * @brief Header file for class ErrorInformation
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

 * @details This header file contains the declaration of the class ErrorInformation
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ERRORINFORMATION_H_
#define ERRORINFORMATION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ErrorType.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

namespace ErrorManagement {

/**
 @brief Information structure associated to the error.
 */
struct ErrorInformation {

    /**
     * Definition of the header.
     */
    struct {

        /**
         * The error code.
         */
        ErrorType errorType;

        /**
         * The error line number.
         */
        int16 lineNumber;

        /**
         * Specified is the error is triggered within an object.
         */
        bool isObject :1;

    } header;

    /**
     *  High resolution timer ticks.
     */
    uint64 hrtTime;

    /**
     * Time in seconds from the epoch
     */
    int32 timeSeconds;

    /**
     * The object name (if available).
     */
    const char8 * objectName;

    /**
     * The class name (if available)
     */
    const char8 * className;

    /**
     * The error file name.
     */
    const char8 * fileName;

    /**
     * The error function name
     */
    const char8 * functionName;

    /**
     * A string which is taken directly from an external hook to 
     * retrieve platform specific data (e.g. the platform core identifier or the build type)
    */
    const char8 * platformSpecificString;

    /**
     * thread ID of the threads who generate the error.
     */
//    ThreadIdentifier threadId;
    /**
     * The Address of the object that produced the error.
     * Object may be temporary in memory because the
     * objectPointer will only be printed, not used
     */
    const void * objectPointer;


};
}

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ERRORINFORMATION_H_ */

