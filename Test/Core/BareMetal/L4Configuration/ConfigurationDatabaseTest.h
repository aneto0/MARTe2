/**
 * @file ConfigurationDatabaseTest.h
 * @brief Header file for class ConfigurationDatabaseTest
 * @date 19/11/2015
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class ConfigurationDatabaseTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONFIGURATIONDATABASETEST_H_
#define CONFIGURATIONDATABASETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests the ClassRegistryDatabase functions.
 */
class ConfigurationDatabaseTest {
public:
    /**
     * @brief Tests that the default constructor initialises the shared mutex Semaphore.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the CreateAbsolute function with a valid path
     */
    bool TestCreateAbsolute_ValidPath();

    /**
     * @brief Tests the CreateAbsolute function trying to create a node that already exists
     */
    bool TestCreateAbsolute_InvalidPath();

    /**
     * @brief Tests the CreateRelative function with a valid path
     */
    bool TestCreateRelative_ValidPath();

    /**
     * @brief Tests the CreateRelative function trying to create a node that already exists
     */
    bool TestCreateRelative_InvalidPath();

    /**
     * @brief Tests the MoveToRoot function.
     */
    bool TestMoveToRoot();

    /**
     * @brief Tests the MoveToRoot function.
     */
    bool TestMoveToAncestor_ValidNumberOfGenerations();

    /**
     * @brief Tests the MoveToRoot function.
     */
    bool TestMoveToAncestor_InvalidNumberOfGenerations();

    /**
     * @brief Tests the MoveAbsolute function with valid paths.
     */
    bool TestMoveAbsolute_ValidPaths();

    /**
     * @brief Tests the MoveAbsolute function with invalid paths.
     */
    bool TestMoveAbsolute_InvalidPaths();

    /**
     * @brief Tests the MoveRelative function with valid paths.
     */
    bool TestMoveRelative_ValidPaths();

    /**
     * @brief Tests the MoveRelative function with invalid paths.
     */
    bool TestMoveRelative_InvalidPaths();

    /**
     * @brief Tests the MoveRelative function.
     */
    bool TestDelete();

    /**
     * @brief Tests the Read function with a valid name
     */
    bool TestRead_Valid();

    /**
     * @brief Tests the Read function with an invalid name
     */
    bool TestRead_Invalid();

    /**
     * @brief Tests the AddToCurrentNode function.
     */
    bool TestAddToCurrentNode();

    /**
     * @brief Tests the Write function with a valid name
     */
    bool TestWrite_Valid();

    /**
     * @brief Tests the Write function with a name that already exists
     */
    bool TestWrite_Overwrite();

    /**
     * @brief Tests the Write function with an invalid name
     */
    bool TestWrite_Invalid();

    /**
     * @see StructuredDataI::GetType
     */
    //virtual AnyType GetType(const char8 * const name);
    /**
     * @see StructuredDataI::Copy
     */
    //virtual bool Copy(StructuredDataI &destination);
    /**
     * @brief Locks the shared semaphore.
     * @param[in] timeout maximum time to wait for the semaphore to be unlocked.
     * @return true if the shared semaphore is successfully locked.
     */
    //bool Lock(const TimeoutType &timeout);
    /**
     * @brief Unlocks the shared semaphore.
     * @return true if the shared semaphore is successfully unlocked.
     */
    //void Unlock();
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONFIGURATIONDATABASETEST_H_ */

