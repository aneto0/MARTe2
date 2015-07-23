/**
 * @file LoadableLibraryTest.cpp
 * @brief Source file for class LoadableLibraryTest2
 * @date 23/07/2015
 * @author Llorenç Capellà
 *
 * @copyright Copyright 2015 F4E | European Joint Undertaking for ITER and
 * the Development of Fusion Energy ('Fusion for Energy').
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing, 
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This source file contains the definition of all the methods for
 * the class LoadableLibraryTest2 (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "LoadableLibraryTest.h"
#include "GeneralDefinitions.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

LoadableLibraryTest::LoadableLibraryTest() {
    // Auto-generated constructor stub for LoadableLibraryTest2
    // TODO Verify if manual additions are needed
}

LoadableLibraryTest::~LoadableLibraryTest() {
    // Auto-generated destructor stub for LoadableLibraryTest2
    // TODO Verify if manual additions are needed
}

bool LoadableLibraryTest::TestConstructor() {
    LoadableLibrary defaultLoadableLibrary;
    return (defaultLoadableLibrary.GetModule() == NULL);
}

bool LoadableLibraryTest::TestOpenLibrary(const char *dllName) {
    return myLoadableLibrary.Open(dllName);
}

bool LoadableLibraryTest::TestCloseLibrary(const char *dllName) {
    bool retValue;
    float (*pf)(float);
    pf = NULL;
    retValue = myLoadableLibrary.Open(dllName);
    myLoadableLibrary.Close();
    return retValue;
}

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

