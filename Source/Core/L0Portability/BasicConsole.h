/**
 * @file BasicConsole.h
 * @brief Header file for class BasicConsole
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

 * @details This header file contains the declaration of the class BasicConsole
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef BASICCONSOLE_H_
#define 		BASICCONSOLE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"
#include "TimeoutType.h"

class BasicConsole;

/** Flags determining the console operating modes */
enum ConsoleOpeningMode {
    /** Default mode */
    ConsoleDefault = 0,

    /** WIN32 only: operates on a display buffer different from the current one */
    CreateNewBuffer = 1,

    /** Does not wait for newline */
    PerformCharacterInput = 2,

    /** CTRL-C does not create an exception */
    DisableControlBreak = 4,

    /** Enable paging */
    EnablePaging = 8
};

/**
 * @brief Allows combining ConsoleOpeningMode data doing a logical or between ConsoleOpeningMode flags.
 * @param[in] a is the first flag.
 * @param[in] b is the second flag.
 * */
static inline ConsoleOpeningMode operator|(ConsoleOpeningMode a,
                                           ConsoleOpeningMode b) {
    return (ConsoleOpeningMode) ((int) a | (int) b);
}

/**
 * @brief Allows combining ConsoleOpeningMode data doing a logical and between ConsoleOpeningMode flags.
 * @param[in] a is the first flag.
 * @param[in] b is the second flag.
 */
static inline ConsoleOpeningMode operator&(ConsoleOpeningMode a,
                                           ConsoleOpeningMode b) {
    return (ConsoleOpeningMode) ((int) a & (int) b);
}

/**
 * @brief Allows combining ConsoleOpeningMode data doing a not operator on a ConsoleOpeningMode flags.
 * @param[in] a is the flag which must be inverted.
 */
static inline ConsoleOpeningMode Not(ConsoleOpeningMode a) {
    return (ConsoleOpeningMode) (~(int) a);
}

extern "C" {

/**
 * @see BasicConsole::Open.
 */
bool BasicConsoleOpen(BasicConsole &con,
                      ConsoleOpeningMode openingMode,
                      int32 numberOfColumns,
                      int32 numberOfRows,
                      TimeoutType msecTimeout = TTInfiniteWait);

/**
 * @see BasicConsole::Show.
 */
bool BasicConsoleShow(BasicConsole &con);

/**
 * @see BasicConsole::Close.
 */
bool BasicConsoleClose(BasicConsole &con);

/**
 * @see BasicConsole::Write.
 */
bool BasicConsoleWrite(BasicConsole &con,
                       const void* buffer,
                       uint32 &size,
                       TimeoutType msecTimeout);

/**
 * @see BasicConsole::Read.
 */
bool BasicConsoleRead(BasicConsole &con,
                      void* buffer,
                      uint32 &size,
                      TimeoutType msecTimeout);

/**
 * @see BasicConsole::SetTitleBar.
 */
bool BasicConsoleSetTitleBar(BasicConsole &con,
                             const char8 *title);

/**
 * @see BasicConsole::SetSize.
 */
bool BasicConsoleSetSize(BasicConsole &con,
                         int32 numberOfColumns,
                         int32 numberOfRows);

/**
 * @see BasicConsole::GetSize.
 */
bool BasicConsoleGetSize(BasicConsole &con,
                         int32 &numberOfColumns,
                         int32 &numberOfRows);

/**
 * @see BasicConsole::SetWindowSize().
 */
bool BasicConsoleSetWindowSize(BasicConsole &con,
                               int32 numberOfColumns,
                               int32 numberOfRows);

/**
 * @see BasicConsole::GetWindowSize().
 */
bool BasicConsoleGetWindowSize(BasicConsole &con,
                               int32 &numberOfColumns,
                               int32 &numberOfRows);

/**
 * @see BasicConsole::SetCursorPosition.
 */
bool BasicConsoleSetCursorPosition(BasicConsole &con,
                                   int32 column,
                                   int32 row);

/** @see BasicConsole::GetCursorPosition. */
bool BasicConsoleGetCursorPosition(BasicConsole &con,
                                   int32 &column,
                                   int32 &row);

/**
 * @see BasicConsole::Clear.
 */
bool BasicConsoleClear(BasicConsole &con);

/**
 * @see BasicConsole::SetColour.
 */
bool BasicConsoleSetColour(BasicConsole &con,
                           Colours foreGroundColour,
                           Colours backGroundColour);

/**
 * @see BasicConsole::PlotChar.
 */
bool BasicConsolePlotChar(BasicConsole &con,
                          char8 c,
                          Colours foreGroundColour,
                          Colours backGroundColour,
                          int column,
                          int row);

}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Development of basic console operations.
 *
 * @details Most of the implementation is therefore delegated in BasicConsoleOS.h implementation
 * which is different for each operating system.
 *
 * @details These methods are particularly useful to obtain a first basic portable human interface,
 * where the user can interact with the system truth read and write operations allowing
 * for example commands, debugging ecc.
 */
