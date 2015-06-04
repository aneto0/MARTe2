/*
 * StaticQueueHolderTest.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: shareuser
 */

#include "StaticQueueHolderTest.h"
#include "stdio.h"

bool StaticQueueHolderTest::QueueAddTest() {
    successful = true;
    int32 i;
    numberOfElements = 66;
    //Fulfill table
    elementInPtr = (intptr *) &i;
    i = 0;
    while (i < numberOfElements) {
        sqh.QueueAdd(elementInPtr);
        i++;
    }
    //Check the size of the table
    successful &= (numberOfElements == (int32) sqh.QueueSize());
    return successful;
}

bool StaticQueueHolderTest::QueueExtractTest() {
    successful = true;
    int32 i;
    numberOfElements = 66;
    //Fulfill table
    elementInPtr = (intptr *) &i;
    i = 0;
    while (i < numberOfElements) {
        sqh.QueueAdd(elementInPtr);
        i++;
    }
    //Check the size of the table
    successful &= (numberOfElements == (int32) sqh.QueueSize());

    //extract all the elements
    i = 0;
    while (i < numberOfElements) {
        //Extract the last element of the queue
        successful &= sqh.QueueExtract(elementOutPtr);
        //Check that the extracted element is the expected value
        successful &= (i == elementOut);
        i++;
    }
    //Check the size of the Queue
    successful &= (0 == sqh.QueueSize());
    //try to extract from an empty table
    successful &= !sqh.QueueExtract(elementOutPtr);
    //

    return successful;
}

bool StaticQueueHolderTest::QueuePeekTest() {
    successful = true;
    int32 i;
    numberOfElements = 66;
    //Fulfill table
    elementInPtr = (intptr *) &i;
    i = 0;
    while (i < numberOfElements) {
        sqh.QueueAdd(elementInPtr);
        i++;
    }
    //Check the size of the table
    successful &= (numberOfElements == (int32) sqh.QueueSize());

    //Read the Queue
    i = 0;
    while (i < numberOfElements) {
        //Check that the read of the position i is done successfully
        successful &= sqh.QueuePeek(elementOutPtr, i);
        //Check that the value in the position i is as expected
        successful &= (elementOut == numberOfElements - 1 - i);
        i++;
    }
    //Check that the read of the last element introduced (the first
    //position) is done successfully
    successful &= sqh.QueuePeekLast(elementOutPtr);
    //Check that the value in the first position is as expected
    successful &= (elementOut == numberOfElements - 1);

    //Check that the read of the last element introduced (the first
    //position) is done successfully
    successful &= sqh.Last(elementOutPtr);

    //Check that the value in the first position is as expected
    successful &= (elementOut == numberOfElements - 1);
    return successful;
}

