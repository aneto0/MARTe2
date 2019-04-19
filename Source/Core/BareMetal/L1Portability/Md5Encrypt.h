/**
 * @file Md5Encrypt.h
 * @brief Header file for class Md5Encrypt
 * @date 23/08/2018
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

 * @details This header file contains the declaration of the class Md5Encrypt
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MD5ECRYPT_H_
#define MD5ECRYPT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Contains the functions to compute MD5 hash functions.
 */
namespace Md5Encrypt {


/**
 * @brief Performs the MD5 hash function on \a input generating the result in \a output
 * @param[in] input the input.
 * @param[in] ilen the input length.
 * @param[out] output the result of the MD5(input) function.
 */
void Md5(uint8 *const input,
         const uint32 ilen,
         uint8 *const output);



/**
 * @brief Adds the HMAC authentication mode to the MD5 hash function.
 * @param[in] key the secret key
 * @param[in] keylen the length of \a key
 * @param[in] input the input.
 * @param[in] ilen the length of \a input.
 * @param[out] output the result of MD5_HMAC(input)
 */
void Md5Hmac(const uint8 *const key,
             const uint32 keylen,
             uint8 *const input,
             const uint32 ilen,
             uint8 *const output);

}

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L1PORTABILITY_MD5ECRYPT_H_ */