class BasicConsole {

public:

    /** How long since last paging. */
    int64 lastPagingTime;

    /** How long to wait when reading. */
    TimeoutType msecTimeout;

    /** How many lines since last paging. */
    uint32 lineCount;

    /** The column counter. */
    uint32 colCount;

    /** Flag which describes the console status. */
    ConsoleOpeningMode openingMode;

    /** The number of columns. */
    int32 numberOfColumns;

    /** The number of rows */
    int32 numberOfRows;

    /** The input console handle */
    ConsoleHandle inputConsoleHandle;

    /** The output console handle */
    ConsoleHandle outputConsoleHandle;

    /** Initial settings of the console. The destructor will restores this initial configurations. */
    ConsoleInfo initialInfo;

private:

    friend bool BasicConsoleOpen(BasicConsole &con,
                                 ConsoleOpeningMode openingMode,
                                 int32 numberOfColumns,
                                 int32 numberOfRows,
                                 TimeoutType msecTimeout);

    friend bool BasicConsoleClose(BasicConsole &con);
    friend bool BasicConsoleShow(BasicConsole &con);
    friend bool BasicConsoleWrite(BasicConsole &con,
                                  const void* buffer,
                                  uint32 &size,
                                  TimeoutType msecTimeout);
    friend bool BasicConsoleRead(BasicConsole &con,
                                 void* buffer,
                                 uint32 &size,
                                 TimeoutType msecTimeout);
    friend bool BasicConsoleSetTitleBar(BasicConsole &con,
                                        const char8 *title);
    friend bool BasicConsoleSetSize(BasicConsole &con,
                                    int32 numberOfColumns,
                                    int32 numberOfRows);
    friend bool BasicConsoleGetSize(BasicConsole &con,
                                    int32 &numberOfColumns,
                                    int32 &numberOfRows);
    friend bool BasicConsoleSetCursorPosition(BasicConsole &con,
                                              int32 column,
                                              int32 row);
    friend bool BasicConsoleSetWindowSize(BasicConsole &con,
                                          int32 numberOfColumns,
                                          int32 numberOfRows);
    friend bool BasicConsoleGetWindowSize(BasicConsole &con,
                                          int32 &numberOfColumns,
                                          int32 &numberOfRows);
    friend bool BasicConsoleGetCursorPosition(BasicConsole &con,
                                              int32 &column,
                                              int32 &row);
    friend bool BasicConsoleClear(BasicConsole &con);
    friend bool BasicConsoleSetColour(BasicConsole &con,
                                      Colours foreGroundColour,
                                      Colours backGroundColour);

    friend bool BasicConsolePlotChar(BasicConsole &con,
                                     char8 c,
                                     Colours foreGroundColour,
                                     Colours backGroundColour,
                                     int32 column,
                                     int32 row);

public:

    /**
     * @brief Constructor.
     * @details Sets the console opening mode, the number of rows and columns, and the timeout for read and write operations.
     * @param[in] openingMode is a combination of the ConsoleOpeningMode flags.
     * @param[in] numberOfColumns is the number of columns for the new console.
     * @param[in] numberOfRows is the number of rows for the new console.
     * @param[in] msecTimeout is the timeout.
     * @return true if the initialization (at system level) of the console goes fine.
     */
     BasicConsole(ConsoleOpeningMode openingMode = ConsoleDefault,
                 int32 numberOfColumns = -1,
                 int32 numberOfRows = -1,
                 TimeoutType msecTimeout = TTInfiniteWait);

    /**
     * @brief Destructor.
     * @details Closes the console after restored its previous configuration.
     */
    virtual ~BasicConsole();

    /**
     * @brief Switches to display this console buffer.
     * @details It is possible to write to an inactive screen buffer and then use this function to display the buffer's contents.
     * @return true if successful, false otherwise.
     */
    inline bool Show();

    /**
     * @brief Write operation on this console.
     * @details A specified number of bytes of the buffer passed by argument is written on the console .
     * If the mode is paging, it writes until the number of rows of the console then advises the user to press enter for the pagination.\n
     * It writes characters on the same row until they are less than the number of columns of the buffer size, then it
     * writes automatically on the next row.
     * @param[in] buffer is the data to write on the console.
     * @param[in,out] size is maximum size in byte to write on the console. If a minor number of bytes is written,
     * size become the number of bytes written.
     * @param[in] msecTimeout is the timeout.
     * @return true if a number greater than 0 of bytes is written.
     */
    inline bool Write(const void* buffer,
                      uint32 & size,
                      TimeoutType msecTimeout=TTInfiniteWait);

    /**
     * @brief Read operation on this console.
     * @details Reads size bytes from the console and puts them on the buffer passed by argument.
     * If PerformCharacterInput mode is enabled, returns immediately.
     * @param[in] buffer is the buffer where the read bytes are written.
     * @param[in,out] size is the number of bytes to read.
     * @param[in] msecTimeout is the desired timeout.
     * @return true if at least one byte it is read.
     */
    inline bool Read(void* buffer,
                     uint32 & size,
                     TimeoutType msecTimeout=TTInfiniteWait);

    /**
     * @brief Set Title Bar text.
     * @param[in] title is the desired title.
     * @return true if successful, false otherwise.
     */
    inline bool SetTitleBar(const char8 *title);

    /**
     * @brief Set the buffer size of this console.
     * @param[in] numberOfColumns is the number of columns to set in the console.
     * @param[in] numberOfRows is the number of rows to set in the console.
     * @return true if successful, false otherwise.
     */
    inline bool SetSize(int32 numberOfColumns,
                        int32 numberOfRows);

    /**
     * @brief Get the buffer size of this console.
     * @param[out] numberOfColumns is the width of the buffer.
     * @param[out] numberOfRows is the height of the buffer.
     * @return true if successful, false otherwise.
     */
    inline bool GetSize(int32 & numberOfColumns,
                        int32 & numberOfRows);

    /**
     * @brief Sets the window size.
     * @param[in] numberOfColumns is the desired window width.
     * @param[in] numberOfRows is the desired window height.
     * @return true if successful, false otherwise.
     */
    inline bool SetWindowSize(int32 numberOfColumns,
                              int32 numberOfRows);

    /**
     * @brief Returns the size of the window.
     * @param[out] numberOfColumns is the window width.
     * @param[out] numberOfRows is the window height.
     * @return true if successful, false otherwise.
     */
    inline bool GetWindowSize(int32 & numberOfColumns,
                              int32 & numberOfRows);

    /**
     * @brief Sets the cursor position.
     * @param[in] column is the desired cursor x coordinate.
     * @param[in] row is the desired cursor y coordinate.
     * @return true if successful, false otherwise.
     */
    inline bool SetCursorPosition(int32 column,
                                  int32 row);

    /**
     * @brief Gets the cursor position.
     * @param[out] column is the x coordinate of the cursor in return.
     * @param[out] row is the y coordinate of the cursor in return.
     * @return true if successful, false otherwise.
     */
    inline bool GetCursorPosition(int32 & column,
                                  int32 & row);

    /**
     * @brief Sets the font foreground and background colors.
     * @param[in] foreGroundColour is the desired foreground color.
     * @param[in] backGroundColour is the desired background color.
     * @return true if successful, false otherwise.
     */
    inline bool SetColour(Colours foreGroundColour,
                          Colours backGroundColour);

    /**
     * @brief Clears the screen.
     * @return true if successful, false otherwise.
     */
    inline bool Clear();

    /**
     * @brief Enables or disables the paging mode using the operators.
     * @param[in] enable is true to activate paging mode, false otherwise.
     */
    inline void SetPaging(bool enable);

    /**
     * @brief Writes a single char8 on the console at a given position and with given color set.
     * @param[in] c is the character to be printed.
     * @param[in] foreGroundColour is the desired console foreground color.
     * @param[in] backGroundColour is the desired console background color.
     * @param[in] column is the desired x-axis position of the character.
     * @param[in] row is the desired y-axis position of the character.
     * @return true if successful, false otherwise.
     */
    inline bool PlotChar(char8 c,
                         Colours foreGroundColour,
                         Colours backGroundColour,
                         int column,
                         int row);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

bool BasicConsole::Show() {
    return BasicConsoleShow(*this);
}


bool BasicConsole::Write(const void* buffer,
                  uint32 & size,
                  TimeoutType msecTimeout) {
    return BasicConsoleWrite(*this, buffer, size, msecTimeout);
}


bool BasicConsole::Read(void* buffer,
                 uint32 & size,
                 TimeoutType msecTimeout) {
    return BasicConsoleRead(*this, buffer, size, msecTimeout);
}

bool BasicConsole::SetTitleBar(const char8 *title) {
    return BasicConsoleSetTitleBar(*this, title);
}

bool BasicConsole::SetSize(int32 numberOfColumns,
                    int32 numberOfRows) {
    return BasicConsoleSetSize(*this, numberOfColumns, numberOfRows);
}


bool BasicConsole::GetSize(int32 & numberOfColumns,
                    int32 & numberOfRows) {
    return BasicConsoleGetSize(*this, numberOfColumns, numberOfRows);
}


bool BasicConsole::SetWindowSize(int32 numberOfColumns,
                          int32 numberOfRows) {
    return BasicConsoleSetWindowSize(*this, numberOfColumns, numberOfRows);
}


bool BasicConsole::GetWindowSize(int32 & numberOfColumns,
                          int32 & numberOfRows) {
    return BasicConsoleGetWindowSize(*this, numberOfColumns, numberOfRows);
}


bool BasicConsole::SetCursorPosition(int32 column,
                              int32 row) {
    return BasicConsoleSetCursorPosition(*this, column, row);
}


bool BasicConsole::GetCursorPosition(int32 & column,
                              int32 & row) {
    return BasicConsoleGetCursorPosition(*this, column, row);
}


bool BasicConsole::SetColour(Colours foreGroundColour,
                      Colours backGroundColour) {
    return BasicConsoleSetColour(*this, foreGroundColour, backGroundColour);
}


bool BasicConsole::Clear() {
    lineCount = 0;
    return BasicConsoleClear(*this);
}


void BasicConsole::SetPaging(bool enable) {
    lineCount = 0;
    if (enable) {
        openingMode = openingMode | EnablePaging;
    }
    else {
        openingMode = openingMode & Not(EnablePaging);
    }
}


bool BasicConsole::PlotChar(char8 c,
                     Colours foreGroundColour,
                     Colours backGroundColour,
                     int column,
                     int row) {
    return BasicConsolePlotChar(*this, c, foreGroundColour, backGroundColour, column, row);
}


#endif /* BASICCONSOLE_H_ */

