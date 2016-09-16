/**
 * @file QueuedMessageITest.h
 * @brief Header file for class QueuedMessageITest
 * @date 16/09/2016
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

 * @details This header file contains the declaration of the class QueuedMessageITest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef QUEUEDMESSAGEITEST_H_
#define QUEUEDMESSAGEITEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "QueuedMessageI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Tests the QueuedMessageI public methods.
 */
class QueuedMessageITest {
public:

    /**
     * @brief Default constructor
     */
    QueuedMessageITest();

    /**
     * @brief Destructor
     */
    virtual ~QueuedMessageITest();

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the Start method.
     */
    bool TestStart();

    /**
     * @brief Verifies that if Start is not called then the remote function is not executed.
     */
    bool TestStart2();

    /**
     * @brief Tests the Stop method.
     */
    bool TestStop();

    /**
     * @brief Tests the InstallMessageFilterInQueue method
     */
    bool TestInstallMessageFilterInQueue();

    /**
     * @brief Tests the RemoveMessageFilterInQueue method
     */
    bool TestRemoveMessageFilterInQueue();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MESSAGEITEST_H_ */

