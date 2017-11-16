/**
 * @file BasicConsole.h
 * @brief Header file for class BasicConsole
 * @date 04/07/2015
 * @author André Neto
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

#include "ErrorManagement.h"
#include "ErrorType.h"
#include "FlagsType.h"
#include "GeneralDefinitions.h"
#include "HandleI.h"
#include "StreamI.h"
#include "StringHelper.h"
#include "TimeoutType.h"

namespace MARTe {

namespace BasicConsoleMode {

/**
 * Default operation mode. In this mode users have to explicitly feed
 * a new line to write to the console output, CTRL-C will raise an exception
 * and kill the underlying program and paging will not be enabled.
 */
const FlagsType Default(0u);
/**
 * Operates on a display buffer different from the current one.
 */
const FlagsType CreateNewBuffer(1u);
/**
 * Does not wait for a newline when the user enters input.
 */
const FlagsType PerformCharacterInput(2u);
/**
 * CTRL-C does not raise an exception.
 */
const FlagsType DisableControlBreak(4u);
/**
 * Text is divided in pages that fit into the console dimension. Users are
 * requested to input a new line in order to change page.
 */
const FlagsType EnablePaging(8u);

}

}

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/**
 * Forward declaration of the operating system specific properties (defined in the operating system
 * specific unit file).
 */
struct BasicConsoleProperties;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Multi-platform basic console support.
 * @details The framework offers a portable basic console. This console enables framework
 * developers to offer a classic I/O interface to the framework users.
 * The specificities of the behaviour are delegated to the underlying operating system.
 * Not all of the features are necessarily implemented in all the operating systems.
 */
class DLL_API BasicConsole: public StreamI, public HandleI {

public:

    /**
     * @brief Initialises the console operating system specific properties.
     */
    BasicConsole();

    /**
     * @brief If it was not already closed, the destructor closes the console.
     * @details Even if the destructor closes the console it should be noticed
     * that in such case a failure is not handled and any exception arising
     * from such action is not caught.
     */
    virtual ~BasicConsole();

    /**
     * @brief Opens and display the console.
     * @details Opens and displays the console.
     * @param[in] mode the desired opening mode, which changes some behaviours of the console.
     * The only values accepted are those defined in MARTe::BasicConsoleMode.
     * @return true if the console is opened correctly or false if there is any operating
     * system related problem while performing the operation.
     * @pre Even if the behaviour will be operating system dependent, it is advisable to call
     * SetSize(numberOfColumns, numberOfRows) before opening the console.
     */
    virtual bool Open(const FlagsType &mode);

    /**
     * @brief Retrieves the mode that was used to open the console.
     * @return the openingMode used to open the console.
     * @pre the behaviour will be unspecified and operating system dependent if this method
     * is called on a console that was never open.
     */
    virtual FlagsType GetOpeningMode() const;

    /**
     * @brief Closes the console.
     * @details Closes the console and leaves it in a state where it can be opened again.
     * @return true if the console is opened correctly or false if there is any operating
     * system related problem while performing the operation.
     */
    bool Close();

    /**
     * @brief Update the console size.
     * @details Be aware of calling this function after the open the console.
     * @param[in] numberOfColumns number of columns to set in the console.
     * @param[in] numberOfRows number of rows to set in the console.
     * @return true if the new size can be set or false if there is any operating
     * system related problem while performing the operation.
     */
    virtual bool SetSceneSize(const uint32 &numberOfColumns,
                              const uint32 &numberOfRows);

    /**
     * @brief Returns the current console size.
     * @param[out] numberOfColumns currently number of columns being displayed.
     * @param[out] numberOfRows currently number of rows being displayed.
     * @return true if the size can be retrieved or false if there is any operating
     * system related problem while performing the operation.
     */
    virtual bool GetSceneSize(uint32 & numberOfColumns,
                              uint32 & numberOfRows) const;

    /**
     * @brief Sets the console window size (@see WindowSizeSupported).
     * @details The console window size must be smaller than the console size (@see GetSize and @see SetSize).\n
     * The actual implementation details depend on the operating system but it is expected that a window
     * will enable scroll bars if its size is smaller than the underlying console size.
     * @param[in] numberOfColumns desired x axis size.
     * @param[in] numberOfRows desired y axis size.
     * @return true if the size can be set, false in case of error. The error code will be: \n
     * ErrorManagement::OSError if there is any operating
     * system related problem while performing the operation;
     * ErrorManagement::UnsupportedFeature if the feature is not available
     * in the operating system specific implementation.
     */
    virtual bool SetWindowSize(const uint32 &numberOfColumns,
                               const uint32 &numberOfRows);

