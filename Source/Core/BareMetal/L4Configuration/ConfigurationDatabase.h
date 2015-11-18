/**
 * @file ConfigurationDatabase.h
 * @brief Header file for class ConfigurationDatabase
 * @date 27/10/2015
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class ConfigurationDatabase
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONFIGURATIONDATABASE_H_
#define CONFIGURATIONDATABASE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "AnyObject.h"
#include "TypeConversion.h"
#include "StructuredDataI.h"
#include "ReferenceT.h"
#include "ReferenceContainer.h"
#include "ReferenceContainerFilterObjectName.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief An implementation of StructuredDataI where node paths are identified and separated by dots.
 * @details Paths are constructed by concatenating node names with dots (e.g. A.B.C.D), where D is a leaf
 * and A, B and C are nodes.
 *
 * A shared semaphore that can be used by the users of a database instance to have concurrent access to the database.
 */
class ConfigurationDatabase: public StructuredDataI {
public:

    /**
     * @brief Default constructor. Initialises the shared mutex Semaphore.
     */
    ConfigurationDatabase();

    /**
     * Default constructor.
     */
    virtual ~ConfigurationDatabase();

    /**
     * @see StructuredDataI::Read
     */
    virtual bool Read(const char * const name,
                      const AnyType &value);

    /**
     * @see StructuredDataI::GetType
     */
    virtual AnyType GetType(const char * const name);

    /**
     * @see StructuredDataI::Write
     */
    virtual bool Write(const char * const name,
                       const AnyType &value);

    /**
     * @see StructuredDataI::Copy
     */
    virtual bool Copy(StructuredDataI &destination);

    /**
     * @see StructuredDataI::MoveToRoot
     */
    virtual bool MoveToRoot();

    /**
     * @see StructuredDataI::MoveToAncestor
     */
    virtual bool MoveToAncestor(uint32 generations);

    /**
     * @see StructuredDataI::MoveAbsolute
     */
    virtual bool MoveAbsolute(const char * const path);

    /**
     * @see StructuredDataI::MoveRelative
     */
    virtual bool MoveRelative(const char * const path);

    /**
     * @see StructuredDataI::CreateNodesAbsolute
     */
    virtual bool CreateNodesAbsolute(const char * const path);

    /**
     * @see StructuredDataI::CreateNodesRelative
     */
    virtual bool CreateNodesRelative(const char * const path);

    /**
     * @see StructuredDataI::AddToCurrentNode
     */
    virtual bool AddToCurrentNode(Reference node);

    /**
     * @brief Locks the shared semaphore.
     * @param[in] timeout maximum time to wait for the semaphore to be unlocked.
     * @return true if the shared semaphore is successfully locked.
     */
    bool Lock(const TimeoutType &timeout);

    /**
     * @brief Unlocks the shared semaphore.
     * @return true if the shared semaphore is successfully unlocked.
     */
    void Unlock();

private:
    /**
     * @brief Create nodes relative to the currentNode.
     * @param path the path to be created.
     * @return true if the path creation is successful.
     */
    bool CreateNodes(const char * const path);

    /**
     * The current node to where the database is pointing.
     */
    ReferenceT<ReferenceContainer> currentNode;

    /**
     * The root node of the database.
     */
    ReferenceT<ReferenceContainer> rootNode;

    /**
     * The shared mutex semaphore.
     */
    FastPollingMutexSem mux;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONFIGURATIONDATABASE_H_ */

