/**
 * @file ReferenceContainerNode.h
 * @brief Header file for class ReferenceContainerNode
 * @date 12/08/2015
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

 * @details This header file contains the declaration of the class ReferenceContainerNode
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REFERENCECONTAINERNODE_H_
#define REFERENCECONTAINERNODE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LinkedListable.h"
#include "Reference.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Wraps a Reference around a LinkedListable so that it can be added to a container.
 */
class ReferenceContainerNode: public LinkedListable {
public:
    /**
     * @brief Constructor. NOOP.
     */
    ReferenceContainerNode();
    /**
     * @brief Destructor. NOOP.
     */
    ~ReferenceContainerNode();

    /**
     * @brief Returns the Reference hold by this node.
     * @return the Reference hold by this node.
     */
    Reference GetReference() const;

    /**
     * @brief Sets the to Reference hold by this node.
     * @param[in] newReference the reference to be set.
     * @return true if \a newReference is valid.
     */
    bool SetReference(Reference newReference);

private:
    /**
     * Reference hold by node.
     */
    Reference reference;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REFERENCECONTAINERNODE_H_ */

