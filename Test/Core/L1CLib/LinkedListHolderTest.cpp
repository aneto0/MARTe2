/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id:  $
 *
 **/

#include "LinkedListHolderTest.h"

bool LinkedListHolderTest::TestInsertAndSorting() {

    IntegerList myRoot;
    IntegerList* current = &myRoot;
    ;
    IntegerList* another;

    //Create three lists.
    LinkedListHolder* listOne = new LinkedListHolder();
    LinkedListHolder* listTwo = new LinkedListHolder();
    LinkedListHolder* listThree = new LinkedListHolder();
    uint32 lSizeOne;
    uint32 lSizeTwo;
    uint32 lSizeThree;

    //Check if the inizial sizes are zero.
    if ((lSizeOne = listOne->ListSize()) != 0
            || (lSizeTwo = listTwo->ListSize()) != 0
            || (lSizeThree = listThree->ListSize()) != 0) {
        return False;
    }

    //Insert integers in lists in different ways. In the list1 they are added in the queue, in the list2 and list3 on the top.
    for (uint32 i = 0; i < 11; i++) {
        IntegerList* myInt1 = new IntegerList();
        myInt1->intNumber = i;
        IntegerList* myInt2 = new IntegerList();
        myInt2->intNumber = i;
        IntegerList* myInt3 = new IntegerList();
        myInt3->intNumber = i;
        myRoot.Insert(myInt2);
        //Insert an element at the end (the position is > of the size).
        listOne->ListInsert((myInt1), 12);
        //Insert only an element a time.
        listThree->FastListInsertSingle(*myInt3);
    }
    //Insert the complete linklistable in a step.
    listTwo->ListInsert(myRoot.Next());

    //Check if the size is correct.
    lSizeOne = listOne->ListSize();
    lSizeTwo = listTwo->ListSize();
    lSizeThree = listThree->ListSize();

    if (lSizeOne != 11 || lSizeTwo != 11 || lSizeThree != 11) {
        delete listOne;
        delete listTwo;
        delete listThree;
        return False;
    }

    //Check if all elements are consistent in the list.
    for (uint32 i = 0; i < lSizeOne; i++) {
        current = (IntegerList*) (listOne->ListPeek(i));
        if (current->intNumber != i) {
            delete listOne;
            delete listTwo;
            delete listThree;
            return False;
        }
        current = (IntegerList*) (listTwo->ListPeek(lSizeOne - i - 1));
        if (current->intNumber != i) {
            delete listOne;
            delete listTwo;
            delete listThree;
            return False;
        }
        current = (IntegerList*) (listThree->ListPeek(lSizeOne - i - 1));
        if (current->intNumber != i) {
            delete listOne;
            delete listTwo;
            delete listThree;
            return False;
        }
    }

    //Insert the list2 on the top of list1. 
    listOne->ListInsertL(listTwo);
    lSizeOne = listOne->ListSize();

    //Check if the size is correct.
    if (lSizeOne != 2 * lSizeTwo) {
        delete listOne;
        return False;
    }

    //Check if all elements are in the list as we expected by them.
    for (uint32 i = 0; i < lSizeTwo; i++) {
        current = (IntegerList*) (listOne->ListPeek(i));
        another = (IntegerList*) (listOne->ListPeek(lSizeOne - i - 1));
        uint32 tmp = current->intNumber;
        if (tmp != another->intNumber || tmp != (lSizeTwo - i - 1)) {
            delete listOne;
            return False;
        }
    }

    //Insert the number 11 in the position 15. The insert function take a linkedlistable by argument 
    int32 aNumber = 11;
    int32 aPosition = 15;
    IntegerList* newElement = new IntegerList();
    newElement->intNumber = aNumber;

    listOne->ListInsert(newElement, aPosition);

    //Peek the position 15 and check if the returned number is precisely 11.
    current = (IntegerList*) listOne->ListPeek(aPosition);
    lSizeOne++;
    if (current->intNumber != newElement->intNumber
            || lSizeOne != listOne->ListSize()) {
        delete listOne;
        return False;
    }

    //Insert the number 11 in the position 19. The insert function take a linkedlistholder by argument 
    aPosition = 19;
    aNumber = 11;
    newElement = new IntegerList();
    newElement->intNumber = aNumber;
    listTwo = new LinkedListHolder();
    listTwo->ListInsert(newElement);
    listOne->ListInsertL(listTwo, aPosition);

    //Peek the position 19 and check if the returned number is precisely 11.
    current = (IntegerList*) listOne->ListPeek(aPosition);
    lSizeOne++;
    if (current->intNumber != newElement->intNumber
            || lSizeOne != listOne->ListSize()) {
        delete listOne;
        return False;
    }

    SortDecrescent mySorter;
    //Sorts elements in a decrescent order
    listOne->ListBSort(&mySorter);

    //Check if elements are in decrescent order.
    for (uint32 i = 0; i < (lSizeOne - 1); i++) {
        current = (IntegerList*) (listOne->ListPeek(i));
        another = (IntegerList*) (listOne->ListPeek(i + 1));
        if (current == NULL || another == NULL) {
            return False;
            delete listOne;
        }
        if (another->intNumber > current->intNumber) {
            delete listOne;
            return False;
        }
    }

    //Insert the list3 in the list1 holding the decrescent order.
    listOne->ListInsertL(listThree, &mySorter);
    lSizeOne += lSizeThree;

    //check again if all elements are in decrescent order.
    for (uint32 i = 0; i < (lSizeOne - 1); i++) {
        current = (IntegerList*) (listOne->ListPeek(i));
        another = (IntegerList*) (listOne->ListPeek(i + 1));
        if (current == NULL || another == NULL) {
            delete listOne;
            return False;
        }
        if (another->intNumber > current->intNumber) {
            delete listOne;
            return False;
        }
    }

    delete listOne;
    return True;

}

