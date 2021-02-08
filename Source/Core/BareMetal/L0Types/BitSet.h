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
    BitSet();
    BitSet(uint32);
    BitSet(uint64);
    BitSet(const BitSet &other);
    BitSet(uint32* bytes, uint32 size);

    ~BitSet();

    uint32 GetByteSize();

    bool Bit(uint32 index);
    void Set(uint32 index, bool value);
    
    BitSet & operator=(const BitSet& other);
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
    bool operator!=(const BitSet& rhm) const;
    bool operator!=(const uint32& rhm) const;

    BitSet & operator|=(const BitSet& rhm);
    BitSet & operator&=(const BitSet& rhm);
    BitSet & operator^=(const BitSet& rhm);
private:
    uint32 bytesSize;
    uint32 * bytes;

};

/*---------------------------------------------------------------------------*/
/*                        Methods Implementation                             */
/*---------------------------------------------------------------------------*/


}
#endif // L0TYPESDEV_BITSET_H_
