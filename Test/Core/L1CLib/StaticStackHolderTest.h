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
/**
 * @file StaticStackHolderTest.h
 * @brief Tests the StaticList class and associated functions
 */
#ifndef STATICLISCTHOLDER_test_H_
#define STATICLISCTHOLDER_test_H_
#include "StaticListHolder.h"
#include "StaticStackHolder.h"
#include "EventSem.h"
#include "MutexSem.h"

/** @brief Class used for tests on StaticStackHolder functions. */
class StaticStackHolderTest {
public:
    /** @brief Constructor */
    StaticStackHolderTest() {
        successful = false;
        elements[0] = 0;
        elements[1] = 1;
        elements[2] = 2;
        elements[3] = 3;
        elementsPtr[0] = (intptr *) &elements[0];
        elementsPtr[1] = (intptr *) &elements[1];
        elementsPtr[2] = (intptr *) &elements[2];
        elementsPtr[3] = (intptr *) &elements[3];
        returnElement = 0;
        returnElementPtr = (intptr *) &returnElement;

    }
    StaticStackHolder ssh;
    bool successful;
    int32 elements[4];
    intptr *elementsPtr[4];
    int32 returnElement;
    intptr *returnElementPtr;
    virtual ~StaticStackHolderTest() {

    }
    /**
     * @brief Test all the functions of @see StaticStackHolder.
     *
     * The function tested are: StackPush (notice that in returns void), StackPop
     * StackDepth, StackPeek, StackTop. All the functions are a direct call to
     * the StaticListHolder.
     *
     * @return true when the values are as expected.
     */
    bool StackTest();
};
#endif
