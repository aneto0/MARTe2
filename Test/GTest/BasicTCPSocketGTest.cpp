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

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "gtest/gtest.h"
#include "BasicTCPSocketTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BasicTCPSocketGTest,TesDefaultConstructor) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestDefaultConstructor());
}

TEST(BasicTCPSocketGTest,TestSeek) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestSeek());
}

TEST(BasicTCPSocketGTest,TestSize) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestSize());
}

TEST(BasicTCPSocketGTest,TestRelativeSeek) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestRelativeSeek());
}

TEST(BasicTCPSocketGTest,TestPosition) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestPosition());
}

TEST(BasicTCPSocketGTest,TestSetSize) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestSetSize());
}

TEST(BasicTCPSocketGTest,TestCanWrite) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestCanWrite());
}

TEST(BasicTCPSocketGTest,TestCanRead) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestCanRead());
}

TEST(BasicTCPSocketGTest,TestCanSeek) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestCanSeek());
}

// Note: in linux (at least on my pc) the the listen accepts +3 clients than the max specified


const ConnectListenTestTable tableListenConnect[]={
        {"a",4444,0, 3, 10, true,true, true},
        {"a",4444,0, 4, 10, false,true, true},
        {"a",4444,28, 32, 10, false,true, true},
        {"a",4444,29, 32, 10, true,true, true},
        {"a",4444,60, 64, 10, false,true, true},
        {"a",4444,60, 128, 10, false,true, true},
        {"a",4444,128, 60, 10, true,true, true},
        {"a",4444,128, 128, 10, true,true, true},
        {"a",4444,128, 128, 10, false, true, false},
        {"a",4444,0, 3, 10, true,false, true},
                {"a",4444,0, 4, 10, false,false, true},
                {"a",4444,28, 32, 10, false,false, true},
                {"a",4444,29, 32, 10, true,false, true},
                {"a",4444,60, 64, 10, false,false, true},
                {"a",4444,60, 128, 10, false,false, true},
                {"a",4444,128, 60, 10, true,false, true},
                {"a",4444,128, 128, 10, true,false, true},
        {"a",4444,128, 128, 10, false, false, false},
        {0,0,0,0,0,0,0,0}
};


const ConnectListenTestTable tableListenConnectByServiceName[]={
        {"nsca",0,0, 3, 10, true,true, true},
        {"nsca",0,0, 4, 10, false,true, true},
        {"nsca",0,28, 32, 10, false,true, true},
        {"nsca",0,29, 32, 10, true,true, true},
        {"nsca",0,60, 64, 10, false,true, true},
        {"nsca",0,60, 128, 10, false,true, true},
        {"nsca",0,128, 60, 10, true,true, true},
        {"nsca",0,128, 128, 10, true,true, true},
        {"nsca",0,128, 128, 10, false, true, false},
        {"fuffa",0,128, 128, 10, false, true, true},
        {"nsca",0,0, 3, 10, true,false, true},
                {"nsca",0,0, 4, 10, false,false, true},
                {"nsca",0,28, 32, 10, false,false, true},
                {"nsca",0,29, 32, 10, true,false, true},
                {"nsca",0,60, 64, 10, false,false, true},
                {"nsca",0,60, 128, 10, false,false, true},
                {"nsca",0,128, 60, 10, true,false, true},
                {"nsca",0,128, 128, 10, true,false, true},
                {"fuffa",0,128, 128, 10, false,false, true},
        {"nsca",0,128, 128, 10, false, false, false},
        {0,0,0,0,0,0,0,0}
};

TEST(BasicTCPSocketGTest,TestListen) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestListen(tableListenConnect));
}

TEST(BasicTCPSocketGTest,TestListenByServiceName) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestListenByServiceName(tableListenConnectByServiceName));
}

TEST(BasicTCPSocketGTest,TestConnect) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestListen(tableListenConnect));
}

TEST(BasicTCPSocketGTest,TestConnectByServiceName) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestListenByServiceName(tableListenConnectByServiceName));
}




