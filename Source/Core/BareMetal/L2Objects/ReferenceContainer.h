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
 *
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

#include "LinkedListHolder.h"
#include "FastPollingMutexSem.h"
#include "TimeoutType.h"
#include "Object.h"
#include "Reference.h"
#include "LinkedListable.h"
#include "ReferenceContainerFilter.h"
#include "ReferenceContainerNode.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Container of references.
 * @details One of the basilar classes of the framework. Linear container of references which may also
 * include other containers of references (generating a tree). The access to the container is protected
 * by an internal FastPollingMutexSem whose timeout can be specified.
 */
/*lint -e{9109} forward declaration in ReferenceContainerFilter.h is required to define the class*/
/*lint -e{763} forward declaration in ReferenceContainerFilter.h is required to define the class*/
class DLL_API ReferenceContainer: public Object {
public:

    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Default constructor.
     * @details Initialises the semaphore and set the default timeout to infinite.
     * @post
     *   GetTimeout() == TTInfiniteWait
     */
ReferenceContainer    ();

    /**
     * @brief Copy Constructor
     * @details Inserts all the references contained by \a copy in this.
     * @post
     *   GetTimeout() == copy.GetTimeout() &&
     *   GetSize() == copy.GetSize()
     */
    /*lint -e{1724} Parameter of copy constructor is not modified, but can not be declared const.*/
    ReferenceContainer(ReferenceContainer &copy);

    /**
     * @brief Inserts all the references contained by \a copy in this.
     * @post
     *   GetTimeout() == copy.GetTimeout() &&
     *   GetSize() == copy.GetSize()
     */
    ReferenceContainer& operator=(ReferenceContainer &copy);

    /**
     * @brief Destructor. Deletes all the elements hold by the container.
     */
    virtual ~ReferenceContainer();

    /**
     * @brief Full Destruction of the database.
     * @details This function destroys each element of the database also in case of reference loops (the father contains a reference to the children and the
     * children contains a reference to the father) by extracting all elements from the tree and destroying them separately.
     * @warning This function does not perform the behavior of the ReferenceContainer destructor because if another ReferenceContainer shares some
     * ReferenceT<ReferenceContainer> with this, this function will destroy also its sub-trees related with that shared elements. Vice versa, the
     * destructor extracts references from the tree only if they are not referenced by someone else.
     */
    void CleanUp();

    /**
     * @brief Inserts a new reference to the container.
     * @details If \a position = -1 the reference is added to the end of the container.
     * @param[in] ref the reference to be inserted.
     * @param[in] position the position in the container where the reference is to be inserted.
     * @return true if \a ref is valid and if it can be successfully added to the container.
     */
    bool Insert(Reference ref, const int32 &position = -1);

    /**
     * @brief Inserts a new reference in the specified path.
     * @details Creates all the nodes in the \a path if needed before adding \a ref as a leaf.
     * @param[in] path is the path where \a ref must be inserted to.
     * @param[in] red is the Reference to be inserted in the container.
     * @return false if \a ref is not valid or in case of errors, true otherwise.
     */
    bool Insert(const char8 * const path, Reference ref);

    /**
     * @brief Removes a reference from the container.
     * @details This call is not recursive, i.e. if the container contains other containers, the \a ref
     * will not be recursively searched (this can be achieved with the Find method and ReferenceContainerFilterReferences filter).
     * @param[in] ref the reference to be deleted.
     * @return true if the reference can be successfully removed (i.e. if the Size() of the list is decreased by 1).
     */
    bool Delete(Reference ref);

    /**
     * @brief Delete the reference denoted by the \a path in input.
     * @param[in] path is the path of the reference to be deleted from the database.
     * @return false if the reference is not found in the specified path, true if it will be successfully deleted.
     */
    bool Delete(const char8 * const path);

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
     * @brief Finds the first element identified by \a path in RECURSIVE mode.
     * @param[in] path is the name of the element to be found or its full path.
     * @return the element if it is found or an invalid reference if not.
     */
    Reference Find(const char8 * const path, const bool recursive=false);

    /**
     * @brief Retrieves the Reference at the given address.
     * @param[in] path is the address of the Reference into the database. The syntax is
     * "A.B.C" where A, B and C must be replaced with the specific object names.
     * param[in] current is the research start point. In this case we admit the syntax "::A.B.C"
     * where the ':' symbol set the search start point to the previous domain with respect to \a current.
     * If no ':' is found at the beginning of the path, the start point is the root.
     * @return the reference found at the provided \a path or an invalid reference in case of failure.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2].*/
    Reference Find(const char8 * const path, const Reference current, bool relative=false);

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

    /**
     * @brief Explores the StructuredDataI in input and builds Objects storing
     * their References.
     * @details The Object will be built only if the node name in the data tree
     * has the special symbol '+' or '$' at the beginning. The symbol '$' marks the
     * node as a domain and will be used for relative researches by path in ObjectRegistryDatabase::Find(*)
     * @param[in] data is the StructuredData in input.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @see Object::ExportData(*)
     */
    virtual bool ExportData(StructuredDataI & data);

    /**
     * @brief Locks the internal spin-lock mutex.
     * @return true if the lock succeeds.
     */
    bool Lock();

    /**
     * @Unlocks the internal spin-lock mutex.
     */
    void UnLock();

private:

    /**
     * The list of references
     */
    LinkedListHolderT<ReferenceContainerNode> list;

    /**
     * Protects multiple access to the internal resources
     */
    FastPollingMutexSem mux;

    /**
     * Timeout
     */
    TimeoutType muxTimeout;

    /**
     * List used to destroy the database.
     */
    LinkedListHolderT<ReferenceContainerNode> purgeList;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REFERENCECONTAINER_H_ */

