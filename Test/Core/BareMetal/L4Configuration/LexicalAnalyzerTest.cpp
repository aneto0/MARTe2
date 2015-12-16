/**
 * @file LexicalAnalyzerTest.cpp
 * @brief Source file for class LexicalAnalyzerTest
 * @date 26/11/2015
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
 * the class LexicalAnalyzerTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "LexicalAnalyzerTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool LexicalAnalyzerTest::TestConstructor() {

    StreamString configString = " Hello ";
    configString.Seek(0);
    LexicalAnalyzer la(configString, "", " ", "", "", "");

    Token *tok = la.GetToken();

    if (tok->GetId() != STRING_TOKEN) {
        return false;
    }

    const char8* token = (const char8*) (tok->GetData());
    if (StringHelper::Compare(token, "Hello") != 0) {
        return false;
    }

    tok = la.GetToken();
    return tok->GetId() == EOF_TOKEN;
}

bool LexicalAnalyzerTest::TestGetToken() {

    StreamString configString = "//This is a one line comment\n"
            "/* This is a multiple \n"
            "line comment */ \n"
            "\"+PID\" {\n "
            "/Kp= 10, \n"
            "Ki =100.2//comment\n"
            "Kd= -10 } \n"
            "123a1 /*end*";

    configString.Seek(0);

    LexicalAnalyzer la(configString, "{}=", " ,\n", "//", "/*", "*/");

    // get +PID
    Token *tok = la.GetToken();

    if (tok->GetId() != STRING_TOKEN) {
        return false;
    }

    if (StringHelper::Compare((const char8*) (tok->GetData()), "+PID")) {
        return false;
    }

    if (tok->GetLineNumber() != 4) {
        return false;
    }

    // get {
    tok = la.GetToken();

    if (tok->GetId() != TERMINAL_TOKEN) {
        return false;
    }

    if (*((char8*) (tok->GetData())) != '{') {
        return false;
    }
    if (tok->GetLineNumber() != 4) {
        return false;
    }

    // get Kp
    tok = la.GetToken();

    if (tok->GetId() != STRING_TOKEN) {
        return false;
    }

    if (StringHelper::Compare((const char8*) (tok->GetData()), "/Kp")) {
        return false;
    }

    if (tok->GetLineNumber() != 5) {
        return false;
    }

    // get =
    tok = la.GetToken();

    if (tok->GetId() != TERMINAL_TOKEN) {
        return false;
    }

    if (*((char8*) (tok->GetData())) != '=') {
        return false;
    }

    if (tok->GetLineNumber() != 5) {
        return false;
    }

    // get 10
    tok = la.GetToken();

    if (tok->GetId() != NUMBER_TOKEN) {
        return false;
    }

    if (StringHelper::Compare((tok->GetData()), "10")) {
        return false;
    }

    if (tok->GetLineNumber() != 5) {
        return false;
    }

    // get Ki
    tok = la.GetToken();

    if (tok->GetId() != STRING_TOKEN) {
        return false;
    }

    if (StringHelper::Compare((const char8*) (tok->GetData()), "Ki")) {
        return false;
    }

    if (tok->GetLineNumber() != 6) {
        return false;
    }
    // get =
    tok = la.GetToken();

    if (tok->GetId() != TERMINAL_TOKEN) {
        return false;
    }

    if (*((char8*) (tok->GetData())) != '=') {
        return false;
    }

    if (tok->GetLineNumber() != 6) {
        return false;
    }
    // get 100.2
    tok = la.GetToken();

    if (tok->GetId() != NUMBER_TOKEN) {
        return false;
    }

    if (StringHelper::Compare((tok->GetData()), "100.2")) {
        return false;
    }

    if (tok->GetLineNumber() != 6) {
        return false;
    }
    // get Kd
    tok = la.GetToken();

    if (tok->GetId() != STRING_TOKEN) {
        return false;
    }

    if (StringHelper::Compare((const char8*) (tok->GetData()), "Kd")) {
        return false;
    }
    if (tok->GetLineNumber() != 7) {
        return false;
    }
    // get =
    tok = la.GetToken();

    if (tok->GetId() != TERMINAL_TOKEN) {
        return false;
    }

    if (*((char8*) (tok->GetData())) != '=') {
        return false;
    }

    if (tok->GetLineNumber() != 7) {
        return false;
    }
    // get -10
    tok = la.GetToken();

    if (tok->GetId() != NUMBER_TOKEN) {
        return false;

    }

    if (StringHelper::Compare((tok->GetData()), "-10")) {
        return false;
    }

    if (tok->GetLineNumber() != 7) {
        return false;
    }
    // get }
    tok = la.GetToken();

    if (tok->GetId() != TERMINAL_TOKEN) {
        return false;
    }

    if (*((char8*) (tok->GetData())) != '}') {
        return false;
    }
    if (tok->GetLineNumber() != 7) {
        return false;
    }
    // get 123a
    tok = la.GetToken();

    if (tok->GetId() != ERROR_TOKEN) {
        return false;
    }

    if (tok->GetLineNumber() != 8) {
        return false;
    }

    // get END
    tok = la.GetToken();

    if (tok->GetId() != EOF_TOKEN) {
        return false;
    }

    // get over the END
    tok = la.GetToken();

    return tok->GetId() == EOF_TOKEN;

}

