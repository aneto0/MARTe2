/**
 * @file BasicConsole.h
 * @brief Header file for class BasicConsole
 * @date 22/06/2015
 * @author Giuseppe Ferr�
 *
 * @copyright Copyright 2015 F4E | European Joint32 Undertaking for ITER and
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
#include "IBasicConsole.h"
#include INCLUDE_FILE_OPERATING_SYSTEM(OPERATING_SYSTEM,BasicConsoleOS.h)

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Basic console support.
 *
 * @details This class provides a basic portable console, where users can interact
 * with the system through read and write operations allowing. This is particularly
 * useful for interacting with more embedded systems.\n
 * Note that this class is a proxy to the operating system specific implementation
 * where some of the features might not be available (query methods are provided).
 */
class BasicConsole: public BasicConsoleOS {

public:
    /**
     * @brief Default constructor.
     */
    BasicConsole();

    /**
     * @brief Default destructor.
     * @details Closes the console after restored its previous configuration.
     */
    virtual ~BasicConsole();

    /**
     * @see IBasicConsole::Write
     * @brief Write to the console output.
     * @param[in] buffer text to write to the console.
     * @param[in] size size of the text in characters.
     * @param[in] timeout maximum time allowed for the operation to succeed.
     * @details If paging is enabled this method will page the buffer and display each page individually (using BasicConsoleOS::Write).\n
     * The user will be asked to enter a new line so that the next page can be displayed.
     * @return true if all the BasicConsoleOS::Write calls are successful.
     * @pre the console was successfully opened and GetSize > 0 for both the rows and columns.
     */
    /*lint -e{1735} the default parameter is only specified at this level. BasicConsole is not expected to be sub-classed.*/
    virtual bool Write(const char8* const buffer, uint32& size, const TimeoutType &timeout = TTInfiniteWait);

    /**
     * @copydetails IBasicConsole::Read
     */
    /*lint -e{1735} the default parameter is only specified at this level. BasicConsole is not expected to be sub-classed.*/
    virtual bool Read(char8 * const buffer, uint32 & size, const TimeoutType &timeout = TTInfiniteWait);
private:
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
    bool PagedWrite(const char8* const buffer, const uint32 &size, const TimeoutType &timeout);

    /** How long since last paging. */
    int64 lastPagingCounter;

    /** How many lines since last paging. */
    uint32 lineCount;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
#endif /* BASICCONSOLE_H_ */
