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
#include "stdio.h"
using namespace MARTe;

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
    if (myConsole.GetSceneSize(nColsOut, nRowsOut)) {

        if (nColsOut != 0 || nRowsOut != 0) {
            return false;
        }
    }

    return true;

}
bool BasicConsoleTest::TestOpenModeDefault(FlagsType openingMode) {

    BasicConsole myConsole;
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);
    ErrorManagement::ErrorType error = myConsole.Open(BasicConsoleMode::Default);
    return error;

}

bool BasicConsoleTest::TestOpenModeCreateNewBuffer() {
    BasicConsole myConsole;
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);
    ErrorManagement::ErrorType error = myConsole.Open(BasicConsoleMode::CreateNewBuffer);

    return error;
}

bool BasicConsoleTest::TestOpenModePerformCharacterInput() {
    BasicConsole myConsole;
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);
    ErrorManagement::ErrorType error = myConsole.Open(BasicConsoleMode::PerformCharacterInput);
    return error;

}

bool BasicConsoleTest::TestOpenModeDisableControlBreak() {
    BasicConsole myConsole;
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);
    ErrorManagement::ErrorType error = myConsole.Open(BasicConsoleMode::DisableControlBreak);
    return error;
}

bool BasicConsoleTest::TestOpenModeEnablePaging() {
    BasicConsole myConsole;
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);
    ErrorManagement::ErrorType error = myConsole.Open(BasicConsoleMode::EnablePaging);
    return error;
}

bool BasicConsoleTest::TestGetOpeningMode() {
    bool retValue;
    BasicConsole myConsole;
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);
    retValue = (BasicConsoleMode::Default == myConsole.GetOpeningMode());
    myConsole.Close();
    myConsole.Open(BasicConsoleMode::CreateNewBuffer);
    retValue &= (BasicConsoleMode::CreateNewBuffer == myConsole.GetOpeningMode());
    myConsole.Close();
    myConsole.Open(BasicConsoleMode::PerformCharacterInput);
    retValue &= (BasicConsoleMode::PerformCharacterInput == myConsole.GetOpeningMode());
    myConsole.Close();
    myConsole.Open(BasicConsoleMode::DisableControlBreak);
    retValue &= (BasicConsoleMode::DisableControlBreak == myConsole.GetOpeningMode());
    myConsole.Close();
    myConsole.Open(BasicConsoleMode::EnablePaging);
    retValue &= (BasicConsoleMode::EnablePaging == myConsole.GetOpeningMode());
    retValue &= (BasicConsoleMode::Default != myConsole.GetOpeningMode());
    myConsole.Close();
    return retValue;
}

bool BasicConsoleTest::TestClose() {
    bool retValue;
    BasicConsole myConsole;
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);
    myConsole.Open(BasicConsoleMode::Default);
    retValue = (myConsole.Close());
    return retValue;
}

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
    bool ret = myConsole.Write(string, size, TTInfiniteWait);
    return (ret);
}

bool BasicConsoleTest::TestWriteCheckReturn(const char8 *string,
                                            uint32 padding) {
    BasicConsole myConsole;
    bool retValue;
    if (padding < 0) {
        padding = 0;
    }
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);
    retValue = BasicConsoleTestWrite(string, padding, myConsole);
    myConsole.Close();
    myConsole.Open(BasicConsoleMode::CreateNewBuffer);
    retValue &= BasicConsoleTestWrite(string, padding, myConsole);
    myConsole.Close();
    myConsole.Open(BasicConsoleMode::DisableControlBreak);
    retValue &= BasicConsoleTestWrite(string, padding, myConsole);
    myConsole.Close();
    myConsole.Open(BasicConsoleMode::EnablePaging);
    retValue &= BasicConsoleTestWrite(string, padding, myConsole);
    myConsole.Close();
    myConsole.Open(BasicConsoleMode::PerformCharacterInput);
    retValue &= BasicConsoleTestWrite(string, padding, myConsole);
    return retValue;
}

