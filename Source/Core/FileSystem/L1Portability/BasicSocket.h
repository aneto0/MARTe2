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
#include INCLUDE_FILE_ENVIRONMENT(ENVIRONMENT,SocketCore.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    /**
     * @brief Implementation of the most common socket functions shared by UDP and TCP sockets.
     */
    class BasicSocket: public StreamI {

        friend class SocketSelect;

    public:

        /**
         * @brief Default constructor.
         */
        BasicSocket();

        /**
         * @brief Destructor
         * @post
         * Close()
         */
        virtual ~BasicSocket();

        /**
         * @brief Set\UnSet blocking mode.
         * @param[in] flag specifies if blocking mode must be set(true) or unset(false).
         * @return true if the desired mode is set correctly, false otherwise.
         */
        virtual bool SetBlocking(const bool flag) const;

        /**
         * @brief Closes the socket.
         * @return true if the socket is closed correctly, false otherwise.
         */
        bool Close();

        /**
         * @brief The source host InternetHost structure.
         */
        InternetHost GetSource() const;

        /**
         * @brief The destination host InternetHost structure.
         */
        InternetHost GetDestination() const;


        /**
         * @brief Sets the destination host parameters.
         * @param[in] destinationIn is an InternetHost which contains all the
         * necessary informations for the destination host handling.
         */
        void SetDestination(const InternetHost &destinationIn);

        /**
         * @brief Sets the source host parameters.
         * @param[in] sourceIn is an InternetHost which contains all the
         * necessary informations for the source host handling.
         */
        void SetSource(const InternetHost &sourceIn);


        /**
         * @brief Checks if the socket handle is valid or not.
         */
        virtual bool IsValid() const;


    protected:


        /**
         * Where the packets go to.
         */
        InternetHost destination;

        /**
         * Where packets come from.
         */
        InternetHost source;

        /**
         * The socket handle.
         */
        SocketCore connectionSocket;

    };

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICSOCKET_H_ */

