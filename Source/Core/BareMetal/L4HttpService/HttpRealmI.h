/**
 * @file HttpRealmI.h
 * @brief Header file for class HttpRealmI
 * @date 22/08/2018
 * @author Giuseppe Ferro
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

#ifndef HTTPREALM_H_
#define HTTPREALM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "HttpDefinition.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief HTTP interface to implement a realm.
 */
class HttpRealmI {
public:

    /**
     * @brief Default constructor
     */
    HttpRealmI();

    /**
     * @brief Destructor
     */
    virtual ~HttpRealmI();

    /**
     * @brief Validates a key
     * @param[in] key the key to be validated
     * @param[in] command the HTTP command
     * @param[in] ipNumber the client ip number
     * @return true if the key is valid, false otherwise
     */
    virtual bool Validate(const char8 * const key, const int32 command, const uint32 ipNumber) = 0;

    /**
     * @brief Asserts if the security digest is needed.
     * @return true if the security digest is needed, false otherwise.
     */
    virtual bool DigestSecurityNeeded() = 0;

    /**
     * @brief Retrieves the authentication request.
     * @param[out] message contains the authentication request in output.
     * @return true if the operation succeeds, false otherwise.
     */
    virtual bool GetAuthenticationRequest(StreamString &message)= 0;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* HTTPREALM_H_ */

