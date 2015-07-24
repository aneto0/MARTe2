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

bool BasicConsoleTest::TestConstructor() {
    BasicConsole myConsole;

    uint32 nRowsOut;
    uint32 nColsOut;
    //in some os you need to open the console first.
    if (myConsole.GetSize(nColsOut, nRowsOut) == NoError) {

        if (nColsOut != 0 || nRowsOut != 0) {
            return false;
        }
    }

    return true;

}

//Open the console with in the mode passed by argument
bool BasicConsoleTest::TestOpenModeDefault(FlagsType openingMode) {

    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    ErrorType error = myConsole.Open(BasicConsole::Mode::Default);
    return error == NoError;

}

//Open the console with in the mode passed by argument
bool BasicConsoleTest::TestOpenModeCreateNewBuffer() {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    ErrorType error = myConsole.Open(BasicConsole::Mode::CreateNewBuffer);

    return error == NoError;
}

//Open the console with in the mode passed by argument
bool BasicConsoleTest::TestOpenModePerformCharacterInput() {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    ErrorType error = myConsole.Open(BasicConsole::Mode::PerformCharacterInput);
    return error == NoError;

}

//Open the console with in the mode passed by argument
bool BasicConsoleTest::TestOpenModeDisableControlBreak() {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    ErrorType error = myConsole.Open(BasicConsole::Mode::DisableControlBreak);
    return error == NoError;
}

//Open the console with in the mode passed by argument
bool BasicConsoleTest::TestOpenModeEnablePaging() {
    BasicConsole myConsole;
    myConsole.SetSize(numberOfColumns, numberOfRows);
    ErrorType error = myConsole.Open(BasicConsole::Mode::EnablePaging);
    return error == NoError;
}

