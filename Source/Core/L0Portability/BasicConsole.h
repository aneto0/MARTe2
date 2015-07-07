/**
 * @file BasicConsole.h
 * @brief Header file for class IBasicConsole
 * @date 04/07/2015
 * @author Andre' Neto
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
#define BASICCONSOLE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "TimeoutType.h"
#include "FlagsType.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * Forward declaration of the properties which are operating system specific.
 */
struct OperatingSystemProperties;
/**
 * @brief Interface class for the basic console operations.
 *
 * @details The framework offers portable basic console support. The specificities
 * of the behaviour are delegated to the underlying operating system. Not all of
 * the features are necessarily implemented in all the operating systems.
 */
class BasicConsole {

public:
    /**
     * @brief Default constructor. No operation.
     */
    BasicConsole();

    /**
     * @brief Default destructor. No operation.
     */
    virtual ~BasicConsole();

    /**
     * FlagsType determining the console operating modes.\n
     * These are usually set when the console is constructed.
     */
    struct Mode {
        /**
         * Default mode.
         */
        static const FlagsType Default;
        /**
         * WIN32 only: operates on a display buffer different from the current one.
         */
        static const FlagsType CreateNewBuffer;
        /**
         * Does not wait for a newline when the user enters input.
         */
        static const FlagsType PerformCharacterInput;
        /**
         * CTRL-C does not raise an exception.
         */
        static const FlagsType DisableControlBreak;
        /**
         * Enables paging.
         */
        static const FlagsType EnablePaging;
    };

    /**
     * @brief Open and display the console.
     * @details Opens the console for display with the specified number of rows and columns.
     * @param[in] mode the desired opening mode, which changes some behaviours of the console.
     * @return true if the console is opened correctly.
     * @pre Even if the behaviour will be operating system dependent, it is advisable to call
     * SetSize(numberOfColumns, numberOfRows) before opening the console.
     */
    virtual bool Open(const FlagsType &mode);

    /**
     * @brief Retrieves the mode that was used to open the console
     * @return the openingMode used to open the console.
     * @pre the behaviour will be unspecified and operating system dependent if this method
     * is called on a console that was never open.
     */
    virtual FlagsType GetOpeningMode() const;

    /**
     * @brief Closes the console.
     * @details Closes the console and leaves it in a state where it can be opened again.
     * @return true if the console could be successfully closed.
     */
    virtual bool Close();

    /**
     * @brief Writes to the console.
     * @details A specified number of bytes of the buffer are written to the console output.
     * @param[in] buffer memory data to be written on the console.
     * @param[in,out] size maximum size in bytes to write on the console. If a minor number of bytes is written,
     * size will contain the number of bytes actually written.
     * @param [in] timeout maximum time to wait for writing to the console.
     * @return true if a number of bytes greater than 0 is successfully written and if a timeout has not occurred.
     */
    virtual bool Write(const char8 * const buffer, uint32 & size, const TimeoutType &timeout);

    /**
     * @brief Reads from the console.
     * @details Reads from the console input into the buffer.
     * If PerformCharacterInput mode is enabled, it returns without waiting for LF.
     * @param[in] buffer memory space where the read bytes are written into.
     * @param[in,out] size number of bytes to read.
     * @param[in] timeout maximum time to wait for the operation to be successfully completed.
     * @return true if at least one byte is read.
     */
    virtual bool Read(char8 * const buffer, uint32 & size, const TimeoutType &timeout);

    /**
     * @brief Update the console size.
     * @param[in] numberOfColumns number of columns to set in the console.
     * @param[in] numberOfRows number of rows to set in the console.
     * @return true if the console can be resized to the new size.
     */
    virtual bool SetSize(const uint32 &numberOfColumns, const uint32 &numberOfRows);

    /**
     * @brief Returns the current console size.
     * @param[out] numberOfColumns currently number of columns being displayed.
     * @param[out] numberOfRows currently number of rows being displayed.
     * @return true if the size can be successfully retrieved.
     */
    virtual bool GetSize(uint32 & numberOfColumns, uint32 & numberOfRows) const;

    /**
     * @brief Sets the console window size (@see WindowSizeSupported).
     * @details The console window size must be smaller than the console size (@see GetSize and @see SetSize).\n
     * The actual implementation details depend on the operating system but it is expected that a window
     * will enable scroll bars if its size is smaller than the underlying console size.
     * @param[in] numberOfColumns desired x axis size.
     * @param[in] numberOfRows desired y axis size.
     * @return true if the window size was successfully set or if WindowSizeSupported returns false.
     */
    virtual bool SetWindowSize(const uint32 &numberOfColumns, const uint32 &numberOfRows);

    /**
     * @brief Returns the window size.
     * @param[out] numberOfColumns is the x axis window size in return.
     * @param[out] numberOfRows is the y axis window size in return.
     * @return true if the size can be retrieved of if WindowSizeSupported returns false.
     */
    virtual bool GetWindowSize(uint32 &numberOfColumns, uint32 &numberOfRows) const;

    /**
     * @brief Checks if it possible to change the window size.
     * @return true if the operating system implementation supports reading and
     * modifying the window size.
     */
    virtual bool WindowSizeSupported() const;

    /**
     * @brief Sets the cursor position (@see CursorPositionSupported).
     * @param[in] column is the desired cursor x coordinate.
     * @param[in] row is the desired cursor y coordinate.
     * @return true if the cursor can be successfully set in the new coordinate
     * or if CursorPositionSupported is false.
     */
    virtual bool SetCursorPosition(const uint32 &column, const uint32 &row);

