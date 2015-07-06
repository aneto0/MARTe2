/**
 * @file ProcessorTest.h
 * @brief Header file for class ProcessorTest
 * @date 25/06/2015
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

 * @details This header file contains the declaration of the class ProcessorTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PROCESSORTEST_H_
#define PROCESSORTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Processor.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the Processor functions.
 */
class ProcessorTest {

public:

    /**
     * @brief Constructor
     */
    ProcessorTest();

    /**
     * @brief Tests the vendor identifier function.
     * @return true if Processor::VendorId() returns a vendor id which is not NULL.
     */
    bool TestVendorId();

    /**
     * @brief Tests the family function.
     * @return true if Processor::Family() returns a value which is not -1.
     */
    bool TestFamily();

    /**
     * @brief Tests the model function.
     * @return true if Processor::Model() returns a value which is not -1.
     */
    bool TestModel();

    /**
     * @brief Tests the available function.
     * @return true if Processor::Available() returns a value which is not -1.
     */
    bool TestAvailable();

};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PROCESSORTEST_H_ */

