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
#define BASICCONSOLETEST_H_

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
     * @brief Default constructor
     * @detail When the BasicConsoleTest is created, automatically is set numberOfRows = 40
     * numberOfColumns = 60.
     */
    BasicConsoleTest() {
        numberOfRows = 40;
        numberOfColumns = 60;
    }

    /**
     * @brief Tests the correct opening of the console with default mode.
     * @param[in] openingMode is a flag with define the properties of the console.
     * @return true if the returned value of BasicConsole.Open() is true.
     */
    bool TestOpenModeDefault(FlagsType openingMode = BasicConsole::Mode::Default);

    /**
     * @brief Tests the correct opening of the console with CreateNewBuffer mode.
     * @param[in] openingMode is a flag with define the properties of the console.
     * @return true if the returned value of BasicConsole.Open() is true.
     */
    bool TestOpenModeCreateNewBuffer();

    /**
     * @brief Tests the correct opening of the console with PerformCharacterInput mode.
     * @param[in] openingMode is a flag with define the properties of the console.
     * @return true if the returned value of BasicConsole.Open() is true.
     */
    bool TestOpenModePerformCharacterInput();

    /**
     * @brief Tests the correct opening of the console with DisableControlBreak mode.
     * @param[in] openingMode is a flag with define the properties of the console.
     * @return true if the returned value of BasicConsole.Open() is true.
     */
    bool TestOpenModeDisableControlBreak();

    /**
     * @brief Tests the correct opening of the console with EnablePaging mode.
     * @param[in] openingMode is a flag with define the properties of the console.
     * @return true if the returned value of BasicConsole.Open() is true.
     */
    bool TestOpenModeEnablePaging();

    /**
     * @brief Test BasicConsole::GetOpeningMode function.
     * @detail Open consoles with different Modes Then checks if the returned value of GetOpeningMode
     * is consistent with the mode specified.
     * @return true if the returned value of the GetOpeningMode is the expected value.
     */
    bool TestGetOpeningMode();

    /**
     * @brief Test BasicConsole::Close() function.
     * @detail Open consoles and then close. Moreover the test tries to close a closed console.
     * @return true if the returned value of the BasicConsole::Close() is true when the console can close the
     * console and false when the console cannot close the console (because is closed).
     */
    bool TestClose();

    /**
     * @brief Tests BasicConsole::Write() function.
     * @details Checks for all modes the returned value of the BasicConsole::Write() function when tries
     * to write a string on the console
     * @param[in] string is the string to write.
     * @param[in] padding is the difference between the size that we want to pass to the write function
     * and the real string size. In this test when padding is negative automatically it is set to 0.
     * @return true if the BasicConsol::write() returns NoError for all open modes.
     */
    bool TestWriteCheckReturn(const char8 *string,
                              uint32 padding);
    /**
     * @brief Tests BasicConsole::Write() function.
     * @details Checks for the mode Default, the returned value of the BasicConsole::Write() function when
     * tries to write a NULL string on the console
     * @return true if the BasicConsol::write() returns Warning.
     */
    bool TestWriteNullString();

    /**
     * @brief Tests BasicConsole::Write() function.
     * @details Checks that the written bytes == "the size of the string" when the size input is in fact
     * the real size of the string.
     * @param[in] string is the string to write.
     * @return true if the sizes of the written string is the same than the real size of the string.
     */
    bool TestWriteExactSize(const char8 *string);

    /**
     * @brief Tests BasicConsole::Write() function.
     * @details Checks that the written bytes is = size when size input is less than the RealStringSize.
     * BasicConsole::Write() only writes the first size character of the string.
     * @param[in] string is the string to write.
     * @return true if the sizes of the written string = realStringSize+padding (where padding is negative)
     */
    bool TestWriteSmallSize(const char8 *string);

    /**
     * @brief Tests BasicConsole::Write() function.
     * @details If we give in input a size of the string greater than the size of the string passed,
     * the write function must write only the string passed (until the character '\0') and size must
     * be reduced at the real string size. This test checks that the written bytes = realStringSize.
     * @param[in] string is the string to write.
     * @return true if the sizes of the written string is the same than the real size of the string.
     */
    bool TestWriteLargeSize(const char8 *string);

    /**
     * @brief Tests BasicConsole::Write() function.
     * @details If the string is larger than the screen columns, the BasicConsole::Write() should
     * continue writing in the next line.
     * @return true if the sizes of the written string is the same than the real size of the string.
     * Moreover a visual check can be done in the console. Should appear "abcd" in one line and
     * "efgh" in the next line.
     */
    bool TestWriteEndColumn();

    /**
     * @brief Tests the behavior of the paging property.
     * @details If the rows of string to write exceeds the row dimension of the console, you must press enter to begin a new page.
     * @param[in] overflow is the difference between the rows of the string to write and the row dimension of the console.
     * @param[in] rows is the desired row dimension of the console.
     * @param[in] columns is the desired column dimension of the console.
     * @return true if BasicConsol::Write() returns NoError. Also can be visually check that is written
     * in the right place
     */
    bool TestPaging(uint32 overflow,
                    uint32 rows,
                    uint32 columns);

    /**
     * @brief Test BasicConsole::Read() function
     * @detail Test if the string read by console is the same of the string passed by argument (the user must write in console the same word) with the same size.
     * @param[in] stringArg is the string to match with the read string.
     * @return true if the read string is equal to the passed argument.
     */
    bool TestRead(const char8 *stringArg);

    /**
     * @brief Tests the BasicConsole::SetSize and BasicConsole::GetSize functions
     * @param[in] numberOfColumns is the desired number of columns.
     * @param[in] numberOfRows is the desired number of rows
     * @return true if the get function returns the values set by the set function.
     */
    bool TestSetGetSize(uint32 numberOfRows,
                        uint32 numberOfColumns);

    /**
     * @brief Tests the BasicConsole::GetWindowSize and BasicConsole::SetWindowSize functions
     * @param[in] numberOfColumns is the desired x-axis window size.
     * @param[in] numberOfRows is the desired y-axis window size.
     * @return true if the get function returns a value minor or equal to the value passed to the set function.
     */
    bool TestSetGetWindowSize(uint32 numberOfColumns,
                              uint32 numberOfRows);

    /**
     * @brief Tests the BasicConsole::Clear function
     * @return true if the BasicConsol::Clear() returns true, false otherwise.
     */
    bool TestClear();

    /**
     * @brief Tests the behavior of the PerfChar property, namely an immediate return after the first
     * character read.
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
     * @brief Tests the BasicConsole::SetCursorPosition and BAsicConsole::SetCursorPositon functions.
     * @param[in] column is the desired cursor x position.
     * @param[in] row is the desired cursor y position.
     * @return the return value of the tested function.
     */
    bool TestSetGetCursorPosition(uint32 column,
                                  uint32 row);

    /**
     * @brief Tests the BasicConsole::SetColour and BasicConsole::GetColourù
     * @param[in] foregroundColour is the desired foreground color.
     * @param[in] backgroundColour is the desired background color.
     * @return the return value of the tested function.
     */
    bool TestSetColour(Colours foregroundColour,
                       Colours backgroundColour);

    /**
     * @brief Tests the BasicConsole::PlotChar function.
     * @param[in] c is the character to be plotted.
     * @param[in] foregroundColour is the desired foreground color.
     * @param[in] backgroundColour is the desired background color.
     * @param[in] columns is the desired character x position.
     * @param[in] row is the desired character y position.
     * @return the return value of the tested function
     */
    bool TestPlotChar(char8 c,
                      Colours foregroundColour,
                      Colours backgroundColour,
                      int column,
                      int row);
    /**
     * @brief Auxiliar function to get the numberOfColumns
     * @return the numberOfColumns
     */
    uint32 GetNumberOfColumns() const {
        return numberOfColumns;
    }
    /**
     * @brief Auxiliar function to get the numberOfRows
     * @return the numberOfRows
     */
    uint32 GetNumberOfRows() const {
        return numberOfRows;
    }

private:
    /**
     * Definitions of the of the console number of rows.
     */
    uint32 numberOfRows;

    /**
     * Definitions of the of the console number of columns.
     */
    uint32 numberOfColumns;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICCONSOLETEST_H_ */

