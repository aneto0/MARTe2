#include <stdio.h>
#include "ConfigurationDatabase.h"
#include "StreamString.h"
#include "Vector.h"
#include "Matrix.h"

using namespace MARTe;

void TestScalar() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateNodes("MyControllers.PID1", false);
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
    cdb.CreateNodes("MyControllers.PID1", false);

    float32 floatArrWrite[] = { 1.2f, 2.0f, 3.0f };
    cdb.Write("ArrayValues", floatArrWrite);
    float32 floatArrRead[3];
    cdb.Read("ArrayValues", floatArrRead);
    printf("[%f][%f][%f]\n", floatArrRead[0], floatArrRead[1], floatArrRead[2]);
}

void TestVectorNumbersDynamic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateNodes("MyControllers.PID1", false);

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
    bool ok = cdb.CreateNodes("MyControllers.PID1", false);

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
    bool ok = cdb.CreateNodes("MyControllers.PID1", false);
    const char *strWrite = "ThisIsAString";
    ok &= cdb.Write("AString", strWrite);
    char strRead[StringHelper::Length(strWrite)];
    ok &= cdb.Read("AString", strRead);
    printf("[%s]\n", strRead);
}

void TestVectorCharArray() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateNodes("MyControllers.PID1", false);
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
    bool ok = cdb.CreateNodes("MyControllers.PID1", false);

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
    cdb.CreateNodes("MyControllers.PID1", false);

    Vector<const char *> vectorWrite(3);
    vectorWrite[0] = "a";
    vectorWrite[1] = "bb";
    vectorWrite[2] = "ccc";
    cdb.Write("VectorFromArray", vectorWrite);
    Vector<char *> vectorRead(3);
    cdb.Read("VectorFromArray", vectorRead);
    printf("[%s][%s][%s]\n", vectorRead[0], vectorRead[1], vectorRead[2]);
}

void TestVectorCharStarExistentArray() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateNodes("MyControllers.PID1", false);

    const char **arr = new const char*[3];
    arr[0] = "a";
    arr[1] = "bb";
    arr[2] = "ccc";
    Vector<const char *> vectorWrite(arr, 3);
    cdb.Write("VectorFromArray", vectorWrite);
    const char **arrRead = new const char*[3];
    Vector<const char *> vectorRead(arrRead, 3);
    cdb.Read("VectorFromArray", vectorRead);
    printf("[%s][%s][%s]\n", vectorRead[0], vectorRead[1], vectorRead[2]);
    printf("[%s][%s][%s]\n", arrRead[0], arrRead[1], arrRead[2]);
}

void TestScalarString() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateNodes("MyControllers.PID1", false);

    StreamString strWrite = "ThisIsAString";
    ok &= cdb.Write("AString", strWrite);
    StreamString strRead;
    ok &= cdb.Read("AString", strRead);
    printf("[%s]\n", strRead.Buffer());
}

void TestVectorStreamStringStatic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    bool ok = cdb.CreateNodes("MyControllers.PID1", false);

    StreamString strArrWrite[2] = { "a", "bb" };
    Vector<StreamString> vecStrArrWrite(strArrWrite);
    ok &= cdb.Write("AStringArray", vecStrArrWrite);
    StreamString strArrRead[2];
    Vector<StreamString> vecStrArrRead(strArrRead);
    ok &= cdb.Read("AStringArray", vecStrArrRead);
    printf("[%s][%s]\n", vecStrArrRead[0].Buffer(), vecStrArrRead[1].Buffer());
}

void TestVectorStreamStringDynamic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateNodes("MyControllers.PID1", false);

    Vector<StreamString> vectorWrite(3);
    vectorWrite[0] = "a";
    vectorWrite[1] = "bb";
    vectorWrite[2] = "ccc";
    cdb.Write("VectorFromArray", vectorWrite);
    Vector<StreamString> vectorRead(3);
    cdb.Read("VectorFromArray", vectorRead);
    printf("[%s][%s][%s]\n", vectorRead[0].Buffer(), vectorRead[1].Buffer(), vectorRead[2].Buffer());
}

void TestVectorStreamStringExistentArray() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateNodes("MyControllers.PID1", false);

    StreamString *arr = new StreamString[3];
    arr[0] = "a";
    arr[1] = "bb";
    arr[2] = "ccc";
    Vector<StreamString> vectorWrite(arr, 3);
    cdb.Write("VectorFromArray", vectorWrite);
    StreamString *arrRead = new StreamString[3];
    Vector<StreamString> vectorRead(arrRead, 3);
    cdb.Read("VectorFromArray", vectorRead);
    printf("[%s][%s][%s]\n", vectorRead[0].Buffer(), vectorRead[1].Buffer(), vectorRead[2].Buffer());
    printf("[%s][%s][%s]\n", arrRead[0].Buffer(), arrRead[1].Buffer(), arrRead[2].Buffer());
}

