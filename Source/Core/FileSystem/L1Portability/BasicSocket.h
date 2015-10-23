/*
 * Copyright 2011 EFDA | European Fusion Development Agreement
 *
 * Licensed under the EUPL, Version 1.1 or – as soon they 
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
 * $Id: BasicSocket.h 30 2012-01-31 15:02:09Z boncagni $
 *
**/

/**
 * @file
 * The base class for the sockets implementation
 */
#if !defined( BASIC_SOCKET_H)
#define BASIC_SOCKET_H
#ifndef EINTR
#define EINTR 4
#endif

#include "System.h"
#include "InternetAddress.h"
#include "ErrorManagement.h"
#include "TimeoutType.h"

#define BasicSocketVersion "$Id: BasicSocket.h 30 2012-01-31 15:02:09Z boncagni $"

class BasicSocket;
/** Implements basic generic socket functions */
class  BasicSocket{
protected:

    /** where the packet goes to */
    InternetAddress    destination;

    /** where packets comes from */
    InternetAddress    source;

    /** the socket handle */
    int32              connectionSocket;

public:
    /** constructor  */
    BasicSocket(int32 socket = 0){
        connectionSocket = socket;
    }

    /** destructor */
    ~BasicSocket(){
        Close();
    }

    /**  set blocking mode for the stream! */
    bool SetBlocking(bool flag){
        int stat;
        if (flag == True) stat = 0;
        else stat = 1;
#if defined(_VXWORKS)
        return (ioctl(connectionSocket,FIONBIO,(int)&stat) >= 0);
#elif defined(_RTAI)
        stat = fcntl(connectionSocket, F_GETFL, 0);
	if (stat < 0) 
            return False;
        if (flag == True)
            stat &= ~O_NONBLOCK;
        else
            stat |= O_NONBLOCK;
	return fcntl(connectionSocket, F_SETFL, stat) >= 0;
#else
        int ret = ioctl(connectionSocket,FIONBIO,(char *)&stat,sizeof(stat));
        return ( ret >= 0);
#endif
    }

    /** returns the socket number */
    int32 Socket(){
        return connectionSocket;
    }

    /** closes the socket */
    bool Close(){
        if (connectionSocket != 0){
            int ret = soclose(connectionSocket);
            connectionSocket = 0;
            if(ret<0){
                CStaticAssertErrorCondition(OSError,"BasicSocket::Close failed returning %i\n",ret);
                return False;
            }
        } else return False;
        return True;
    }

    /** where the packet came from */
    InternetAddress &Source(){
        return source;
    }

    /** where the packet is going to */
    InternetAddress &Destination(){
        return destination;
    }


#if  defined(_VXWORKS)
    void AssertSocketErrorCondition(EMFErrorType errorCode,const char *errorDescription=NULL,...);
#else
    /** Sets the error status and depending on setup does appropriate action */
    void AssertSocketErrorCondition(EMFErrorType errorCode,const char *errorDescription=NULL,...){
        va_list argList;
        va_start(argList,errorDescription);
        VCAssertSocketErrorCondition(errorCode,this,"BasicSocket",errorDescription,argList);
        va_end(argList);
    }
#endif


};

#endif






