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
#include "LinkedListable.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Implementation of a dynamic single linked list.
 *
 * Adding the memory of the root and using functions defined for LinkedListable objects,
 * this class performs all the basic methods to manipulate a list.
 *
 * To implement specific object lists this class must be derived.
 */

class LinkedListHolder {

public:

    /**
     * @brief Deallocates all the contents.
     */
    void CleanUp();

    /**
     * @brief Sets 0 to llhSize and NULL to llhRoot->next.
     */
    void Reset();

    /**
     * @brief Constructor.
     */
    LinkedListHolder();

    /**
     * @brief Destructor.
     */
    virtual ~LinkedListHolder();

    /**
     * @brief Returns the first element.
     * @return a pointer to the first element of the list.
     */
    LinkedListable *List();

    /**
     * @brief Returns the number of elements in the list.
     * @return the number of the elements in the list.
     */
    uint32 ListSize() const;

    /**
     * @brief Inserts in the first location the element in input.
     * @param[in] p is the LinkedListable object to insert.
     */
    inline void FastListInsertSingle(LinkedListable &p);

    /**
     * @brief Inserts in the first location the input list.
     * @param[in] p is a pointer to the LinkedListable to insert.
     */
    void ListInsert(LinkedListable * const p);

    /**
     * @brief Inserts the input list using a SortFilter.
     * @details if the SortFilter pointer is null, the function performs the normal behavior inserting at the
     * input list at the beginning.
     * @param[in] p is the pointer to the LinkedListable to insert.
     * @param[in] sorter defines the comparison criteria.
     */
    void ListInsert(LinkedListable * const p,
                    SortFilter * const sorter);

    /**
     * @brief Inserts the list p using a SortFilterFn function.
     * @details if the SortFilterFn pointer is null, the function performs the normal behavior inserting at the
     * input list at the beginning.
     * @param[in] p is the pointer to the LinkedListable to insert.
     * @param[in] sorter defines the comparison criteria.
     */
    void ListInsert(LinkedListable * const p,
                    SortFilterFn * const sorter);

    /**
     * @brief Inserts a list in the specified position of the list.
     * @details if the specified position is greater the the list size, the list input is inserted on the queue.
     * @param[in] q is the pointer to the LinkedListable to insert.
     * @param[in] index is the position in the list where p must be inserted.
     */
    void ListInsert(LinkedListable * const q,
                    const uint32 index);

    /**
     * @brief Adds an element at the end of this list.
     * @param[in] p is a pointer to the LinkedListable element to be appended.
     */
    void ListAdd(LinkedListable * const p);

    /**
     * @brief Adds a LinkedListable list at the end of the list.
     * @param[in] p is the pointer to the LinkedListable list to added.
     */
    void ListAddL(LinkedListable * const p);

    /**
     * @brief Searches a specified element in this list.
     * @param[in] p is a pointer to the element to search.
     * @return true if p is in the list, false otherwise.
     */
    bool ListSearch(const LinkedListable * const p);

    /**
     * @brief Searches an element using a SearchFilter.
     * @param[in] filter defines the search criteria.
     * @return a pointer to the element if it is found in the list, NULL otherwise.
     */
    LinkedListable *ListSearch(SearchFilter * const filter);

    /**
     * @brief Searches an element using a SearchFn function.
     * @param[in] filter defines the search criteria.
     * @return a pointer to the element if it is found in the list, NULL otherwise.
     */
    LinkedListable *ListSearch(SearchFilterFn * const filter);

    /**
     * @brief Removes the requested element from the list.
     * @param[in] p is the element to be removed.
     * @return true if p was in the list, false otherwise.
     */
    bool ListExtract(LinkedListable * const p);

    /**
     * @brief Finds and removes one element from list using a SearchFilter.
     * @param[in] filter defines the search criteria.
     * @return the extracted element, NULL if it is not found in the list.
     */
    LinkedListable *ListExtract(SearchFilter * const filter);

    /**
     * @brief Finds and removes one element from list using a SearchFn function.
     * @param[in] filter defines the search criteria.
     * @return the extracted element, NULL if it is not found in the list.
     */
    LinkedListable *ListExtract(SearchFilterFn * const filter);

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
     * @details Safe from reentrance from destructor of object (complex graph destruction)
     * @param[in] filter defines the search criteria.
     * @return true if at least one object it is deleted, false otherwise.
     */
    bool ListSafeDelete(SearchFilter * const filter);

    /**
     * @brief Deletes elements using a SearchFn function.
     * @param[in] filter defines the search criteria.
     * @return true if at least one element it is deleted, false otherwise.
     */
    bool ListDelete(SearchFilterFn * const filter);

    /**
     * @brief Sorts the elements in the list using a SortFilter.
     * @param[in] sorter defines the comparison criteria.
     */
    void ListBSort(SortFilter * const sorter);

    /**
     * @brief Sorts the elements in the list.
     * @param[in] sorter defines the comparison criteria.
     */
    void ListBSort(SortFilterFn * const sorter);

    /**
     * @brief Browses the list.
     * @param[in] index is the position of the requested element (0 means the first element).
     * @return a pointer to the element at index position.
     */
    LinkedListable *ListPeek(const uint32 index);

    /**
     * @brief Removes and gets the element of the list in a specified position.
     * @param[in] index is the position of the requested element.
     * @return a pointer to the element at index position.
     */
    LinkedListable *ListExtract(uint32 index = 0u);

    /**
     * @brief For each item in the list do something.
     * @param[in] it defines what to do.
     */
    void ListIterate(Iterator * const it);

    /**
     * @brief For each item in the list do something.
     * @param[in] it defines what to do.
     */
    void ListIterate(IteratorFn * const it);

private:

    /**
     * List root
     */
    LinkedListable llhRoot;

    /**
     * List size
     */
    uint32 llhSize;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

void LinkedListHolder::FastListInsertSingle(LinkedListable &p) {
    llhSize++;
    p.SetNext(llhRoot.Next());
    llhRoot.SetNext(&p);
}
#endif /* LINKEDLISTHOLDER_H_ */

