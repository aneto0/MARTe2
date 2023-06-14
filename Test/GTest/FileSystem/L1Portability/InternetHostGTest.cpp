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


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "InternetHostTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



TEST(FileSystem_L1Portability_InternetHostGTest,TesDefaultConstructor) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestDefaultConstructor());
}


TEST(FileSystem_L1Portability_InternetHostGTest,TestFullConstructor) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestFullConstructor(80,"1.1.1.1","1.1.1.1"));
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestFullConstructor_Wrong_Format_1) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestFullConstructor(80,"255.1","255.0.0.1"));
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestFullConstructor_Wrong_Format_2) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestFullConstructor(80,"1.1.256.1","0.0.0.0"));
}

TEST(FileSystem_L1Portability_InternetHostGTest,TesFullConstructor_Wrong_Format_3) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestFullConstructor(80,"1.256.1","0.0.0.0"));
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestGetPort) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetPort(80));
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestGetPort_0) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetPort(0));
}


TEST(FileSystem_L1Portability_InternetHostGTest,TestGetPort_MAX) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetPort(0xffff));
}


TEST(FileSystem_L1Portability_InternetHostGTest,TestGetAddress) {
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


TEST(FileSystem_L1Portability_InternetHostGTest,TestGetHostName) {
    InternetHostTest internetHostTest;
    const char8 *table[][2]={
            {"8.8.8.8","dns.google"},
            {0,0}
    };

    ASSERT_TRUE(internetHostTest.TestGetHostName(table));
}


TEST(FileSystem_L1Portability_InternetHostGTest,TestGetAddressAsNumber) {
    InternetHostTest internetHostTest;
    const InternetHostTestTable table[]={
            {"255.0.0.1",0x10000ff,0},
            {"8.8",0x8000008,0},
            {0,0}
    };

    ASSERT_TRUE(internetHostTest.TestGetAddressAsNumber(table));
}


TEST(FileSystem_L1Portability_InternetHostGTest,TestGetLocalAddress) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetLocalAddress());
}


TEST(FileSystem_L1Portability_InternetHostGTest,TestGetLocalIpNumber) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetLocalAddress());
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestGetLocalAddressAsNumber) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetLocalAddressAsNumber());
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestGetMACAddress) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetMACAddress());
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestConvertInterfaceNameToInterfaceAddressNumber) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestConvertInterfaceNameToInterfaceAddressNumber());
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestConvertInterfaceNameToInterfaceAddress) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestConvertInterfaceNameToInterfaceAddress());
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestSetPort) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestSetPort(80));
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestSetPort_0) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestSetPort(0));
}


TEST(FileSystem_L1Portability_InternetHostGTest,TestSetPort_MAX) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestSetPort(0xffff));
}


TEST(FileSystem_L1Portability_InternetHostGTest,TestSetAddress) {
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


TEST(FileSystem_L1Portability_InternetHostGTest,TestSetAddressByHostName) {
    InternetHostTest internetHostTest;
    const char8 *table[][2]={
            {"127.0.0.1","localhost"},
            {"8.8.8.8","google-public-dns-a.google.com"},
            {0,0}
    };

    ASSERT_TRUE(internetHostTest.TestSetAddressByHostName(table));
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestSetAddressByNumber) {
    InternetHostTest internetHostTest;
    const InternetHostTestTable table[]={
            {"255.0.0.1",0x10000ff,0},
            {"8.0.0.8",0x8000008,0},
            {0,0}
    };

    ASSERT_TRUE(internetHostTest.TestSetAddressByNumber(table));
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestSetLocalAddress) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestSetLocalAddress());
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestSetMulticastGroup) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestSetMulticastGroup());
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestGetMulticastGroup) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetMulticastGroup());
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestSetMulticastInterfaceAddress) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestSetMulticastInterfaceAddress());
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestSetMulticastInterfaceAddressWithNumber) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestSetMulticastInterfaceAddressWithNumber());
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestGetMulticastInterfaceAddress) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetMulticastInterfaceAddress());
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestGetInternetHost) {
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

TEST(FileSystem_L1Portability_InternetHostGTest,TestGetInternetMulticastHost) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetInternetMulticastHost());
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestSize) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestSize());
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestMulticastSize) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestMulticastSize());
}

TEST(FileSystem_L1Portability_InternetHostGTest,TestGetLocalHostName) {
    InternetHostTest internetHostTest;
    ASSERT_TRUE(internetHostTest.TestGetLocalHostName());
}


//TEST(FileSystem_L1Portability_InternetServiceGTest,Prova) {
//    InternetHostTest internetServiceTest;
//    ASSERT_TRUE(internetServiceTest.Prova());
//}
