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
     * @brief array constructor
     * @details copy an array of 32-bit words to the bitset.
     * @param[in] array to be copied.
     * @param[in] size number of 32-bit elements in the array.
     */
    BitSet(const uint32 marray[], const uint32 size);


    /**
     * @brief uint32 constructor.
     * @details copy the uint32 value to the bitset.
     * @param[in] value uint32 value to copy.
     */
    BitSet(const uint32 value);

    /**
     * @brief uint64 constructor
     * @details copy the uint64 value as a pair of uint32 in the bitset.
     * @param[in] value uint64 value to copy.
     */
    BitSet(const uint64 value);

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
    uint32 GetNumberOfElements() const;

    /**
     * @brief Get the status of a single bit.
     * @param[in] index bit position.
     * @return value of the bit, if not defined 0.
     */
    bool Bit(const uint32 index) const;

    /**
     * @brief Set a single bit.
     * @param[in] index bit position.
     * @param[in] value bit value.
     */
    void Set(const uint32 index, bool value);

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

    /**
     * @brief assing BitSet operator from uint64.
     * @param[in] other uint64 value to be copied.
     * @return initialized BitSet.
     */
    BitSet & operator=(const uint64& other);

    /**
     * @brief uint32 cast operator for back compatibility.
     * @return uint32 truncated value.
     */
    operator uint32() const;

    /**
     * @brief uint64 cast operator for back comaptibilty.
     * @return uint64 truncated value.
     */
    operator uint64() const;


    /**
     * @brief bitwise `and` operator.
     * @param[in] rhm right operand.
     * @return result of the bitwise `and` operator.
     */
    BitSet operator&(const BitSet& rhm) const;

    /**
     * @brief bitwise `or` operator.
     * @param[in] rhm right operand.
     * @return result of the bitwise `or` operator.
     */
    BitSet operator|(const BitSet& rhm) const;

    /**
     * @brief bitwise `xor` operator.
     * @param[in] rhm right operand.
     * @return result of the bitwise `xor` operator.
     */
    BitSet operator^(const BitSet& rhm) const;

    /**
     * @brief bitwise `not` operator.
     * @return inverse bitset.
     */
    BitSet operator~() const;

    /**
     * @brief left bit shift operator.
     * @param[in] rhm number of bits to be shifted.
     * @return left shifted bitset.
     */
    BitSet operator<<(const uint32& rhm) const;

    /**
     * @brief right bit shift operator.
     * @param[in] rhm number of bits to be shifted.
     * @return right shifted bitset. 
     */
    BitSet operator>>(const uint32& rhm) const;

    /**
     * @brief equality operator between two BitSets.
     * @param[in] rhm right operand.
     * @return true if the two bitsets are equivalents else false.
     */
    bool operator==(const BitSet& rhm) const;

    /**
     * @brief equality operator between a BitSet and an uint32.
     * @param[in] rhm right operand.
     * @return true if the two values are equivalents else false.
     */
    bool operator==(const uint32& rhm) const;

    /**
     * @brief equality operator between a BitSet and an uint64.
     * @param[in] rhm right operand.
     * @return true if the two values are equivalents else false.
     */
    bool operator==(const uint64& rhm) const;

    /**
     * @brief disequality operator between two BitSets.
     * @param[in] rhm right operand.
     * @return false if the two bitsets are equivalents else true.
     */
    bool operator!=(const BitSet& rhm) const;

    /**
     * @brief disequality operator between a BitSet and an uint32.
     * @param[in] rhm right operand.
     * @return false if the two values are equivalents else true.
     */
    bool operator!=(const uint32& rhm) const;

    /**
     * @brief disequality operator between a BitSet and an uint64.
     * @param[in] rhm right operand.
     * @return false if the two values are equivalents else true.
     */
    bool operator!=(const uint64& rhm) const;

    /**
     * @brief bitwise `or` assign operator.
     * @param[in] rhm right operands.
     * @return BitSet with updated value resulting from the bitwise `or` operator.
     */
    BitSet & operator|=(const BitSet& rhm);

    /**
     * @brief bitwise `and` assign operator.
     * @param[in] rhm right operands.
     * @return BitSet with updated value resulting from the bitwise `and` operator.
     */
    BitSet & operator&=(const BitSet& rhm);

    /**
     * @brief bitwise `xor` assign operator.
     * @param[in] rhm right operands.
     * @return BitSet with updated value resulting from the bitwise `xor` operator.
     */
    BitSet & operator^=(const BitSet& rhm);

    /**
     * @brief change array size.
     * @param[in] size new size (number of 32-bit elements).
     */
    void Resize(const uint32 size);

    /**
     * @brief Access to the internal array.
     */
    uint32 *GetArray();
private:


    /**
     * @brief size of the uint32 array used to store the bitset.
     */
    uint32 nElements;

    /**
     * @brief array of uint32 used to store the bitset.
     */
    uint32 * array;

    /*lint -e{1739} the operators are member functions*/
};

/*---------------------------------------------------------------------------*/
/*                        Methods Implementation                             */
/*---------------------------------------------------------------------------*/

}
#endif // L0TYPESDEV_BITSET_H_
