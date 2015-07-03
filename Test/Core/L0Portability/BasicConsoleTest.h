/**
 * @file BasicConsoleTest.h
 * @brief Header file for class BasicConsoleTest
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

 * @details This header file contains the declaration of the class BasicConsoleTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICCONSOLETEST_H_
#define 		BASICCONSOLETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BasicConsole.h"
#include "Threads.h"
#include "Sleep.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A class to test the BasicConsole functions.
 */
class BasicConsoleTest {

public:

    /**
     * Definitions of the of the console number of rows.
     */
    static const uint32 N_ROWS = 40;

    /**
     * Definitions of the of the console number of columns.
     */
    static const uint32 N_COLUMNS = 170;

    /**
     * @brief Empty constructor.
     */
    BasicConsoleTest() {
    }

    /**
     * @brief Tests the correct opening of the console with different options.
     * @param[in] openingMode is a flag with define the properties of the console.
     * @return true if the console is open correctly.
     */
    bool TestOpen(ConsoleOpeningMode openingMode = ConsoleDefault);

    /**
     * @brief Tests the correct behavior of the write operations.
     * @details If we give in input a size of the string greater than the size of the string passed, the write function must write only the string passed (until
     * the character '\0') and size must be reduced at the real string size.
     * @param[in] string is the string to write.
     * @param[in] padding is the difference between the size that we want to pass to the write function and the real string size.
     * @return true if the size returned is the real string size.
     */
    bool TestWrite(const char8* string,
                   int32 padding);

    /**
     * @brief Tests if the string read by console is the same of the string passed by argument (the user must write in console the same word) with the same size.
     * @param[in] stringArg is the string to match with the read string.
     * @return true if the read string is equal to the passed argument.
     */
    bool TestRead(const char8* stringArg);

    /**
     * @brief Tests the behavior of the paging property.
     * @details If the rows of string to write exceeds the row dimension of the console, you must press enter to begin a new page.
     * @param[in] overflow is the difference between the rows of the string to write and the row dimension of the console.
     * @param[in] rows is the desired row dimension of the console.
     * @param[in] columns is the desired column dimension of the console.
     * @return true is the write does not fail.
     */
    bool TestPaging(int32 overflow,
                    int32 rows,
                    int32 columns);

    /**
     * @brief Tests the behavior of the PerfChar property, namely an immediate return after the first character read.
     * @return true if the size of the read string is equal to one.
     */
    bool TestPerfChar();

    /**
     * @brief Tests the BasicConsole::Show function.
     * @return the return value of the function tested.
     */
    bool TestShow();

    /**
     * @brief Tests the BasicConsole::SetTitleBar function
     * @param[in] title is the desired title.
     * @return the return value of the function tested.
     */
    bool TestSetTitleBar(const char8 *title);

    /**
     * @brief Tests the BasicConsole::SetSize and BasicConsole::GetSize functions
     * @param[in] numberOfColumns is the desired number of columns.
     * @param[in] numberOfRows is the desired number of rows
     * @return true if the get function returns the values set by the set function.
     */
    bool TestSetGetSize(int32 numberOfColumns,
                        int32 numberOfRows);

    /**
     * @brief Tests the BasicConsole::GetWindowSize and BasicConsole::SetWindowSize functions
     * @param[in] numberOfColumns is the desired x-axis window size.
     * @param[in] numberOfRows is the desired y-axis window size.
     * @return true if the get function returns a value minor or equal to the value passed to the set function.
     */
    bool TestSetGetWindowSize(int32 numberOfColumns,
                              int32 numberOfRows);

    /**
     * @brief Tests the BasicConsole::SetCursorPosition and BAsicConsole::SetCursorPositon functions.
     * @param[in] column is the desired cursor x position.
     * @param[in] row is the desired cursor y position.
     * @return the return value of the tested function.
     */
    bool TestSetGetCursorPosition(int32 column,
                                  int32 row);

    /**
     * @brief Tests the BasicConsole::SetColour and BasicConsole::GetColourù
     * @param[in] foreGroundColour is the desired foreground color.
     * @param[in] backGroundColour is the desired background color.
     * @return the return value of the tested function.
     */
    bool TestSetColour(Colours foreGroundColour,
                       Colours backGroundColour);

    /**
     * @brief Tests the BasicConsole::Clear function
     * @return the return value of the tested function.
     */
    bool TestClear();

    /**
     * @brief Calls the BasicConsole::SetPaging.
     * @return true.
     */
    bool TestSetPaging();

    /**
     * @brief Tests the BasicConsole::PlotChar function.
     * @param[in] c is the character to be plotted.
     * @param[in] foreGroundColour is the desired foreground color.
     * @param[in] backGroundColour is the desired background color.
     * @param[in] columns is the desired character x position.
     * @param[in] row is the desired character y position.
     * @return the return value of the tested function
     */
    bool TestPlotChar(char8 c,
                      Colours foreGroundColour,
                      Colours backGroundColour,
                      int column,
                      int row);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICCONSOLETEST_H_ */