void TestMatrixNumbersStatic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateNodes("MyControllers.PID1", false);

    float32 floatTableWrite[2][3] = { { 3.2f, 4.0f, 5.0f }, { -1.2f, -2.0f, -3.0f } };
    Matrix<float32> matWrite(floatTableWrite);
    cdb.Write("ArrayValues", matWrite);
    float32 floatTableRead[2][3];
    Matrix<float32> matRead(floatTableRead);
    cdb.Read("ArrayValues", matRead);
    printf("[%f][%f][%f]\n", matRead[0][0], matRead[0][1], matRead[0][2]);
    printf("[%f][%f][%f]\n", matRead[1][0], matRead[1][1], matRead[1][2]);

}

void TestMatrixNumbersDynamic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateNodes("MyControllers.PID1", false);

    Matrix<float32> matWrite(2, 3);
    matWrite[0][0] = 1.1;
    matWrite[0][1] = 2.1;
    matWrite[0][2] = 3.1;
    matWrite[1][0] = -1.1;
    matWrite[1][1] = -2.1;
    matWrite[1][2] = -3.1;
    cdb.Write("MatrixFromArray", matWrite);
    Matrix<float32> matRead(2, 3);
    cdb.Read("MatrixFromArray", matRead);
    printf("[%f][%f][%f]\n", matRead[0][0], matRead[0][1], matRead[0][2]);
    printf("[%f][%f][%f]\n", matRead[1][0], matRead[1][1], matRead[1][2]);
}

void TestMatrixExistentArray() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateNodes("MyControllers.PID1", false);

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
    cdb.CreateNodes("MyControllers.PID1", false);

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
    cdb.CreateNodes("MyControllers.PID1", false);

    Matrix<char *> matWrite(2, 3);
    matWrite[0][0] = "[0][0]";
    matWrite[0][1] = "[0][1]";
    matWrite[0][2] = "[0][2]";
    matWrite[1][0] = "[1][0]";
    matWrite[1][1] = "[1][1]";
    matWrite[1][2] = "[1][2]";
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
    cdb.CreateNodes("MyControllers.PID1", false);

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
    cdb.CreateNodes("MyControllers.PID1", false);

    StreamString arr[2][3] = { { "a", "bb", "ccc" }, { "d", "ee", "fff" } };
    Matrix<StreamString> matWrite(arr);
    cdb.Write("ArrayValues", matWrite);
    StreamString arrRead[2][3];
    Matrix<StreamString> matRead(arrRead);
    cdb.Read("ArrayValues", matRead);

    printf("[%s][%s][%s]\n", matRead[0][0].Buffer(), matRead[0][1].Buffer(), matRead[0][2].Buffer());
    printf("[%s][%s][%s]\n", matRead[1][0].Buffer(), matRead[1][1].Buffer(), matRead[1][2].Buffer());

}

void TestMatrixStreamStringDynamic() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateNodes("MyControllers.PID1", false);

    Matrix<StreamString> matWrite(2, 3);
    matWrite[0][0] = "[0][0]";
    matWrite[0][1] = "[0][1]";
    matWrite[0][2] = "[0][2]";
    matWrite[1][0] = "[1][0]";
    matWrite[1][1] = "[1][1]";
    matWrite[1][2] = "[1][2]";
    cdb.Write("MatrixFromArray", matWrite);
    Matrix<StreamString> matRead(2, 3);
    cdb.Read("MatrixFromArray", matRead);
    printf("[%s][%s][%s]\n", matRead[0][0].Buffer(), matRead[0][1].Buffer(), matRead[0][2].Buffer());
    printf("[%s][%s][%s]\n", matRead[1][0].Buffer(), matRead[1][1].Buffer(), matRead[1][2].Buffer());
}

