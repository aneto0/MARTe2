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
 * @file ProcessorTest.h
 * @brief Tests the Processor functions. */
#ifndef PROCESSOR_TEST_H
#define PROCESSOR_TEST_H

#include "Processor.h"

/** @brief A class for testing of the Processor functions. */
class ProcessorTest {

public:

    /** @brief Constructor */
    ProcessorTest() {
    }

    /**
     * @brief Tests the vendor identifier function.
     * @return true if the function returns the vendor id. */
    bool TestVendorId() {
        bool testResult = false;
        const char * vendorId = NULL;

        vendorId = Processor::VendorId();
        testResult = (vendorId != NULL);

        return testResult;
    }

    /**
     * @brief Tests the family function.
     * @return true.
     */
    bool TestFamily() {
        bool testResult = false;
        uint32 family = -1;
        uint32 familyAux = family;

        family = Processor::Family();
        testResult = (family != familyAux);

        return testResult;
    }

    /**
     * @brief Tests the model function.
     * @return true.
     */
    bool TestModel() {
        bool testResult = false;
        uint32 model = -1;
        uint32 modelAux = model;

        model = Processor::Model();
        testResult = (model != modelAux);

        return testResult;
    }

    /**
     * @brief Tests the available function.
     * @return true.
     */
    bool TestAvailable() {
        bool testResult = false;
        uint32 available = -1;
        uint32 availableAux = available;

        available = Processor::Available();
        testResult = (available != availableAux);

        return testResult;
    }

};

#endif
