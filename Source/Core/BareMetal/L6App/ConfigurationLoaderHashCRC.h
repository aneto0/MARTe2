/**
 * @file ConfigurationLoaderHashCRC.h
 * @brief Header file for class ConfigurationLoaderHashCRC
 * @author Andre' Neto
 * @date 04/02/2022
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

#ifndef CONFIGURATION_LOADER_HASH_CRC_H_
#define CONFIGURATION_LOADER_HASH_CRC_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ConfigurationLoaderHashI.h"
#include "CRC.h"
#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief CRC based implementation of a configuration hash mechanism. 
 * <pre>
 * +ConfigHashCRC = {
 *   Class = ConfigurationLoaderHashCRC
 *   Key = 0x11111 //Key to compute the CRC table.
 * }
 * </pre>
 *
 */
class ConfigurationLoaderHashCRC : public Object, public ConfigurationLoaderHashI {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    ConfigurationLoaderHashCRC();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ConfigurationLoaderHashCRC();

    /**
     * @brief Reads the CRC key.
     * @return true if the CRC key is set.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief see ConfigurationLoaderHashI::GetSeed.
     * @return a seed that is based on the HRT::Counter.
     */
    virtual uint32 GetSeed();

    /**
     * @brief Compute hash against the input string using the MARTe CRC implementation.
     * @return the computed hash.
     */
    virtual uint32 ComputeHash(const char8 *input, const uint32 inputSize);

private:

    /**
     * CRC engine.
     */
    CRC<uint32> crc;

    /**
     * Last seed.
     */
    uint32 lastSeed;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONFIGURATION_LOADER_HASH_CRC_H_ */

