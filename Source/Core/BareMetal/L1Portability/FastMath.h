/**
 * @file FastMath.h
 * @brief Header file for class FastMath
 * @date 26/06/2015
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

 * @details This header file contains the declaration of the module FastMath
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef FASTMATH_H_
#define FASTMATH_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include <math.h>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorManagement.h"
#include "CompilerTypes.h"
#include INCLUDE_FILE_ARCHITECTURE(BareMetal,L1Portability,ARCHITECTURE,FastMathA.h)


/*---------------------------------------------------------------------------*/
/*                           Module declaration                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

    namespace FastMath {

        /**
         * The constant for e
         */
        const float64 E = 2.7182818284590452354;

        /**
         * The constant for log2(e)
         */
        const float64 LOG2E = 1.4426950408889634074;

        /**
         * The constant for log10(e)
         */
        const float64 LOG10E = 0.43429448190325182765;

        /**
         * The constant for log(2)
         */
        const float64 LN2 = 0.69314718055994530942;

        /**
         * The constant for log(10)
         */
        const float64 LN10 = 2.30258509299404568402;

        /**
         * The constant for pi
         */
        const float64 PI = 3.14159265358979323846;

        /**
         * The constant for pi/2
         */
        const float64 PI_2 = 1.57079632679489661923;

        /**
         * The constant for pi/4
         */
        const float64 PI_4 = 0.78539816339744830962;

        /**
         * The constant for 1/pi
         */
        const float64 ONE_PI = 0.31830988618379067154;

        /**
         * The constant for 2/pi
         */
        const float64 TWO_PI = 0.63661977236758134308;

        /**
         * The constant for 2/sqrt(pi)
         */
        const float64 TWO_SQRTPI = 1.12837916709551257390;

        /**
         * The constant for sqrt(2)
         */
        const float64 SQRT2 = 1.41421356237309504880;

        /**
         * The constant for sqrt(1/2)
         */
        const float64 SQRT1_2 = 0.70710678118654752440;

        /**
         * @brief Converts a float to an integer using a processor instruction.
         * @param[in] input the value to convert
         * @return the input as an integer
         */
        /*lint -e(762) This declaration is redundant. */
        inline int32 FloatToInt(const float32 input);

        /**
         * @brief Computes the cosine of an angle using a processor instruction.
         * @param[in] angle the angle to compute
         * @return the cosine of angle
         */
        /*lint -e(762) This declaration is redundant. */
        inline float32 Cos(const float32 angle);

        /**
         * @brief Computes the sine of an angle using a processor instruction.
         * @param[in] angle the angle to compute
         * @return the sine of angle
         */
        /*lint -e(762) This declaration is redundant. */
        inline float32 Sin(const float32 angle);

        /**
         * @brief Generic template implementation to compute square root of number.
         * @details If applied to a negative signed integer, the function typecasts
         * its additive inverse to unsigned and computes the square root of the casted integer.
         * @details If applied to a negative float, the function computes the square root of the additive inverse.
         * @tparam Type type of number and square root result.
         * @param[in] x is argument to compute the square root of.
         * @return the truncated (in the same type) square root of x.
         */
        template<typename Type> inline Type SquareRoot(const Type x);

        /**
         * @brief Template implementation for uint8.
         */
        template<> inline uint8 SquareRoot<uint8>(const uint8 x);

        /**
         * @brief Template implementation for uint16.
         */
        template<> inline uint16 SquareRoot<uint16>(const uint16 x);

        /**
         * @brief Template implementation for uint32.
         */
        template<> inline uint32 SquareRoot<uint32>(const uint32 x);

        /**
         * @brief Template implementation for uint64.
         */
        template<> inline uint64 SquareRoot<uint64>(const uint64 x);

        /**
         * @brief Template implementation for int8.
         * @details If applied to a negative signed integer, the function typecasts
         * it to unsigned and computes the square root of the casted integer.
         */
        template<> inline int8 SquareRoot<int8>(const int8 x);

        /**
         * @brief Template implementation for int16.
         * @details If applied to a negative signed integer, the function typecasts
         * it to unsigned and computes the square root of the casted integer.
         */
        template<> inline int16 SquareRoot<int16>(const int16 x);

        /**
         * @brief Template implementation for int32.
         * @details If applied to a negative signed integer, the function typecasts
         * it to unsigned and computes the square root of the casted integer.
         */
        template<> inline int32 SquareRoot<int32>(const int32 x);

        /**
         * @brief Template implementation for int64.
         * @details If applied to a negative signed integer, the function typecasts
         * it to unsigned and computes the square root of the casted integer.
         */
        template<> inline int64 SquareRoot<int64>(const int64 x);

        /**
         * @brief Template implementation for float32.
         * @details If applied to a negative float, the function typecasts
         * it to unsigned and computes the square root of the casted float.
         */
        template<> inline float32 SquareRoot<float32>(const float32 x);

        /**
         * @brief Computes the product of two integers into two numbers of the same type.
         * @param[in] x1 first  multiplicand
         * @param[in] x2 second multiplicand
         * @param[out] high the high word of the result
         * @return the normal product of two numbers
         */
        inline uint8  CompleteMultiply(uint8  x1,uint8  x2,uint8  &high);

        /**
         * @brief Computes the product of two integers into two numbers of the same type.
         * @param[in] x1 first  multiplicand
         * @param[in] x2 second multiplicand
         * @param[out] high the high word of the result
         * @return the normal product of two numbers
         */
        inline uint16 CompleteMultiply(uint16 x1,uint16 x2,uint16 &high);

        /**
         * @brief Computes the product of two integers into two numbers of the same type.
         * @param[in] x1 first  multiplicand
         * @param[in] x2 second multiplicand
         * @param[out] high the high word of the result
         * @return the normal product of two numbers
         */
        inline uint32 CompleteMultiply(uint32 x1,uint32 x2,uint32 &high);

        /**
         * @brief Computes the product of two integers into two numbers of the same type.
         * @param[in] x1 first  multiplicand
         * @param[in] x2 second multiplicand
         * @param[out] high the high word of the result
         * @return the normal product of two numbers
         */
        inline uint64 CompleteMultiply(uint64 x1,uint64 x2,uint64 &high);

        /**
         * @brief Computes the product of two integers into two numbers of the same type.
         * @param[in] x1 first  multiplicand
         * @param[in] x2 second multiplicand
         * @param[out] high the high word of the result
         * @return the normal product of two numbers
         */
        inline int8  CompleteMultiply(int8  x1,int8  x2,int8  &high);

        /**
         * @brief Computes the product of two integers into two numbers of the same type.
         * @param[in] x1 first  multiplicand
         * @param[in] x2 second multiplicand
         * @param[out] high the high word of the result
         * @return the normal product of two numbers
         */
        inline int16 CompleteMultiply(int16 x1,int16 x2,int16 &high);

        /**
         * @brief Computes the product of two integers into two numbers of the same type.
         * @param[in] x1 first  multiplicand
         * @param[in] x2 second multiplicand
         * @param[out] high the high word of the result
         * @return the normal product of two numbers
         */
        inline int32 CompleteMultiply(int32 x1,int32 x2,int32 &high);

        /**
         * @brief Computes the product of two integers into two numbers of the same type.
         * @param[in] x1 first  multiplicand
         * @param[in] x2 second multiplicand
         * @param[out] high the high word of the result
         * @return the normal product of two numbers
         */
        inline int64 CompleteMultiply(int64 x1,int64 x2,int64 &high);

    }
}
    /*---------------------------------------------------------------------------*/
    /*                        Inline method definitions                          */
    /*---------------------------------------------------------------------------*/


