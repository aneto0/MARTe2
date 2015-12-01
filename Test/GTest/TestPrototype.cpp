#include <stdio.h>
#include "ConfigurationDatabase.h"
#include "StreamString.h"
#include "Vector.h"
#include "Matrix.h"

using namespace MARTe;

void TestScalar() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("MyControllers.PID1");
    ok &= cdb.Write("Kp", (float32) 10.0);
    ok &= cdb.Write("Kd", (float32) 2.0);
    ok &= cdb.Write("Ki", (float32) -1.0);

    float32 kp = 0;
    float32 kd = 0;
    float32 ki = 0;
    ok &= cdb.Read("Kp", kp);
    printf("Kp = %f\n", kp);
    ok &= cdb.Read("Kd", kd);
    printf("Kd = %f\n", kd);
    ok &= cdb.Read("Ki", ki);
    printf("Ki = %f\n", ki);
}

void TestVectorNumbersStatic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    float32 floatArrWrite[] = { 1.2f, 2.0f, 3.0f };
    cdb.Write("ArrayValues", floatArrWrite);
    float32 floatArrRead[3];
    cdb.Read("ArrayValues", floatArrRead);
    printf("[%f][%f][%f]\n", floatArrRead[0], floatArrRead[1], floatArrRead[2]);
}

void TestVectorNumbersDynamic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    Vector<float> vecFloatWriteFromArray(3);
    vecFloatWriteFromArray[0] = 1.0f;
    vecFloatWriteFromArray[1] = -1.0f;
    vecFloatWriteFromArray[2] = 2.0f;
    cdb.Write("VectorFromArray", vecFloatWriteFromArray);
    Vector<float> vecFloatReadFromArray(3);
    cdb.Read("VectorFromArray", vecFloatReadFromArray);
    printf("[%f][%f][%f]\n", vecFloatReadFromArray[0], vecFloatReadFromArray[1], vecFloatReadFromArray[2]);
}

void TestVectorNumbersExistentArray() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("MyControllers.PID1");

    float32 *vector = new float32[10];
    Vector<float> vecFloatWriteFromArrayBrackets(vector, 10);
    vecFloatWriteFromArrayBrackets[0] = 1.0f;
    vecFloatWriteFromArrayBrackets[1] = -1.0f;
    vecFloatWriteFromArrayBrackets[2] = 2.0f;
    ok &= cdb.Write("VectorFromArray", vecFloatWriteFromArrayBrackets);
    uint32 nOfElements = cdb.GetType("VectorFromArray").GetNumberOfElements(0u);
    printf("nOfElements = %d\n", nOfElements);
    Vector<float> vecFloatReadFromArrayBrackets(nOfElements);
    ok &= cdb.Read("VectorFromArray", vecFloatReadFromArrayBrackets);
    printf("[%f][%f][%f]\n", vecFloatReadFromArrayBrackets[0], vecFloatReadFromArrayBrackets[1], vecFloatReadFromArrayBrackets[2]);
}

void TestScalarCharStar() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("MyControllers.PID1");
    const char *strWrite = "ThisIsAString";
    ok &= cdb.Write("AString", strWrite);
    char strRead[StringHelper::Length(strWrite)];
    ok &= cdb.Read("AString", strRead);
    printf("[%s]\n", strRead);
}

void TestVectorCharArray() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("MyControllers.PID1");
    char charArrayWrite[7];
    memcpy(charArrayWrite, "456789", 6);
    charArrayWrite[6] = '\0';
    Vector<char> vecCharArrayWrite(charArrayWrite);
    ok &= cdb.Write("CharArray", vecCharArrayWrite);
    char charArrayRead[7];
    Vector<char> vecCharArrayRead(charArrayRead);
    ok &= cdb.Read("CharArray", vecCharArrayRead);
    printf("[%s]\n", &vecCharArrayRead[0]);
}

void TestVectorCharStarStatic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("MyControllers.PID1");

    const char *strArrWrite[2] = { "a", "bb" };
    Vector<const char *> vecStrArrWrite(strArrWrite);
    ok &= cdb.Write("AStringArray", vecStrArrWrite);
    char *strArrRead[2];
    strArrRead[0] = (char *) HeapManager::Malloc(3);
    strArrRead[1] = (char *) HeapManager::Malloc(3);
    Vector<char *> vecStrArrRead(strArrRead);
    ok &= cdb.Read("AStringArray", vecStrArrRead);
    printf("[%s][%s]\n", vecStrArrRead[0], vecStrArrRead[1]);
}

void TestVectorCharStarDynamic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    Vector<const char *> vectorWrite(3);
    vectorWrite[0] = "a";
    vectorWrite[1] = "bb";
    vectorWrite[2] = "ccc";
    cdb.Write("VectorFromArray", vectorWrite);
    Vector<char *> vectorRead(3);
    vectorRead[0] = (char *) HeapManager::Malloc(3);
    vectorRead[1] = (char *) HeapManager::Malloc(3);
    vectorRead[2] = (char *) HeapManager::Malloc(3);
    cdb.Read("VectorFromArray", vectorRead);
    printf("[%s][%s][%s]\n", vectorRead[0], vectorRead[1], vectorRead[2]);
}

void TestVectorCharStarExistentArray() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    const char **arr = new const char*[3];
    arr[0] = "a";
    arr[1] = "bb";
    arr[2] = "ccc";
    Vector<const char *> vectorWrite(arr, 3);
    cdb.Write("VectorFromArray", vectorWrite);
    char **arrRead = new char*[3];
    arrRead[0] = (char *) HeapManager::Malloc(3);
    arrRead[1] = (char *) HeapManager::Malloc(3);
    arrRead[2] = (char *) HeapManager::Malloc(3);
    Vector<char *> vectorRead(arrRead, 3);
    cdb.Read("VectorFromArray", vectorRead);
    printf("[%s][%s][%s]\n", vectorRead[0], vectorRead[1], vectorRead[2]);
    printf("[%s][%s][%s]\n", arrRead[0], arrRead[1], arrRead[2]);
}

void TestScalarString() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("MyControllers.PID1");

    String strWrite = "ThisIsAString";
    ok &= cdb.Write("AString", strWrite);
    String strRead;
    ok &= cdb.Read("AString", strRead);
    printf("[%s]\n", strRead.Buffer());
}

void TestVectorStreamStringStatic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateAbsolute("MyControllers.PID1");

    String strArrWrite[2] = { "a", "bb" };
    Vector<String> vecStrArrWrite(strArrWrite);
    ok &= cdb.Write("AStringArray", vecStrArrWrite);
    String strArrRead[2];
    Vector<String> vecStrArrRead(strArrRead);
    ok &= cdb.Read("AStringArray", vecStrArrRead);
    printf("[%s][%s]\n", vecStrArrRead[0].Buffer(), vecStrArrRead[1].Buffer());
}

void TestVectorStreamStringDynamic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    Vector<String> vectorWrite(3);
    vectorWrite[0] = "a";
    vectorWrite[1] = "bb";
    vectorWrite[2] = "ccc";
    cdb.Write("VectorFromArray", vectorWrite);
    Vector<String> vectorRead(3);
    cdb.Read("VectorFromArray", vectorRead);
    printf("[%s][%s][%s]\n", vectorRead[0].Buffer(), vectorRead[1].Buffer(), vectorRead[2].Buffer());
}

void TestVectorStreamStringExistentArray() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    String *arr = new String[3];
    arr[0] = "a";
    arr[1] = "bb";
    arr[2] = "ccc";
    Vector<String> vectorWrite(arr, 3);
    cdb.Write("VectorFromArray", vectorWrite);
    String *arrRead = new String[3];
    Vector<String> vectorRead(arrRead, 3);
    cdb.Read("VectorFromArray", vectorRead);
    printf("[%s][%s][%s]\n", vectorRead[0].Buffer(), vectorRead[1].Buffer(), vectorRead[2].Buffer());
    printf("[%s][%s][%s]\n", arrRead[0].Buffer(), arrRead[1].Buffer(), arrRead[2].Buffer());
}

void TestMatrixNumbersStatic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    float32 floatTableWrite[2][3] = { { 3.2f, 4.0f, 5.0f }, { -1.2f, -2.0f, -3.0f } };
    //Matrix<float32> matWrite(floatTableWrite);
    cdb.Write("ArrayValues", floatTableWrite);
    float32 floatTableRead[2][3];
    Matrix<float32> matRead(floatTableRead);
    cdb.Read("ArrayValues", matRead);
    printf("[%f][%f][%f]\n", matRead[0][0], matRead[0][1], matRead[0][2]);
    printf("[%f][%f][%f]\n", matRead[1][0], matRead[1][1], matRead[1][2]);

}

void TestMatrixNumbersDynamic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    Matrix<float64> matWrite(2, 3);
    matWrite[0][0] = 1.1;
    matWrite[0][1] = 2.1;
    matWrite[0][2] = 3.1;
    matWrite[1][0] = -1.1;
    matWrite[1][1] = -2.1;
    matWrite[1][2] = -3.1;
    cdb.Write("MatrixFromArray", matWrite);
    matWrite[0][0] = 0.0;
    matWrite[0][1] = 0.0;
    matWrite[0][2] = 0.0;
    matWrite[1][0] = 0.0;
    matWrite[1][1] = 0.0;
    matWrite[1][2] = 0.0;
    Matrix<float64> matRead(2, 3);
    cdb.Read("MatrixFromArray", matRead);
    printf("[%f][%f][%f]\n", matRead[0][0], matRead[0][1], matRead[0][2]);
    printf("[%f][%f][%f]\n", matRead[1][0], matRead[1][1], matRead[1][2]);
}

void TestMatrixExistentArray() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    float32 **arr = new float32*[2];
    arr[0] = new float32[3];
    arr[1] = new float32[3];

    Matrix<float32> matWrite(arr, 2, 3);
    matWrite[0][0] = 1.1;
    matWrite[0][1] = 2.1;
    matWrite[0][2] = 3.1;
    matWrite[1][0] = -1.1;
    matWrite[1][1] = -2.1;
    matWrite[1][2] = -3.1;
    cdb.Write("MatrixFromArray", matWrite);
    matWrite[0][0] = 0.0;
    matWrite[0][1] = 0.0;
    matWrite[0][2] = 0.0;
    matWrite[1][0] = 0.0;
    matWrite[1][1] = 0.0;
    matWrite[1][2] = 0.0;
    float32 **arrRead = new float32*[2];
    arrRead[0] = new float32[3];
    arrRead[1] = new float32[3];
    Matrix<float32> matRead(arrRead, 2, 3);
    cdb.Read("MatrixFromArray", matRead);
    printf("[%f][%f][%f]\n", matRead[0][0], matRead[0][1], matRead[0][2]);
    printf("[%f][%f][%f]\n", matRead[1][0], matRead[1][1], matRead[1][2]);
}

void TestMatrixCharStarStatic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    const char *arr[2][3] = { { "g", "hh", "iii" }, { "jjj", "kk", "l" } };
    Matrix<const char *> matWrite(arr);
    cdb.Write("ArrayValues", matWrite);
    char *arrRead[2][3];
    arrRead[0][0] = (char *) HeapManager::Malloc(3);
    arrRead[0][1] = (char *) HeapManager::Malloc(3);
    arrRead[0][2] = (char *) HeapManager::Malloc(3);
    arrRead[1][0] = (char *) HeapManager::Malloc(3);
    arrRead[1][1] = (char *) HeapManager::Malloc(3);
    arrRead[1][2] = (char *) HeapManager::Malloc(3);
    Matrix<char *> matRead(arrRead);
    cdb.Read("ArrayValues", matRead);
    printf("[%s][%s][%s]\n", matRead[0][0], matRead[0][1], matRead[0][2]);
    printf("[%s][%s][%s]\n", matRead[1][0], matRead[1][1], matRead[1][2]);

}

void TestMatrixCharStarDynamic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    Matrix<char *> matWrite(2, 3);
    matWrite[0][0] = "[0][0]";
    matWrite[0][1] = "[0][1]";
    matWrite[0][2] = "[0][2]";
    matWrite[1][0] = "[1][0]";
    matWrite[1][1] = "[1][1]";
    matWrite[1][2] = "[1][2]";
    printf("[%s][%s][%s]\n", matWrite[0][0], matWrite[0][1], matWrite[0][2]);
    printf("[%s][%s][%s]\n", matWrite[1][0], matWrite[1][1], matWrite[1][2]);
    cdb.Write("MatrixFromArray", matWrite);
    Matrix<char *> matRead(2, 3);
    matRead[0][0] = (char *) HeapManager::Malloc(12);
    matRead[0][1] = (char *) HeapManager::Malloc(12);
    matRead[0][2] = (char *) HeapManager::Malloc(12);
    matRead[1][0] = (char *) HeapManager::Malloc(12);
    matRead[1][1] = (char *) HeapManager::Malloc(12);
    matRead[1][2] = (char *) HeapManager::Malloc(12);
    cdb.Read("MatrixFromArray", matRead);
    printf("[%s][%s][%s]\n", matRead[0][0], matRead[0][1], matRead[0][2]);
    printf("[%s][%s][%s]\n", matRead[1][0], matRead[1][1], matRead[1][2]);
}

