/**
 * @file SafeMath.h
 * @brief Header file for class SafeMath
 * @date 14/04/2020
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

 * @details This header file contains the declaration of the class SafeMath
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef SAFEMATH_H_
#define SAFEMATH_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "FastMath.h"
#include "TypeCharacteristics.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Module declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

    namespace SafeMath{

        /**
         * @brief Generic template implementation to find the sign bit of number type.
         * @tparam T the type whose sign bit is required.
         * @return sign bit mask of type T.
         */
        template <typename T> inline const T SignBit();

        /**
         * @brief Template specialization for int8.
         */
        template <> inline const int8 SignBit<int8>();

        /**
         * @brief Template specialization for int16.
         */
        template <> inline const int16 SignBit<int16>();

        /**
         * @brief Template specialization for int32.
         */
        template <> inline const int32 SignBit<int32>();

        /**
         * @brief Template specialization for int64.
         */
        template <> inline const int64 SignBit<int64>();

        /**
         * @brief Generic template implementation that returns a number of type T with all bits set.
         * @tparam T the type of number to return.
         * @return number of type T with all bits set.
         */
        template <typename T> inline const T AllOnes();

        /**
         * @brief Template specialization for int8.
         */
        template <> inline const int8 AllOnes<int8>();

        /**
         * @brief Template specialization for int16.
         */
        template <> inline const int16 AllOnes<int16>();

        /**
         * @brief Template specialization for int32.
         */
        template <> inline const int32 AllOnes<int32>();

        /**
         * @brief Template specialization for int64.
         */
        template <> inline const int64 AllOnes<int64>();

        /**
         * @brief     Template implementation to safely check equality
         *            between two numbers.
         * @tparam    T   type of numbers undergoing equality check operation.
         * @param[in] a1  first operation argument.
         * @param[in] a2  second operation argument.
         * @details   Comparison of floating-point types may often be
         *            implementation-dependent. This implementation 
         *            of the comparison operation takes into account
         *            the floating-point granularity (machine epsilon)
         *            to achieve a safely portable equality check.  
         * @return    `true` if difference between the two numbers is
         *            less than the machine epsilon.
         */
        template <typename T> inline bool IsEqual(T a1, T a2);

        /**
         * @brief Generic template implementation to compute addition of two numbers.
         * @tparam T type of numbers receiving operation.
         * @param[in] a1 is first operation argument.
         * @param[in] a2 is second operation argument.
         * @param[out] sum is the operation result.
         * @return ErrorManagement::NoError|ErrorManagement::Overflow|ErrorManagement::Underflow depending on operation result is within T type range.
         */
        template <typename T> ErrorManagement::ErrorType Addition(T a1, T a2, T &sum);

        /**
         * @brief Generic template implementation to compute subtraction of two numbers.
         * @tparam T type of numbers receiving operation.
         * @param[in] a1 is first operation argument.
         * @param[in] a2 is second operation argument.
         * @param[out] sum is the operation result.
         * @return ErrorManagement::NoError|ErrorManagement::Overflow|ErrorManagement::Underflow depending on operation result is within T type range.
         */
        template <typename T> ErrorManagement::ErrorType Subtraction(T a1, T a2, T &sum);

        /**
         * @brief Generic template implementation to compute multiplication of two numbers.
         * @tparam T type of numbers receiving operation.
         * @param[in] a1 is first operation argument.
         * @param[in] a2 is second operation argument.
         * @param[out] product is the operation result.
         * @return ErrorManagement::NoError|ErrorManagement::Overflow|ErrorManagement::Underflow depending on operation result is within T type range.
         */
        template <typename T> ErrorManagement::ErrorType Multiplication(T a1, T a2, T &product);

    } // SafeMath

} // MARTe


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe{

    namespace SafeMath{

        template <typename T>
        inline const T SignBit(){
            return  0;
        }

        template <>
        inline const int8 SignBit<int8>(){
            return static_cast<int8>(0x80);
        }

        template <>
        inline const int16 SignBit<int16>(){
            return static_cast<int16>(0x8000);
        }

        template <>
        inline const int32 SignBit<int32>(){
            return static_cast<int32>(0x80000000LL);
        }

        template <>
        inline const int64 SignBit<int64>(){
            return static_cast<int64>(0x8000000000000000ULL);
        }

        template <typename T>
        inline const T AllOnes(){
            return  0;
        }

        template <>
        inline const int8 AllOnes<int8>(){
            return static_cast<int8>(0xFF);
        }

        template <>
        inline const int16 AllOnes<int16>(){
            return static_cast<int16>(0xFFFF);
        }

        template <>
        inline const int32 AllOnes<int32>(){
            return static_cast<int32>(0xFFFFFFFFLL);
        }

        template <>
        inline const int64 AllOnes<int64>(){
            return static_cast<int64>(0xFFFFFFFFFFFFFFFFULL);
        }

        template <typename T>
        inline bool IsEqual(T a1, T a2) {
            return (a1 == a2);
        }

        template <>
        inline bool IsEqual(float32 a1, float32 a2) {
            bool result = false;
            float32 difference = a1 - a2;
            result = (difference < TypeCharacteristics<float32>::Epsilon()) && (difference > -TypeCharacteristics<float32>::Epsilon());
            return result;
        }

        template <>
        inline bool IsEqual(float64 a1, float64 a2) {
            bool result = false;
            float64 difference = a1 - a2;
            result = (difference < TypeCharacteristics<float64>::Epsilon()) && (difference > -TypeCharacteristics<float64>::Epsilon());
            return result;
        }

        template <typename T>
        ErrorManagement::ErrorType Addition(T a1, T a2, T &sum){
            ErrorManagement::ErrorType ret;

            sum = static_cast<T>(a1 + a2);
            // check saturation
            if (TypeCharacteristics<T>::IsSigned()){
                // for signed numbers overflows happens on the sign bit:
                // this means only happens if signs were the same and if result has flipped sign
        //		const T signBit = static_cast<T>(static_cast<T>(1u) << (sizeof(T)*8u-1u));
                const T signBit = SignBit<T>();
                if ( (((a1 ^ a2) & signBit) == 0u) && (((a1 ^ sum) & signBit) != 0u)){
                    if ((a1 & signBit) == 0u){
                        ret = ErrorManagement::Overflow;
                    } else {
                        ret = ErrorManagement::Underflow;
                    }
                }
                // for unsigned the result is smaller than one of them
            } else {
                if ((sum < a1 ) && (sum < a2)){
                    ret = ErrorManagement::Overflow;
                }
            }

            return ret;
        }


        template <typename T>
        ErrorManagement::ErrorType Subtraction(T a1, T a2, T &sum){
            ErrorManagement::ErrorType ret;

            sum = static_cast<T>(a1 - a2);
            // check saturation
            if (TypeCharacteristics<T>::IsSigned()){
                // for signed numbers overflows happens on the sign bit:
                // this means only happens if signs were the same and if result has flipped sign
        //		const T signBit = static_cast<T>(static_cast<T>(1u) << (sizeof(T)*8u-1u));
                const T signBit = SignBit<T>();
                if ( (((a1 ^ a2) & signBit) != 0u) && (((a1 ^ sum) & signBit) != 0u)){
                    if ((a1 & signBit) == 0u){
                        ret = ErrorManagement::Overflow;
                    } else {
                        ret = ErrorManagement::Underflow;
                    }
                }
                // for unsigned the result is smaller than one of them
            } else {
                if (a2 > a1){
                    ret = ErrorManagement::Underflow;
                }
            }

            return ret;
        }

        template <typename T>
        ErrorManagement::ErrorType Multiplication(T a1, T a2, T &product){
            ErrorManagement::ErrorType ret;

            T overflow;
            product = FastMath::CompleteMultiply(a1,a2,overflow);

            //check overflow for signed numbers
            if (TypeCharacteristics<T>::IsSigned()){
                               /* positive ovf */
                ret.overflow  =  (overflow > 0)            ||
                               /* change in sign --> positive overflow into the sign bit of product*/
                                ((overflow == 0) && (product < 0));
                               /* negative ovf - allones() would be sign extension */
                ret.underflow =  (overflow < AllOnes<T>()) ||
                               /* product should be negative - overflow into sign bit of product */
                                ((overflow == AllOnes<T>()) && (product >= 0));
            } else {
                /* positive ovf */
                ret.overflow  =  (overflow != 0);
            }

            return ret;
        }

    } // SafeMath

} // MARTe


#endif /* SOURCE_CORE_BAREMETAL_L1PORTABILITY_SAFEMATH_H_ */
