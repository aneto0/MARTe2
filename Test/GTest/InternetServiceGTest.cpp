/**
 * @file InternetServiceGTest.cpp
 * @brief Source file for class InternetServiceGTest
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
 * the class InternetServiceGTest (public, protected, and private). Be aware that some 
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
#include "InternetServiceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



TEST(InternetServiceGTest,TesDefaultConstructor) {
    InternetServiceTest internetServiceTest;
    ASSERT_TRUE(internetServiceTest.TestDefaultConstructor());
}



TEST(InternetServiceGTest,TestSearchByName) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {NULL, NULL, 0x1500, "tcp", false},
            {"ftp", NULL, 0x1500, "tcp", true},
            {"http", NULL, 0x5000, "tcp", true},
            {"http", "tcp", 0x5000, "tcp", true},
            {"http", "udp", 0x5000, "udp", true},
            {"smtp", "tcp", 0x1900, "tcp", true},
            {"smtp", "udp", 0x1900, "udp", false},
            {"git", "tcp", 0xca24, "tcp", true},
            {0,0,0,0,0}
    };

    ASSERT_TRUE(internetServiceTest.TestSearchByName(table));
}



TEST(InternetServiceGTest,TestSearchByPort) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {"ftp", NULL, 0x1500, "tcp", true},
            {"http", NULL, 0x5000, "tcp", true},
            {"http", "tcp", 0x5000, "tcp", true},
            {"http", "udp", 0x5000, "udp", true},
            {"smtp", "tcp", 0x1900, "tcp", true},
            {"smtp", "udp", 0x1900, "udp", false},
            {"git", "tcp", 0xca24, "tcp", true},
            {0,0,0,0,0}
    };

    ASSERT_TRUE(internetServiceTest.TestSearchByPort(table));
}


TEST(InternetServiceGTest,TestPort) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {"ftp", NULL, 0x1500, "tcp", true},
            {"http", NULL, 0x5000, "tcp", true},
            {"http", "tcp", 0x5000, "tcp", true},
            {"http", "udp", 0x5000, "udp", true},
            {"smtp", "tcp", 0x1900, "tcp", true},
            {"smtp", "udp", 0x1900, "udp", false},
            {"git", "tcp", 0xca24, "tcp", true},
            {0,0,0,0,0}
    };

    ASSERT_TRUE(internetServiceTest.TestPort(table));
}


TEST(InternetServiceGTest,TestName) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {"ftp", NULL, 0x1500, "tcp", true},
            {"http", NULL, 0x5000, "tcp", true},
            {"http", "tcp", 0x5000, "tcp", true},
            {"http", "udp", 0x5000, "udp", true},
            {"smtp", "tcp", 0x1900, "tcp", true},
            {"smtp", "udp", 0x1900, "udp", false},
            {"git", "tcp", 0xca24, "tcp", true},
            {0,0,0,0,0}
    };

    ASSERT_TRUE(internetServiceTest.TestName(table));
}



TEST(InternetServiceGTest,TestProtocol) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {"ftp", NULL, 0x1500, "tcp", true},
            {"http", NULL, 0x5000, "tcp", true},
            {"http", "tcp", 0x5000, "tcp", true},
            {"http", "udp", 0x5000, "udp", true},
            {"smtp", "tcp", 0x1900, "tcp", true},
            {"smtp", "udp", 0x1900, "udp", false},
            {"git", "tcp", 0xca24, "tcp", true},
            {0,0,0,0,0}
    };

    ASSERT_TRUE(internetServiceTest.TestProtocol(table));
}


TEST(InternetServiceGTest,TestGetPortByName) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {"ftp", NULL, 0x1500, "tcp", true},
            {"http", NULL, 0x5000, "tcp", true},
            {"http", "tcp", 0x5000, "tcp", true},
            {"http", "udp", 0x5000, "udp", true},
            {"smtp", "tcp", 0x1900, "tcp", true},
            {"git", "tcp", 0xca24, "tcp", true},
            {0,0,0,0,0}
    };

    ASSERT_TRUE(internetServiceTest.TestGetPortByName(table));
}



