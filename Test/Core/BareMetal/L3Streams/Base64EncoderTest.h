/**
 * @file Base64EncoderTest.h
 * @brief Header file for class Base64EncoderTest
 * @date 26/09/2018
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

 * @details This header file contains the declaration of the class Base64EncoderTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASE64ENCODERTEST_H_
#define BASE64ENCODERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Base64Encoder.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests the Base64Encoder methods
 */
class Base64EncoderTest {
public:

    /**
     * @brief Constructor
     */
    Base64EncoderTest();

    /**
     * @brief Destructor
     */
    virtual ~Base64EncoderTest();

    /**
     * @brief Tests the Encode method
     */
    bool TestEncode(const char8 *toEncodeIn, const char8* expected);

    /**
     * @brief Tests the Decode method.
     */
    bool TestDecode(const char8 *toDecodeIn, const char8* expected);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASE64ENCODERTEST_H_ */

