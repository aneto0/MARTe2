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
 * @brief Tests the ConfigurationDatabase functions.
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
    template<typename T>
    bool TestRead_Valid(T value);

    /**
     * @brief Tests the Read passing a StructuredDataI.
     */
    bool TestRead_StructuredData();

    /**
     * @brief Tests the Read passing a structured object.
     */
    bool TestRead_Object();

    /**
     * @brief Tests the Read function with an invalid name.
     */
    bool TestRead_Invalid();

    /**
     * @brief Tests the AddToCurrentNode function.
     */
    bool TestAddToCurrentNode();

    /**
     * @brief Tests the AddToCurrentNode function with an invalid reference.
     */
    bool TestAddToCurrentNode_InvalidReference();

    /**
     * @brief Tests the Write function with a valid name
     */
    template<typename T>
    bool TestWrite_Valid(T valie);

    /**
     * @brief Tests the Write function with a name that already exists
     */
    bool TestWrite_Overwrite();

    /**
     * @brief Tests the Write function with an invalid name
     */
    bool TestWrite_Invalid();

    /**
     * @brief Tests the Write on a StructuredDataI
     */
    bool TestWrite_StructuredData();

    /**
     * @brief Tests the Write on a structured object
     */
    bool TestWrite_Object();

    /**
     * @brief Tests the GetType function with a valid name
     */
    template<typename T>
    bool TestGetType_Valid(T value);

    /**
     * @brief Tests the GetType function with an invalid name
     */
    bool TestGetType_Invalid();

    /**
     * @brief Tests the GetChildName function.
     */
    bool TestGetChildName();

    /**
     * @brief Tests the GetName function.
     */
    bool TestGetName();

    /**
     * @brief Tests the TestGetNumberOfChildren function.
     */
    bool TestGetNumberOfChildren();

    /**
     * @brief Tests the Copy function
     */
    bool TestCopy();

    /**
     * @brief Tests the Lock function
     */
    bool TestLock();

    /**
     * @brief Tests the Unlock function.
     */
    bool TestUnlock();

    /**
     * @brief Tests the Initialise function
     */
    bool TestInitialise();

    /**
     * @brief Tests the CleanUp function
     */
    bool TestCleanUp();

    /**
     * @brief Tests the GetCurrentNode function
     */
    bool TestGetCurrentNode();

    /**
     * @brief Tests the CopyConstructor
     */
    bool TestCopyConstructor();

    /**
     * @brief Tests the Operator =
     */
    bool TestCopyOperatorEqual();

    /**
     * @brief Tests the MoveToChild method
     */
    bool TestMoveToChild();

    /**
     * @brief Tests the SetCurrentNodeAsRootNode method
     */
    bool TestSetCurrentNodeAsRootNode();

    /**
     * @brief Tests the Purge method
     */
    bool TestPurge();

    /**
     * @brief Tests the copy equal operator forcing a condition that led to a bug in release 1.4.0
     */
    bool TestCopyOperatorEqualBug();
};
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template<typename T>
bool ConfigurationDatabaseTest::TestWrite_Valid(T value) {
    MARTe::ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("A.B.C");
    ok &= cdb.Write("value", value);
    T readValue;
    ok &= cdb.Read("value", readValue);
    ok &= (value == readValue);
    return ok;
}

template<typename T>
bool ConfigurationDatabaseTest::TestRead_Valid(T value) {
    MARTe::ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("A.B.C");
    ok &= cdb.Write("value", value);
    T readValue;
    ok &= cdb.Read("value", readValue);
    ok &= (value == readValue);
    return ok;
}

template<typename T>
bool ConfigurationDatabaseTest::TestGetType_Valid(T value) {
    MARTe::ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("A.B.C");
    ok &= cdb.Write("value", value);
    MARTe::AnyType inValue(value);
    MARTe::AnyType readValue = cdb.GetType("value");
    ok &= (inValue.GetTypeDescriptor() == readValue.GetTypeDescriptor());
    return ok;
}
#endif /* CONFIGURATIONDATABASETEST_H_ */

