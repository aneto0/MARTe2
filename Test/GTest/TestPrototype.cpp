#include <stdio.h>
#include "ConfigurationDatabase.h"
#include "StreamString.h"
#include "AnyVector.h"


using namespace MARTe;

int main(int argc,
         char **argv) {
    ConfigurationDatabase cdb;
    StreamString ss = "MyControllers.PID1";
    ss.Seek(0);
    StreamString token;
    char c;
    ss.GetToken(token, ".", c);
    printf("token = %s\n", token.Buffer());


    bool ok = cdb.CreateNodes("MyControllers.PID1", false);
    ok &= cdb.Write("Kp", (float32)10.0);
    ok &= cdb.Write("Kd", (float32)2.0);
    ok &= cdb.Write("Ki", (float32)-1.0);

    float32 kp = 0;
    ok &= cdb.Read("Kp", kp);
    printf("kps = %f\n", kp);

    float32 fa[] = {1.2f, 2.0f, 3.0f};
    AnyVector v(fa);
    ok &= cdb.Write("ArrayValues", fa);

    float32 readV[3];
    AnyVector arr(&readV[0]);
    printf("dataPointer = %p\n", readV);
    ok &= cdb.ReadAnyType("ArrayValues", arr);
    printf("[%f][%f][%f]\n", readV[0], readV[1], readV[2]);

    return 0;
}

