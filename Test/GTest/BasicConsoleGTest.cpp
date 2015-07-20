/*
 *  AtomicGTest.cpp
 *
 *  Created on: Feb 27, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "BasicConsoleTest.h"

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

TEST(BasicConsoleGTest,TestWriteLargeSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteLargeSize("abcd"));
}

TEST(BasicConsoleGTest,TestWriteEndColumn) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteEndColumn());
}

TEST(BasicConsoleGTest,TestWrite2) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWriteCheckReturn("Hello bye bye world\nWorld with spaces\n\n\nand new lines\nand\nsingle\nwords.", 0));
}

TEST(BasicConsoleGTest,TestPaging) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestPaging(14, 15, 15));
}
//This tests need user intervent. Do not uncomment for automatic tests.

TEST(BasicConsoleGTest,TestRead) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestRead("Hello\n"));
}
/*
 TEST(BasicConsoleGTest,TestSetGetSize) {
 BasicConsoleTest console;
 ASSERT_TRUE(console.TestSetGetSize(10, 13));
 }

 TEST(BasicConsoleGTest,TestSetGetWindowsSize) {
 BasicConsoleTest console;
 ASSERT_TRUE(console.TestSetGetWindowSize(100, 70));
 }
 */

TEST(BasicConsoleGTest,TestClear) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestClear());
}

TEST(BasicConsoleGTest,TestPerfChar) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestPerfChar());
}
/*
 //Not supported
 TEST(BasicConsoleGTest,TestShow) {
 BasicConsoleTest console;
 ASSERT_TRUE(console.TestShow());
 }
 */
TEST(BasicConsoleGTest,TestSetTitleBar) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetTitleBar("title"));
}
/*
 //Not supported
 TEST(BasicConsoleGTest,TestSetGetCursorPosition) {
 BasicConsoleTest console;
 ASSERT_TRUE(console.TestSetGetCursorPosition(20, 10));
 }
 //Not supported
 TEST(BasicConsoleGTest,TestSetColour) {
 BasicConsoleTest console;
 ASSERT_TRUE(console.TestSetColour(White, Black));
 }
 */

TEST(BasicConsoleGTest,TestPlotChar) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestPlotChar('c', White, Black, 20, 10));
}

