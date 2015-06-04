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
#include "LinkedListableTest.h"


bool LinkedListableTest::TestInsertAndSorting() {
    //Create the first list. 
    IntegerList myRoot;
    myRoot.intNumber=0;
    IntegerList* current;
    current = &myRoot;
    IntegerList myInt[11];
    //Insert integers in a crescent order.
    for (uint32 i = 1; i < 11; i++) {
        myInt[i].intNumber = i;
        current->Insert(&(myInt[i]));
        current = (IntegerList*) (current->Next());
    }
    //Check if the size is correct.
    uint32 listLength = myRoot.Size();
    if (listLength != 11) {
        return False;
    }
    current = &myRoot;
    //Check if all elements are consistent in the list.
    for (uint32 i = 1; i < listLength; i++) {
        current = (IntegerList*) (current->Next());
        if (current->intNumber != (myRoot.intNumber + i)) {
            return False;
        }
    }

   
    SortDecrescent mySorter;
    //Sorts elements in a decrescent order
    myRoot.BSort(&mySorter);

    //Check if the dimension remain the same after sorting minus one (the root is not sorted).
    current=(IntegerList*) (myRoot.Next());
    IntegerList myNewRoot = (*current);
    listLength--;
    if (myNewRoot.Size() != listLength) {
        return False;
    }

    //Check if elements are in decrescent order (the root is not sorted).
    for (uint32 i = 1; i < (listLength - 1); i++) {
        current = (IntegerList*) (current->Next());
        if (current == NULL)
            return False;

        if (current->intNumber != (myNewRoot.intNumber - i)) {
            return False;
        }
    }

    //Create another list.
    IntegerList myInt2[5];
    current = myInt2;
    myInt2[0].intNumber = 0;
    for (int32 i = 1; i < 5; i++) {
        myInt2[i].intNumber = 2 * i;
        current->Insert(&(myInt2[i]));
        current = (IntegerList*) (current->Next());
    }
    current = myInt2;

    //Insert the second in the first list imposing a decrescent order.
    myNewRoot.Insert(current, &mySorter);
    current = &myNewRoot;

    //Check if the dimension of the resulted list is correct.
    listLength = myNewRoot.Size();
    if (listLength != 15) {
        return False;
    }

    //Check if all elements are sorted in decrescent order.
    for (uint32 i = 0; i < (listLength - 1); i++) {
        IntegerList* current_1 = current;
        current = (IntegerList*) (current->Next());
        if (current == NULL) {
            return False;
        }
        if ((current->intNumber) > (current_1->intNumber)) {
            return False;
        }
    }
    return True;

}

bool LinkedListableTest::TestAddAndRemove() {

    //Create a new list.
    IntegerList myRoot;
    myRoot.intNumber=0;
    IntegerList* current;
    current = &myRoot;
    IntegerList myInt[10];

    //Add elements to the queue of the list.
    for (uint32 i = 1; i < 10; i++) {
        myInt[i].intNumber = i;
        myRoot.Add(&(myInt[i]));
    }
     

    //Check if the dimension is correct
    uint32 listLength = myRoot.Size();
    if (listLength != 10) {
        return False;
    }
    current = &myRoot;


    //Check if elements are in crescent order, namely were added on the queue.
    for (uint32 i = 1; i < listLength; i++) {
        current = (IntegerList*) (current->Next());
        if (current->intNumber != (myRoot.intNumber + i)) {
            return False;
        }
    }

    //Initialize a searcher for the integer 5 and extract it from the list.
    SearchInteger mySearcher(5);
    if (((IntegerList*) (myRoot.Extract(&mySearcher)))->intNumber != 5) {
        return False;
    }

    //Try the boolean extract 
    if (myRoot.Extract(&mySearcher)) {
        return False;
    }

    listLength--;

    //Check if the size is correct
    if (myRoot.Size() != listLength) {
        return False;
    }

    //Create a new element to insert
    IntegerList newFive;
    newFive.intNumber = 5;

    //Use searcher to search the integer 4 due to re-add 5 
    mySearcher.ChangeSearchNumber(4);
    current = (IntegerList*) (myRoot.Search(&mySearcher));
    current->Insert(&newFive);

    //Check if the dimension is correct
    if (myRoot.Size() != (listLength + 1)) {
        return False;
    }

    //Check if 5 is in the correct position in the list.
    if (current->Next() != myRoot.Peek(5)) {
        return False;
    }
    return True;
}

