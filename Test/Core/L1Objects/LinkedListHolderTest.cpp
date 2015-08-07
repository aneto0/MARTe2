/**
 * @file LinkedListHolderTest.cpp
 * @brief Source file for class LinkedListHolderTest
 * @date 06/08/2015
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

 * @details This source file contains the definition of all the methods for
 * the class LinkedListHolderTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "LinkedListHolderTest.h"
#include "ListTestHelper.h"
#include "Threads.h"
#include "Sleep.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool LinkedListHolderTest::TestConstructor() {

    LinkedListHolder list;

    return (list.ListSize() == 0) && (list.List() == NULL);

}

bool LinkedListHolderTest::TestDestructor() {

    LinkedListHolder list;

    LinkedListable *element = new LinkedListable;

    list.ListInsert(element);
    if (list.ListSize() != 1) {
        return false;
    }

    list.~LinkedListHolder();

    return (list.ListSize() == 0) && (list.List() == NULL);

}

bool LinkedListHolderTest::TestReset(uint32 nElements) {

    LinkedListHolder list;

    LinkedListable *firstElement = new LinkedListable;

    list.ListInsert(firstElement);

    for (uint32 i = 0; i < nElements; i++) {
        LinkedListable *element = new LinkedListable;

        list.ListInsert(element);
    }

    if (list.List() == NULL) {
        return false;
    }

    list.Reset();

    //the elements should be still valid
    if (firstElement->Size() == 0) {
        return false;
    }

    return (list.ListSize() == 0) && (list.List() == NULL);
}

bool LinkedListHolderTest::TestCleanup(uint32 nElements) {

    LinkedListHolder list;

    for (uint32 i = 0; i < nElements; i++) {
        LinkedListable *element = new LinkedListable;

        list.ListInsert(element);
    }

    if (list.List() == NULL) {
        return false;
    }

    list.Reset();

    return (list.ListSize() == 0) && (list.List() == NULL);
}

bool LinkedListHolderTest::TestList(uint32 nElements) {

    LinkedListHolder list;

    if (list.List() != NULL) {
        return false;
    }

    LinkedListable *firstElement = new LinkedListable;

    list.ListAdd(firstElement);

    for (uint32 i = 0; i < nElements; i++) {
        LinkedListable *element = new LinkedListable;

        list.ListAdd(element);
    }

    if (list.List() != firstElement) {
        return false;
    }

    LinkedListable *newFirstElement = firstElement->Next();
    list.ListExtract(firstElement);

    return list.List() == newFirstElement;

}

bool LinkedListHolderTest::TestListSize(uint32 nElements) {

    LinkedListHolder list;

    if (list.ListSize() != 0) {
        return false;
    }
    for (uint32 i = 0; i < nElements; i++) {
        LinkedListable *element = new LinkedListable;

        list.ListInsert(element);
    }

    return list.ListSize() == nElements;

}

bool LinkedListHolderTest::TestFastListInsertSingle(uint32 nElements) {

    LinkedListHolder list;

    IntegerList *first = new IntegerList(0);
    IntegerList *cursor = first;
    for (uint32 i = 1; i < nElements; i++) {
        IntegerList *element = new IntegerList(i);
        cursor->SetNext(element);
        cursor = (IntegerList*) (cursor->Next());
    }

    cursor = first;
    for (uint32 i = 0; i < nElements; i++) {
        IntegerList *next = (IntegerList*) (cursor->Next());
        list.FastListInsertSingle(*cursor);
        if (list.ListSize() != (i + 1)) {
            return false;
        }
        cursor = next;
    }

    for (uint32 i = 0; i < nElements; i++) {
        if (((IntegerList*) list.ListPeek(i))->intNumber != (31 - i)) {
            return false;
        }
    }

    return list.ListSize() == nElements;
}

void BuildLists(IntegerList* list1Cursor,
                IntegerList *list2Cursor,
                uint32 size,
                bool oppositeOrder) {

    for (uint32 i = 1; i < size; i++) {
        IntegerList *list1Element;
        if (!oppositeOrder) {
            list1Element = new IntegerList(i);
        }
        else {
            list1Element = new IntegerList(size - i);
        }
        IntegerList *list2Element = new IntegerList(size + i);

        list1Cursor->SetNext(list1Element);
        list2Cursor->SetNext(list2Element);

        list1Cursor = (IntegerList*) list1Cursor->Next();
        list2Cursor = (IntegerList*) list2Cursor->Next();
    }
}

bool LinkedListHolderTest::TestListInsert(uint32 nElements) {

    LinkedListHolder list;

    IntegerList *first1 = new IntegerList(0);
    IntegerList *first2 = new IntegerList(nElements);

    BuildLists(first1, first2, nElements, false);

    list.ListInsert(first2);
    list.ListInsert(first1);

    for (uint32 i = 0; i < (2 * nElements); i++) {
        if (((IntegerList *) (list.ListPeek(i)))->intNumber != i) {
            return false;
        }
    }

    list.ListInsert((LinkedListable*) NULL);

    for (uint32 i = 0; i < (2 * nElements); i++) {
        if (((IntegerList *) (list.ListPeek(i)))->intNumber != i) {
            return false;
        }
    }

    return list.ListSize() == (2 * nElements);

}

bool LinkedListHolderTest::TestListInsertSortedSorter(uint32 nElements) {

    LinkedListHolder list;

    IntegerList *first1 = new IntegerList(0);
    IntegerList *first2 = new IntegerList(nElements);

    BuildLists(first1, first2, nElements, false);

    SortDecrescent decrescentSorter;

    list.ListInsert(first1, &decrescentSorter);

    list.ListInsert(first2, &decrescentSorter);

    for (uint32 i = 0; i < (2 * nElements - 1); i++) {
        if ((((IntegerList*) list.ListPeek(i + 1))->intNumber) > (((IntegerList*) list.ListPeek(i))->intNumber)) {
            return false;
        }

    }

    return list.ListSize() == (2 * nElements);

}

bool LinkedListHolderTest::TestListInsertSortedFn(uint32 nElements) {

    LinkedListHolder list;

    IntegerList *first1 = new IntegerList(0);
    IntegerList *first2 = new IntegerList(nElements);

    BuildLists(first1, first2, nElements, false);

    list.ListInsert(first2, DecrescentSortFn);

    list.ListInsert(first1, DecrescentSortFn);

    for (uint32 i = 0; i < (2 * nElements - 1); i++) {
        if ((((IntegerList*) list.ListPeek(i + 1))->intNumber) > (((IntegerList*) list.ListPeek(i))->intNumber)) {
            return false;
        }

    }

    return list.ListSize() == (2 * nElements);

}

bool LinkedListHolderTest::TestListInsertNullSorter(uint32 nElements) {

    LinkedListHolder list;

    IntegerList *first1 = new IntegerList(0);
    IntegerList *first2 = new IntegerList(nElements);

    BuildLists(first1, first2, nElements, false);

    list.ListInsert(first2, (SortFilter*) NULL);

    list.ListInsert(first1, (SortFilterFn*) NULL);

    //insert without sorting
    for (uint32 i = 0; i < (2 * nElements); i++) {
        if ((((IntegerList*) list.ListPeek(i))->intNumber) != i) {
            return false;
        }

    }

    return list.ListSize() == (2 * nElements);

}

bool LinkedListHolderTest::TestListInsertIndex(uint32 index,
                                               uint32 size) {
    LinkedListHolder list;

    IntegerList *first1 = new IntegerList(0);
    IntegerList *first2 = new IntegerList(size);

    BuildLists(first1, first2, size, false);

    //list is empty: it inserts on the queue
    list.ListInsert(first1, index);

    list.ListInsert(first2, index);

    //for the check saturate index
    if (index > size) {
        index = size;
    }

    //the first list
    for (uint32 i = 0; i < index; i++) {
        if ((((IntegerList*) list.ListPeek(i))->intNumber) != i) {
            return false;
        }
    }

    //the second list
    for (uint32 i = 0; i < size; i++) {
        if ((((IntegerList*) list.ListPeek(index + i))->intNumber) != (size + i)) {
            return false;
        }
    }

    //the first list again
    for (uint32 i = 0; i < (size - index); i++) {
        if ((((IntegerList*) list.ListPeek(index + size + i))->intNumber) != (index + i)) {
            return false;
        }
    }

    return list.ListSize() == 2 * size;

}

bool LinkedListHolderTest::TestListAddL(uint32 nElements) {
    LinkedListHolder list;

    IntegerList *first1 = new IntegerList(0);
    IntegerList *first2 = new IntegerList(nElements);

    BuildLists(first1, first2, nElements, false);

    //list is empty: it inserts on the queue
    list.ListAddL(first1);

    list.ListAddL((LinkedListable *) NULL);

    if (list.ListSize() != (nElements)) {
        return false;
    }

    for (uint32 i = 0; i < nElements; i++) {
        if ((((IntegerList*) list.ListPeek(i))->intNumber) != i) {
            return false;
        }
    }

    list.ListAddL(first2);

    for (uint32 i = 0; i < (2 * nElements); i++) {
        if ((((IntegerList*) list.ListPeek(i))->intNumber) != i) {
            return false;
        }
    }

    return list.ListSize() == (2 * nElements);

}

bool LinkedListHolderTest::TestListAdd(uint32 nElements) {
    LinkedListHolder list;

    IntegerList *first1 = new IntegerList(0);
    IntegerList *first2 = new IntegerList(nElements);

    BuildLists(first1, first2, nElements, false);

    //list is empty: it inserts on the queue
    list.ListAddL(first1);

    list.ListAdd((LinkedListable *) NULL);

    if (list.ListSize() != (nElements)) {
        return false;
    }

    for (uint32 i = 0; i < nElements; i++) {
        if ((((IntegerList*) list.ListPeek(i))->intNumber) != i) {
            return false;
        }
    }

    list.ListAdd(first2);

    if (list.ListSize() != (nElements + 1)) {
        return false;
    }

    return ((IntegerList*) list.ListPeek(nElements))->intNumber == first2->intNumber;

}

bool LinkedListHolderTest::TestListSearch() {

    LinkedListHolder list;

    LinkedListable *stored[32];

    stored[31] = new LinkedListable;

    for (uint32 i = 0; i < 31; i++) {

        stored[i] = new LinkedListable;
        list.ListAdd(stored[i]);

    }

    for (uint32 i = 0; i < 31; i++) {
        if (!list.ListSearch(stored[i])) {
            return false;
        }
    }

    return (!list.ListSearch(stored[31]));
}

bool LinkedListHolderTest::TestListSearchFilter() {
    LinkedListHolder list;

    LinkedListable *stored[32];

    for (uint32 i = 0; i < 32; i++) {

        stored[i] = new IntegerList(i);
        list.ListAdd(stored[i]);

    }

    for (uint32 i = 0; i < 32; i++) {
        SearchInteger searchNumber(i);
        if (list.ListSearch(&searchNumber) != stored[i]) {
            return false;
        }
    }

    SearchInteger searchNotIn(33);
    return (list.ListSearch(&searchNotIn) == NULL) && (list.ListSearch((SearchFilter*) NULL) == NULL);
}

bool LinkedListHolderTest::TestListSearchFn() {
    LinkedListHolder list;

    LinkedListable *stored[32];

    for (uint32 i = 0; i < 32; i++) {

        stored[i] = new IntegerList(i);
        list.ListAdd(stored[i]);

    }

    for (uint32 i = 0; i < 32; i++) {
        nToSearch = i;
        if (list.ListSearch(SearchIntFn) != stored[i]) {
            return false;
        }
    }

    nToSearch = 33;
    return (list.ListSearch(SearchIntFn) == NULL) && (list.ListSearch((SearchFilterFn*) NULL) == NULL);
}

bool LinkedListHolderTest::TestListExtract() {
    LinkedListHolder list;

    LinkedListable *stored[32];

    for (uint32 i = 0; i < 31; i++) {

        stored[i] = new LinkedListable;
        list.ListAdd(stored[i]);

    }

    stored[31] = new LinkedListable;

    if (list.ListExtract(stored[31])) {
        return false;
    }

    for (uint32 i = 0; i < 31; i++) {
        if (!list.ListExtract(stored[i])) {
            return false;
        }

        if (stored[i]->Next() != NULL) {
            return false;
        }

        if (list.ListSize() != (30 - i)) {
            return false;
        }
    }

    return list.ListSize() == 0;
}

bool LinkedListHolderTest::TestListExtractSearchFn() {

    LinkedListHolder list;

    LinkedListable *stored[32];

    for (uint32 i = 0; i < 32; i++) {

        stored[i] = new IntegerList(i);
        list.ListAdd(stored[i]);

    }

    SearchInteger searchNotIn(33);

    if (list.ListExtract(&searchNotIn) != NULL) {
        return false;
    }

    if (list.ListExtract((SearchFilter*) NULL) != NULL) {
        return false;
    }

    for (uint32 i = 0; i < 32; i++) {
        SearchInteger searchNumber(i);
        if (list.ListExtract(&searchNumber) != stored[i]) {
            return false;
        }

        if (stored[i]->Next() != NULL) {
            return false;
        }

        if (list.ListExtract(&searchNumber) != NULL) {
            return false;
        }

        if (list.ListSize() != (31 - i)) {
            return false;
        }
    }

    return list.ListSize() == 0;

}

bool LinkedListHolderTest::TestListExtractSearchFilter() {

    LinkedListHolder list;

    LinkedListable *stored[32];

    for (uint32 i = 0; i < 32; i++) {

        stored[i] = new IntegerList(i);
        list.ListAdd(stored[i]);

    }

    nToSearch = 33;

    if (list.ListExtract(SearchIntFn) != NULL) {
        return false;
    }

    if (list.ListExtract((SearchFilterFn*) NULL) != NULL) {
        return false;
    }

    for (uint32 i = 0; i < 32; i++) {
        nToSearch = i;
        if (list.ListExtract(SearchIntFn) != stored[i]) {
            return false;
        }

        if (stored[i]->Next() != NULL) {
            return false;
        }

        if (list.ListExtract(SearchIntFn) != NULL) {
            return false;
        }

        if (list.ListSize() != (31 - i)) {
            return false;
        }
    }

    return list.ListSize() == 0;

}

bool LinkedListHolderTest::TestListExtractIndex() {
    LinkedListHolder list;

    LinkedListable *stored[32];

    for (uint32 i = 0; i < 32; i++) {

        stored[i] = new LinkedListable;
        list.ListAdd(stored[i]);

    }

    if (list.ListExtract(32) != NULL) {
        return false;
    }

    for (int32 i = 31; i >= 0; i--) {
        if (list.ListExtract(i) != stored[i]) {
            return false;
        }

        if (stored[i]->Next() != NULL) {
            return false;
        }

        if (list.ListSize() != (uint32) i) {
            return false;
        }
    }

    return list.ListSize() == 0;
}

bool LinkedListHolderTest::TestListDelete() {

    LinkedListHolder list;

    LinkedListable *stored[32];

    for (uint32 i = 0; i < 32; i++) {

        stored[i] = new LinkedListable;
        list.ListAdd(stored[i]);

    }

    LinkedListable* notIn = new LinkedListable;

    if (list.ListDelete(notIn)) {
        return false;
    }

    if (list.ListDelete((LinkedListable*) NULL)) {
        return false;
    }

    for (uint32 i = 0; i < 32; i++) {
        if (!list.ListDelete(stored[i])) {
            return false;
        }
        if (list.ListSize() != (31 - i)) {
            return false;
        }
    }

    return list.ListSize() == 0;
}

bool LinkedListHolderTest::TestListDeleteSearchFilter() {

    LinkedListHolder list;

    LinkedListable *stored[32];

    for (uint32 i = 0; i < 32; i++) {

        stored[i] = new IntegerList(i % 16);
        list.ListAdd(stored[i]);

    }

    SearchInteger searchNotIn(33);

    if (list.ListDelete(&searchNotIn)) {
        return false;
    }
    if (list.ListDelete((SearchFilter*) NULL)) {
        return false;
    }

    for (uint32 i = 0; i < 16; i++) {
        SearchInteger searchNumber(i % 16);
        if (!list.ListDelete(&searchNumber)) {
            return false;
        }

        if (list.ListDelete(&searchNumber)) {
            return false;
        }

        if (list.ListSize() != (32 - 2 * (i + 1))) {
            return false;
        }
    }

    return list.ListSize() == 0;

}

bool LinkedListHolderTest::TestListDeleteSearchFn() {

    LinkedListHolder list;

    LinkedListable *stored[32];

    for (uint32 i = 0; i < 32; i++) {

        stored[i] = new IntegerList(i % 16);
        list.ListAdd(stored[i]);

    }

    nToSearch = 33;

    if (list.ListDelete(SearchIntFn)) {
        return false;
    }

    if (list.ListDelete((SearchFilterFn*) NULL)) {
        return false;
    }

    for (uint32 i = 0; i < 16; i++) {

        nToSearch = i % 16;
        if (!list.ListDelete(SearchIntFn)) {
            return false;
        }

        if (list.ListDelete(SearchIntFn)) {
            return false;
        }

        if (list.ListSize() != (32 - 2 * (i + 1))) {
            return false;
        }
    }

    return list.ListSize() == 0;

}

void DeleteList(LinkedListHolderTest &t) {

    for (uint32 i = 0; i < 32; i++) {
        SearchInteger searchNumber(i);
        t.internalList.ListSafeDelete(&searchNumber);
        Sleep::MSec(1);
    }

}

bool LinkedListHolderTest::TestListSafeDelete() {

    LinkedListHolder list;

    LinkedListable *stored[32];

    for (uint32 i = 0; i < 32; i++) {

        stored[i] = new IntegerList(i % 16);
        list.ListAdd(stored[i]);

    }

    SearchInteger searchNotIn(33);

    if (list.ListSafeDelete(&searchNotIn)) {
        return false;
    }
    if (list.ListSafeDelete((SearchFilter*) NULL)) {
        return false;
    }

    for (uint32 i = 0; i < 16; i++) {
        SearchInteger searchNumber(i % 16);
        if (!list.ListSafeDelete(&searchNumber)) {
            return false;
        }

        if (list.ListSafeDelete(&searchNumber)) {
            return false;
        }

        if (list.ListSize() != (32 - 2 * (i + 1))) {
            return false;
        }
    }

    return list.ListSize() == 0;/*
     for (uint32 i = 0; i < 32; i++) {

     IntegerList *element = new IntegerList(i);
     internalList.ListAdd(element);
     }

     Threads::BeginThread((ThreadFunctionType) DeleteList, this);
     Threads::BeginThread((ThreadFunctionType) DeleteList, this);

     while (Threads::NumberOfThreads() != 0)
     ;
     return true;*/
}

