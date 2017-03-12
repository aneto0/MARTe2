/**
 * @file ConsoleLogger.h
 * @brief Header file for class ConsoleLogger
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

 * @details This header file contains the declaration of the class ConsoleLogger
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONSOLELOGGER_H_
#define CONSOLELOGGER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "BasicConsole.h"
#include "LoggerConsumerI.h"
#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief A LoggerConsumerI which outputs the log messages to a console instance.
 * @details The configuration syntax is (names are only given as an example):
 * +ConsoleLogger = {
 *     Class = ConsoleLogger
 *     Format = ItOoFm //Compulsory. As described in LoggerConsumerI::LoadPrintPreferences
 * }
 */
class ConsoleLogger: public Object, public LoggerConsumerI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. Opens the console.
     */
ConsoleLogger    ();

    /**
     * @brief Destructor. Closes the console.
     */
    virtual ~ConsoleLogger();

    /**
     * @brief Prints the logPage in the console output.
     * @param logPage the log message to be printed.
     */
    virtual void ConsumeLogMessage(LoggerPage *logPage);

    /**
     * @brief Calls Object::Initialise and reads the Format parameter (see class description) .
     * @param[in] data see Object::Initialise.
     * @return true if Object::Initialise returns true.
     */
    virtual bool Initialise(StructuredDataI &data);
private:

    /**
     *  The basic console where the logs are printed to.
     */
    BasicConsole console;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONSOLELOGGERCONSUMER_H_ */

