/**
 * @file StructuredDataIHelperTest.cpp
 * @brief Source file for class StructuredDataIHelperTest
 * @date 09/06/2022
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
 * the class ConfigurationDatabase (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AnyObject.h"
#include "ConfigurationDatabase.h"
#include "StructuredDataIHelperTest.h"
#include "IntrospectionTestHelper.h"
#include "Matrix.h"
#include "ObjectRegistryDatabase.h"
#include "StandardParser.h"
#include "Vector.h"

using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool StructuredDataIHelperTest::TestDefaultConstructor() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    return !sdi.HasErrors();
}

bool StructuredDataIHelperTest::TestResetErrors() {
    ConfigurationDatabase cdb;
    Reference invalidRef;
    StructuredDataIHelper sdi(cdb, invalidRef);
    bool ok = sdi.HasErrors();
    sdi.ResetErrors();
    if (ok) {
        ok = !sdi.HasErrors();
    }
    return ok;
}

bool StructuredDataIHelperTest::TestHasErrors() {
    return TestResetErrors();
}

bool StructuredDataIHelperTest::TestRead() {
    ConfigurationDatabase cdb;
    cdb.Write("Param", 10);
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    uint32 param = 0;
    if (ok) {
        ok = sdi.Read("Param", param);
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = (param == 10);
    }
    return ok;
}

bool StructuredDataIHelperTest::TestRead_False() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    uint32 param = 0;
    if (ok) {
        ok = !sdi.Read("Param", param);
    }
    if (ok) {
        ok = sdi.HasErrors();
    }
    return ok;
}

bool StructuredDataIHelperTest::TestRead_DefaultValue() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    uint32 param = 0;
    if (ok) {
        ok = sdi.Read("Param", param, 10);
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = (param == 10);
    }
    return ok;
}

bool StructuredDataIHelperTest::TestRead_DefaultValue_BadConversion() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    Vector<uint32> param(1);
    if (ok) {
        ok = !sdi.Read("Param", param, 10);
    }
    if (ok) {
        ok = sdi.HasErrors();
    }
    return ok;
}

bool StructuredDataIHelperTest::TestReadEnum() {
    ConfigurationDatabase cdb;
    cdb.Write("MyEnum", "Orange");
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    uint32 enumValue = 0;
    if (ok) {
        const char8 * options[] = {"Blue", "Black", "Orange"};
        uint32 optionValues[] = {0, 1, 5};
        ok = sdi.ReadEnum("MyEnum", enumValue, options, optionValues);
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = (enumValue == 5);
    }
    return ok;
}

bool StructuredDataIHelperTest::TestReadEnum_DefaultValue() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    uint32 enumValue = 0;
    if (ok) {
        const char8 * options[] = {"Blue", "Black", "Orange"};
        uint32 optionValues[] = {0, 1, 2};
        StreamString defaultValue = "Black";
        ok = sdi.ReadEnum("MyEnum", enumValue, options, optionValues, defaultValue);
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = (enumValue == 1);
    }
    return ok;
}

bool StructuredDataIHelperTest::TestReadEnum_False_NoDefaultValue() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    uint32 enumValue = 0;
    if (ok) {
        const char8 * options[] = {"Blue", "Black", "Orange"};
        uint32 optionValues[] = {0, 1, 2};
        ok = !sdi.ReadEnum("MyEnum", enumValue, options, optionValues);
    }
    if (ok) {
        ok = sdi.HasErrors();
    }
    return ok;
}

bool StructuredDataIHelperTest::TestReadEnum_False_InvalidOption() {
    ConfigurationDatabase cdb;
    cdb.Write("MyEnum", "Circle");
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    uint32 enumValue = 0;
    if (ok) {
        const char8 * options[] = {"Blue", "Black", "Orange"};
        uint32 optionValues[] = {0, 1, 2};
        ok = !sdi.ReadEnum("MyEnum", enumValue, options, optionValues);
    }
    if (ok) {
        ok = sdi.HasErrors();
    }
    return ok;
}

bool StructuredDataIHelperTest::TestReadEnum_False_OptionSizeMismatch() {
    ConfigurationDatabase cdb;
    cdb.Write("MyEnum", "Orange");
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    uint32 enumValue = 0;
    if (ok) {
        const char8 * options[] = {"Blue", "Black", "Orange"};
        uint32 optionValues[] = {0, 1};
        ok = !sdi.ReadEnum("MyEnum", enumValue, options, optionValues);
    }
    if (ok) {
        ok = sdi.HasErrors();
    }
    return ok;
}

bool StructuredDataIHelperTest::TestReadEnum_False_BadConversion() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    uint32 enumValue = 0;
    Vector<uint32> param(1);
    if (ok) {
        const char8 * options[] = {"Blue", "Black", "Orange"};
        uint32 optionValues[] = {0, 1, 2};
        Vector<uint32> defaultValue(1);
        ok = !sdi.ReadEnum("MyEnum", enumValue, options, optionValues, defaultValue);
    }
    if (ok) {
        ok = sdi.HasErrors();
    }
    return ok;
}

bool StructuredDataIHelperTest::TestReadValidated() {
    ConfigurationDatabase cdb;
    cdb.Write("Param", 3);
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    uint32 param;
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = sdi.ReadValidated("Param", param, "(Param > (uint32)2) && (Param < (uint32)4)");
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = (param == 3);
    }
    return ok;
}

bool StructuredDataIHelperTest::TestReadValidated_InvalidCondition() {
    ConfigurationDatabase cdb;
    cdb.Write("Param", 6);
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    uint32 param;
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = !sdi.ReadValidated("Param", param, "(Param > (uint32)2) && (Param < (uint32)4)");
    }
    if (ok) {
        ok = sdi.HasErrors();
    }
    return ok;
}

bool StructuredDataIHelperTest::TestReadArray() {
    ConfigurationDatabase cdb;
    uint32 arr[] = {1, 2};
    cdb.Write("Param", arr);
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    uint32 *param = NULL_PTR(uint32 *);
    uint32 nOfElements = 0u;
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = sdi.ReadArray("Param", param, nOfElements);
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = (nOfElements == 2u);
    }
    for (uint32 i=0; (i<nOfElements) && (ok); i++) {
        ok = (param[i] == arr[i]);
    }
    if (param != NULL_PTR(uint32 *)) {
        delete []param;
    }
    return ok;
}

bool StructuredDataIHelperTest::TestReadArray_False() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    uint32 *param = NULL_PTR(uint32 *);
    uint32 nOfElements = 0u;
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = !sdi.ReadArray("Param", param, nOfElements);
    }
    if (ok) {
        ok = sdi.HasErrors();
    }
    return ok;
}

bool StructuredDataIHelperTest::TestReadMatrix() {
    ConfigurationDatabase cdb;
    uint32 mat[3][2] = {{1, 2}, {3, 4}, {5, 6}};
    cdb.Write("Param", mat);
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    uint32 **param = NULL_PTR(uint32 **);
    uint32 nOfRows = 0u;
    uint32 nOfCols = 0u;
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = sdi.ReadMatrix("Param", param, nOfRows, nOfCols);
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = (nOfRows == 3u);
    }
    if (ok) {
        ok = (nOfCols == 2u);
    }
    for (uint32 i=0; (i<nOfRows) && (ok); i++) {
        for (uint32 j=0; (j<nOfCols) && (ok); j++) {
            ok = (param[i][j] == mat[i][j]);
        }
    }
    for (uint32 i=0; (i<nOfRows) && (ok); i++) {
        if (param[i] != NULL_PTR(uint32 *)) {
            delete [] param[i];
        }
    }
    if (param != NULL_PTR(uint32 **)) {
        delete []param;
    }
    return ok;
}

bool StructuredDataIHelperTest::TestReadMatrix_False() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    uint32 **param = NULL_PTR(uint32 **);
    uint32 nOfRows = 0u;
    uint32 nOfCols = 0u;
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = !sdi.ReadMatrix("Param", param, nOfRows, nOfCols);
    }
    if (ok) {
        ok = sdi.HasErrors();
    }
    return ok;
}

bool StructuredDataIHelperTest::TestReadMatrix_False_Dimensions() {
    ConfigurationDatabase cdb;
    uint32 mat[2] = {1, 2};
    cdb.Write("Param", mat);
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    uint32 **param = NULL_PTR(uint32 **);
    uint32 nOfRows = 0u;
    uint32 nOfCols = 0u;
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = !sdi.ReadMatrix("Param", param, nOfRows, nOfCols);
    }
    if (ok) {
        ok = sdi.HasErrors();
    }
    return ok;
}

bool StructuredDataIHelperTest::TestWrite() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = sdi.Write("Param", 3);
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    uint32 param;
    if (ok) {
        ok = cdb.Read("Param", param);
    }
    if (ok) {
        ok = (param == 3);
    }
    if (ok) {
        ok = !sdi.Write("A", voidAnyType);
    }
    if (ok) {
        ok = sdi.HasErrors();
    }
    return ok;
}

bool StructuredDataIHelperTest::TestCopy() {
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("A");
    cdb.MoveAbsolute("A");
    cdb.Write("Param", 3);
    cdb.MoveToRoot();
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    ConfigurationDatabase cdbOut;
    if (ok) {
        ok = sdi.Copy(cdbOut);
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = cdbOut.MoveAbsolute("A");
    }
    uint32 param;
    if (ok) {
        ok = cdbOut.Read("Param", param);
    }
    if (ok) {
        ok = (param == 3);
    }
    return ok;
}

bool StructuredDataIHelperTest::TestGetType() {
    ConfigurationDatabase cdb;
    uint32 param = 3;
    cdb.Write("Param", param);
    cdb.MoveToRoot();
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    if (ok) {
        AnyType tt = sdi.GetType("Param");
        ok = (tt.GetTypeDescriptor() == UnsignedInteger32Bit);
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    return ok;
}

bool StructuredDataIHelperTest::TestMoveToRoot() {
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("A.B.C");
    cdb.MoveAbsolute("A.B");
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = (sdi.MoveToRoot());
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = cdb.MoveRelative("A");
    }
    if (ok) {
        ok = !sdi.MoveRelative("D");
    }
    if (ok) {
        ok = !sdi.MoveToRoot();
    }

    return ok;
}

bool StructuredDataIHelperTest::TestMoveToAncestor() {
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("A.B.C");
    cdb.MoveAbsolute("A.B");
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = (sdi.MoveToAncestor(1u));
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = sdi.MoveRelative("B");
    }
    if (ok) {
        ok = !sdi.MoveRelative("D");
    }
    if (ok) {
        ok = !sdi.MoveToAncestor(1u);
    }

    return ok;
}

bool StructuredDataIHelperTest::TestMoveAbsolute() {
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("A.B.C");
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = (sdi.MoveAbsolute("A.B.C"));
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = !sdi.MoveAbsolute("D");
    }
    if (ok) {
        ok = !sdi.MoveAbsolute("A.B.C");
    }

    return ok;
}

bool StructuredDataIHelperTest::TestMoveRelative() {
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("A.B.C");
    bool ok = cdb.MoveAbsolute("A");
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    if (ok) {
       ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = (sdi.MoveRelative("B.C"));
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = sdi.MoveAbsolute("A");
    }
    if (ok) {
        ok = !sdi.MoveRelative("C");
    }
    if (ok) {
        ok = !sdi.MoveRelative("B.C");
    }

    return ok;
}

bool StructuredDataIHelperTest::TestMoveToChild() {
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("A.B.C");
    cdb.MoveAbsolute("A");
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = (sdi.MoveToChild(0u));
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = !sdi.MoveAbsolute("B");
    }
    if (ok) {
        ok = !sdi.MoveToChild(0);
    }

    return ok;
}

bool StructuredDataIHelperTest::TestCreateAbsolute() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = (sdi.CreateAbsolute("A.B"));
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = sdi.MoveAbsolute("A");
    }
    if (ok) {
        ok = !sdi.MoveAbsolute("B");
    }
    if (ok) {
        ok = !sdi.CreateAbsolute("B.C");
    }

    return ok;
}

bool StructuredDataIHelperTest::TestCreateRelative() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = (sdi.CreateRelative("A.B"));
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = sdi.MoveAbsolute("A");
    }
    if (ok) {
        ok = !sdi.MoveAbsolute("B");
    }
    if (ok) {
        ok = !sdi.CreateRelative("B.C");
    }

    return ok;
}

bool StructuredDataIHelperTest::TestAddToCurrentNode() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = (sdi.CreateRelative("A.B"));
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = sdi.MoveAbsolute("A.B");
    }
    ReferenceT<ConfigurationDatabaseNode> obj1(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (ok) {
        ok = sdi.AddToCurrentNode(obj1);
    }
    if (ok) {
        ok = !sdi.MoveAbsolute("B");
    }
    ReferenceT<ConfigurationDatabaseNode> obj2(GlobalObjectsDatabase::Instance()->GetStandardHeap());
    if (ok) {
        ok = !sdi.AddToCurrentNode(obj2);
    }

    return ok;
}

bool StructuredDataIHelperTest::TestDelete() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = (sdi.CreateRelative("A.B"));
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        ok = (sdi.MoveAbsolute("A"));
    }
    if (ok) {
        ok = sdi.Delete("B");
    }
    if (ok) {
        ok = !sdi.MoveAbsolute("B");
    }
    if (ok) {
        ok = !sdi.Delete("A");
    }

    return ok;
}

bool StructuredDataIHelperTest::TestGetName() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = (sdi.CreateRelative("A.B"));
    }
    if (ok) {
        ok = (sdi.MoveAbsolute("A"));
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        StreamString n = sdi.GetName();
        ok = (n == "A");
    }

    return ok;
}

bool StructuredDataIHelperTest::TestGetChildName() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = (sdi.CreateRelative("A.B"));
    }
    if (ok) {
        ok = (sdi.MoveAbsolute("A"));
    }
    if (ok) {
        ok = !sdi.HasErrors();
    }
    if (ok) {
        StreamString n = sdi.GetChildName(0u);
        ok = (n == "B");
    }

    return ok;
}

bool StructuredDataIHelperTest::TestGetNumberOfChildren() {
    ConfigurationDatabase cdb;
    Reference ref("Object");
    ref->SetName("Test");
    StructuredDataIHelper sdi(cdb, ref);
    bool ok = !sdi.HasErrors();
    if (ok) {
        ok = (sdi.CreateRelative("A.B"));
    }
    if (ok) {
        ok = (sdi.CreateAbsolute("A.C"));
    }
    if (ok) {
        ok = (sdi.MoveAbsolute("A"));
    }
    if (ok) {
        ok = (sdi.GetNumberOfChildren() == 2u);
    }

    return ok;
}

