/**
 * @file LinkedListableTest.cpp
 * @brief Source file for class LinkedListableTest
 * @date 05/06/2015
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
 * the class LinkedListableTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "LinkedListableTest.h"
#include "ListTestHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

uint32 nToSearch = 0;

bool LinkedListableTest::TestConstructor() {

    LinkedListable root;
    return (root.Next() == NULL) && (root.Size() == 1);
}

bool LinkedListableTest::TestDestructor() {

    LinkedListable root;
    LinkedListable firstElement;

    root.SetNext(&firstElement);

    if (root.Size() != 2) {
        return false;
    }

    root.~LinkedListable();

    return (root.Next() == NULL) && (root.Size() == 1);
}

bool LinkedListableTest::TestSetGetNext() {
    LinkedListable root;

    //checks if the next is null at the beginning
    if (root.Next() != NULL) {
        return false;
    }
    LinkedListable firstElement;

    //sets the next element
    root.SetNext(&firstElement);

    //checks that it is set correctly
    if (root.Next() != &firstElement) {
        return false;
    }
    LinkedListable secondElement;

    //sets the second element after the first
    firstElement.SetNext(&secondElement);

    //checks
    if (firstElement.Next() != &secondElement) {
        return false;
    }
    LinkedListable newFirstElement;

    //sets the first element again: all the previous sublist
    //should be replaced by the sublist of this new input
    root.SetNext(&newFirstElement);

    if (root.Next() != &newFirstElement || newFirstElement.Next() != NULL) {
        return false;
    }
    return firstElement.Next() == &secondElement;
}

bool LinkedListableTest::TestSize() {

    LinkedListable root;
    LinkedListable element[32];

    //size should be zero at the beginning
    if (root.Size() != 1) {
        return false;
    }

    root.SetNext(&element[0]);

    //fills the list
    for (uint32 i = 1; i < 32; i++) {
        element[i - 1].SetNext(&element[i]);
    }

    //checks if the size is equal to the current number of elements
    if (root.Size() != 33) {
        return false;
    }

    //checks if all element see only the sublist on its right
    for (uint32 i = 0; i < 32; i++) {
        if (element[i].Size() != (32 - i)) {
            return false;
        }
    }
    return true;
}

bool LinkedListableTest::TestBSortSorter() {
    IntegerList root;
    IntegerList number[32];

    number[0].intNumber = 0;
    root.SetNext(&number[0]);

    //fills the list in crescent order
    for (uint32 i = 1; i < 32; i++) {
        number[i].intNumber = i;
        number[i - 1].SetNext(&number[i]);
    }

    //nothing should happen
    root.BSort((SortFilter*) NULL);
    LinkedListable *cursor = root.Next();
    for (uint32 i = 0; i < 32; i++) {
        if (((IntegerList*) (cursor))->intNumber != i) {
            return false;
        }
        cursor = cursor->Next();
    }

    //sort the list in decrescent order
    SortDecrescent decrescentSorter;

    //sort only the last half part of the list
    number[14].BSort(&decrescentSorter);

    //checks if the order is crescent in the first half list
    cursor = root.Next();
    for (uint32 i = 0; i <= 14; i++) {
        if (((IntegerList*) (cursor))->intNumber != i) {
            return false;
        }
        cursor = cursor->Next();
    }

    //checks if the order is decrescent in the second half list
    for (uint32 i = 31; i >= 15; i--) {
        if (((IntegerList*) (cursor))->intNumber != i) {
            return false;
        }
        cursor = cursor->Next();
    }

    root.BSort(&decrescentSorter);

    //checks if the order is decrescent in all list
    cursor = root.Next();
    for (uint32 i = 0; i < 32; i++) {
        if (((IntegerList*) (cursor))->intNumber != (31 - i)) {
            return false;
        }
        cursor = cursor->Next();
    }

    return true;
}

bool LinkedListableTest::TestBSortFn() {
    IntegerList root;
    IntegerList number[32];

    number[0].intNumber = 0;
    root.SetNext(&number[0]);

    //fills the list in crescent order
    for (uint32 i = 1; i < 32; i++) {
        number[i].intNumber = i;
        number[i - 1].SetNext(&number[i]);
    }

    //nothing should happen
    root.BSort((SortFilterFn*) NULL);
    LinkedListable *cursor = root.Next();
    for (uint32 i = 0; i < 32; i++) {
        if (((IntegerList*) (cursor))->intNumber != i) {
            return false;
        }
        cursor = cursor->Next();
    }

    //sort only the last half part of the list
    number[14].BSort(DecrescentSortFn);

    //checks if the order is crescent in the first half list
    cursor = root.Next();
    for (uint32 i = 0; i <= 14; i++) {
        if (((IntegerList*) (cursor))->intNumber != i) {
            return false;
        }
        cursor = cursor->Next();
    }

    //checks if the order is decrescent in the second half list
    for (uint32 i = 31; i >= 15; i--) {
        if (((IntegerList*) (cursor))->intNumber != i) {
            return false;
        }
        cursor = cursor->Next();
    }

    root.BSort(DecrescentSortFn);

    //checks if the order is decrescent in all list
    cursor = root.Next();
    for (uint32 i = 0; i < 32; i++) {
        if (((IntegerList*) (cursor))->intNumber != (31 - i)) {
            return false;
        }
        cursor = cursor->Next();
    }

    return true;
}

void BuildLists(IntegerList* list1Element,
                IntegerList *list2Element,
                uint32 size) {

    list1Element[0].intNumber = 0;
    list2Element[0].intNumber = size;

    for (uint32 i = 1; i < size; i++) {
        list1Element[i].intNumber = i;
        list2Element[i].intNumber = size + i;

        list1Element[i - 1].SetNext(&list1Element[i]);
        list2Element[i - 1].SetNext(&list2Element[i]);
    }
}

bool LinkedListableTest::TestInsert(uint32 firstListInsertPoint,
                                    uint32 secondListBeginPoint) {

    //saturates because lists contains 32 elements
    if (firstListInsertPoint > 31) {
        firstListInsertPoint = 31;
    }
    if (secondListBeginPoint > 31) {
        secondListBeginPoint = 31;
    }

    //build the two lists
    IntegerList rootList1;
    IntegerList rootList2;

    IntegerList list1Element[32];
    IntegerList list2Element[32];

    rootList1.SetNext(&list1Element[0]);
    rootList2.SetNext(&list2Element[0]);

    BuildLists(list1Element, list2Element, 32);
    //insert the second (sub)list on the first at the specified point
    list1Element[firstListInsertPoint].Insert(&list2Element[secondListBeginPoint]);

    LinkedListable *cursor = rootList1.Next();
    for (uint32 i = 0; i < (firstListInsertPoint + 1); i++) {
        if (((IntegerList*) (cursor))->intNumber != i) {
            return false;
        }
        cursor = cursor->Next();
    }

    //the point when the second list end and the second continue
    uint32 point = (32 - secondListBeginPoint);

    for (uint32 i = 0; i < point; i++) {
        if (((IntegerList*) (cursor))->intNumber != (i + 32 + secondListBeginPoint)) {
            return false;
        }
        cursor = cursor->Next();
    }

    //checks until the end if the elements are of the first list
    for (uint32 i = 0; i < (32 - firstListInsertPoint - 1); i++) {
        if (((IntegerList*) (cursor))->intNumber != (i + firstListInsertPoint + 1)) {
            return false;
        }
        cursor = cursor->Next();
    }

    return rootList1.Size() == (64 - secondListBeginPoint + 1);

}

bool LinkedListableTest::TestInsertEntireList() {

    //build the two lists
    IntegerList rootList1;
    IntegerList rootList2;

    IntegerList list1Element[32];
    IntegerList list2Element[32];

    rootList1.SetNext(&list1Element[0]);
    rootList2.SetNext(&list2Element[0]);

    BuildLists(list1Element, list2Element, 32);

    rootList1.Insert(&list2Element[0]);

    LinkedListable *cursor = rootList1.Next();
    for (uint32 i = 0; i < 32; i++) {
        if (((IntegerList*) (cursor))->intNumber != (i + 32)) {
            return false;
        }
        cursor = cursor->Next();
    }

    for (uint32 i = 0; i < 32; i++) {
        if (((IntegerList*) (cursor))->intNumber != (i)) {
            return false;
        }
        cursor = cursor->Next();
    }

    return rootList1.Size() == 65;

}

bool LinkedListableTest::TestInsertSortedSorter(uint32 secondListPosition) {

    if (secondListPosition > 31) {
        secondListPosition = 31;
    }

    //build the two lists
    IntegerList rootList1;
    IntegerList rootList2;

    IntegerList list1Element[32];
    IntegerList list2Element[32];

    rootList1.SetNext(&list1Element[0]);
    rootList2.SetNext(&list2Element[0]);

    list1Element[0].intNumber = 31;
    list2Element[0].intNumber = 32;

    for (uint32 i = 1; i < 32; i++) {
        //decrescent 31-->0
        list1Element[i].intNumber = 31 - i;

        //crescent 32-->63
        list2Element[i].intNumber = 32 + i;

        list1Element[i - 1].SetNext(&list1Element[i]);
        list2Element[i - 1].SetNext(&list2Element[i]);
    }

    SortDecrescent decrescentSorter;

    rootList1.Insert(&list2Element[secondListPosition], &decrescentSorter);

    uint32 nElements = 64 - secondListPosition;
    LinkedListable *cursor = rootList1.Next();
    for (uint32 i = 0; i < (nElements - 1); i++) {
        if ((((IntegerList*) (cursor->Next()))->intNumber) > (((IntegerList*) (cursor))->intNumber)) {
            return false;
        }
        cursor = cursor->Next();
    }

    return rootList1.Size() == (nElements + 1);
}

bool LinkedListableTest::TestInsertNullListInput() {
    //build the two lists
    IntegerList rootList1;

    IntegerList list1Element[32];

    rootList1.SetNext(&list1Element[0]);

    list1Element[0].intNumber = 0;

    for (uint32 i = 1; i < 32; i++) {
        list1Element[i].intNumber = i;

        list1Element[i - 1].SetNext(&list1Element[i]);
    }
    //root2 is Null, never should happen!
    rootList1.Insert((LinkedListable*) NULL);

    LinkedListable *cursor = rootList1.Next();
    for (uint32 i = 0; i < 32; i++) {
        if (((IntegerList*) (cursor))->intNumber != i) {
            return false;
        }
        cursor = cursor->Next();
    }

    return rootList1.Size() == 33;
}

bool LinkedListableTest::TestInsertNullSorterInput() {
    //build the two lists
    IntegerList rootList1;
    IntegerList rootList2;

    IntegerList list1Element[32];
    IntegerList list2Element[32];

    rootList1.SetNext(&list1Element[0]);
    rootList2.SetNext(&list2Element[0]);

    BuildLists(list1Element, list2Element, 32);

    //in this case it calls the default insert
    rootList1.Insert(&list2Element[0], (SortFilter*) NULL);

    LinkedListable *cursor = &rootList1;
    for (uint32 i = 0; i < 32; i++) {
        if (((IntegerList*) (cursor->Next()))->intNumber != (i + 32)) {
            return false;
        }
        cursor = cursor->Next();
    }

    for (uint32 i = 0; i < 32; i++) {
        if (((IntegerList*) (cursor->Next()))->intNumber != (i)) {
            return false;
        }
        cursor = cursor->Next();
    }

    return rootList1.Size() == 65;
}

bool LinkedListableTest::TestAdd(uint32 secondListPosition) {

    if (secondListPosition > 31) {
        secondListPosition = 31;
    }
    //build the two lists
    IntegerList rootList1;
    IntegerList rootList2;

    IntegerList list1Element[32];
    IntegerList list2Element[32];

    rootList1.SetNext(&list1Element[0]);
    rootList2.SetNext(&list2Element[0]);

    BuildLists(list1Element, list2Element, 32);

    rootList1.Add(&list2Element[secondListPosition]);

    IntegerList* lastElement = ((IntegerList*) list1Element[31].Next());
    if (lastElement->intNumber != (32 + secondListPosition)) {
        return false;
    }

    return lastElement->Next() == NULL;

}

bool LinkedListableTest::TestAddL(uint32 secondListPosition) {
    if (secondListPosition > 31) {
        secondListPosition = 31;
    }
    //build the two lists
    IntegerList rootList1;
    IntegerList rootList2;

    IntegerList list1Element[32];
    IntegerList list2Element[32];

    rootList1.SetNext(&list1Element[0]);
    rootList2.SetNext(&list2Element[0]);

    BuildLists(list1Element, list2Element, 32);

    rootList1.AddL(&list2Element[secondListPosition]);

    IntegerList* lastElement = ((IntegerList*) list1Element[31].Next());

    for (uint32 i = 0; i < (32 - secondListPosition); i++) {
        if (lastElement->intNumber != (32 + secondListPosition + i)) {
            return false;
        }
        lastElement = (IntegerList*) (lastElement->Next());
    }

    return lastElement == NULL;

}

bool LinkedListableTest::TestSearch() {
    //build the two lists
    LinkedListable rootList1;

    LinkedListable list1Element[32];

    rootList1.SetNext(&list1Element[0]);

    for (uint32 i = 1; i < 31; i++) {
        list1Element[i - 1].SetNext(&list1Element[i]);
    }

    for (uint32 i = 0; i < 31; i++) {
        if (!rootList1.Search(&list1Element[i])) {
            return false;
        }
    }

    return (!rootList1.Search(&list1Element[31])) && (!list1Element[2].Search(&list1Element[1]));
}

bool LinkedListableTest::TestSearchFilter() {
    //build the two lists
    IntegerList rootList1;
    rootList1.intNumber = 255;

    IntegerList list1Element[32];

    list1Element[0].intNumber = 0;
    rootList1.SetNext(&list1Element[0]);

    for (uint32 i = 1; i < 32; i++) {
        list1Element[i].intNumber = i;
        list1Element[i - 1].SetNext(&list1Element[i]);
    }

    for (uint32 i = 0; i < 32; i++) {
        SearchInteger searchNumber(i);
        if (rootList1.Search(&searchNumber) != &list1Element[i]) {
            return false;
        }
    }

    SearchInteger searchNotIn(33);
    return (rootList1.Search(&searchNotIn) == NULL) && (rootList1.Search((SearchFilter*) NULL) == NULL);
}

bool LinkedListableTest::TestSearchFilterPreviousElement() {
    //build the two lists
    IntegerList rootList1;

    IntegerList list1Element[2];

    list1Element[0].intNumber = 1;
    list1Element[1].intNumber = 2;

    rootList1.SetNext(&list1Element[0]);
    list1Element[0].SetNext(&list1Element[1]);

    SearchInteger searchNumber(1);
    return (rootList1.Search(&searchNumber) == &list1Element[0]) && (list1Element[1].Search(&searchNumber) == NULL);
}

bool LinkedListableTest::TestExtract() {
    //build the two lists
    LinkedListable rootList1;

    LinkedListable list1Element[32];

    rootList1.SetNext(&list1Element[0]);

    for (uint32 i = 1; i < 31; i++) {
        list1Element[i - 1].SetNext(&list1Element[i]);
    }

    if (list1Element[2].Extract(&list1Element[1])) {
        return false;
    }

    if (rootList1.Extract(&list1Element[31])) {
        return false;
    }

    for (uint32 i = 0; i < 31; i++) {
        if (!rootList1.Extract(&list1Element[i])) {
            return false;
        }

        if (list1Element[i].Next() != NULL) {
            return false;
        }

        if (rootList1.Size() != (31 - i)) {
            return false;
        }
    }

    return true;
}

bool LinkedListableTest::TestExtractSearchFilter() {

    //build the two lists
    IntegerList rootList1;
    rootList1.intNumber = 255;

    IntegerList list1Element[32];

    list1Element[0].intNumber = 0;
    rootList1.SetNext(&list1Element[0]);

    for (uint32 i = 1; i < 32; i++) {
        list1Element[i].intNumber = i;
        list1Element[i - 1].SetNext(&list1Element[i]);
    }

    SearchInteger searchNotIn(33);
    if (rootList1.Extract(&searchNotIn) != NULL) {
        return false;
    }

    if (rootList1.Extract((SearchFilter*) NULL) != NULL) {
        return false;
    }

    for (uint32 i = 0; i < 32; i++) {
        SearchInteger searchNumber(i);
        if (rootList1.Extract(&searchNumber) != &list1Element[i]) {
            return false;
        }

        if (list1Element[i].Next() != NULL) {
            return false;
        }

        if (rootList1.Extract(&searchNumber) != NULL) {
            return false;
        }

        if (rootList1.Size() != (32 - i)) {
            return false;
        }
    }

    return true;

}

bool LinkedListableTest::TestDelete() {

    LinkedListable *root = new LinkedListable;
    LinkedListable *cursor = root;

    for (uint32 i = 0; i < 32; i++) {
        LinkedListable *list1Element = new LinkedListable;
        cursor->SetNext(list1Element);
        cursor = cursor->Next();
    }

    LinkedListable* notIn = new LinkedListable;
    if (root->Delete(notIn)) {
        return false;
    }

    cursor = root->Next();
    for (uint32 i = 0; i < 32; i++) {
        if (!root->Delete(cursor)) {
            return false;
        }
        if (root->Size() != (32 - i)) {
            return false;
        }
        cursor = root->Next();
    }

    return true;

}

bool LinkedListableTest::TestDeleteSearchFilter() {

    IntegerList *root = new IntegerList;

    IntegerList *cursor = root;

    for (uint32 i = 0; i < 32; i++) {
        IntegerList *listElement = new IntegerList;
        listElement->intNumber = (i % 16);
        cursor->SetNext(listElement);
        cursor = (IntegerList*) cursor->Next();
    }

    SearchInteger searchNotIn(33);
    if (root->Delete(&searchNotIn) != 0) {
        return false;
    }

    if (root->Delete((SearchFilter*) NULL) != 0) {
        return false;
    }

    for (uint32 i = 0; i < 16; i++) {
        SearchInteger searchNumber(i % 16);
        if (root->Delete(&searchNumber) != 2) {
            return false;
        }

        if (root->Delete(&searchNumber) != 0) {
            return false;
        }

        if (root->Size() != (33 - 2 * (i + 1))) {
            return false;
        }
    }

    return true;

}

bool LinkedListableTest::TestPeek() {

    LinkedListable root;
    LinkedListable element[32];

    root.SetNext(&element[0]);

    for (uint32 i = 1; i < 32; i++) {
        element[i - 1].SetNext(&element[i]);
    }

    for (uint32 i = 1; i <= 32; i++) {
        if (root.Peek(i) != &element[i - 1]) {
            return false;
        }
    }

    return (root.Size() == 33) && (root.Peek(0) == &root) && (root.Peek(33) == NULL);
}

bool LinkedListableTest::TestIterateIterator() {
    IntegerList root;
    IntegerList element[32];

    element[0].intNumber = 0;

    root.SetNext(&element[0]);

    for (uint32 i = 1; i < 32; i++) {
        element[i].intNumber = i;
        element[i - 1].SetNext(&element[i]);
    }

    root.Iterate((Iterator*) NULL);

    LinkedListable *cursor = root.Next();

    for (uint32 i = 0; i < 32; i++) {
        if (((IntegerList*) cursor)->intNumber != i) {
            return false;
        }

        cursor = cursor->Next();
    }

    IncrementIterator addOne;

    root.Iterate(&addOne);

    cursor = root.Next();
    for (uint32 i = 0; i < 32; i++) {
        if (((IntegerList*) cursor)->intNumber != (i + 1)) {
            return false;
        }

        cursor = cursor->Next();
    }

    return root.Size() == 33;

}
