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
#include "Object.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief A ReferenceContainer like implementation optimised for the ConfigurationDatabase implementation.
 * @details With respect to the ReferenceContainer, the ConfigurationDatabaseNode offers an optimised Find method,
 * based on the BinaryTree and allows to directly navigate to the parent Container.
 *
 * A shared semaphore that can be used by the users of a database instance to have concurrent access to the database.
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
     * TODO
     */
    void Purge();

    /**
     * TODO
     */
    void Purge(ReferenceContainer &purgeList);

    /**
     * TODO
     */
    bool Insert(Reference ref, const int32 &position = -1);

    /**
     * TODO
     */
    uint32 Size();

    /**
     * TODO
     */
    Reference Get(uint32 idx);

    /**
     * TODO
     */
    Reference Find(const char8 * const path);

    /**
     * TODO
     */
    bool Delete(Reference ref);

    /**
     * TODO
     */
    ReferenceT<ConfigurationDatabaseNode> GetParent();

    /**
     * TODO
     */
    void SetParent(ReferenceT<ConfigurationDatabaseNode> parentIn);

private:

    /**
     * TODO
     */
    ReferenceContainer refContainer;

    /**
     * Binary tree containing indexes
     */
    BinaryTree<uint32, Fnv1aHashFunction> binTree;

    /**
     * The father node
     */
    ReferenceT<ConfigurationDatabaseNode> parent;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L4CONFIGURATION_CONFIGURATIONDATABASENODE_H_ */

