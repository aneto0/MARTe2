/**
 * @file LinkedListHolderT.h
 * @brief Header file for class LinkedListHolderT
 * @date 12/04/2016
 * @author Filippo Sartori
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

#include "IteratorT.h"
#include "LinkedListable.h"
#include "LinkedListHolder.h"
#include "SortFilter.h"
#include "SearchFilter.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Template implementation of LinkedListHolder.
 */
template<class T, bool canDestroy = true>
class LinkedListHolderT: protected LinkedListHolder {

public:

    /**
     * @brief Default constructor.
     */
    LinkedListHolderT();

    /**
     * @brief Destructor.
     */
    virtual ~LinkedListHolderT();

    /**
     * @brief @see LinkedListHolder::List()
     */
    inline T *List();

    /**
     * @brief @see LinkedListHolder::ListSize()
     */
    inline uint32 ListSize() const;

    /**
     * @brief @see LinkedListHolder::Reset()
     */
    inline void Reset();

    /**
     * @brief @see LinkedListHolder::CleanUp()
     */
    inline void CleanUp();

    /**
     * @brief @see LinkedListHolder::FastListInsertSingle()
     */
    inline void FastListInsertSingle(T &p);

    /**
     * @brief @see LinkedListHolder::ListInsert()
     */
    inline void ListInsert(T * const p);

    /**
     * @brief @see LinkedListHolder::ListInsert()
     */
    inline void ListInsert(T * const p,
                           SortFilter * const sorter);

    /**
     * @brief @see LinkedListHolder::ListInsert()
     */
    inline void ListInsert(T * const q, const uint32 index);

    /**
     * @brief @see LinkedListHolder::ListAdd()
     */
    inline void ListAdd(T * const p);

    /**
     * @brief @see LinkedListHolder::ListAdd()
     */
    inline void ListAddL(T * const p);

    /**
     * @brief @see LinkedListHolder::ListSearch()
     */
    inline bool ListSearch(const T * const p);

    /**
     * @brief @see LinkedListHolder::ListSearch()
     */
    inline T *ListSearch(SearchFilter * const filter);

    /**
     * @brief @see LinkedListHolder::ListExtract()
     */
    inline bool ListExtract(T * const p);

    /**
     * @brief @see LinkedListHolder::ListExtract()
     */
    inline T *ListExtract(SearchFilter * const filter);

    /**
     * @brief @see LinkedListHolder::ListDelete()
     */
    inline bool ListDelete(T * const p);

    /**
     * @brief @see LinkedListHolder::ListDelete()
     */
    inline bool ListDelete(SearchFilter * const filter);

    /**
     * @brief @see LinkedListHolder::ListSafeDelete()
     */
    inline bool ListSafeDelete(SearchFilter * const filter);

    /**
     * @brief @see LinkedListHolder::ListBSort()
     */
    inline void ListBSort(SortFilter * const sorter);

    /**
     * @brief @see LinkedListHolder::ListPeek()
     */
    inline T *ListPeek(const uint32 index);

    /**
     * @brief @see LinkedListHolder::ListExtract()
     */
    inline T *ListExtract(uint32 index = 0u);

    /**
     * @brief @see LinkedListHolder::ListIterate()
     */
    inline void ListIterate(IteratorT<T> * const it);
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template <class T, bool canDestroy>
LinkedListHolderT<T, canDestroy>::LinkedListHolderT() :
        LinkedListHolder(canDestroy) {

}

template <class T, bool canDestroy>
LinkedListHolderT<T, canDestroy>::~LinkedListHolderT() {
}

template <class T, bool canDestroy>
T *LinkedListHolderT<T, canDestroy>::List() {
    return static_cast<T*>(LinkedListHolder::List());
}

template <class T, bool canDestroy>
uint32 LinkedListHolderT<T, canDestroy>::ListSize() const {
    return LinkedListHolder::ListSize();
}

template <class T, bool canDestroy>
void LinkedListHolderT<T, canDestroy>::Reset() {
    return LinkedListHolder::Reset();
}

template <class T, bool canDestroy>
void LinkedListHolderT<T, canDestroy>::CleanUp() {
    LinkedListHolder::CleanUp();
}

template <class T, bool canDestroy>
void LinkedListHolderT<T, canDestroy>::FastListInsertSingle(T &p) {
    LinkedListHolder::FastListInsertSingle(p);
}

template <class T, bool canDestroy>
void LinkedListHolderT<T, canDestroy>::ListInsert(T * const p) {
    LinkedListHolder::ListInsert(p);
}

template <class T, bool canDestroy>
void LinkedListHolderT<T, canDestroy>::ListInsert(T * const p,
                                   SortFilter * const sorter) {
    LinkedListHolder::ListInsert(p, sorter);
}

template <class T, bool canDestroy>
void LinkedListHolderT<T, canDestroy>::ListInsert(T * const q,
                                   const uint32 index) {
    LinkedListHolder::ListInsert(q, index);
}

template <class T, bool canDestroy>
void LinkedListHolderT<T, canDestroy>::ListAdd(T * const p) {
    LinkedListHolder::ListAdd(p);
}

template <class T, bool canDestroy>
void LinkedListHolderT<T, canDestroy>::ListAddL(T * const p) {
    LinkedListHolder::ListAddL(p);
}

template <class T, bool canDestroy>
bool LinkedListHolderT<T, canDestroy>::ListSearch(const T * const p) {
    return LinkedListHolder::ListSearch(p);
}

template <class T, bool canDestroy>
T *LinkedListHolderT<T, canDestroy>::ListSearch(SearchFilter * const filter) {
    LinkedListable *ll = LinkedListHolder::ListSearch(filter);
    return static_cast<T*>(ll);
}

template <class T, bool canDestroy>
bool LinkedListHolderT<T, canDestroy>::ListExtract(T * const p) {
    return LinkedListHolder::ListExtract(p);
}

template <class T, bool canDestroy>
T *LinkedListHolderT<T, canDestroy>::ListExtract(SearchFilter * const filter) {
    return static_cast<T*>(LinkedListHolder::ListExtract(filter));
}

template <class T, bool canDestroy>
bool LinkedListHolderT<T, canDestroy>::ListDelete(T * const p) {
    return LinkedListHolder::ListDelete(p);
}

template <class T, bool canDestroy>
bool LinkedListHolderT<T, canDestroy>::ListDelete(SearchFilter * const filter) {
    return LinkedListHolder::ListDelete(filter);
}

template <class T, bool canDestroy>
bool LinkedListHolderT<T, canDestroy>::ListSafeDelete(SearchFilter * const filter) {
    return LinkedListHolder::ListSafeDelete(filter);
}

template <class T, bool canDestroy>
void LinkedListHolderT<T, canDestroy>::ListBSort(SortFilter * const sorter) {
    LinkedListHolder::ListBSort(sorter);
}

template <class T, bool canDestroy>
T *LinkedListHolderT<T, canDestroy>::ListPeek(const uint32 index) {
    return static_cast<T*>(LinkedListHolder::ListPeek(index));
}

template <class T, bool canDestroy>
T *LinkedListHolderT<T, canDestroy>::ListExtract(uint32 index) {
    return static_cast<T*>(LinkedListHolder::ListExtract(index));
}

template <class T, bool canDestroy>
void LinkedListHolderT<T, canDestroy>::ListIterate(IteratorT<T> * const it) {
    LinkedListHolder::ListIterate(it);
}
}

#endif /* LINKEDLISTHOLDERT_H_ */