void TestMatrixCharStarExistentArray() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    char ***arr = new char**[2];
    arr[0] = new char *[3];
    arr[1] = new char *[3];

    Matrix<char *> matWrite(arr, 2, 3);
    matWrite[0][0] = "[0][0]";
    matWrite[0][1] = "[0][1]";
    matWrite[0][2] = "[0][2]";
    matWrite[1][0] = "[1][0]";
    matWrite[1][1] = "[1][1]";
    matWrite[1][2] = "[1][2]";
    cdb.Write("MatrixFromArray", matWrite);

    char ***arrRead = new char**[2];
    arrRead[0] = new char*[3];
    arrRead[1] = new char*[3];
    Matrix<char *> matRead(arrRead, 2, 3);
    matRead[0][0] = (char *) HeapManager::Malloc(12);
    matRead[0][1] = (char *) HeapManager::Malloc(12);
    matRead[0][2] = (char *) HeapManager::Malloc(12);
    matRead[1][0] = (char *) HeapManager::Malloc(12);
    matRead[1][1] = (char *) HeapManager::Malloc(12);
    matRead[1][2] = (char *) HeapManager::Malloc(12);
    cdb.Read("MatrixFromArray", matRead);
    printf("[%s][%s][%s]\n", matRead[0][0], matRead[0][1], matRead[0][2]);
    printf("[%s][%s][%s]\n", matRead[1][0], matRead[1][1], matRead[1][2]);
}

void TestMatrixStreamStringStatic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    String arr[2][3] = { { "a", "bb", "ccc" }, { "d", "ee", "fff" } };
    Matrix<String> matWrite(arr);
    printf("[%s][%s][%s]\n", matWrite[0][0].Buffer(), matWrite[0][1].Buffer(), matWrite[0][2].Buffer());
    printf("[%s][%s][%s]\n", matWrite[1][0].Buffer(), matWrite[1][1].Buffer(), matWrite[1][2].Buffer());
    cdb.Write("ArrayValues", matWrite);
    String arrRead[2][3];
    Matrix<String> matRead(arrRead);
    cdb.Read("ArrayValues", matRead);

    printf("[%s][%s][%s]\n", matRead[0][0].Buffer(), matRead[0][1].Buffer(), matRead[0][2].Buffer());
    printf("[%s][%s][%s]\n", matRead[1][0].Buffer(), matRead[1][1].Buffer(), matRead[1][2].Buffer());

}

void TestMatrixStreamStringDynamic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    Matrix<String> matWrite(2, 3);
    matWrite[0][0] = "[0][0]";
    matWrite[0][1] = "[0][1]";
    matWrite[0][2] = "[0][2]";
    matWrite[1][0] = "[1][0]";
    matWrite[1][1] = "[1][1]";
    matWrite[1][2] = "[1][2]";
    cdb.Write("MatrixFromArray", matWrite);
    Matrix<String> matRead(2, 3);
    cdb.Read("MatrixFromArray", matRead);
    printf("[%s][%s][%s]\n", matRead[0][0].Buffer(), matRead[0][1].Buffer(), matRead[0][2].Buffer());
    printf("[%s][%s][%s]\n", matRead[1][0].Buffer(), matRead[1][1].Buffer(), matRead[1][2].Buffer());
}

void TestMatrixStreamStringExistentArray() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateAbsolute("MyControllers.PID1");

    String **arr = new String*[2];
    arr[0] = new String[3];
    arr[1] = new String[3];

    Matrix<String> matWrite(arr, 2, 3);
    matWrite[0][0] = "[0][0]";
    matWrite[0][1] = "[0][1]";
    matWrite[0][2] = "[0][2]";
    matWrite[1][0] = "[1][0]";
    matWrite[1][1] = "[1][1]";
    matWrite[1][2] = "[1][2]";
    cdb.Write("MatrixFromArray", matWrite);

    String **arrRead = new String*[2];
    arrRead[0] = new String[3];
    arrRead[1] = new String[3];

    Matrix<String> matRead(arrRead, 2, 3);
    cdb.Read("MatrixFromArray", matRead);
    printf("[%s][%s][%s]\n", matRead[0][0].Buffer(), matRead[0][1].Buffer(), matRead[0][2].Buffer());
    printf("[%s][%s][%s]\n", matRead[1][0].Buffer(), matRead[1][1].Buffer(), matRead[1][2].Buffer());
}

int main(int argc,
         char **argv) {

    TestScalar();
    TestVectorNumbersStatic();
    TestVectorNumbersDynamic();
    TestVectorNumbersExistentArray();
    TestScalarCharStar();
    TestVectorCharStarDynamic();
    TestVectorCharStarStatic();
    TestVectorCharStarExistentArray();
    TestVectorCharArray();
    TestScalarString();
    TestVectorStreamStringStatic();
    TestVectorStreamStringDynamic();
    TestVectorStreamStringExistentArray();
    TestMatrixNumbersStatic();
    uint32 i=0;
    for(i=0; i<1000; i++)
    TestMatrixNumbersDynamic();
    TestMatrixExistentArray();

    TestMatrixCharStarStatic();
    TestMatrixCharStarDynamic();
    TestMatrixCharStarExistentArray();

    TestMatrixStreamStringStatic();
    TestMatrixStreamStringDynamic();
    TestMatrixStreamStringExistentArray();

    return 0;
}

