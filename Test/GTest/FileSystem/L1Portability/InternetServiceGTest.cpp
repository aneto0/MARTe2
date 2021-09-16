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


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "InternetServiceTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



TEST(FileSystem_L1Portability_InternetServiceGTest,TesDefaultConstructor) {
    InternetServiceTest internetServiceTest;
    ASSERT_TRUE(internetServiceTest.TestDefaultConstructor());
}



TEST(FileSystem_L1Portability_InternetServiceGTest,TestSearchByName) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {NULL, NULL, 0, "tcp", false},
            {"ftp", NULL, 21, "tcp", true},
            {"http", NULL, 80, "tcp", true},
            {"http", "tcp", 80, "tcp", true},
            {"ntp", "udp", 123, "udp", true},//change because port 80 is tcp{"http", "udp", 80, "udp", true},
            {"smtp", "tcp", 25, "tcp", true},
            {0,0,0,0,0}
    };

    ASSERT_TRUE(internetServiceTest.TestSearchByName(table));
}



TEST(FileSystem_L1Portability_InternetServiceGTest,TestSearchByPort) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {"ftp", NULL, 21, "tcp", true},
            {"http", NULL, 80, "tcp", true},
            {"http", "tcp", 80, "tcp", true},
            {"ntp", "udp", 123, "udp", true}, //change because port 80 is tcp{"http", "udp", 80, "udp", true},
            {"smtp", "tcp", 25, "tcp", true},
            {0,0,0,0,0}
    };

    ASSERT_TRUE(internetServiceTest.TestSearchByPort(table));
}


TEST(FileSystem_L1Portability_InternetServiceGTest,TestPort) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {"ftp", NULL, 21, "tcp", true},
            {"http", NULL, 80, "tcp", true},
            {"http", "tcp", 80, "tcp", true},
            {"ntp", "udp", 123, "udp", true},//change because port 80 is tcp{"http", "udp", 80, "udp", true},
            {"smtp", "tcp", 25, "tcp", true},
            {0,0,0,0,0}
    };

    ASSERT_TRUE(internetServiceTest.TestPort(table));
}


TEST(FileSystem_L1Portability_InternetServiceGTest,TestName) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {"ftp", NULL, 21, "tcp", true},
            {"http", NULL, 80, "tcp", true},
            {"http", "tcp", 80, "tcp", true},
            {"ntp", "udp", 123, "udp", true},//change because port 80 is tcp{"http", "udp", 80, "udp", true},
            {"smtp", "tcp", 25, "tcp", true},
            {0,0,0,0,0}
    };

    ASSERT_TRUE(internetServiceTest.TestName(table));
}



TEST(FileSystem_L1Portability_InternetServiceGTest,TestProtocol) {
    InternetServiceTest internetServiceTest;
    const InternetServiceTestTable table[] ={
            {"ftp", NULL, 21, "tcp", true},
            {"http", NULL, 80, "tcp", true},
            {"http", "tcp", 80, "tcp", true},
            {"ntp", "udp", 123, "udp", true},//change because port 80 is tcp{"http", "udp", 80, "udp", true},
            {"smtp", "tcp", 25, "tcp", true},
            {0,0,0,0,0}
    };

    ASSERT_TRUE(internetServiceTest.TestProtocol(table));
}





