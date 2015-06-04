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
     * @return true if the library is open, false otherwise. **/
    bool TestOpenCloseLibrary(const char *dllName);
    
    /**@brief Tests the correct load of a library function.
     * @param dllName is the name of the library to open.
     * @param dllFunction is the name of the function to load. **/
    bool TestLoadFunction(const char *dllName, const char *dllFunction);
};

#endif
