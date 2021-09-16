/**
 * @file ConfigurationDatabaseNode.h
 * @brief Header file for class ConfigurationDatabaseNode
 * @date 20/12/2019
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

 * @details This header file contains the declaration of the class ConfigurationDatabaseNode
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_BAREMETAL_L4CONFIGURATION_CONFIGURATIONDATABASENODE_H_
#define SOURCE_CORE_BAREMETAL_L4CONFIGURATION_CONFIGURATIONDATABASENODE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BinaryTree.h"
#include "Fnv1aHashFunction.h"
#include "Object.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief A ReferenceContainer like node implementation optimised for the ConfigurationDatabase.
 * @details With respect to the ReferenceContainer, the ConfigurationDatabaseNode offers an optimised Find method,
 * based on the BinaryTree and allows to directly navigate to the parent Container.
 *
 */
class DLL_API ConfigurationDatabaseNode: public Object {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
ConfigurationDatabaseNode    ();

    /**
     * @brief Destructor. Cleans the binary tree.
     */
    virtual ~ConfigurationDatabaseNode();

    /**
     * @brief Purges the internal containers and breaks the link to the parent ConfigurationDatabaseNode.
     */
    void Purge();

    /**
     * @brief Inserts a reference to this node.
     * @param[in] ref the reference to be added.
     * @return true if the reference is successfully added.
     */
    bool Insert(Reference ref);

    /**
     * @brief Gets the number of references held by this node.
     * @return the number of references held by this node.
     */
    uint32 Size();

    /**
     * @brief Gets the reference in position \a idx.
     * @param[in] idx the index to be retrieved.
     * @return the reference in position \a idx.
     */
    Reference Get(const uint32 idx);

    /**
     * @brief Recursively finds a Reference against a given \a path.
     * @param[in] path the path leading to the Reference. The path is dot separated (A.B.C).
     * @return the Reference at the provided path, or an invalid Reference if the path is not valid (or if the node does not exist).
     */
    Reference Find(const char8 * const path);

    /**
     * @brief Gets the Reference against a given \a name (which must be a leaf of this node).
     * @param[in] name the name of the leaf.
     * @return the Reference with the requested name, or an invalid Reference if the leaf does not exist.
     */
    Reference FindLeaf(const char8 * const name);

    /**
     * @brief Deletes the reference from this node.
     * @param[in] ref the Reference to be deleted.
     * @return true if the Reference was successfully deleted.
     */
    bool Delete(Reference ref);

    /**
     * @brief Gets the parent node.
     * @return the parent node.
     */
    ReferenceT<ConfigurationDatabaseNode> GetParent() const;

    /**
     * @brief Sets the parent node.
     * @param[in] parentIn the parent node to be set.
     */
    void SetParent(ReferenceT<ConfigurationDatabaseNode> parentIn);

    /**
     * @brief Gets the number of nodes that are of type ConfigurationDatabaseNode.
     * @return the numbe of nodes that are of type ConfigurationDatabaseNode.
     */
    uint32 GetNumberOfNodes();

private:

    /**
     * @brief Locks the internal spin-lock mutex.
     * @return true if the lock succeeds.
     */
    bool Lock();

    /**
     * @brief Unlocks the internal spin-lock mutex.
     */
    void UnLock();

    /**
     * The container holding all the nodes directly underneath this node.
     */
    Reference *container;

    /**
     * Number of elements to grow the array when it gets out of space.
     */
    uint32 granularity;

    /**
     * Current actual size of the array.
     */
    uint32 containerSize;

    /**
     * Number of ConfigurationDatabaseNode nodes.
     */
    uint32 numberOfNodes;

    /**
     * Maximum size of the array without requiring a realloc.
     */
    uint32 maxSize;

    /**
     * Protects multiple access to the internal resources
     */
    FastPollingMutexSem mux;

    /**
     * Timeout
     */
    TimeoutType muxTimeout;


    /**
     * Binary tree containing the indexes of the elements in the container.
     */
    BinaryTree<uint32, Fnv1aHashFunction> binTree;

    /**
     * The parent node
     */
    ReferenceT<ConfigurationDatabaseNode> parent;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L4CONFIGURATION_CONFIGURATIONDATABASENODE_H_ */

