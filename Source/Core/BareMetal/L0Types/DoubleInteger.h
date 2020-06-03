/**
 * @file DoubleInteger.h
 * @brief Header file for class DoubleInteger
 * @date 21/10/2015
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

 * @details This header file contains the declaration of the class DoubleInteger
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DOUBLEINTEGER_H_
#define DOUBLEINTEGER_H_

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
 * @brief Template class which represents an integer which duplicates the size
 * of the also integer template T type.
 * @tparam T the integer type of the upper and lower part of the double integer
 * defined by this class. This integer type must provide at list the same
 * operators that the class provides.
 * @remark The interpretation of the endianity is system dependent.
 */
template<typename T>
class DoubleInteger {
public:
    /**
     * @brief Default constructor.
     * @post lower=0\n
     * upper=0.
     */
    DoubleInteger();

    /**
     * @brief Copy constructor.
     * @post[in] n Instance to be copied.
     * @post lower = n.lower\n
     * upper = n.upper.
     */
    DoubleInteger(const DoubleInteger<T> &n);

    /**
     * @brief Constructor by upper and lower part.
     * @param[in] upperIn upper part of the double integer.
     * @param[in] lowerIn lower part of the double integer.
     * @post lower = lowerIn\n
     * upper = upperIn.
     */
    DoubleInteger(const T &upperIn,
                  const T &lowerIn);

    /**
     * @brief Smart copy constructor by generic integer type.
     * @details Builds the double integer depending on the size of T2.
     * @tparam T2 the integer type of the n parameter.
     * @param[in] n the integer value
     */
    template<typename T2>
    DoubleInteger(T2 n);

    /**
     * @brief Implementation of arithmetic left shift.
     * @param[in] shift is the desired shift.
     */
    void operator<<=(uint16 shift);

    /**
     * @brief Implementation of arithmetic right shift.
     * @param[in] shift is the desired shift.
     */
    void operator>>=(uint16 shift);

    /**
     * @brief Implementation of arithmetic right shift.
     * @param[in] shift is the desired shift.
     * @return this right shifted of shift.
     */
    DoubleInteger<T> operator>>(const uint8 shift) const;

    /**
     * @brief Implementation of arithmetic left shift.
     * @param[in] shift is the desired shift.
     * @return this left shifted of shift.
     */
    DoubleInteger<T> operator<<(const uint8 shift) const;

    /**
     * @brief Bitwise and operator.
     * @param[in,out] n is the DoubleInteger argument.
     */
    void operator &=(const DoubleInteger<T> &n);

    /**
     * @brief Bitwise or operator.
     * @param[in] n is the DoubleInteger argument.
     */
    void operator|=(const DoubleInteger<T> &n);

    /**
     * @brief Is Different operator.
     * @param[in] n is the DoubleInteger argument for comparison.
     * @return false if this and n are equal, true otherwise.
     */
    bool operator!=(const DoubleInteger<T> &n) const;

    /**
     * @brief Is Equal operator.
     * @param[in] n is the DoubleInteger to be compared.
     * @return true if this and n are equal, false otherwise.
     */
    bool operator==(const DoubleInteger<T> &n) const;

    /**
     * @brief Bitwise invert.
     */
    DoubleInteger<T> operator~() const;

    /**
     * @brief Bitwise and.
     * @param[in,out] n is the DoubleInteger argument.
     * @return the result of the bitwise and between this and n.
     */
    DoubleInteger<T> operator &(const DoubleInteger<T> &n) const;

    /**
     * @brief Bitwise or.
     * @param[in] n is the DoubleInteger argument.
     * @return the result of the bitwise or between this and n.
     */
    DoubleInteger<T> operator|(const DoubleInteger<T> &n) const;

    /**
     * @brief Math subtraction.
     * @param[in] n is the number to subtract.
     * @return the result of the subtraction.
     */
    DoubleInteger<T> operator-(const DoubleInteger<T> &n) const;

    /**
     * @brief Math sum.
     * @param[in] n is the number to add.
     * @return the sum.
     */
    DoubleInteger<T> operator+(const DoubleInteger<T> &n) const;

    /**
     * @brief Logical >.
     * @param[in] n is the DoubleInteger to be compared.
     * @return true if this is greater than n.
     */
    bool operator>(const DoubleInteger<T> &n) const;

    /**
     * @brief Logical <.
     * @param[in] n is the DoubleInteger to be compared.
     * @return true if this is minor than n.
     */
    bool operator<(const DoubleInteger<T> &n) const;

    /**
     * @brief Logical >=.
     * @param[in] n is the DoubleInteger be compared.
     * @return true if this is greater or equal than n.
     */
    bool operator>=(const DoubleInteger<T> &n) const;

    /**
     * @brief Logical <=.
     * @param[in] n is the DoubleInteger for be compared.
     * @return true if this is minor than n.
     */
    bool operator<=(const DoubleInteger<T> &n) const;

    /**
     * @brief Math subtraction.
     * @param[in] n is the number to subtract.
     */
    void operator-=(const DoubleInteger<T> &n);

