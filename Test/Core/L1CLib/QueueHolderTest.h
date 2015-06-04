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
 */
/**
 * @file QueueHolderTest.h
 * @brief Tests the QueueHolder class and Iterators associated functions.
 *
 * The test consists in insert and remove elements from the queue observing if the operations are FIFO.
 */

#ifndef QUEUEHOLDERTEST_H_
#define QUEUEHOLDERTEST_H_

#include "QueueHolder.h"
#include "Iterators.h"


/** @brief Create a list of integers**/
class IntegerLinkedListable: public LinkedListable {
public:
    uint32 intNumber;
public:
    IntegerLinkedListable() {
        intNumber = 0;
    }

    ~IntegerLinkedListable() {

    }
};

/** @brief Class used for tests on QueueHolder functions. **/
class QueueHolderTest {
public:

    QueueHolderTest() {

    }
    virtual ~QueueHolderTest() {

    }

    /**
     * @brief Tests many functions of QueueHolder class. 
     * @return true if all operations works correctly. **/
    bool TestAll();

    /**
     * @brief Tests the adding, peek and extracting functions with null or index out of bounds as argument.
     * @return true if all functions return as expected. **/
    bool TestNULLConditions();

};

#endif 
