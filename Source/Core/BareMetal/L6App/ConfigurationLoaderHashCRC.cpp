/**
 * @file ConfigurationLoaderHashCRC.cpp
 * @brief Source file for class ConfigurationLoaderHashCRC
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
 * the class ConfigurationLoaderHashCRC (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ConfigurationLoaderHashCRC.h"
#include "ObjectRegistryDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
ConfigurationLoaderHashCRC::ConfigurationLoaderHashCRC() : Object() {
    lastSeed = 0u;
}

ConfigurationLoaderHashCRC::~ConfigurationLoaderHashCRC() {
}

uint32 ConfigurationLoaderHashCRC::GetSeed() {
    lastSeed = static_cast<uint32>(HighResolutionTimer::Counter());
    return lastSeed; 
}

bool ConfigurationLoaderHashCRC::Initialise(StructuredDataI & data) {
    bool ok = Object::Initialise(data);
    uint32 key = 0u;
    if (ok) {
        ok = data.Read("Key", key);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "The CRC key shall be set"); 
        }
    }
    if (ok) {
        crc.ComputeTable(key);
    }
    return ok;
}

uint32 ConfigurationLoaderHashCRC::ComputeHash(const char8 * const input, const uint32 inputSize) {
    //The seed should be the same
    uint32 hashMemSize = static_cast<uint32>(sizeof(uint32) + inputSize);
    uint8 *hashMem = new uint8[hashMemSize];
    //lint -e{927} -e{826} both sides have sizeof(uint32) size
    *(reinterpret_cast<uint32 *>(hashMem)) = lastSeed;
    (void) MemoryOperationsHelper::Copy(reinterpret_cast<void *>(&hashMem[sizeof(uint32)]), reinterpret_cast<const void *>(input), inputSize);
    uint32 computedHash = crc.Compute(hashMem, static_cast<int32>(hashMemSize), 0u, false);
    delete[] hashMem;

    return computedHash;

}

CLASS_REGISTER(ConfigurationLoaderHashCRC, "1.0")

}

