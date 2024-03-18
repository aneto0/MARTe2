/**
 * @file LinkedListHolder.h
 * @brief Header file for class LinkedListHolder
 * @date 06/06/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class LinkedListHolder
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LINKEDLISTHOLDER_H_
#define LINKEDLISTHOLDER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "LinkedListable.h"

/*---------------------------------------------------------------------------*/
/*                         Forward declarations                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
    class Iterator;
    class SearchFilter;
    class SortFilter;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief This class is a holder for a LinkedListable node, which acts as the
 * first node of an unbounded singly linked list.
 *
 * @details This list complements the LinkedListable class by adding the
 * concept of a first element of the list. In this way it is always possible
 * to link to a well defined item (e.g. to perform a search on the full list).
 */
class DLL_API LinkedListHolder {

public:

    /**
     * @brief Deallocates all the contents of the list.
     * @details This functions deletes all the elements of the list, sets its size to 0 and the root element to NULL.
     */
    void CleanUp();

    /**
     * @brief Resets the list without deleting the objects.
     * @details This functions sets the size of the list to 0 and the root element to NULL.
     * Note that the element of the list are not destroyed. This could lead to memory leaks.
     */
    void Reset();

    /**
     * @brief Constructor.
     */
    LinkedListHolder(const bool destroyIn=true);

    /**
     * @brief Destructor.
     */
    virtual ~LinkedListHolder();

    /**
     * @brief Returns the first element of the list.
     * @return a pointer to the first element of the list.
     */
    LinkedListable *List();

    /**
     * @brief Returns the number of elements in the list.
     * @return the number of the elements in the list.
     */
    uint32 ListSize() const;

    /**
     * @brief Inserts \p at the beginning of the list.
     * @param[in] p the LinkedListable object to insert.
     * @pre \a p must be a single LinkedListable element, as his next pointer will be set
     *      to the previous first element of the list after the execution of the function.
     *      If instead \a p is a list, this could lead to memory leaks.
     */
    inline void FastListInsertSingle(LinkedListable &p);

    /**
     * @brief Inserts at the end of the list \a p. \a p can be a single element or an entire list.
     * @param[in] p a pointer to the LinkedListable to insert.
     */
    void ListInsert(LinkedListable * const p);

    /**
     * @brief Inserts the input list using a SortFilter.
     * @details If sorter is NULL, the unsorted ListInsert(LinkedListable *) function is called,
     * inserting the input list at the beginning.
     * @param[in] p the pointer to the LinkedListable to insert.
     * @param[in] sorter implements the comparison criteria for the sorting.
     */
    void ListInsert(LinkedListable * const p,
                    SortFilter * const sorter);

    /**
     * @brief Inserts a list in the specified position of the list.
     * @details If the specified position is greater the the list size, the list input is inserted at the end.
     * @param[in] q the pointer to the LinkedListable to be inserted.
     * @param[in] index is the position in the list where \a p must be inserted.
     */
    void ListInsert(LinkedListable * const q,
                    const uint32 index);

    /**
     * @brief Adds an element at the end of this list.
     * @param[in] p the pointer to the LinkedListable element to be appended.
     * @pre \a p must be a single LinkedListable element, as his next pointer will be set
     *      to NULL after the execution of the function. If instead \a p is a list, Add
     *      could lead to memory leaks.
     */
    void ListAdd(LinkedListable * const p);

    /**
     * @brief Adds a LinkedListable list at the end of the list.
     * @param[in] p the pointer to the LinkedListable list to added.
     */
    void ListAddL(LinkedListable * const p);

    /**
     * @brief Searches a specified element in this list.
     * @param[in] p a pointer to the element to search.
     * @return true if \a p is in the list, false otherwise.
     */
    bool ListSearch(const LinkedListable * const p);

    /**
     * @brief Searches an element using a SearchFilter.
     * @param[in] filter defines the search criteria.
     * @return a pointer to the element if it is found in the list, NULL otherwise.
     */
    LinkedListable *ListSearch(SearchFilter * const filter);

    /**
     * @brief Removes the requested element from the list.
     * @param[in] p the element to be removed.
     * @return true if \a p was in the list, false otherwise.
     */
    bool ListExtract(LinkedListable * const p);

    /**
     * @brief Finds and removes one element from list using a SearchFilter.
     * @param[in] filter defines the search criteria.
     * @return the extracted element, NULL if it is not found in the list.
     */
    LinkedListable *ListExtract(SearchFilter * const filter);

    /**
     * @brief Deletes the requested element.
     * @param[in] p is the element which must be deleted.
     * @return true if p is in the list, false otherwise.
     */
    bool ListDelete(LinkedListable * const p);

    /**
     * @brief Deletes elements using a SearchFilter.
     * @param[in] filter defines the search criteria.
     * @return true if at least one element is deleted, false otherwise.
     */
    bool ListDelete(SearchFilter * const filter);

    /**
     * @brief Deletes a elements using a SearchFilter in safe mode.
     * @details Safe from being reentrant from the destructor of the object (complex graph destruction)
     * @param[in] filter defines the search criteria.
     * @return true if at least one object it is deleted, false otherwise.
     */
    bool ListSafeDelete(SearchFilter * const filter);

    /**
     * @brief Sorts the elements in the list using a SortFilter.
     * @param[in] sorter defines the comparison criteria.
     */
    void ListBSort(SortFilter * const sorter);

    /**
     * @brief Browses the list.
     * @param[in] index the position of the requested element (0 means the first element).
     * @return a pointer to the element at index position.
     */
    LinkedListable *ListPeek(const uint32 index);

    /**
     * @brief Removes and gets the element of the list in a specified position.
     * @param[in] index the position of the requested element.
     * @return a pointer to the element at index position.
     */
    LinkedListable *ListExtract(uint32 index = 0u);

    /**
     * @brief For each item in the list perform the action defined by the Iterator.
     * @param[in] it defines the action to be performed on each element.
     */
    void ListIterate(Iterator * const it);

private:

    /**
     * List root
     */
    LinkedListable llhRoot;

    /**
     * List size
     */
    uint32 llhSize;

    bool destroy;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

void LinkedListHolder::FastListInsertSingle(LinkedListable &p) {
    llhSize++;
    p.SetNext(llhRoot.Next());
    llhRoot.SetNext(&p);
}

}
#endif /* LINKEDLISTHOLDER_H_ */