TEST(BasicTCPSocketGTest,TestRead) {
    BasicTCPSocketTest basicTCPSocketTest;

    const ReadWriteTestTable table[]={
            {11,11,"HelloWorld", "HelloWorld", 2, TTInfiniteWait, true, true, true, false},
            {5,5,"HelloWorld", "Hello", 2, TTInfiniteWait, true, true, true, true},
            {32,32,"HelloWorld","HelloWorld", 2, TTInfiniteWait, true, true, true, false},
            {66,64,"HelloWorld","HelloWorld", 2, TTInfiniteWait, true, true, true, true},
            {11,11,"HelloWorld", "HelloWorld", 32, TTInfiniteWait, true, true, true, true},
            {5,5,"HelloWorld", "Hello", 32, TTInfiniteWait, true, true, true, false},
            {32,32,"HelloWorld","HelloWorld", 32, TTInfiniteWait, true, true, true, false},
            {66,64,"HelloWorld","HelloWorld", 32, TTInfiniteWait, true, true, true, false},
            {11,11,"HelloWorld","HelloWorld", 32, 100, false, true, false, true},
            {11,11,"HelloWorld","HelloWorld", 32, 100, false, false, false, true},
            {0,0,0,0,0,0,0,0,0}
    };

    ASSERT_TRUE(basicTCPSocketTest.TestRead(table));
}


TEST(BasicTCPSocketGTest,TestPeek) {
    BasicTCPSocketTest basicTCPSocketTest;

    const ReadWriteTestTable table[]={
            {11,11,"HelloWorld", "HelloWorld", 2, TTInfiniteWait, true, true, true, false},
            {5,5,"HelloWorld", "Hello", 2, TTInfiniteWait, true, true, true, true},
            {32,32,"HelloWorld","HelloWorld", 2, TTInfiniteWait, true, true, true, false},
            {66,64,"HelloWorld","HelloWorld", 2, TTInfiniteWait, true, true, true, true},
            {11,11,"HelloWorld", "HelloWorld", 32, TTInfiniteWait, true, true, true, true},
            {5,5,"HelloWorld", "Hello", 32, TTInfiniteWait, true, true, true, false},
            {32,32,"HelloWorld","HelloWorld", 32, TTInfiniteWait, true, true, true, false},
            {66,64,"HelloWorld","HelloWorld", 32, TTInfiniteWait, true, true, true, false},
            {0,0,0,0,0,0,0,0,0}
    };

    ASSERT_TRUE(basicTCPSocketTest.TestPeek(table));
}



TEST(BasicTCPSocketGTest,TestWrite) {
    BasicTCPSocketTest basicTCPSocketTest;

    const ReadWriteTestTable table[]={
            {11,11,"HelloWorld", "HelloWorld", 2, TTInfiniteWait, true, true, true, false},
            {5,5,"HelloWorld", "Hello", 2, TTInfiniteWait, true, true, true, true},
            {32,32,"HelloWorld","HelloWorld", 2, TTInfiniteWait, true, true, true, false},
            {66,64,"HelloWorld","HelloWorld", 2, TTInfiniteWait, true, true, true, true},
            {11,11,"HelloWorld", "HelloWorld", 32, TTInfiniteWait, true, true, true, true},
            {5,5,"HelloWorld", "Hello", 32, TTInfiniteWait, true, true, true, false},
            {32,32,"HelloWorld","HelloWorld", 32, TTInfiniteWait, true, true, true, false},
            {66,64,"HelloWorld","HelloWorld", 32, TTInfiniteWait, true, true, true, false},
            {11,11,"HelloWorld","HelloWorld", 32, 100, false, true, false, true},
            {11,11,"HelloWorld","HelloWorld", 32, 100, false, false, false, true},
            {0,0,0,0,0,0,0,0,0}
    };

    ASSERT_TRUE(basicTCPSocketTest.TestWrite(table));
}



TEST(BasicTCPSocketGTest,TestWaitConnection) {
    BasicTCPSocketTest basicTCPSocketTest;
    const WaitConnectionTestTable table[]={
            {10, true, TTInfiniteWait,true, true},
            {10, false, TTInfiniteWait,true, true},
            {0, false, 100, true, false},
            {0, false, 100, false, false},
            {10, false, 1000, true, true},
            {0,0,0,0}
    };

    ASSERT_TRUE(basicTCPSocketTest.TestWaitConnection(table));
}


TEST(BasicTCPSocketGTest,TestIsValid) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestIsValid());
}

TEST(BasicTCPSocketGTest,TestIsConnected) {
    BasicTCPSocketTest basicTCPSocketTest;
    ASSERT_TRUE(basicTCPSocketTest.TestIsConnected(true, true));
    ASSERT_TRUE(basicTCPSocketTest.TestIsConnected(false, false));
}



