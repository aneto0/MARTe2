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
     * @return true if the test explained in the details section has success, false otherwise.
     */
    bool TestNumberOfReferences();


    /**
     * @brief Tests the Object::SetName and Object::GetName functions.
     * @details name is the desired object name.
     * @return true if the get function returns the name set using the set function.
     */
    bool TestSetGetName(const char8* name);

    /**
     * @brief Tests if the string set is copied in the memory.
     * @return true if the string assigned to the object is copied in the memory and it is not only a pointer assignment.
     */
    bool TestDuplicateName();


    /**
     * @brief Tests the Object::GetUniqueName function.
     * @details Checks that the initial part of the returned string is the object pointer. It is possible
     * set the buffer size to pass to the GetUniqueName function and check if the string returned until that size.
     * @param[in] name is the desired object name.
     * @param[in] size is the size of the buffer to pass to the GetUniqueName function.
     * @return true if the tests explained in the details section have success, false otherwise.
     */
    bool TestGetUniqueName(const char8* name,uint32 buffSize);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* OBJECTTEST_H_ */

