/**
 * @file BasicConsoleTest.cpp
 * @brief Source file for class BasicConsoleTest
 * @date 30/06/2015
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
 * the class BasicConsoleTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "BasicConsoleTest.h"
#include "StringTestHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

//Open the console with in the mode passed by argument
bool BasicConsoleTest::TestOpenModeDefault(FlagsType openingMode) {

    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    return (0 == myConsole.Open(BasicConsole::Mode::Default));
}

//Open the console with in the mode passed by argument
bool BasicConsoleTest::TestOpenModeCreateNewBuffer() {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    return (0 == myConsole.Open(BasicConsole::Mode::CreateNewBuffer));
}

//Open the console with in the mode passed by argument
bool BasicConsoleTest::TestOpenModePerformCharacterInput() {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    return (0 == myConsole.Open(BasicConsole::Mode::PerformCharacterInput));
}

//Open the console with in the mode passed by argument
bool BasicConsoleTest::TestOpenModeDisableControlBreak() {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    return (0 == myConsole.Open(BasicConsole::Mode::DisableControlBreak));
}

//Open the console with in the mode passed by argument
bool BasicConsoleTest::TestOpenModeEnablePaging() {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    return (0 == myConsole.Open(BasicConsole::Mode::EnablePaging));
}

bool BasicConsoleTest::TestGetOpeningMode() {
    bool retValue;
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);
    retValue = (BasicConsole::Mode::Default == myConsole.GetOpeningMode());
    myConsole.Close();
    myConsole.Open(BasicConsole::Mode::CreateNewBuffer);
    retValue &= (BasicConsole::Mode::CreateNewBuffer == myConsole.GetOpeningMode());
    myConsole.Close();
    myConsole.Open(BasicConsole::Mode::PerformCharacterInput);
    retValue &= (BasicConsole::Mode::PerformCharacterInput == myConsole.GetOpeningMode());
    myConsole.Close();
    myConsole.Open(BasicConsole::Mode::DisableControlBreak);
    retValue &= (BasicConsole::Mode::DisableControlBreak == myConsole.GetOpeningMode());
    myConsole.Close();
    myConsole.Open(BasicConsole::Mode::EnablePaging);
    retValue &= (BasicConsole::Mode::EnablePaging == myConsole.GetOpeningMode());
    retValue &= (BasicConsole::Mode::Default != myConsole.GetOpeningMode());
    myConsole.Close();
    return retValue;
}

bool BasicConsoleTest::TestClose() {
    bool retValue;
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);
    retValue = (0 == myConsole.Close());
    return retValue;
}

//write the string passed by argument
bool BasicConsoleTestWrite(const char8 *string,
                           uint32 padding,
                           BasicConsole &myConsole) {
    uint32 stringSize;
    //calculate the size of the string
    if ((stringSize = StringTestHelper::Size(string)) < 0) {
        return false;
    }
    //add something to the size to pass as argument to test the write function
    uint32 size = stringSize + padding;
    //invalid parameters
    if (size < 0) {
        return false;
    }
    ErrorType ret = myConsole.Write(string, size, TTInfiniteWait);
    return (ret == NoError);
}

//write the string passed by argument
bool BasicConsoleTest::TestWriteCheckReturn(const char8 *string,
                                            uint32 padding) {
    BasicConsole myConsole;
    bool retValue;
    if (padding < 0) {
        padding = 0;
    }
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);
    retValue = BasicConsoleTestWrite(string, padding, myConsole);
    myConsole.Close();
    myConsole.Open(BasicConsole::Mode::CreateNewBuffer);
    retValue &= BasicConsoleTestWrite(string, padding, myConsole);
    myConsole.Close();
    myConsole.Open(BasicConsole::Mode::DisableControlBreak);
    retValue &= BasicConsoleTestWrite(string, padding, myConsole);
    myConsole.Close();
    myConsole.Open(BasicConsole::Mode::EnablePaging);
    retValue &= BasicConsoleTestWrite(string, padding, myConsole);
    myConsole.Close();
    myConsole.Open(BasicConsole::Mode::PerformCharacterInput);
    retValue &= BasicConsoleTestWrite(string, padding, myConsole);

    return retValue;
}

bool BasicConsoleTest::TestWriteNullString() {
    BasicConsole myConsole;
    uint32 realStringSize;
    bool retValue;
    const char8 *string = "";
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);
    //calculate the size of the string
    if ((realStringSize = StringTestHelper::Size(string)) < 0) {
        return false;
    }
    uint32 size = realStringSize;
    //invalid parameters
    if (size < 0) {
        return false;
    }
    retValue = (Warning == myConsole.Write(string, size, TTInfiniteWait));
    return retValue;
}

bool BasicConsoleTest::TestWriteExactSize(const char8 *string) {
    BasicConsole myConsole;
    uint32 realStringSize;
    bool retValue;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);
    //calculate the size of the string
    if ((realStringSize = StringTestHelper::Size(string)) < 0) {
        return false;
    }
    //add something to the size to pass as argument to test the write function
    uint32 size = realStringSize;
    //invalid parameters
    if (size < 0) {
        return false;
    }
    myConsole.Write(string, size, TTInfiniteWait);
    retValue = (realStringSize == size);
    return retValue;
}

bool BasicConsoleTest::TestWriteSmallSize(const char8 *string) {
    BasicConsole myConsole;
    uint32 realStringSize;
    bool retValue;
    uint32 size;
    int32 padding = -2;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);
    //calculate the size of the string
    if ((realStringSize = StringTestHelper::Size(string)) < 0) {
        return false;
    }
    //Enter a smaller size than the real size of the string (-2 is arbitrary number).
    size = realStringSize + padding;
    myConsole.Write(string, size, TTInfiniteWait);
    retValue = (realStringSize + padding == size);
    return retValue;
}

bool BasicConsoleTest::TestWriteLargeSize(const char8 *string) {
    BasicConsole myConsole;
    uint32 realStringSize;
    bool retValue;
    uint32 size;
    int32 padding = 2;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);
    //calculate the size of the string
    if ((realStringSize = StringTestHelper::Size(string)) < 0) {
        return false;
    }
    //Enter a smaller size than the real size of the string (+2 is arbitrary number).
    size = realStringSize + padding;
    myConsole.Write(string, size, TTInfiniteWait);
    retValue = (realStringSize == size);
    return retValue;
}

bool BasicConsoleTest::TestWriteEndColumn() {
    BasicConsole myConsole;
    uint32 realStringSize;
    bool retValue;
    uint32 size;
    const char8 *string = "abcd efgh";
    myConsole.SetSize(4, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);
    //calculate the size of the string
    if ((realStringSize = StringTestHelper::Size(string)) < 0) {
        return false;
    }
    size = realStringSize;
    myConsole.Write(string, size, TTInfiniteWait);
    retValue = (realStringSize == size);
    return retValue;
}

//Test the paging feature
static bool BasicConsoleTestPaging(uint32 overflow,
                                   uint32 rows,
                                   uint32 columns,
                                   BasicConsole &myConsole) {
    //open the console in enable paging mode
    myConsole.SetSize(rows, columns);
    //define the size of the string to print
    uint32 n = 0;
    uint32 limit = 2 * (rows + overflow - 1);
    char8 string[64];
    //define the string to print
    while (n < limit) {
        string[n] = '-';
        string[n + 1] = '\n';
        n += 2;
    }
    string[limit] = '\0';
    //print the string
    return BasicConsoleTestWrite(string, 0, myConsole);
}

//Test the paging feature
bool BasicConsoleTest::TestPaging(uint32 overflow,
                                  uint32 rows,
                                  uint32 columns) {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::EnablePaging);
    bool ret = BasicConsoleTestPaging(overflow, rows, columns, myConsole);
    return ret;
}

//compare the read string with the string passed by argument
bool BasicConsoleTest::TestRead(const char8 *stringArg) {
    BasicConsole myConsole;
    char8 string[numberOfColumns];
    char8 result[numberOfColumns + 20];
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);

    uint32 stringSize;
    //calculate the size of the string
    if ((stringSize = StringTestHelper::Size(stringArg)) < 0) {
        return false;
    }
    const char8 *prefix = "\nPut: ";
    //define the request to print
    StringTestHelper::Append((char8*) prefix, stringArg, result);
    //print the request: the user must insert the string passed by argument
    BasicConsoleTestWrite(result, 0, myConsole);
    //read the string
    ErrorType err = myConsole.Read(string, stringSize, TTInfiniteWait);
    string[stringSize] = '\0';
    //compare the read string with the argument
    bool stringLengthOK = StringTestHelper::Compare(string, stringArg);

    return (err == NoError) && stringLengthOK;
}

bool BasicConsoleTest::TestSetGetSize(uint32 rowIn,
                                      uint32 columnIn) {
    BasicConsole myConsole;
    uint32 rowOut;
    uint32 columnOut;
    myConsole.SetSize(rowIn, columnIn);
    myConsole.GetSize(rowOut, columnOut);
    return ((rowIn == rowOut) && (columnIn == columnOut));
}

bool BasicConsoleTest::TestSetGetWindowSize(uint32 numberOfColumns,
                                            uint32 numberOfRows) {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);
    uint32 nRows = 0;
    uint32 nCols = 0;
    uint32 nRowsBuff = 0;
    uint32 nColsBuff = 0;
    myConsole.GetSize(nColsBuff, nRowsBuff);
    if (nCols < numberOfColumns) {
        return nCols == nColsBuff;
    }
    if (nRows < numberOfRows) {
        return nRows == nRowsBuff;
    }
    return (nRows == numberOfRows && nCols == numberOfColumns);
}

bool BasicConsoleTest::TestClear() {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, 5);
    myConsole.Open(BasicConsole::Mode::Default);
    ErrorType err = myConsole.Clear();
    return (err == NoError);
}

//Test the perform character input feature
bool BasicConsoleTest::TestPerfChar() {
    BasicConsole myConsole;
    const char8 *myMessage = "press any key\n";
    uint32 sizeMyMessage;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::PerformCharacterInput);
    sizeMyMessage = StringTestHelper::Size(myMessage);
    myConsole.Write(myMessage, sizeMyMessage, TTInfiniteWait);
    char8 read[1];
    uint32 size = 5;
    bool ok = (NoError == myConsole.Read(read, size, TTInfiniteWait));
    return ok && (size == 1);
}

bool BasicConsoleTest::TestShow() {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);
    return myConsole.ShowBuffer();
}

bool BasicConsoleTest::TestSetTitleBar(const char8 *title) {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);
    bool ok = !myConsole.TitleBarSupported() || (myConsole.SetTitleBar(title) && !myConsole.SetTitleBar(NULL));

    return ok;
}

bool BasicConsoleTest::TestSetGetCursorPosition(uint32 column,
                                                uint32 row) {

    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);
    bool ok = true;

    ok = myConsole.SetCursorPosition(column, row);

    uint32 colRet = 0;
    uint32 rowRet = 0;

    ok &= myConsole.GetCursorPosition(colRet, rowRet);

//Not implemented in Linux. Avoid false positives...
    ok &= !myConsole.CursorPositionSupported() || ((colRet == column) && (rowRet == row));
    return ok;

}

bool BasicConsoleTest::TestSetColour(Colours foregroundColour,
                                     Colours backgroundColour) {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);
    bool ok = !myConsole.ColourSupported() || myConsole.SetColour(foregroundColour, backgroundColour);
    return ok;
}

bool BasicConsoleTest::TestPlotChar(char8 c,
                                    Colours foregroundColour,
                                    Colours backgroundColour,
                                    int column,
                                    int row) {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsole::Mode::Default);
    return myConsole.PlotChar(c, foregroundColour, backgroundColour, column, row);
}