bool BasicConsoleTest::TestWriteNullString() {
    BasicConsole myConsole;
    uint32 realStringSize;
    bool retValue;
    const char8 *string = "";
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

    //calculate the size of the string
    if ((realStringSize = StringTestHelper::Size(string)) < 0) {
        return false;
    }
    uint32 size = realStringSize;
    //invalid parameters
    if (size < 0) {
        return false;
    }
    retValue = (!myConsole.Write(string, size));
    return retValue;
}

bool BasicConsoleTest::TestWriteExactSize(const char8 *string) {
    BasicConsole myConsole;
    uint32 realStringSize;
    bool retValue;
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

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
    myConsole.Write(string, size);
    retValue = (realStringSize == size);
    return retValue;
}

bool BasicConsoleTest::TestWriteSmallSize(const char8 *string) {
    BasicConsole myConsole;
    uint32 realStringSize;
    bool retValue;
    uint32 size;
    int32 padding = -2;
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

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
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(4, numberOfRows);

    //calculate the size of the string
    if ((realStringSize = StringTestHelper::Size(string)) < 0) {
        return false;
    }
    size = realStringSize;
    myConsole.Write(string, size);
    retValue = (realStringSize == size);
    return retValue;
}

static bool BasicConsoleTestPaging(uint32 overflow,
                                   uint32 rows,
                                   uint32 columns,
                                   BasicConsole &myConsole) {
    //open the console in enable paging mode
    myConsole.SetSceneSize(rows, columns);
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

bool BasicConsoleTest::TestPaging(uint32 overflow,
                                  uint32 rows,
                                  uint32 columns) {
    BasicConsole myConsole;
    myConsole.Open(BasicConsoleMode::EnablePaging);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

    bool ret = BasicConsoleTestPaging(overflow, rows, columns, myConsole);
    return ret;
}

bool BasicConsoleTest::TestRead(const char8 *stringArg) {
    BasicConsole myConsole;
    char8 string[64];
    char8 result[128];
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

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
    bool err = myConsole.Read(string, totalSize, TTInfiniteWait);
    string[stringSize] = '\0';
    //compare the read string with the argument
    bool stringLengthOK = StringTestHelper::Compare(string, stringArg);

    return (err) && stringLengthOK;
}

bool BasicConsoleTest::TestTimeoutRead(uint32 timeout) {

    BasicConsole myConsole;
    if (!myConsole.TimeoutSupported()) {
        return true;
    }

    myConsole.Open(BasicConsoleMode::Default);

    char buffer[32];
    uint32 size = 32;
    return !myConsole.Read(buffer, size, timeout);

}

bool BasicConsoleTest::TestSetGetSceneSize(uint32 columnIn,
                                           uint32 rowIn) {
    BasicConsole myConsole;
    bool error = true;
    uint32 rowOut = 0;
    uint32 columnOut = 0;
    myConsole.Open(BasicConsoleMode::Default);
    error = myConsole.SetSceneSize(columnIn, rowIn);

    if (!error) {
        return false;
    }
    error = myConsole.GetSceneSize(columnOut, rowOut);
    if (!error) {
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
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

    ErrorManagement::ErrorType error = true;
    error = myConsole.SetWindowSize(numberOfColumns - 2, numberOfRows - 2);
    if (!error) {
        return false;
    }

    uint32 nRows = 0;
    uint32 nCols = 0;

    error = myConsole.GetWindowSize(nCols, nRows);
    if (!error) {
        return false;
    }
    if ((nRows != (numberOfRows - 2) || nCols != (numberOfColumns - 2))) {
        return false;
    }

    error = myConsole.SetWindowSize(numberOfColumns + 10, numberOfRows + 10);
    if (!error) {
        return false;
    }
    error = myConsole.GetWindowSize(nCols, nRows);
    if (!error) {
        return false;
    }

    return nRows == numberOfRows && nCols == numberOfColumns;
}

bool BasicConsoleTest::TestClear() {
    BasicConsole myConsole;
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

    ErrorManagement::ErrorType err = myConsole.Clear();
    return (err);
}

bool BasicConsoleTest::TestPerfChar() {
    BasicConsole myConsole;
    const char8 *myMessage = "press any key\n";
    uint32 sizeMyMessage;
    myConsole.Open(BasicConsoleMode::PerformCharacterInput);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

    sizeMyMessage = StringTestHelper::Size(myMessage);
    myConsole.Write(myMessage, sizeMyMessage, TTInfiniteWait);
    char8 read[1];
    uint32 size = 5;
    bool ok = (myConsole.Read(read, size, TTInfiniteWait));
    return ok && (size == 1);
}

bool BasicConsoleTest::TestShowBuffer() {
    BasicConsole myConsole;
    if (!myConsole.ConsoleBufferSupported()) {
        return true;
    }
    myConsole.Open(BasicConsoleMode::CreateNewBuffer);

    ErrorManagement::ErrorType error = myConsole.ShowBuffer();
    uint32 size = 5;
    myConsole.Write("Hello", size, TTInfiniteWait);
    Sleep::Sec(1.5);
    return error;
}

bool BasicConsoleTest::TestSetGetTitleBar(const char8 *title) {

    BasicConsole myConsole;
    if (!myConsole.TitleBarSupported()) {
        return true;
    }
    myConsole.Open(BasicConsoleMode::Default);

    if (myConsole.SetTitleBar(NULL)) {
        return false;
    }

    if (!myConsole.SetTitleBar(title)) {
        return false;
    }

    char buffer[64];

    if (!myConsole.GetTitleBar(buffer, 64)) {
        return false;
    }

    if (!StringTestHelper::Compare(buffer, title)) {
        return false;
    }

    Sleep::Sec(1.5);
    return !myConsole.GetTitleBar(NULL, 10);
}

bool BasicConsoleTest::TestSetGetCursorPosition(uint32 column,
                                                uint32 row) {

    BasicConsole myConsole;
    if (!myConsole.CursorPositionSupported()) {
        return true;
    }

    myConsole.Open(BasicConsoleMode::Default);

    bool ok = true;

    ok = myConsole.SetCursorPosition(column, row);

    uint32 colRet = 0;
    uint32 rowRet = 0;

    ok &= myConsole.GetCursorPosition(colRet, rowRet);
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
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.Clear();
    myConsole.SetCursorPosition(0, 0);
    bool ok = (myConsole.SetColour(foregroundColour, backgroundColour));
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
    myConsole.Open(BasicConsoleMode::Default);
    if (!myConsole.ColourSupported()) {
        return true;
    }
    myConsole.Clear();
    myConsole.SetCursorPosition(0, 0);
    bool ok = myConsole.PlotChar(c, foregroundColour, backgroundColour, column, row);
    Sleep::Sec(1.5);
    return ok;
}

bool BasicConsoleTest::TestCanWrite() {
    BasicConsole console;
    return console.CanWrite();
}

bool BasicConsoleTest::TestCanRead() {
    BasicConsole console;
    return console.CanRead();
}

bool BasicConsoleTest::TestCanSeek() {
    BasicConsole console;
    return !console.CanSeek();
}

bool BasicConsoleTest::TestTimeoutWrite(const char8 *string,
                                        TimeoutType timeout) {
    BasicConsole myConsole;
    uint32 realStringSize;
    bool retValue;
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

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
    myConsole.Write(string, size, timeout);
    retValue = (realStringSize == size);
    return retValue;
}

bool BasicConsoleTest::TestSize() {
    BasicConsole console;
    return console.Size() == 0xffffffffffffffff;
}

bool BasicConsoleTest::TestSeek() {
    BasicConsole console;
    return !console.Seek(0);
}

bool BasicConsoleTest::TestRelativeSeek() {
    BasicConsole console;
    return !console.RelativeSeek(0);
}

bool BasicConsoleTest::TestPosition() {
    BasicConsole console;
    return console.Position() == 0xffffffffffffffff;
}

bool BasicConsoleTest::TestSetSize() {
    BasicConsole console;
    return !console.SetSize(1);
}
