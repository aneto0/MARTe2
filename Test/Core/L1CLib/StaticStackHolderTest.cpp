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
 *
 **/
#include "stdio.h"
#include "StaticStackHolderTest.h"
#include "Threads.h"
#include "Sleep.h"

void changeA(int32 *el) {
    int32 b = 2;
    int32 *bp = &b;
    printf("\nel[1] %d\n", el[0]);
    printf("\nel %p\n", el);
    el[0] = bp[0];
}

bool StaticStackHolderTest::StackTest() {
    int32 i = 0;
    successful = true;

    //Fulfill the List
    i = 0;
    while (i < 4) {
        ssh.StackPush(elementsPtr[i]);
        i++;
    }

    // Check that the size of the list as expected
    successful &= (ssh.StackDepth() == 4);

    //read the table from the beginning to the end
    i = 0;
    while (i < 4) {
        ssh.StackPeek(returnElementPtr, i);
        // check that the returned values are as expected
        successful &= returnElement == elements[4 - 1 - i];
        i++;
    }

    // check that with with a incorrect index it returns false
    successful &= !ssh.StackPeek(returnElementPtr, 10);

    //the returned value should be 0
    successful &= 0 == returnElement;

    //Checks StackTop returns true
    successful &= ssh.StackTop(returnElementPtr);

    //check that the return value is the top value
    successful &= (returnElement == elements[3]);

    //Checks StackPop (extracts from the top returns true
    successful &= ssh.StackPop(returnElementPtr);

    //check that the extracted value is the top value
    successful &= (returnElement == elements[3]);

    //check that the size of the table is reduced
    successful &= (3 == ssh.StackDepth());

    //

    return successful;
}

