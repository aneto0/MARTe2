/**
 * @file IntrospectionTest.h
 * @brief Header file for class IntrospectionTest
 * @date 18/01/2016
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

 * @details This header file contains the declaration of the class IntrospectionTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTROSPECTIONTEST_H_
#define INTROSPECTIONTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "IntrospectionT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
using namespace MARTe;


/**
 * @brief Tests all the IntrospectionEntry functions.
 */
class IntrospectionTest {

public:
    /**
     * @brief Tests if the constructor initializes correctly the class member.
     */
    bool TestConstructor();

    /**
     * @brief Checks if the [] operator returns the IntrospectionEntry in the specified index.
     */
    bool TestPositionOperator();

    /**
     * @brief Checks if the ClassRegistryDatabase saves correctly the introspection of a basic type member after the registration
     * of the class using defined macros.
     */
    bool TestMacroToAddBasicInClassRegistryDatabase();

    /**
     * @brief Checks if the ClassRegistryDatabase saves correctly the introspection of a structured type member after the registration
     * of the class using defined macros.
     */
    bool TestMacroToAddStructuredInClassRegistryDatabase();

    /**
     * @brief Checks if the ClassRegistryDatabase saves correctly the introspection of a object-derived class after the registration
     * of the class using defined macros.
     */
    bool TestMacroToAddObjectClassRegistryDatabase();


    /**
     * @brief Tests if the function returns correctly the number of members.
     */
    bool TestGetNumberOfMembers();

    /**
     * @brief Tests if the function returns correctly the class size.
     */
    bool TestGetClassSize();

    /**
     * @brief Tests the SetIntrospectionEntryMemory.
     */
    bool TestSetIntrospectionEntryMemory();

    /**
     * @brief Tests the GetIntrospectionEntryMemory.
     */
    bool TestGetIntrospectionEntryMemory();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTROSPECTIONTEST_H_ */

