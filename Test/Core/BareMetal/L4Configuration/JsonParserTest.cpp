/**
 * @file JsonParserTest.cpp
 * @brief Source file for class JsonParserTest
 * @date 10/12/2015
 * @author Giuseppe Ferro
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
 * the class JsonParserTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "JsonParserTest.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

bool JsonParserTest::TestConstructor() {

    StreamString configString = "";
    ConfigurationDatabase database;

    StreamString err;

    JsonParser myParser(configString, database, &err);
    GrammarInfo myGrammar = myParser.GetGrammarInfo();

    bool ok=(StringHelper::Compare(myGrammar.separators, JsonGrammar.separators)==0);

    ok &= (StringHelper::Compare(myGrammar.beginOneLineComment, JsonGrammar.beginOneLineComment)==0);
    ok &= (StringHelper::Compare(myGrammar.beginMultipleLinesComment, JsonGrammar.beginMultipleLinesComment)==0);
    ok &= (StringHelper::Compare(myGrammar.endMultipleLinesComment, JsonGrammar.endMultipleLinesComment)==0);
    ok &= (StringHelper::Compare(&myGrammar.assignment, &JsonGrammar.assignment)==0);
    return ok;
}

bool JsonParserTest::TestGetGrammarInfo() {
    return TestConstructor();
}

bool JsonParserTest::TestParseScalar() {
    ConfigurationDatabase database;
    StreamString errors;
    StreamString configString = "+PID:{\n"
            "Kp:100.5\n"
            "Ki: 2\n"
            "Kd : 5\n"
            "}";

    configString.Seek(0);
    JsonParser myParser(configString, database, &errors);
    if (!myParser.Parse()) {
        return false;
    }

    if (!database.MoveAbsolute("+PID")) {
        return false;
    }
    float32 Kp = 0.0;
    database.Read("Kp", Kp);

    if (Kp != 100.5) {
        return false;
    }

    uint8 Ki = 0;
    database.Read("Ki", Ki);
    if (Ki != 2) {
        return false;
    }

    float32 Kd = 0.0;

    database.Read("Kd", Kd);
    if (Kd != 5.0) {
        return false;
    }

    return true;
}

bool JsonParserTest::TestParseVector() {
    ConfigurationDatabase database;
    StreamString errors;
    StreamString configString = "+PID: {\n"
            "    Gains: [100.5,2,5]\n"
            "}\n"
            "+Process: {\n"
            "    Names: [\"Pendulum\" , \"ChemicalPlant\"]\n"
            "    FDT: {\n"
            "        Num: [ 1 ] ,\n"
            "        Den: [1 2.5 30.25]\n"
            "    }\n"
            "}\n";

    configString.Seek(0);

    JsonParser myParser(configString, database, &errors);
    if (!myParser.Parse()) {
        return false;
    }

    if (!database.MoveRelative("+PID")) {
        return false;
    }

    float32 gains[3];
    if (!database.Read("Gains", gains)) {
        return false;
    }

    bool ok = true;
    ok &= gains[0] == 100.5;
    ok &= gains[1] == 2.0;
    ok &= gains[2] == 5.0;

    if (!ok) {
        return false;
    }

    if (!database.MoveAbsolute("+Process")) {
        return false;
    }

    char8 processName[2][16];
    if (!database.Read("Names", processName)) {
        return false;
    }

    if (StringHelper::Compare(&processName[0][0], "Pendulum") != 0) {
        return false;
    }

    if (StringHelper::Compare(&processName[1][0], "ChemicalPlant") != 0) {
        return false;
    }

    if (!database.MoveAbsolute("+Process.FDT")) {
        return false;
    }

    uint8 num[1];

    if (!database.Read("Num", num)) {
        return false;
    }

    if (num[0] != 1) {
        return false;
    }

    float32 den[3];
    if (!database.Read("Den", den)) {
        return false;
    }
    ok &= den[0] == 1.0;
    ok &= den[1] == 2.5;
    ok &= den[2] == 30.25;

    if (!ok) {
        return false;
    }
    return true;
}

bool JsonParserTest::TestParseMatrix() {

    ConfigurationDatabase database;
    StreamString errors;
    StreamString configString = "+MatrixTest: {\n"
            "    Matrix: [[-100.5, 0xFF -1][2, -5, +7.5]]\n"
            "}\n"
            "+Process: {\n"
            "    FDT: {\n"
            "        A :  [[1 2][3 4]] ,\n"
            "        B :  [[0][-1]]\n"
            "        C :  [[ -0.125 100.5 ]]\n"
            "        D: 0"
            "    }\n"
            "    Names: [[\"Pend} {ulum\"] , [\"ChemicalPlant\"]]\n"
            "}\n";

    configString.Seek(0);
    JsonParser myParser(configString, database, &errors);

    if (!myParser.Parse()) {
        return false;
    }

    database.MoveAbsolute("+MatrixTest");
    float32 matrix[2][3];

    if (!database.Read("Matrix", matrix)) {
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
        return false;
    }

    if (!database.MoveAbsolute("+Process.FDT")) {
        return false;
    }

    uint8 A[2][2];
    if (!database.Read("A", A)) {
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
        return false;
    }

    ok &= B[0][0] == 0;
    ok &= B[1][0] == -1;

    if (!ok) {
        return false;
    }

    float64 C[1][2];
    if (!database.Read("C", C)) {
        return false;
    }

    ok &= C[0][0] == -0.125;
    ok &= C[0][1] == 100.5;

    if (!ok) {
        return false;
    }

    char8 names[2][1][16];
    database.MoveToAncestor(1u);
    if (!database.Read("Names", names)) {
        return false;
    }

    if (StringHelper::Compare(&names[0][0][0], "Pend} {ulum") != 0) {
        return false;
    }
    if (StringHelper::Compare(&names[0][1][0], "ChemicalPlant") != 0) {
        return false;
    }
    return true;
}

bool JsonParserTest::TestNestedBlocks() {

    ConfigurationDatabase database;
    StreamString errors;
    StreamString configString = "block1: {\n"
            "    block2: {block3: {var: 1}}\n"
            "}\n"
            "block4: {\n"
            "    block5: {\n"
            "        var: 2\n"
            "    }\n"
            "    block6: {\n"
            "        var: [3]\n"
            "        block7: {\n"
            "               block8: {\n"
            "                  var: 4\n"
            "               }"
            "        }"
            "    }\n"
            "    var: 5\n"
            "}\n";

    configString.Seek(0);
    JsonParser myParser(configString, database, &errors);

    if (!myParser.Parse()) {
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

bool JsonParserTest::TestCanonical() {
    ConfigurationDatabase database;
    StreamString errors;
    StreamString configString = "{\n+PID:{\n"
            "Kp:100.5\n"
            "Ki: 2\n"
            "Kd : 5\n"
            "}\n}";

    configString.Seek(0);
    JsonParser myParser(configString, database, &errors);
    if (!myParser.Parse()) {
        return false;
    }

    if (!database.MoveAbsolute("+PID")) {
        return false;
    }
    float32 Kp = 0.0;
    database.Read("Kp", Kp);

    if (Kp != 100.5) {
        return false;
    }

    uint8 Ki = 0;
    database.Read("Ki", Ki);
    if (Ki != 2) {
        return false;
    }

    float32 Kd = 0.0;

    database.Read("Kd", Kd);
    if (Kd != 5.0) {
        return false;
    }

    return true;
}

bool JsonParserTest::TestCanonical_Error() {
    ConfigurationDatabase database;
    StreamString errors;
    StreamString configString = "{\n+PID:{\n"
            "Kp:100.5\n"
            "Ki: 2\n"
            "Kd : 5\n"
            "}";

    configString.Seek(0);
    JsonParser myParser(configString, database, &errors);
    if (myParser.Parse()) {
        return false;
    }
    return true;
}


bool JsonParserTest::TestParseErrors(const char8 *configStringIn) {

    StreamString configString = configStringIn;
    configString.Seek(0);
    StreamString errors;
    ConfigurationDatabase database;

    JsonParser myParser(configString, database, &errors);

    bool ret = myParser.Parse();
    return !ret;

}

bool JsonParserTest::TestArrayOfNodes() {
    ConfigurationDatabase database;
    StreamString errors;
    StreamString configString = ""
        "{\n"
        "  \"A\": 1,\n"
        "  \"B\": [\n"
        "    {\n"
        "      \"B\": [\n"
        "        {\n"
        "          \"C\": 1,\n"
        "          \"D\": 2\n"
        "        },\n"
        "        {\n"
        "          \"C\": 3,\n"
        "          \"D\": 4\n"
        "        }\n"
        "      ]\n"
        "    },\n"
        "    {\n"
        "      \"C\": [\n"
        "        {\n"
        "          \"C\": 5,\n"
        "          \"D\": 6\n"
        "        },\n"
        "        {\n"
        "          \"C\": 7,\n"
        "          \"D\": {\n"
        "            \"E\": 8\n"
        "            \"F\": [1, 2, 3, 4]\n"
        "            \"G\": [[1.0, 2], [3.1, 4]]\n"
        "          }\n"
        "        }\n"
        "      ]\n"
        "    }\n"
        "  ],\n"
        "  \"C\": 1\n"
        "}\n";

    configString.Seek(0);
    JsonParser myParser(configString, database, &errors);
    bool ok = myParser.Parse();
    if (ok) {
        ok = database.MoveToRoot();
    }
    if (ok) {
        ok = database.MoveRelative("B[0].B[1]");
    }
    if (ok) {
        ok = database.MoveAbsolute("B[1].C[1].D");
    }

    return ok;
}


