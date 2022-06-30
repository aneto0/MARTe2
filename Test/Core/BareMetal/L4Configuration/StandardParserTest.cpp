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

    GrammarInfo myGrammar=myParser.GetGrammarInfo();

    bool ok=(StringHelper::Compare(myGrammar.separators, StandardGrammar.separators)==0);

    ok &= (StringHelper::Compare(myGrammar.beginOneLineComment, StandardGrammar.beginOneLineComment)==0);
    ok &= (StringHelper::Compare(myGrammar.beginMultipleLinesComment, StandardGrammar.beginMultipleLinesComment)==0);
    ok &= (StringHelper::Compare(myGrammar.endMultipleLinesComment, StandardGrammar.endMultipleLinesComment)==0);
    ok &= (StringHelper::Compare(&myGrammar.assignment, &StandardGrammar.assignment)==0);
    return ok;
}

bool StandardParserTest::TestGetGrammarInfo() {
    return TestConstructor();
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

bool StandardParserTest::TestParseString() {
    ConfigurationDatabase database;
    StreamString errors;
    StreamString configString = "+Test={\n"
            "Str1=\"TEST_STRING\"\n"
            "Str2=\"TEST STRING\"\n"
            "}";

    configString.Seek(0);
    StandardParser myParser(configString, database, &errors);
    bool ok = myParser.Parse();
    if (ok) {
        ok = (database.MoveAbsolute("+Test"));
    }
    StreamString str1;
    if (ok) {
        ok = database.Read("Str1", str1);
    }
    if (ok) {
        ok = (str1 == "TEST_STRING");
    }
    StreamString str2;
    if (ok) {
        ok = database.Read("Str2", str2);
    }
    if (ok) {
        ok = (str2 == "TEST STRING");
    }
    return ok;
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

bool StandardParserTest::TestParseErrors(const char8 *configStringIn) {

    StreamString configString = configStringIn;
    configString.Seek(0);
    StreamString errors;
    ConfigurationDatabase database;

    StandardParser myParser(configString, database, &errors);

    bool ret = myParser.Parse();
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

bool StandardParserTest::TestGetExprCast() {
    const char8 * const config = ""
        "vars = {\n"
        "  extra = {\n"
        "    var1 = (uint32)3\n"
        "  }\n"
        "  var2 = (uint32|\"(uint32)2\")\n"
        "}\n"
        "var3 = (uint32|\"vars.extra.var1 * (uint32)3 * vars.var2\")\n";

    StreamString configString = config;
    configString.Seek(0);
    StreamString errors;
    ConfigurationDatabase database;

    StandardParser myParser(configString, database, &errors);
    if (!myParser.Parse()) {
        return false;
    }
    database.MoveToRoot();
    uint32 var1 = 0;
    uint32 var2 = 0;
    uint32 var3 = 0;
    bool ok = database.MoveAbsolute("vars.extra");
    ok &= database.Read("var1", var1);
    ok &= database.MoveAbsolute("vars");
    ok &= database.Read("var2", var2);
    ok &= database.MoveToRoot();
    ok &= database.Read("var3", var3);
    ok &= (var1 == 3);
    ok &= (var2 == 2);
    ok &= (var3 == 18);
    return ok;
}

bool StandardParserTest::TestGetExprCast_InvalidExpression() {
    const char8 * const config = ""
        "vars = {\n"
        "  var1 = (uint32)3\n"
        "  var2 = (uint32|\"(uint64)2\")\n"
        "}\n"
        "var3 = (uint32|\"vars.var1 * (uint32 3 * vars.var2\")\n";

    StreamString configString = config;
    configString.Seek(0);
    StreamString errors;
    ConfigurationDatabase database;

    StandardParser myParser(configString, database, &errors);
    bool ok = !myParser.Parse();
    return ok;
}

bool StandardParserTest::TestGetExprCast_InvalidOutput() {
    const char8 * const config = ""
        "vars = {\n"
        "  var1 = (uint32)3\n"
        "  var2 = (uint32|\"(uint64)2\")\n"
        "}\n"
        "var3 = (wrong|\"vars.var1 * (uint32)3 * vars.var2\")\n";

    StreamString configString = config;
    configString.Seek(0);
    StreamString errors;
    ConfigurationDatabase database;

    StandardParser myParser(configString, database, &errors);
    bool ok = !myParser.Parse();
    return ok;
}

bool StandardParserTest::TestGetExprCast_FailCompile() {
    const char8 * const config = ""
        "vars = {\n"
        "  var1 = (uint32)3\n"
        "  var2 = (uint32|\"(uint64)2\")\n"
        "}\n"
        "var3 = (float32|\"vars.var1 * (uint32)3 * vars.var2\")\n";

    StreamString configString = config;
    configString.Seek(0);
    StreamString errors;
    ConfigurationDatabase database;

    StandardParser myParser(configString, database, &errors);
    bool ok = !myParser.Parse();
    return ok;
}

bool StandardParserTest::TestGetExprCast_FailVariableNotFound() {
    const char8 * const config = ""
        "vars = {\n"
        "  var1 = (uint32)3\n"
        "  var2 = (uint32|\"(uint64)2\")\n"
        "}\n"
        "var3 = (uint32|\"var4 + vars.var1 * (uint32)3 * vars.var2\")\n";

    StreamString configString = config;
    configString.Seek(0);
    StreamString errors;
    ConfigurationDatabase database;

    StandardParser myParser(configString, database, &errors);
    bool ok = !myParser.Parse();
    return ok;
}

bool StandardParserTest::TestGetExprCast_FailVariablePathNotFound() {
    const char8 * const config = ""
        "vars = {\n"
        "  var1 = (uint32)3\n"
        "  var2 = (uint32|\"(uint64)2\")\n"
        "}\n"
        "var3 = (uint32|\"var.var1 * (uint32)3 * vars.var2\")\n";

    StreamString configString = config;
    configString.Seek(0);
    StreamString errors;
    ConfigurationDatabase database;

    StandardParser myParser(configString, database, &errors);
    bool ok = !myParser.Parse();
    return ok;
}

