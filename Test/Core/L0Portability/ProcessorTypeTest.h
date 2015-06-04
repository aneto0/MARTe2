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
 * $Id$
 *
 **/
/**
 * @file ProcessorTypeTest.h 
 * @brief Tests the ProcessorType class and associated functions
 */

#ifndef PROCESSORTYPE_TEST_H
#define PROCESSORTYPE_TEST_H

#include "ProcessorType.h"

/** @brief A class for testing of the ProcessorType functions. */
class ProcessorTypeTest {
private:
    ProcessorType ptFirst;
    ProcessorType ptSecond;
    ProcessorType ptAny;
    ProcessorType ptNone;

public:

    ProcessorTypeTest() :
            ptFirst(0xFE), ptSecond(0xFD), ptAny(0xFF), ptNone(0x0) {

    }

    /**
     * @brief Tests the overloaded assignment operator in ProcessorType
     * @return True if the test is successful
     */
    bool TestAssignmentOperator() {

        bool result = true;

        ProcessorType ptTest;
        ptTest = 0xFD;
        result &= (ptTest.processorMask == 0xFD);

        ptTest = ptSecond;
        result &= (ptTest.processorMask == ptSecond.processorMask);

        return result;
    }

    /**
     * @brief Tests the overloaded OR operator in ProcessorType
     * @return True if the test is successful
     */
    bool TestOROperator() {

        bool result = true;

        ProcessorType ptTest(ptSecond);
        ptTest |= ptAny;
        result &= (ptTest == ptAny);

        ptTest = ptSecond;
        ptTest |= ptFirst;
        result &= (ptTest == ptAny);

        ptTest = ptSecond;
        ptTest |= ptNone;
        result &= (ptTest == ptSecond);

        return result;

    }

    /**
     * @brief Tests the overloaded equality operator in ProcessorType
     * @return True if the test is successful
     */
    bool TestEqualityOperator() {

        bool result = true;

        ProcessorType ptTest(ptSecond);
        result &= (ptTest == 0xFD);

        ptTest = 0xFD;
        result &= (ptTest == ptSecond);

        return result;

    }

    /**
     * @brief Tests the overloaded inequality operator in ProcessorType
     * @return True if the test is successful
     */
    bool TestInequalityOperator() {

        bool result = true;

        ProcessorType ptTest(0xFA);
        result &= (ptTest != 0xFD);

        result &= (ptTest != ptSecond);

        return result;

    }

    /**
     * @brief Tests Get/SetDefaultCPUs in ProcessorType
     * @return True if the test is successful
     */
    bool TestGetSetDefaultCPUs() {
        ProcessorType ptTest(ptSecond);

        ptTest.SetDefaultCPUs(0xAA);
        uint32 test = ptTest.GetDefaultCPUs();

        return (test == 0xAA);
    }

    /**
     * @brief Tests ProcessorType's constructors
     * @return True if the test is successful
     */
    bool TestConstructors() {
        ProcessorType ptDefault;             // Std contructor
        ProcessorType ptFromMask(0xFC);      // Mask constructor
        ProcessorType ptFromPT(ptFromMask);  // Constructor from other PT

        bool result = (ptDefault.processorMask == 0xFE);
        result &= (ptFromMask.processorMask == 0xFC);
        result &= (ptFromPT.processorMask == 0xFC);

        return result;
    }

};

#endif

