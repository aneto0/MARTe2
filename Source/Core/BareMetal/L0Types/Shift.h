/**
 * @file Shift.h
 * @brief Header file for class Shift
 * @date 19/10/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class Shift
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L0TYPES_SHIFT_H_
#define L0TYPES_SHIFT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#include "GeneralDefinitions.h"

namespace MARTe {

/** @brief A collector of functions that implements logical and mathematic shift in safe mode.
 *
 * These methods implements shift operations in the right way, returning zero when the shift is
 * greater than the size of the number. Furthermore they implements the logical shift which
 * does not extend the sign for negative numbers and it is very useful in many functionalities.
 */
class Shift {

private:

    /** @brief Logical right shift for uint8 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number right shifted. */
    static inline uint8 LogicalRightShift(uint8 number,
                                          uint8 shift) {
        return number >> shift;
    }

    /** @brief Logical right shift for uint16 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number right shifted. */
    static inline uint16 LogicalRightShift(uint16 number,
                                           uint8 shift) {
        return number >> shift;
    }

    /** @brief Logical right shift for uint32 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number right shifted. */
    static inline uint32 LogicalRightShift(uint32 number,
                                           uint8 shift) {
        return number >> shift;
    }

    /** @brief Logical right shift for uint64 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number right shifted. */
    static inline uint64 LogicalRightShift(uint64 number,
                                           uint8 shift) {
        return number >> shift;
    }

    /** @brief Logical right shift for int8 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number right shifted. */
    static inline int8 LogicalRightShift(int8 number,
                                         uint8 shift) {
        return ((uint8) number) >> shift;
    }

    /** @brief Logical right shift for int16 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number right shifted. */
    static inline int16 LogicalRightShift(int16 number,
                                          uint8 shift) {
        return ((uint16) number) >> shift;
    }

    /** @brief Logical right shift for int32 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number right shifted. */
    static inline int32 LogicalRightShift(int32 number,
                                          uint8 shift) {
        return ((uint32) number) >> shift;
    }

    /** @brief Logical right shift for int64 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number right shifted. */
    static inline int64 LogicalRightShift(int64 number,
                                          uint8 shift) {
        return ((uint64) number) >> shift;
    }

    /** @brief Logical left shift for uint8 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number left shifted. */
    static inline uint8 LogicalLeftShift(uint8 number,
                                         uint8 shift) {
        return number << shift;
    }

    /** @brief Logical left shift for uint16 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number left shifted. */
    static inline uint16 LogicalLeftShift(uint16 number,
                                          uint8 shift) {
        return number << shift;
    }

    /** @brief Logical left shift for uint32 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number left shifted. */
    static inline uint32 LogicalLeftShift(uint32 number,
                                          uint8 shift) {
        return number << shift;
    }

    /** @brief Logical left shift for uint64 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number left shifted. */
    static inline uint64 LogicalLeftShift(uint64 number,
                                          uint8 shift) {
        return number << shift;
    }

    /** @brief Logical left shift for int8 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number left shifted. */
    static inline int8 LogicalLeftShift(int8 number,
                                        uint8 shift) {
        return number << shift;
    }

    /** @brief Logical left shift for int16 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number left shifted. */
    static inline int16 LogicalLeftShift(int16 number,
                                         uint8 shift) {
        return number << shift;
    }

    /** @brief Logical left shift for int32 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number left shifted. */
    static inline int32 LogicalLeftShift(int32 number,
                                         uint8 shift) {
        return number << shift;
    }

    /** @brief Logical left shift for int64 numbers.
     * @param number is the number to shift.
     * @param shift is the shift in bits.
     * @return the number left shifted. */
    static inline int64 LogicalLeftShift(int64 number,
                                         uint8 shift) {
        return number << shift;
    }

public:

    /** @brief Implementation of a template function for safe logical shifts.
     * @param number is the number to be shifted.
     * @param shift is the shift in bits.
     * @return the number shifted. */
    template<typename T>
    static inline T LogicalRightSafeShift(T number,
                                          uint8 shift) {

        uint8 bound = static_cast<uint8>(sizeof(T) * 8u);

        //if |shift| is greater than the size of the number return 0.
        if (shift >= bound) {
            return (T) 0;
        }

        return LogicalRightShift(number, shift);
    }

    /** @brief Implementation of a template function for safe logical shifts.
     * @param number is the number to be shifted.
     * @param shift is the shift in bits.
     * @return the number shifted. */
    template<typename T>
    static inline T LogicalLeftSafeShift(T number,
                                         uint8 shift) {

        uint8 bound = sizeof(T) * 8u;

        //if |shift| is greater than the size of the number return 0.
        if (shift >= bound) {
            return (T) 0;
        }

        return number << shift;
    }

    /** @brief Implementation of a template function for safe mathematic shifts.
     * @param number is the number to be shifted.
     * @param shift is the shift in bits.
     * @return the number shifted. */
    template<typename T>
    static inline T MathematicSafeShift(T number,
                                        int8 shift) {

        int bound = sizeof(T) * 8;

        //if |shift| is greater than the number size return 0.
        if (shift >= bound || shift <= -bound) {
            return (T) 0;
        }

        //if shift is greater than 0 d the left shift.
        if (shift > 0)
            return number << shift;
        //if the shift is minor than zero do the right shift.
        else
            return number >> (-shift);
    }

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* L0TYPES_SHIFT_H_ */

