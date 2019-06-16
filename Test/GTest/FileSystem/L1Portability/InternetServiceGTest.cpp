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

#include "InternetServiceTest.h"
#include "TestSupport.h"

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
            {emptyString, emptyString, 0, "tcp", false},
            {"ftp", emptyString, 21, "tcp", true},
            {"http", emptyString, 80, "tcp", true},
            {"http", "tcp", 80, "tcp", true},
            {"ntp", "udp", 123, "udp", true},//change because port 80 is tcp{"http", "udp", 80, "udp", true},
            {"smtp", "tcp", 25, "tcp", true},
            {emptyString,emptyString,0,emptyString,false}
    };

    ASSERT_TRUE(internetServiceTest.TestSearchByName(table));
}



TEST(InternetServiceGTest,TestSearchByPort) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {"ftp", emptyString, 21, "tcp", true},
            {"http", emptyString, 80, "tcp", true},
            {"http", "tcp", 80, "tcp", true},
            {"ntp", "udp", 123, "udp", true}, //change because port 80 is tcp{"http", "udp", 80, "udp", true},
            {"smtp", "tcp", 25, "tcp", true},
            {emptyString,emptyString,0,emptyString,false}
    };

    ASSERT_TRUE(internetServiceTest.TestSearchByPort(table));
}


TEST(InternetServiceGTest,TestPort) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {"ftp", emptyString, 21, "tcp", true},
            {"http", emptyString, 80, "tcp", true},
            {"http", "tcp", 80, "tcp", true},
            {"ntp", "udp", 123, "udp", true},//change because port 80 is tcp{"http", "udp", 80, "udp", true},
            {"smtp", "tcp", 25, "tcp", true},
            {emptyString,emptyString,0,emptyString,false}
    };

    ASSERT_TRUE(internetServiceTest.TestPort(table));
}


TEST(InternetServiceGTest,TestName) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {"ftp", emptyString, 21, "tcp", true},
            {"http", emptyString, 80, "tcp", true},
            {"http", "tcp", 80, "tcp", true},
            {"ntp", "udp", 123, "udp", true},//change because port 80 is tcp{"http", "udp", 80, "udp", true},
            {"smtp", "tcp", 25, "tcp", true},
            {emptyString,emptyString,0,emptyString,false}
    };

    ASSERT_TRUE(internetServiceTest.TestName(table));
}



TEST(InternetServiceGTest,TestProtocol) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {"ftp", emptyString, 21, "tcp", true},
            {"http", emptyString, 80, "tcp", true},
            {"http", "tcp", 80, "tcp", true},
            {"ntp", "udp", 123, "udp", true},//change because port 80 is tcp{"http", "udp", 80, "udp", true},
            {"smtp", "tcp", 25, "tcp", true},
            {emptyString,emptyString,0,emptyString,false}
    };

    ASSERT_TRUE(internetServiceTest.TestProtocol(table));
}





