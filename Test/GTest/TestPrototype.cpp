#include <stdio.h>
#include "ConfigurationDatabase.h"
#include "StreamString.h"
//#include "AnyVector.h"

using namespace MARTe;

int main(int argc,
         char **argv) {
    ConfigurationDatabase cdb;
    StreamString ss = "MyControllers.PID1";
    ss.Seek(0);

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

    printf("\nArrays===========\n");
    float32 floatArrWrite[] = { 1.2f, 2.0f, 3.0f };
    ok &= cdb.Write("ArrayValues", floatArrWrite);
    float32 floatArrRead[3];
    ok &= cdb.Read("ArrayValues", floatArrRead);
    printf("[%f][%f][%f]\n", floatArrRead[0], floatArrRead[1], floatArrRead[2]);

    printf("\nString===========\n");
    const char *strWrite = "ThisIsAString";
    ok &= cdb.Write("AString", strWrite);
    char strRead[StringHelper::Length(strWrite)];
    ok &= cdb.Read("AString", strRead);
    printf("[%s]\n", strRead);

    printf("\nString array===========\n");
    const char *strArrWrite[] = {"a", "bb"};
    ok &= cdb.Write("AStringArray", strArrWrite);

    char *strArrRead[2];
    strArrRead[0] = (char *)HeapManager::Malloc(3);
    strArrRead[1] = (char *)HeapManager::Malloc(3);
    AnyType anyTypeStrArrRead(&strArrRead[0], 2);
    ok &= cdb.Read("AStringArray", anyTypeStrArrRead);
    printf("[%s][%s]\n", strArrRead[0], strArrRead[1]);

    printf("\nTable===========\n");
    float32 floatTableWrite[2][3] = { {3.2f, 4.0f, 5.0f},  {-1.2f, -2.0f, -3.0f}};
    ok &= cdb.Write("TableValues", floatTableWrite);
    float32 floatTableRead[2][3];
    ok &= cdb.Read("TableValues", floatTableRead);
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
    ok &= cdb.Write("StreamStringArray", writeStreamStringArray);
    StreamString readStreamStringArray[2];
    ok &= cdb.Read("StreamStringArray", readStreamStringArray);

    printf("[%s][%s]\n", readStreamStringArray[0].Buffer(), readStreamStringArray[1].Buffer());

    return 0;
}