bool BasicConsoleTest::TestGetOpeningMode() {
    bool retValue;
    BasicConsole myConsole;
    myConsole.Open(BasicConsole::Mode::Default);
    myConsole.SetSize(numberOfColumns, numberOfRows);
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
    myConsole.Open(BasicConsole::Mode::Default);
    myConsole.SetSize(numberOfColumns, numberOfRows);
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
    myConsole.Open(BasicConsole::Mode::Default);
    myConsole.SetSize(numberOfColumns, numberOfRows);

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
    myConsole.Open(BasicConsole::Mode::Default);
    myConsole.SetSize(numberOfColumns, numberOfRows);

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
    myConsole.Open(BasicConsole::Mode::Default);
    myConsole.SetSize(numberOfColumns, numberOfRows);

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

bool BasicConsoleTest::TestWriteEndColumn() {
    BasicConsole myConsole;
    uint32 realStringSize;
    bool retValue;
    uint32 size;
    const char8 *string = "abcd efgh";
    myConsole.Open(BasicConsole::Mode::Default);
    myConsole.SetSize(4, numberOfRows);

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
    myConsole.Open(BasicConsole::Mode::EnablePaging);
    myConsole.SetSize(numberOfColumns, numberOfRows);

    bool ret = BasicConsoleTestPaging(overflow, rows, columns, myConsole);
    return ret;
}

//compare the read string with the string passed by argument
bool BasicConsoleTest::TestRead(const char8 *stringArg) {
    BasicConsole myConsole;
    char8 string[64];
    char8 result[128];
    myConsole.Open(BasicConsole::Mode::Default);
    myConsole.SetSize(numberOfColumns, numberOfRows);

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
    uint32 totalSize = N_CHARS_NEWLINE + stringSize;
    ErrorType err = myConsole.Read(string, totalSize, TTInfiniteWait);
    string[stringSize] = '\0';
    //compare the read string with the argument
    bool stringLengthOK = StringTestHelper::Compare(string, stringArg);

    return (err == NoError) && stringLengthOK;
}

bool BasicConsoleTest::TestTimeoutRead(uint32 timeout) {

    BasicConsole myConsole;
    if (!myConsole.TimeoutSupported()) {
        return true;
    }

    myConsole.Open(BasicConsole::Mode::Default);

    char buffer[32];
    uint32 size = 32;
    return myConsole.Read(buffer, size, timeout) == Timeout;

}

bool BasicConsoleTest::TestSetGetSize(uint32 columnIn,
                                      uint32 rowIn) {
    BasicConsole myConsole;
    ErrorType error = NoError;
    uint32 rowOut = 0;
    uint32 columnOut = 0;
    myConsole.Open(BasicConsole::Mode::Default);
    error = myConsole.SetSize(columnIn, rowIn);

    if (error != NoError) {
        return false;
    }
    error = myConsole.GetSize(columnOut, rowOut);
    if (error != NoError) {
        return false;
    }
    return ((rowIn == rowOut) && (columnIn == columnOut));
}

bool BasicConsoleTest::TestSetGetWindowSize(uint32 numberOfColumns,
                                            uint32 numberOfRows) {
    BasicConsole myConsole;
    if (!myConsole.WindowSizeSupported()) {
        return true;
    }
    myConsole.Open(BasicConsole::Mode::Default);
    myConsole.SetSize(numberOfColumns, numberOfRows);

    ErrorType error = NoError;
    error = myConsole.SetWindowSize(numberOfColumns - 2, numberOfRows - 2);
    if (error != NoError) {
        return false;
    }

    uint32 nRows = 0;
    uint32 nCols = 0;

    error = myConsole.GetWindowSize(nCols, nRows);
    if (error != NoError) {
        return false;
    }
    if ((nRows != (numberOfRows - 2) || nCols != (numberOfColumns - 2))) {
        return false;
    }

    error = myConsole.SetWindowSize(numberOfColumns + 10, numberOfRows + 10);
    if (error != NoError) {
        return false;
    }
    error = myConsole.GetWindowSize(nCols, nRows);
    if (error != NoError) {
        return false;
    }

    return nRows == numberOfRows && nCols == numberOfColumns;
}

bool BasicConsoleTest::TestClear() {
    BasicConsole myConsole;
    myConsole.Open(BasicConsole::Mode::Default);
    myConsole.SetSize(numberOfColumns, numberOfRows);

    ErrorType err = myConsole.Clear();
    return (err == NoError);
}

//Test the perform character input feature
bool BasicConsoleTest::TestPerfChar() {
    BasicConsole myConsole;
    const char8 *myMessage = "press any key\n";
    uint32 sizeMyMessage;
    myConsole.Open(BasicConsole::Mode::PerformCharacterInput);
    myConsole.SetSize(numberOfColumns, numberOfRows);

    sizeMyMessage = StringTestHelper::Size(myMessage);
    myConsole.Write(myMessage, sizeMyMessage, TTInfiniteWait);
    char8 read[1];
    uint32 size = 5;
    bool ok = (NoError == myConsole.Read(read, size, TTInfiniteWait));
    return ok && (size == 1);
}

bool BasicConsoleTest::TestShow() {
    BasicConsole myConsole;
    if (!myConsole.ConsoleBufferSupported()) {
        return true;
    }
    myConsole.Open(BasicConsole::Mode::CreateNewBuffer);

    ErrorType error = myConsole.ShowBuffer();
    uint32 size = 5;
    myConsole.Write("Hello", size, TTInfiniteWait);
    Sleep::Sec(1.5);
    return error == NoError;
}

bool BasicConsoleTest::TestSetGetTitleBar(const char8 *title) {

    BasicConsole myConsole;
    if (!myConsole.TitleBarSupported()) {
        return true;
    }
    myConsole.Open(BasicConsole::Mode::Default);

    if (myConsole.SetTitleBar(NULL) == NoError) {
        return false;
    }

    if (myConsole.SetTitleBar(title) != NoError) {
        return false;
    }

    char buffer[64];

    if (myConsole.GetTitleBar(buffer, 64) != NoError) {
        return false;
    }

    if (!StringTestHelper::Compare(buffer, title)) {
        return false;
    }

    Sleep::Sec(1.5);
    return myConsole.GetTitleBar(NULL, 10) == Warning;
}

bool BasicConsoleTest::TestSetGetCursorPosition(uint32 column,
                                                uint32 row) {

    BasicConsole myConsole;
    if (!myConsole.CursorPositionSupported()) {
        return true;
    }

    myConsole.Open(BasicConsole::Mode::Default);

    bool ok = true;

    ok = myConsole.SetCursorPosition(column, row) == NoError;

    uint32 colRet = 0;
    uint32 rowRet = 0;

    ok &= myConsole.GetCursorPosition(colRet, rowRet) == NoError;
    Sleep::Sec(1.5);

    myConsole.Clear();
//Not implemented in Linux. Avoid false positives...
    return (colRet == column) && (rowRet == row);

}

bool BasicConsoleTest::TestSetColour(Colours foregroundColour,
                                     Colours backgroundColour) {
    BasicConsole myConsole;
    if (!myConsole.ColourSupported()) {
        return true;
    }
    myConsole.Open(BasicConsole::Mode::Default);
    myConsole.Clear();
    myConsole.SetCursorPosition(0, 0);
    bool ok = (myConsole.SetColour(foregroundColour, backgroundColour) == NoError);
    uint32 size = 11;
    myConsole.Write("Hello World", size, TTInfiniteWait);
    Sleep::Sec(1.5);
    return ok;

}

bool BasicConsoleTest::TestPlotChar(char8 c,
                                    Colours foregroundColour,
                                    Colours backgroundColour,
                                    int column,
                                    int row) {
    BasicConsole myConsole;
    myConsole.Open(BasicConsole::Mode::Default);
    if (!myConsole.ColourSupported()) {
        return true;
    }
    myConsole.Clear();
    myConsole.SetCursorPosition(0, 0);
    bool ok = myConsole.PlotChar(c, foregroundColour, backgroundColour, column, row) == NoError;
    Sleep::Sec(1.5);
    return ok;
}

