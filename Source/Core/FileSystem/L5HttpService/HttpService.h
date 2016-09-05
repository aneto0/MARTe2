/**
 * @file HttpService.h
 * @brief Header file for class HttpService
 * @date Sep 5, 2016
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

 * @details This header file contains the declaration of the class HttpService
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L5HTTPSERVICE_HTTPSERVICE_H_
#define L5HTTPSERVICE_HTTPSERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainer.h"
#include "TCPSocket.h"

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class HttpServiceConnector{

private:
    TCPSocket conectedSocket;


};

/**
 * The container may contain objects of any type.
 * But only object of type HttpResourceContainer result in accessible pages
 *
 */
class HttpService: public ReferenceContainer{
public:

    /**
    * TODO
    * same as object interface
    */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * Open socket
     * launches threads
     */
    virtual ErrorManagement::ErrorType Start();

    /**
     * closes threads
     * destroys all HttpServiceConnector
     */
    virtual ErrorManagement::ErrorType Stop();

private:

    /// allows sending a message
    static void SendHttpMessage(CCString url, Stream & content);

    /// here to wait for connections
    BasicTCPSocket listeningSocket;

};


/** may contain references to any object
 * Objects of type HttpLinkResource are automatically transformed in links to an Object in GODB
 * Object of type HttpRealm allows setting the security level. By default is no access!
 *
 * */
class HttpResourceContainer: public ReferenceContainer{

};

/**
 * allows mapping a simple URL into a full parametrised method call
 */
class HttpMapResource{

};

/**
 * provides a streightforward reply to an URL
 */
class HttpResource{

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


}
#endif /* L5HTTPSERVICE_HTTPSERVICE_H_ */
	
