/**
 * @file BasicUDPSocketGTest.cpp
 * @brief Source file for class BasicUDPSocketGTest
 * @date 02/11/2015
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
 * the class BasicUDPSocketGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "BasicUDPSocketTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TesDefaultConstructor) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestDefaultConstructor());
}

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestSeek) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestSeek());
}

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestSize) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestSize());
}

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestRelativeSeek) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestRelativeSeek());
}

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestPosition) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestPosition());
}

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestSetSize) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestSetSize());
}

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestCanWrite) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestCanWrite());
}

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestCanRead) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestCanRead());
}

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestCanSeek) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestCanSeek());
}

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestIsValid) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestIsValid());
}

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestOpen) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestOpen());
}

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestClose) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestClose());
}

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestListen) {
    BasicUDPSocketTest basicUDPSocketTest;

    #ifndef MARTe2_REDUCED_TESTS
    const ConnectListenUDPTestTable table[]={
            {4444,2,true, true},
            {4444,32,true, true},
            {4444,64,true, true},
            {4444,128,true, true},
            {4444,32,false, false},
            {0,0,0}
    };
    #else
    const ConnectListenUDPTestTable table[]={
            {4444,2,true, true},
            {4444,4,true, true},
            //{4444,6,true, true},
            //{4444,8,true, true},
            {4444,2,false, false},
            {0,0,0}
    };
    #endif

    ASSERT_TRUE(basicUDPSocketTest.TestListen(table));
}

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestJoin) {
    BasicUDPSocketTest basicUDPSocketTest;

    #ifndef MARTe2_REDUCED_TESTS
    const ConnectListenUDPTestTable table[]={
            {4444,2,true, true},
            {4444,32,true, true},
            {4444,64,true, true},
            {4444,128,true, true},
            {4444,32,false, false},
            {0,0,0}
    };
    #else
    const ConnectListenUDPTestTable table[]={
            {4444,1,true, true},
            {4444,2,true, true},
            {4444,4,true, true},
            {4444,6,true, true},
            {4444,2,false, false},
            {0,0,0}
    };
    #endif

    ASSERT_TRUE(basicUDPSocketTest.TestJoin(table));
}


TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestConnect) {
    BasicUDPSocketTest basicUDPSocketTest;
    #ifndef MARTe2_REDUCED_TESTS
    const ConnectListenUDPTestTable table[]={
            {4444,2,true,true},
            {4444,32,true,true},
            {4444,64,true,true},
            {4444,128,true, true},
            {4444,32,false, false},
            {0,0,0}
    };
    #else
    const ConnectListenUDPTestTable table[]={
            {4444,1,true,true},
            {4444,2,true,true},
            {4444,4,true,true},
            {4444,8,true, true},
            {4444,2,false, false},
            {0,0,0}
    };
    #endif

    ASSERT_TRUE(basicUDPSocketTest.TestConnect(table));
}



TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestRead) {
    BasicUDPSocketTest basicUDPSocketTest;

    #ifndef MARTe2_REDUCED_TESTS
    const ReadWriteUDPTestTable table[]={
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,2,true,true,true,false,true},
            {"abcdefghil","abcde",5,5,4444,TTInfiniteWait,32,true,true,true,true,true},
            {"HelloWorld1","HelloWorld1",64,64,4444,TTInfiniteWait,64,true,true,true,false,true},
            {"HelloWorld2","HelloWorld2",32,32,4444,TTInfiniteWait,128,true,true,true,true,true},
            {"HelloWorld3","HelloWorld3",32,32,4444,1000,32,false,true,false,true,true},//{"HelloWorld3","HelloWorld3",32,32,4444,100,32,false,true,false,true,true},
            {"HelloWorld4","HelloWorld4",32,32,4444,TTInfiniteWait,32,false,false,false,true,true},
            {"HelloWorld5","HelloWorld5",32,32,4444,TTInfiniteWait,32,false,false,false,false,true},
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,128,false,true,true,true,false},
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,128,false,true,true,false,false},
            {0}
    };
    #else
    const ReadWriteUDPTestTable table[]={
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,1,true,true,true,false,true},
            {"abcdefghil","abcde",5,5,4444,TTInfiniteWait,2,true,true,true,true,true},
            {"HelloWorld1","HelloWorld1",64,64,4444,TTInfiniteWait,4,true,true,true,false,true},
            {"HelloWorld2","HelloWorld2",32,32,4444,TTInfiniteWait,8,true,true,true,true,true},
            {"HelloWorld3","HelloWorld3",32,32,4444,1000,2,false,true,false,true,true},//{"HelloWorld3","HelloWorld3",32,32,4444,100,32,false,true,false,true,true},
            {"HelloWorld4","HelloWorld4",32,32,4444,TTInfiniteWait,2,false,false,false,true,true},
            {"HelloWorld5","HelloWorld5",32,32,4444,TTInfiniteWait,2,false,false,false,false,true},
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,8,false,true,true,true,false},
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,8,false,true,true,false,false},
            {0}
    };
    #endif

    ASSERT_TRUE(basicUDPSocketTest.TestRead(table));
}

TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestPeek) {
    BasicUDPSocketTest basicUDPSocketTest;

    #ifndef MARTe2_REDUCED_TESTS
    const ReadWriteUDPTestTable table[]={
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,2,true,true,true,false,true},
            {"abcdefghil","abcde",5,5,4444,TTInfiniteWait,32,true,true,true,true,true},
            {"HelloWorld","HelloWorld",64,64,4444,TTInfiniteWait,64,true,true,true,false,true},
            {"HelloWorld","HelloWorld",32,32,4444,TTInfiniteWait,128,true,true,true,true,true},
            {"HelloWorld","HelloWorld",32,32,4444,TTInfiniteWait,32,false,false,false,false,true},
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,128,false,true,true,true,false},
            {0}
    };
    #else
    const ReadWriteUDPTestTable table[]={
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,1,true,true,true,false,true},
            {"abcdefghil","abcde",5,5,4444,TTInfiniteWait,2,true,true,true,true,true},
            {"HelloWorld","HelloWorld",64,64,4444,TTInfiniteWait,4,true,true,true,false,true},
            {"HelloWorld","HelloWorld",32,32,4444,TTInfiniteWait,8,true,true,true,true,true},
            {"HelloWorld","HelloWorld",32,32,4444,TTInfiniteWait,2,false,false,false,false,true},
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,8,false,true,true,true,false},
            {0}
    };
    #endif
    ASSERT_TRUE(basicUDPSocketTest.TestPeek(table));
}



TEST(FileSystem_L1Portability_BasicUDPSocketGTest,TestWrite) {
    BasicUDPSocketTest basicUDPSocketTest;

    #ifndef MARTe2_REDUCED_TESTS
    const ReadWriteUDPTestTable table[]={
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,2,true,true,true,false,true},
            {"abcdefghil","abcde",5,5,4444,TTInfiniteWait,32,true,true,true,true,true},
            {"HelloWorld","HelloWorld",64,64,4444,TTInfiniteWait,64,true,true,true,false,true},
            {"HelloWorld","HelloWorld",32,32,4444,TTInfiniteWait,128,true,true,true,true,true},
       /*     {"HelloWorld","HelloWorld",0xffffffff,32,4444,100,32,false,true,false,true,true},
            {"HelloWorld","HelloWorld",0xffffffff,32,4444,TTInfiniteWait,32,false,false,false,true,true},
            {"HelloWorld","HelloWorld",0xffffffff,32,4444,TTInfiniteWait,32,false,false,false,false,true},*/
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,128,false,true,true,true,false},
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,128,false,true,true,false,false},
            {0}
    };
    #else
    const ReadWriteUDPTestTable table[]={
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,1,true,true,true,false,true},
            {"abcdefghil","abcde",5,5,4444,TTInfiniteWait,2,true,true,true,true,true},
            {"HelloWorld","HelloWorld",64,64,4444,TTInfiniteWait,4,true,true,true,false,true},
            {"HelloWorld","HelloWorld",32,32,4444,TTInfiniteWait,8,true,true,true,true,true},
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,8,false,true,true,true,false},
            {"HelloWorld","HelloWorld",11,11,4444,TTInfiniteWait,8,false,true,true,false,false},
            {0}
    };
    #endif

    ASSERT_TRUE(basicUDPSocketTest.TestWrite(table));
}

