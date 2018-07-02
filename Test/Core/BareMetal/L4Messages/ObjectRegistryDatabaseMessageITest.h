/**
 * @file ObjectRegistryDatabaseMessageITest.h
 * @brief Header file for class ObjectRegistryDatabaseMessageITest
 * @date 19/06/2018
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

 * @details This header file contains the declaration of the class ObjectRegistryDatabaseMessageITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OBJECTREGISTYDATABASEMESSAGEITEST_H_
#define OBJECTREGISTYDATABASEMESSAGEITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MessageI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Tests the ObjectRegistryDatabaseMessageI public methods.
 */
class ObjectRegistryDatabaseMessageITest {
public:

    /**
     * @brief Default constructor
     */
    ObjectRegistryDatabaseMessageITest();

    /**
     * @brief Destructor
     */
    virtual ~ObjectRegistryDatabaseMessageITest();

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OBJECTREGISTYDATABASEMESSAGEITEST_H_ */

