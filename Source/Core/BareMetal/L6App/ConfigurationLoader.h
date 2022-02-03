/**
 * @file ConfigurationLoader.h
 * @brief Header file for class ConfigurationLoader
 * @author Andre' Neto
 * @date 03/02/2022
 *
 * @copyright Copyright 2019 EPFL SPC Lausanne
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing,
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class ConfigurationLoaderGAM
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CONFIGURATION_LOADER_H_
#define CONFIGURATION_LOADER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationDatabase.h"
#include "Loader.h"
#include "ReferenceContainer.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Abstract class to facilitate the implementation of configuration interfaces (e.g. TCP, serial, ...).
 *
 * The class will find the MARTe::Loader class and use it to apply new configurations.
 *
 * If a ConfigurationHashI object is set, it will be used to verify that the configuration matches with the one
 *  offered by the caller. In order to do so, the configuration stream shall have a field "Hash = MYCOMPUTEDHASH" at the root level. The hash shall be computed using the GetSeed as the algorithm seed.
 *
 * Three messages may be set as described in the example below.
 *
 * <pre>
 *  +MyConfigurationLoader = {
 *    Class = ConfigurationLoader
 *    ReloadOld = true //Reload old configuration in case of (re)configuration failure? Default = true
 *    +Messages = {
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
 *      +FailedConfiguration = { //Optional message to send if the configuration failed.
 *        Class = Message
 *        Destination = SomeObject
 *        Function = SomeFunction
 *        Mode = ExpectsReply
 *      }
 *    }
 *    +Hash = { //Optional. If set the configuration will be verified against the Hash function.
 *      Class = MyHasher
 *    }
 *  }
 * </pre>
 */
class ConfigurationLoader: public ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     */
    ConfigurationLoader();

    /**
     * @brief Destructor
     */
    virtual ~ConfigurationLoader();

    /**
     * @brief TODO.
     * @details TODO.
     * @return true TODO if the ReferenceContainer is successfully initialised.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Returns a unique seed that can be used to hash a configuration request.
     * @return a unique seed based on the HighResolutionTimer::Counter
     */
    virtual uint64 GetSeed();

    /**
     * @brief Applies a new configuration as delivered by the StreamI interface.
     * @details TODO The configuration is parsed against the configured Parser. If the parsing
     * is successful all the ObjectRegistryDatabase objects (with expection of this handler are purged).
     * Finally, the ObjectRegistryDatabase is Initialised with the new configuration. 
     * If the reconfiguration fails and ReloadOld=true, the previous working configuration is loaded. 
     * @param[in] source the StreamI with the configuration request.
     * @return true if the reconfiguration was successful.
     */
    ErrorManagement::ErrorType ApplyConfiguration(StreamI &source);

    /**
     * @brief TODO
     */
    ErrorManagement::ErrorType ApplyConfigurationWithHash(StreamString &source, StreamString &hash);

private:
    /**
     * The application bootstrap loader.
     */
    ReferenceT<Loader> loader;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONFIGURATION_LOADER_H_ */

