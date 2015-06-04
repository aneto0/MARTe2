/*
 * StaticQueueHolderTest.h
 *
 *  Created on: Mar 13, 2015
 *      Author: shareuser
 */

/** @file StaticQueueHolderTest.h
  * @brief Tests the StaticQueueHolder class and associate functions. */

#ifndef STATICQUEUEHOLDERTEST_H_
#define STATICQUEUEHOLDERTEST_H_

#include "StaticStackHolder.h"
#include "StaticQueueHolder.h"


/** @brief Class used for tests on StaticQueueHolder functions. */
class StaticQueueHolderTest {
public:
    StaticQueueHolderTest() {
        elementInPtr = NULL;
        elementOut = 0;
        ;
        elementOutPtr = (intptr *) &elementOut;
        numberOfElements = 10000;
        counter = 0;
        successful = false;
    }
    virtual ~StaticQueueHolderTest() {

    }
    StaticQueueHolder sqh;

    intptr *elementInPtr;

    int32 elementOut;

    intptr *elementOutPtr;

    int32 numberOfElements;

    int32 counter;

    bool successful;

    /**
     * @brief Test QueueAdd
     * @return true when the size of the table is as expected
     */
    bool QueueAddTest();

    /**
     * @brief Test QueueExtract
     *
     * Fulfill a list and then extract all the elements. Also is tested the extraction from
     * an empty List
     *
     * @return true when all the elements are extracted correctly if is possible.
     */
    bool QueueExtractTest();

    /**
     * @brief Test the function @see StaticQueueHolder::QueuePeekTest
     * @brief, @see StaticQueueHolder::QueuePeekLast and
     * @brief StaticQueueHolder::Last work as expected
     * @return true when the values of different positions are obtained correctly
     */
    bool QueuePeekTest();

};

#endif /* TEST_L1CLIB_STATICQUEUEHOLDERTEST_H_ */
