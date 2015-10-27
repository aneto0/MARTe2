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
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe{

static const uint32 SELECT_WIDTH= 256u;


/** Allows synchronising to a group of sockets.
CAN ONLY SELECT SOCKETS FROM A SINGLE SOURCE TYPE. CANNOT MIX ATM and UDP (on NT only?) */
class  SocketSelect {


public:
    /** */
    SocketSelect();

    /** */
    void Reset();

    /** */
    void AddWaitOnWriteReady(BasicSocket *s);

    /** */
    void DeleteWaitOnWriteReady(BasicSocket *s);

    /** */
    void AddWaitOnReadReady(BasicSocket *s);

    /** */
    void DeleteWaitOnReadReady(BasicSocket *s);

    /** */
    void AddWaitOnExceptReady(BasicSocket *s);

    /** */
    void DeleteWaitOnExceptReady(BasicSocket *s);

    /** Wait for all the event*/
    bool Wait(TimeoutType msecTimeout = TTInfiniteWait);

    /** wait for data on the input */
    bool WaitRead(TimeoutType msecTimeout = TTInfiniteWait);

    /** wait on free space on the output buffer*/
    bool WaitWrite(TimeoutType msecTimeout = TTInfiniteWait);

    /** wait for an exception */
    bool WaitExcept(TimeoutType msecTimeout = TTInfiniteWait);

    /** */
    int32 ReadySockets();

    /** */
    bool CheckRead(BasicSocket *s);

    /** */
    bool CheckWrite(BasicSocket *s);

    /** */
    bool CheckExcept(BasicSocket *s);

    /** */
    fd_set &ReadFDS();

    /** */
    fd_set &WriteFDS();

    /** */
    fd_set &ExceptFDS();


private:
    /** */
    fd_set readFDS;

    /** */
    fd_set writeFDS;

    /** */
    fd_set exceptFDS;

    /** */
    fd_set readFDS_done;

    /** */
    fd_set writeFDS_done;

    /** */
    fd_set exceptFDS_done;

    /** */
    int32 readySockets;

};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOCKETSELECT_H_ */

