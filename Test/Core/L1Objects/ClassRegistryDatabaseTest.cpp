/**
 * @file ClassRegistryDatabaseTest.cpp
 * @brief Source file for class ClassRegistryDatabaseTest
 * @date 06/08/2015
 * @author Andre' Neto
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
 * the class ClassRegistryDatabaseTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ClassRegistryDatabaseTest.h"
#include "ReferenceT.h"
#include "TestObjectHelper2.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
#include <stdio.h>
#include "Reference.h"
#include "TestObjectHelper1.h"
#include "StringHelper.h"
bool ClassRegistryDatabaseTest::TestConstructor() {
    const char8 *className = "MyDll::MyClass";
    const uint32 maxSize = 129u;
    char8 dllName[maxSize];
    dllName[0] = '\0';

    //Check for the string pattern dllName::className
    const char8 *classOnlyPartName = StringHelper::SearchString(className, "::");
    if (classOnlyPartName != NULL) {
        uint32 size = StringHelper::SearchIndex(className, "::");
        if (size > (maxSize - 1)) {
            size = (maxSize - 1);
        }
        StringHelper::CopyN(dllName, className, size);
        dllName[size] = '\0';
        className = &classOnlyPartName[2];
    }

    printf("DLL=%s\n", dllName);
    printf("ClassName=%s\n", className);
    return true;
}

