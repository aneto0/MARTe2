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
     * @see StructuredDataI::Read
     */
    //virtual bool Read(const char8 * const name, const AnyType &value);

    /**
     * @see StructuredDataI::GetType
     */
    //virtual AnyType GetType(const char8 * const name);

    /**
     * @see StructuredDataI::Write
     */
    //virtual bool Write(const char8 * const name, const AnyType &value);

    /**
     * @see StructuredDataI::Copy
     */
    //virtual bool Copy(StructuredDataI &destination);

    /**
     * @see StructuredDataI::MoveToRoot
     */
    //virtual bool MoveToRoot();

    /**
     * @see StructuredDataI::MoveToAncestor
     */
    //virtual bool MoveToAncestor(const uint32 generations);

    /**
     * @see StructuredDataI::MoveAbsolute
     */
    //virtual bool MoveAbsolute(const char8 * const path);

    /**
     * @see StructuredDataI::MoveRelative
     */
    //virtual bool MoveRelative(const char8 * const path);

    /**
     * @see StructuredDataI::CreateAbsolute
     */
    //virtual bool CreateAbsolute(const char8 * const path);

    /**
     * @see StructuredDataI::CreateRelative
     */
    //virtual bool CreateRelative(const char8 * const path);

    /**
     * @see StructuredDataI::AddToCurrentNode
     */
    //virtual bool AddToCurrentNode(Reference node);

    /**
     * @see StructuredDataI::Delete
     */
    //virtual bool Delete();


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

