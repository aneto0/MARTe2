/**
 * @file SlkActionTest.cpp
 * @brief Source file for class SlkActionTest
 * @date 07/12/2015
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
 * the class SlkActionTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SlkActionTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool SlkActionTest::TestConstructor() {

    StreamString configString = "";
    ConfigurationDatabase database;

    const char8* terminals = "";
    const char8* separators = "";

    StreamString err = "";

    SlkAction myParser(configString, database, terminals, separators, &err);

    return true;

}

bool SlkActionTest::TestParseScalarStandardGrammar() {
    StreamString configString = "block={\n"
            "                           var=1\n"
            "                           }";

    configString.Seek(0);
    ConfigurationDatabase database;

    const char8* terminals = "={}()";

    const char8* separators = "\n\r\t, ";

    StreamString err = "";

    SlkAction myParser(configString, database, terminals, separators, &err);

    myParser.Parse();

    if (!database.MoveAbsolute("block")) {
        return false;
    }

    int32 var = 0;
    database.Read("var", var);

    printf("\n%d\n", var);
    return var == 1;
}

bool SlkActionTest::TestParseVectorStandardGrammar() {
    StreamString configString = "block={\n"
            "                           var={1 2 3}\n"
            "                           }";

    configString.Seek(0);
    ConfigurationDatabase database;

    const char8* terminals = "={}()";
    const char8* separators = "\n\r\t, ";

    StreamString err = "";

    SlkAction myParser(configString, database, terminals, separators, &err);

    myParser.Parse();

    if (!database.MoveAbsolute("block")) {
        return false;
    }

    int32 var[3] = { 0 };
    database.Read("var", var);

    printf("\n%d\n", var);
    bool ok = var[0] == 1;
    ok = var[1] == 2;
    ok = var[2] == 3;

    return ok;
}

bool SlkActionTest::TestParseMatrixStandardGrammar() {

    StreamString configString = "block={\n"
            "                           var={ {1 2}{3,4} }\n"
            "                           }";

    configString.Seek(0);
    ConfigurationDatabase database;

    const char8* terminals = "={}()";
    const char8* separators = "\n\r\t, ";

    StreamString err = "";

    SlkAction myParser(configString, database, terminals, separators, &err);

    myParser.Parse();

    if (!database.MoveAbsolute("block")) {
        return false;
    }

    int32 var[2][2] = { { 0 } };
    database.Read("var", var);

    printf("\n%d\n", var);
    bool ok = var[0][0] == 1;
    ok = var[0][1] == 2;
    ok = var[1][0] == 3;
    ok = var[1][1] == 4;

    return ok;
}

bool SlkActionTest::TestParseScalarXMLGrammar() {
    StreamString configString = "<block>\n"
            "                           <var>1</var>\n"
            "                           </block>";

    configString.Seek(0);
    ConfigurationDatabase database;

    const char8* terminals = "<>/()";
    const char8* separators = "\n\r\t, ";

    StreamString err = "";

    SlkAction myParser(configString, database, terminals, separators, &err);

    myParser.Parse();

    if (!database.MoveAbsolute("block")) {
        return false;
    }

    int32 var = 0;
    database.Read("var", var);

    printf("\n%d\n", var);
    return var == 1;
}

bool SlkActionTest::TestParseVectorXMLGrammar() {

    StreamString configString = "<block>\n"
            "                           <var><> 1 2 3 </></var>\n"
            "                           </block>";
    configString.Seek(0);
    ConfigurationDatabase database;

    const char8* terminals = "<>/()";

    const char8* separators = "\n\r\t, ";

    StreamString err = "";

    SlkAction myParser(configString, database, terminals, separators, &err);

    myParser.Parse();

    if (!database.MoveAbsolute("block")) {
        return false;
    }

    int32 var[3] = { 0 };
    database.Read("var", var);

    printf("\n%d\n", var);
    bool ok = var[0] == 1;
    ok = var[1] == 2;
    ok = var[2] == 3;

    return ok;
}

bool SlkActionTest::TestParseMatrixXMLGrammar() {

    StreamString configString = "<block>\n"
            "                           <var><><>1 2</><>3,4</></></var>\n"
            "                           </block>";

    configString.Seek(0);
    ConfigurationDatabase database;

    const char8* terminals = "<>/()";

    const char8* separators = "\n\r\t, ";

    StreamString err = "";

    SlkAction myParser(configString, database, terminals, separators, &err);

    myParser.Parse();

    if (!database.MoveAbsolute("block")) {
        return false;
    }

    int32 var[2][2] = { { 0 } };
    database.Read("var", var);

    printf("\n%d\n", var);
    bool ok = var[0][0] == 1;
    ok = var[0][1] == 2;
    ok = var[1][0] == 3;
    ok = var[1][1] == 4;

    return ok;
}
