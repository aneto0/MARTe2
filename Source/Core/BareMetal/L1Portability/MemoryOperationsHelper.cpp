/**
 * @file MemoryOperationsHelper.cpp
 * @brief Source file for class MemoryOperationsHelper
 * @date 24/05/2018
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

 * @details This source file contains the definition of all the methods for
 * the class MemoryOperationsHelper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

void MemoryOperationsHelper::InterleavedToFlat(uint8 * const originSource,
        uint8 * const originDest,
        const uint32 beginIndex,
        const uint32 * const packetMemberSize,
        const uint32 packetByteSize,
        const uint32 numberOfPacketMembers,
        const uint32 numberOfSamples) {

    //this should be the number of samples
    /*lint -e{613} null pointer checked before.*/
    for (uint32 k = 0u; k < numberOfSamples; k++) {
        //return to the gam pointet
        uint8 *dest = originDest;
        //go to the k-th packet in the data source
        /*lint -e{613} null pointer checked before.*/
        uint32 index = (k * packetByteSize);
        /*lint -e{613} null pointer checked before.*/
        uint8 *source = &originSource[index];
        /*lint -e{613} null pointer checked before.*/
        for (uint32 i = 0u; i < numberOfPacketMembers; i++) {
            //get the chunk size
            index = (beginIndex + i);
            /*lint -e{613} null pointer checked before.*/
            uint32 chunkSize = packetMemberSize[index];
            //put the k-th element
            index = (k * chunkSize);
            dest = &dest[index];
            for (uint32 j = 0u; j < chunkSize; j++) {
                dest[j] = source[j];
            }
            //(void) MemoryOperationsHelper::Copy(dest, source, chunkSize);
            //skip the N-k other samples to go to the next chunk mem
            index = ((numberOfSamples - k) * chunkSize);
            dest = &dest[index];
            //get the next chunk in source
            source = &source[chunkSize];
        }
    }
}

void MemoryOperationsHelper::FlatToInterleaved(uint8 * const originSource, uint8 * const originDest, const uint32 beginIndex,
                                               const uint32 * const packetMemberSize, const uint32 packetByteSize, const uint32 numberOfPacketMembers,
                                               const uint32 numberOfSamples) {

    //this should be the number of samples
    /*lint -e{613} null pointer checked before.*/
    for (uint32 k = 0u; k < numberOfSamples; k++) {
        //return to the gam pointet
        uint8 *source = originSource;
        //go to the k-th packet in the data source
        /*lint -e{613} null pointer checked before.*/
        uint32 index = (k * packetByteSize);
        /*lint -e{613} null pointer checked before.*/
        uint8 *dest = &originDest[index];
        /*lint -e{613} null pointer checked before.*/
        for (uint32 i = 0u; i < numberOfPacketMembers; i++) {
            //get the chunk size
            index = (beginIndex + i);
            /*lint -e{613} null pointer checked before.*/
            uint32 chunkSize = packetMemberSize[index];
            //put the k-th element
            index = (k * chunkSize);
            source = &source[index];
            for (uint32 j = 0u; j < chunkSize; j++) {
                dest[j] = source[j];
            }
            //skip the N-k other samples to go to the next chunk mem
            index = ((numberOfSamples - k) * chunkSize);
            source = &source[index];
            //get the next chunk in source
            dest = &dest[chunkSize];
        }
    }
}
}