bool LinkedListHolderTest::TestAddRemoveAndSearch() {

    //Create a new listlinkholder.
    IntegerList myRoot;
    IntegerList* current = &myRoot;

    LinkedListHolder* listOne = new LinkedListHolder();
    uint32 initialSize = 10;

    //Add elements on by one on the queue of the listlinkable and of the linklistholder. 
    for (uint32 i = 1; i < (initialSize + 1); i++) {
        IntegerList* myInt1 = new IntegerList();
        IntegerList* myInt2 = new IntegerList();
        myInt1->intNumber = initialSize + i;
        myInt2->intNumber = i;
        listOne->ListAdd(myInt2);
        myRoot.Add(myInt1);
    }

    //Check if the dimension is correct
    uint32 lSizeOne = listOne->ListSize();

    if (lSizeOne != initialSize) {
        delete listOne;
        return False;
    }

    //Test the add of a list. The linkedlistable is added on the queue of the linklistholder list1. 
    listOne->ListAddL(myRoot.Next());

    //Check again if the dimension is correct
    lSizeOne = listOne->ListSize();
    if (lSizeOne != (2 * initialSize)) {
        delete listOne;
        return False;
    }

    //Check if all elements are in crescent order. This means that the function ListAddL works correctly.
    for (uint32 i = 0; i < lSizeOne; i++) {
        current = (IntegerList*) (listOne->ListPeek(i));
        if (current->intNumber != (i + 1)) {
            delete listOne;
            return False;
        }
    }

    //Peek the fifth element from the list and test the function search which should return true.
    uint32 aNumber = 5;
    current = (IntegerList*) listOne->ListPeek(aNumber);
    if (!listOne->ListSearch(current)) {
        delete listOne;
        return False;
    }

    //Create a new element without inserting it in the list and test the search function on this element. It should return false.
    current = new IntegerList();
    current->intNumber = aNumber;
    if (listOne->ListSearch(current)) {
        delete current;
        delete listOne;
        return False;
    }

    delete current;

    //Initialize a searcher for the integer 5.
    aNumber = 5;
    SearchInteger mySearcher(aNumber);

    //Extract the integer 5 from the list. This tests the extract function with a sorter passed by argument.
    current = (IntegerList*) (listOne->ListExtract(&mySearcher));

    //Since there are not other 5 in the list, the search must return NULL.
    if (listOne->ListSearch(&mySearcher) != NULL) {
        delete current;
        delete listOne;
        return False;
    }

    delete current;

    //Test the extract function with the position in the list passed by argument. The integer 4 should be 
    //in the position 3 of the list (position begin from 0)
    mySearcher.ChangeSearchNumber(aNumber - 1);
    current = (IntegerList*) (listOne->ListExtract(aNumber - 2));

    if (listOne->ListSearch(&mySearcher) != NULL) {
        delete current;
        delete listOne;
        return False;
    }

    delete current;

    //Test the delete function with the pointer of the element passed by argument. Delete the integer 10 from the list.
    aNumber = 10;
    mySearcher.ChangeSearchNumber(aNumber);
    current = (IntegerList*) (listOne->ListSearch(&mySearcher));

    if (current == NULL) {
        delete listOne;
        return False;
    }

    listOne->ListDelete(current);
    if (listOne->ListSearch(&mySearcher) != NULL) {
        delete listOne;
        return False;
    }

    //Test the delete function with a searcher in the argument. Delete all integers greater than aNumber.
    aNumber = 15;
    SearchGreatInteger myDeleterSearcher(aNumber);

    listOne->ListDelete(&myDeleterSearcher);
    lSizeOne = listOne->ListSize();

    //Check that all integers int the list are minor or equal to aNumber.
    for (uint32 i = 0; i < lSizeOne; i++) {
        current = (IntegerList*) (listOne->ListPeek(i));
        if ((current->intNumber) > aNumber) {
            delete listOne;
            return False;
        }
    }

    //Test the ListSafeDelete function deleting all integers greater than aNumber from the list.
    aNumber = 9;
    myDeleterSearcher.ChangeSearchNumber(aNumber);

    listOne->ListSafeDelete(&myDeleterSearcher);

    lSizeOne = listOne->ListSize();
    for (uint32 i = 0; i < lSizeOne; i++) {
        current = (IntegerList*) (listOne->ListPeek(i));
        if ((current->intNumber) > aNumber) {
            delete listOne;
            return False;
        }
    }

    //Test the Iterate function with an iterator that sums aNumber to each element of the list.
    SumIterator mySumIterator(aNumber);

    listOne->ListIterate(&mySumIterator);

    //Test that all elements are greater or equal to aNumber.
    for (uint32 i = 0; i < lSizeOne; i++) {
        current = (IntegerList*) (listOne->ListPeek(i));
        if ((current->intNumber) < aNumber) {
            delete listOne;
            return False;
        }
    }

    delete listOne;
    return True;
}

