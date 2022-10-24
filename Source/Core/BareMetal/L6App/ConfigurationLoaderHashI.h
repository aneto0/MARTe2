/**
 * @file ConfigurationLoaderHashI.h
 * @brief Header file for class ConfigurationLoaderHashI
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

#ifndef CONFIGURATION_LOADER_HASH_I_H_
#define CONFIGURATION_LOADER_HASH_I_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Abstract class to facilitate the implementation of configuration hash mechanism.
 * See the Loader class description for a usage example.
 */
class ConfigurationLoaderHashI {
public:
    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ConfigurationLoaderHashI() {}

    /**
     * @brief Returns a unique seed that can be used to hash a configuration request.
     * @return a unique seed seed that can be used to hash a configuration request.
     */
    virtual uint32 GetSeed() = 0;

    /**
     * @brief Compute hash against input string.
     * @return the computed hash.
     */
    virtual uint32 ComputeHash(const char8 *input, uint32 inputSize) = 0;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CONFIGURATION_LOADER_HASH_I_H_ */