void TestMatrixStreamStringExistentArray() {
    printf("==== %s ====\n", __FUNCTION__);
    ConfigurationDatabase cdb;
    cdb.CreateNodes("MyControllers.PID1", false);

    StreamString **arr = new StreamString*[2];
    arr[0] = new StreamString[3];
    arr[1] = new StreamString[3];

    Matrix<StreamString> matWrite(arr, 2, 3);
    matWrite[0][0] = "[0][0]";
    matWrite[0][1] = "[0][1]";
    matWrite[0][2] = "[0][2]";
    matWrite[1][0] = "[1][0]";
    matWrite[1][1] = "[1][1]";
    matWrite[1][2] = "[1][2]";
    cdb.Write("MatrixFromArray", matWrite);

    StreamString **arrRead = new StreamString*[2];
    arrRead[0] = new StreamString[3];
    arrRead[1] = new StreamString[3];

    Matrix<StreamString> matRead(arrRead, 2, 3);
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
    TestVectorCharStarStatic();
    TestVectorCharStarDynamic();
    TestVectorCharStarExistentArray();
    TestVectorCharArray();
    TestScalarString();
    TestVectorStreamStringStatic();
    TestVectorStreamStringDynamic();
    TestVectorStreamStringExistentArray();
    TestMatrixNumbersStatic();
    TestMatrixNumbersDynamic();
    TestMatrixExistentArray();

    TestMatrixCharStarStatic();
    TestMatrixCharStarDynamic();
    TestMatrixCharStarExistentArray();

    TestMatrixStreamStringStatic();
    TestMatrixStreamStringDynamic();
    TestMatrixStreamStringExistentArray();

#if 0

    printf("\nTable===========\n");
    float32 floatTableWrite[2][3] = { {3.2f, 4.0f, 5.0f}, {-1.2f, -2.0f, -3.0f}};
    ok &= cdb.Write("TableValues", floatTableWrite);
    float32 floatTableRead[2][3];
    ok &= cdb.Read("TableValues", floatTableRead);
    printf("[%f][%f][%f]\n", floatTableRead[0][0], floatTableRead[0][1], floatTableRead[0][2]);
    printf("[%f][%f][%f]\n", floatTableRead[1][0], floatTableRead[1][1], floatTableRead[1][2]);

    printf("\n*Table===========\n");
    float32 **floatTableWriteStar = reinterpret_cast<float32 **>(floatTableWrite);
    ok &= cdb.Write("TableValuesStar", floatTableWriteStar, 2, 3);
    float32 **floatTableReadStart = reinterpret_cast<float32 **>(floatTableRead);
    ok &= cdb.Read("TableValuesStar", floatTableReadStart, 2, 3);
    printf("[%f][%f][%f]\n", floatTableRead[0][0], floatTableRead[0][1], floatTableRead[0][2]);
    printf("[%f][%f][%f]\n", floatTableRead[1][0], floatTableRead[1][1], floatTableRead[1][2]);

    printf("\nStreamString===========\n");
    StreamString writeStreamString = "ABCDEF";
    ok &= cdb.Write("StreamString", writeStreamString);
    StreamString readStreamString;
    ok &= cdb.Read("StreamString", readStreamString);
    readStreamString.Seek(0);
    printf("[%s]\n", readStreamString.Buffer());

    printf("\nStreamString[]===========\n");
    StreamString writeStreamStringArray[2];
    writeStreamStringArray[0] = "1ABCDEF2";
    writeStreamStringArray[1] = "2FEDCBA1";
    Vector<StreamString> vecWriteStreamStringArray(writeStreamStringArray);
    ok &= cdb.Write("StreamStringArray", vecWriteStreamStringArray);
    StreamString readStreamStringArray[2];
    Vector<StreamString> vecReadStreamStringArray(readStreamStringArray);
    ok &= cdb.Read("StreamStringArray", vecReadStreamStringArray);

    printf("[%s][%s]\n", vecReadStreamStringArray[0].Buffer(), vecReadStreamStringArray[1].Buffer());

    printf("\n*StreamString===========\n");
    StreamString *writeStreamStringArrayStar = new StreamString[2];
    writeStreamStringArrayStar[0] = "-1ABCDEF2-";
    writeStreamStringArrayStar[1] = "-2FEDCBA1-";
    Vector<StreamString> writeStreamStringArrayStarAT(writeStreamStringArrayStar, 2);

    ok &= cdb.Write("StreamStringArray2", writeStreamStringArrayStarAT);

    Vector<StreamString> readStreamStringArrayStarAT(2);
    ok &= cdb.Read("StreamStringArray2", readStreamStringArrayStarAT);

    printf("[%s][%s]\n", readStreamStringArrayStarAT[0].Buffer(), readStreamStringArrayStarAT[1].Buffer());

    printf("\nStreamString[][]===========\n");
    StreamString writeStreamStringTable[2][3];
    writeStreamStringTable[0][0] = "[0][0]";
    writeStreamStringTable[0][1] = "[0][1]";
    writeStreamStringTable[0][2] = "[0][2]";
    writeStreamStringTable[1][0] = "[1][0]";
    writeStreamStringTable[1][1] = "[1][1]";
    writeStreamStringTable[1][2] = "[1][2]";

    ok &= cdb.Write("StreamStringTable", writeStreamStringTable);
    Matrix<StreamString> readStreamStringTable(2, 3);
    ok &= cdb.Read("StreamStringTable", readStreamStringTable);
    printf("[%s][%s][%s]\n", readStreamStringTable[0][0].Buffer(), readStreamStringTable[0][1].Buffer(), readStreamStringTable[0][2].Buffer());
    printf("[%s][%s][%s]\n", readStreamStringTable[1][0].Buffer(), readStreamStringTable[1][1].Buffer(), readStreamStringTable[1][2].Buffer());

    printf("\nStreamString[][] 2===========\n");
    Matrix<StreamString> readStreamStringTableStatic(writeStreamStringTable);
    ok &= cdb.Read("StreamStringTable", readStreamStringTableStatic);
    printf("[%s][%s][%s]\n", readStreamStringTableStatic[0][0].Buffer(), readStreamStringTableStatic[0][1].Buffer(),
            readStreamStringTableStatic[0][2].Buffer());
    printf("[%s][%s][%s]\n", readStreamStringTableStatic[1][0].Buffer(), readStreamStringTableStatic[1][1].Buffer(),
            readStreamStringTableStatic[1][2].Buffer());

    printf("\nStreamString 2 String===========\n");
    StreamString writeStreamStringToString = "ABCDEF";
    ok &= cdb.Write("StreamStringToString", writeStreamString);
    char readStreamStringToString[20];
    ok &= cdb.Read("StreamStringToString", (char *) readStreamStringToString);
    printf("[%s]\n", readStreamStringToString);

    printf("\nString 2 nStreamString===========\n");
    const char *writeStringToStreamString = "#ABCDEF$";
    ok &= cdb.Write("StringToStreamString", writeStringToStreamString);
    StreamString readStringToStreamString;
    ok &= cdb.Read("StringToStreamString", readStringToStreamString);
    readStringToStreamString.Seek(0);
    printf("[%s]\n", readStringToStreamString.Buffer());

    printf("\nVector===========\n");
    Vector<float> vecFloatWrite(3);
    vecFloatWrite[0] = 1.0f;
    vecFloatWrite[1] = -1.0f;
    vecFloatWrite[2] = 2.0f;
    ok &= cdb.Write("Vector", vecFloatWrite);
    Vector<float> vecFloatRead(3);
    ok &= cdb.Read("Vector", vecFloatRead);
    printf("[%f][%f][%f]\n", vecFloatRead[0], vecFloatRead[1], vecFloatRead[2]);

    printf("\nMatrix===========\n");
    Matrix<float> matFloatWrite(3, 2);
    matFloatWrite[0][0] = 1.0f;
    matFloatWrite[0][1] = -1.0f;
    matFloatWrite[1][0] = 2.0f;
    matFloatWrite[1][1] = -2.0f;
    matFloatWrite[2][0] = 3.0f;
    matFloatWrite[2][1] = -3.0f;
    ok &= cdb.Write("Matrix", matFloatWrite);
    Matrix<float> matFloatRead(3, 2);
    ok &= cdb.Read("Matrix", matFloatRead);
    printf("[%f][%f]\n", matFloatRead[0][0], matFloatRead[0][1]);
    printf("[%f][%f]\n", matFloatRead[1][0], matFloatRead[1][1]);
    printf("[%f][%f]\n", matFloatRead[2][0], matFloatRead[2][1]);

    printf("\nMatrix from array===========\n");
    Matrix<float> matFloatWriteFromArray(floatTableWrite);
    ok &= cdb.Write("MatrixFromArray", matFloatWriteFromArray);
    Matrix<float> matFloatReadFromArray(floatTableRead);
    ok &= cdb.Read("MatrixFromArray", matFloatReadFromArray);
    printf("[%f][%f][%f]\n", matFloatReadFromArray[0][0], matFloatReadFromArray[0][1], matFloatReadFromArray[0][2]);
    printf("[%f][%f][%f]\n", matFloatReadFromArray[1][0], matFloatReadFromArray[1][1], matFloatReadFromArray[1][2]);
#endif
    return 0;
}

