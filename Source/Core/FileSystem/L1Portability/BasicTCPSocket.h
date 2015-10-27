/**
 * @file BasicTCPSocket.h
 * @brief Header file for class BasicTCPSocket
 * @date 23/10/2015
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

 * @details This header file contains the declaration of the class BasicTCPSocket
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICTCPSOCKET_H_
#define BASICTCPSOCKET_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "System.h"
#include "BasicSocket.h"
#include "SocketSelect.h"
#include "InternetService.h"
#include "Sleep.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class BasicTCPSocket: public BasicSocket {
public:

    /** just a constructor */
    BasicTCPSocket(int32 socket = 0);

    /** destructor */
    ~BasicTCPSocket();

    /**  set blocking mode for the stream! */
    bool SetBlocking(bool flag);

    /** returns the socket number */
    int32 Socket();

    /** closes the socket */
    bool Close();

    /** where the packet came from */
    InternetAddress &Source();

    /** where the packet is going to */
    InternetAddress &Destination();

    /** basic Read*/
    bool BasicRead(void* buffer,
                   uint32 &size);

    /** basic write */
    bool BasicWrite(const void* buffer,
                    uint32 &size);

    /** Read without consuming */
    bool Peek(void* buffer,
              uint32 &size);

    /** Opens a stream socket */
    bool Open();

    /** Opens a socket as a server at port port */
    bool Listen(int32 port,
                int32 maxConnections = 1);

    /** Opens a socket as a server at port port */
    bool Listen(char8 *serviceName,
                int32 maxConnections = 1);


    /** connects an unconnected socket to address address and with port port */
    bool Connect(const char8 *address,
                 const char8 *serviceName,
                 TimeoutType msecTimeout = TTInfiniteWait);

    /** True if we are still connected  Still experimental */
    bool IsConnected();

    /** this is a BasicTCPSocket constructor .. */
    BasicTCPSocket *WaitConnection(TimeoutType msecTimeout = TTInfiniteWait,
                                   BasicTCPSocket *client = NULL);

private:

    /** where the packet goes to */
    InternetAddress destination;

    /** where packets comes from */
    InternetAddress source;

    /** the socket handle */
    int32 connectionSocket;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICTCPSOCKET_H_ */

