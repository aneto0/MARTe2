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
 * @file LoadableLibraryTest.h
 * @brief Tests the LoadableLibrary and associated functions.
 *
 * The test consists in concurrent threads incrementing a shared variable.
 * These will only be allowed to change this variable value when the semaphore is locked.
 */

#ifndef LOADABLE_LIBRARY_TEST_H
#define LOADABLE_LIBRARY_TEST_H

#include "LoadableLibrary.h"
#include "Sleep.h"

/** @brief Class for testing of LoadableLibrary functions. */
class LoadableLibraryTest {

private:

public:
    /** LoadableLibrary object */
    LoadableLibrary myLoadableLibrary;

    LoadableLibraryTest() {
    }

    /**
     * @brief Tests the correct open of the library specified in the name.
     * @param dllName is the name of the library.
     * @return true if LoadableLibrary::Open() returns true, false otherwise.
     **/
    bool TestOpenLibrary(const char *dllName);

    /**
     * @brief Tests the correct close of the library specified in the name.
     * @details First a library is opened then it is closed, however cannot be tested that the close function
     * close the library correctly.
     * @param dllName is the name of the library.
     * @return true if LoadableLibrary::Open() returns true, false otherwise.
     * @pre The library has to be opened previously.
     **/
    bool TestCloseLibrary(const char *dllName);

    /**
     * @brief Tests the correct load of a library function.
     * @details Use one of the libm.so function to ensure the correct behavior of the
     * LoadableLibrary::Function
     * @return true if cosf(0.0) = 1.0;
     **/
    bool TestLoadFunction();

    /**
     * @brief Tests the correct load of a library function using square brackets.
     * @details Use one of the libm.so function to ensure the correct behavior of the
     * LoadableLibrary::Function using the [] syntax.
     * @return true if cosf(0.0) = 1.0;.
     **/
    bool TestLoadFunctionSquareBracketsOperator();

    /**
     * @brief Tests LoadableLibrary::GetModule function.
     * @details This test opens a library and the checks the returned HANDLE.
     * @return true if the returned HANDLE is not NULL;.
     **/
    bool TestGetModule();

    /**
     * @brief Tests LoadableLibrary::SetModule function.
     * @detail Open library and change its HANDEL several times checking the set value using
     * LoadableLibrary::GetModule().
     * @return true if LoadableLibrary::GetModule() returns the Set HANDLE.
     */
    bool TestSetModule();

};

#endif
