/**
 * @file ConfigurationLoader.cpp
 * @brief Source file for class ConfigurationLoader
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

 * @details This source file contains the definition of all the methods for
 * the class ConfigurationLoader (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationLoader.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
ConfigurationLoader::ConfigurationLoader() :
        ReferenceContainer() {
}

ConfigurationLoader::~ConfigurationLoader() {
}

uint64 ConfigurationLoader::GetSeed() {
    return HighResolutionTimer::Counter();
}

bool ConfigurationLoader::Initialise(StructuredDataI & data) {
    bool ok = ReferenceContainer::Initialise(data);
    return ok;
}

ErrorManagement::ErrorType ConfigurationLoader::ApplyConfiguration(StreamI &source) {
    return ErrorManagement::FatalError;
}

ErrorManagement::ErrorType ConfigurationLoader::ApplyConfigurationWithHash(StreamString &source, StreamString &hash) {
    return ErrorManagement::FatalError;
}

CLASS_REGISTER(ConfigurationLoader, "1.0")

}

