/**
 * @file HttpRealmI.h
 * @brief Header file for class HttpRealmI
 * @date Sep 6, 2016
 * @author fsartori
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

 * @details This header file contains the declaration of the class HttpRealmI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L5HTTPSERVICE_HTTPREALMI_H_
#define L5HTTPSERVICE_HTTPREALMI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Object.h"

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 *
 */
class HttpRealmI{
    /**
     * TODO
     */
    typedef uint8 SecurityModel ;
    /**
     *  use digest security
     */
    static const SecurityModel digest = 0x00;
    /**
     *  use simple base64 password encoding
     */
    static const SecurityModel base64       = 0x5A;
    /**
     *  password in clear
     */
    static const SecurityModel clear        = 0xBB;
    /**
     *  free access
     */
    static const SecurityModel noSecurity   = 0xA5;

    /**
     * TODO
     */
    typedef uint8 CommandMask ;
    /**
     *  GET enabled
     */
    static const CommandMask get = 0x01;
    /**
     *  POST enabled
     */
    static const CommandMask post = 0x02;
    /**
     *  PUT enabled
     */
    static const CommandMask put  = 0x04;
    /**
     *  HEAD enabled
     */
    static const CommandMask head = 0x08;

public:
    /** True if the key is valid in the context
        @param command is the HTTP command used
        @param ipNumber is the ip of the client */
    virtual bool        Validate(
                            CCString key,
                            HSHttpCommand           command,
                            uint32                  ipNumber) = 0;

    /** True if digest authorisation needed */
    virtual bool        DigestSecurityNeeded() = 0;

    /** Return the value to associate to WWW-Authenticate */
    virtual ErrorManagement::ErrorType GetAuthenticationRequest(StreamString &message)= 0;

    /** requires Object or derivatives in the parenthood */
    CCString            RealmName();

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

CCString HttpRealmI::RealmName(){
    Object *asObject = dynamic_cast<Object *>(this);
    if (asObject != NULL_PTR(Object *)) return asObject->GetName();
    return "";
}


}

#endif /* L5HTTPSERVICE_HTTPREALMI_H_ */
	
