/**
 * @file ReferenceContainer.h
 * @brief Header file for class ReferenceContainer
 * @date 13/08/2015
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

 * @details This header file contains the declaration of the class ReferenceContainer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REFERENCECONTAINER_H_
#define REFERENCECONTAINER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "FastPollingMutexSem.h"
#include "TimeoutType.h"
#include "LinkedListHolder.h"
#include "Object.h"
#include "Reference.h"
#include "LinkedListable.h"
#include "ReferenceContainerFilter.h"
#include "ReferenceContainerNode.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Container of references.
 * @details One of the basilar classes of the framework. Linear container of references which may also
 * include other containers of references (generating a tree). The access to the container is protected
 * by an internal FastPollingMutexSem whose timeout can be specified.
 */
/*lint -e{9109} forward declaration in ReferenceContainerFilter.h is required to define the class*/
class ReferenceContainer: public Object {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details Initialises the semaphore and set the default timeout to infinite.
     */
    ReferenceContainer();

    /**
     * @brief Destructor. Deletes all the elements hold by the container.
     */
    virtual ~ReferenceContainer();

    /**
     * @brief Inserts a new reference to the container.
     * @details If \a position = -1 the reference is added to the end of the container.
     * @param[in] ref the reference to be inserted.
     * @param[in] position the position in the container where the reference is to be inserted.
     * @return true if \a ref is valid and it can be successfully added to the container.
     */
    bool Insert(Reference ref,
                const int32 &position = -1);

    /**
     * @brief Removes the references from the container.
     * @details This call is not recursive, i.e. if the container contains other containers, the \a ref
     * will not be recursively searched (this can be achieved with the Find method and ReferenceContainerFilterReferences filter).
     * @param[in] ref the reference to be deleted.
     * @return true if the reference can be successfully removed (i.e. if the Size() of the list is decreased by 1).
     */
    bool Delete(Reference ref);

    /**
     * @brief Finds on or more elements in the container.
     * @details The container is walked and its elements are tested against a \a filter. Valid results are
     * stored in the \a result container.
     * @param[in,out] result container where to store the elements found (which may include a path to a given container).
     * @param[in,out] filter the searching criteria to be applied.
     */
    void Find(ReferenceContainer &result,
              ReferenceContainerFilter &filter);

    /**
     * @brief Checks if \a ref holds a container.
     * @param[in] ref the reference to check.
     * @return true if \a ref holds ReferenceContainer.
     */
    bool IsContainer(const Reference &ref) const;

    /**
     * @brief Returns the number of elements in the container.
     * @return the number of elements in the container.
     */
    uint32 Size();

    /**
     * @brief Returns the reference at position \a idx.
     * @param[in] idx the desired reference position.
     * @return the Reference at position \a idx or an empty Reference if \a idx < 0 or \a idx >  Size().
     */
    Reference Get(const uint32 idx);

    /**
     * @brief Returns the semaphore timeout time.
     * @return the semaphore timeout time.
     */
    TimeoutType GetTimeout() const;

    /**
     * @brief Updates the semaphore timeout time.
     * @param[in] timeout the timeout to be set.
     */
    void SetTimeout(const TimeoutType &timeout);

private:
    /**
     * The list of references
     */
    LinkedListHolder list;

    /**
     * Protects multiple access to the internal resources
     */
    FastPollingMutexSem mux;

    /**
     * Timeout
     */
    TimeoutType muxTimeout;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REFERENCECONTAINER_H_ */

