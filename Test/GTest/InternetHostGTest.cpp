/**
 * @file InternetHostGTest.cpp
 * @brief Source file for class InternetHostGTest
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
 * the class InternetHostGTest (public, protected, and private). Be aware that some 
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
#include "InternetHostTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



TEST(InternetHostGTest,TesDefaultConstructor) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestDefaultConstructor());
}


TEST(InternetHostGTest,TestFullConstructor) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestFullConstructor(80,"1.1.1.1","1.1.1.1"));
}

TEST(InternetHostGTest,TestFullConstructor_Wrong_Format_1) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestFullConstructor(80,"255.1","255.0.0.1"));
}

TEST(InternetHostGTest,TestFullConstructor_Wrong_Format_2) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestFullConstructor(80,"1.1.256.1","0.0.0.0"));
}

TEST(InternetHostGTest,TesFullConstructor_Wrong_Format_3) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestFullConstructor(80,"1.256.1","0.0.0.0"));
}

TEST(InternetHostGTest,TestGetPort) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetPort(80));
}

TEST(InternetHostGTest,TestGetPort_0) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetPort(0));
}


TEST(InternetHostGTest,TestGetPort_MAX) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetPort(0xffff));
}


TEST(InternetHostGTest,TestGetAddress) {
    InternetHostTest internetHostTest;
    const char8 *table[][2]={
            {"1.1.1.1","1.1.1.1"},
            {"255.1","255.0.0.1"},
            {"1.1.256.1","0.0.0.0"},
            {"1.256.1","0.0.0.0"},
            {"1","0.0.0.1"},
            {"1.1.1.1.1","0.0.0.0"},
            {0,0}
    };

    ASSERT_TRUE(internetHostTest.TestGetAddress(table));
}


TEST(InternetHostGTest,TestGetHostName) {
    InternetHostTest internetHostTest;
    const char8 *table[][2]={
            {"127.0.0.1","localhost"},
            {"8.8.8.8","google-public-dns-a.google.com"},
            {0,0}
    };

    ASSERT_TRUE(internetHostTest.TestGetHostName(table));
}


TEST(InternetHostGTest,TestGetAddressAsNumber) {
    InternetHostTest internetHostTest;
    const InternetHostTestTable table[]={
            {"255.0.0.1",0x10000ff,0},
            {"8.8",0x8000008,0},
            {0,0}
    };

    ASSERT_TRUE(internetHostTest.TestGetAddressAsNumber(table));
}


TEST(InternetHostGTest,TestGetLocalAddress) {

    InternetHostTest internetHostTest;

    ASSERT_TRUE(internetHostTest.TestGetLocalAddress());
}


TEST(InternetHostGTest,TestGetLocalIpNumber) {

    InternetHostTest internetHostTest;

    ASSERT_TRUE(internetHostTest.TestGetLocalAddress());
}

TEST(InternetHostGTest,TestGetLocalAddressAsNumber) {

    InternetHostTest internetHostTest;

    ASSERT_TRUE(internetHostTest.TestGetLocalAddressAsNumber());
}

TEST(InternetHostGTest,TestSetPort) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestSetPort(80));
}

TEST(InternetHostGTest,TestSetPort_0) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestSetPort(0));
}


TEST(InternetHostGTest,TestSetPort_MAX) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestSetPort(0xffff));
}


TEST(InternetHostGTest,TestSetAddress) {
    InternetHostTest internetHostTest;
    const char8 *table[][2]={
            {"1.1.1.1","1.1.1.1"},
            {"255.1","255.0.0.1"},
            {"1.1.256.1","0.0.0.0"},
            {"1.256.1","0.0.0.0"},
            {"1","0.0.0.1"},
            {"1.1.1.1.1","0.0.0.0"},
            {0,0}
    };

    ASSERT_TRUE(internetHostTest.TestSetAddress(table));
}


TEST(InternetHostGTest,TestSetAddressByHostName) {
    InternetHostTest internetHostTest;
    const char8 *table[][2]={
            {"127.0.0.1","localhost"},
            {"8.8.8.8","google-public-dns-a.google.com"},
            {0,0}
    };

    ASSERT_TRUE(internetHostTest.TestSetAddressByHostName(table));
}

TEST(InternetHostGTest,TestSetAddressByNumber) {
    InternetHostTest internetHostTest;
    const InternetHostTestTable table[]={
            {"255.0.0.1",0x10000ff,0},
            {"8.0.0.8",0x8000008,0},
            {0,0}
    };

    ASSERT_TRUE(internetHostTest.TestSetAddressByNumber(table));
}

TEST(InternetHostGTest,TestSetLocalAddress) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestSetLocalAddress());
}

TEST(InternetHostGTest,TestGetInternetHost) {
    InternetHostTest internetHostTest;
    const InternetHostTestTable table[]={
            {"255.0.0.1",0x10000ff,0xff},
            {"8.0.0.8",0x8000008,0xffff},
            {"8.127.0.8",0x8007f8,1234},
            {"8.3.10.8",0x80a0308,0},
            {0,0}
    };

    ASSERT_TRUE(internetHostTest.TestGetInternetHost(table));
}

TEST(InternetHostGTest,TestSize) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestSize());
}

TEST(InternetHostGTest,TestGetLocalHostName) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetLocalHostName());
}

