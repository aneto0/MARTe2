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

#include "StackHolderTest.h"

//Define the sorting rule.
int32 SortStackDecrescent(LinkedListable* data1, LinkedListable* data2) {
    IntegerLinkedList* number1 = (IntegerLinkedList*) data1;
    IntegerLinkedList* number2 = (IntegerLinkedList*) data2;

    if (number1->intNumber < number2->intNumber)
        return 1;
    else
        return -1;
}

//Define the searching rule.
bool SearchGreaterThan5(LinkedListable* data) {
    IntegerLinkedList* number = (IntegerLinkedList*) data;

    return number->intNumber > 5;
}


bool StackHolderTest::TestAll() {
    StackHolder *myStack = new StackHolder();

    //Push in the stack a single element.
    IntegerLinkedList* myNewInt = new IntegerLinkedList();
    myNewInt->intNumber = 10;
    myStack->StackFastPushSingle(myNewInt);

    IntegerLinkedList* myRoot = new IntegerLinkedList();
    myRoot->intNumber = 0;

    //Push in the stack a list.
    for (int32 i = 1; i < 10; i++) {
        IntegerLinkedList* myInt;
        myInt = new IntegerLinkedList();
        myInt->intNumber = i;
        myRoot->Add(myInt);
    }

    myStack->StackPush(myRoot);
	

    //If the push works correctly the elements must be extracted in a crescent order.
    if (myStack->StackDepth() != 11) {
	delete myStack;
        return False;
    }

    for (int32 i = 0; i < 11; i++) {
        int32 num = ((IntegerLinkedList*) (myStack->StackPeek(i)))->intNumber;
        if (num != i) {
	    delete myStack;
            return False;
        }
    }

    //Pop an element from the stack and check if it's removed.
    IntegerLinkedList* top = (IntegerLinkedList*) (myStack->StackPop());
    if (top->intNumber != 0 || myStack->StackDepth() != 10) {
        delete top;
	delete myStack;
        return False;
    }

    delete top;

    //Get the top element of the stack and check if it's correct.
    IntegerLinkedList* newTop = (IntegerLinkedList*) (myStack->StackTop());
    if (newTop->intNumber != 1 || myStack->StackDepth() != 10) {
	delete myStack;
        return False;
    }

    //Extract a number from its position and check if it's the expected one.
    uint32 aPosition=5;
    IntegerLinkedList* fifthElement = (IntegerLinkedList*) (myStack->StackExtract(
            aPosition));
    if (fifthElement->intNumber != (aPosition+1) || myStack->StackDepth() != 9) {
	delete myStack;
        return False;
    }

    //Insert newly the element in the list at the right position and check that the stack is consistent.
    myStack->StackInsert(fifthElement, aPosition);

    for (int32 i = 1; i < 11; i++) {
        int32 num = ((IntegerLinkedList*) (myStack->StackPeek(i - 1)))->intNumber;
        if (num != i) {
	    delete myStack;
            return False;
        }
    }

    if (myStack->StackDepth() != 10) {
        delete myStack;
        return False;
    }

    //Sort the stack using a sorterFn sorter.
    myStack->ListBSort(SortStackDecrescent);

    for (int32 i = 1; i < 11; i++) {
        int32 num = ((IntegerLinkedList*) (myStack->StackPeek(i - 1)))->intNumber;
        if (num != 11 - i) {
	    delete myStack;
            return False;
        }
    }

    //Add a list of two elements in crescent order to the stack in a decrescent order.
    IntegerLinkedList* newSix = new IntegerLinkedList();
    IntegerLinkedList* newFour = new IntegerLinkedList();
    newSix->intNumber = 6;
    newFour->intNumber = 4;
    newFour->Insert(newSix);
    myStack->ListInsert(newFour, SortStackDecrescent);

    //Tests if all elements are in a decrescent order.
    for (uint32 i = 0; i < (myStack->StackDepth() - 1); i++) {
        uint32 num1 = ((IntegerLinkedList*) (myStack->StackPeek(i)))->intNumber;
        uint32 num2 =
                ((IntegerLinkedList*) (myStack->StackPeek(i + 1)))->intNumber;
        if (num1 < num2) {
    	    delete myStack;
            return False;
        }
    }

    //Delete all elements greater than 5 using a SearchFn searcher.
    myStack->ListDelete(SearchGreaterThan5);

    //Check if all elements greater than 5 were removed from the stack.
    for (uint32 i = 0; i < myStack->StackDepth(); i++) {
        uint32 num = ((IntegerLinkedList*) (myStack->StackPeek(i)))->intNumber;
        if (num > 5) {
            delete myStack;
            return False;
        }
    }
    delete myStack;
    return True;

}



bool StackHolderTest::TestNULLConditions() {
    StackHolder* myStack = new StackHolder();

    if (myStack->StackDepth() != 0) {
        return False;
    }

    //Test Add and Insert functions with NULL as argument.
    myStack->StackPush(NULL);
    myStack->StackPop();
    myStack->StackInsert(NULL,10);

    //Check if the size remains zero.
    if (myStack->StackDepth() != 0) {
        return False;
    }

    //Add ten elements to the queue.
    for (uint32 i = 0; i < 10; i++) {
        IntegerLinkedList* myInt = new IntegerLinkedList();
	if(myInt==NULL){
		return False;
	}
        myInt->intNumber = i;
        myStack->StackFastPushSingle(myInt);
    }

    //Test Extract functions with index out of bounds and with NULL as argument.
    if (myStack->StackExtract(11)!=NULL) {
        delete myStack;
        return False;
    }

    //Test Peek function with index out of bounds.
    if (myStack->StackPeek(11) != NULL) {
        delete myStack;
        return False;
    }

    //Check if the dimension of the list remains 10.
    if (myStack->StackDepth() != 10) {
        return False;
    }

    delete myStack;
    return True;

}