    /**
     * @brief Math sum.
     * @param[in] n is the number to add.
     */
    void operator+=(const DoubleInteger<T> &n);

    /**
     * @brief Gets the the second part of the number.
     */
    T GetLower() const;

    /**
     * @brief Gets the first part of the number.
     */
    T GetUpper() const;

    /**
     * @brief Sets the the second part of the number.
     */
    void SetLower(T lowerIn);

    /**
     * @brief Sets the the first part of the number.
     */
    void SetUpper(T upperIn);

private:

    /**
     * @brief Checks if the template is a signed type.
     * @return true if the template is a signed type, false otherwise.
     */
    static inline bool IsSigned();

    /**
     * Least significative.
     */
    T lower;

    /**
     * Most significative.
     */
    T upper;

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
bool DoubleInteger<T>::IsSigned() {
    return (static_cast<T>(-1)) < static_cast<T>(0);
}

template<typename T>
DoubleInteger<T>::DoubleInteger() {
    lower = static_cast<T>(0);
    upper = static_cast<T>(0);
}

template<typename T>
DoubleInteger<T>::DoubleInteger(const T &upperIn,
                                const T &lowerIn) {
    lower = lowerIn;
    upper = upperIn;
}

template<typename T>
DoubleInteger<T>::DoubleInteger(const DoubleInteger<T> &n) {
    lower = n.lower;
    upper = n.upper;
}

template<typename T>
template<typename T2>
DoubleInteger<T>::DoubleInteger(T2 n) {
    //if n=0 initialize to zero.
    if (n == static_cast<T2>(0)) {
        lower = static_cast<T>(0);
        upper = static_cast<T>(0);
    }
    else if (n > static_cast<T2>(0)) {
        //cast the lower bits
        lower = static_cast<T>(n);
        upper = static_cast<T>(0);
        if (sizeof(T2) > sizeof(T)) {
            uint8 shift = (sizeof(T) * 8u);
            //put in upper the most significative bits remained.
            n >>= shift;
            upper = static_cast<T>(n);
        }
    }
    else {
        //cast the lower bits
        lower = static_cast<T>(n);
        upper = static_cast<T>(-1);
        if (sizeof(T2) > sizeof(T)) {
            uint8 shift = static_cast<uint8>(sizeof(T) * 8u);
            //put in upper the most significative bits remained.
            n >>= shift;
            upper = static_cast<T>(n);
        }

    }
}

template<typename T>
void DoubleInteger<T>::operator<<=(uint16 shift) {
    // shift of sizeof(T)*8 is treated as shift 0
    // for this reason exit here to avoid this pitfall
    if (shift > 0u) {

        // calculates n of bits of T
        uint16 bitSize = static_cast<uint16>(sizeof(T) * 8u);
        // shift within one half
        if (shift < bitSize) {
            // shift upper first
            upper = upper << shift;
            // add overflow from lower
            // this would fail if shift is 0
            upper |= (lower >> (bitSize - shift));
            // complete the lower
            lower = lower << shift;
        }
        else { // more than half!
               // remove half
            shift -= bitSize;
            // swap lower -> upper and shift with the remainder
            upper = lower << shift;
            // lower is 0
            lower = static_cast<T>(0);
        }
    }
}

template<typename T>
void DoubleInteger<T>::operator>>=(uint16 shift) {
    // shift of sizeof(T)*8 is treated as shift 0
    // for this reason exit here to avoid this pitfall
    if (shift > 0u) {

        // calculates n of bits of T
        uint16 bitSize = static_cast<uint16>(sizeof(T) * 8u);
        // shift within one half
        if (shift < bitSize) {
            // shift lower first
            lower = lower >> shift;
            // add overflow from upper
            // this would fail if shift is 0
            lower |= (upper << (bitSize - shift));
            // complete the upper
            upper = upper >> shift;
        }
        else { // more than half!
               // remove half
            shift -= bitSize;
            // swap upper -> lower and shift with the remainder
            lower = upper >> shift;
            // upper is 0
            if (upper < static_cast<T>(0)) {
                upper = static_cast<T>(-1);
            }
            else {
                upper = static_cast<T>(0);
            }
        }
    }
}

template<typename T>
DoubleInteger<T> DoubleInteger<T>::operator>>(const uint8 shift) const {
    DoubleInteger<T> output(*this);
    // uses unary op
    output >>= shift;
    return output;
}

template<typename T>
DoubleInteger<T> DoubleInteger<T>::operator<<(const uint8 shift) const {
    DoubleInteger<T> output(*this);
    // uses unary op
    output <<= shift;
    return output;
}

template<typename T>
void DoubleInteger<T>::operator&=(const DoubleInteger<T> &n) {
    lower &= n.lower;
    upper &= n.upper;
}

template<typename T>
void DoubleInteger<T>::operator|=(const DoubleInteger<T> &n) {
    lower |= n.lower;
    upper |= n.upper;
}

template<typename T>
bool DoubleInteger<T>::operator!=(const DoubleInteger<T> &n) const {
    return ((upper != n.upper) || (lower != n.lower));
}

template<typename T>
bool DoubleInteger<T>::operator==(const DoubleInteger<T> &n) const {
    return !((*this) != n);
}

template<typename T>
DoubleInteger<T> DoubleInteger<T>::operator~() const {
    DoubleInteger<T> output;
    output.upper = ~upper;
    output.lower = ~lower;
    return output;
}

template<typename T>
DoubleInteger<T> DoubleInteger<T>::operator&(const DoubleInteger<T> &n) const {
    DoubleInteger<T> output;
    output.upper = upper & n.upper;
    output.lower = lower & n.lower;
    return output;
}

template<typename T>
DoubleInteger<T> DoubleInteger<T>::operator|(const DoubleInteger<T> &n) const {
    DoubleInteger<T> output;
    output.upper = upper | n.upper;
    output.lower = lower | n.lower;
    return output;
}

template<typename T>
DoubleInteger<T> DoubleInteger<T>::operator-(const DoubleInteger<T> &n) const {
    DoubleInteger<T> output;
    bool canReturn = false;
    // subtract upper
    output.upper = upper - n.upper;
    output.lower = lower - n.lower;

    //if the template is signed we must consider negative number
    //as unsigned number then we must decrement upper when:
    //lowers both negative or both positive, if n.lower > lower.
    //lower positive and n.lower negative.
    if (IsSigned()) {
        if ((lower >= static_cast<T>(0)) && (n.lower < static_cast<T>(0))) {
            output.upper--;
            canReturn = true;
        }

        if ((lower < static_cast<T>(0)) && (n.lower >= static_cast<T>(0))) {
            canReturn = true;
        }

    }

    if (!canReturn) {
        // if there will be an underflow borrow from upper
        if (n.lower > lower) {
            output.upper--;
        }
    }
    // subtract lower
    return output;
}

template<typename T>
DoubleInteger<T> DoubleInteger<T>::operator+(const DoubleInteger<T> &n) const {
    DoubleInteger<T> output;
    bool canReturn = false;
    // add upper
    output.upper = upper + n.upper;
    // add lower
    output.lower = lower + n.lower;

    //if the template is signed and the the two lowers are negative increment upper
    //because we need to consider them as unsigned values.
    if (IsSigned()) {
        if ((lower < static_cast<T>(0)) && (n.lower < static_cast<T>(0))) {
            output.upper++;
            canReturn = true;
        }
    }
    if (!canReturn) {

        // handle overflow
        //case are both positive or one negative one positive.
        //if result is negative, no overflow happens.
        //the result has to be be minor than the positive otherwise is overflow.
        if (((output.lower < lower) || (output.lower < n.lower)) && (output.lower >= 0)) {
            output.upper++;
        }
    }
    return output;
}

template<typename T>
bool DoubleInteger<T>::operator>(const DoubleInteger<T> &n) const {

    bool canReturn = false;
    bool retVal = false;
    if (upper == n.upper) {
        //in case of same upper
        if (IsSigned()) {
            //consider the lower as unsigned.
            if ((lower < static_cast<T>(0)) && (n.lower >= static_cast<T>(0))) {
                retVal = true;
                canReturn = true;
            }
            if ((lower >= static_cast<T>(0)) && (n.lower < static_cast<T>(0))) {
                retVal = false;
                canReturn = true;
            }
        }
    }
    else {
        retVal = (upper > n.upper);
        canReturn = true;
    }

    if (!canReturn) {
        retVal = (lower > n.lower);
    }

    return retVal;
}

template<typename T>
bool DoubleInteger<T>::operator<(const DoubleInteger<T> &n) const {
    DoubleInteger<T> output = (*this);
    return (!(output > n)) && (output != n);
}

template<typename T>
bool DoubleInteger<T>::operator>=(const DoubleInteger<T> &n) const {
    DoubleInteger<T> output = (*this);
    return !(output < n);
}

template<typename T>
bool DoubleInteger<T>::operator<=(const DoubleInteger<T> &n) const {
    DoubleInteger<T> output = (*this);
    return !(output > n);
}

template<typename T>
void DoubleInteger<T>::operator-=(const DoubleInteger<T> &n) {
    DoubleInteger<T> output = (*this);
    (*this) = output - n;
}

template<typename T>
void DoubleInteger<T>::operator+=(const DoubleInteger<T> &n) {
    DoubleInteger<T> output = (*this);
    (*this) = output + n;
}

template<typename T>
T DoubleInteger<T>::GetLower() const {
    return lower;
}

template<typename T>
T DoubleInteger<T>::GetUpper() const {
    return upper;
}

template<typename T>
void DoubleInteger<T>::SetLower(T lowerIn) {
    lower = lowerIn;
}

template<typename T>
void DoubleInteger<T>::SetUpper(T upperIn) {
    upper = upperIn;
}

}

#endif /* DOUBLEINTEGER_H_ */

