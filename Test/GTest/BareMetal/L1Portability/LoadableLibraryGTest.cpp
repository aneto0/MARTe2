/**
 * @file LoadableLibraryGTest.cpp
 * @brief Source file for class LoadableLibraryGTest
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
 * the class LoadableLibraryGTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "LoadableLibraryTest.h"
#include "TestSupport.h"
#include "CCString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


#define str(x) #x

MARTe::CCString GetLibName(){
	MARTe::CCString platform;
	MARTe::CCString libName;
	platform = str(ENVIRONMENT);

	if (platform == "Windows"){
		libName == "msvcr120.dll";
	} else
	if (platform == "Linux"){
		libName == "libm.so";
	}

	return libName;
}

#define libName GetLibName()

TEST(LoadableLibraryGTest,TestConstructor) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestConstructor());
}

TEST(LoadableLibraryGTest,TestOpenCloseLibrary) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestOpenLibrary(libName));
}

TEST(LoadableLibraryGTest,TestCloseLibrary) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestCloseLibrary(libName));
}

TEST(LoadableLibraryGTest,TestLoadFunction) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestLoadFunction(libName));
}

TEST(LoadableLibraryGTest,TestLoadFunctionSquareBracketsOperator) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestLoadFunctionSquareBracketsOperator(libName));
}

TEST(LoadableLibraryGTest,TestGetModule) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestGetModule(libName));
}

TEST(LoadableLibraryGTest,TestSetModule) {
    LoadableLibraryTest loadablelibrarytest;
    ASSERT_TRUE(loadablelibrarytest.TestSetModule(libName));
}
