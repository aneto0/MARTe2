/**
 * @file MemoryMapBrokerGTest.cpp
 * @brief Source file for class MemoryMapBrokerGTest
 * @date 30/07/2016
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

 * @details This source file contains the definition of all the methods for
 * the class MemoryMapBrokerGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MemoryMapBrokerTest.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L5GAMs_MemoryMapBrokerGTest,TestConstructor) {
    MemoryMapBrokerTest test;
    ASSERT_TRUE(test.TestConstructor());
}

TEST(BareMetal_L5GAMs_MemoryMapBrokerGTest,TestInit_Input) {
    MemoryMapBrokerTest test;
    ASSERT_TRUE(test.TestInit_Input());
}

TEST(BareMetal_L5GAMs_MemoryMapBrokerGTest,TestInit_Input_Ranges) {
    MemoryMapBrokerTest test;
    ASSERT_TRUE(test.TestInit_Input_Ranges());
}

TEST(BareMetal_L5GAMs_MemoryMapBrokerGTest,TestInit_Input_Samples) {
    MemoryMapBrokerTest test;
    ASSERT_TRUE(test.TestInit_Input_Samples());
}

TEST(BareMetal_L5GAMs_MemoryMapBrokerGTest,TestInit_Output) {
    MemoryMapBrokerTest test;
    ASSERT_TRUE(test.TestInit_Output());
}

TEST(BareMetal_L5GAMs_MemoryMapBrokerGTest,TestInit_Output_Ranges) {
    MemoryMapBrokerTest test;
    ASSERT_TRUE(test.TestInit_Output_Ranges());
}

TEST(BareMetal_L5GAMs_MemoryMapBrokerGTest,TestInit_Output_Samples) {
    MemoryMapBrokerTest test;
    ASSERT_TRUE(test.TestInit_Output_Samples());
}
