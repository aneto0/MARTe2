/**
 * @file Loader.h
 * @brief Header file for class Loader
 * @date 4/4/2018
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

 * @details This header file contains the declaration of the class Loader
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L6APP_LOADER_H_
#define L6APP_LOADER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "Message.h"
#include "ParserI.h"
#include "ProcessorType.h"
#include "ReferenceT.h"


/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Starts a generic MARTe application from a given configuration stream.
 * @details The Loader parses (see Initialise) a given configuration stream and sends (see Start) a Message to a provided destination.
 */
class DLL_API Loader: public Object {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    Loader();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~Loader();

    /**
     * @brief Initialises the Loader with the parameters specified in \a data and with the \a configuration stream.
     * @param[in] data the loader parameters: \n
     * - DefaultCPUs (optional): sets the threads defaults CPUs (see ProcessorType::SetDefaultCPUs);\n
     * - SchedulerGranularity (optional): sets the scheduler granularity in micro-seconds (i.e. any requests to sleep no more than this value, will busy sleep).
     * - Parser: the type of parser to be parse the \a configuration as one of:cdb, xml and json;\n
     * - MessageDestination (optional): the name of the Object that will receive the message when Start is called;\n
     * - MessageFunction (optional, but compulsory if MessageDestination is set): the name of the Function to be called in the MessageDestination.
     * @param[in] configuration the MARTe configuration stream to be loaded (and parsed using the Parser defined above).
     * @return ErrorManagement::NoError if the Parser is specified, the \a configuration can be parsed and if the ObjectRegistryDatabase can be Initialised with the parsed configuration. An error is returned otherwise.
     */
    virtual ErrorManagement::ErrorType Configure(StructuredDataI &data, StreamI &configuration);

    /**
     * @brief Allows to reconfigure an application.
     * @details It will parse the input configuration and attempt to call ObjectRegistryDatabase::Initialise.
     * It is expected that the caller will have called ObjectRegistryDatabase::Purge before.
     * After this function is successfully called, the GetLastValidConfiguration will return the updated configuration.
     * @param[in] configuration the stream with the new configuration to be loaded.
     * @param[out] parserError any parser errors that may be raised.
     * @return ErrorManagement::NoError if the configuration is successfully parsed and the ObjectRegistryDatabase::Initialise is successful.
     * @pre
     *   Optional - ObjectRegistryDatabase::Purge
     */
    virtual ErrorManagement::ErrorType Reconfigure(StreamI &configuration, StreamString &parserError);

    /**
     * @brief Reloads the last valid configuration.
     * @details It is expected that the caller will have called ObjectRegistryDatabase::Purge before.
     * @return ErrorManagement::NoError if the last valid configuration is successfully loaded.
     */
    ErrorManagement::ErrorType ReloadLastValidConfiguration();

    /**
     * @brief Returns the last valid configuration.
     * @param[out] output the destination ConfigurationDatabase.
     * @return ErrorManagement::NoError if the current valid configuration is successfully copied to the output.
     */
    ErrorManagement::ErrorType GetLastValidConfiguration(ConfigurationDatabase &output);

    /**
     * @brief If the MessageDestination was specified in Initialise, sends the Message to the specified destination.
     * @return ErrorManagement::NoError if the MessageDestination was specified and if the Message was successfully sent. An error is returned otherwise.
     */
    virtual ErrorManagement::ErrorType Start();

    /**
     * @brief NOOP.
     * @return ErrorManagement::NoError.
     */
    virtual ErrorManagement::ErrorType Stop();

protected:
    /**
     * @brief The loader parameters.
     */
    ConfigurationDatabase parsedConfiguration;

    /**
     * @brief The selected type of parser.
     */
    StreamString parserType;

private:
    /**
     * @brief The destination of the message to be sent at Start.
     */
    StreamString messageDestination;

    /**
     * @brief The function of the message to be sent at Start.
     */
    StreamString messageFunction;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L6APP_LOADER_H_ */