    /**
     * @brief Returns the window size.
     * @param[out] numberOfColumns is the x axis window size in return.
     * @param[out] numberOfRows is the y axis window size in return.
     * @return true if the size can be retrieved, false in case of error. The error code will be: \n
     * ErrorManagement::OSError if there is any operating
     * system related problem while performing the operation;
     * ErrorManagement::UnsupportedFeature if the feature is not available
     * in the operating system specific implementation.
     */
    virtual bool GetWindowSize(uint32 &numberOfColumns,
                               uint32 &numberOfRows) const;

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
     * @return true if the cursor can be set in the required position, false in case of error. The error code will be: \n
     * ErrorManagement::OSError if there is any operating
     * system related problem while performing the operation;
     * ErrorManagement::UnsupportedFeature if the feature is not available
     * in the operating system specific implementation.
     */
    virtual bool SetCursorPosition(const uint32 &column,
                                   const uint32 &row);

    /**
     * @brief Retrieves the cursor position.
     * @param[out] column is the current x coordinate of the cursor.
     * @param[out] row is the current y coordinate of the cursor.
     * @return true if the cursor coordinates be retrieved, false in case of error. The error code will be: \n
     * ErrorManagement::OSError if there is any operating
     * system related problem while performing the operation;
     * ErrorManagement::UnsupportedFeature if the feature is not available
     * in the operating system specific implementation.
     */
    virtual bool GetCursorPosition(uint32 & column,
                                   uint32 & row) const;

    /**
     * @brief Checks if it possible to interact with the console cursor position.
     * @return true if the operating system implementation supports reading and
     * modifying the cursor position.
     */
    virtual bool CursorPositionSupported() const;

    /**
     * @brief Switches to display this console buffer (@see ConsoleBufferSupported).
     * @details It is possible to write to an inactive screen buffer and then use
     * this function to display the buffer's contents.
     * @return true if the buffer is successfully displayed, false in case of error. The error code will be: \n
     * ErrorManagement::OSError if there is any operating
     * system related problem while performing the operation;
     * ErrorManagement::UnsupportedFeature if the feature is not available
     * in the operating system specific implementation.
     */
    virtual bool ShowBuffer();

    /**
     * @brief Checks if it possible to use an inactive console buffer for writing.
     * @return true if the operating system implementation supports double buffering.
     */
    virtual bool ConsoleBufferSupported() const;

    /**
     * @brief Sets the font foreground and background colours.
     * @param[in] foregroundColour the desired foreground colour.
     * @param[in] backgroundColour the desired background colour.
     * @return true if the new colours can be successfully set, false in case of error. The error code will be: \n
     * ErrorManagement::OSError if there is any operating
     * system related problem while performing the operation;
     * ErrorManagement::UnsupportedFeature if the feature is not available
     * in the operating system specific implementation.
     */
    virtual bool SetColour(const Colours &foregroundColour,
                           const Colours &backgroundColour);

    /**
     * @brief Checks if colour changing is supported by the operating system implementation.
     * @return true if the operating system implementation allows to change the console background
     * and foreground colours.
     */
    virtual bool ColourSupported() const;

    /**
     * @brief Clears the console output.
     * @return true if the console is cleared correctly or ErrorManagement::OSError if there is any operating
     * system related problem while performing the operation.
     */
    virtual bool Clear();

    /**
     * @brief Writes a single char on the console at a given position and with the given colour set.
     * @param[in] c is the character to be printed.
     * @param[in] foregroundColour is the desired console foreground colour.
     * @param[in] backgroundColour is the desired console background colour.
     * @param[in] column is the desired x-axis position of the character.
     * @param[in] row is the desired y-axis position of the character.
     * @return true if the char is successfully plotted with the correct attributes, false in case of error. The error code will be: \n
     * ErrorManagement::OSError if there is any operating
     * system related problem while performing the operation;
     * ErrorManagement::UnsupportedFeature if the feature is not available
     * in the operating system specific implementation.
     */
    virtual bool PlotChar(const char8 &c,
                          const Colours &foregroundColour,
                          const Colours &backgroundColour,
                          const uint32 &column,
                          const uint32 &row);

