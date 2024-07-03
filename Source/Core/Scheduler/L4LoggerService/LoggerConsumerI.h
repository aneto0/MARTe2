/**
 * @file LoggerConsumerI.h
 * @brief Header file for class LoggerConsumerI
 * @date 10/03/2017
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class LoggerConsumerI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef LOGGERCONSUMERI_H_
#define LOGGERCONSUMERI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Logger.h"
#include "BufferedStreamI.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Classes that inherit from this interface (and are inserted into a LoggerService instance)
 *  will have the function ConsumeLogMessage called every time a log message is received.
 */
class DLL_API LoggerConsumerI {
public:
    /**
     * @brief Constructor. NOOP.
     */
    LoggerConsumerI();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~LoggerConsumerI();

    /**
     * @brief This function is called every time a log message is received.
     * @param[in] logPage the logging message to be consumed.
     */
    virtual void ConsumeLogMessage(LoggerPage *logPage) = 0;

protected:
    /**
     * @brief Helper function which prints the log message into a stream.
     * @param[in] logPage the logging information.
     * @param[out] err the stream where to write the information into.
     * @pre
     *   LoadPrintPreferences()
     */
    void PrintToStream(LoggerPage *logPage, BufferedStreamI &err) const;

    /**
     * @brief Reads the Printing preference from a StructuredDataI.
     * @param[in] data shall contain a string with the name Format and with a value that encodes, with no separator and in any order, the logging error descriptions that should be output:
     * - E: error code
     * - T: time in HRT at which the error occurred
     * - t: time in the format HH:MM:SS at which the PrintToStream above was called plus the HRT at which the error occurred
     * - O: the object name
     * - o: the object pointer
     * - f: the function name
     * - F: the file name (includes the line number)
     * - R: the thread identifier
     * - m: the error message
     * - C: the class name.
     * - P: platform specific string.
     *
     * An example could be Format="ItOoFm".
     *
     * data may contain a parameter named "PrintKeys" with value 0 or 1. If 1 the a key identifying the logging error descriptions will prefix each description with the format |KEY=, where KEY is:
     * - |E= : error code
     * - |TM= : time in HRT at which the error occurred or time in the format HH:MM:SS at which the PrintToStream above was called plus the HRT at which the error occurred
     * - |o= : the object name
     * - |O= : the object pointer
     * - |T= : the thread identifier
     * - |f= : the function name
     * - |F= : the file name
     * - |D= : the message
     * - |C= : the class name
     * - |P= : platform specific string
     *
     * @return true if the Format exists and can be successfully parsed.
     */
    bool LoadPrintPreferences(StructuredDataI &data);

    /**
     * @brief Checks if if the error description keys are to be printed.
     * @return true if the error description keys are to be printed.
     */
    bool IsPrintKeys() const;

    /**
     * @brief Describes what information about the logging message is to be made available.
     */
    /*lint ++flb*/
    union FormatPreferences {
        /**
         * Print the log error information?
         */
        BitBoolean<uint16, 0u> info;

        /**
         * Print the log time with seconds resolution?
         */
        BitBoolean<uint16, 1u> timeShort;

        /**
         * Print the log time with full resolution?
         */
        BitBoolean<uint16, 2u> timeFull;

        /**
         * Print the object name?
         */
        BitBoolean<uint16, 3u> objectName;

        /**
         * Print the object pointer?
         */
        BitBoolean<uint16, 4u> objectPointer;

        /**
         * Print the function name?
         */
        BitBoolean<uint16, 5u> functionName;

        /**
         * Print the filename?
         */
        BitBoolean<uint16, 6u> fileName;

        /**
         * Print the log message?
         */
        BitBoolean<uint16, 7u> message;

        /**
         * Print the thread id?
         */
        BitBoolean<uint16, 8u> threadId;

        /**
         * Print the class name?
         */
        BitBoolean<uint16, 9u> className;

        /**
         * Print the platform specific name?
        */
        BitBoolean<uint16, 10u> platformSpecificString;

        /**
         * Unmapped area
         */
        BitRange<uint16, 5u, 11u> unMapped;

        /**
         * Output as uint16
         */
        uint16 asUint16;
    };
    /*lint --flb*/

    /**
     * @brief Returns a copy of the format preferences.
     * @return a copy of the format preferences.
     */
    FormatPreferences GetFormatPreferences() const;

private:

    /**
     * True if the keys are to be printed.
     */
    bool printKeys;

    /**
     * Stores the format preferences.
     */

    FormatPreferences formatPrefs;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* LOGGERCONSUMERI_H_ */

