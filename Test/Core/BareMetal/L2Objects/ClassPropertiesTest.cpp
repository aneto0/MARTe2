/**
 * @file ClassPropertiesTest.cpp
 * @brief Source file for class ClassPropertiesTest
 * @date 11/08/2015
 * @author Giuseppe Ferrò
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
 * the class ClassPropertiesTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassPropertiesTest.h"
#include "StringHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool ClassPropertiesTest::TestDefaultConstructor() {
    ClassProperties classProperties;
    return (classProperties.GetName() == NULL) && (classProperties.GetVersion() == NULL);
}

bool ClassPropertiesTest::TestFullConstructor(const char8 *name,
                                              const char8 *version) {
    ClassProperties classProperties(name, version);

    if ((StringHelper::Compare(name, classProperties.GetName()) != 0) || (StringHelper::Compare(version, classProperties.GetVersion())!=0)) {
        return false;
    }

    ClassProperties classProperties1((const char8*) NULL, version);

    if ((classProperties1.GetName() != NULL) || (StringHelper::Compare(version, classProperties1.GetVersion())!=0)) {
        return false;
    }

    ClassProperties classProperties2(name, (const char8*) NULL);

    if ((StringHelper::Compare(name, classProperties2.GetName()) != 0) || (classProperties2.GetVersion() != NULL)) {
        return false;
    }

    ClassProperties classProperties3(NULL, NULL);

    return (classProperties3.GetName() == NULL) && (classProperties3.GetVersion() == NULL);

}

bool ClassPropertiesTest::TestGetName(const char8*name) {
    ClassProperties classProperties;
    if (classProperties.GetName() != NULL) {
        return false;
    }

    ClassProperties classProperties1(name, NULL);

    return (StringHelper::Compare(classProperties1.GetName(), name) == 0);

}

bool ClassPropertiesTest::TestGetVersion(const char8*version) {
    ClassProperties classProperties;
    if (classProperties.GetVersion() != NULL) {
        return false;
    }

    ClassProperties classProperties1(NULL, version);

    return (StringHelper::Compare(classProperties1.GetVersion(), version) == 0);

}

