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
#include "ConfigurationDatabaseNode.h"
#include "ReferenceT.h"
#include "ReferenceContainer.h"
#include "ReferenceContainerFilterObjectName.h"
#include "StructuredDataI.h"
#include "TypeConversion.h"

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
class DLL_API ConfigurationDatabase: public StructuredDataI, public Object {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * Initialises the shared mutex Semaphore.
     * @post
     *   MoveToRoot() == true
     */
ConfigurationDatabase    ();

    /**
     * @brief Copy constructor
     * @details Create a copy of the ConfigurationDatabase which copies a reference to the current and root node, so that
     *  the copied ConfigurationDatabase can move independently on the database.
     * @param[in] toCopy the ConfigurationDatabase where to copy from
     */
    ConfigurationDatabase(const ConfigurationDatabase &toCopy);

    /**
     * @brief Operator = (does not perform a deep copy, see Copy)
     * @details Create a copy of the ConfigurationDatabase which copies a reference to the current and root node, so that
     *  the copied ConfigurationDatabase can move independently on the database.
     * @param[in] toCopy the ConfigurationDatabase where to copy from
     */
    ConfigurationDatabase & operator = (const ConfigurationDatabase &toCopy);

    /**
     * @brief Destructor.
     */
    virtual ~ConfigurationDatabase();

    /**
     * @brief Removes all the elements from the database.
     */
    void Purge() const;

    /**
     * @see StructuredDataI::Read
     * @details It is possible read directly a structure or a class if
     * the node with the \a name provided has a compatible structure.
     * In this case the class must be introspectable and registered into
     * the ConfigurationDatabase.
     */
    virtual bool Read(const char8 * const name,
            const AnyType &value);

    /**
     * @see StructuredDataI::GetType
     */
    virtual AnyType GetType(const char8 * const name);

    /**
     * @see StructuredDataI::Write
     * @details It is possible write directly a structure or a class
     * if it is introspectable and registered into
     * the ConfigurationDatabase.
     */
    virtual bool Write(const char8 * const name,
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
    virtual bool MoveToAncestor(const uint32 generations);

    /**
     * @see StructuredDataI::MoveAbsolute
     */
    virtual bool MoveAbsolute(const char8 * const path);

    /**
     * @see StructuredDataI::MoveRelative
     */
    virtual bool MoveRelative(const char8 * const path);

    /**
     * @see StructuredDataI::MoveToChild
     */
    virtual bool MoveToChild(const uint32 childIdx);

    /**
     * @see StructuredDataI::CreateAbsolute
     */
    virtual bool CreateAbsolute(const char8 * const path);

    /**
     * @see StructuredDataI::CreateRelative
     */
    virtual bool CreateRelative(const char8 * const path);

    /**
     * @see StructuredDataI::AddToCurrentNode
     */
    virtual bool AddToCurrentNode(Reference node);

    /**
     * @see StructuredDataI::Delete
     */
    virtual bool Delete(const char8 * const name);

    /**
     * @brief StructuredDataI::GetName
     */
    virtual const char8 *GetName();

    /**
     * @brief StructuredDataI::GetChildName
     */
    virtual const char8 *GetChildName(const uint32 index);

    /**
     * @brief StructuredDataI::GetNumberOfChildren
     */
    virtual uint32 GetNumberOfChildren();

    /**
     * @brief Initialises the contents of this ConfigurationDatabase from a StructuredDataI
     * @details Calls data.Copy(*this)
     * @return true if data.Copy(*this) is successful.
     */
    virtual bool Initialise(StructuredDataI &data);

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

    /**
     * @brief Gets a reference to the current node as a ReferenceContainer.
     * @return a reference to the current node as a ReferenceContainer.
     */
    //Reference GetCurrentNode() const;

    /**
     * @brief Sets the current node as the root node.
     */
    void SetCurrentNodeAsRootNode();

private:

    /**
     * @brief Create nodes relative to the currentNode.
     * @param[in] path the path to be created.
     * @return true if the path creation is successful.
     */
    bool CreateNodes(const char8 * const path);

    /**
     * The current node to where the database is pointing.
     */
    ReferenceT<ConfigurationDatabaseNode> currentNode;

    /**
     * The root node of the database.
     */
    ReferenceT<ConfigurationDatabaseNode> rootNode;

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

