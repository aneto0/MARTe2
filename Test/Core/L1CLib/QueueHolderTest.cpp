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

#include "QueueHolderTest.h"

bool QueueHolderTest::TestAll() {

    //Peek from the new to the old element, Extract at the beginning And add on the queue. Insert on the beginning (as a stack).

    IntegerLinkedListable* myRoot = new IntegerLinkedListable();
    QueueHolder* myQueue = new QueueHolder();

    //Add a List in the queue using insert function.
    for (int32 i = 0; i < 5; i++) {
        IntegerLinkedListable* myInt = new IntegerLinkedListable();
        myInt->intNumber = i;
        myRoot->Add(myInt);
    }
    myQueue->QueueInsert(myRoot->Next());

    //Add a single element in the queue.
    IntegerLinkedListable* myNewInt = new IntegerLinkedListable();
    myNewInt->intNumber = 5;
    myQueue->FastQueueInsertSingle(*myNewInt);

    delete myRoot;
    myRoot = new IntegerLinkedListable();

    //Add a List in the queue using add function.
    for (int32 i = 6; i < 11; i++) {
        IntegerLinkedListable* myInt = new IntegerLinkedListable();
        myInt->intNumber = i;
        myRoot->Add(myInt);
    }

    myQueue->QueueAdd(myRoot->Next());

    //If the operations were done correctly we must peek from the list number in decrescent order.
    if (myQueue->QueueSize() != 11) {
        delete myQueue;
        return False;
    }

    for (int32 i = 0; i < 11; i++) {
        int32 num =
                ((IntegerLinkedListable*) (myQueue->QueuePeek(i)))->intNumber;
        if (num != (10 - i)) {
            delete myQueue;
            return False;
        }
    }

    //Extract the oldest number (FIFO) and check that it's correct.
    IntegerLinkedListable* oldest =
            (IntegerLinkedListable*) (myQueue->QueueExtract());
    if (oldest->intNumber != 0 || myQueue->QueueSize() != 10) {
        delete oldest;
        delete myQueue;
        return False;
    }

    delete oldest;

    //Extract the newest number (FIFO) and check that it's correct.
    IntegerLinkedListable* last =
            (IntegerLinkedListable*) (myQueue->QueuePeekLast());
    if (!myQueue->QueueExtract(last)) {
        delete myQueue;
        return False;
    }
    if (last->intNumber != 10 || myQueue->QueueSize() != 9) {
        delete last;
        delete myQueue;
        return False;
    }

    delete last;

    //Extract the newest and the oldest numbers and check that are correct.
    last = (IntegerLinkedListable*) (myQueue->QueuePeekLast());
    IntegerLinkedListable* newOldest =
            (IntegerLinkedListable*) (myQueue->Oldest());
    if (last->intNumber != 9 || newOldest->intNumber != 1
            || myQueue->QueueSize() != 9) {
        delete myQueue;
        return False;
    }

    //Tests the peek and the extract functions for an element of the list.
    //Since the stack now is 9-8-...-1 the i element has 9-i value (the index begin from 0).
    uint32 aPosition = 5;
    IntegerLinkedListable* fifthElement =
            (IntegerLinkedListable*) (myQueue->QueuePeek(aPosition));
    myQueue->QueueExtract(fifthElement);
    if (fifthElement->intNumber != (9 - aPosition)
            || myQueue->QueueSize() != 8) {
        delete fifthElement;
        delete myQueue;
        return False;
    }

    delete fifthElement;

    delete myQueue;

    return True;

}

bool QueueHolderTest::TestNULLConditions() {
    QueueHolder* myQueue = new QueueHolder();

    if (myQueue->QueueSize() != 0) {
        return False;
    }

    //Test Add and Insert functions with NULL as argument.
    myQueue->QueueInsert(NULL);
    myQueue->QueueAdd(NULL);
    myQueue->QueueExtract();

    //Check if the size remains zero.
    if (myQueue->QueueSize() != 0) {
        return False;
    }

    //Add ten elements to the queue.
    for (uint32 i = 0; i < 10; i++) {
        IntegerLinkedListable* myInt = new IntegerLinkedListable();
        if (myInt == NULL) {
            return False;
        }
        myInt->intNumber = i;
        myQueue->FastQueueInsertSingle(*myInt);
    }

    //Test Extract functions with index out of bounds and with NULL as argument.
    if (myQueue->QueueExtract(NULL)) {
        delete myQueue;
        return False;
    }

    //Test Peek function with index out of bounds.
    if (myQueue->QueuePeek(11) != NULL) {
        delete myQueue;
        return False;
    }

    //Check if the dimension of the list remains 10.
    if (myQueue->QueueSize() != 10) {
        return False;
    }

    delete myQueue;
    return True;

}
