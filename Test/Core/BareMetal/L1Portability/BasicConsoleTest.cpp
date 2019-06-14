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

#include "BasicConsoleTest.h"
#include "StaticCString.h"
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

bool BasicConsoleTestWrite(CCString string,
                           uint32 padding,
                           BasicConsole &myConsole) {
    uint32 stringSize = string.GetSize();

    //add something to the size to pass as argument to test the write function
    uint32 size = stringSize + padding;
    //invalid parameters
    if (size == 0) {
        return false;
    }
    bool ret = myConsole.Write(string, size,MilliSeconds::Infinite );
    return (ret);
}

bool BasicConsoleTest::TestWriteCheckReturn(CCString string,
                                            uint32 padding) {
    BasicConsole myConsole;
    bool retValue;
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
    bool retValue;
    CCString string = "";
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

    uint32 realStringSize = string.GetSize();
    retValue = (!myConsole.Write(string.GetList(), realStringSize));
    return retValue;
}

bool BasicConsoleTest::TestWriteExactSize(CCString string) {
    BasicConsole myConsole;
    bool retValue;
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

    uint32 size = string.GetSize();
    uint32 sizeSave = size;
    myConsole.Write(string.GetList(), size);
    retValue = (sizeSave == size);
    return retValue;
}

bool BasicConsoleTest::TestWriteSmallSize(CCString string) {
    BasicConsole myConsole;
    bool retValue;
    uint32 size;
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

    uint32 realStringSize = string.GetSize();
    uint32 padding = 2;

    //calculate the size of the string
    if (realStringSize < padding) {
        return false;
    }
    //Enter a smaller size than the real size of the string (-2 is arbitrary number).
    size = realStringSize - padding;
    myConsole.Write(string.GetList(), size,MilliSeconds::Infinite);
    retValue = (realStringSize + padding == size);
    return retValue;
}

bool BasicConsoleTest::TestWriteEndColumn() {
    BasicConsole myConsole;
    bool retValue;
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(4, numberOfRows);

    CCString string = "abcd efgh";
    uint32 realStringSize = string.GetSize();
    uint32 size = realStringSize;
    myConsole.Write(string.GetList(), size);
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

bool BasicConsoleTest::TestRead(CCString stringArg) {
    BasicConsole myConsole;
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

    uint32 stringSize = stringArg.GetSize();
    DynamicCString prefix = "\nPut: ";

    prefix().Append(stringArg);

    //print the request: the user must insert the string passed by argument
    BasicConsoleTestWrite(prefix.GetList(), 0, myConsole);
    //read the string
    char8 buffer[128];
    StaticCString<128> reply(buffer);
    uint32 totalSize = N_CHARS_NEWLINE + stringSize;
    bool err = myConsole.Read(reply.GetList(), totalSize, MilliSeconds::Infinite);
    reply[stringSize] = '\0';
    //compare the read string with the argument
    bool stringLengthOK = stringArg.IsSameAs(reply);

    return (err) && stringLengthOK;
}


bool BasicConsoleTest::TestTimeoutRead(MilliSeconds timeout) {

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
    myConsole.Open(BasicConsoleMode::PerformCharacterInput);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

    CCString myMessage = "press any key\n";
    uint32 sizeMyMessage = myMessage.GetSize();
    myConsole.Write(myMessage, sizeMyMessage, MilliSeconds::Infinite);
    char8 read[32];
    uint32 size = 5;
    bool ok = (myConsole.Read(read, size, MilliSeconds::Infinite));
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
    myConsole.Write("Hello", size, MilliSeconds::Infinite);
    Sleep::Long(1.5,Units::s);
    return error;
}

bool BasicConsoleTest::TestSetGetTitleBar(CCString title) {

    BasicConsole myConsole;
    if (!myConsole.TitleBarSupported()) {
        return true;
    }
    myConsole.Open(BasicConsoleMode::Default);

    if (myConsole.SetTitleBar(emptyString)) {
        return false;
    }

    if (!myConsole.SetTitleBar(title)) {
        return false;
    }

    char buffer[64];
    StaticCString<64> titleName(buffer);

    if (!myConsole.GetTitleBar(CString(titleName), 64)) {
        return false;
    }

    if (!title.IsSameAs(buffer)){
        return false;
    }

    Sleep::Long(1.5,Units::s);
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
    Sleep::Long(1.5,Units::s);

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
    myConsole.Write("Hello World", size, MilliSeconds::Infinite);
    Sleep::Long(1.5,Units::s);

    return ok;

}

bool BasicConsoleTest::TestPlotChar(char8 c,
                                    Colours foregroundColour,
                                    Colours backgroundColour,
                                    uint32 column,
                                    uint32 row) {
    BasicConsole myConsole;
    myConsole.Open(BasicConsoleMode::Default);
    if (!myConsole.ColourSupported()) {
        return true;
    }
    myConsole.Clear();
    myConsole.SetCursorPosition(0, 0);
    bool ok = myConsole.PlotChar(c, foregroundColour, backgroundColour, column, row);
    Sleep::Long(1.5,Units::s);
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

bool BasicConsoleTest::TestTimeoutWrite(CCString string,MilliSeconds timeout) {
    BasicConsole myConsole;
    bool retValue;
    myConsole.Open(BasicConsoleMode::Default);
    myConsole.SetSceneSize(numberOfColumns, numberOfRows);

    uint32 realStringSize = string.GetSize();


    //add something to the size to pass as argument to test the write function
    uint32 size = realStringSize;

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
