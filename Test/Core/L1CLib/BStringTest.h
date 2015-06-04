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
 * @class BStringTest
 * @brief Tests the BString class and associated functions.
 *
 * The test consists in observe the results of the various operators between BStrings, const char* and char.
 */

#ifndef BSTRING_TEST_H
#define BSTRING_TEST_H

#include "BString.h"

class BStringTest {

private:

public:

    BStringTest() {
    }
     
    /**
     * Tests FSRead() and FSWrite() functions copying a char buffer on the BString and the contrary.
     * @return true if the functions works as expected. **/
    bool TestFSReadAndFSWrite();

    /**
     * Tests the operators and the functions between const char* and BStrings.
     * @return true if all operations work as expected. **/
    bool TestStringOperators(const char* string1, const char* string2);

    /**
     * Tests the operators and the functions between char and BStrings.
     * @return true if all operations work as expected. **/
    bool TestCharOperators(char char1, char char2);

};

#endif
