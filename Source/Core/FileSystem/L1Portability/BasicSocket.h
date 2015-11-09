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
#include "System.h"
#include "InternetAddress.h"
#include "ErrorManagement.h"
#include "TimeoutType.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/** Implements basic generic socket functions */
class BasicSocket {

public:

    BasicSocket();

    /** constructor  */
    BasicSocket(int32 socket);

    /** destructor */
    ~BasicSocket();

    /**  set blocking mode for the stream! */
    bool SetBlocking(bool flag);

    /** returns the socket number */
    int32 Socket();

    /** closes the socket */
    bool Close();

    /** where the packet came from */
    InternetAddress &GetSource();

    /** where the packet is going to */
    InternetAddress &GetDestination();


    int32 GetConnectionSocket() const;

    void SetDestination(const InternetAddress &destinationIn);

    void SetSource(const InternetAddress &sourceIn);

    void SetConnectionSocket(int32 connectionSocketIn);

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

#endif /* BASICSOCKET_H_ */

