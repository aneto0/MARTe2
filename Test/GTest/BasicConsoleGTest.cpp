/*
 *  AtomicGTest.cpp
 *
 *  Created on: Feb 27, 2015
 */
#include <limits.h>
#include "gtest/gtest.h"
#include "BasicConsoleTest.h"



TEST(BasicConsoleGTest,TestOpen) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestOpen());
}

TEST(BasicConsoleGTest,TestWrite) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWrite("Hello bye bye world\nWorld with spaces\n\n\nand new lines\nand\nsingle\nwords.", 0));
}

TEST(BasicConsoleGTest,TestWrite2) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestWrite("Hello bye bye world\nWorld with spaces\n\n\nand new lines\nand\nsingle\nwords.", 0));
}

//This tests need user intervent. Do not uncomment for automatic tests.

TEST(BasicConsoleGTest,TestRead) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestRead("Hello\n"));
}

TEST(BasicConsoleGTest,TestPaging) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestPaging(14, 15, 15));
}

TEST(BasicConsoleGTest,TestPerfChar) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestPerfChar());
}

TEST(BasicConsoleGTest,TestShow) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestShow());
}

TEST(BasicConsoleGTest,TestSetTitleBar) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetTitleBar("title"));
}

TEST(BasicConsoleGTest,TestSetGetSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetGetSize(100, 70));
}

TEST(BasicConsoleGTest,TestSetGetWindowsSize) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetGetWindowSize(100, 70));
}

TEST(BasicConsoleGTest,TestSetGetCursorPosition) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetGetCursorPosition(20, 10));
}

TEST(BasicConsoleGTest,TestSetColour) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetColour(White, Black));
}

TEST(BasicConsoleGTest,TestClear) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestClear());
}
TEST(BasicConsoleGTest,TestSetPaging) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestSetMode());
}

TEST(BasicConsoleGTest,TestPlotChar) {
    BasicConsoleTest console;
    ASSERT_TRUE(console.TestPlotChar('c', White, Black, 20, 10));
}

