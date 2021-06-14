/**
 * @file Md5EncryptGTest.cpp
 * @brief Source file for class Md5EncryptGTest
 * @date 08/10/2018
 * @author Giuseppe Ferro
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
 * the class Md5EncryptGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include <Md5EncryptTest.h>
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
TEST(BareMetal_L1Portability_Md5EncryptGTest,TestMd5) {
    Md5EncryptTest test;
    StreamString expected = "8B1A9953C4611296A827ABF8C47804D7";
    ASSERT_TRUE(test.TestMd5("Hello", expected));
}

TEST(BareMetal_L1Portability_Md5EncryptGTest,TestMd5Hmac) {
    Md5EncryptTest test;
    StreamString expected = "2FE05E3122FD81A3D0B2E32FC2C4D2BB";
    ASSERT_TRUE(test.TestMd5Hmac("Hello", "1234", expected));
}
