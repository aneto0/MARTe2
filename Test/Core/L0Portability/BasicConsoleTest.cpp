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
#include "stdio.h"

//Open the console with in the mode passed by argument
bool BasicConsoleTest::TestOpen(ConsoleOpeningMode openingMode) {

    BasicConsole myConsole(openingMode, N_ROWS, N_COLUMNS);
    return True;
}

//write the string passed by argument
bool BasicConsoleTestWrite(const char* string,
                           int32 padding,
                           BasicConsole &myConsole) {

    uint32 stringSize;

    //calculate the size of the string
    if ((stringSize = StringTestHelper::Size(string)) < 0) {
        return False;
    }

    //add something to the size to pass as argument to test the write function
    uint32 size = stringSize - padding;

    //invalid parameters
    if (size < 0) {
        return False;
    }

    //Only to return true in this case
    if (padding < 0) {
        stringSize += padding;
    }

    bool condition1 = BasicConsoleWrite(myConsole, string, size, TTInfiniteWait);

    if (!condition1) {
        printf("\nError 0\n");
    }

    printf("\nsize=%d, stringSize=%d\n", size, stringSize);
    //return true if the size is correct
    return condition1 && (size == (stringSize - padding));

}

//write the string passed by argument
bool BasicConsoleTest::TestWrite(const char* string,
                                 int32 padding) {
    BasicConsole myConsole(ConsoleDefault, N_ROWS, N_COLUMNS);
    return BasicConsoleTestWrite(string, padding, myConsole);

}

//compare the read string with the string passed by argument
bool BasicConsoleTestRead(const char* stringArg,
                          BasicConsole &myConsole) {

    char string[N_COLUMNS];
    char result[N_COLUMNS + 20];
    uint32 stringSize;

    //calculate the size of the string
    if ((stringSize = StringTestHelper::Size(stringArg)) < 0) {
        return False;
    }
    const char* prefix = "\nPut: ";
    //define the request to print
    StringTestHelper::Append((char*) prefix, stringArg, result);

    //print the request: the user must insert the string passed by argument
    BasicConsoleTestWrite(result, 0, myConsole);

    uint32 sizeTest = stringSize;
    //read the string
    bool condition1 = BasicConsoleRead(myConsole, string, stringSize, TTInfiniteWait);

    string[stringSize] = '\0';
    printf("\nstring=|%s|\n", string);
    printf("\nstringArg=|%s|\n", stringArg);

    if (!condition1) {
        printf("\nError 1\n");
    }

    //compare the read string with the argument
    bool condition2 = StringTestHelper::Compare(string, stringArg);
    if (!condition2) {
        printf("\nError 2\n");
    }

    //return true if the read string is equal to the argument
    if (stringSize > 1) {
        return condition1 && condition2 && (stringSize == sizeTest);
    }
    else {
        return (stringSize == 1); //&& TestWrite(string, 0);
    }

}

//compare the read string with the string passed by argument
bool BasicConsoleTest::TestRead(const char* stringArg) {
    BasicConsole myConsole(ConsoleDefault, N_ROWS, N_COLUMNS);
    return BasicConsoleTestRead(stringArg, myConsole);

}

//Test the paging feature
bool BasicConsoleTestPaging(int32 overflow,
                            int32 rows,
                            int32 columns,
                            BasicConsole &myConsole) {

    /*  //open the console in enable paging mode
     if (!BasicConsoleTestOpen(EnablePaging, myConsole)) {
     return False;
     }
     */
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
    return BasicConsoleTestWrite(string, 0, myConsole);
}

//Test the paging feature
bool BasicConsoleTest::TestPaging(int32 overflow,
                                  int32 rows,
                                  int32 columns) {
    BasicConsole myConsole(EnablePaging, N_ROWS, N_COLUMNS);
    bool ret = BasicConsoleTestPaging(overflow, rows, columns, myConsole);
    //BasicConsoleClose(myConsole);
    return ret;
}

//Test the perform character input feature
bool BasicConsoleTestPerfChar(BasicConsole &myConsole) {
    /* //open the console in perform character input mode
     if (!BasicConsoleTestOpen(PerformCharacterInput, myConsole)) {
     return False;
     }*/

    const char* request = "press any key\n";
    //return true if the size of the read string is one as aspected.
    return BasicConsoleTestRead((char*) request, myConsole);
}

//Test the perform character input feature
bool BasicConsoleTest::TestPerfChar() {
    BasicConsole myConsole(PerformCharacterInput, N_ROWS, N_COLUMNS);
    bool ret = BasicConsoleTestPerfChar(myConsole);
    //BasicConsoleClose(myConsole);
    return ret;
}

bool BasicConsoleTestNotImplemented(BasicConsole &myConsole) {
    int32 rows = 0;
    int32 cols = 0;

    if (!myConsole.SetWindowSize(15, 15)) {
        return False;
    }

    if (!myConsole.GetWindowSize(rows, cols)) {
        return False;
    }

    if (rows != 15 || cols != 15) {
        return False;
    }

    Colours black=Black;
    Colours white=White;
    Colours red=Red;

    if (myConsole.SetColour(black, white)) {
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

    if (myConsole.PlotChar('c', black, red, rows, cols)) {
        return False;
    }

    if (!myConsole.Clear()) {
        return False;
    }

    return True;
}

bool BasicConsoleTest::TestNotImplemented() {
    BasicConsole myConsole;
    bool ret = BasicConsoleTestNotImplemented(myConsole);
    BasicConsoleClose(myConsole);
    return ret;
}
