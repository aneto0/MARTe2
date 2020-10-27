/**
 * @file AnyObjectTest.cpp
 * @brief Source file for class AnyObjectTest
 * @date 21/11/2015
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
 * the class AnyObjectTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AnyObjectTest.h"
#include "StringHelper.h"
#include "StreamString.h"
#include "Reference.h"
using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool AnyObjectTest::TestDefaultConstructor() {
    AnyObject anyObj;
    return (anyObj.GetType().GetDataPointer() == NULL);
}

bool AnyObjectTest::TestGetType_Scalar_String() {
    MARTe::AnyObject anyObj;
    StreamString s = "ThisIsAString";
    bool ok = anyObj.Serialise(s);
    ok = (anyObj.GetType().GetTypeDescriptor().type == BT_CCString);
    return ok;
}

bool AnyObjectTest::TestSerialise_ScalarCharStar() {
    const char8 *strWrite = "ThisIsAString";
    AnyObject anyObj;
    bool ok = anyObj.Serialise(strWrite);
    ok &= (StringHelper::Compare(strWrite, static_cast<char8 *>(anyObj.GetType().GetDataPointer())) == 0);
    return ok;
}

bool AnyObjectTest::TestSerialise_StaticCharArray() {
    const uint32 nOfElements = 128;
    char8 charArrayWrite[nOfElements];
    uint32 i;
    for (i = 0; i < nOfElements; i++) {
        charArrayWrite[i] = 'A' + i;
    }
    charArrayWrite[nOfElements - 1] = '\0';
    AnyObject anyObj;
    bool ok = anyObj.Serialise(charArrayWrite);

    ok = (StringHelper::Compare(&charArrayWrite[0], static_cast<char8 *>(anyObj.GetType().GetDataPointer())) == 0);
    return ok;
}

bool AnyObjectTest::TestSerialise_VectorChar() {
    const uint32 nOfElements = 128;
    char8 charArrayWrite[nOfElements];
    uint32 i;
    for (i = 0; i < nOfElements; i++) {
        charArrayWrite[i] = 'A' + i;
    }
    charArrayWrite[nOfElements - 1] = '\0';
    Vector<char8> vecCharArrayWrite(charArrayWrite);
    AnyObject anyObj;
    bool ok = anyObj.Serialise(vecCharArrayWrite);

    ok = (StringHelper::Compare(&charArrayWrite[0], static_cast<char8 *>(anyObj.GetType().GetDataPointer())) == 0);
    return ok;
}

bool AnyObjectTest::TestSerialise_StaticArrayCharStar() {
    const char8 *strArrWrite[2] = { "a", "bb" };
    AnyObject anyObj;
    bool ok = anyObj.Serialise(strArrWrite);

    char8 **readStr = static_cast<char8 **>(anyObj.GetType().GetDataPointer());
    ok &= (StringHelper::Compare(readStr[0], strArrWrite[0]) == 0);
    ok &= (StringHelper::Compare(readStr[1], strArrWrite[1]) == 0);

    return ok;
}

bool AnyObjectTest::TestSerialise_StaticMatrixChar() {
    char8 strArrWrite[2][3] = { { 'a', 'a', 0 }, { 'b', 'b', 0 } };
    AnyObject anyObj;
    bool ok = anyObj.Serialise(strArrWrite);

    char8 *readStr = static_cast<char8 *>(anyObj.GetType().GetDataPointer());

    ok &= (StringHelper::Compare(&(readStr[0]), &(strArrWrite[0][0])) == 0);
    ok &= (StringHelper::Compare(&(readStr[3]), &(strArrWrite[1][0])) == 0);

    return ok;
}

bool AnyObjectTest::TestSerialise_StaticMeshChar() {
    char8 strArrWrite[2][3][4] = { { { 'a', 'a', 'a', 0 }, { 'b', 'b', 'b', 0 }, { 'c', 'c', 'c', 0 } }, { { 'd', 'd', 'd', 0 }, { 'e', 'e', 'e', 0 }, { 'f',
            'f', 0, 0 } } };
    AnyObject anyObj;
    bool ok = anyObj.Serialise(strArrWrite);

    char8 *readStr = static_cast<char8 *>(anyObj.GetType().GetDataPointer());

    ok &= (StringHelper::Compare(&(readStr[0]), &(strArrWrite[0][0][0])) == 0);
    ok &= (StringHelper::Compare(&(readStr[4]), &(strArrWrite[0][1][0])) == 0);
    ok &= (StringHelper::Compare(&(readStr[8]), &(strArrWrite[0][2][0])) == 0);
    ok &= (StringHelper::Compare(&(readStr[12]), &(strArrWrite[1][0][0])) == 0);
    ok &= (StringHelper::Compare(&(readStr[16]), &(strArrWrite[1][1][0])) == 0);
    ok &= (StringHelper::Compare(&(readStr[20]), &(strArrWrite[1][2][0])) == 0);

    return ok;
}

bool AnyObjectTest::TestSerialise_ScalarString() {
    StreamString strWrite = "ThisIsAString";
    AnyObject anyObj;
    bool ok = anyObj.Serialise(strWrite);
    ok &= (StringHelper::Compare(strWrite.Buffer(), static_cast<char8 *>(anyObj.GetType().GetDataPointer())) == 0);
    return ok;
}

bool AnyObjectTest::TestSerialise_StaticStringArray() {
    StreamString strArrWrite[2] = { "a", "bb" };
    AnyObject anyObj;
    bool ok = anyObj.Serialise(strArrWrite);
    char8 **readStr = static_cast<char8 **>(anyObj.GetType().GetDataPointer());
    ok &= (StringHelper::Compare(readStr[0], strArrWrite[0].Buffer()) == 0);
    ok &= (StringHelper::Compare(readStr[1], strArrWrite[1].Buffer()) == 0);
    return ok;
}

bool AnyObjectTest::TestSerialise_VectorString() {
    AnyObject anyObj;
    Vector<StreamString> vecArrWrite(3);
    vecArrWrite[0] = "a";
    vecArrWrite[1] = "bb";
    vecArrWrite[2] = "ccc";
    bool ok = anyObj.Serialise(vecArrWrite);
    char8 **readStr = static_cast<char8 **>(anyObj.GetType().GetDataPointer());
    ok &= (StringHelper::Compare(readStr[0], vecArrWrite[0].Buffer()) == 0);
    ok &= (StringHelper::Compare(readStr[1], vecArrWrite[1].Buffer()) == 0);
    ok &= (StringHelper::Compare(readStr[2], vecArrWrite[2].Buffer()) == 0);
    return ok;
}

bool AnyObjectTest::TestSerialise_StaticMatrixCharStar() {
    const char8 *strArrWrite[3][2] = { { "a", "bb" }, { "ccc", "dd" }, { "eeeeee", "ff" } };
    AnyObject anyObj;
    bool ok = anyObj.Serialise(strArrWrite);

    uint32 i = 0;
    uint32 j = 0;
    uint32 nOfRows = 3;
    uint32 nOfColumns = 2;
    for (i = 0; ok && (i < nOfRows); i++) {
        char8 ** beginMemory = reinterpret_cast<char8 **>(anyObj.GetType().GetDataPointer());
        MARTe::Vector<char8 *> vec = MARTe::Vector<char8 *>(&beginMemory[i * nOfColumns], nOfColumns);
        for (j = 0; j < nOfColumns; j++) {
            ok = (StringHelper::Compare(vec[j], strArrWrite[i][j]) == 0);
        }

    }

    return ok;
}

bool AnyObjectTest::TestSerialise_MatrixCharStar() {
    uint32 i = 0;
    uint32 j = 0;
    uint32 nOfRows = 3;
    uint32 nOfColumns = 2;
    const char8 *strArrWrite[3][2] = { { "a", "bb" }, { "ccc", "dd" }, { "eeeeee", "ff" } };

    Matrix<const char8 *> mat(nOfRows, nOfColumns);
    for (i = 0; i < nOfRows; i++) {
        for (j = 0; j < nOfColumns; j++) {
            mat[i][j] = strArrWrite[i][j];
        }
    }

    AnyObject anyObj;
    bool ok = anyObj.Serialise(mat);
    Matrix<char8 *> testMat(reinterpret_cast<char8 ***>(anyObj.GetType().GetDataPointer()), nOfRows, nOfColumns);

    for (i = 0; ok && (i < nOfRows); i++) {
        for (j = 0; ok && (j < nOfColumns); j++) {
            ok = (StringHelper::Compare(mat[i][j], testMat[i][j]) == 0);
        }
    }

    return ok;
}

bool AnyObjectTest::TestSerialise_StaticMatrixString() {
    StreamString strArrWrite[3][2] = { { "a", "bb" }, { "ccc", "dd" }, { "eeeeee", "ff" } };
    AnyObject anyObj;
    bool ok = anyObj.Serialise(strArrWrite);

    uint32 i = 0;
    uint32 j = 0;
    uint32 nOfRows = 3;
    uint32 nOfColumns = 2;
    for (i = 0; ok && (i < nOfRows); i++) {
        char8 ** beginMemory = reinterpret_cast<char8 **>(anyObj.GetType().GetDataPointer());
        MARTe::Vector<char8 *> vec = MARTe::Vector<char8 *>(&beginMemory[i * nOfColumns], nOfColumns);
        for (j = 0; j < nOfColumns; j++) {
            ok = (StringHelper::Compare(vec[j], strArrWrite[i][j].Buffer()) == 0);
        }

    }

    return ok;
}

bool AnyObjectTest::TestSerialise_MatrixString() {
    uint32 i = 0;
    uint32 j = 0;
    uint32 nOfRows = 3;
    uint32 nOfColumns = 2;
    const char8 *strArrWrite[3][2] = { { "a", "bb" }, { "ccc", "dd" }, { "eeeeee", "ff" } };

    Matrix<StreamString> mat(nOfRows, nOfColumns);
    for (i = 0; i < nOfRows; i++) {
        for (j = 0; j < nOfColumns; j++) {
            mat[i][j] = strArrWrite[i][j];
        }
    }

    AnyObject anyObj;
    bool ok = anyObj.Serialise(mat);
    Matrix<char8 *> testMat(reinterpret_cast<char8 ***>(anyObj.GetType().GetDataPointer()), nOfRows, nOfColumns);

    for (i = 0; ok && (i < nOfRows); i++) {
        for (j = 0; ok && (j < nOfColumns); j++) {
            ok = (StringHelper::Compare(mat[i][j].Buffer(), testMat[i][j]) == 0);
        }
    }

    return ok;
}

bool AnyObjectTest::TestSerialise_StaticTensor() {
    uint32 nOfRows = 2;
    uint32 nOfColumns = 3;
    uint32 nOfPages = 3;
    const uint32 intArrWrite[3][2][3] = { { {1 , 2 , 3 }, {4 , 5 , 6 } },
                                          { {7 , 8 , 9 }, {10, 11, 12} },
                                          { {13, 14, 15}, {16, 17, 18} }
                                        };
    AnyType at(UnsignedInteger32Bit, 0u, intArrWrite);
    at.SetNumberOfDimensions(3u);
    at.SetNumberOfElements(0u, nOfRows);
    at.SetNumberOfElements(1u, nOfColumns);
    at.SetNumberOfElements(2u, nOfPages);
    
    AnyObject anyObj;
    bool ok = anyObj.Serialise(at);
    uint32* testTensor = reinterpret_cast<uint32*>(anyObj.GetType().GetDataPointer());
    for (uint32 k = 0; ok && (k < nOfPages); k++) {
        for (uint32 i = 0; ok && (i < nOfRows); i++) {
            for (uint32 j = 0; ok && (j < nOfColumns); j++) {
                ok = (MemoryOperationsHelper::Compare( testTensor + (j + nOfColumns*(i + nOfRows*k)) , (void*) &intArrWrite[k][i][j], 4u) == 0u);
            }
        }
    }

    return ok;
}

bool AnyObjectTest::TestGetClassProperties() {
    AnyObject anyObj;
    const ClassProperties *classProps = anyObj.GetClassProperties();
    return (StringHelper::Compare("AnyObject", classProps->GetName()) == 0);
}

//bool AnyObjectTest::TestGetClassPropertiesCopy() {
//    AnyObject anyObj;
//    ClassProperties classProps;
//    anyObj.GetClassPropertiesCopy(classProps);
//    return (StringHelper::Compare("AnyObject", classProps.GetName()) == 0);
//}

bool AnyObjectTest::TestGetObjectBuildFunction() {
    Reference ref("AnyObject");
    return ref.IsValid();
}

bool AnyObjectTest::TestCleanUp() {

    AnyObject anyObj;
    if (anyObj.GetType().GetDataPointer() != NULL) {
        return false;
    }

    int32 element = 4;
    anyObj.Serialise(element);

    int32 *serialized = (int32*) (anyObj.GetType().GetDataPointer());
    if (serialized != NULL) {
        if(serialized[0]!=4) {
            return false;
        }
    }
    else {
        return false;
    }

    anyObj.CleanUp();

    return anyObj.GetType().GetDataPointer() == NULL;
}

