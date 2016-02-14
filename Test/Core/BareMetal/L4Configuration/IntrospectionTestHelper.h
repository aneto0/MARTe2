/**
 * @file IntrospectionTestHelper.h
 * @brief Header file for class IntrospectionTestHelper
 * @date 26/01/2016
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

 * @details This header file contains the declaration of the class IntrospectionTestHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTROSPECTIONTESTHELPER_H_
#define INTROSPECTIONTESTHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @brief Utility for tests which uses Introspection
 */
struct TestIntrospectionNestedStructureFrom {
    uint32 *nestedMember1_from;
    uint32 nestedMember2_from;
};

/**
 * @brief Utility for tests which uses Introspection
 */
struct TestIntrospectionNestedStructureTo {
    uint32 *nestedMember1_to;
    char8* nestedMember2_to;
};

/**
 * @brief Utility for tests which uses Introspection
 */
struct TestIntrospectionObjectFrom {
    uint32 member1_from;
    float32 *member2_from;
    float64 member3_from[32];
    char8 * member4_from[2][2];
    TestIntrospectionNestedStructureFrom member5_from;
};

/**
 * @brief Utility for tests which uses Introspection
 */
struct TestIntrospectionObjectTo {
    char8 member1_to[32];
    uint64 member2_to;
    float32 member3_to[32];
    uint32 member4_to[2][2];
    TestIntrospectionNestedStructureTo member5_to;
};

/**
 * @brief Utility for tests which uses Introspection
 */
struct TestIntrospectionObjectNoCompatibility {
    char8 member1_to[32];
    uint64 member2_to;
    float32 member3_to[32];
    uint32 member4_to[2][2];
    int32 x;
    TestIntrospectionNestedStructureTo member5_to;
};

/**
 * @brief Utility for tests which uses Introspection
 */
struct TestNoIntrospectionObject {
    uint32 member1_from;
    float32 *member2_from;
    float64 member3_from[32];
    const char8 * member4_from[2][2];
    TestIntrospectionNestedStructureFrom member5_from;
};





/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTROSPECTIONTESTHELPER_H_ */

