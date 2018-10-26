/**
 * @file Md5EncryptTest.h
 * @brief Header file for class Md5EncryptTest
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

 * @details This header file contains the declaration of the class Md5EncryptTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MD5ENCRYPTTEST_H_
#define MD5ENCRYPTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Md5Encrypt.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the Md5Encrypt methods
 */
class Md5EncryptTest {
public:

    /**
     * @brief Constructor
     */
    Md5EncryptTest();

    /**
     * @brief Destructor
     */
    virtual ~Md5EncryptTest();

    /**
     * @brief Tests the Md5 method
     */
    bool TestMd5(const char8 *input, StreamString &expected);

    /**
     * @brief Tests the Md5Hmac method.
     */
    bool TestMd5Hmac(const char8 *input, const char8 *key, StreamString &expected);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MD5ENCRYPTTEST_H_ */

