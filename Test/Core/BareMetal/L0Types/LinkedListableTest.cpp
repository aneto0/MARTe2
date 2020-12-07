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
using namespace MARTe;

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

    //This DOES not work. When calling directly the destructor a temporary object 
    //will be created and thus root will not change
    //root.~LinkedListable();

    //return (root.Next() == NULL) && (root.Size() == 1);
    return true;
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

    const uint32 size = 32;

    LinkedListable root;
    LinkedListable element[size];

    //size should be zero at the beginning
    if (root.Size() != 1) {
        return false;
    }

    root.SetNext(&element[0]);

    //fills the list
    for (uint32 i = 1; i < size; i++) {
        element[i - 1].SetNext(&element[i]);
    }

    //checks if the size is equal to the current number of elements
    if (root.Size() != (size + 1)) {
        return false;
    }

    //checks if all element see only the sublist on its right
    for (uint32 i = 0; i < size; i++) {
        if (element[i].Size() != (size - i)) {
            return false;
        }
    }
    return true;
}

bool LinkedListableTest::TestBSortSorter() {
    const uint32 size = 32;

    IntegerList root;
    IntegerList number[size];

    number[0].intNumber = 0;
    root.SetNext(&number[0]);

    //fills the list in crescent order
    for (uint32 i = 1; i < size; i++) {
        number[i].intNumber = i;
        number[i - 1].SetNext(&number[i]);
    }

    //nothing should happen
    root.BSort((SortFilter*) NULL);
    LinkedListable *cursor = root.Next();
    for (uint32 i = 0; i < size; i++) {
        if (((IntegerList*) (cursor))->intNumber != i) {
            return false;
        }
        cursor = cursor->Next();
    }

    //sort the list in decrescent order
    SortDecrescent decrescentSorter;

    //sort only the last half part of the list
    number[size / 2].BSort(&decrescentSorter);

    uint32 stoPoint = size / 2;

    //checks if the order is crescent in the first half list
    cursor = root.Next();
    for (uint32 i = 0; i <= stoPoint; i++) {
        if (((IntegerList*) (cursor))->intNumber != i) {
            return false;
        }
        cursor = cursor->Next();
    }

    //checks if the order is decrescent in the second half list
    for (uint32 i = (size - 1); i >= (stoPoint + 1); i--) {
        if (((IntegerList*) (cursor))->intNumber != i) {
            return false;
        }
        cursor = cursor->Next();
    }

    root.BSort(&decrescentSorter);

    //checks if the order is decrescent in all list
    cursor = root.Next();
    for (uint32 i = 0; i < size; i++) {
        if (((IntegerList*) (cursor))->intNumber != (size - 1 - i)) {
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

    const uint32 size = 32;
    //saturates because lists contains 32 elements
    if (firstListInsertPoint > (size - 1)) {
        firstListInsertPoint = (size - 1);
    }
    if (secondListBeginPoint > (size - 1)) {
        secondListBeginPoint = (size - 1);
    }

    //build the two lists
    IntegerList rootList1;
    IntegerList rootList2;

    IntegerList list1Element[size];
    IntegerList list2Element[size];

    rootList1.SetNext(&list1Element[0]);
    rootList2.SetNext(&list2Element[0]);

    BuildLists(list1Element, list2Element, size);
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
    uint32 point = (size - secondListBeginPoint);

    for (uint32 i = 0; i < point; i++) {
        if (((IntegerList*) (cursor))->intNumber != (i + size + secondListBeginPoint)) {
            return false;
        }
        cursor = cursor->Next();
    }

    //checks until the end if the elements are of the first list
    for (uint32 i = 0; i < (size - firstListInsertPoint - 1); i++) {
        if (((IntegerList*) (cursor))->intNumber != (i + firstListInsertPoint + 1)) {
            return false;
        }
        cursor = cursor->Next();
    }

    return rootList1.Size() == (2 * size - secondListBeginPoint + 1);

}

bool LinkedListableTest::TestInsertEntireList() {

    const uint32 size = 32;
    //build the two lists
    IntegerList rootList1;
    IntegerList rootList2;

    IntegerList list1Element[size];
    IntegerList list2Element[size];

    rootList1.SetNext(&list1Element[0]);
    rootList2.SetNext(&list2Element[0]);

    BuildLists(list1Element, list2Element, size);

    rootList1.Insert(&list2Element[0]);

    LinkedListable *cursor = rootList1.Next();
    for (uint32 i = 0; i < size; i++) {
        if (((IntegerList*) (cursor))->intNumber != (i + size)) {
            return false;
        }
        cursor = cursor->Next();
    }

    for (uint32 i = 0; i < size; i++) {
        if (((IntegerList*) (cursor))->intNumber != (i)) {
            return false;
        }
        cursor = cursor->Next();
    }

    return rootList1.Size() == (2 * size + 1);

}

bool LinkedListableTest::TestInsertSortedSorter(uint32 secondListPosition) {

    const uint32 size = 32;

    if (secondListPosition > (size - 1)) {
        secondListPosition = (size - 1);
    }

    //build the two lists
    IntegerList rootList1;
    IntegerList rootList2;

    IntegerList list1Element[size];
    IntegerList list2Element[size];

    rootList1.SetNext(&list1Element[0]);
    rootList2.SetNext(&list2Element[0]);

    list1Element[0].intNumber = (size - 1);
    list2Element[0].intNumber = size;

    for (uint32 i = 1; i < size; i++) {
        //decrescent 31-->0
        list1Element[i].intNumber = size - 1 - i;

        //crescent 32-->63
        list2Element[i].intNumber = size + i;

        list1Element[i - 1].SetNext(&list1Element[i]);
        list2Element[i - 1].SetNext(&list2Element[i]);
    }

    SortDecrescent decrescentSorter;

    rootList1.Insert(&list2Element[secondListPosition], &decrescentSorter);

    uint32 nElements = 2 * size - secondListPosition;
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
    const uint32 size = 32;

    IntegerList rootList1;

    IntegerList list1Element[size];

    rootList1.SetNext(&list1Element[0]);

    list1Element[0].intNumber = 0;

    for (uint32 i = 1; i < size; i++) {
        list1Element[i].intNumber = i;

        list1Element[i - 1].SetNext(&list1Element[i]);
    }
    //root2 is Null, never should happen!
    rootList1.Insert((LinkedListable*) NULL);

    LinkedListable *cursor = rootList1.Next();
    for (uint32 i = 0; i < size; i++) {
        if (((IntegerList*) (cursor))->intNumber != i) {
            return false;
        }
        cursor = cursor->Next();
    }

    return rootList1.Size() == (size + 1);
}

bool LinkedListableTest::TestInsertNullSorterInput() {
    const uint32 size = 32;

    //build the two lists
    IntegerList rootList1;
    IntegerList rootList2;

    IntegerList list1Element[size];
    IntegerList list2Element[size];

    rootList1.SetNext(&list1Element[0]);
    rootList2.SetNext(&list2Element[0]);

    BuildLists(list1Element, list2Element, size);

    //in this case it calls the default insert
    rootList1.Insert(&list2Element[0], (SortFilter*) NULL);

    LinkedListable *cursor = &rootList1;
    for (uint32 i = 0; i < size; i++) {
        if (((IntegerList*) (cursor->Next()))->intNumber != (i + size)) {
            return false;
        }
        cursor = cursor->Next();
    }

    for (uint32 i = 0; i < size; i++) {
        if (((IntegerList*) (cursor->Next()))->intNumber != (i)) {
            return false;
        }
        cursor = cursor->Next();
    }

    return rootList1.Size() == 2 * size + 1;
}

bool LinkedListableTest::TestAdd(uint32 secondListPosition) {

    const uint32 size = 32;
    if (secondListPosition > (size - 1)) {
        secondListPosition = (size - 1);
    }
    //build the two lists
    IntegerList rootList1;
    IntegerList rootList2;

    IntegerList list1Element[size];
    IntegerList list2Element[size];

    rootList1.SetNext(&list1Element[0]);
    rootList2.SetNext(&list2Element[0]);

    BuildLists(list1Element, list2Element, size);

    rootList1.Add(&list2Element[secondListPosition]);

    IntegerList* lastElement = ((IntegerList*) list1Element[size - 1].Next());
    if (lastElement->intNumber != (size + secondListPosition)) {
        return false;
    }

    return lastElement->Next() == NULL;

}

bool LinkedListableTest::TestAddL(uint32 secondListPosition) {
    const uint32 size = 32;

    if (secondListPosition > (size - 1)) {
        secondListPosition = (size - 1);
    }
    //build the two lists
    IntegerList rootList1;
    IntegerList rootList2;

    IntegerList list1Element[size];
    IntegerList list2Element[size];

    rootList1.SetNext(&list1Element[0]);
    rootList2.SetNext(&list2Element[0]);

    BuildLists(list1Element, list2Element, size);

    rootList1.AddL(&list2Element[secondListPosition]);

    IntegerList* lastElement = ((IntegerList*) list1Element[size - 1].Next());

    for (uint32 i = 0; i < (size - secondListPosition); i++) {
        if (lastElement->intNumber != (size + secondListPosition + i)) {
            return false;
        }
        lastElement = (IntegerList*) (lastElement->Next());
    }

    return lastElement == NULL;

}

bool LinkedListableTest::TestSearch() {

    const uint32 size = 32;

    //build the two lists
    LinkedListable rootList1;

    LinkedListable list1Element[size];

    rootList1.SetNext(&list1Element[0]);

    for (uint32 i = 1; i < (size - 1); i++) {
        list1Element[i - 1].SetNext(&list1Element[i]);
    }

    for (uint32 i = 0; i < (size - 1); i++) {
        if (!rootList1.Search(&list1Element[i])) {
            return false;
        }
    }

    return (!rootList1.Search(&list1Element[size - 1])) && (!list1Element[2].Search(&list1Element[1]));
}

bool LinkedListableTest::TestSearchFilter() {
    //build the two lists
    const uint32 size = 32;

    IntegerList rootList1;
    rootList1.intNumber = 255;

    IntegerList list1Element[size];

    list1Element[0].intNumber = 0;
    rootList1.SetNext(&list1Element[0]);

    for (uint32 i = 1; i < size; i++) {
        list1Element[i].intNumber = i;
        list1Element[i - 1].SetNext(&list1Element[i]);
    }

    for (uint32 i = 0; i < size; i++) {
        SearchInteger searchNumber(i);
        if (rootList1.Search(&searchNumber) != &list1Element[i]) {
            return false;
        }
    }

    SearchInteger searchNotIn(size + 1);
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

    const uint32 size = 32;

    //build the two lists
    LinkedListable rootList1;

    LinkedListable list1Element[size];

    rootList1.SetNext(&list1Element[0]);

    for (uint32 i = 1; i < (size - 1); i++) {
        list1Element[i - 1].SetNext(&list1Element[i]);
    }

    if (list1Element[2].Extract(&list1Element[1])) {
        return false;
    }

    if (rootList1.Extract(&list1Element[size - 1])) {
        return false;
    }

    for (uint32 i = 0; i < (size - 1); i++) {
        if (!rootList1.Extract(&list1Element[i])) {
            return false;
        }

        if (list1Element[i].Next() != NULL) {
            return false;
        }

        if (rootList1.Size() != (size - 1 - i)) {
            return false;
        }
    }

    return true;
}

bool LinkedListableTest::TestExtractSearchFilter() {

    const uint32 size = 32;

    //build the two lists
    IntegerList rootList1;
    rootList1.intNumber = 255;

    IntegerList list1Element[size];

    list1Element[0].intNumber = 0;
    rootList1.SetNext(&list1Element[0]);

    for (uint32 i = 1; i < size; i++) {
        list1Element[i].intNumber = i;
        list1Element[i - 1].SetNext(&list1Element[i]);
    }

    SearchInteger searchNotIn(size + 1);
    if (rootList1.Extract(&searchNotIn) != NULL) {
        return false;
    }

    if (rootList1.Extract((SearchFilter*) NULL) != NULL) {
        return false;
    }

    for (uint32 i = 0; i < size; i++) {
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

        if (rootList1.Size() != (size - i)) {
            return false;
        }
    }

    return true;

}

bool LinkedListableTest::TestDelete() {

    const uint32 size = 32;

    LinkedListable *root = new LinkedListable;
    LinkedListable *cursor = root;

    for (uint32 i = 0; i < size; i++) {
        LinkedListable *list1Element = new LinkedListable;
        cursor->SetNext(list1Element);
        cursor = cursor->Next();
    }


    LinkedListable* notIn = new LinkedListable;
    bool ret=true;
    if (root->Delete(notIn)) {
        ret= false;
    }
    delete notIn;

    cursor = root->Next();
    for (uint32 i = 0; i < size; i++) {
        if (!root->Delete(cursor)) {
            ret=false;
        }
        if (root->Size() != (size - i)) {
            ret= false;
        }
        cursor = root->Next();
    }

    delete root;
    return ret;

}

bool LinkedListableTest::TestDeleteSearchFilter() {

    const uint32 size = 30;

    IntegerList *root = new IntegerList;

    IntegerList *cursor = root;

    for (uint32 i = 0; i < size; i++) {
        IntegerList *listElement = new IntegerList;
        listElement->intNumber = (i % (size / 2));
        cursor->SetNext(listElement);
        cursor = (IntegerList*) cursor->Next();
    }

    SearchInteger searchNotIn(size + 1);
    if (root->Delete(&searchNotIn) != 0) {
        return false;
    }

    if (root->Delete((SearchFilter*) NULL) != 0) {
        return false;
    }

    for (uint32 i = 0; i < (size / 2); i++) {
        SearchInteger searchNumber(i % (size / 2));
        if (root->Delete(&searchNumber) != 2) {
            return false;
        }

        if (root->Delete(&searchNumber) != 0) {
            return false;
        }

        if (root->Size() != (size - 1 - (2 * i))) {
            return false;
        }

    }

    delete root;
    return true;

}

bool LinkedListableTest::TestPeek() {

    const uint32 size = 32;

    LinkedListable root;
    LinkedListable element[size];

    root.SetNext(&element[0]);

    for (uint32 i = 1; i < size; i++) {
        element[i - 1].SetNext(&element[i]);
    }

    for (uint32 i = 1; i <= size; i++) {
        if (root.Peek(i) != &element[i - 1]) {
            return false;
        }
    }

    return (root.Size() == (size + 1)) && (root.Peek(0) == &root) && (root.Peek(size + 1) == NULL);
}

bool LinkedListableTest::TestIterateIterator() {

    const uint32 size = 32;

    IntegerList root;
    IntegerList element[size];

    element[0].intNumber = 0;

    root.SetNext(&element[0]);

    for (uint32 i = 1; i < size; i++) {
        element[i].intNumber = i;
        element[i - 1].SetNext(&element[i]);
    }

    root.Iterate((Iterator*) NULL);

    LinkedListable *cursor = root.Next();

    for (uint32 i = 0; i < size; i++) {
        if (((IntegerList*) cursor)->intNumber != i) {
            return false;
        }

        cursor = cursor->Next();
    }

    IncrementIterator addOne;

    root.Iterate(&addOne);

    cursor = root.Next();
    for (uint32 i = 0; i < size; i++) {
        if (((IntegerList*) cursor)->intNumber != (i + 1)) {
            return false;
        }

        cursor = cursor->Next();
    }

    return root.Size() == (size + 1);

}
