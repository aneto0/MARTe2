/**
 * @file FormatDescriptorTest.h
 * @brief Header file for class FormatDescriptorTest
 * @date 1/09/2015
 * @author Llorenç Capellà
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

 * @details This header file contains the declaration of the class FormatDescriptorTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_L0TYPEDEV_FORMATDESCRIPTORTEST_H_
#define TEST_CORE_L0TYPEDEV_FORMATDESCRIPTORTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "../../../Source/Core/L0TypeDev/FormatDescriptor.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace TypeDefinition;

class FormatDescriptorTest {
public:
    FormatDescriptorTest();
    virtual ~FormatDescriptorTest();

    bool retVal;

    const char8* s;

    FormatDescriptor fd;

    /**
     * @brief Test InitialiseFromString().
     * @return True if the function InitialiseFromString() returns true && the variables as set as expected
     */
    bool TestInitialiseFromString();

    //bool TestDefaultConstructor();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TEST_CORE_L0TYPEDEV_FORMATDESCRIPTORTEST_H_ */

