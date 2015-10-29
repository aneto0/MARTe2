/**
 * @file SocketSelect.h
 * @brief Header file for class SocketSelect
 * @date 27/10/2015
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

 * @details This header file contains the declaration of the class SocketSelect
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOCKETSELECT_H_
#define SOCKETSELECT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TimeoutType.h"
#include "BasicSocket.h"
#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT,SocketSelectCore.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {



    /**
     * @brief A class using to check if sockets are ready for a read, write operation or if
     * an exception is throw.
     */
    class SocketSelect {

    public:
        /**
         * @brief Default constructor.
         * @post
         * The state of the sockets select mask is reset.
         * readySockets = 0.
         */
        SocketSelect();

        /**
         * @brief Reset the state of the socket select mask.
         * @post
         * readySocket = 0.
         */
        void Reset();

        /**
         * @brief Set a specific socket to check when it is ready for write operations.
         * @param[in] s is the socket to set.
         */
        void AddWaitOnWriteReady(const BasicSocket * const s);

        /**
         * @brief UnSet a specific socket from the checking when it is ready for write operations.
         * @param[in] s is the socket to unset.
         */
        void DeleteWaitOnWriteReady(const BasicSocket * const s);

        /**
         * @brief Set a specific socket to check when it is ready for read operations.
         * @param[in] s is the socket to set.
         */
        void AddWaitOnReadReady(const BasicSocket * const s);

        /**
         * @brief UnSet a specific socket from the checking when it is ready for read operations.
         * @param[in] s is the socket to unset.
         */
        void DeleteWaitOnReadReady(const BasicSocket * const s);

        /**
         * @brief Set a specific socket to check when an exception is throw.
         * @param[in] s is the socket to set.
         */
        void AddWaitOnExceptReady(const BasicSocket * const s);

        /**
         * @brief UnSet a specific socket from the checking for an exception.
         * @param[in] s is the socket to unset.
         */
        void DeleteWaitOnExceptReady(const BasicSocket * const s);

        /**
         * @brief Wait for all the events within a timeout.
         * @param[in] timeout is the maximum time to wait for an event.
         * @post
         * readySocket is the number of the sockets when an event happened.
         * @return true if at least one socket is ready before the timeout expire.
         */
        bool Wait(const TimeoutType &timeout = TTInfiniteWait);

        /**
         * @brief Wait for all the read events within a timeout.
         * @param[in] timeout is the maximum time to wait for a read event.
         * @post
         * readySocket is the number of the sockets when a read event happened.
         * @return true if at least one socket is ready before the timeout expire.
         */
        bool WaitRead(const TimeoutType &timeout = TTInfiniteWait);

        /**
         * @brief Wait for all the write events within a timeout.
         * @param[in] timeout is the maximum time to wait for a write event.
         * @post
         * readySocket is the number of the sockets when a write event happened.
         * @return true if at least one socket is ready before the timeout expire.
         */
        bool WaitWrite(const TimeoutType &timeout = TTInfiniteWait);

        /**
         * @brief Wait for all the exception events within a timeout.
         * @param[in] timeout is the maximum time to wait for an exception.
         * @post
         * readySocket is the number of the sockets when an exception happened.
         * @return true if at least one socket is ready before the timeout expire.
         */
        bool WaitExcept(const TimeoutType &timeout = TTInfiniteWait);

        /**
         * @brief Retrieve the number of ready sockets for the specified operation requested.
         */
        int32 ReadySockets() const;

        /**
         * @brief Check if a socket is ready for a read operation.
         * @param[in] s is the socket to check.
         */
        bool CheckRead(const BasicSocket * const s);

        /**
         * @brief Check if a socket is ready for a write operation.
         * @param[in] s is the socket to check.
         */
        bool CheckWrite(const BasicSocket * const s);

        /**
         * @brief Check if an exception is throw on the specified socket.
         * @param[in] s is the socket to check.
         */
        bool CheckExcept(const BasicSocket * const s);


    private:

        /**
         * The SocketSelect handle
         */
        SocketSelectCore selectHandle;

        /**
         * The number of ready sockets.
         */
        int32 readySockets;

    };
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOCKETSELECT_H_ */

