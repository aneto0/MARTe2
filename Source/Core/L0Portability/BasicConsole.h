/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 * $Id: $
 *
 **/
/**
 * @file
 * @brief The simplest human interface: a text terminal
 */
#ifndef BASIC_CONSOLE_H
#define BASIC_CONSOLE_H

#include "GeneralDefinitions.h"
#include "TimeoutType.h"

class BasicConsole;

/** flags determining the console operating modes */
enum ConsoleOpeningMode {
    /** Default mode */
    ConsoleDefault = 0,

    /** WIN32 only: operates on a display buffer different from the current one*/
    CreateNewBuffer = 1,

    /** Does not wait for \n\r */
    PerformCharacterInput = 2,

    /** CTRL-C does not create an exception */
    DisableControlBreak = 4,

    /** Enable paging */
    EnablePaging = 8
};

/** @brief Allows combining ConsoleOpeningMode data doing a logical or between ConsoleOpeningMode flags.
  * @param a is the first flag.
  * @param b is the second flag. */
static inline ConsoleOpeningMode operator|(ConsoleOpeningMode a,
                                           ConsoleOpeningMode b) {
    return (ConsoleOpeningMode) ((int) a | (int) b);
}

/** @brief Allows combining ConsoleOpeningMode data doing a logical and between ConsoleOpeningMode flags.
  * @param a is the first flag.
  * @param b is the second flaf. */
static inline ConsoleOpeningMode operator&(ConsoleOpeningMode a,
                                           ConsoleOpeningMode b) {
    return (ConsoleOpeningMode) ((int) a & (int) b);
}

/** @brief Allows combining ConsoleOpeningMode data doing a not operator on a ConsoleOpeningMode flags.
  * This function
  * @param a is the flag which must be inverted. */
static inline ConsoleOpeningMode Not(ConsoleOpeningMode a) {
    return (ConsoleOpeningMode) (~(int) a);
}

extern "C" {
/** @brief Set the console opening mode, the number of rows and columns, and the timeout for read and write operations.
  * @param con is new console to open.
  * @param openingMode is a combination of the ConsoleOpeningMode flags.
  * @param numberOfComumns is the number of columns for the new console.
  * @param numberOfRows is the number of rows for the new console.
  * @param msecTimeout is the timeout.
  * @return true if the initialization (at system api level) of the console does not return errors. */
bool BasicConsoleOpen(BasicConsole &con, ConsoleOpeningMode openingMode,
                      int numberOfColumns, int numberOfRows,
                      TimeoutType msecTimeout = TTInfiniteWait);

/** @brief Not implemented in Linux.
  * @return true. */
bool BasicConsoleShow(BasicConsole &con);

/** @brief Close terminal con.
  * @param con is the console to close.
  * @return true. */
bool BasicConsoleClose(BasicConsole &con);

/** @brief Write operation on the console con.
  * The buffer passed by argument is written on the console until a /0 char is
  * found or for the size passed by argument. If the mode is paging, it writes until the
  * number of rows of the console then advices the user to press enter for the pagination.
  * It writes characters on the same row until they are less than the number of columns, then it
  * writes automatically on the next row.
  * @param con is the console.
  * @param buffer is the data to write on the console.
  * @param size is maximum size in byte to write on the console. If a minor number of bytes is written,
  * size become the number of bytes written.
  * @param msecTimeout is the timeout (not implemented in linux).
  * @return true if a number greater than 0 of bytes is written. */
bool BasicConsoleWrite(BasicConsole &con, const void* buffer, uint32 &size,
                       TimeoutType msecTimeout);

/** @brief Read operation on the console con.
  * Reads size bytes from the console and puts them on the buffer passed by argument.
  * If PerformCharacterInput mode is enabled, returns after one character read.
  * @param con is the console.
  * @param buffer is the buffer where the read bytes are written.
  * @param size is the number of bytes to read.
  * @param msecTimeout is the timeout (not implemented in linux).
  * @return true if at least one byte it is read.*/
bool BasicConsoleRead(BasicConsole &con, void* buffer, uint32 &size,
                      TimeoutType msecTimeout);

/** @brief Not implemented.  */
bool BasicConsoleSetTitleBar(BasicConsole &con, const char *title);

/** @brief Set the size of the console con.
  * @param con is the console.
  * @param numberOfColumns is the number of columns to set in the console.
  * @param numberOfRows is the number of rows to set in the console.
  * @return true. */
bool BasicConsoleSetSize(BasicConsole &con, int numberOfColumns,
                         int numberOfRows);

/** @brief Get the size of the console con.
  * @param con is the console.
  * @param numberOfColumns is the number of columns to set in the console.
  * @param numberOfRows is the number of rows to set in the console.
  * @return true. */
bool BasicConsoleGetSize(BasicConsole &con, int &numberOfColumns,
                         int &numberOfRows);

/** @see BasicConsoleSetSize(). */
bool BasicConsoleSetWindowSize(BasicConsole &con, int numberOfColumns,
                               int numberOfRows);

/** @see BasicConsoleGetSize(). */
bool BasicConsoleGetWindowSize(BasicConsole &con, int &numberOfColumns,
                               int &numberOfRows);

/** @brief Not implemented. */
bool BasicConsoleSetCursorPosition(BasicConsole &con, int column, int row);

/** @brief Not implemented. */
bool BasicConsoleGetCursorPosition(BasicConsole &con, int &column, int &row);

/** @brief Clear the console window.
  * Go to the next row nOfColumns times.
  * @param con is the console.
  * @return true. */
bool BasicConsoleClear(BasicConsole &con);

/** @brief Not implemented. */
bool BasicConsoleSetColour(BasicConsole &con, Colours foreGroundColour,
                           Colours backGroundColour);

/** @brief Not implemented. */
bool BasicConsoleSwitch(BasicConsole &con, const char *name);

/** @brief Not implemented. */
bool BasicConsolePlotChar(BasicConsole &con, char c, Colours foreGroundColour,
                          Colours backGroundColour, int column, int row);

}

