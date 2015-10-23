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
 * $Id: BasicATMSocket.cpp 3 2012-01-15 16:26:07Z aneto $
 *
**/

#include "BasicATMSocket.h"
#include "InternetAddress.h"
#include "ErrorManagement.h"
#include "Sleep.h"

#if defined (_WIN32)
#include <ws2atm.h>
#elif defined (_LINUX)
#include <linux/atm.h>
#endif

#define MAX_SDU   1024

/** the actual Write function. This is needed to allow access to the private
    members of InternetAddress */
inline bool BasicATMSocket::_Write(const void* buffer, uint32 &size,TimeoutType msecTimeout){
#if defined (_WIN32) | defined (_LINUX)
    uint32 total = size;
    char *p = (char *)buffer;
    int32 ret = sendto(connectionSocket,p,size,0,destination.Address(),destination.Size());
    if (ret == -1) {
        AssertSocketErrorCondition(OSError,"BasicATMSocket::Write::sendto (VCI=%i) failed ",VCI);
        return False;
    }

    size = ret;
    // to avoid polling continuously release CPU time when reading 0 bytes
    if (size <= 0) SleepMsec(1);
    return (ret >0);
#else
    return False;
#endif
}

bool ATMWrite(BasicATMSocket &as,const void* buffer,uint32 &size,TimeoutType msecTimeout){
    return as._Write(buffer,size,msecTimeout);
}

bool BasicATMSocket::_Read(const void* buffer, uint32 &size,TimeoutType msecTimeout){
#if defined (_WIN32) | defined (_LINUX)
    uint32 total = size;
    char *p = (char *)buffer;
#if defined (_WIN32)
    int sz ;
#else
    socklen_t sz;
#endif 
    sz  = source.Size();

    int32 ret = recvfrom(connectionSocket,p,total,0,source.Address(),&sz);
    if (ret == -1) {
        AssertSocketErrorCondition(OSError,"BasicATMSocket::Read::recvfrom (VCI=%i) failed ",VCI);
        return False;
    }

    size = ret;
    // to avoid polling continuously release CPU time when reading 0 bytes
    if (size <= 0) SleepMsec(1);
    return (ret >0);
#else
    return False;
#endif
}

bool ATMRead(BasicATMSocket &as,void* buffer,uint32 &size,TimeoutType msecTimeout){
    return as._Read(buffer,size,msecTimeout);
}

bool ATMOpen(BasicATMSocket &as){
    as.connectionSocket = -1;
#if defined (_WIN32)
//    connectionSocket = WSASocketW(AF_ATM, SOCK_RAW, ATMPROTO_AAL5, info, g, flags)) == SOCKET_ERROR)
    as.connectionSocket = socket(AF_ATM,SOCK_RAW,ATMPROTO_AAL5);
#elif defined (_LINUX)
	as.connectionSocket = socket(PF_ATMPVC, SOCK_DGRAM, 0);
#else

#endif
    if (as.connectionSocket < 0) as.AssertSocketErrorCondition(OSError,"Open:Cannot open socket to ATM socket");
    return (as.connectionSocket > 0);
}

bool ATMSetVCI(BasicATMSocket &as,int32 VCI){
#if defined (_WIN32)
    as.VCI = VCI;

    ATM_PVC_PARAMS pvcParams;
    memset(&pvcParams,0,sizeof(ATM_PVC_PARAMS));
    pvcParams.PvcConnectionId.VCI = VCI;
    pvcParams.PvcConnectionId.VPI = 0;
    pvcParams.PvcConnectionId.DeviceNumber = 0; // was 1 but 0 works on both NT4 and XP/2K

    pvcParams.PvcQos.ProviderSpecific.buf = NULL;
    pvcParams.PvcQos.ProviderSpecific.len = 0;

    pvcParams.PvcQos.SendingFlowspec.TokenRate           = -1;
    pvcParams.PvcQos.SendingFlowspec.TokenBucketSize     = -1;
    pvcParams.PvcQos.SendingFlowspec.PeakBandwidth       = -1;
    pvcParams.PvcQos.SendingFlowspec.Latency             = -1;
    pvcParams.PvcQos.SendingFlowspec.DelayVariation      = -1;
    pvcParams.PvcQos.SendingFlowspec.ServiceType         =  SERVICETYPE_BESTEFFORT;
    pvcParams.PvcQos.SendingFlowspec.MaxSduSize          = -1;
    pvcParams.PvcQos.SendingFlowspec.MinimumPolicedSize  = -1;

    pvcParams.PvcQos.ReceivingFlowspec.TokenRate           = -1;
    pvcParams.PvcQos.ReceivingFlowspec.TokenBucketSize     = -1;
    pvcParams.PvcQos.ReceivingFlowspec.PeakBandwidth       = -1;
    pvcParams.PvcQos.ReceivingFlowspec.Latency             = -1;
    pvcParams.PvcQos.ReceivingFlowspec.DelayVariation      = -1;
    pvcParams.PvcQos.ReceivingFlowspec.ServiceType         = SERVICETYPE_BESTEFFORT;
    pvcParams.PvcQos.ReceivingFlowspec.MaxSduSize          = -1;
    pvcParams.PvcQos.ReceivingFlowspec.MinimumPolicedSize  = -1;

    uint32 bytes;
    int ret = WSAIoctl(as.connectionSocket, SIO_ASSOCIATE_PVC, &pvcParams,
                    sizeof(pvcParams), NULL, 0, (unsigned long *)&bytes, NULL, NULL);

    if (ret == -1) as.AssertSocketErrorCondition(OSError,"SetVCI::Cannot WASIoctl SIO_ASSOCIATE_PVC ");
    return (ret != -1);
#elif defined (_LINUX)
    struct atm_qos qos;
    memset(&qos, 0, sizeof(qos));
    qos.aal = ATM_AAL5;
    qos.txtp.traffic_class  = ATM_UBR;
    qos.txtp.max_sdu        = MAX_SDU;
    qos.rxtp                = qos.txtp;
    qos.rxtp.spare          = 1;
    int ret = setsockopt(as.connectionSocket, SOL_ATM, SO_ATMQOS, &qos, sizeof(qos));
    if (ret == -1) {
        as.AssertSocketErrorCondition(OSError,"SetVCI::Cannot setsockopt(as.connectionSocket, SOL_ATM, SO_ATMQOS, &qos ");
        return False;
    }

    struct sockaddr_atmpvc addr;
    memset(&addr, 0, sizeof(addr));
    addr.sap_family         = AF_ATMPVC;
    addr.sap_addr.itf       = ATM_ITF_ANY;
    addr.sap_addr.vpi       = 0;
    addr.sap_addr.vci       = VCI;
    ret = bind(as.connectionSocket, (struct sockaddr *)&addr, sizeof(addr));
    if (ret == -1) {
        as.AssertSocketErrorCondition(OSError,"SetVCI::Cannot setsockopt(as.connectionSocket, SOL_ATM, SO_ATMQOS, &qos ");
        return False;
    }

#else
    return False;
#endif

}
//#endif
