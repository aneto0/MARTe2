/* Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http: //ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id:$
 *
 **/

#include "GeneralDefinitions.h"
#include "BasicConsoleTest.h"
#include "StringTestHelper.h"

//Open the console with in the mode passed by argument
bool BasicConsoleTest::TestOpen(ConsoleOpeningMode openingMode) {
    return BasicConsoleOpen(myConsole, openingMode, N_COLUMNS, N_ROWS,
                            TTInfiniteWait);
}

//write the string passed by argument
bool BasicConsoleTest::TestWrite(const char* string, int32 padding) {
    //since this function is used by other tests, open the console only if padding !=0.
    if (padding != 0) {
        if (!TestOpen(ConsoleDefault)) {
            return False;
        }
    }
    uint32 stringSize;

    //calculate the size of the string
    if ((stringSize = StringTestHelper::Size(string)) < 0) {
        return False;
    }

    //add something to the size to pass as argument to test the write function
    uint32 size = stringSize + padding;

    //Only to return true in this case
    if (padding < 0) {
        stringSize -= padding;
    }

    bool condition1 = BasicConsoleWrite(myConsole, string, size,
                                        TTInfiniteWait);
    //return true if the size is correct
    return condition1 && (size == stringSize);

}

//compare the read string with the string passed by argument
bool BasicConsoleTest::TestRead(const char* stringArg, int32 sizeArg) {
    //size must be positive
    if (sizeArg < 0) {
        return False;
    }

    //since this function is used by other tests, open the console only if padding !=0.
    if (sizeArg != 0) {
        if (!TestOpen(ConsoleDefault)) {
            return False;
        }
    }

    char string[N_COLUMNS];
    char result[N_COLUMNS + 20];
    uint32 size = N_COLUMNS;
    uint32 stringSize;

    //calculate the size of the string
    if ((stringSize = StringTestHelper::Size(stringArg)) < 0) {
        return False;
    }
    const char* prefix = "\nPut: ";
    //define the request to print
    StringTestHelper::Append((char*) prefix, stringArg, result);

    //print the request: the user must insert the string passed by argument
    TestWrite(result, 0);

    //read the string
    bool condition1 = BasicConsoleRead(myConsole, string, size, TTInfiniteWait);

    //compare the read string with the argument
    bool condition2 = StringTestHelper::Compare(string, stringArg);

    //return true if the read string is equal to the argument
    if (sizeArg != 0) {
        return condition1 && condition2 && (size == ((uint32) sizeArg));
    }
    else {
        string[size] = '\n';
        string[size + 1] = '\0';
        return (size == 1); //&& TestWrite(string, 0);
    }

}

//Test the paging feature
bool BasicConsoleTest::TestPaging(int32 overflow, int32 rows, int32 columns) {

    //open the console in enable paging mode
    if (!TestOpen(EnablePaging)) {
        return False;
    }

    if (!myConsole.SetSize(rows, columns)) {
        return False;
    }
    int32 retRows = 0;
    int32 retCols = 0;

    if (!myConsole.GetSize(retRows, retCols)) {
        return False;
    }

    if (rows != retRows || columns != retCols) {
        return False;
    }

    //define the size of the string to print
    int32 n = 0;
    int32 limit = 2 * (rows + overflow - 1);
    char string[64];

    //define the string to print
    while (n < limit) {
        string[n] = '-';
        string[n + 1] = '\n';
        n += 2;
    }
    string[limit] = '\0';

    //print the string
    return TestWrite(string, 0);
}

//Test the perform character input feature
bool BasicConsoleTest::TestPerfChar() {

    //open the console in perform character input mode
    if (!TestOpen(PerformCharacterInput)) {
        return False;
    }

    const char* request = "press any key\n";
    //return true if the size of the read string is one as aspected.
    return TestRead((char*) request, 0);
}

bool BasicConsoleTest::TestNotImplemented() {
    int32 rows = 0;
    int32 cols = 0;

    if (!myConsole.SetWindowSize(5, 5)) {
        return False;
    }

    if (!myConsole.GetWindowSize(rows, cols)) {
        return False;
    }

    if (rows != 5 || cols != 5) {
        return False;
    }

    Colours black;

    if (myConsole.SetColour(black, black)) {
        return False;
    }

    if (myConsole.SetCursorPosition(rows, cols)) {
        return False;
    }

    if (myConsole.GetCursorPosition(rows, cols)) {
        return False;
    }

    if (myConsole.SetTitleBar("Title")) {
        return False;
    }

    if (!myConsole.Show()) {
        return False;
    }

    if (myConsole.PlotChar('c', black, black, rows, cols)) {
        return False;
    }

    if (!myConsole.Clear()) {
        return False;
    }

    return True;
}
