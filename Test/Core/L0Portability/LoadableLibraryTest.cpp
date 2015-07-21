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
bool LoadableLibraryTest::TestOpenLibrary(const char *dllName) {
    return myLoadableLibrary.Open(dllName);
}

//Close cannot be properly tested.
bool LoadableLibraryTest::TestCloseLibrary(const char *dllName) {
    bool retValue;
    float (*pf)(float);
    pf = NULL;
    retValue = myLoadableLibrary.Open(dllName);
    myLoadableLibrary.Close();
    return retValue;

}

//Tests if the function in the argument is loaded.
bool LoadableLibraryTest::TestLoadFunction() {
    float argument = 0;
    float (*pf)(float) = NULL;
    float result;
    if (!myLoadableLibrary.Open("libm.so")) {
        return false;
    }
    if (myLoadableLibrary.Function(NULL) != NULL) {
        return false;
    }
    *(void **) (&pf) = myLoadableLibrary.Function("cosf");
    result = pf(argument);
    pf = NULL;
    myLoadableLibrary.Close();
    return (result == 1.0);
}

bool LoadableLibraryTest::TestLoadFunctionSquareBracketsOperator() {
    float argument = 0;
    float (*pf)(float) = NULL;
    float result;
    if (!myLoadableLibrary.Open("libm.so")) {
        return false;
    }
    if (myLoadableLibrary.Function(NULL) != NULL) {
        return false;
    }
    *(void **) (&pf) = myLoadableLibrary["cosf"];
    result = pf(argument);
    pf = NULL;
    myLoadableLibrary.Close();
    return (result == 1.0);
}

bool LoadableLibraryTest::TestGetModule() {
    bool retValue;
    HANDLE m = NULL;
    retValue = (m == NULL);
    myLoadableLibrary.Open("libm.so");
    m = myLoadableLibrary.GetModule();
    retValue &= (m != NULL);
    myLoadableLibrary.Close();
    return retValue;
}

bool LoadableLibraryTest::TestSetModule() {
    bool retValue;
    HANDLE m = NULL;
    HANDLE copy = NULL;
    myLoadableLibrary.Open("libm.so");
    m = myLoadableLibrary.GetModule();
    myLoadableLibrary.SetModule(copy);
    retValue = (myLoadableLibrary.GetModule() == copy);
    myLoadableLibrary.SetModule(m);
    retValue = retValue && (myLoadableLibrary.GetModule() == m);
    myLoadableLibrary.Close();
    return retValue;
}