    /**
     * @brief Retrieves the cursor position.
     * @param[out] column is the current x coordinate of the cursor.
     * @param[out] row is the current y coordinate of the cursor.
     * @return true if the cursor coordinate can be successfully retrieved or
     * if CursorPositionSupported is false.
     */
    virtual bool GetCursorPosition(uint32 & column, uint32 & row) const;

    /**
     * @brief Checks if it possible to interact with the console cursor position.
     * @return true if the operating system implementation supports reading and
     * modifying the cursor position.
     */
    virtual bool CursorPositionSupported() const;

    /**
     * @brief Switches to display this console buffer (@see ConsoleBufferSupported).
     * @details It is possible to write to an inactive screen buffer and then use this function to display the buffer's contents.
     * @return true if the buffer is successfully displayed or if ConsoleBufferSupported is false;
     */
    virtual bool Show();

    /**
     * @brief Checks if it possible to use an inactive console buffer for writing.
     * @return true if the operating system implementation supports double buffering.
     */
    virtual bool ConsoleBufferSupported() const;

    /**
     * @brief Sets the font foreground and background colours.
     * @param[in] foregroundColour the desired foreground colour.
     * @param[in] backgroundColour the desired background colour.
     * @return true if the colours can be successfully set or if ColourSupported is false.
     */
    virtual bool SetColour(const Colours &foregroundColour, const Colours &backgroundColour);

    /**
     * @brief Checks if colour changing is supported by the operating system implementation.
     * @return true if the operating system implementation allows to change the console background
     * and foreground colours.
     */
    virtual bool ColourSupported() const;

    /**
     * @brief Clears the console output.
     * @return true if the console can be successfully cleared.
     */
    virtual bool Clear();

    /**
     * @brief Writes a single char on the console at a given position and with the given color set.
     * @param[in] c is the character to be printed.
     * @param[in] foregroundColour is the desired console foreground colour.
     * @param[in] backgroundColour is the desired console background colour.
     * @param[in] column is the desired x-axis position of the character.
     * @param[in] row is the desired y-axis position of the character.
     * @return true if successful the char is successfully plotted with the correct attributes or if any of the following
     * returns false: CursorPositionSupported, ColourSupported.
     */
    virtual bool PlotChar(const char8 &c, const Colours &foregroundColour, const Colours &backgroundColour, const uint32 &column, const uint32 &row);

    /**
     * @brief Update the console title (@see TitleBarSupported).
     * @param[in] title is the desired title.
     * @return true if the title can be successfully set or if TitleBarSupported is false.
     */
    virtual bool SetTitleBar(const char8 *title);

    /**
     * @brief Returns the console title (@see TitleBarSupported).
     * @param[out] title is a destination buffer.
     * @return true if the title can be successfully retrieved
     * or if TitleBarSupported is false;
     */
    virtual bool GetTitleBar(char8 *title) const;

    /**
     * @brief Checks if the changing or reading the console title is supported
     * @return true if the operating system implementation enables the reading and updating
     * of the console title.
     */
    virtual bool TitleBarSupported() const;

    /**
     * @brief Portable paged write implementation.
     * @details If the mode is paging, it writes until the number of rows of the console then advises
     * the user to press enter for the pagination.\n
     * It writes characters on the same row until they are less than the number of columns of the buffer size, then it
     * writes automatically on the next row.
     * @param[in] buffer is the data to write on the console.
     * @param[in,out] size is maximum size in byte to write on the console. If a minor number of bytes is written,
     * size become the number of bytes written.
     * @param[in] msecTimeout is the timeout.
     */
    inline bool PagedWrite(const char8* const buffer, const uint32 &size, const TimeoutType &timeout);

private:
    /**
     * Operating system specific properties to be used by the operating system specific implementation
     */
    OperatingSystemProperties *osProperties;

    /** How long since last paging. */
    int64 lastPagingCounter;

    /** How many lines since last paging. */
    uint32 lineCount;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
bool BasicConsole::PagedWrite(const char8 * const buffer, const uint32 &size, const TimeoutType &timeout) {

    uint32 numberOfColumnsTmp = 0u;
    uint32 numberOfRowsTmp = 0u;
    bool ok = GetSize(numberOfColumnsTmp, numberOfRowsTmp);
    if (ok) {

        //-1 means the maximum size.
        uint32 numberOfRows = numberOfRowsTmp;

        int64 t0 = lastPagingCounter;
        int64 t1 = HighResolutionTimer::Counter();

        int64 dTicks = t1 - t0;
        float64 dTime = static_cast<float64>(dTicks) * HighResolutionTimer::Period();
        if (dTime > 0.05) {
            lineCount = 0u;
            lastPagingCounter = t1;
        }

        const char8 *p = buffer;
        uint32 index = 0u;
        uint32 start = 0u;
        uint32 sizeT;
        bool end = false;
        while ((index < size) && (!end)) {
            while ((lineCount < (numberOfRows - 1u)) && (index < size) && !end) {
                if (p[index] == '\n') {
                    lineCount++;
                }
                if (p[index] == '\0') {
                    end = true;
                }
                index++;
            }
            sizeT = index - start;
            ok = !Write(&p[start], sizeT, TTInfiniteWait);
            if (!ok) {
                end = true;
            }
            if (ok && (lineCount >= (numberOfRows - 1u))) {
                start = index;
                lastPagingCounter = t1;
                lineCount = 0u;
                const char8 *message = "[PAGING] ENTER TO CONTINUE\r";
                sizeT = static_cast<uint32>(StringHelper::Length(message));
                ok = Write(message, sizeT, TTInfiniteWait);
                if (ok) {
                    char8 readBuffer[32];
                    sizeT = N_CHARS_NEWLINE;
                    ok = Read(&readBuffer[0], sizeT, timeout);
                }
            }
        }
    }

    return ok;
}

#endif /*BASICCONSOLE_H_ */