bool LinkedListHolderTest::TestListBSortSorter(uint32 nElements) {

    LinkedListHolder list;

    IntegerList *first1 = new IntegerList(0);
    IntegerList *first2 = new IntegerList(nElements);

    BuildLists(first1, first2, nElements, true);

    list.ListAddL(first2);
    list.ListAddL(first1);

    list.ListBSort((SortDecrescent*) NULL);

    //still in crescent order, nothing should happen
    for (uint32 i = 0; i < (nElements - 1); i++) {
        if ((((IntegerList*) list.ListPeek(i))->intNumber + 1) != ((IntegerList*) list.ListPeek(i + 1))->intNumber) {
            return false;
        }
    }

    SortDecrescent decrescentSorter;

    list.ListBSort(&decrescentSorter);
    //still in crescent order, nothing should happen
    for (uint32 i = 0; i < (2 * nElements - 1); i++) {
        if (((IntegerList*) list.ListPeek(i))->intNumber != (((IntegerList*) list.ListPeek(i + 1))->intNumber + 1)) {

            return false;
        }
    }

    return true;
}

bool LinkedListHolderTest::TestListBSortFn(uint32 nElements) {

    LinkedListHolder list;

    IntegerList *first1 = new IntegerList(0);
    IntegerList *first2 = new IntegerList(nElements);

    BuildLists(first1, first2, nElements, true);

    list.ListAddL(first2);
    list.ListAddL(first1);

    list.ListBSort((SortFilterFn*) NULL);

    //still in crescent order, nothing should happen
    for (uint32 i = 0; i < (nElements - 1); i++) {
        if ((((IntegerList*) list.ListPeek(i))->intNumber + 1) != ((IntegerList*) list.ListPeek(i + 1))->intNumber) {
            return false;
        }
    }

    list.ListBSort(DecrescentSortFn);
    //still in crescent order, nothing should happen
    for (uint32 i = 0; i < (2 * nElements - 1); i++) {
        if (((IntegerList*) list.ListPeek(i))->intNumber != (((IntegerList*) list.ListPeek(i + 1))->intNumber + 1)) {
            return false;
        }
    }

    return true;
}

bool LinkedListHolderTest::TestListPeek(uint32 nElements) {

    LinkedListHolder list;

    for (uint32 i = 0; i < nElements; i++) {

        IntegerList* element = new IntegerList(i);
        list.ListAdd(element);
    }

    if (list.ListPeek(nElements) != NULL) {
        return false;
    }

    for (uint32 i = 0; i < nElements; i++) {
        if (((IntegerList*) list.ListPeek(i))->intNumber != i) {
            return false;
        }
    }

    return list.ListSize() == nElements;
}

bool LinkedListHolderTest::TestListIterateIterator() {

    LinkedListHolder list;

    IntegerList *stored[32];

    for (uint32 i = 0; i < 32; i++) {

        stored[i] = new IntegerList(i);
        list.ListAdd(stored[i]);

    }

    list.ListIterate((Iterator*) NULL);

    for (uint32 i = 0; i < 32; i++) {
        if (stored[i]->intNumber != i) {
            return false;
        }
    }

    IncrementIterator addOne;

    list.ListIterate(&addOne);

    for (uint32 i = 0; i < 32; i++) {
        if (stored[i]->intNumber != (i + 1)) {
            return false;
        }
    }

    return list.ListSize() == 32;

}

bool LinkedListHolderTest::TestListIterateFn() {

    LinkedListHolder list;

    IntegerList *stored[32];

    for (uint32 i = 0; i < 32; i++) {

        stored[i] = new IntegerList(i);
        list.ListAdd(stored[i]);

    }

    list.ListIterate((IteratorFn*) NULL);

    for (uint32 i = 0; i < 32; i++) {
        if (stored[i]->intNumber != i) {
            return false;
        }
    }

    list.ListIterate(IncrementNumFn);

    for (uint32 i = 0; i < 32; i++) {
        if (stored[i]->intNumber != (i + 1)) {
            return false;
        }
    }

    return list.ListSize() == 32;

}
