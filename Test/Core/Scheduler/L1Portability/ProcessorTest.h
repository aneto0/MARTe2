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
using namespace MARTe;

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
     * @param[in] testEqual is used to test if processor functions return values equal or different
     * to the test input arguments.
     */
    ProcessorTest(bool testEqual=false);

    /**
     * @brief Tests the Processor::VendorId() function.
     * @param[in] vendorIdTest is the value which will be compared with the value returned by Processor::VendorId() function.
     * @return If testEqual is false returns true if Processor::VendorId() returns a vendor id different from vendorIdTest, otherwise
     * if testEqual is true returns true if Processor::VendorId() returns a vendor id equal to vendorIdTest.
     */
    bool TestVendorId(const char8 * vendorIdTest);

    /**
     * @brief Tests the Processor::Family() function.
     * @param[in] familyTest is the value which will be compared with the value returned by Processor::Family() function.
     * @return If testEqual is false returns true if Processor::Family() returns a value different from familyTest, otherwise
     * if testEqual is true returns true if Processor::Family() returns a value equal to familyTest.
     */
    bool TestFamily(uint32 familyTest);

    /**
     * @brief Tests the model function.
     * @param[in] modelTest is the value which will be compared with the value returned by Processor::Model() function.
     * @return If testEqual is false returns true if Processor::Model() returns a value different from modelTest, otherwise
     * if testEqual is true returns true if Processor::Model() returns a value equal to modelTest.
     */
    bool TestModel(uint32 modelTest);

    /**
     * @brief Tests the available function.
     * @param[in] availableCPUTest is the value which will be compared with the value returned by Processor::Available() function.
     * @return If testEqual is false returns true if Processor::Available() returns a value different from availableCPUTest, otherwise
     * if testEqual is true returns true if Processor::Available() returns a value equal to availableCPUTest.
     */
    bool TestAvailable(uint32 availableCPUTest);

private:

    /**
     * Specifies if the tests must return true if the values compared are equal (true)
     * or different (false).
     */
    bool testEqual;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PROCESSORTEST_H_ */
