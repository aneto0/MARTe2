/**
 * @file ProcessorTypeGTest.cpp
 * @brief Source file for class ProcessorTypeGTest
 * @date 25/06/2015
 * @author Giuseppe Ferro
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
 * the class ProcessorTypeGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "ProcessorTypeTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L1Portability_ProcessorTypeGTest,TestAssignmentOperator) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestAssignmentOperator());
}

TEST(BareMetal_L1Portability_ProcessorTypeGTest,TestOROperator) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestOROperator());
}

TEST(BareMetal_L1Portability_ProcessorTypeGTest,TestEqualityOperator) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestEqualityOperator());
}

TEST(BareMetal_L1Portability_ProcessorTypeGTest,TestInequalityOperator) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestInequalityOperator());
}

TEST(BareMetal_L1Portability_ProcessorTypeGTest,TestSetGetDefaultCPUs) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestSetGetDefaultCPUs());
}


TEST(BareMetal_L1Portability_ProcessorTypeGTest,TestDefaultConstructor) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestDefaultConstructor());
}

TEST(BareMetal_L1Portability_ProcessorTypeGTest,TestConstructorFromMask) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestConstructorFromMask());
}

TEST(BareMetal_L1Portability_ProcessorTypeGTest,TestConstructorFromProcessorType) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestConstructorFromProcessorType());
}


TEST(BareMetal_L1Portability_ProcessorTypeGTest,TestSetMask) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestSetMask(0xff));
}

TEST(BareMetal_L1Portability_ProcessorTypeGTest,TestAddCPU) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestAddCPU(3,9));
}

TEST(BareMetal_L1Portability_ProcessorTypeGTest,TestGetProcessorMask) {
    ProcessorTypeTest processorTypeTest;
    ASSERT_TRUE(processorTypeTest.TestGetProcessorMask());
}
