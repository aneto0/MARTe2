/**
 * @file ProcessorTest.cpp
 * @brief Source file for class ProcessorTest
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

 * @details This source file contains the definition of all the methods for
 * the class ProcessorTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ProcessorTest.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

ProcessorTest::ProcessorTest(bool testEqual) {
    this->testEqual=testEqual;
}

bool ProcessorTest::TestVendorId(const char8* vendorIdTest) {
    const char8 * vendorId = NULL;

    vendorId = Processor::VendorId();

    //return true if testEqual & vendorId==vendorIdTest || !testEqual & !vendorIdTest
    return !(testEqual^(StringHelper::Compare(vendorId,vendorIdTest)));
}

bool ProcessorTest::TestFamily(uint32 familyTest) {
    uint32 family;

    family = Processor::Family();

    return !(testEqual^(family == familyTest));
}

bool ProcessorTest::TestModel(uint32 modelTest) {
    uint32 model;

    model = Processor::Model();
    return !(testEqual^(model == modelTest));
}

bool ProcessorTest::TestAvailable(uint32 availableCPUTest) {
    uint32 available;

    available = Processor::Available();
    return !(testEqual^(available == availableCPUTest));
}
