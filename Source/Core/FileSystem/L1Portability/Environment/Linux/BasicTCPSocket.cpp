/**
 * @file BasicTCPSocket.cpp
 * @brief Source file for class BasicTCPSocket
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

 * @details This source file contains the definition of all the methods for
 * the class BasicTCPSocket (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



#include "System.h"
#include "BasicSocket.h"
#include "SocketSelect.h"
#include "InternetService.h"
#include "Sleep.h"


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

        int ret = ioctl(connectionSocket,FIONBIO,(char *)&stat,sizeof(stat));
        return ( ret >= 0);
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



    /** Sets the error status and depending on setup does appropriate action */
    void AssertSocketErrorCondition(EMFErrorType errorCode,const char *errorDescription=NULL,...){
        va_list argList;
        va_start(argList,errorDescription);
        VCAssertSocketErrorCondition(errorCode,this,"BasicSocket",errorDescription,argList);
        va_end(argList);
    }






#define BasicTCPSocketVersion "$Id: BasicTCPSocket.h 3 2012-01-15 16:26:07Z aneto $"
class BasicTCPSocket: public BasicSocket {
public:

    /** basic Read*/
    bool BasicRead(void* buffer, uint32 &size){
        int32 ret = recv(connectionSocket,(char *)buffer,size,0);
        if (ret < 0) {
            AssertSocketErrorCondition(OSError,"BasicTCPSocket::BlockRead");
            size = 0;
            return False;
        }
        size = ret;
        // to avoid polling continuously release CPU time when reading 0 bytes
        if (size == 0) SleepMsec(1);
        return True;
    }

    /** basic write */
    bool BasicWrite(const void* buffer, uint32 &size){
        int32 ret = send(connectionSocket,(char *)buffer,size,0);
        if (ret < 0) {
            AssertSocketErrorCondition(OSError,"BasicTCPSocket::BlockWrite");
            size = 0;
            return False;
        }
        size = ret;
        return True;
    }

    /** Read without consuming */
    bool Peek(void* buffer, uint32 &size){
        int32 ret = recv(connectionSocket,(char *)buffer,size,MSG_PEEK);
        if (ret < 0) {
            AssertSocketErrorCondition(OSError,"BasicTCPSocket::BlockPeek");
            size = 0;
            return False;
        }
        size = ret;
        return True;
    }

    /** just a constructor */
    BasicTCPSocket(int32 socket = 0){
        signal (SIGPIPE, SIG_IGN);
        connectionSocket = socket;
    }

    /** Opens a stream socket */
    bool Open(){
        SocketInit();
        connectionSocket = socket(PF_INET,SOCK_STREAM,0);
        const int one = 1;
        if (setsockopt(connectionSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) < 0){
            AssertSocketErrorCondition(OSError,"BasicTCPSocket::constructor failed calling setsockopt(connectionSocket, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one) (this machine ip is %s)",GetLocalIpNumber());
            return False;
        }

        if (connectionSocket < 0){
            AssertSocketErrorCondition(OSError,"BasicTCPSocket::constructor failed calling socket(PF_INET,SOCK_STREAM,0) (this machine ip is %s)",GetLocalIpNumber());
            return False;
        }

        return True;
    };

    /** Opens a socket as a server at port port */
    bool Listen(int port,int maxConnections=1){
        InternetAddress    server;

        server.SetPort(port);
        int errorCode = bind(connectionSocket, server.Address(),server.Size());
        if (errorCode < 0) {
            AssertSocketErrorCondition(OSError,"BasicTCPSocket::Listen::bind");
            return False;
        }

        errorCode = listen (connectionSocket,maxConnections);
        if (errorCode < 0) {
            AssertSocketErrorCondition(OSError,"BasicTCPSocket::Listen::listen");
            return False;
        }

        return True;
    }

    /** Opens a socket as a server at port port */
    bool Listen(char *serviceName,int maxConnections=1){
        int32 port = InternetService::GetPortByName(serviceName);
        if (port == -1) return False;
        return Listen(port,maxConnections);
    }

    /** connects an unconnected socket to address address and with port port
        if msecTimeout is TTDefault then select is not used */
    bool Connect(const char *address,int port,TimeoutType msecTimeout = TTInfiniteWait, int retry=12){
        destination.SetPort(port);
        if (destination.SetAddressByDotName(address) == False) {
            if (destination.SetAddressByName(address) == False) {
                CStaticAssertErrorCondition(ParametersError,"BasicTCPSocket::Connect:wrong address [%s], port [%d]",address,port);
                return False;
            }
        }

        int32 errorCode = connect(connectionSocket,destination.Address(),destination.Size());
        if (errorCode < 0){

            errorCode = sock_errno();
            if (errorCode == EINTR){
                if(retry > 0){
                    return Connect(address, port, msecTimeout, retry--);
                }
                else{
                    AssertSocketErrorCondition(OSError,"BasicTCPSocket::Connect failed. Interrupted system call...");
                    return False;
                }
            }
            if ((errorCode != 0) &&
                (errorCode != EINPROGRESS) &&
                (errorCode != EWOULDBLOCK)){
                AssertSocketErrorCondition(OSError,"BasicTCPSocket::Connect failed ");
                return False;
            } else {
                if (msecTimeout != TTDefault){
                    SocketSelect sel;
                    sel.AddWaitOnWriteReady(this);
                    bool ret = sel.WaitWrite(msecTimeout);
                    if (ret) return True;
                }
                AssertSocketErrorCondition(OSError,"BasicTCPSocket::Timeout on Connect");
                return False;
            }
        }
        return True;
    }

    /** connects an unconnected socket to address address and with port port */
    bool Connect(const char *address,const char *serviceName,TimeoutType msecTimeout = TTInfiniteWait){
        int32 port = InternetService::GetPortByName(serviceName);
        if (port == -1) return False;
        return Connect(address,port,msecTimeout);
    }

    /** True if we are still connected  Still experimental */
    bool IsConnected(){


        int32           errorCode;
        InternetAddress information;

        socklen_t len   = information.Size();

        int32 ret = getpeername(connectionSocket,information.Address(),&len);

        errorCode = sock_errno();


        return (ret == 0);

    }

    /** this is a BasicTCPSocket constructor .. */
    BasicTCPSocket *WaitConnection(TimeoutType msecTimeout = TTInfiniteWait,BasicTCPSocket *client=NULL){
        int size = source.Size();
        int newSocket = accept(connectionSocket,source.Address(),(socklen_t *)&size);

        if  ( newSocket != -1 ) {
            if (!client) client = new BasicTCPSocket(newSocket);
            client->destination = source;
            client->source      = source;
            client->connectionSocket = newSocket;
            return client;
        } else
        if (msecTimeout != TTDefault){
            int32 errorCode;
            errorCode = sock_errno();

            if ((errorCode != 0) &&
                (errorCode != EINPROGRESS) &&
                (errorCode != EWOULDBLOCK)){
                AssertSocketErrorCondition(OSError,"BasicTCPSocket::WaitConnection failed ");
                return NULL;
            } else {
                SocketSelect sel;
                sel.AddWaitOnReadReady(this);
                bool ret = sel.WaitRead(msecTimeout);
                if (ret) return WaitConnection(TTDefault,client);
                //AssertSocketErrorCondition(Warning,"BasicTCPSocket::Timeout on Connect");
                return NULL;
            }

        }
        return NULL;
    }

};
	