bool LexicalAnalyzerTest::TestPeekToken() {

    StreamString configString = "//This is a one line comment\n"
            "/* This is a multiple *\n"
            "line comment */ \n"
            "\"+PID\" {\n "
            "Kp= 10, \n"
            "Ki =100.2\n"
            "Kd= -10 } //end!";

    configString.Seek(0);

    LexicalAnalyzer la(configString, "{}=", " ,\n", "//", "/*", "*/");

    // get +PID
    Token *tok = la.PeekToken(0u);

    if (tok->GetId() != STRING_TOKEN) {
        return false;
    }

    if (StringHelper::Compare((const char8*) (tok->GetData()), "+PID")) {
        return false;
    }

    if (tok->GetLineNumber() != 4) {
        return false;
    }

    // get {
    tok = la.PeekToken(1u);

    if (tok->GetId() != TERMINAL_TOKEN) {
        return false;
    }

    if (*((char8*) (tok->GetData())) != '{') {
        return false;
    }
    if (tok->GetLineNumber() != 4) {
        return false;
    }

    // get Kp
    tok = la.PeekToken(2u);

    if (tok->GetId() != STRING_TOKEN) {
        return false;
    }

    if (StringHelper::Compare((const char8*) (tok->GetData()), "Kp")) {
        return false;
    }

    if (tok->GetLineNumber() != 5) {
        return false;
    }

    // get =
    tok = la.PeekToken(3u);

    if (tok->GetId() != TERMINAL_TOKEN) {
        return false;
    }

    if (*((char8*) (tok->GetData())) != '=') {
        return false;
    }

    if (tok->GetLineNumber() != 5) {
        return false;
    }

    // get 10
    tok = la.PeekToken(4u);

    if (tok->GetId() != NUMBER_TOKEN) {
        return false;
    }

    if (StringHelper::Compare((tok->GetData()), "10")) {
        return false;
    }

    if (tok->GetLineNumber() != 5) {
        return false;
    }

    // get Ki
    tok = la.PeekToken(5u);

    if (tok->GetId() != STRING_TOKEN) {
        return false;
    }

    if (StringHelper::Compare((const char8*) (tok->GetData()), "Ki")) {
        return false;
    }

    if (tok->GetLineNumber() != 6) {
        return false;
    }
    // get =
    tok = la.PeekToken(6u);

    if (tok->GetId() != TERMINAL_TOKEN) {
        return false;
    }

    if (*((char8*) (tok->GetData())) != '=') {
        return false;
    }

    if (tok->GetLineNumber() != 6) {
        return false;
    }
    // get 100.2
    tok = la.PeekToken(7u);

    if (tok->GetId() != NUMBER_TOKEN) {
        return false;
    }

    if (StringHelper::Compare((tok->GetData()), "100.2")) {
        return false;
    }

    if (tok->GetLineNumber() != 6) {
        return false;
    }
    // get Kd
    tok = la.PeekToken(8u);

    if (tok->GetId() != STRING_TOKEN) {
        return false;
    }

    if (StringHelper::Compare((const char8*) (tok->GetData()), "Kd")) {
        return false;
    }
    if (tok->GetLineNumber() != 7) {
        return false;
    }
    // get =
    tok = la.PeekToken(9u);

    if (tok->GetId() != TERMINAL_TOKEN) {
        return false;
    }

    if (*((char8*) (tok->GetData())) != '=') {
        return false;
    }

    if (tok->GetLineNumber() != 7) {
        return false;
    }
    // get -10
    tok = la.PeekToken(10u);

    if (tok->GetId() != NUMBER_TOKEN) {
        return false;

    }

    if (StringHelper::Compare((tok->GetData()), "-10")) {
        return false;
    }

    if (tok->GetLineNumber() != 7) {
        return false;
    }
    // get }
    tok = la.PeekToken(11u);

    if (tok->GetId() != TERMINAL_TOKEN) {
        return false;
    }

    if (*((char8*) (tok->GetData())) != '}') {
        return false;
    }
    if (tok->GetLineNumber() != 7) {
        return false;
    }

    // get END
    tok = la.PeekToken(12u);

    if (tok->GetId() != EOF_TOKEN) {
        return false;
    }

    // get over the END
    tok = la.PeekToken(13u);

    if (tok->GetId() != EOF_TOKEN) {
        return false;
    }

    // the stack is not consumed.
    tok = la.GetToken();

    if (tok->GetId() != STRING_TOKEN) {
        return false;
    }

    if (StringHelper::Compare((const char8*) (tok->GetData()), "+PID")) {
        return false;
    }

    if (tok->GetLineNumber() != 4) {
        return false;
    }

    // peek the next
    // get {
    tok = la.PeekToken(0u);

    if (tok->GetId() != TERMINAL_TOKEN) {
        return false;
    }

    if (*((char8*) (tok->GetData())) != '{') {
        return false;
    }
    return tok->GetLineNumber() == 4;
}

