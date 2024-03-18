/**
 * @file RealTimeLoader.h
 * @brief Header file for class RealTimeLoader
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

 * @details This header file contains the declaration of the class RealTimeLoader
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L6APP_REALTIMELOADER_H_
#define L6APP_REALTIMELOADER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "Loader.h"
#include "Message.h"
#include "ParserI.h"
#include "RealTimeApplication.h"
#include "ReferenceT.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Starts a generic MARTe RealTimeApplication from a given configuration stream.
 * @details The Loader parses (see Initialise) a given configuration stream and configures the RealTimeApplication.
 * The Start method allows to trigger the Start of the application, either by specifying the first state or by sending a message to given destination (see Loader::Start).
 */
class DLL_API RealTimeLoader: public Loader {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor. NOOP.
     */
    RealTimeLoader();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~RealTimeLoader();

    /**
     * @brief Initialises the RealTimeApplication with the parameters specified in \a data and with the \a configuration stream (see Loader::Initialise).
     * @details If Loader::Initialise succeeds, a RealTimeApplication is search in the ObjectRegistryDatabase and, if found, the RealTimeApplication::ConfigureApplication is called.
     * @param[in] data see Loader::Initialise for other parameters:
     * - FirstState (optional): the first state to be called in the RealTimeApplication when Start is called.
     * @param[in] configuration see Loader::Initialise.
     * @return ErrorManagement::NoError if the Parser is specified, the \a configuration can be parsed, the ObjectRegistryDatabase can be Initialised with the parsed configuration and if the RealTimeApplication::ConfigureApplication is successful. An error is returned otherwise.
     */
    virtual ErrorManagement::ErrorType Configure(StructuredDataI &data, StreamI &configuration);

    /**
     * @brief Configures all the RealTimeApplication instances.
     * @return ErrorManagement::NoError if at least one RealTimeApplication is found and all RealTimeApplication instances configuration is sucessfully executed.
     */
    virtual ErrorManagement::ErrorType PostReconfigure();

    /**
     * @brief Start the RealTimeApplication.
     * @details If FirstState was set, calls RealTimeApplication::StartNextStateExecution with this state. Otherwise Loader::Start is called.
     * @return ErrorManagement::NoError if the FirstState was set and RealTimeApplication::StartNextStateExecution or if FirstState was not set and Loader::Start succeeds. An error is returned otherwise.
     */
    virtual ErrorManagement::ErrorType Start();

    /**
     * @brief Stops the RealTimeApplication.
     * @details Calls RealTimeApplication::StopCurrentStateExecution.
     * @return  ErrorManagement::NoError if the application was successfully stopped.
     */
    virtual ErrorManagement::ErrorType Stop();

private:
    /**
     * @brief The (optional) first state of the RealTimeApplication.
     */
    StreamString firstState;

    /**
     * @brief The RealTimeApplication.
     */
    ReferenceContainer rtApps;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L6APP_APPLICATION_H_ */

