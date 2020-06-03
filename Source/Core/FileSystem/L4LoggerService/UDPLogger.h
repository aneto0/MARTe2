/**
 * @file UDPLogger.h
 * @brief Header file for class UDPLogger
 * @date 14/03/2017
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

 * @details This header file contains the declaration of the class UDPLogger
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L4LOGGERSERVICE_UDPLOGGER_H_
#define L4LOGGERSERVICE_UDPLOGGER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "LoggerConsumerI.h"
#include "Object.h"
#include "UDPSocket.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief A LoggerConsumerI which outputs the log messages to a UDP socket.
 * @details The configuration syntax is (names are only given as an example):
 * <pre>
 * +UDPLogger = {
 *     Class = UDPLogger
 *     Address = 127.0.0.1 //Compulsory. The address of the destination where the logs are to be sent to.
 *     Port = 44444 //Compulsory. The port of the destination where the logs are to be sent to.
 *     Format = ItOoFm //Compulsory. As described in LoggerConsumerI::LoadPrintPreferences
 *     PrintKeys = 1 //Optional. As described in LoggerConsumerI::LoadPrintPreferences
 * }
 * </pre>
 */
class UDPLogger: public Object, public LoggerConsumerI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor.
     */
    UDPLogger();

    /**
     * @brief Destructor. Closes the socket.
     */
    virtual ~UDPLogger();

    /**
     * @brief Prints the logPage in the socket output.
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
     *  The UDP socket where the logs are printed to.
     */
    UDPSocket udpSocket;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L4LOGGERSERVICE_UDPLOGGER_H_ */
