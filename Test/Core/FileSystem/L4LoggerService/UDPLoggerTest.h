/**
 * @file UDPLoggerTest.h
 * @brief Header file for class UDPLoggerTest
 * @date 15/03/2017
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class UDPLoggerTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef UDPLOGGERTEST_H_
#define UDPLOGGERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the UDPLogger public methods.
 */
class UDPLoggerTest {
public:
    /**
     * @brief Tests the default constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests the initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the initialise method without specifying a port.
     */
    bool TestInitialise_False_Port();

    /**
     * @brief Tests the initialise method without specifying an address.
     */
    bool TestInitialise_False_Address();

    /**
     * @brief Tests the initialise method without specifying an unrecheable address/port.
     */
    bool TestInitialise_False_Address_Port();

    /**
     * @brief Tests the ConsumeLogMessage method .
     */
    bool TestConsumeLogMessage();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* UDPLOGGERTEST_H_ */

