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
bool BasicConsoleTest::TestOpen(Flags openingMode) {

    BasicConsole myConsole(IBasicConsole::Mode::Default, BasicConsoleTest::N_COLUMNS, BasicConsoleTest::N_ROWS);
    return true;
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
    uint32 size = stringSize - padding;

    //invalid parameters
    if (size < 0) {
        return false;
    }

    //Only to return true in this case
    if (padding < 0) {
        stringSize += padding;
    }

    printf("\n1. size = %d\n", size);
    bool condition1 = myConsole.Write(string, size, TTInfiniteWait);

    if (!condition1) {
    }
    printf("\n2. size = %d\n", size);

    //return true if the size is correct
    uint32 dummySize = 1;
    return condition1 && (size == (stringSize - padding)) && !myConsole.Write(NULL, dummySize);

}

//write the string passed by argument
bool BasicConsoleTest::TestWrite(const char8 *string,
                                 uint32 padding) {
    BasicConsole myConsole(IBasicConsole::Mode::Default, BasicConsoleTest::N_COLUMNS, BasicConsoleTest::N_ROWS);
    return BasicConsoleTestWrite(string, padding, myConsole);

}

//compare the read string with the string passed by argument
bool BasicConsoleTestRead(const char8 *stringArg,
                          BasicConsole &myConsole) {

    char8 string[BasicConsoleTest::N_COLUMNS];
    char8 result[BasicConsoleTest::N_COLUMNS + 20];
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

    uint32 sizeTest = stringSize;
    //read the string
    bool condition1 = myConsole.Read(string, stringSize, TTInfiniteWait);

    string[stringSize] = '\0';

    if (!condition1) {
    }

    //compare the read string with the argument
    bool condition2 = StringTestHelper::Compare(string, stringArg);
    if (!condition2) {
    }

    //return true if the read string is equal to the argument
    if (stringSize > 1) {
        uint32 dummySize = 1;
        return condition1 && condition2 && (stringSize == sizeTest) && !myConsole.Read(NULL, dummySize);
    }
    else {
        uint32 dummySize = 1;
        return (stringSize == 1) && !myConsole.Read(NULL, dummySize); //&& TestWrite(string, 0);
    }

}

//compare the read string with the string passed by argument
bool BasicConsoleTest::TestRead(const char8 *stringArg) {
    BasicConsole myConsole(IBasicConsole::Mode::Default, BasicConsoleTest::N_COLUMNS, BasicConsoleTest::N_ROWS);
    return BasicConsoleTestRead(stringArg, myConsole);

}

//Test the paging feature
bool BasicConsoleTestPaging(uint32 overflow,
                            uint32 rows,
                            uint32 columns,
                            BasicConsole &myConsole) {

    /*  //open the console in enable paging mode
     if (!BasicConsoleTestOpen(EnablePaging, myConsole)) {
     return false;
     }
     */
    if (!myConsole.SetSize(rows, columns)) {
        return false;
    }
    uint32 retRows = 0;
    uint32 retCols = 0;

    if (!myConsole.GetSize(retRows, retCols)) {
        return false;
    }

    if (rows != retRows || columns != retCols) {
        return false;
    }

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
    BasicConsole myConsole(IBasicConsole::Mode::EnablePaging, BasicConsoleTest::N_COLUMNS, BasicConsoleTest::N_ROWS);
    bool ret = BasicConsoleTestPaging(overflow, rows, columns, myConsole);
    //BasicConsoleClose(myConsole);
    return ret;
}

//Test the perform character input feature
bool BasicConsoleTestPerfChar(BasicConsole &myConsole) {
    /* //open the console in perform character input mode
     if (!BasicConsoleTestOpen(PerformCharacterInput, myConsole)) {
     return false;
     }*/

    const char8 *request = "press any key\n";
    //return true if the size of the read string is one as aspected.
    return BasicConsoleTestRead((char8*) request, myConsole);
}

//Test the perform character input feature
bool BasicConsoleTest::TestPerfChar() {
    BasicConsole myConsole(IBasicConsole::Mode::PerformCharacterInput, BasicConsoleTest::N_COLUMNS, BasicConsoleTest::N_ROWS);
    bool ret = BasicConsoleTestPerfChar(myConsole);
    //BasicConsoleClose(myConsole);
    return ret;
}

bool BasicConsoleTest::TestShow() {
    BasicConsole myConsole(IBasicConsole::Mode::Default);
    return myConsole.Show();
}

bool BasicConsoleTest::TestSetTitleBar(const char8 *title) {
    BasicConsole myConsole(IBasicConsole::Mode::Default);
    bool ok = !myConsole.TitleBarSupported() || (myConsole.SetTitleBar(title) && !myConsole.SetTitleBar(NULL));

    return ok;
}

bool BasicConsoleTest::TestSetGetSize(uint32 numberOfColumns,
                                      uint32 numberOfRows) {

    BasicConsole myConsole(IBasicConsole::Mode::Default);
    bool ok = myConsole.SetSize(numberOfColumns, numberOfRows);
    uint32 nRows = 0;
    uint32 nCols = 0;

    ok &= myConsole.GetSize(nCols, nRows);
    ok &= (nRows == numberOfRows && nCols == numberOfColumns);
    return ok;
}

bool BasicConsoleTest::TestSetGetWindowSize(uint32 numberOfColumns,
                                            uint32 numberOfRows) {
    BasicConsole myConsole(IBasicConsole::Mode::Default);
    myConsole.SetWindowSize(numberOfColumns, numberOfRows);
    uint32 nRows = 0;
    uint32 nCols = 0;

    myConsole.GetWindowSize(nCols, nRows);

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

bool BasicConsoleTest::TestSetGetCursorPosition(uint32 column,
                                                uint32 row) {

    BasicConsole myConsole(IBasicConsole::Mode::Default);
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

    BasicConsole myConsole(IBasicConsole::Mode::Default);
    bool ok = !myConsole.ColourSupported() || myConsole.SetColour(foregroundColour, backgroundColour);
    return ok;

}

bool BasicConsoleTest::TestClear() {

    BasicConsole myConsole(IBasicConsole::Mode::Default);
    return myConsole.Clear();
}

bool BasicConsoleTest::TestSetMode() {
    BasicConsole myConsole(IBasicConsole::Mode::PerformCharacterInput);
    bool ok = true;

    Flags mode = myConsole.GetOpeningMode();

    ok = (mode == IBasicConsole::Mode::PerformCharacterInput);
    myConsole.Close();
    myConsole.Open(IBasicConsole::Mode::DisableControlBreak);
    mode = myConsole.GetOpeningMode();
    ok &= (mode != IBasicConsole::Mode::PerformCharacterInput);
    ok &= (mode == IBasicConsole::Mode::DisableControlBreak);

    return ok;

}

bool BasicConsoleTest::TestPlotChar(char8 c,
                                    Colours foregroundColour,
                                    Colours backgroundColour,
                                    int column,
                                    int row) {

    BasicConsole myConsole(IBasicConsole::Mode::Default);
    return myConsole.PlotChar(c, foregroundColour, backgroundColour, column, row);
}

