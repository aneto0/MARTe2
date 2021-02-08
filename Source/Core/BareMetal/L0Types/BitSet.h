/**
 * @file BitSet.h
 * @brief Header file for class BitSet
 * @date 04/02/2021
 * @author Martino Ferrari
 *
 * @copyright ITER | International Thermonuclear Experimental Reactor.
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved
 * by the European Commission - subsequent versions of the EUPL (the "Licence")
 * You may not use this work except in compliance with the Licence.
 * You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
 *
 * @warning Unless required by applicable law or agreed to in writing,
 * software distributed under the Licence is distributed on an "AS IS"
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the Licence permissions and limitations under the Licence.

 * @details This header file contains the declaration of the class BitSet
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L0TYPESDEV_BITSET_H_
#define L0TYPESDEV_BITSET_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <cstdlib>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "TypeCharacteristics.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 *  @brief A variable size bit set compatible with standard bitwise operations.
 *  @details Using this type allows to create bitmask of albitrary size.
 */
class BitSet {
public:
    
    /**
     * @brief empty constructor.
     * @details initialize with a single uint32 0 value.
     */  
    BitSet();
    
    /**
     * @brief uint32 constructor.
     * @details copy the uint32 value to the bitset.
     * @param[in] value uint32 value to copy.
     */ 
    BitSet(uint32 value);

    /**
     * @brief uint64 constructor
     * @details copy the uint64 value as a pair of uint32 in the bitset.
     * @param[in] value uint64 value to copy.
     */
    BitSet(uint64 value);

    /**
     * @brief clone constructor.
     * @param[in] other bitset to be cloned.
     */
    BitSet(const BitSet &other);

    /**
     * @brief simple distructor.
     * @details free the memory used to store the bitset.
     */
    ~BitSet();

    /**
     * @brief Get the number of uint32 values used.
     * @return Number of elements used to store the bitset.
     */  
    uint32 GetByteSize();

    /**
     * @brief Get the status of a single bit.
     * @param[in] index bit position.
     * @return value of the bit, if not defined 0.
     */ 
    bool Bit(uint32 index);

    /**
     * @brief Set a single bit.
     * @param[in] index bit position.
     * @param[in] value bit value.
     */
    void Set(uint32 index, bool value);
    
    
    /**
     * @brief assign BitSet operator.
     * @param[in] other value to be copied.
     * @return initialized BitSet with same value than the original.
     */
    BitSet & operator=(const BitSet& other);

    /**
     * @brief assign BitSet operator from uint32.
     * @param[in] other uint32 value to be copied.
     * @return initialized BitSet.
     */
    BitSet & operator=(const uint32& other);
    BitSet & operator=(const uint64& other);

    operator uint32() const;
    operator uint64() const;

    BitSet operator&(const BitSet& rhm);
    BitSet operator|(const BitSet& rhm);
    BitSet operator^(const BitSet& rhm);
    BitSet operator~();

    BitSet operator<<(const int& rhf);
    BitSet operator>>(const int& rhm);

    bool operator==(const BitSet& rhm) const;
    bool operator==(const uint32& rhm) const;
    bool operator==(const uint64& rhm) const;
    bool operator!=(const BitSet& rhm) const;
    bool operator!=(const uint32& rhm) const;
    bool operator!=(const uint64& rhm) const;

    BitSet & operator|=(const BitSet& rhm);
    BitSet & operator&=(const BitSet& rhm);
    BitSet & operator^=(const BitSet& rhm);

private:
    /**
     * @brief array constructor
     * @details copy an array of uint32 bytes to the bitset.
     * @param[in] bytes array to be copied.
     * @param[in] size number of elements in the array.
     */
    BitSet(uint32* bytes, uint32 size);
    uint32 bytesSize;
    uint32 * bytes;

};

/*---------------------------------------------------------------------------*/
/*                        Methods Implementation                             */
/*---------------------------------------------------------------------------*/


}
#endif // L0TYPESDEV_BITSET_H_
