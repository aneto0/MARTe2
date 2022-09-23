/**
 * @file BasicSocket.h
 * @brief Header file for class BasicSocket
 * @date 26/10/2015
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

 * @details This header file contains the declaration of the class BasicSocket
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICSOCKET_H_
#define BASICSOCKET_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "InternetHost.h"
#include "StreamI.h"
#include "HandleI.h"
#include INCLUDE_FILE_ENVIRONMENT(FileSystem,L1Portability,ENVIRONMENT,SocketCore.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    /**
     * @brief Abstract class which represents the basic configuration for any
     * type of network socket.
     * @details The main responsibilities of this class are:
     * + Setting and querying the configuration of the source host.
     * + Setting and querying the configuration of the destination host.
     * + Setting and querying the blocking status of the socket itself.
     */
    class DLL_API BasicSocket: public virtual StreamI, public HandleI {
    public:
        /**
         * @brief Default constructor.
         */
        BasicSocket();

        /**
         * @brief Destructor
         * @post
         *   Close()
         */
        virtual ~BasicSocket();

        /**
         * @brief Set \ UnSet blocking mode.
         * @param[in] flag specifies if blocking mode must be set(true) or unset(false).
         * @return true if the desired mode is set correctly.
         */
        bool SetBlocking(const bool flag);

        /**
         * @brief Checks if the socket is in blocking mode or not.
         * @return true if the socket is in blocking mode, false otherwise.
         */
        bool IsBlocking() const;

        /**
         * @brief Closes the socket.
         * @return true if the socket is closed correctly.
         */
        bool Close();

        /**
         * @brief Returns the Internet host address of the connection source, where the packets are received from.
         * @return the the Internet host address of the connection source
         */
        InternetHost GetSource() const;

        /**
         * @brief Returns the Internet host address of the socket destination, where the packets are sent to.
         * @return the Internet host address of the socket destination.
         */
        InternetHost GetDestination() const;

        /**
         * @brief Sets the destination Internet host address, where the packets are sent to.
         * @param[in] destinationIn the Internet host address of the socket destination.
         * @post
         *   GetDestination() == destinationIn
         */
        void SetDestination(const InternetHost &destinationIn);

        /**
         * @brief Sets the source Internet host address, where the packets are received from.
         * @param[in] sourceIn the Internet host address of the socket source connection.
         * @post
         *   GetSource() == sourceIn
         */
        void SetSource(const InternetHost &sourceIn);

        /**
         * @brief Checks if the socket handle is valid or not.
         */
        virtual bool IsValid() const;

        /**
         * @brief Queries the read socked handle.
         * @details For the BasicSocket the read handle and the write handle are the same,
         * however the BasicConsol has two different handles: one for read and one for write.
         */
        virtual Handle GetReadHandle() const;

        /**
         * @brief Queries the write socked handle.
         * @details For the BasicSocket the read handle and the write handle are the same,
         * however the BasicConsol has two different handles: one for read and one for write.
         */
        virtual Handle GetWriteHandle() const;

        /**
         * @brief Allows to bind to a port in use by another socket.
         * @param[in] reuse true (false) allows (disallows) to bind to a port in use by another socket.
         */
        bool SetReuseAddr(bool reuse);

    protected:

        /**
         * Address of the destination target, where the packets are sent to.
         */
        InternetHost destination;

        /**
         * Address of the source connection, where the packets are received from.
         */
        InternetHost source;

        /**
         * The socket low-level handle.
         */
        SocketCore connectionSocket;

    private:

        bool isBlocking;

    };

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICSOCKET_H_ */

