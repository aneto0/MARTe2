/**
 * @file BasicConsoleGTest.cpp
 * @brief Source file for class BasicConsoleGTest
 * @date 24/07/2015
 * @author Giuseppe Ferr�
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
 * the class BasicConsoleGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */


/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>
#include <stdio.h>


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TestSupport.h"
#include "MilliSeconds.h"
#include "BasicConsoleTest.h"

#define Windows 2
#define Linux 1
#if ENVIRONMENT == Linux
#undef Linux
#define Linux Linux
#include <unistd.h>
#endif
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

void RedirectConsoleInput(FILE * &inputFile,CCString inputString) {
#if ENVIRONMENT == Linux
    inputFile = fopen("inputFile_Test.txt","RW");
    fputs(inputString.GetList(),inputFile);
    if (N_CHARS_NEWLINE == 1u) {
        fputs("\n",inputFile);
    }
    else {
        fputs("\r\n",inputFile);
    }
    fseek(inputFile,0,SEEK_SET);

    dup2(fileno(inputFile), fileno(stdin));
#endif
}

void Clean(FILE * &inputFile) {
#if ENVIRONMENT == Linux
    fclose(inputFile);
    remove("inputFile_Test.txt");
#endif
}

TEST(BasicConsoleGTest,TestConstructor) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestConstructor());
}

TEST(BasicConsoleGTest,TestOpenModeDefault) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestOpenModeDefault());
}

TEST(BasicConsoleGTest,TestOpenModeCreateNewBuffer) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestOpenModeCreateNewBuffer());
}

TEST(BasicConsoleGTest,TestOpenModePerformCharacterInput) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestOpenModePerformCharacterInput());
}

TEST(BasicConsoleGTest,TestOpenModeDisableControlBreak) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestOpenModeDisableControlBreak());
}

TEST(BasicConsoleGTest,TestOpenModeEnablePaging) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestOpenModeEnablePaging());
}

TEST(BasicConsoleGTest,TestGetOpeningMode) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestGetOpeningMode());
}

TEST(BasicConsoleGTest,TestClose) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestClose());
}

TEST(BasicConsoleGTest,TestWriteCheckReturn) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteCheckReturn("ABC\nDE\n", 1));
}

TEST(BasicConsoleGTest,TestWriteNullString) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteNullString());
}

TEST(BasicConsoleGTest,TestWriteExactSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteExactSize("abcd"));
}

TEST(BasicConsoleGTest,TestWriteSmallSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteSmallSize("abcd"));
}

TEST(BasicConsoleGTest,TestWriteEndColumn) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteEndColumn());
}

TEST(BasicConsoleGTest,TestWrite2) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteCheckReturn("Hello bye bye world\nWorld with spaces\n\n\nand new lines\nand\nsingle\nwords.", 0));
}

//This test needs user intervention. Do not uncomment for automatic tests.
TEST(BasicConsoleGTest,TestPaging) {
    BasicConsoleTest console;
    FILE * inputFile;
    RedirectConsoleInput(inputFile, "\n");
    ASSERT_TRUE(console.TestPaging(14, 15, 15));
    Clean(inputFile);
}

//This test needs user intervention. Do not uncomment for automatic tests.
TEST(BasicConsoleGTest,TestRead) {
    BasicConsoleTest console;
    FILE * inputFile;
    RedirectConsoleInput(inputFile, "Hello");
    ASSERT_TRUE(console.TestRead("Hello"));
    Clean(inputFile);
}

TEST(BasicConsoleGTest,TestTimeoutRead) {
    BasicConsoleTest console;
    FILE * inputFile;
    RedirectConsoleInput(inputFile, "");
    ASSERT_TRUE(console.TestTimeoutRead(MilliSeconds(100,Units::ms)));
    Clean(inputFile);
}

TEST(BasicConsoleGTest,TestSetGetSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetGetSceneSize(100, 70));
}

TEST(BasicConsoleGTest,TestSetGetWindowsSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetGetWindowSize(50, 20));
}

TEST(BasicConsoleGTest,TestClear) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestClear());
}

//This test needs user intervention. Do not uncomment for automatic tests.
TEST(BasicConsoleGTest,TestPerfChar) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestPerfChar());
}

TEST(BasicConsoleGTest,TestShow) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestShowBuffer());
}
TEST(BasicConsoleGTest,TestSetGetTitleBar) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetGetTitleBar("title"));
}

//This test is not supported.
TEST(BasicConsoleGTest,TestSetGetCursorPosition) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetGetCursorPosition(20, 10));
}

//This test is not supported.
TEST(BasicConsoleGTest,TestSetColour) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetColour(Blue, Red));
}

TEST(BasicConsoleGTest,TestPlotChar) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestPlotChar('c', Blue, Yellow, 20, 10));
}

TEST(BasicConsoleGTest,TestCanWrite) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestCanWrite());
}

TEST(BasicConsoleGTest,TestCanRead) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestCanRead());
}

TEST(BasicConsoleGTest,TestCanSeek) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestCanSeek());
}

TEST(BasicConsoleGTest,TestTimeoutWrite) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestTimeoutWrite("HelloWorld", MilliSeconds(500,Units::ms)));
}

TEST(BasicConsoleGTest,TestSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSize());
}

TEST(BasicConsoleGTest,TestSeek) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSeek());
}

TEST(BasicConsoleGTest,TestRelativeSeek) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestRelativeSeek());
}

TEST(BasicConsoleGTest,TestPosition) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestPosition());
}

TEST(BasicConsoleGTest,TestSetSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetSize());
}
