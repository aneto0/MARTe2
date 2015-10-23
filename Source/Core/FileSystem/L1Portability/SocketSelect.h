/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they 
   will be approved by the European Commission - subsequent  
   versions of the EUPL (the "Licence"); 
 * You may not use this work except in compliance with the 
   Licence. 
 * You may obtain a copy of the Licence at: 
 *  
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in 
   writing, software distributed under the Licence is 
   distributed on an "AS IS" basis, 
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either 
   express or implied. 
 * See the Licence for the specific language governing 
   permissions and limitations under the Licence. 
 *
 * $Id: SocketSelect.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

/** 
 * @file
 * A wrapper of the select function for sockets
 */
#if !defined SocketSelect_
#define SocketSelect_

#include "System.h"
#include "BasicSocket.h"
#include "TimeoutType.h"

#if defined(_VX5100) || defined(_VX5500) || defined(_V6X5100) || defined(_V6X5500)
#define SELECT_WIDTH FD_SETSIZE
#else
#define SELECT_WIDTH 256
#endif


/** Allows synchronising to a group of sockets.
CAN ONLY SELECT SOCKETS FROM A SINGLE SOURCE TYPE. CANNOT MIX ATM and UDP (on NT only?) */
class  SocketSelect {
protected:
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

public:
    /** */
    SocketSelect(){
        Reset();
    }

    /** */
    void Reset(){
        FD_ZERO(&readFDS);
        FD_ZERO(&writeFDS);
        FD_ZERO(&exceptFDS);
        FD_ZERO(&readFDS_done);
        FD_ZERO(&writeFDS_done);
        FD_ZERO(&exceptFDS_done);
    }

    /** */
    void AddWaitOnWriteReady(BasicSocket *s){
        if (s==NULL) return ;
        FD_SET(s->Socket(),&writeFDS);
    }
    /** */
    void DeleteWaitOnWriteReady(BasicSocket *s){
        if (s==NULL) return ;
        FD_CLR(s->Socket(),&writeFDS);
    }
    /** */
    void AddWaitOnReadReady(BasicSocket *s){
        if (s==NULL) return ;
        FD_SET(s->Socket(),&readFDS);
    }
    /** */
    void DeleteWaitOnReadReady(BasicSocket *s){
        if (s==NULL) return ;
        FD_CLR(s->Socket(),&readFDS);
    }
    /** */
    void AddWaitOnExceptReady(BasicSocket *s){
        if (s==NULL) return ;
        FD_SET(s->Socket(),&exceptFDS);
    }
    /** */
    void DeleteWaitOnExceptReady(BasicSocket *s){
        if (s==NULL) return ;
        FD_CLR(s->Socket(),&exceptFDS);
    }
    /** Wait for all the event*/
    bool Wait(TimeoutType msecTimeout = TTInfiniteWait){
        readFDS_done   = readFDS;
        writeFDS_done  = writeFDS;
        exceptFDS_done = exceptFDS;

        timeval timeWait;
        if (msecTimeout == TTInfiniteWait) {
            readySockets = select(SELECT_WIDTH,&readFDS_done,&writeFDS_done,&exceptFDS_done,NULL);
        } else {
            timeWait.tv_sec  = msecTimeout.msecTimeout / 1000;
            timeWait.tv_usec = 1000 * (msecTimeout.msecTimeout - (timeWait.tv_sec * 1000));
            readySockets = select(SELECT_WIDTH,&readFDS_done,&writeFDS_done,&exceptFDS_done,&timeWait);
        }
        return (readySockets > 0);
    }

    /** wait for data on the input */
    bool WaitRead(TimeoutType msecTimeout = TTInfiniteWait){
        readFDS_done   = readFDS;

        timeval timeWait;
        if (msecTimeout == TTInfiniteWait) {
            readySockets = select(SELECT_WIDTH,&readFDS_done,NULL,NULL,NULL);
        } else {
            timeWait.tv_sec  = msecTimeout.msecTimeout / 1000;
            timeWait.tv_usec = 1000 * (msecTimeout.msecTimeout - (timeWait.tv_sec * 1000));
            readySockets = select(SELECT_WIDTH,&readFDS_done,NULL,NULL,&timeWait);
        }
        return (readySockets > 0);
    }

    /** wait on free space on the output buffer*/
    bool WaitWrite(TimeoutType msecTimeout = TTInfiniteWait){
        writeFDS_done   = writeFDS;

        timeval timeWait;
        if (msecTimeout == TTInfiniteWait) {
            readySockets = select(SELECT_WIDTH,NULL,&writeFDS_done,NULL,NULL);
        } else {
            timeWait.tv_sec  = msecTimeout.msecTimeout / 1000;
            timeWait.tv_usec = 1000 * (msecTimeout.msecTimeout - (timeWait.tv_sec * 1000));
            readySockets = select(SELECT_WIDTH,NULL,&writeFDS_done,NULL,&timeWait);
        }
        return (readySockets > 0);
    }

    /** wait for an exception */
    bool WaitExcept(TimeoutType msecTimeout = TTInfiniteWait){
        exceptFDS_done = exceptFDS;

        timeval timeWait;
        if (msecTimeout == TTInfiniteWait) {
            readySockets = select(SELECT_WIDTH,NULL,NULL,&exceptFDS_done,NULL);
        } else {
            timeWait.tv_sec  = msecTimeout.msecTimeout / 1000;
            timeWait.tv_usec = 1000 * (msecTimeout.msecTimeout - (timeWait.tv_sec * 1000));
            readySockets = select(SELECT_WIDTH,NULL,NULL,&exceptFDS_done,&timeWait);
        }
        return (readySockets > 0);
    }

    /** */
    int32 ReadySockets(){ return readySockets; }

    /** */
    bool CheckRead(BasicSocket *s){ return (FD_ISSET(s->Socket(),&readFDS_done)!= 0); }

    /** */
    bool CheckWrite(BasicSocket *s){ return (FD_ISSET(s->Socket(),&writeFDS_done)!= 0); }

    /** */
    bool CheckExcept(BasicSocket *s){ return (FD_ISSET(s->Socket(),&exceptFDS_done)!= 0); }

    /** */
    fd_set &ReadFDS(){ return readFDS_done; }

    /** */
    fd_set &WriteFDS(){ return readFDS_done; }

    /** */
    fd_set &ExceptFDS(){ return readFDS_done; }

};

#endif