    /**
     * @brief Update the console title (@see TitleBarSupported).
     * @param[in] title is the desired title.
     * @return true if the title can be successfully set, false in case of error. The error code will be: \n
     * ErrorManagement::OSError if there is any operating
     * system related problem while performing the operation;
     * ErrorManagement::UnsupportedFeature if the feature is not available
     * in the operating system specific implementation.
     */
    virtual bool SetTitleBar(CCString title);

    /**
     * @brief Returns the console title (@see TitleBarSupported).
     * @param[out] title is a destination buffer.
     * @return true if the title can be successfully retrieved. In case of errors it returns false and the error code will be: \n
     * ErrorManagement::OSError if there is any operating
     * system related problem while performing the operation;
     * ErrorManagement::UnsupportedFeature if the feature is not available
     * in the operating system specific implementation;
     */
    template <uint32 sizeOfBuffer>
    inline bool GetTitleBar(StaticCString<sizeOfBuffer>title) const{
        uint32 size = sizeOfBuffer-1;
        return GetTitleBar(title.GetList(),size);
    }


    /**
     * @brief Checks if changing or reading the console title is supported
     * @return true if the operating system implementation enables the reading and updating
     * of the console title.
     */
    virtual bool TitleBarSupported() const;

    /**
     * @brief Checks if the console supports timeout in the Read/Write operation.
     * @return true if the operating system implementation enables to timeout in the Read/Write operations.
     */
    virtual bool TimeoutSupported() const;

    /**
     * @brief The console supports writing.
     * @return true.
     */
    virtual bool CanWrite() const;

    /**
     * @brief The console supports reading.
     * @return true.
     */
    virtual bool CanRead() const;

    /**
     * @brief The console does not support seeking.
     * @return false.
     */
    virtual bool CanSeek() const;

    /**
     * @see StreamI::Read
     */
    virtual bool Read(char8 * const output,  uint32 & size);

    /**
     * @see StreamI::Write
     */
    virtual bool Write(const char8 * const input,  uint32 & size);

    /**
     * @brief Writes to the console.
     * @details A specified number of bytes of the buffer are written to the console output.
     * @param[in] buffer memory data to be written on the console.
     * @param[in,out] size maximum size in bytes to write on the console. If a minor number of bytes is written,
     * size will contain the number of bytes actually written.
     * @param [in] timeout maximum time to wait for writing to the console.
     * @return - true if a number of bytes greater than 0 is successfully written.
     * Returns false in case of errors. The error code will be:
     * - ErrorManagement::Timeout if the time to complete the operation is greater than the specified timeout (@see TimeoutSupported);
     * - ErrorManagement::Warning if zero bytes are written and no ErrorManagement::OSError is flagged;
     * - ErrorManagement::OSError if there is any operating system related problem while performing the operation.
     * @pre Size has not to be larger than the buffer size.
     */
    virtual bool Write(const char8 * const input,
                       uint32 & size,
                       const TimeoutType &timeout);

    /**
     * @brief Reads from the console.
     * @details Reads from the console input into the buffer.
     * If PerformCharacterInput mode is enabled, it will return without waiting for LF.
     * @param[in] buffer memory space where the read bytes are written into.
     * @param[in,out] size number of bytes to read.
     * @param[in] timeout maximum time to wait for the operation to be successfully completed.
     * @return - true if at least one byte is read, false in case of errors. The error code will be:
     * - ErrorManagement::Timeout if the time to complete the operation is greater than the specified timeout (@see TimeoutSupported);
     * - ErrorManagement::Warning if zero bytes are read and no ErrorManagement::OSError is flagged;
     * - ErrorManagement::OSError if there is any operating system related problem while performing the operation.
     */
    virtual bool Read(char8 * const output,
                      uint32 & size,
                      const TimeoutType &timeout);

    /**
     * @brief Unsupported feature.
     * @return 0xFFFFFFFFFFFFFFFF.
     */
    virtual uint64 Size() const;

    /**
     * @brief Unsupported feature.
     * @return false.
     */
    virtual bool Seek(uint64 pos);

    /**
     * @brief Unsupported feature.
     * @return false.
     */
    virtual bool RelativeSeek(const int64 deltaPos);

    /**
     * @brief Unsupported feature.
     * @return 0xFFFFFFFFFFFFFFFF.
     */
    virtual uint64 Position() const ;

    /**
     * @brief Unsupported feature.
     * @return false.
     */
    virtual bool SetSize(uint64 size);

    /**
     * @brief Queries the read console handle.
     */
    virtual Handle GetReadHandle() const;

    /**
     * @brief Queries the write console handle.
     */
    virtual Handle GetWriteHandle() const;

private:

    /**
     * @brief Returns the console title (@see TitleBarSupported).
     * @param[out] title is a destination buffer.
     * @param[out] size is the size of the title.
     * @return true if the title can be successfully retrieved. In case of errors it returns false and the error code will be: \n
     * ErrorManagement::OSError if there is any operating
     * system related problem while performing the operation;
     * ErrorManagement::UnsupportedFeature if the feature is not available
     * in the operating system specific implementation;
     */
    virtual bool GetTitleBar(CString const title, const uint32 &size) const;

    /**
     * @brief Portable paged write implementation.
     * @details If the mode is paging, it divides the text to write in chunks (pages) that fit the console size.
     * The user is asked to press a key in order to change to the next the page.
     * @param[in] buffer is the data to write on the console.
     * @param[in] size is maximum size in byte to write on the console. If a minor number of bytes is written,
     * size become the number of bytes written.
     * @param[in] msecTimeout is the timeout.
     * @return true if a number of bytes greater than 0 is successfully written. Otherwise, it will return false.
     * if the time to complete the operation is greater than the specified timeout (@see TimeoutSupported) or ErrorManagement::OSError
     * if there is any operating system related problem while performing the operation.
     */
    inline bool PagedWrite(const char8* const buffer,
                           const uint32 &size,
                           const TimeoutType &timeout);

    /**
     * @copybrief BasicConsole::Write
     * @details The BasicConsole::Write will call based on the opening mode either the Write or the
     * OSWrite function. This is where the actual operating system call is implemented.
     * @param[in] buffer memory data to be written on the console.
     * @param[in,out] size maximum size in bytes to write on the console. If a minor number of bytes is written,
     * size will contain the number of bytes actually written.
     * @param [in] timeout maximum time to wait for writing to the console.
     * @return - true if a number of bytes greater than 0 is successfully written;
     * - ErrorManagement::Timeout if the time to complete the operation is greater than the specified timeout (@see TimeoutSupported);
     * - ErrorManagement::Warning if zero bytes are written and no ErrorManagement::OSError is flagged;
     * - ErrorManagement::OSError if there is any operating system related problem while performing the operation.
     */
    bool OSWrite(const char8 * const buffer,
                 uint32 & size,
                 const TimeoutType &timeout);

    /**
     * Operating system specific properties to be used by the operating system specific implementation
     */
    BasicConsoleHandle handle;

    /**
     * How long since the last paging.
     */
    int64 lastPagingCounter;

    /**
     * How many lines since the last paging.
     */
    uint32 lineCount;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

bool BasicConsole::PagedWrite(const char8 * const buffer,
                              const uint32 &size,
                              const TimeoutType &timeout) {

    uint32 numberOfColumnsTmp = 0u;
    uint32 numberOfRowsTmp = 0u;
    bool err = GetSceneSize(numberOfColumnsTmp, numberOfRowsTmp);
    if (err) {

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
            err = OSWrite(&p[start], sizeT, timeout);
            if (!err) {
                end = true;
            }
            else if (lineCount >= (numberOfRows - 1u)) {
                start = index;
                lastPagingCounter = t1;
                lineCount = 0u;
                CCString message("[PAGING] ENTER TO CONTINUE\r");
                sizeT = message.GetSize();
                err = OSWrite(message.GetList(), sizeT, timeout);
                if (err) {
                    char8 readBuffer[32];     // TODO why 32
                    sizeT = N_CHARS_NEWLINE;  // TODO why this size
                    err = Read(&readBuffer[0], sizeT, timeout);
                }
            }
        }
    }

    return err;
}

}

#endif /*BASICCONSOLE_H_ */
