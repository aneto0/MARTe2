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
#include "ConfigurationLoaderHashI.h"
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
 * A second initialisation stage is available, so that further parameters can be read once the configuration input is parsed for the first time. The parameters are expected to be found at the root level of the ObjectRegistryDatabase, following structure below. Note that if a ConfigurationHashI object is set, it will be used to verify that the configuration matches with the one
 *  offered by the caller. The hash shall be computed using the GetSeed as the algorithm seed. Three messages may be also be set as described in the example below.
 * <pre>
 *  //The names of the nodes shall not be modified.
 *  +LoaderPostInit = {
 *    Class = ReferenceContainer
 *    +Parameters = {
 *        Class = ConfigurationDatabase
 *        ReloadLast = true //Optional. Reload old configuration in case of (re)configuration failure? Default = true. If true the FailedConfiguration message will not to be sent.
 *        KeepAlive = { "MyObj1" "MyObj2" "MyObj3" } //Optional. Array with name of the objects (at the root level) that shall not be purged and will always (i.e. even in case of error) be readded to the ObjectRegistryDatabase after a reconfiguration is triggered.
 *    }
 *    +Messages = {
 *      Class = ReferenceContainer
 *      +PreConfiguration = { //Optional message to send before the configuration is applied.
 *        Class = Message
 *        Destination = SomeObject
 *        Function = SomeFunction
 *        Mode = ExpectsReply
 *      }
 *      +PostConfiguration = { //Optional message to send if the configuration was successfully applied.
 *        Class = Message
 *        Destination = SomeObject
 *        Function = SomeFunction
 *        Mode = ExpectsReply
 *      }
 *      +FailedConfiguration = { //Optional message to send if the configuration failed. Note that given that the ObjectRegistryDatabase will have been purged, it is likely that this message will not reach its destination...see KeepAlive.
 *        Class = Message
 *        Destination = SomeObject
 *        Function = SomeFunction
 *        Mode = ExpectsReply
 *      }
 *      +ReloadedConfiguration = { //Optional message to send if the configuration was reloaded as a consequence of a failed reconfiguration attempt.
 *        Class = Message
 *        Destination = SomeObject
 *        Function = SomeFunction
 *        Mode = ExpectsReply
 *      }

 *    }
 *    +Hash = { //Optional. If set the configuration will be verified against the Hash function.
 *      Class = ConfigurationLoaderHashI
 *    }
 *  }
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
     * If successful the Loader instance will be added to the ObjectRegistryDatabase.
     */
    virtual ErrorManagement::ErrorType Configure(StructuredDataI &data, StreamI &configuration);

    /**
     * @brief Allows to reconfigure an application.
     * @details It will parse the input configuration and attempt to call ObjectRegistryDatabase::Initialise.
     * It will call ObjectRegistryDatabase::Purge before applying the new configuration.
     * After this function is successfully called, the GetLastValidConfiguration will return the updated configuration.
     * @param[in] configuration the stream with the new configuration to be loaded.
     * @param[out] errStream any errors that may be raised, including parser errors.
     * @return ErrorManagement::NoError if the configuration is successfully parsed and the ObjectRegistryDatabase::Initialise is successful.
     */
    virtual ErrorManagement::ErrorType Reconfigure(StreamI &configuration, StreamString &errStream);

    /**
     * @brief Allows to reconfigure an application.
     * @details It will attempt to call ObjectRegistryDatabase::Initialise.
     * It will call ObjectRegistryDatabase::Purge before applying the new configuration.
     * After this function is successfully called, the GetLastValidConfiguration will return the updated configuration.
     * @param[in] configuration the StructuredDataI with the new configuration to be loaded.
     * @param[out] errStream any errors that may be raised.
     * @return ErrorManagement::NoError if the configuration is successfully parsed and the ObjectRegistryDatabase::Initialise is successful.
     */
    virtual ErrorManagement::ErrorType Reconfigure(StructuredDataI &configuration, StreamString &errStream);

    /**
     * @brief Validates the configuration against a given hash. The Loader will compute the hash of the input \a configuration using the provided ConfigurationLoaderHashI - see class description. 
     * @param[in] configuration the new new configuration to be applied.
     * @param[out] errStream any errors that may be raised.
     * @param[in] hash independently computed hash.
     * @return ErrorManagement::NoError if the hash matches the hash computed by the ConfigurationLoaderHashI, the configuration is successfully parsed and the ObjectRegistryDatabase::Initialise is successful.
     */
    virtual ErrorManagement::ErrorType Reconfigure(StreamString &configuration, StreamString &errStream, uint32 hash);

    /**
     * @brief Returns a unique seed that can be used to hash a configuration request.
     * @details Only implemented if a ConfigurationLoaderHashI has been set.
     * @return see ConfigurationLoaderHashI::GetSeed.
     */
    virtual uint32 GetSeed();

    /**
     * @brief Reloads the last valid configuration.
     * @details This function will call ObjectRegistryDatabase::Purge.
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

    /**
     * @brief Load post-init parameters.
     * @return ErrorManagement::NoError if the LoaderPostInit parameters are successfully loaded.
     */
    virtual ErrorManagement::ErrorType PostInit();

    /**
     * @brief Called by Reconfigure upon a successful (re)loading of the ObjectRegistryDatabase.
     * @return ErrorManagement::NoError if the post reconfiguration is sucessfully executed.
     */
    virtual ErrorManagement::ErrorType PostReconfigure();

protected:
    /**
     * Reconfigure implementation allowing to select if to send the post message.
     */
    ErrorManagement::ErrorType ReconfigureImpl(StructuredDataI &configuration, StreamString &errStream, bool sendPostMsg);


    /**
     * @brief The loader parameters.
     */
    ConfigurationDatabase parsedConfiguration;

    /**
     * @brief The selected type of parser.
     */
    StreamString parserType;

    /**
     * @brief The post-init parameters
     */
    ReferenceT<ConfigurationDatabase> postInitParameters;

    /**
     * Helper function to send message.
     */
    ErrorManagement::ErrorType SendConfigurationMessage(ReferenceT<Message> msg);

    /**
     * The message to send after the configuration is applied.
     */
    ReferenceT<Message> postConfigMsg;

private:

    /**
     * @brief The destination of the message to be sent at Start.
     */
    StreamString messageDestination;

    /**
     * @brief The function of the message to be sent at Start.
     */
    StreamString messageFunction;

    /**
     * The message to send before the configuration is applied.
     */
    ReferenceT<Message> preConfigMsg;

    /**
     * The message to send if a configuration error is detected.
     */
    ReferenceT<Message> failedConfigMsg;

    /**
     * The message to send if the last configuration was reloaded, as a consequence of an error.
     */
    ReferenceT<Message> reloadedConfigurationMsg;

    /**
     * If true and the configuration fails, it will automatically reload the last good configuration.
     */
    bool reloadLast;

    /**
     * Hash component.
     */
    ReferenceT<ConfigurationLoaderHashI> loaderHash;

    /**
     * First loading?
     */
    bool firstLoading;

    /**
     * Objects that are always to be kept alive.
     */
    ReferenceContainer keepAliveObjs;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L6APP_LOADER_H_ */

