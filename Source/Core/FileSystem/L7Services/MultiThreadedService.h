/**
 * @file MultiThreadedService.h
 * @brief Header file for class MultiThreadedService
 * @date Mar 17, 2016
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

 * @details This header file contains the declaration of the class MultiThreadedService
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MULTITHREADEDSERVICE_H_
#define MULTITHREADEDSERVICE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include "Object.h"
#include "ReferenceContainer.h"
#include "Threads.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * one instance of a service handler
 * */
class MultiThreadedServerInstance: public Object {

    ThreadIdentifier threadID;

public:
    CLASS_REGISTER_DECLARATION()

   /**
    * constructor : launches thread
    */
    MultiThreadedServerInstance(){

    }

    /**
     * destructor : destroys thread
     */
    virtual ~MultiThreadedServerInstance(){

    }


};

/**
 * */
class MultiThreadedService: public ReferenceContainer{


    /**
     * Waits no more than timeout for a client request to connect
     * Returns true on connection establishment. In this case clientInfo contains the client information
     *
     */
    virtual bool WaitForClientCOnnection(const TimeoutType &timeout,void *&clientInfo)=0; // could be templated to avoid void *

    /**
     * Waits no more than timeout for a client request to connect
     * performs one interaction with the client
     *
     */
    virtual bool ServeClientRequest(const TimeoutType &timeout,void *&clientInfo)=0; // could be templated to avoid void *

public:
    CLASS_REGISTER_DECLARATION()

    MultiThreadedService(){

    }

    virtual MultiThreadedService(){

    }

    void Start(){

    }

    void Stop(){

    }
};

} // end of namespace

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MULTITHREADEDSERVICE_H_ */
	