bool LexicalAnalyzerTest::TestEscape() {

    StreamString configString = "Hello\\nWorld Hello\\tWorld\nHello\\rWorld \"\\\"Hello\\\"World\\\"\" Hello\\World";
    configString.Seek(0);

    LexicalAnalyzer la(configString, "{}=", " ,\n", "//", "/*", "*/");

    Token *tok = la.GetToken();

    if (StringHelper::Compare("Hello\nWorld", tok->GetData()) != 0) {
        return false;
    }

    tok = la.GetToken();

    if (StringHelper::Compare("Hello\tWorld", tok->GetData()) != 0) {
        return false;
    }
    tok = la.GetToken();

    if (StringHelper::Compare("Hello\rWorld", tok->GetData()) != 0) {
        return false;
    }
    tok = la.GetToken();

    if (StringHelper::Compare("\"Hello\"World\"", tok->GetData()) != 0) {
        return false;
    }

    tok = la.GetToken();
    return (StringHelper::Compare("Hello\\World", tok->GetData()) == 0);

}

bool LexicalAnalyzerTest::TestComments() {
    StreamString configString = "<a<!--comment-->b"
            "                    <c,,//comment >\n"
            "                    <!-\n-fake -->d<!-- comment->--<!--> <!--\n unterminated comment --";
    configString.Seek(0);

    LexicalAnalyzer la(configString, "<>/", " ,\n", "//", "<!--", "-->");

    Token *tok = la.GetToken();

    if (StringHelper::Compare(tok->GetData(), "<") != 0) {
        return false;
    }

    tok = la.GetToken();

    if (StringHelper::Compare(tok->GetData(), "a") != 0) {
        return false;
    }

    tok = la.GetToken();

    if (StringHelper::Compare(tok->GetData(), "b") != 0) {
        return false;
    }


    tok = la.GetToken();

    if (StringHelper::Compare(tok->GetData(), "<") != 0) {
        return false;
    }


    tok = la.GetToken();

    if (StringHelper::Compare(tok->GetData(), "c") != 0) {
        return false;
    }


    tok = la.GetToken();

    if (StringHelper::Compare(tok->GetData(), "<") != 0) {
        return false;
    }


    tok = la.GetToken();

    if (StringHelper::Compare(tok->GetData(), "!-") != 0) {
        return false;
    }

    tok = la.GetToken();

    if (StringHelper::Compare(tok->GetData(), "-fake") != 0) {
        return false;
    }


    tok = la.GetToken();

    if (StringHelper::Compare(tok->GetData(), "--") != 0) {
        return false;
    }


    tok = la.GetToken();

    if (StringHelper::Compare(tok->GetData(), ">") != 0) {
        return false;
    }


    tok = la.GetToken();

    if (StringHelper::Compare(tok->GetData(), "d") != 0) {
        return false;
    }


    tok = la.GetToken();

    return tok->GetId() == EOF_TOKEN;

}