namespace MARTe {
namespace FastMath {

template<typename Type>
inline Type SquareRoot(const Type x) {

    Type tmp = x;

    if (tmp < (Type) 0) {
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "SquareRoot<Type> of negative number");
        tmp *= -1;
    }

    return sqrt(tmp);
}

template<>
inline uint8 SquareRoot<uint8>(const uint8 x) { /* From http://en.wikipedia.org/wiki/Methods_of_computing_square_roots */

    uint8 tmp = x;
    uint8 res = 0u;
//uint8 bit = static_cast<uint8>(1u << 6); // The second-to-top bit is set
    uint8 bit = static_cast<uint8>(0x40u); // The second-to-top bit is set

// "bit" starts at the highest power of four <= the argument.
    while (bit > tmp) {
        bit >>= 2;
    }

    while (bit != 0u) {
        if (tmp >= (res + bit)) {
            tmp -= res + bit;
            res = (res >> 1) + bit;
        }
        else {
            res >>= 1;
        }

        bit >>= 2;
    }

    return res;

}

template<>
inline uint16 SquareRoot<uint16>(const uint16 x) { /* From http://en.wikipedia.org/wiki/Methods_of_computing_square_roots */

    uint16 tmp = x;
    uint16 res = 0u;
//uint16 bit = static_cast<uint16>(1u << 14); // The second-to-top bit is set
    uint16 bit = static_cast<uint16>(0x4000u); // The second-to-top bit is set

// "bit" starts at the highest power of four <= the argument.
    while (bit > tmp) {
        bit >>= 2;
    }

    while (bit != 0u) {
        if (tmp >= (res + bit)) {
            tmp -= res + bit;
            res = (res >> 1) + bit;
        }
        else {
            res >>= 1;
        }

        bit >>= 2;
    }

    return res;

}

template<>
inline uint32 SquareRoot<uint32>(const uint32 x) { /* From http://en.wikipedia.org/wiki/Methods_of_computing_square_roots */

    uint32 tmp = x;
    uint32 res = 0u;
//uint32 bit = static_cast<uint32>(1u << 30); // The second-to-top bit is set
    uint32 bit = 0x40000000u; // The second-to-top bit is set

// "bit" starts at the highest power of four <= the argument.
    while (bit > tmp) {
        bit >>= 2;
    }

    while (bit != 0u) {
        if (tmp >= (res + bit)) {
            tmp -= res + bit;
            res = (res >> 1) + bit;
        }
        else {
            res >>= 1;
        }

        bit >>= 2;
    }

    return res;

}

template<>
inline uint64 SquareRoot<uint64>(const uint64 x) { /* From http://en.wikipedia.org/wiki/Methods_of_computing_square_roots */

    uint64 tmp = x;
    uint64 res = 0u;
//uint64 bit = static_cast<uint64>(1ul << 62); // The second-to-top bit is set
    uint64 bit = 0x4000000000000000ul; // The second-to-top bit is set

// "bit" starts at the highest power of four <= the argument.
    while (bit > tmp) {
        bit >>= 2;
    }

    while (bit != 0u) {
        if (tmp >= (res + bit)) {
            tmp -= res + bit;
            res = (res >> 1) + bit;
        }
        else {
            res >>= 1;
        }

        bit >>= 2;
    }

    return res;

}

template<>
inline int8 SquareRoot<int8>(const int8 x) {

    int8 tmp2 = x;
    if (x < 0) {
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "SquareRoot<int8> of negative number");
        tmp2 *= -1;
    }
    uint8 tmp = static_cast<uint8>(tmp2);
    int8 res = static_cast<int8>(SquareRoot<uint8>(tmp));

