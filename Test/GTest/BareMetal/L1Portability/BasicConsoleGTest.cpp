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


/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "MARTe2UTest.h"
#include "BasicConsoleTest.h"
#define Windows 2
#define Linux 1
#if ENVIRONMENT == Linux
#undef Linux
#define Linux Linux
#include "BasicFile.h"
#include "Directory.h"
#else
#define BasicFile int32
#endif
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

void RedirectConsoleInput(BasicFile &inputFile,
                          const char8* inputString) {
#if ENVIRONMENT == Linux
    inputFile.Open("inputFile_Test.txt", BasicFile::FLAG_CREAT | BasicFile::ACCESS_MODE_R | BasicFile::ACCESS_MODE_W);
    uint32 size = StringHelper::Length(inputString);
    inputFile.Write(inputString, size);
    if (N_CHARS_NEWLINE == 1u) {
        size = 1u;
        inputFile.Write("\n", size);
    }
    else {
        size = 2u;
        inputFile.Write("\r\n", size);
    }
    inputFile.Seek(0);
    dup2(inputFile.GetWriteHandle(), fileno(stdin));
#endif
}

void Clean(BasicFile &inputFile) {
#if ENVIRONMENT == Linux
    inputFile.Close();
    Directory remFile("inputFile_Test.txt");
    remFile.Delete();
#endif
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestConstructor) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestConstructor());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestOpenModeDefault) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestOpenModeDefault());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestOpenModeCreateNewBuffer) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestOpenModeCreateNewBuffer());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestOpenModePerformCharacterInput) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestOpenModePerformCharacterInput());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestOpenModeDisableControlBreak) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestOpenModeDisableControlBreak());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestOpenModeEnablePaging) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestOpenModeEnablePaging());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestGetOpeningMode) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestGetOpeningMode());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestClose) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestClose());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestWriteCheckReturn) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteCheckReturn("ABC\nDE\n", 1));
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestWriteNullString) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteNullString());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestWriteExactSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteExactSize("abcd"));
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestWriteSmallSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteSmallSize("abcd"));
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestWriteEndColumn) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteEndColumn());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestWrite2) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteCheckReturn("Hello bye bye world\nWorld with spaces\n\n\nand new lines\nand\nsingle\nwords.", 0));
}

//This test needs user intervention. Do not uncomment for automatic tests.
TEST(BareMetal_L1Portability_BasicConsoleGTest,TestPaging) {
    BasicConsoleTest console;
    BasicFile inputFile;
    RedirectConsoleInput(inputFile, "\n");
    ASSERT_TRUE(console.TestPaging(14, 15, 15));
    Clean(inputFile);
}

//This test needs user intervention. Do not uncomment for automatic tests.
TEST(BareMetal_L1Portability_BasicConsoleGTest,TestRead) {
    BasicConsoleTest console;
    BasicFile inputFile;
    RedirectConsoleInput(inputFile, "Hello");
    ASSERT_TRUE(console.TestRead("Hello"));
    Clean(inputFile);
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestTimeoutRead) {
    BasicConsoleTest console;
    BasicFile inputFile;
    RedirectConsoleInput(inputFile, "");
    ASSERT_TRUE(console.TestTimeoutRead(100));
    Clean(inputFile);
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestSetGetSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetGetSceneSize(100, 70));
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestSetGetWindowsSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetGetWindowSize(50, 20));
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestClear) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestClear());
}

//This test needs user intervention. Do not uncomment for automatic tests.
TEST(BareMetal_L1Portability_BasicConsoleGTest,TestPerfChar) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestPerfChar());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestShow) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestShowBuffer());
}
TEST(BareMetal_L1Portability_BasicConsoleGTest,TestSetGetTitleBar) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetGetTitleBar("title"));
}

//This test is not supported.
TEST(BareMetal_L1Portability_BasicConsoleGTest,TestSetGetCursorPosition) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetGetCursorPosition(20, 10));
}

//This test is not supported.
TEST(BareMetal_L1Portability_BasicConsoleGTest,TestSetColour) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetColour(Blue, Red));
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestPlotChar) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestPlotChar('c', Blue, Yellow, 20, 10));
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestCanWrite) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestCanWrite());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestCanRead) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestCanRead());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestCanSeek) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestCanSeek());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestTimeoutWrite) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestTimeoutWrite("HelloWorld", 500));
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSize());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestSeek) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSeek());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestRelativeSeek) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestRelativeSeek());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestPosition) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestPosition());
}

TEST(BareMetal_L1Portability_BasicConsoleGTest,TestSetSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetSize());
}
