/**
 * @file IntrospectionStructureTest.cpp
 * @brief Source file for class IntrospectionStructureTest
 * @date 19/11/2018
 * @author Andre Neto
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
 * the class IntrospectionStructureTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "IntrospectionStructure.h"
#include "IntrospectionStructureTest.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool IntrospectionStructureTest::TestStructureRegistration() {
    using namespace MARTe;
    const char8 * const config = ""
            "+Types = {"
            "  Class = ReferenceContainer"
            "  +IntrospectionStructureTestEx1 = {"
            "     Class = IntrospectionStructure"
            "     Field1 = {"
            "       Type = uint32"
            "       NumberOfElements = {1, 1}"
            "     }"
            "     Field2 = {"
            "       Type = float32"
            "       NumberOfElements = {3, 2}"
            "     }"
            "   }"
            "   +IntrospectionStructureTestEx2 = {"
            "     Class = IntrospectionStructure"
            "     Field1 = {"
            "       Type = IntrospectionStructureTestEx1"
            "       NumberOfElements = {3, 1, 2}"
            "     }"
            "     Field2 = {"
            "       Type = float32"
            "       NumberOfElements = 3"
            "     }"
            "     Field3 = {"
            "       Type = float64"
            "       NumberOfElements = {1}"
            "     }"
            "     Field4 = {"
            "       Type = float64"
            "       NumberOfElements = 1"
            "     }"
            "     Field5 = {"
            "       Type = float64"
            "       NumberOfElements = {2}"
            "     }"
            "     Field6 = {"
            "       Type = float64"
            "       NumberOfElements = 2"
            "     }"
            "   }"
            "}";

    ConfigurationDatabase cdb;
    StreamString configStr = config;
    configStr.Seek(0);
    StandardParser parser(configStr, cdb);
    bool ok = parser.Parse();

    if (ok) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    const ClassRegistryItem *registeredMember = ClassRegistryDatabase::Instance()->Find("IntrospectionStructureTestEx2");
    const Introspection *intro = NULL_PTR(const Introspection *);

    if (ok) {
        ok = (registeredMember != NULL_PTR(const ClassRegistryItem *));
    }
    if (ok) {
        intro = registeredMember->GetIntrospection();
        ok = (intro != NULL_PTR(const Introspection *));
    }
    if (ok) {
        ok = (intro->GetNumberOfMembers() == 6u);
    }
    if (ok) {
        IntrospectionEntry entry = intro->operator [](0u);
        StreamString extype = "IntrospectionStructureTestEx1";
        StreamString exname = "Field1";
        if (ok) {
            ok = (exname == entry.GetMemberName());
        }
        if (ok) {
            ok = (extype == entry.GetMemberTypeName());
        }
        if (ok) {
            StreamString exmods = "[3][1][2]";
            ok = (exmods == entry.GetMemberModifiers());
        }
        const ClassRegistryItem *creEx1 = ClassRegistryDatabase::Instance()->Find("IntrospectionStructureTestEx1");
        const Introspection *introEx1 = NULL_PTR(const Introspection *);
        if (ok) {
            ok = (creEx1 != NULL_PTR(const ClassRegistryItem *));
        }
        if (ok) {
            introEx1 = creEx1->GetIntrospection();
            ok = (introEx1 != NULL_PTR(const Introspection *));
        }
        if (ok) {
            uint32 exmsize = introEx1->GetClassSize();
            ok = (exmsize == entry.GetMemberSize());
        }
    }
    if (ok) {
        IntrospectionEntry entry = intro->operator [](1u);
        StreamString extype = "float32";
        StreamString exname = "Field2";
        if (ok) {
            ok = (exname == entry.GetMemberName());
        }
        if (ok) {
            ok = (extype == entry.GetMemberTypeName());
        }
        if (ok) {
            StreamString exmods = "[3]";
            ok = (exmods == entry.GetMemberModifiers());
        }
    }
    if (ok) {
        IntrospectionEntry entry = intro->operator [](2u);
        StreamString extype = "float64";
        StreamString exname = "Field3";
        if (ok) {
            ok = (exname == entry.GetMemberName());
        }
        if (ok) {
            ok = (extype == entry.GetMemberTypeName());
        }
        if (ok) {
            StreamString exmods = "";
            ok = (exmods == entry.GetMemberModifiers());
        }
    }
    if (ok) {
        IntrospectionEntry entry = intro->operator [](3u);
        StreamString extype = "float64";
        StreamString exname = "Field4";
        if (ok) {
            ok = (exname == entry.GetMemberName());
        }
        if (ok) {
            ok = (extype == entry.GetMemberTypeName());
        }
        if (ok) {
            StreamString exmods = "";
            ok = (exmods == entry.GetMemberModifiers());
        }
    }
    if (ok) {
        IntrospectionEntry entry = intro->operator [](4u);
        StreamString extype = "float64";
        StreamString exname = "Field5";
        if (ok) {
            ok = (exname == entry.GetMemberName());
        }
        if (ok) {
            ok = (extype == entry.GetMemberTypeName());
        }
        if (ok) {
            StreamString exmods = "[2]";
            ok = (exmods == entry.GetMemberModifiers());
        }
    }
    if (ok) {
        IntrospectionEntry entry = intro->operator [](5u);
        StreamString extype = "float64";
        StreamString exname = "Field6";
        if (ok) {
            ok = (exname == entry.GetMemberName());
        }
        if (ok) {
            ok = (extype == entry.GetMemberTypeName());
        }
        if (ok) {
            StreamString exmods = "[2]";
            ok = (exmods == entry.GetMemberModifiers());
        }
    }
    return ok;
}

bool IntrospectionStructureTest::TestStructureRegistration_False_TypeAlreadyRegistered() {
    using namespace MARTe;
    const char8 * const config = ""
            "+Types = {"
            "  Class = ReferenceContainer"
            "  +IntrospectionStructureTestEx3 = {"
            "     Class = IntrospectionStructure"
            "     Field1 = {"
            "       Type = uint32"
            "       NumberOfElements = {1, 1}"
            "     }"
            "     Field2 = {"
            "       Type = float32"
            "       NumberOfElements = {3, 2}"
            "     }"
            "   }"
            "}";
    bool ok = true;
    {
        ConfigurationDatabase cdb;
        StreamString configStr = config;
        configStr.Seek(0);
        StandardParser parser(configStr, cdb);
        ok = parser.Parse();

        if (ok) {
            cdb.MoveToRoot();
            ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
        }
    }
    {
        ConfigurationDatabase cdb;
        StreamString configStr = config;
        configStr.Seek(0);
        StandardParser parser(configStr, cdb);
        ok = parser.Parse();
        if (ok) {
            cdb.MoveToRoot();
            ok = !ObjectRegistryDatabase::Instance()->Initialise(cdb);
        }
    }
    ObjectRegistryDatabase::Instance()->Purge();
    return ok;
}

bool IntrospectionStructureTest::TestStructureRegistration_False_TypeNoParameters() {
    using namespace MARTe;
    const char8 * const config = ""
            "+Types = {"
            "  Class = ReferenceContainer"
            "  +IntrospectionStructureTestEx4 = {"
            "     Class = IntrospectionStructure"
            "   }"
            "}";
    bool ok = true;
    ConfigurationDatabase cdb;
    StreamString configStr = config;
    configStr.Seek(0);
    StandardParser parser(configStr, cdb);
    ok = parser.Parse();

    if (ok) {
        cdb.MoveToRoot();
        ObjectRegistryDatabase::Instance()->Purge();
        ok = !ObjectRegistryDatabase::Instance()->Initialise(cdb);
    }
    return ok;
}
