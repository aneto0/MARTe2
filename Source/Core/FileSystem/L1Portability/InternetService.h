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
 * $Id: InternetService.h 3 2012-01-15 16:26:07Z aneto $
 *
**/

#if !defined (INTERNET_SERVICE_H)
#define INTERNET_SERVICE_H

#include "System.h"
#include "Endianity.h"

/** 
 * @file 
 * Allows handling ports by name
 */

class InternetService{
    /** */
    servent service;

public:
    /** */
    bool SearchByName(const char *name,char *protocol=NULL){
#ifndef _RTAI
        servent *serv = getservbyname(name,protocol);
        if (serv == NULL) return False;
        service = *serv;
        return True;
#else
        int ret = fcomm_getservbyname(name,protocol,&service);
        return (ret==0);
#endif
    }

    /** */
    bool SearchByPort(int32 port,char *protocol=NULL){
#ifndef _RTAI
        servent *serv = getservbyport(port,protocol);
        if (serv == NULL) return False;
        service = *serv;
        return True;
#else
        int ret = fcomm_getservbyport(port,protocol,&service);
        return (ret == 0);
#endif
    }

    /** */
    int32 Port() {
        return service.s_port;
    }

    /** */
    const char *Name() {
        return service.s_name;
    }

    /** */
    const char *Protocol() {
        return service.s_proto;
    }

    /** */
    static int32 GetPortByName(const char *name){
        InternetService service;
        service.SearchByName(name);
        return service.Port();
    }
};

#endif

