/**
 * @file StandardParserTest.cpp
 * @brief Source file for class StandardParserTest
 * @date 09/12/2015
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
 * the class StandardParserTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StandardParserTest.h"
#include "BasicFile.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

bool StandardParserTest::TestConstructor() {

    StreamString configString = "";
    ConfigurationDatabase database;

    StreamString err;

    StandardParser myParser(configString, database, &err);

    return true;

}

bool StandardParserTest::TestParseScalar() {
    ConfigurationDatabase database;
    StreamString errors;
    StreamString configString = "+PID={\n"
            "Kp=100.5\n"
            "Ki=(uint8)2\n"
            "Kd=(float32)5\n"
            "}";

    configString.Seek(0);
    StandardParser myParser(configString, database, &errors);
    if (!myParser.Parse()) {
        return false;
    }

    if (!database.MoveAbsolute("+PID")) {
        printf("\n2\n");
        return false;
    }
    float32 Kp = 0.0;
    database.Read("Kp", Kp);

    if (Kp != 100.5) {
        printf("\n %s Kp=%f\n", errors.Buffer(), Kp);
        return false;
    }

    uint8 Ki = 0;
    database.Read("Ki", Ki);
    if (Ki != 2) {
        printf("\nKi=%d\n", Ki);
        return false;
    }

    float32 Kd = 0.0;

    database.Read("Kd", Kd);
    if (Kd != 5.0) {
        printf("\nKd=%f\n", Kd);
        return false;
    }

    return true;
}

bool StandardParserTest::TestParseVector() {
    ConfigurationDatabase database;
    StreamString errors;
    StreamString configString = "+PID={\n"
            "    Gains={100.5,2,5}\n"
            "}\n"
            "+Process={\n"
            "    Names={\"Pendulum\" , \"ChemicalPlant\"}\n"
            "    FDT={\n"
            "        Num=(uint8){ 1 } ,\n"
            "        Den=(float32){1 2.5 30.25}\n"
            "    }\n"
            "}\n";

    configString.Seek(0);

    StandardParser myParser(configString, database, &errors);
    if (!myParser.Parse()) {
        printf("\nerrors=%s\n", errors.Buffer());
        return false;
    }

    if (!database.MoveRelative("+PID")) {
        printf("\n0\n");
        return false;
    }

    float32 gains[3];
    if (!database.Read("Gains", gains)) {
        printf("\n1\n");
        return false;
    }

    bool ok = true;
    ok &= gains[0] == 100.5;
    ok &= gains[1] == 2.0;
    ok &= gains[2] == 5.0;

    if (!ok) {
        printf("\n2\n");
        return false;
    }

    if (!database.MoveAbsolute("+Process")) {
        printf("\n3\n");
        return false;
    }

    char8 processName[2][16];
    if (!database.Read("Names", processName)) {
        printf("\n4\n");
        return false;
    }

    if (StringHelper::Compare(&processName[0][0], "Pendulum") != 0) {
        printf("\n%s\n", &processName[0][0]);
        printf("\n%s\n", &processName[1][0]);
        printf("\n5\n");
        return false;
    }

    if (StringHelper::Compare(&processName[1][0], "ChemicalPlant") != 0) {
        printf("\n5a\n");
        return false;
    }

    if (!database.MoveAbsolute("+Process.FDT")) {
        printf("\n6\n");
        return false;
    }

    uint8 num[1];

    if (!database.Read("Num", num)) {
        printf("\nFailed here 7\n");
        return false;
    }

    if (num[0] != 1) {
        printf("\n8\n");
        return false;
    }

    float32 den[3];
    if (!database.Read("Den", den)) {
        printf("\n9\n");
        return false;
    }
    ok &= den[0] == 1.0;
    ok &= den[1] == 2.5;
    ok &= den[2] == 30.25;

    if (!ok) {
        printf("\n10\n");
        return false;
    }
    return true;
}

bool StandardParserTest::TestParseMatrix() {

    ConfigurationDatabase database;
    StreamString errors;
    StreamString configString = "+MatrixTest={\n"
            "    Matrix={{-100.5, 0xFF -1}{2, -5, +7.5}}\n"
            "}\n"
            "+Process={\n"
            "    FDT={\n"
            "        A = (uint8){{1 2}{3 4}} ,\n"
            "        B = (int16){{0}{-1}}\n"
            "        C = (float64){{ -0.125 100.5 }}\n"
            "        D=0"
            "    }\n"
            "    Names={{\"Pend} {ulum\"} , {\"ChemicalPlant\"}}\n"
            "}\n";

    configString.Seek(0);
    StandardParser myParser(configString, database, &errors);

    if(!myParser.Parse()){
        return false;
    }

    database.MoveAbsolute("+MatrixTest");
    float32 matrix[2][3];

    if (!database.Read("Matrix", matrix)) {
        printf("\n0\n");
        return false;
    }
    bool ok = true;
    ok &= matrix[0][0] == -100.5;
    ok &= matrix[0][1] == 255.0;
    ok &= matrix[0][2] == -1.0;
    ok &= matrix[1][0] == 2.0;
    ok &= matrix[1][1] == -5.0;
    ok &= matrix[1][2] == 7.5;

    if (!ok) {
        printf("\n1\n");
        return false;
    }

    if (!database.MoveAbsolute("+Process.FDT")) {
        printf("\n2\n");
        return false;
    }

    uint8 A[2][2];
    if (!database.Read("A", A)) {
        printf("\n3\n");
        return false;
    }

    ok &= A[0][0] == 1;
    ok &= A[0][1] == 2;
    ok &= A[1][0] == 3;
    ok &= A[1][1] == 4;

    if (!ok) {
        return false;
    }

    int16 B[2][1];
    if (!database.Read("B", B)) {
        printf("\n4\n");
        return false;
    }

    ok &= B[0][0] == 0;
    ok &= B[1][0] == -1;

    if (!ok) {
        printf("\n5\n");
        return false;
    }

    float64 C[1][2];
    if (!database.Read("C", C)) {
        printf("\n6\n");
        return false;
    }

    ok &= C[0][0] == -0.125;
    ok &= C[0][1] == 100.5;

    if (!ok) {
        printf("\n7\n");
        return false;
    }

    char8 names[2][1][16];
    database.MoveToAncestor(1u);
    if (!database.Read("Names", names)) {
        printf("\n8a\n");
        return false;
    }

    if (StringHelper::Compare(&names[0][0][0], "Pend} {ulum") != 0) {
        printf("\n8 %s\n", &names[0][0][0]);
        return false;
    }
    if (StringHelper::Compare(&names[0][1][0], "ChemicalPlant") != 0) {
        printf("\n9\n");
        return false;
    }
    return true;
}

bool StandardParserTest::TestNestedBlocks() {

    ConfigurationDatabase database;
    StreamString errors;
    StreamString configString = "block1={\n"
            "    block2={block3={var=1}}\n"
            "}\n"
            "block4={\n"
            "    block5={\n"
            "        var=2\n"
            "    }\n"
            "    block6={\n"
            "        var={3}\n"
            "        block7={\n"
            "               block8={\n"
            "                  var=4\n"
            "               }"
            "        }"
            "    }\n"
            "    var=5\n"
            "}\n";

    configString.Seek(0);
    StandardParser myParser(configString, database, &errors);

    if(!myParser.Parse()){
        return false;
    }

    if (!database.MoveAbsolute("block1.block2.block3")) {
        return false;
    }

    int32 var = 0;
    if (!database.Read("var", var)) {
        return false;
    }

    if (var != 1) {
        return false;
    }
    if (!database.MoveAbsolute("block4.block5")) {
        return false;
    }
    if (!database.Read("var", var)) {
        return false;
    }

    if (var != 2) {
        return false;
    }

    if (!database.MoveAbsolute("block4.block6")) {
        return false;
    }
    int32 varVector[1] = { 0 };
    if (!database.Read("var", varVector)) {
        return false;
    }

    if (varVector[0] != 3) {
        return false;
    }

    if (!database.MoveRelative("block7.block8")) {
        return false;
    }
    if (!database.Read("var", var)) {
        return false;
    }

    if (var != 4) {
        return false;
    }

    if (!database.MoveAbsolute("block4")) {
        return false;
    }
    if (!database.Read("var", var)) {
        return false;
    }

    return var == 5;
}

bool StandardParserTest::TestParseErrors(const char8 *configStringIn) {

    StreamString configString = configStringIn;
    configString.Seek(0);
    StreamString errors;
    ConfigurationDatabase database;

    StandardParser myParser(configString, database, &errors);

    bool ret = myParser.Parse();
    printf("\nerrors=%s\n", errors.Buffer());
    return !ret;

}

bool StandardParserTest::TestStandardCast() {
    StreamString configString = "var1= (boh) 1\n";
    configString.Seek(0);
    StreamString errors;
    ConfigurationDatabase database;

    StandardParser myParser(configString, database, &errors);
    if (!myParser.Parse()) {
        return false;
    }

    int32 var = 0;
    database.Read("var1", var);


    return var == 1;
}


bool StandardParserTest::TestExistentFile() {
    BasicFile configurationFile;
    if (!configurationFile.Open("MARTe-WaterTank.cfg", BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W)) {
        printf("\nError! The file is not opened!\n");
    }

    configurationFile.Seek(0);
    StreamString errors;

    ConfigurationDatabase database;
    StandardParser myParser(configurationFile, database, &errors);
    if (!myParser.Parse()) {
        printf("\nerrors=%s\n", errors.Buffer());

        return false;
    }

    return true;
}