    return res;
}

template<>
inline int16 SquareRoot<int16>(const int16 x) {

    int16 tmp2 = x;
    if (x < 0) {
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "SquareRoot<int16> of negative number");
        tmp2 *= -1;
    }
    uint16 tmp = static_cast<uint16>(tmp2);
    int16 res = static_cast<int16>(SquareRoot<uint16>(tmp));

    return res;
}

template<>
inline int32 SquareRoot<int32>(const int32 x) {

    int32 tmp2 = x;
    if (x < 0) {
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "SquareRoot<int32> of negative number");
        tmp2 *= -1;
    }
    uint32 tmp = static_cast<uint32>(tmp2);
    int32 res = static_cast<int32>(SquareRoot<uint32>(tmp));

    return res;
}

template<>
inline int64 SquareRoot<int64>(const int64 x) {

    int64 tmp2 = x;
    if (x < 0) {
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "SquareRoot<int64> of negative number");
        tmp2 *= -1LL;
    }
    uint64 tmp = static_cast<uint64>(tmp2);
    int64 res = static_cast<int64>(SquareRoot<uint64>(tmp));

    return res;
}

template<>
inline float32 SquareRoot(const float32 x) {

    float32 tmp = x;

    if (tmp < 0.0F) {
        REPORT_ERROR_STATIC_0(ErrorManagement::Warning, "SquareRoot<float32> of negative number");
        tmp *= -1.0F;
    }

    return sqrtf(tmp);
}

} /* namespace FastMath */
} /* namespace MARTe */

#endif /* FASTMATH_H_ */

/*lint -e(766) The header file FastMathA.h is not used here. This file is just for
 * declaration and documentation. */
