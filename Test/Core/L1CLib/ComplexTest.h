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
 * $Id:$
 *
 **/
/**
 * @class ComplexTest
 * @brief Tests the Complex class and associated functions.
 *
 * The test make operations with complex numbers provided from Complex class and check if the mathematical resuls are correct.
 */

#ifndef COMPLEX_TEST_H
#define COMPLEX_TEST_H

/** Definition of the PI constant **/
#define pi 3.1415
#define e 2.71828 

#include "Complex.h"

class ComplexTest {

private:

public:

    ComplexTest() {
    }

    /**
     * Tests the different constructors for Complex class. 
     * @return true if constructors initialize the complex number correctly. **/
    bool TestConstructors();

    /**
     * Tests the operators of add and sub operations.
     * @return true if all operations give the expected result. **/
    bool TestAddAndDifferenceOperators();

    /** 
     * Tests the operators of the product operations.
     * @return true if all operations give the expected result. **/
    bool TestProductOperators();

    /**
     * Tests the operators of the division operations and the functions which returns the norm of a complex number.
     * @return true if all operations give the expected result. **/
    bool TestNormAndDivision();

    /**
     * Tests the mathematical operations like sin, cos, log, sqrt on complex numbers.
     * @return true if all operations give the expected result. **/
    bool TestMathOperations();

};

#endif
