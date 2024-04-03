/**
 * @file BasicConsoleTest.h
 * @brief Header file for class BasicConsoleTest
 * @date 30/06/2015
 * @author Giuseppe Ferro
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
#include "Sleep.h"

using namespace MARTe;

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
     * @details When the BasicConsoleTest is created, automatically is set numberOfRows = 40
     * numberOfColumns = 60.
     */
    BasicConsoleTest() {
        numberOfRows = 40;
        numberOfColumns = 60;
    }

    /**
     * @brief Tests the BasicConsole constructor.
     * @return true if the initial size of the console is set to zero.
     */
    bool TestConstructor();

    /**
     * @brief Tests the correct opening of the console with default mode.
     * @param[in] openingMode is a flag with define the properties of the console.
     * @return true if the returned value of BasicConsole.Open() is ErrorManagement::NoError.
     */
    bool TestOpenModeDefault(FlagsType openingMode = BasicConsoleMode::Default);

    /**
     * @brief Tests the correct opening of the console with CreateNewBuffer mode.
     * @return true if the returned value of BasicConsole.Open() is ErrorManagement::NoError.
     */
    bool TestOpenModeCreateNewBuffer();

    /**
     * @brief Tests the correct opening of the console with PerformCharacterInput mode.
     * @return true if the returned value of BasicConsole.Open() is ErrorManagement::NoError.
     */
    bool TestOpenModePerformCharacterInput();

    /**
     * @brief Tests the correct opening of the console with DisableControlBreak mode.
     * @return true if the returned value of BasicConsole.Open() is ErrorManagement::NoError.
     */
    bool TestOpenModeDisableControlBreak();

    /**
     * @brief Tests the correct opening of the console with EnablePaging mode.
     * @return true if the returned value of BasicConsole.Open() is ErrorManagement::NoError.
     */
    bool TestOpenModeEnablePaging();

    /**
     * @brief Test BasicConsole::GetOpeningMode function.
     * @details Open consoles with different Modes Then checks if the returned value of GetOpeningMode
     * is consistent with the mode specified.
     * @return true if the returned value of the GetOpeningMode is the expected value.
     */
    bool TestGetOpeningMode();

    /**
     * @brief Test BasicConsole::Close() function.
     * @details Open consoles and then close. Moreover the test tries to close a closed console.
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
     * @return true if the BasicConsol::write() returns ErrorManagement::NoError for all open modes.
     */
    bool TestWriteCheckReturn(const char8 *string,
                              uint32 padding);
    /**
     * @brief Tests BasicConsole::Write() function.
     * @details Checks for the mode Default, the returned value of the BasicConsole::Write() function when
     * tries to write a NULL string on the console
     * @return true if the BasicConsol::write() returns ErrorManagement::Warning.
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
     * @return true if BasicConsol::Write() returns ErrorManagement::NoError. Also can be visually check that is written
     * in the right place
     */
    bool TestPaging(uint32 overflow,
                    uint32 rows,
                    uint32 columns);

    /**
     * @brief Tests BasicConsole::Read() function
     * @details Test if the string read by console is the same of the string passed by argument (the user must write in console the same word) with the same size.
     * @param[in] stringArg is the string to match with the read string.
     * @return true if the read string is equal to the passed argument.
     */
    bool TestRead(const char8 *stringArg);

    /**
     * @brief Tests BasicConsole::Read passing a finite timeout.
     * @param[in] timeout is the desired timeout.
     * @return true if the read function returns after the timeout expire, false otherwise.
     */
    bool TestTimeoutRead(uint32 timeout);

    /**
     * @brief Tests the BasicConsole::SetSize and BasicConsole::GetSize functions
     * @param[in] numberOfColumns is the desired number of columns.
     * @param[in] numberOfRows is the desired number of rows
     * @return true if the get function returns the values set by the set function.
     */
    bool TestSetGetSceneSize(uint32 numberOfRows,
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
     * @brief Tests the BasicConsole::ShowBuffer function.
     * @return the return value of the tested function.
     */
    bool TestShowBuffer();

    /**
     * @brief Tests the BasicConsole::SetTitleBar function
     * @param[in] title is the desired title.
     * @return the return value of the tested function.
     */
    bool TestSetGetTitleBar(const char8 *title);

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
     * @param[in] column is the desired character x position.
     * @param[in] row is the desired character y position.
     * @return the return value of the tested function
     */
    bool TestPlotChar(char8 c,
                      Colours foregroundColour,
                      Colours backgroundColour,
                      int column,
                      int row);


    bool TestCanWrite();

    bool TestCanRead();

    bool TestCanSeek();

    bool TestTimeoutWrite(const char8 *string, TimeoutType timeout);

    bool TestSize();

    bool TestSeek();

    bool TestRelativeSeek();

    bool TestPosition();

    bool TestSetSize();

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

