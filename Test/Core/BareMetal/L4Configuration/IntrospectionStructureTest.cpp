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
            "     Field7 = {"
            "       Type = bool"
            "       NumberOfElements = 1"
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
        ok = (intro->GetNumberOfMembers() == 7u);
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
        if (ok) {
            uint32 exmsize = sizeof(float32);
            ok = (exmsize == entry.GetMemberSize());
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
        if (ok) {
            uint32 exmsize = sizeof(float64);
            ok = (exmsize == entry.GetMemberSize());
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
        if (ok) {
            uint32 exmsize = sizeof(float64);
            ok = (exmsize == entry.GetMemberSize());
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
        if (ok) {
            uint32 exmsize = sizeof(float64);
            ok = (exmsize == entry.GetMemberSize());
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
        if (ok) {
            uint32 exmsize = sizeof(float64);
            ok = (exmsize == entry.GetMemberSize());
        }
    }
    if (ok) {
        IntrospectionEntry entry = intro->operator [](6u);
        StreamString extype = "bool";
        StreamString exname = "Field7";
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
        if (ok) {
            uint32 exmsize = sizeof(bool);
            ok = (exmsize == entry.GetMemberSize());
        }
    }

    return ok;
}

bool IntrospectionStructureTest::TestStructureRegistration_TypeAlreadyRegistered() {
    using namespace MARTe;

    bool ok = true;
    {
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
        const char8 * const config = ""
                "+Types = {"
                "  Class = ReferenceContainer"
                "  +IntrospectionStructureTestEx3 = {"
                "     Class = IntrospectionStructure"
                "     Field1 = {"
                "       Type = uint64"
                "       NumberOfElements = {2, 3}"
                "     }"
                "     Field2 = {"
                "       Type = float64"
                "       NumberOfElements = {1, 2}"
                "     }"
                "   }"
                "}";
        ConfigurationDatabase cdb;
        StreamString configStr = config;
        configStr.Seek(0);
        StandardParser parser(configStr, cdb);
        ok = parser.Parse();
        if (ok) {
            cdb.MoveToRoot();
            ok = ObjectRegistryDatabase::Instance()->Initialise(cdb);
        }
        if (ok) {
            const Introspection *intro = ClassRegistryDatabase::Instance()->Find("IntrospectionStructureTestEx3")->GetIntrospection();
            IntrospectionEntry entry0 = intro->operator [](0u);
            StreamString extype = "uint64";
            StreamString exname = "Field1";
            if (ok) {
                ok = (exname == entry0.GetMemberName());
            }
            if (ok) {
                ok = (extype == entry0.GetMemberTypeName());
            }
            if (ok) {
                StreamString exmods = "[2][3]";
                ok = (exmods == entry0.GetMemberModifiers());
            }
            IntrospectionEntry entry1 = intro->operator [](1u);
            extype = "float64";
            exname = "Field2";
            if (ok) {
                ok = (exname == entry1.GetMemberName());
            }
            if (ok) {
                ok = (extype == entry1.GetMemberTypeName());
            }
            if (ok) {
                StreamString exmods = "[1][2]";
                ok = (exmods == entry1.GetMemberModifiers());
            }
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

bool IntrospectionStructureTest::TestStructureRegistration_False_TypeAlreadyRegistered_Class() {
    using namespace MARTe;

    bool ok = true;
    {
        const char8 * const config = ""
                "+Types = {"
                "  Class = ReferenceContainer"
                "  +Object = {"
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

bool IntrospectionStructureTest::TestRegisterStructuredDataI() {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("IntrospectionStructureTestEx4");
    uint32 a = 0u;
    cdb.Write("a", a);
    cdb.CreateRelative("IntrospectionStructureTestEx5");
    float32 b = 0.;
    cdb.Write("b", b);
    uint32 bv[4];
    cdb.Write("bv", bv);
    uint32 bvv[4][2];
    cdb.Write("bvv", bvv);

    cdb.CreateRelative("IntrospectionStructureTestEx6");
    StreamString str = "c";
    cdb.Write("s", str);

    cdb.MoveAbsolute("IntrospectionStructureTestEx4");
    bool ok = IntrospectionStructure::RegisterStructuredDataI(cdb);
    cdb.MoveToRoot();
    const ClassRegistryItem *registeredMember = ClassRegistryDatabase::Instance()->Find("AutoGeneratedIntrospectionStructureTestEx4");
    const Introspection *intro = NULL_PTR(const Introspection *);

    if (ok) {
        ok = (registeredMember != NULL_PTR(const ClassRegistryItem *));
    }
    if (ok) {
        intro = registeredMember->GetIntrospection();
        ok = (intro != NULL_PTR(const Introspection *));
    }
    if (ok) {
        ok = (intro->GetNumberOfMembers() == 2u);
    }
    if (ok) {
        IntrospectionEntry entry = intro->operator [](0u);
        StreamString extype = "uint32";
        StreamString exname = "a";
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
        IntrospectionEntry entry = intro->operator [](1u);
        StreamString extype = "AutoGeneratedIntrospectionStructureTestEx5";
        StreamString exname = "IntrospectionStructureTestEx5";
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
    registeredMember = ClassRegistryDatabase::Instance()->Find("AutoGeneratedIntrospectionStructureTestEx5");
    intro = NULL_PTR(const Introspection *);
    if (ok) {
        ok = (registeredMember != NULL_PTR(const ClassRegistryItem *));
    }
    if (ok) {
        intro = registeredMember->GetIntrospection();
        ok = (intro != NULL_PTR(const Introspection *));
    }
    if (ok) {
        ok = (intro->GetNumberOfMembers() == 4u);
    }
    if (ok) {
        IntrospectionEntry entry = intro->operator [](1u);
        StreamString extype = "uint32";
        StreamString exname = "bv";
        if (ok) {
            ok = (exname == entry.GetMemberName());
        }
        if (ok) {
            ok = (extype == entry.GetMemberTypeName());
        }
        if (ok) {
            StreamString exmods = "[4]";
            ok = (exmods == entry.GetMemberModifiers());
        }
    }
    if (ok) {
        IntrospectionEntry entry = intro->operator [](2u);
        StreamString extype = "uint32";
        StreamString exname = "bvv";
        if (ok) {
            ok = (exname == entry.GetMemberName());
        }
        if (ok) {
            ok = (extype == entry.GetMemberTypeName());
        }
        if (ok) {
            StreamString exmods = "[2][4]";
            ok = (exmods == entry.GetMemberModifiers());
        }
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}

bool IntrospectionStructureTest::TestRegisterStructuredDataI_TypeDeclared() {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("A");
    cdb.Write("TypeId", "IntrospectionStructureTestEx4");
    uint32 a = 0u;
    cdb.Write("a", a);
    cdb.CreateRelative("i1");
    cdb.Write("TypeId", "IntrospectionStructureTestEx5");
    float32 b = 0.;
    cdb.Write("b", b);
    uint32 bv[4];
    cdb.Write("bv", bv);
    uint32 bvv[4][2];
    cdb.Write("bvv", bvv);

    cdb.CreateRelative("i2");
    cdb.Write("TypeId", "IntrospectionStructureTestEx6");
    StreamString str = "c";
    cdb.Write("s", str);

    cdb.MoveAbsolute("A");
    bool ok = IntrospectionStructure::RegisterStructuredDataI(cdb, "TypeId");
    cdb.MoveToRoot();
    const ClassRegistryItem *registeredMember = ClassRegistryDatabase::Instance()->Find("IntrospectionStructureTestEx4");
    const Introspection *intro = NULL_PTR(const Introspection *);

    if (ok) {
        ok = (registeredMember != NULL_PTR(const ClassRegistryItem *));
    }
    if (ok) {
        intro = registeredMember->GetIntrospection();
        ok = (intro != NULL_PTR(const Introspection *));
    }
    if (ok) {
        ok = (intro->GetNumberOfMembers() == 2u);
    }
    if (ok) {
        IntrospectionEntry entry = intro->operator [](0u);
        StreamString extype = "uint32";
        StreamString exname = "a";
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
        IntrospectionEntry entry = intro->operator [](1u);
        StreamString extype = "IntrospectionStructureTestEx5";
        StreamString exname = "i1";
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
    registeredMember = ClassRegistryDatabase::Instance()->Find("IntrospectionStructureTestEx5");
    intro = NULL_PTR(const Introspection *);
    if (ok) {
        ok = (registeredMember != NULL_PTR(const ClassRegistryItem *));
    }
    if (ok) {
        intro = registeredMember->GetIntrospection();
        ok = (intro != NULL_PTR(const Introspection *));
    }
    if (ok) {
        ok = (intro->GetNumberOfMembers() == 4u);
    }

    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}

bool IntrospectionStructureTest::TestRegisterStructuredDataI_ArraysStructures() {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("IntrospectionStructureTestEx10");
    uint32 a = 0u;
    cdb.Write("a", a);
    cdb.CreateRelative("IntrospectionStructureTestEx11[0]");
    float32 b = 0.;
    cdb.Write("b", b);
    cdb.MoveToAncestor(1u);
    cdb.CreateRelative("IntrospectionStructureTestEx11[1]");
    cdb.Write("b", b);
    cdb.MoveToAncestor(1u);
    cdb.CreateRelative("IntrospectionStructureTestEx11[2]");
    cdb.Write("b", b);
    cdb.MoveToAncestor(1u);
    cdb.CreateRelative("IntrospectionStructureTestEx11[3]");
    cdb.Write("b", b);
    cdb.MoveToAncestor(1u);

    cdb.MoveAbsolute("IntrospectionStructureTestEx10");
    bool ok = IntrospectionStructure::RegisterStructuredDataI(cdb);
    cdb.MoveToRoot();
    const ClassRegistryItem *registeredMember = ClassRegistryDatabase::Instance()->Find("AutoGeneratedIntrospectionStructureTestEx10");
    const Introspection *intro = NULL_PTR(const Introspection *);

    if (ok) {
        ok = (registeredMember != NULL_PTR(const ClassRegistryItem *));
    }
    if (ok) {
        intro = registeredMember->GetIntrospection();
        ok = (intro != NULL_PTR(const Introspection *));
    }
    if (ok) {
        ok = (intro->GetNumberOfMembers() == 2u);
    }
    if (ok) {
        IntrospectionEntry entry = intro->operator [](0u);
        StreamString extype = "uint32";
        StreamString exname = "a";
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
        IntrospectionEntry entry = intro->operator [](1u);
        StreamString exname = "IntrospectionStructureTestEx11";
        StreamString extype = "AutoGeneratedIntrospectionStructureTestEx11";
        if (ok) {
            ok = (exname == entry.GetMemberName());
        }
        if (ok) {
            ok = (extype == entry.GetMemberTypeName());
        }
        if (ok) {
            StreamString exmods = "[4]";
            ok = (exmods == entry.GetMemberModifiers());
        }
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}

bool IntrospectionStructureTest::TestRegisterStructuredDataI_ArraysStructures_TypeDeclared() {
    using namespace MARTe;

    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("IntrospectionStructureTestEx10");
    uint32 a = 0u;
    cdb.Write("a", a);
    cdb.Write("TypeId", "IntrospectionStructureTestEx10");

    cdb.CreateRelative("AA[0]");
    float32 b = 0.;
    cdb.Write("b", b);
    cdb.Write("TypeId", "IntrospectionStructureTestEx11");

    cdb.MoveToAncestor(1u);
    cdb.CreateRelative("AA[1]");
    cdb.Write("b", b);
    cdb.MoveToAncestor(1u);
    cdb.CreateRelative("AA[2]");
    cdb.Write("b", b);
    cdb.MoveToAncestor(1u);
    cdb.CreateRelative("AA[3]");
    cdb.Write("b", b);
    cdb.MoveToAncestor(1u);

    cdb.MoveAbsolute("IntrospectionStructureTestEx10");
    bool ok = IntrospectionStructure::RegisterStructuredDataI(cdb, "TypeId");
    cdb.MoveToRoot();
    const ClassRegistryItem *registeredMember = ClassRegistryDatabase::Instance()->Find("IntrospectionStructureTestEx10");
    const Introspection *intro = NULL_PTR(const Introspection *);

    if (ok) {
        ok = (registeredMember != NULL_PTR(const ClassRegistryItem *));
    }
    if (ok) {
        intro = registeredMember->GetIntrospection();
        ok = (intro != NULL_PTR(const Introspection *));
    }
    if (ok) {
        ok = (intro->GetNumberOfMembers() == 2u);
    }
    if (ok) {
        IntrospectionEntry entry = intro->operator [](0u);
        StreamString extype = "uint32";
        StreamString exname = "a";
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
        IntrospectionEntry entry = intro->operator [](1u);
        StreamString exname = "AA";
        StreamString extype = "IntrospectionStructureTestEx11";
        if (ok) {
            ok = (exname == entry.GetMemberName());
        }
        if (ok) {
            ok = (extype == entry.GetMemberTypeName());
        }
        if (ok) {
            StreamString exmods = "[4]";
            ok = (exmods == entry.GetMemberModifiers());
        }
    }
    ObjectRegistryDatabase::Instance()->Purge();

    return ok;
}

