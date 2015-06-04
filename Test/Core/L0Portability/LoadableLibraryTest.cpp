/* Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http: //ec.europa.eu/idabc/eupl
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

#include "GeneralDefinitions.h"
#include "LoadableLibraryTest.h"

//Test if the the library in the argument is opened.
bool LoadableLibraryTest::TestOpenCloseLibrary(const char *dllName) {
    if (myLoadableLibrary.Open(dllName)) {
        myLoadableLibrary.Close();
        return True;
    }
    else {
        return False;
    }
}

//Tests if the function in the argument is loaded.
bool LoadableLibraryTest::TestLoadFunction(const char *dllName,
                                           const char *dllFunction) {
    if (!myLoadableLibrary.Open(dllName)) {
        return False;
    }

    if (myLoadableLibrary.Function(NULL) != NULL) {
        return False;
    }

    return myLoadableLibrary[dllFunction] != NULL;
}
