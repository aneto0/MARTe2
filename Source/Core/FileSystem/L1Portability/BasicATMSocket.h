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
 * $Id: BasicATMSocket.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

/**
 * @file
 * ATM AAL5 support. 
 */ 
#if !defined(BASIC_ATM_SOCKET_H)
#define BASIC_ATM_SOCKET_H

#include "BasicSocket.h"
#include "TimeoutType.h"

class BasicATMSocket;

extern "C" {

    /** */
    bool ATMWrite(BasicATMSocket &as,const void* buffer,uint32 &size,TimeoutType msecTimeout);

    /** */
    bool ATMRead(BasicATMSocket &as,void* buffer,uint32 &size,TimeoutType msecTimeout);

    /** */
    bool ATMOpen(BasicATMSocket &as);

    /** */
    bool ATMSetVCI(BasicATMSocket &as,int32 VCI);
}


/** Implements ATM sockets AAL5 */
class BasicATMSocket: public BasicSocket {

    friend bool ATMWrite(BasicATMSocket &as,const void* buffer, uint32 &size,TimeoutType msecTimeout);
    friend bool ATMRead(BasicATMSocket &as,void* buffer, uint32 &size,TimeoutType msecTimeout);
    friend bool ATMOpen(BasicATMSocket &as);
    friend bool ATMSetVCI(BasicATMSocket &as,int32 VCI);
protected:
    /** the virtual circuit */
    uint32 VCI;

private:
    /** the actual Write function. This is needed to allow access to the private
        members of InternetAddress */
    bool _Write(const void* buffer,uint32 &size,TimeoutType msecTimeout);

    /** the actual Read function. This is needed to allow access to the private
        members of InternetAddress */
    bool _Read(const void* buffer,uint32 &size,TimeoutType msecTimeout);

public:
    /** constructor */
                        BasicATMSocket(int32 socket = 0)
    {
        connectionSocket = socket;
    }

    /** destructor */
                        ~BasicATMSocket()
    {
        Close();
    }

    /** Opens a socket on ATM */
    bool                Open()
    {
        return ATMOpen(*this);
    }

    /** Reads a block of data: size is the maximum size. on return size is what was read
        timeout not supported yet */
    inline bool         Read(
                            void*               buffer,
                            uint32 &            size,
                            TimeoutType         msecTimeout     = TTDefault)
    {
        return ATMRead(*this,buffer,size,msecTimeout);
    }


    /** Writes a block of data: size is its size. on return size is what was written.
        timeout not supported yet */
    inline bool         Write(
                            const void*         buffer,
                            uint32 &            size,
                            TimeoutType         msecTimeout     = TTDefault)
    {
        return ATMWrite(*this,buffer,size,msecTimeout);
    }


    /** Sets the virtual circuit */
    inline bool         SetVCI(int32 VCI)
    {
        return ATMSetVCI(*this,VCI);
    }

    /** Get The virtual circuit*/
    inline uint32       GetVCI()
    {
        return VCI;
    }

};

#endif



