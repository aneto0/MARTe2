/**
 * @file InternetAddressGTest.cpp
 * @brief Source file for class InternetAddressGTest
 * @date 28/10/2015
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
 * the class InternetAddressGTest (public, protected, and private). Be aware that some 
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
#include "InternetAddressTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



TEST(InternetAddressGTest,TesDefaultConstructor) {
    InternetAddressTest internetAddressTest;
    ASSERT_TRUE(internetAddressTest.TestDefaultConstructor());
}


TEST(InternetAddressGTest,TestFullConstructor) {
    InternetAddressTest internetAddressTest;
    ASSERT_TRUE(internetAddressTest.TestFullConstructor(80,"1.1.1.1","1.1.1.1"));
}

TEST(InternetAddressGTest,TestFullConstructor_Wrong_Format_1) {
    InternetAddressTest internetAddressTest;
    ASSERT_TRUE(internetAddressTest.TestFullConstructor(80,"255.1","255.0.0.1"));
}

TEST(InternetAddressGTest,TestFullConstructor_Wrong_Format_2) {
    InternetAddressTest internetAddressTest;
    ASSERT_TRUE(internetAddressTest.TestFullConstructor(80,"1.1.256.1","0.0.0.0"));
}

TEST(InternetAddressGTest,TesFullConstructor_Wrong_Format_3) {
    InternetAddressTest internetAddressTest;
    ASSERT_TRUE(internetAddressTest.TestFullConstructor(80,"1.256.1","0.0.0.0"));
}

TEST(InternetAddressGTest,TestGetPort) {
    InternetAddressTest internetAddressTest;
    ASSERT_TRUE(internetAddressTest.TestGetPort(80));
}

TEST(InternetAddressGTest,TestGetPort_0) {
    InternetAddressTest internetAddressTest;
    ASSERT_TRUE(internetAddressTest.TestGetPort(0));
}


TEST(InternetAddressGTest,TestGetPort_MAX) {
    InternetAddressTest internetAddressTest;
    ASSERT_TRUE(internetAddressTest.TestGetPort(0xffff));
}


TEST(InternetAddressGTest,TestGetDotName) {
    InternetAddressTest internetAddressTest;
    const char8 *table[][2]={
            {"1.1.1.1","1.1.1.1"},
            {"255.1","255.0.0.1"},
            {"1.1.256.1","0.0.0.0"},
            {"1.256.1","0.0.0.0"},
            {"1","0.0.0.1"},
            {"1.1.1.1.1","0.0.0.0"},
            {0,0}
    };

    ASSERT_TRUE(internetAddressTest.TestGetDotName(table));
}


TEST(InternetAddressGTest,TestGetHostName) {
    InternetAddressTest internetAddressTest;
    const char8 *table[][2]={
            {"127.0.0.1","localhost"},
            {"8.8.8.8","google-public-dns-a.google.com"},
            {0,0}
    };

    ASSERT_TRUE(internetAddressTest.TestGetHostName(table));
}


TEST(InternetAddressGTest,TestGetHostNumber) {
    InternetAddressTest internetAddressTest;
    const InternetAddressTestTable table[]={
            {"255.0.0.1",0x10000ff,0},
            {"8.8",0x8000008,0},
            {0,0}
    };

    ASSERT_TRUE(internetAddressTest.TestGetHostNumber(table));
}


TEST(InternetAddressGTest,TestGetLocalAddress) {

    InternetAddressTest internetAddressTest;

    ASSERT_TRUE(internetAddressTest.TestGetLocalAddress());
}


TEST(InternetAddressGTest,TestGetLocalIpNumber) {

    InternetAddressTest internetAddressTest;

    ASSERT_TRUE(internetAddressTest.TestGetLocalIpNumber());
}

TEST(InternetAddressGTest,TestGetLocalAddressAsNumber) {

    InternetAddressTest internetAddressTest;

    ASSERT_TRUE(internetAddressTest.TestGetLocalAddressAsNumber());
}

TEST(InternetAddressGTest,TestSetPort) {
    InternetAddressTest internetAddressTest;
    ASSERT_TRUE(internetAddressTest.TestSetPort(80));
}

TEST(InternetAddressGTest,TestSetPort_0) {
    InternetAddressTest internetAddressTest;
    ASSERT_TRUE(internetAddressTest.TestSetPort(0));
}


TEST(InternetAddressGTest,TestSetPort_MAX) {
    InternetAddressTest internetAddressTest;
    ASSERT_TRUE(internetAddressTest.TestSetPort(0xffff));
}


TEST(InternetAddressGTest,TestSetAddressByDotName) {
    InternetAddressTest internetAddressTest;
    const char8 *table[][2]={
            {"1.1.1.1","1.1.1.1"},
            {"255.1","255.0.0.1"},
            {"1.1.256.1","0.0.0.0"},
            {"1.256.1","0.0.0.0"},
            {"1","0.0.0.1"},
            {"1.1.1.1.1","0.0.0.0"},
            {0,0}
    };

    ASSERT_TRUE(internetAddressTest.TestSetAddressByDotName(table));
}


TEST(InternetAddressGTest,TestSetAddressByName) {
    InternetAddressTest internetAddressTest;
    const char8 *table[][2]={
            {"127.0.0.1","localhost"},
            {"8.8.8.8","google-public-dns-a.google.com"},
            {0,0}
    };

    ASSERT_TRUE(internetAddressTest.TestSetAddressByName(table));
}

TEST(InternetAddressGTest,TestSetAddressByNumber) {
    InternetAddressTest internetAddressTest;
    const InternetAddressTestTable table[]={
            {"255.0.0.1",0x10000ff,0},
            {"8.0.0.8",0x8000008,0},
            {0,0}
    };

    ASSERT_TRUE(internetAddressTest.TestSetAddressByNumber(table));
}

TEST(InternetAddressGTest,TestSetLocalAddress) {
    InternetAddressTest internetAddressTest;
    ASSERT_TRUE(internetAddressTest.TestSetLocalAddress());
}

TEST(InternetAddressGTest,TestGetAddress) {
    InternetAddressTest internetAddressTest;
    const InternetAddressTestTable table[]={
            {"255.0.0.1",0x10000ff,0xff},
            {"8.0.0.8",0x8000008,0xffff},
            {"8.127.0.8",0x8007f8,1234},
            {"8.3.10.8",0x80a0308,0},
            {0,0}
    };

    ASSERT_TRUE(internetAddressTest.TestGetAddress(table));
}

TEST(InternetAddressGTest,TestSize) {
    InternetAddressTest internetAddressTest;
    ASSERT_TRUE(internetAddressTest.TestSize());
}


