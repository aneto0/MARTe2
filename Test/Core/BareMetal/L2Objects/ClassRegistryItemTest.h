/**
 * @file ClassRegistryItemTest.h
 * @brief Header file for class ClassRegistryItemTest
 * @date 11/08/2015
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

 * @details This header file contains the declaration of the class ClassRegistryItemTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSREGISTRYITEMTEST_H_
#define CLASSREGISTRYITEMTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ClassRegistryItem.h"
#include "Object.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests all the ClassRegistryItem functions.
 */
class ClassRegistryItemTest {

public:

    /**
     * @brief Tests the default constructor.
     * @return true if all the class attributes are initialised correctly.
     */
    bool TestConstructor();

    /**
     * @brief Tests the constructor with the Introspection input.
     * @return true if all the class attributes are initialised correctly.
     */
    bool TestIntrospectionCostructor();

    /**
     * @brief Tests the full constructor.
     * @return true if all the class attributes are initialised correctly.
     */
    bool TestFullCostructor();

    /**
     * @brief Tests destructor.
     * @return true if the class is deleted from the database.
     */
    bool TestDestructor();

    /**
     * @brief Tests ClassRegistryItem::IncrementNumberOfInstances.
     * @return true if after the function call the number of instances variable is incremented.
     */
    bool TestIncrementNumberOfInstances();

    /**
     * @brief Tests ClassRegistryItem::DecrementNumberOfInstances.
     * @return true if after the function call the number of instances variable is decremented.
     */
    bool TestDecrementNumberOfInstances();

    /**
     * @brief Tests ClassRegistryItem::GetNumberOfInstances.
     * @details Increments and decrements the number of instances by the specified number, then checks that the
     * number of the instances returned is correct.
     * @param[in] nInstances specified how many increments of the number of instance to do.
     * @return true if the number of instances is incremented and decremented \a nInstances times.
     */
    bool TestGetNumberOfInstances(uint32 nInstances);

    /**
     * @brief Tests the ClassRegistryItem::GetClassPropertiesCopy.
     * @param[in] name is the desired class name.
     * @param[in] typeidName is the desired class name.
     * @param[in] version is the desired class version.
     * @return true if the ClassProperties copy has the same attributes values of the properties passed by constructor, false otherwise.
     */
    bool TestGetClassPropertiesCopy();

    /**
     * @brief Tests the ClassRegistryItem::GetClassProperties.
     * @param[in] name is the desired class name.
     * @param[in] typeidName is the desired class name.
     * @param[in] version is the desired class version.
     * @return true if the ClassProperties returned has the same attributes values of the properties passed by constructor, false otherwise.
     */
    bool TestGetClassProperties();

    /**
     * @brief Tests the ClassRegistryItem::SetLoadableLibrary and ClassRegistryItem::GetLoadableLibrary.
     * @param[in] llname is the desired library name.
     * @return true if the library returned by the get function is equal to the library previously set. False otherwise.
     */
    bool TestSetGetLoadableLibrary(const char8 *llname);

    /**
     * @brief Tests the ClassRegistryItem::GetObjectBuildFcn.
     * @details Set a build function in the ClassProperties passed by parameter to the constructor and checks that the
     * returned value is precisely the pointer to this function. Then calls the function and checks that it does what expected.
     * @return true if the test explained in details section has success, false otherwise.
     */
    bool TestGetObjectBuildFunction();


    /**
     * @brief Checks if the function returns the pointer to the Introspection set by constructor.
     */
    bool TestGetIntrospection();

    /**
     * @brief Tests the ClassRegistryItem::SetUniqueIdentifier function.
     * @param uniqueIdentifier the unique identifier value to test.
     * @return true if after calling ClassRegistryItem::SetUniqueIdentifier(\a uniqueIdentifier),
     * ClassRegistryItem->GetClassProperties()->GetUniqueIdentifier() == \a uniqueIdentifier;
     */
    bool TestSetUniqueIdentifier(uint32 uniqueIdentifier);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSREGISTRYITEMTEST_H_ */

