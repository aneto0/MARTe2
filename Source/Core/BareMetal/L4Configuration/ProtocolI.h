/**
 * @file ProtocolI.h
 * @brief Header file for class ProtocolI
 * @date 14/09/2018
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

 * @details This header file contains the declaration of the class ProtocolI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PROTOCOLI_H_
#define PROTOCOLI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Abstracts the main concepts of a data exchange interface protocol.
 */
class ProtocolI: public ConfigurationDatabase {
public:
    /**
     * @brief Constructor
     */
    ProtocolI();

    /**
     * @brief Destructor
     */
    virtual ~ProtocolI();

    /**
     * @brief Reads the header.
     * @return true if the operation succeeds, false otherwise.
     */
    virtual bool ReadHeader()=0;

    /**
     * @brief Writes the header.
     * @param[in] isMessageCompleted specifies if the message is completed.
     * @param[in] command the command.
     * @param[in] payload the payload.
     * @param[in] id the message id.
     * @return true if the operation succeeds, false otherwise.
     */
    virtual bool WriteHeader(const bool isMessageCompleted, const int32 command, BufferedStreamI *payload, const char8 * const id)=0;

    /**
     * @brief Sets the unmatched id.
     * @param[in] unMatchedIdIn the unmatched id to set.
     */
    virtual void SetUnmatchedId(const char8 *unMatchedIdIn)=0;

    /**
     * @brief Retrieves the unmatched id.
     * @return the unmatched id.
     */
    virtual void GetUnmatchedId(StreamString& unmatchedIdOut)=0;

    /**
     * @brief Retrieves the path.
     * @param[out] pathOut the path in output.
     */
    virtual void GetPath(StreamString& pathOut)=0;

    /**
     * @brief Retrieves the id.
     * @param[out] pathOut the id in output.
     */
    virtual void GetId(StreamString& idOut)=0;

    /**
     * @brief Retrieves the value of the input command \a commandName.
     * @param[in] commandName the name of the command.
     * @param[out] commandValue the value of the required command.
     */
    virtual bool GetInputCommand(const char8 * const commandName, const AnyType &commandValue)=0;

    /**
     * @brief Sets the value of the output command \a commandName.
     * @param[in] commandName the name of the command.
     * @param[in] commandValue the value of the command to set.
     */
    virtual bool SetOutputCommand(const char8 * const commandName, const AnyType &commandValue)=0;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PROTOCOLI_H_ */