/**
 * @brief Development of read and write operation on a console.
 *
 * Most of the implementation is therefore delegated in BasicConsoleOS.h implementation
 * which is different for each operating system due to implement portable functions.
 *
 * These methods are particularly useful to obtain a first basic portable human interface,
 * where the user can interact with the system truth read and write operations allowing
 * for example commands, debugging ecc.
 */


class BasicConsole {


    /** how long since last paging. */
    int64 lastPagingTime;

   /** how long to wait when reading. */
    TimeoutType msecTimeout;

public:
     /** how many lines since last paging. */
    uint32 lineCount;

    /** the column counter. */
    uint32 colCount;

    /** sets of flags describing the console status. */
    ConsoleOpeningMode openingMode;

    /** sets the number of rows and columns. */
    int32 numberOfColumns;

    int32 numberOfRows;

    /** input and output console handles (aka void*), used in windows. */
    HANDLE inputConsoleHandle;
    HANDLE outputConsoleHandle;


    /** to save the original console parameters in case of modifications.*/
    struct DefaultParam {
	uint32 minReadByte;
	uint32 minTimeBetweenInputs;
    } consoleDefaultParam;


private:

    friend bool BasicConsoleOpen(BasicConsole &con,
                                 ConsoleOpeningMode openingMode,
                                 int32 numberOfColumns, int32 numberOfRows,
                                 TimeoutType msecTimeout);

    friend bool BasicConsoleClose(BasicConsole &con);
    friend bool BasicConsoleShow(BasicConsole &con);
    friend bool BasicConsoleWrite(BasicConsole &con, const void* buffer,
                                  uint32 &size, TimeoutType msecTimeout);
    friend bool BasicConsoleRead(BasicConsole &con, void* buffer, uint32 &size,
                                 TimeoutType msecTimeout);
    friend bool BasicConsoleSetTitleBar(BasicConsole &con, const char *title);
    friend bool BasicConsoleSetSize(BasicConsole &con, int32 numberOfColumns,
                                    int32 numberOfRows);
    friend bool BasicConsoleGetSize(BasicConsole &con, int32 &numberOfColumns,
                                    int32 &numberOfRows);
    friend bool BasicConsoleSetCursorPosition(BasicConsole &con, int32 column,
                                              int32 row);
    friend bool BasicConsoleSetWindowSize(BasicConsole &con,
                                          int32 numberOfColumns,
                                          int32 numberOfRows);
    friend bool BasicConsoleGetWindowSize(BasicConsole &con,
                                          int32 &numberOfColumns,
                                          int32 &numberOfRows);
    friend bool BasicConsoleGetCursorPosition(BasicConsole &con, int32 &column,
                                              int32 &row);
    friend bool BasicConsoleClear(BasicConsole &con);
    friend bool BasicConsoleSetColour(BasicConsole &con,
                                      Colours foreGroundColour,
                                      Colours backGroundColour);
    friend bool BasicConsoleSwitch(BasicConsole &con, const char *name);
    friend bool BasicConsolePlotChar(BasicConsole &con, char c,
                                     Colours foreGroundColour,
                                     Colours backGroundColour, int32 column,
                                     int32 row);

public:

