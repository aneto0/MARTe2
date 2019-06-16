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

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "BasicUDPSocketTest.h"
#include "TestSupport.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BasicUDPSocketGTest,TesDefaultConstructor) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestDefaultConstructor());
}

TEST(BasicUDPSocketGTest,TestSeek) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestSeek());
}

TEST(BasicUDPSocketGTest,TestSize) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestSize());
}

TEST(BasicUDPSocketGTest,TestRelativeSeek) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestRelativeSeek());
}

TEST(BasicUDPSocketGTest,TestPosition) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestPosition());
}

TEST(BasicUDPSocketGTest,TestSetSize) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestSetSize());
}

TEST(BasicUDPSocketGTest,TestCanWrite) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestCanWrite());
}

TEST(BasicUDPSocketGTest,TestCanRead) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestCanRead());
}

TEST(BasicUDPSocketGTest,TestCanSeek) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestCanSeek());
}

TEST(BasicUDPSocketGTest,TestIsValid) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestIsValid());
}

TEST(BasicUDPSocketGTest,TestOpen) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestOpen());
}

TEST(BasicUDPSocketGTest,TestClose) {
    BasicUDPSocketTest basicUDPSocketTest;
    ASSERT_TRUE(basicUDPSocketTest.TestClose());
}

TEST(BasicUDPSocketGTest,TestListen) {
    BasicUDPSocketTest basicUDPSocketTest;

    const ConnectListenUDPTestTable table[]={
            {4444,2,true, true},
            {4444,32,true, true},
            {4444,64,true, true},
            {4444,128,true, true},
            {4444,32,false, false},
            {0,0,0}
    };

    ASSERT_TRUE(basicUDPSocketTest.TestListen(table));
}


TEST(BasicUDPSocketGTest,TestConnect) {
    BasicUDPSocketTest basicUDPSocketTest;

    const ConnectListenUDPTestTable table[]={
            {4444,2,true,true},
            {4444,32,true,true},
            {4444,64,true,true},
            {4444,128,true, true},
            {4444,32,false, false},
            {0,0,0}
    };

    ASSERT_TRUE(basicUDPSocketTest.TestConnect(table));
}



TEST(BasicUDPSocketGTest,TestRead) {
    BasicUDPSocketTest basicUDPSocketTest;


    const ReadWriteUDPTestTable table[]={
            {"HelloWorld" ,"HelloWorld" ,11,11,4444,MilliSeconds::Infinite,2,true,true,true,false,true},
            {"abcdefghil" ,"abcde"      , 5, 5,4444,MilliSeconds::Infinite,32,true,true,true,true,true},
            {"HelloWorld1","HelloWorld1",64,64,4444,MilliSeconds::Infinite,64,true,true,true,false,true},
            {"HelloWorld2","HelloWorld2",32,32,4444,MilliSeconds::Infinite,128,true,true,true,true,true},
            {"HelloWorld3","HelloWorld3",32,32,4444,MilliSeconds(1000,Units::ms),32,false,true,false,true,true},//{"HelloWorld3","HelloWorld3",32,32,4444,100,32,false,true,false,true,true},
            {"HelloWorld4","HelloWorld4",32,32,4444,MilliSeconds::Infinite,32,false,false,false,true,true},
            {"HelloWorld5","HelloWorld5",32,32,4444,MilliSeconds::Infinite,32,false,false,false,false,true},
            {"HelloWorld" ,"HelloWorld" ,11,11,4444,MilliSeconds::Infinite,128,false,true,true,true,false},
            {"HelloWorld" ,"HelloWorld" ,11,11,4444,MilliSeconds::Infinite,128,false,true,true,false,false},
            {emptyString  ,emptyString  , 0, 0,   0,MilliSeconds::Infinite,128,false,false,false,false,false}
    };

    ASSERT_TRUE(basicUDPSocketTest.TestRead(table));
}

TEST(BasicUDPSocketGTest,TestPeek) {
    BasicUDPSocketTest basicUDPSocketTest;


    const ReadWriteUDPTestTable table[]={
            {"HelloWorld","HelloWorld",11,11,4444,MilliSeconds::Infinite,2,true,true,true,false,true},
            {"abcdefghil","abcde",5,5,4444,MilliSeconds::Infinite,32,true,true,true,true,true},
            {"HelloWorld","HelloWorld",64,64,4444,MilliSeconds::Infinite,64,true,true,true,false,true},
            {"HelloWorld","HelloWorld",32,32,4444,MilliSeconds::Infinite,128,true,true,true,true,true},
            {"HelloWorld","HelloWorld",32,32,4444,MilliSeconds::Infinite,32,false,false,false,false,true},
            {"HelloWorld","HelloWorld",11,11,4444,MilliSeconds::Infinite,128,false,true,true,true,false},
            {emptyString  ,emptyString  , 0, 0,   0,MilliSeconds::Infinite,128,false,false,false,false,false}
    };

    ASSERT_TRUE(basicUDPSocketTest.TestPeek(table));
}



TEST(BasicUDPSocketGTest,TestWrite) {
    BasicUDPSocketTest basicUDPSocketTest;


    const ReadWriteUDPTestTable table[]={
            {"HelloWorld","HelloWorld",11,11,4444,MilliSeconds::Infinite,2,true,true,true,false,true},
            {"abcdefghil","abcde",5,5,4444,MilliSeconds::Infinite,32,true,true,true,true,true},
            {"HelloWorld","HelloWorld",64,64,4444,MilliSeconds::Infinite,64,true,true,true,false,true},
            {"HelloWorld","HelloWorld",32,32,4444,MilliSeconds::Infinite,128,true,true,true,true,true},
       /*     {"HelloWorld","HelloWorld",0xffffffff,32,4444,100,32,false,true,false,true,true},
            {"HelloWorld","HelloWorld",0xffffffff,32,4444,MilliSeconds::Infinite,32,false,false,false,true,true},
            {"HelloWorld","HelloWorld",0xffffffff,32,4444,MilliSeconds::Infinite,32,false,false,false,false,true},*/
            {"HelloWorld","HelloWorld",11,11,4444,MilliSeconds::Infinite,128,false,true,true,true,false},
            {"HelloWorld","HelloWorld",11,11,4444,MilliSeconds::Infinite,128,false,true,true,false,false},
            {emptyString  ,emptyString  , 0, 0,   0,MilliSeconds::Infinite,128,false,false,false,false,false}
    };

    ASSERT_TRUE(basicUDPSocketTest.TestWrite(table));
}
