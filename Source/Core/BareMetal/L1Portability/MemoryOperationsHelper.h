/**
 * @file MemoryOperationsHelper.h
 * @brief Header file for module MemoryOperationsHelper
 * @date 14/08/2015
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the module MemoryOperationsHelper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MEMORYOPERATIONSHELPER_H_
#define MEMORYOPERATIONSHELPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

namespace MemoryOperationsHelper {

/**
 * @brief Copies a block of memory into another.
 * @details A copy of size bytes from source location to destination position.
 * @param[in,out] destination is the pointer to the destination memory location.
 * @param[in] source is the pointer to the source memory location.
 * @param[in] size is the size of the memory to be copied.
 * @return true if source, destination and destination after copy are not NULL.
 * @pre the size parameter must be minor than the memory blocks sizes.
 */
DLL_API bool Copy(void * const destination,
                  const void * const source,
                  uint32 size);

/**
 * @brief Compares the first specified bytes of two blocks of memories.
 * @param[in] mem1 is the pointer to the first memory location.
 * @param[in] mem2 is the pointer to the second memory location.
 * @param[in] size is the number of byte to compare.
 * @return (0 if mem1 == mem2), (1 if mem1 < mem2), (2 if mem1 > mem2).
 * @pre the size parameter must be minor than the memory blocks sizes.
 */
DLL_API int32 Compare(const void * const mem1,
                      const void * const mem2,
                      uint32 size);

/**
 * @brief Searches a character in the specified memory block.
 * @param[in] mem is the memory location.
 * @param[in] c is the character to find.
 * @param[in] size is the size of the memory area.
 * @return the pointer to the first occurrence of c in the memory. NULL if c is absent.
 * @pre the size parameter must be minor than the memory block size.
 */
DLL_API const void *Search(const void * const mem,
                           const char8 c,
                           const uint32 size);

/**
 * @brief Copies a block of memory into another allowing overlapping.
 * @param[in,out] destination is the pointer to the destination memory location.
 * @param[in] source is the pointer to the source memory location.
 * @param[in] size is the number of bytes to be copied.
 * @return true if source, destination, and destination after the copy are not NULL.
 * @pre the size parameter must be minor than the memory blocks sizes.
 */
DLL_API bool Move(void * const destination,
                  const void * const source,
                  const uint32 size);

/**
 * @brief Sets a defined number bytes of the specified memory area equal to a specified character.
 * @param[in,out] mem is the pointer to the memory location.
 * @param[in] c is the character to store.
 * @param[in] size is the number of bytes where c will be written.
 * @return true if the memory could be set to desired value.
 * @pre the size parameter must be minor than the memory block size.
 */
DLL_API bool Set(void * const mem,
                 const char8 c,
                 const uint32 size);

/**
 * @brief Converts the memory from an interleaved to flat representation for a specified number of samples.
 * @details Assume for instance an \a originSource with 2 samples organised as [(uint32) counter_1, (uint16) x_1, (uint8) y_1][(uint32) counter_2, (uint16) x_2, (uint8) y_2].
 * Giving the following parameters:\n
 *   packetMemberSize[] = {4, 2, 1}\n
 *   packetByteSize = 7\n
 *   numberOfPacketMembers = 3\n
 *   numberOfSamples = 2\n
 * Then the output in \a originDest will be [counter_1, counter_2][x_1, x_2][y_1, y_2].
 * param[in] originSource the source memory area
 * param[in] originDest the destination memory area
 * param[in] beginIndex is the start index in the \a packetMemberSize vector
 * param[in] packetMemberSize the size of each member in the sample (i.e. packet) definition
 * param[in] packetByteSize the size of each sample (i.e. of the packet) in bytes
 * param[in] numberOfPacketMembers the number elements of each samples (i.e. packet, i.e. the number of elements in the packetMemberSize)
 * param[in] numberOfSamples the number of samples
 * @pre
 *   originSource must be valid with a size greater than packetByteSize*numberOfSamples;\n
 *   originDest must be valid with a size greater than packetByteSize*numberOfSamples;\n
 * @post
 *   originDest is filled with the re-organised data from interleaved to flat.
 */
void InterleavedToFlat(uint8 * const originSource,
                       uint8 * const originDest,
                       const uint32 beginIndex,
                       const uint32 * const packetMemberSize,
                       const uint32 packetByteSize,
                       const uint32 numberOfPacketMembers,
                       const uint32 numberOfSamples);

/**
 * @brief Converts the memory from a flat to an interleaved representation for a specified number of samples.
 * @details Assume for instance an \a originSource with 2 samples organised as [(uint32)counter_1, counter_2][(uint16)x_1, x_2][(uint8)y_1, y_2].
 * Giving the following parameters:\n
 *   packetMemberSize[] = {4, 2, 1}\n
 *   packetByteSize = 7\n
 *   numberOfPacketMembers = 3\n
 *   numberOfSamples = 2\n
 * Then the output in \a originDest will be [(uint32) counter_1, (uint16) x_1, (uint8) y_1][(uint32) counter_2, (uint16) x_2, (uint8) y_2].
 * param[in] originSource the source memory area
 * param[in] originDest the destination memory area
 * param[in] beginIndex is the start index in the \a packetOutputChunkSize vector
 * param[in] packetMemberSize the size of each member in the sample (i.e. packet) definition
 * param[in] packetByteSize the size of the sample in bytes
 * param[in] numberOfPacketMembers the number elements of each samples (i.e. packet, i.e. the number of elements in the packetMemberSize)
 * param[in] numberOfSamples the number of samples
 * @pre
 *   originSource must be valid with a size greater than outputByteSize*numberOfSamples;\n
 *   originDest must be valid with a size greater than outputByteSize*numberOfSamples;\n
 * @post
 *   originDest is filled with the re-organised data from flat to interleaved.
 */
void FlatToInterleaved(uint8 * const originSource,
                       uint8 * const originDest,
                       const uint32 beginIndex,
                       const uint32 * const packetMemberSize,
                       const uint32 packetByteSize,
                       const uint32 numberOfPacketMembers,
                       const uint32 numberOfSamples);

}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MEMORYOPERATIONSHELPER_H_ */

