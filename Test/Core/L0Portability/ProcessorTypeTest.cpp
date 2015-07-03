/**
 * @file ProcessorTypeTest.cpp
 * @brief Source file for class ProcessorTypeTest
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
 * the class ProcessorTypeTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ProcessorTypeTest.h"
#include "GeneralDefinitions.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ProcessorTypeTest::TestAssignmentOperator() {

    bool result = true;

    ProcessorType ptTest;
    ptTest = 0xFD;
    result &= (ptTest.GetProcessorMask() == 0xFD);

    ptTest = ptSecond;
    result &= (ptTest.GetProcessorMask() == ptSecond.GetProcessorMask());

    return result;
}

bool ProcessorTypeTest::TestOROperator() {

    bool result = true;

    ProcessorType ptTest(ptSecond);
    ptTest |= ptAny;
    result &= (ptTest == ptAny);

    ptTest = ptSecond;
    ptTest |= ptFirst;
    result &= (ptTest == ptAny);

    ptTest = ptSecond;
    ptTest |= ptNone;
    result &= (ptTest == ptSecond);

    return result;

}

bool ProcessorTypeTest::TestEqualityOperator() {

    bool result = true;

    ProcessorType ptTest(ptSecond);
    result &= (ptTest == 0xFD);

    ptTest = 0xFD;
    result &= (ptTest == ptSecond);

    return result;

}

bool ProcessorTypeTest::TestInequalityOperator() {

    bool result = true;

    ProcessorType ptTest(0xFA);
    result &= (ptTest != 0xFD);

    result &= (ptTest != ptSecond);

    return result;

}

bool ProcessorTypeTest::TestGetSetDefaultCPUs() {
    ProcessorType ptTest(ptSecond);

    ptTest.SetDefaultCPUs(0xAA);
    uint32 test = ptTest.GetDefaultCPUs();

    return (test == 0xAA);
}

bool ProcessorTypeTest::TestConstructors() {
    ProcessorType ptDefault;             // Std contructor
    ProcessorType ptFromMask(0xFC);      // Mask constructor
    ProcessorType ptFromPT(ptFromMask);  // Constructor from other PT

    bool result = (ptDefault.GetProcessorMask() == 0xFE);
    result &= (ptFromMask.GetProcessorMask() == 0xFC);
    result &= (ptFromPT.GetProcessorMask() == 0xFC);

    return result;
}

bool ProcessorTypeTest::TestSetMask(uint32 mask) {
    ProcessorType test;
    test.SetMask(mask);
    return test.processorMask == mask;
}

bool ProcessorTypeTest::TestAddCPU(uint32 cpuNumber1,
                                   uint32 cpuNumber2) {
    ProcessorType test;
    test.SetMask(0);
    test.AddCPU(cpuNumber1);

    uint32 save = 1 << (cpuNumber1 - 1);
    if (test.processorMask != save) {
        return false;
    }

    test.AddCPU(cpuNumber2);
    save |= (1 << (cpuNumber2 - 1));
    if (test.processorMask != save) {
        return false;
    }

    //the maximum supported is 32 cpu.
    //if there is an overload it considers the rest.
    uint32 out = 33;
    test.AddCPU(out);

    save|=0x1;
    if (test.processorMask != save) {
        return false;
    }

    //0 is the same of 32.
    test.AddCPU(0);

    save|=0x80000000;

    if (test.processorMask != save) {
        return false;
    }

    return true;
}