bool LinkedListHolderTest::TestNULLConditions() {
    LinkedListHolder* listOne = new LinkedListHolder;

    if (listOne->ListSize() != 0) {
        return False;
    }

    //Test Add and Insert functions with NULL as argument.
    listOne->ListInsert(NULL);
    listOne->ListInsertL(NULL);
    listOne->ListAdd(NULL);
    listOne->ListAddL(NULL);
    listOne->ListInsertL(NULL, (SortFilter *) NULL);
    listOne->ListInsertL(NULL, (SortFilterFn *) NULL);

    //Check if the size remains zero.
    if (listOne->ListSize() != 0) {
        return False;
    }

    //Add ten elements to the List.
    for (uint32 i = 0; i < 10; i++) {
        IntegerList* myInt = new IntegerList();
        myInt->intNumber = i;
        listOne->ListAdd(myInt);
    }

    //Test Extract functions with index out of bounds and with NULL as argument.
    if (listOne->ListExtract(11) != NULL) {
        delete listOne;
        return False;
    }

    if (listOne->ListExtract((LinkedListable*) NULL)) {
        return False;
    }

    if (listOne->ListExtract((SearchFilter*) NULL) != NULL) {
        delete listOne;
        return False;
    }

    //Test Peek function with index out of bounds.
    if (listOne->ListPeek(11) != NULL) {
        delete listOne;
        return False;
    }

    //Test Delete functions with NULL as argument.
    if (listOne->ListDelete((LinkedListable*) NULL)) {
        delete listOne;
        return False;
    }

    if (listOne->ListDelete((SearchFilter*) NULL)) {
        return False;
    }

    if (listOne->ListSafeDelete(NULL)) {
        delete listOne;
        return False;
    }

    //Test Search function with NULL as argument.
    if (listOne->ListSearch((SearchFilter*) NULL) != NULL) {
        delete listOne;
        return False;
    }

    //Check if the dimension of the list remains 10.
    if (listOne->ListSize() != 10) {
        return False;
    }

    delete listOne;
    return True;

}
