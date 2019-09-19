/**
 * @file MultipleEventSemTest.h
 * @brief Header file for class MultipleEventSemTest
 * @date 26/07/2015
 * @author Giuseppe Ferr�
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

 * @details This header file contains the declaration of the class MultipleEventSemTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SEMAPHORETEST_H_
#define SEMAPHORETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "MultipleEventSem.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the MultipleEventSem functions.
 */
namespace MultipleEventSemTest {

    /**
     * @brief Tests the MultipleEventSem::Create function.
     * @return true if the semaphore is successfully created which also implies that a non NULL handle is created.
     */
    bool TestAddEvent();

    /**
     * @brief Tests the MultipleEventSem::Close function.
     * @return true if the semaphore is successfully closed.
     */
    bool TestWait();

    /**
     * @brief Tests the MultipleEventSem::IsClosed function
     * @return true if the semaphore can report effectively if it is closed or not
     */
    bool TestReset();

    /**
     *
     */
    bool TestMultiWait_Threads(uint32 nOfThreads,MilliSeconds timeout,FILE *errorDetails);

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SEMAPHORETEST_H_ */

