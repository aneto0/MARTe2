/**
 * @file BasicConsoleOS.h
 * @brief Header file for class BasicConsoleOS
 * @date 22/06/2015
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

 * @details This header file contains the declaration of the class BasicConsoleOS
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICCONSOLEOS_H_
#define BASICCONSOLEOS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#ifndef LINT
#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#else
#include "lint-linux.h"
#endif
/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "../../IBasicConsole.h"
#include "../../GeneralDefinitions.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Linux BasicConsole implementation.
 * @details Uses the basic UNIX standard I/O streams to implement the console communication.\n
 * Many of the advanced features (e.g. colour) are not available.
 */
class BasicConsoleOS: public IBasicConsole {

public:

    /**
     * @brief Sets the console with the default parameters.
     * @details The openingMode is set to IBasicConsole::Mode::Default and
     * the number of rows and columns to zero.
     */
    BasicConsoleOS();

    /**
     * @brief Closes the console.
     */
    ~BasicConsoleOS();

    /**
     * @copydetails IBasicConsole::Open
     * @pre In Linux setting the size before opening the console is required.
     */
    virtual bool Open(const FlagsType &mode);

    /**
     * @copydetails IBasicConsole::GetOpeningMode
     */
    virtual FlagsType GetOpeningMode() const;

    /**
     * @copydetails IBasicConsole::Close
     */
    virtual bool Close();

    /**
     * @brief Writes to the STDOUT.
     * @copydetails BasicConsole::Write
     */
    virtual bool Write(const char8 * const buffer, uint32 & size, const TimeoutType &timeout);

    /**
     * @brief Reads from the STDIN.
     * @copydetails IBasicConsole::Read
     */
    virtual bool Read(char8 * const  buffer, uint32 & size, const TimeoutType &timeout);

    /**
     * @copydetails IBasicConsole::SetSize.
     * @return true.
     */
    virtual bool SetSize(const uint32 &numberOfColumns, const uint32 &numberOfRows);

    /**
     * @copydetails IBasicConsole::GetSize.
     * @return true.
     */
    virtual bool GetSize(uint32 &numberOfColumns, uint32 &numberOfRows) const;

    /**
     * @brief writes a series of newlines to the console output.
     * @copydetails IBasicConsole::Clear.
     */
    virtual bool Clear();
    /**
     * @copydetails IBasicConsole::CursorPositionSupported.
     * @return false.
     */
    virtual bool CursorPositionSupported() const;

    /**
     * @copydetails IBasicConsole::TitleBarSupported.
     * @return false.
     */
    virtual bool TitleBarSupported() const;

    /**
     * @copydetails IBasicConsole::ColourSupported.
     * @return false.
     */
    virtual bool ColourSupported() const;

    /**
     * @copydetails IBasicConsole::WindowSizeSupported.
     * @return false.
     */
    virtual bool WindowSizeSupported() const;

    /**
     * @copydetails IBasicConsole::ConsoleBufferSupported.
     * @return false.
     */
    virtual bool ConsoleBufferSupported() const;

    /**
     * @copydetails IBasicConsole::Show
     * @warning Not Implemented.
     * @return true.
     */
    virtual bool Show();

    /**
     * @copydetails IBasicConsole::SetColour
     * @warning Not Implemented.
     * @return true.
     */
    virtual bool SetColour(const Colours &foregroundColour, const Colours &backgroundColour);

    /**
     * @copydetails IBasicConsole::SetTitleBar.
     * @warning Not implemented.
     * @return true.
     */
    virtual bool SetTitleBar(const char8 * const title);

    /**
     * @copydetails IBasicConsole::GetTitleBar.
     * @warning Not implemented.
     * @return true.
     */
    virtual bool GetTitleBar(char8 * const title) const;

    /**
     * @copydetails IBasicConsole::SetCursorPosition.
     * @warning Not implemented.
     * @return true.
     */
    virtual bool SetCursorPosition(const uint32 &column, const uint32 &row);

    /**
     * @copydetails IBasicConsole::GetCursorPosition.
     * @warning Not implemented.
     * @return true.
     */
    virtual bool GetCursorPosition(uint32 &column, uint32 &row) const;

    /**
     * @copydetails IBasicConsole::SetWindowSize.
     * @warning Not implemented.
     * @return true.
     */
    virtual bool SetWindowSize(const uint32 &numberOfColumns, const uint32 &numberOfRows);

    /**
     * @copydetails IBasicConsole::GetWindowSize.
     * @warning Not implemented.
     * @return true.
     */
    virtual bool GetWindowSize(uint32 &numberOfColumns, uint32 &numberOfRows) const;

    /**
     * @copydetails IBasicConsole::PlotChar
     * @warning Not Implemented.
     * @return true.
     */
    virtual bool PlotChar(const char8 &c, const Colours &foregroundColour, const Colours &backgroundColour, const uint32 &column, const uint32 &row);

private:

    /**
     * Standard output file descriptor.
     */
    static const int32 STDOUT = 1;
    /**
     * Standard input file descriptor.
     */
    static const int32 STDIN = 0;
    /**
     * Number of rows that will be cleared when BasicConsoleOSClear is called
     */
    static const uint32 BASIC_CONSOLE_LINUX_CLEAR_ROWS = 40u;

    /**
     * @brief Read immediately.
     * @details Sets equal to 1 the characters in the read buffer and equal to zero the time to wait between read operations.\n
     * It does not wait for the newline char8 to be read.
     */
    void SetImmediateRead();

    /**
     * @brief Restore the console default settings.
     */
    void UnSetImmediateRead();

    /**
     * The input console handle
     */
    ConsoleHandle inputConsoleHandle;

    /**
     * The output console handle
     */
    ConsoleHandle outputConsoleHandle;

    /**
     * Initial settings of the console. The destructor will restores this initial configurations.
     */
    ConsoleHandle initialInfo;

    /**
     * The column counter keeps track of the column where the character was written to.
     */
    uint32 columnCount;

    /**
     * Flag which describes the console status.
     */
    FlagsType openingMode;

    /**
     * The number of columns currently set.
     */
    uint32 nOfColumns;

    /**
     * The number of rows currently set.
     */
    uint32 nOfRows;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* BASICCONSOLEOS_H_ */