    /**
     * @brief Constructor.
     * @see BasicConsoleOpen() */
    BasicConsole(ConsoleOpeningMode openingMode = ConsoleDefault,
                 int32 numberOfColumns = -1, int32 numberOfRows = -1,
                 TimeoutType msecTimeout = TTInfiniteWait) {
        BasicConsoleOpen(*this, openingMode, numberOfColumns, numberOfRows,
                         msecTimeout);
    }

    /** @brief Destructor  */
    virtual ~BasicConsole() {
        BasicConsoleClose(*this);
    }

    /** @brief Switches to display this console buffer.
      * @see BasicConsoleShow(). */
    inline bool Show() {
        return BasicConsoleShow(*this);
    }

    /** @brief The actual Write.
	@see BasicConsoleWrite(). */
    inline bool Write(const void* buffer, uint32 & size,
                      TimeoutType msecTimeout) {
        return BasicConsoleWrite(*this, buffer, size, msecTimeout);
    }

    /** @brief The actual Read.
      * @see BasicConsoleRead().  */
    inline bool Read(void* buffer, uint32 & size, TimeoutType msecTimeout) {
        return BasicConsoleRead(*this, buffer, size, msecTimeout);
    }


    /** @brief Set Title Bar text.
      * @see BasicConsoleSetTitleBar(). */
    inline bool SetTitleBar(const char *title) {
        return BasicConsoleSetTitleBar(*this, title);
    }

    /** @brief Sets the size of the buffer.
      * @see BasicConsoleSetSize(). */
    inline bool SetSize(int32 numberOfColumns, int32 numberOfRows) {
        return BasicConsoleSetSize(*this, numberOfColumns, numberOfRows);
    }

    /** @brief Retrieves the size of the console.
      * @see BasicConsoleGetSize(). */
    inline bool GetSize(int32 & numberOfColumns, int32 & numberOfRows) {
        return BasicConsoleGetSize(*this, numberOfColumns, numberOfRows);
    }

    /** @brief Sets the size of the window.
      * @see BasicConsoleSetWindowSize(). */
    inline bool SetWindowSize(int32 numberOfColumns, int32 numberOfRows) {
        return BasicConsoleSetWindowSize(*this, numberOfColumns, numberOfRows);
    }

    /** @brief Returns the size of the window.
      * @see BasicConsoleGetWindowSize()*/
    inline bool GetWindowSize(int32 & numberOfColumns, int32 & numberOfRows) {
        return BasicConsoleGetWindowSize(*this, numberOfColumns, numberOfRows);
    }

    /** @brief Sets the size position of the cursor.
      * @see BasicConsoleSetCursorPosition(). */
    inline bool SetCursorPosition(int32 column, int32 row) {
        return BasicConsoleSetCursorPosition(*this, column, row);
    }

    /** @brief Retrieves the size position of the cursor.
      * @see BasicConsoleGetCursorPosition(). */
    inline bool GetCursorPosition(int32 & column, int32 & row) {
        return BasicConsoleGetCursorPosition(*this, column, row);
    }

    /** @brief Sets the font fg and bg colours.
      * @see BasicConsoleSetColour(). */
    inline bool SetColour(Colours foreGroundColour, Colours backGroundColour) {
        return BasicConsoleSetColour(*this, foreGroundColour, backGroundColour);
    }

    /** @brief Clears content.
      * @see BasicConsoleClear(). */
    inline bool Clear() {
        lineCount = 0;
        return BasicConsoleClear(*this);
    }

    /** @brief Enable or Disable the paging mode using the operators.
      * @param enable is true to activate paging mode, false otherwise. */
    inline void SetPaging(bool enable) {
        lineCount = 0;
        if (enable) {
            openingMode = openingMode | EnablePaging;
        }
        else {
            openingMode = openingMode & Not(EnablePaging);
        }
    }

    /** @brief write a single char on the console at a given position and a given colour set.
      * @see BasicConsolePlotChar. */
    inline bool PlotChar(char c, Colours foreGroundColour,
                         Colours backGroundColour, int column, int row) {
        return BasicConsolePlotChar(*this, c, foreGroundColour,
                                    backGroundColour, column, row);
    }

};

#endif

