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
     * @return true if the attribute numberOfReference is initialized to zero.
     */
    bool TestConstructor();

    /**
     * @brief Tests the destructor.
     * @return true because this function does nothing.
     */
    bool TestDestructor();

    /**
     * @brief Tests the Object::GetIntrospectionCopy.
     * //TODO
     */
    bool TestGetIntrospectionCopy();

    /**
     * @brief Tests the Object::Initialise function.
     * @return the opposite of the tested function (always it returns false).
     */
    bool TestInitialise();

    /**
     * @brief Tests the Object::NumberOfReferences function.
     * @details Creates an object and assigns different references to it checking if the function returns the correct
     * number of references to the object.
     * returns the correct number of references.
     * @return true if successful, false otherwise.
     */
    bool TestNumberOfReferences();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OBJECTTEST_H_ */

