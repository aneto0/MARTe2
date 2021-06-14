/**
 * @file BasicTCPSocketGTest.cpp
 * @brief Source file for class BasicTCPSocketGTest
 * @date 29/10/2015
 * @author Giuseppe Ferrò
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
 * the class BasicTCPSocketGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "BasicTCPSocketTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TesDefaultConstructor) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestDefaultConstructor());
}

TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestSeek) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestSeek());
}

TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestSize) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestSize());
}

TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestRelativeSeek) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestRelativeSeek());
}

TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestPosition) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestPosition());
}

TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestSetSize) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestSetSize());
}

TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestCanWrite) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestCanWrite());
}

TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestCanRead) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestCanRead());
}

TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestCanSeek) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestCanSeek());
}

// Note: in linux (at least on my pc) the the listen accepts +3 clients than the max specified
const ConnectListenTestTable tableListenConnect[]={
        {4444,3, 3, 100, true,true, true, true},
        {4446,0, 4, 10, false,true, true, true},
        {4448,28, 32, 10, false,true, true, true},
        {4450,32, 32, 100, true,true, true, true},
        {4452,60, 64, 10, false,true, true, true},
        {4454,60, 128, 10, false,true, true, true},
        {4456,128, 60, 100, true,true, true, true},
        {4458,128, 128, 100, true,true, true, true},
        {4460,128, 128, 10, false, true, false, true},
        {4462,3, 3, TTInfiniteWait, true,false, true, true},
        {4464,4, 4, TTInfiniteWait, true,false, true, true},
        {4466,32, 32, TTInfiniteWait, true,false, true, true},
        {4468,32, 30, TTInfiniteWait, true,false, true, true},
        {4470,64, 64, TTInfiniteWait, true,false, true, true},
        {4472,128, 128, TTInfiniteWait, true,false, true, true},
        {4474,128, 60, TTInfiniteWait, true,false, true, true},
        {4476,128, 128, TTInfiniteWait, true,false, true, true},
        {4478,128, 128, TTInfiniteWait, false, false, false, true},
        {4480,128, 60, 10, false, true, true, false},
        {0,0,0,0,0,0,0,0}
};


TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestListen) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestListen(tableListenConnect));
}


TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestConnect) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestConnect(tableListenConnect));
}

TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestRead) {
    BasicTCPSocketTest basicTCPSocketTest;

    const ReadWriteTestTable table[]={
            {11,11,"HelloWorld", "HelloWorld", 2, TTInfiniteWait, true, true, true, false,true},
            {5,5,"HelloWorld", "Hello", 2, TTInfiniteWait, true, true, true, true,true},
            {32,32,"HelloWorld","HelloWorld", 2, TTInfiniteWait, true, true, true, false,true},
            {64,64,"HelloWorld","HelloWorld", 2, TTInfiniteWait, true, true, true, false,true},
            {11,11,"HelloWorld", "HelloWorld", 32, TTInfiniteWait, true, true, true, true,true},
            {5,5,"HelloWorld", "Hello", 32, TTInfiniteWait, true, true, true, false,true},
            {32,32,"HelloWorld","HelloWorld", 32, TTInfiniteWait, true, true, true, false,true},
            {64,64,"HelloWorld","HelloWorld", 32, TTInfiniteWait, true, true, true, false,true},
            {11,11,"HelloWorld","HelloWorld", 32, 100, false, true, false, true,true},
            {11,11,"HelloWorld","HelloWorld", 32, TTInfiniteWait, false, false, false, true,true},
            {5,5,"HelloWorld", "Hello", 32, TTInfiniteWait, false, true, true, true,false},
            {11,11,"HelloWorld", "HelloWorld", 2, TTInfiniteWait, false, true, true, false,false},
            {0,0,0,0,0,0,0,0,0}
    };

    ASSERT_TRUE(basicTCPSocketTest.TestRead(table));
}


TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestPeek) {
    BasicTCPSocketTest basicTCPSocketTest;

    const ReadWriteTestTable table[]={
            {11,11,"HelloWorld", "HelloWorld", 2, TTInfiniteWait, true, true, true, false,true},
            {5,5,"HelloWorld", "Hello", 2, TTInfiniteWait, true, true, true, true,true},
            {32,32,"HelloWorld","HelloWorld", 2, TTInfiniteWait, true, true, true, false,true},
            {64,64,"HelloWorld","HelloWorld", 2, TTInfiniteWait, true, true, true, false,true},
            {11,11,"HelloWorld", "HelloWorld", 32, TTInfiniteWait, true, true, true, true,true},
            {5,5,"HelloWorld", "Hello", 32, TTInfiniteWait, true, true, true, false,true},
            {32,32,"HelloWorld","HelloWorld", 32, TTInfiniteWait, true, true, true, false,true},
            {64,64,"HelloWorld","HelloWorld", 32, TTInfiniteWait, true, true, true, false,true},
            {5,5,"HelloWorld", "Hello", 32, TTInfiniteWait, false, true, true, true,false},
            {11,11,"HelloWorld", "HelloWorld", 2, TTInfiniteWait, false, true, true, false,false},
            {0,0,0,0,0,0,0,0,0}
    };

    ASSERT_TRUE(basicTCPSocketTest.TestPeek(table));
}



TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestWrite) {
    BasicTCPSocketTest basicTCPSocketTest;

    const ReadWriteTestTable table[]={
            {11,11,"HelloWorld", "HelloWorld", 2, TTInfiniteWait, true, true, true, false, true},
            {5,5,"HelloWorld", "Hello", 2, TTInfiniteWait, true, true, true, true, true},
            {32,32,"HelloWorld","HelloWorld", 2, TTInfiniteWait, true, true, true, false, true},
            {64,64,"HelloWorld","HelloWorld", 2, TTInfiniteWait, true, true, true, true, true},
            {11,11,"HelloWorld", "HelloWorld", 32, TTInfiniteWait, true, true, true, true, true},
            {5,5,"HelloWorld", "Hello", 32, TTInfiniteWait, true, true, true, false, true},
            {32,32,"HelloWorld","HelloWorld", 32, TTInfiniteWait, true, true, true, false, true},
            {64,64,"HelloWorld","HelloWorld", 32, TTInfiniteWait, true, true, true, false, true},
            {64,64,"HelloWorld","HelloWorld", 32, 50, true, true, true, false, true},
            {11,11,"HelloWorld","HelloWorld", 32, 10, true, true, false, true, true},
            //{0x7fffffff,11,"HelloWorld","HelloWorld", 32, TTInfiniteWait, false, false, false, true, true},
            {5,5,"HelloWorld", "Hello", 32, TTInfiniteWait, false, true, true, true,false},
            {11,11,"HelloWorld", "HelloWorld", 2, TTInfiniteWait, false, true, true, false,false},
            {0,0,0,0,0,0,0,0,0}
    };

    ASSERT_TRUE(basicTCPSocketTest.TestWrite(table));
}



TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestWaitConnection) {
    BasicTCPSocketTest basicTCPSocketTest;
    const WaitConnectionTestTable table[]={
            {10, true, TTInfiniteWait,true, true, true},
            {10, false, TTInfiniteWait,true, true, true},
            {0, false, 100, true, false, true},
            {0, false, 100, false, false, true},
            {10, false, 1000, true, true, true},
            {10, true, TTInfiniteWait,true, false, false},
            {0,0,0,0,0,0}
    };

    ASSERT_TRUE(basicTCPSocketTest.TestWaitConnection(table));
}


TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestIsValid) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestIsValid());
}

TEST(FileSystem_L1Portability_BasicTCPSocketGTest,TestIsConnected) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestIsConnected(true, true));
    ASSERT_TRUE(basicTCPSocketTest.TestIsConnected(false, false));
}



