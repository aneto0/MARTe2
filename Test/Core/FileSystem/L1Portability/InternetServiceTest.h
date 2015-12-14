/**
 * @file InternetServiceTest.h
 * @brief Header file for class InternetServiceTest
 * @date 29/10/2015
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

 * @details This header file contains the declaration of the class InternetServiceTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTERNETSERVICETEST_H_
#define INTERNETSERVICETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "InternetService.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief A structure used for Internet Service tests.
 */
struct InternetServiceTestTable {
    const char8* name;
    const char8* protocol;
    int32 port;
    const char8* expectedProtocol;
    bool expected;

};

/**
 * @brief Tests all the InternetService public functions.
 */
class InternetServiceTest {
public:
    /**
     * @brief Tests if the default constructor sets name, port and protocol to zero.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests if the function sets the port and the protocol related to the service name provided.
     */
    bool TestSearchByName(const InternetServiceTestTable* table);

    /**
     * @brief Tests if the function sets the name and the protocol related to the service port provided.
     */
    bool TestSearchByPort(const InternetServiceTestTable* table);

    /**
     * @brief Tests if the function returns the correct port number.
     */
    bool TestPort(const InternetServiceTestTable* table);

    /**
     * @brief Tests if the function returns the correct service name.
     */
    bool TestName(const InternetServiceTestTable* table);

    /**
     * @brief Tests if the function returns the correct service protocol.
     */
    bool TestProtocol(const InternetServiceTestTable* table);


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTERNETSERVICETEST_H_ */

