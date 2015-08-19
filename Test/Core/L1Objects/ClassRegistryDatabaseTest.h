/**
 * @file ClassRegistryDatabaseTest.h
 * @brief Header file for class ClassRegistryDatabaseTest
 * @date Aug 6, 2015
 * @author aneto
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

 * @details This header file contains the declaration of the class ClassRegistryDatabaseTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSREGISTRYDATABASETEST_H_
#define CLASSREGISTRYDATABASETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ClassRegistryDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the ClassRegistryDatabase functions.
 */
class ClassRegistryDatabaseTest {
public:
    /**
     * @brief Tests the Instance function.
     * @return Since the database is a singleton, this test returns true if the instance function
     * returns always the same database object.
     */
    bool TestInstance();

    /**
     * @brief Tests the Delete function.
     * @details Create objects and checks if they are deleted from the database after the delete call.
     * @return true if ClassRegistryDatabase::Find fails after the delete and the size is decremented.
     */
    bool TestDeleteTrue();

    /**
     * @brief Tests the Delete function passing an invalid element.
     * @return true if the delete fails as expected.
     */
    bool TestDeleteFalse();

    /**
     * @brief Tests the ClassRegistryDatabase::Add function.
     * @return true if after adding a ClassRegistryItem element to the database, it can be found and the database size is incremented.
     */
    bool TestAdd();

    /**
     * @brief Tests the Add function with two classes with the same name.
     * @return true if the last class to be added is saved in the database, false otherwise.
     */
    bool TestAddTheSameName();

    /**
     * @brief Tests the Find function.
     * @param[in] name is the name of the class to find in the database.
     * @param[in] create specifies if the class should be registered or not in the database (determining the result of the find function).
     * @return true if the find on registered class names succeeds and fails if the class is not registered.
     */
    bool TestFind(const char8 *name,
                  bool create);

    /**
     * @brief Tests the find function with the DLLName::ClassName pattern.
     * @param[in] dllName is the desired dll name.
     * @param[in] class name is the desired class name.
     * @param[in] validName specified if the pattern in input dllName::className is valid (the test should return true) or not
     * (the test should return false)
     * @return true if \a validName=true DLLName::ClassName returns true and if \a validName=false and DLLName::ClassName returns false.
     */
    bool TestFindDLL(const char8* dllName,
                     const char8* className,
                     bool validName);

    /**
     * @brief Tests the List function.
     * @return true if is returned the first element registered in the database, false otherwise.
     */
    bool TestList();

    /**
     * @brief Tests the size function.
     * @details Registers a number of classes in the database and checks that the size increases. Then delete these elements and checks that
     * the size decreases.
     * @return true if the test explained in the details section succeeds, false otherwise.
     */
    bool TestSize();

    /**
     * @brief Tests the ElementAt function.
     * @details Tests if passing index = 0 the ElementAt function it returns the first element (same result of List function). Then adds an element
     * and checks that with an index=size-1 the ElementAt function returns the same element (the last in the database). At last checks if passing an index >=size
     * the function returns a NULL pointer.
     * @return true if the tests explained in the details section succeeds, false otherwise.
     */
    bool TestElementAt();

    /**
     * @brief Tests if instantiating objects, the number of instances parameter is correct.
     * @details A certain number of objects of a particular class are created using Reference class and this test checks that
     * the number of instances for the class is correct. Then creates an instance of a collector class which contains other registered classes
     * as attributes and checks that only the instance counter of the collector is incremented.
     * @return true if the tests explained in the details section succeeds, false otherwise.
     */
    bool TestCreateInstances();

    /**
     * @brief Tests the number of instances returned when an object is created by a reference of its father class.
     * @details Using ReferenceT, with the template of the father class, creates the child class. Then checks that only the number
     * of instances of the child class is incremented and that the number of reference is valid.
     * @return true if the test explained in the details section succeeds, false otherwise.
     */
    bool TestPolimorphismChild2Father();

    /**
     * @brief Tests the number of instances returned when an object is created by a reference of its child class.
     * @details Using ReferenceT, with the template of the father class, creates the child class. Since the dynamic_cast fails in
     * this case, the object should not be created and the reference should be invalid.
     * @return true if the test explained in the details section succeeds, false otherwise.
     */
    bool TestPolimorphismFather2Child();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSREGISTRYDATABASETEST_H_ */

