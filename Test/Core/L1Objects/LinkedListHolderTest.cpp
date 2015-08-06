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
#include "stdio.h"
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
            list1Element = new IntegerList(size - i - 1);
        }
        IntegerList *list2Element = new IntegerList(size + i);

        list1Cursor->SetNext(list1Element);
        list2Cursor->SetNext(list2Element);

        list1Cursor = (IntegerList*) list1Cursor->Next();
        list2Cursor = (IntegerList*) list2Cursor->Next();
    }
}

bool LinkedListHolderTest::TestListInsert() {

    LinkedListHolder list;

    IntegerList *first1 = new IntegerList(0);
    IntegerList *first2 = new IntegerList(32);

    BuildLists(first1, first2, 32, false);

    list.ListInsert(first2);
    list.ListInsert(first1);

    for (uint32 i = 0; i < 64; i++) {
        if (((IntegerList *) (list.ListPeek(i)))->intNumber != i) {
            return false;
        }
    }

    list.ListInsert((LinkedListable*) NULL);

    for (uint32 i = 0; i < 64; i++) {
        if (((IntegerList *) (list.ListPeek(i)))->intNumber != i) {
            return false;
        }
    }

    return list.ListSize() == 64;

}

bool LinkedListHolderTest::TestListInsertSortedSorter() {

    LinkedListHolder list;

    IntegerList *first1 = new IntegerList(0);
    IntegerList *first2 = new IntegerList(32);

    BuildLists(first1, first2, 32, false);

    SortDecrescent decrescentSorter;

    list.ListInsert(first1, &decrescentSorter);

    list.ListInsert(first2, &decrescentSorter);

    for (uint32 i = 0; i < 63; i++) {
        if ((((IntegerList*) list.ListPeek(i + 1))->intNumber) > (((IntegerList*) list.ListPeek(i))->intNumber)) {
            return false;
        }

    }

    return list.ListSize() == 64;

}

bool LinkedListHolderTest::TestListInsertSortedFn() {

    LinkedListHolder list;

    IntegerList *first1 = new IntegerList(0);
    IntegerList *first2 = new IntegerList(32);

    BuildLists(first1, first2, 32, false);

    list.ListInsert(first2, DecrescentSortFn);

    list.ListInsert(first1, DecrescentSortFn);

    for (uint32 i = 0; i < 63; i++) {
        if ((((IntegerList*) list.ListPeek(i + 1))->intNumber) > (((IntegerList*) list.ListPeek(i))->intNumber)) {
            return false;
        }

    }

    return list.ListSize() == 64;

}

bool LinkedListHolderTest::TestListInsertNullSorter() {

    LinkedListHolder list;

    IntegerList *first1 = new IntegerList(0);
    IntegerList *first2 = new IntegerList(32);

    BuildLists(first1, first2, 32, false);

    list.ListInsert(first2, (SortFilter*) NULL);

    list.ListInsert(first1, (SortFilterFn*) NULL);

    //insert without sorting
    for (uint32 i = 0; i < 64; i++) {
        if ((((IntegerList*) list.ListPeek(i))->intNumber) != i) {
            return false;
        }

    }

    return list.ListSize() == 64;

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

