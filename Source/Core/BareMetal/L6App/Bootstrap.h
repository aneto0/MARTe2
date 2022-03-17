/**
 * @file Bootstrap.h
 * @brief Header file for class Bootstrap
 * @date 04/04/2018
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

 * @details This header file contains the declaration of the class Bootstrap
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L6APP_BOOTSTRAP_H_
#define L6APP_BOOTSTRAP_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StreamI.h"
#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Bootstraps the main arguments and provides the (environment specific) execution context (see Run).
 * @details The implementation of the class is operating system specific.
 */
class DLL_API Bootstrap {
public:
    /**
     * @brief Constructor. NOOP.
     */
    Bootstrap();

    /**
     * @brief Destructor. NOOP.
     */
    ~Bootstrap();

    /**
     * @brief Translate the main (argc and argv) parameters into a StructuredDataI.
     * @param[in] argc the number of arguments provided in the main.
     * @param[in] argv the list of arguments provided in the main.
     * @param[out] loaderParameters the list of parsed parameters:
     * - Loader: the type of loader class to be used;
     * - Filename: the name of the file to be load;
     * - DefaultCPUs: sets the threads defaults CPUs (see ProcessorType::SetDefaultCPUs);\n
     * - SchedulerGranularity: sets the scheduler granularity in micro-seconds (i.e. any requests to sleep no more than this value, will busy sleep).
     * - Parser: the type of parser to be parse the \a configuration as one of:cdb, xml and json;\n
     * - MessageDestination (optional): the name of the Object that will receive the message upon program Start (see Loader::Initialise);\n
     * - MessageFunction (optional): the name of the Function to be called in the MessageDestination (see Loader::Initialise);\n
     * - FirstState (optional): the name of the FirstState in a RealTimeApplication (see RealTimeLoader::Initialise).
     * @return ErrorManagement::NoError if all the compulsory parameters can be read from the \a argv and written into the loaderParameters. A specific ErrorType otherwise.
     */
    ErrorManagement::ErrorType ReadParameters(int32 argc, char8 **argv, StructuredDataI &loaderParameters);

    /**
     * @brief Gets the configuration stream to be used for the application start.
     * @param[in] loaderParameters the parameters that were read with ReadParameters.
     * @param[out] configurationStream the stream to be read.
     * @return ErrorManagement::NoError if the stream is ready to be read. A specific ErrorType otherwise.
     */
    ErrorManagement::ErrorType GetConfigurationStream(StructuredDataI &loaderParameters, StreamI *&configurationStream);

    /**
     * @brief Setup the execution specific environment and wait for (an environment specific) signal to terminate the program.
     * @return only when the program is to be terminated. ErrorManagement::NoError if the execution was successfully cleaned.
     */
    ErrorManagement::ErrorType Run();

    /**
     * @brief How to printf in the specific environment.
     * @param[in] msg the message to printf.
     */
    void Printf(const char8 * const msg);

    /**
     * @brief Main function callback. It allows, for example, to spawn the Main in the context of a thread.
     * 
     * @param[in] argc the number of arguments provided in the main.
     * @param[in] argv the list of arguments provided in the main.
     */
    void Main(int (*loader)(int32 argc, char8** argv), int32 argc, char8** argv);
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L6APP_BOOTSTRAP_H_ */
