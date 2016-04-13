/**
 * @file LinkedListHolderT.h
 * @brief Header file for class LinkedListHolderT
 * @date Apr 12, 2016
 * @author fsartori
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

 * @details This header file contains the declaration of the class LinkedListHolderT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LINKEDLISTHOLDERT_H_
#define LINKEDLISTHOLDERT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "LinkedListableT.h"
#include "LinkedListHolder.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template <class T>
class LinkedListHolderT:protected LinkedListHolder{


public:


    /**
     * @brief Destructor.
     */
    virtual ~LinkedListHolderT(){
    }

    /**
     * @brief Returns the first element of the list.
     * @return a pointer to the first element of the list.
     */
    inline LinkedListableT<T> *List(){
        return static_cast<LinkedListableT<T> *>(LinkedListHolder::List());
    }

    /**
     * @brief Inserts \p at the beginning of the list.
     * @param[in] p the LinkedListable object to insert.
     * @pre \a p must be a single LinkedListable element, as his next pointer will be set
     *      to the previous first element of the list after the execution of the function.
     *      If instead \a p is a list, this could lead to memory leaks.
     */
    inline void FastListInsertSingle(LinkedListableT<T> &p){
        LinkedListHolder::FastListInsertSingle(p);
    }

    /**
     * @brief Inserts at the beginning of the list \a p. \a p can be a single element or an entire list.
     * @param[in] p a pointer to the LinkedListable to insert.
     */
    inline void ListInsert(LinkedListableT<T> * const p){
        LinkedListHolder::ListInsert(p);
    }

    /**
     * @brief Inserts the input list using a SortFilter.
     * @details If sorter is NULL, the unsorted ListInsert(LinkedListable *) function is called,
     * inserting the input list at the beginning.
     * @param[in] p the pointer to the LinkedListable to insert.
     * @param[in] sorter implements the comparison criteria for the sorting.
     */
    inline void ListInsert(LinkedListableT<T> * const p,SortFilterT<T> * const sorter){
        LinkedListHolder::ListInsert(p,sorter);
    }

    /**
     * @brief Inserts a list in the specified position of the list.
     * @details If the specified position is greater the the list size, the list input is inserted at the end.
     * @param[in] q the pointer to the LinkedListable to be inserted.
     * @param[in] index is the position in the list where \a p must be inserted.
     */
    inline void ListInsert(LinkedListableT<T> * const q,const uint32 index){
        LinkedListHolder::ListInsert(q,index);
    }

    /**
     * @brief Adds an element at the end of this list.
     * @param[in] p the pointer to the LinkedListable element to be appended.
     * @pre \a p must be a single LinkedListable element, as his next pointer will be set
     *      to NULL after the execution of the function. If instead \a p is a list, Add
     *      could lead to memory leaks.
     */
    inline void ListAdd(LinkedListableT<T> * const p){
        LinkedListHolder::ListAdd(p);
    }

    /**
     * @brief Adds a LinkedListable list at the end of the list.
     * @param[in] p the pointer to the LinkedListable list to added.
     */
    inline void ListAddL(LinkedListableT<T> * const p){
        LinkedListHolder::ListAddL(p);
    }

    /**
     * @brief Searches a specified element in this list.
     * @param[in] p a pointer to the element to search.
     * @return true if \a p is in the list, false otherwise.
     */
    inline bool ListSearch(const LinkedListableT<T> * const p){
        return LinkedListHolder::ListSearch(p);
    }

    /**
     * @brief Searches an element using a SearchFilter.
     * @param[in] filter defines the search criteria.
     * @return a pointer to the element if it is found in the list, NULL otherwise.
     */
    inline LinkedListableT<T> *ListSearch(SearchFilterT<T> * const filter){
        return static_cast<LinkedListableT<T> *>(LinkedListHolder::ListSearch(filter));
    }

    /**
     * @brief Removes the requested element from the list.
     * @param[in] p the element to be removed.
     * @return true if \a p was in the list, false otherwise.
     */
    inline bool ListExtract(LinkedListableT<T> * const p){
        return LinkedListHolder::ListExtract(p);
    }

    /**
     * @brief Finds and removes one element from list using a SearchFilter.
     * @param[in] filter defines the search criteria.
     * @return the extracted element, NULL if it is not found in the list.
     */
    inline LinkedListableT<T> *ListExtract(SearchFilterT<T> * const filter){
        return static_cast<LinkedListableT<T> *>(LinkedListHolder::ListExtract(filter));
    }

    /**
     * @brief Deletes the requested element.
     * @param[in] p is the element which must be deleted.
     * @return true if p is in the list, false otherwise.
     */
    inline bool ListDelete(LinkedListableT<T> * const p){
        return LinkedListHolder::ListDelete(p);
    }

    /**
     * @brief Deletes elements using a SearchFilter.
     * @param[in] filter defines the search criteria.
     * @return true if at least one element is deleted, false otherwise.
     */
    inline bool ListDelete(SearchFilterT<T> * const filter){
        return LinkedListHolder::ListDelete(filter);
    }

    /**
     * @brief Deletes a elements using a SearchFilter in safe mode.
     * @details Safe from being reentrant from the destructor of the object (complex graph destruction)
     * @param[in] filter defines the search criteria.
     * @return true if at least one object it is deleted, false otherwise.
     */
    inline bool ListSafeDelete(SearchFilterT<T> * const filter){
        return LinkedListHolder::ListSafeDelete(filter);
    }

    /**
     * @brief Sorts the elements in the list using a SortFilter.
     * @param[in] sorter defines the comparison criteria.
     */
    inline void ListBSort(SortFilterT<T> * const sorter){
        LinkedListHolder::ListBSort(sorter);
    }

    /**
     * @brief Browses the list.
     * @param[in] index the position of the requested element (0 means the first element).
     * @return a pointer to the element at index position.
     */
    inline LinkedListableT<T> *ListPeek(const uint32 index){
        return static_cast<LinkedListableT<T>*>(LinkedListHolder::ListPeek(index));
    }

    /**
     * @brief Removes and gets the element of the list in a specified position.
     * @param[in] index the position of the requested element.
     * @return a pointer to the element at index position.
     */
    inline LinkedListableT<T> *ListExtract(uint32 index = 0u){
        return static_cast<LinkedListableT<T>* >(LinkedListHolder::ListExtract(index));
    }

    /**
     * @brief For each item in the list perform the action defined by the Iterator.
     * @param[in] it defines the action to be performed on each element.
     */
    inline void ListIterate(IteratorT<T> * const it){
        LinkedListHolder::ListIterate(it);
    }
};

}



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LINKEDLISTHOLDERT_H_ */
	
