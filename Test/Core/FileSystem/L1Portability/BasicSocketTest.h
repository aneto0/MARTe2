/**
 * @file BasicSocketTest.h
 * @brief Header file for class BasicSocketTest
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

 * @details This header file contains the declaration of the class BasicSocketTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICSOCKETTEST_H_
#define BASICSOCKETTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BasicTCPSocket.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the BasicSocket public functions.
 */
class BasicSocketTest {
public:

    /**
     * @brief Tests if the source and destination hosts are correctly initialized.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests if the function returns true if the socket handle is valid.
     */
    bool TestSetBlocking(bool isBlock);

    /**
     * @brief Tests if the function returns true if the socket handle is invalid.
     */
    bool TestSetBlocking_false(bool isBlock);

    /**
     * @brief Tests if the function returns true if the socket handle is valid.
     */
    bool TestClose();

    /**
     * @brief Tests if the function returns true if the socket handle is invalid.
     */
    bool TestClose_false();

    /**
     * @brief Tests if the function returns the source host.
     */
    bool TestGetSource();

    /**
     * @brief Tests if the function returns the destination host.
     */
    bool TestGetDestination();

    /**
     * @brief Tests if the function sets the source host correctly.
     */
    bool TestSetSource();

    /**
     * @brief Tests if the function sets the destination host correctly.
     */
    bool TestSetDestination();


    /**
     * @brief Tests if the function returns true if the socket is in blocking mode and false otherwise.
     */
    bool TestIsBlocking(bool isBlocked);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICSOCKETTEST_H_ */

