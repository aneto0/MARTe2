/**
 * @file TestObjectHelper3.h
 * @brief Header file for class TestObjectHelper3
 * @date 22/feb/2016
 * @author pc
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

 * @details This header file contains the declaration of the class TestObjectHelper3
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TESTOBJECTHELPER3_H_
#define TESTOBJECTHELPER3_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class NestedObjectHelper3: public Object {
public:

    CLASS_REGISTER_DECLARATION()

    NestedObjectHelper3();

    int32 x;
};

class TestObjectHelper3: public Object {

public:
    CLASS_REGISTER_DECLARATION()

    TestObjectHelper3();

    ~TestObjectHelper3();

    uint32 a;
    const char8* b;
    float64 c[2][2];
    NestedObjectHelper3** d;
    NestedObjectHelper3 e;
};







/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TESTOBJECTHELPER3_H_ */

