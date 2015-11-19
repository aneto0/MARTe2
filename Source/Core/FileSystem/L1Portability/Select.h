/**
 * @file Select.h
 * @brief Header file for class Select
 * @date 12/11/2015
 * @author Llorenç Capellà
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

 * @details This header file contains the declaration of the class Select
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SELECT_H_
#define SELECT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "TimeoutType.h"
#include "HandleI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT, SelectProperties.h)

namespace MARTe {

    /**
     * @brief Select implementation OS independent.
     * @details The Select class provides a set of functions to monitor several sockets events at the same time.
     * The class allows to add, remove or clear all sockets in read, write or exception mode. When the desired sockets are chosen the WaitUtil()
     * start to monitoring the sockets until an even occurs. The IsSet() can be used to know which events were triggered.
     */
    class DLL_API Select {

    public:

        /**
         * @Brief Default constructor
         * @post
         *    readHandle is empty &&
         *    writeHandle is empty &&
         *    exceptionHandle is empty &&
         *    highestHandle = -1
         */
        Select();

        /**
         * @brief Default destructor.
         */
        virtual ~Select() {
        }

        /**
         * @brief Adds a handle to be monitored in the read mode.
         * @param[in] handle indicates the handle to be added.
         * @pre
         *     The handle must be valid
         *     The handle has not to be included previously.
         * @return True if the handle is added to be monitored.
         */
        bool AddReadHandle(const HandleI &handle);

        /**
         * @brief Adds a handle to be monitored in the write mode.
         * @param[in] handle indicates the handle to be added.
         * @pre
         *     The handle must be valid &&
         *     The handle has not to be included previously.
         * @return True if the handle is added to be monitored.
         */
        bool AddWriteHandle(const HandleI &handle);

        /**
         * @brief Adds a handle to be monitored in the exception mode.
         * @details If a BasicConsole object is added to exception the two handles of BasicConsol will be added.
         * @param[in] handle indicates the handle to be added.
         * @pre
         *     The handle must be valid &&
         *     The handle has not to be included previously.
         * @return True if the handle is added to be monitored.
         */
        bool AddExceptionHandle(const HandleI &handle);

        /**
         * @brief Removes a handle from being monitored in the read mode.
         * @param[in] handle indicates the handle to be removed.
         * @pre
         *     The handle must be valid &&
         *     The handle must be included previously.
         * @return True if the handle is removed from being monitored.
         */
        bool RemoveReadHandle(const HandleI &handle);

        /**
         * @brief Removes a handle from being monitored in the write removed.
         * @param[in] handle indicates the handle to be added.
         * @pre
         *     The handle must be valid &&
         *     The handle must be included previously.
         * @return True if the handle is removed from being monitored.
         */
        bool RemoveWriteHandle(const HandleI &handle);

        /**
         * @brief Removes a handle from being monitored in the exception mode.
         * @param[in] handle indicates the handle to be removed.
         * @pre
         *     The handle must be valid &&
         *     The handle must be included previously.
         * @return True if the handle is removed from being monitored.
         */
        bool RemoveExceptionHandle(const HandleI &handle);

        /**
         * @brief Removes all handles from being monitored in all modes.
         * @pre
         *     true
         * @post
         *    !IsSet()
         * @return True if the handle is removed from being monitored.
         */
        void ClearAllHandle();

        /**
         * @brief Queries if the handle is set in one of the three modes.
         * @param[in] handle indicates the handle to be search for.
         * @return True if the handle is set.
         */
        bool IsSet(const HandleI &handle) const;

        /**
         * @brief Blocks until an event occurs or the msecTimeout unblocks
         * @param[in] msecTimeout is the maximum time waiting,
         * @return -1 if it fails, 0 if the msecTimeout is reached or the sum of occurrences happened.
         */
        int32 WaitUntil(const TimeoutType &msecTimeout = TTInfiniteWait);

    private:
        /*Contains informations about the read handles used in the select. Every AddReadHandle() action an handle informations is added to readHadle.*/
        SetIdentifier readHandle;

        /*Contains informations about the write handles used in the select. Every AddWriteHandle() action an handle informations is added to writeHandle.*/
        SetIdentifier writeHandle;

        /*Contains informations about the exceptions handles used in the select. Every AddExceptionHandle() action an handle informations is added to exceptionHandle.*/
        SetIdentifier exceptionHandle;

        /*It is the highest number of a handle that readHandle, writeHandle or exceptionHandle contains*/
        int32 highestHandle;
    };
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* _SELECT_H_ */

