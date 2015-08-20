/**
 * @file ObjectTest.h
 * @brief Header file for class ObjectTest
 * @date 07/08/2015
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

 * @details This header file contains the declaration of the class ObjectTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef OBJECTTEST_H_
#define OBJECTTEST_H_

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

/**
 * Tests the Object public methods.
 */
class ObjectTest {
public:

    /**
     * @brief Tests the constructor.
     * @return true if the attribute numberOfReferences is initialised to zero.
     */
    bool TestConstructor();

    /**
     * @brief Tests the destructor.
     * @return true if the attribute numberOfReferences is zero after calling the destructor.
     */
    bool TestDestructor();

    /**
     * @brief Tests the Object::GetIntrospectionCopy.
     * //TODO
     */
    bool TestGetIntrospectionCopy();

    /**
     * @brief Tests the Object::Initialise function which should not be implemented in the base Object
     * and should return false.
     * @return true if Object::Initialise returns false.
     */
    bool TestInitialise();

    /**
     * @brief Tests the Object::NumberOfReferences function.
     * @details Creates an object and and assigns different references to it, checking that Object::NumberOfReferences
     * returns a value consistent with the number of references holding the object.
     * @return true if Object::NumberOfReferences always returns a value consistent with the number of references holding the object.
     */
    bool TestNumberOfReferences();

    /**
     * @brief Tests the Object::TestGetProperties function.
     * @return true if the ClassProperties object returned is the equal to the object created at class registration time.
     */
    bool TestGetProperties();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OBJECTTEST_H_ */

