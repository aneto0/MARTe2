/**
 * @file FloatToInteger.cpp
 * @brief Source file for class FloatToInteger
 * @date 19/11/2015
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

 * @details This source file contains the definition of all the methods for
 * the class FloatToInteger (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "GeneralDefinitions.h"
#include "ErrorManagement.h"
#include "Shift.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {


/**
 * @brief Performs the conversion from a float number to an integer number.
 * @param[in] floatNumber is the float number to be converted.
 * @param[out] integerNumber is the converted integer number in output.
 * @return true.
 * @post
 *   If the source float number is negative and the destination type is unsigned, a warning will be generated an
 */
/*lint -e{550} [MISRA C++ Rule 0-1-6], [MISRA C++ Rule 0-1-9]. Justification: the symbol could be not accessed because it depends by input. */
/*lint -e{438} [MISRA C++ Rule 0-1-6], [MISRA C++ Rule 0-1-9]. Justification: the variable could be not used because it depends by input.*/
/*lint -e{774} [MISRA C++ Rule 0-1-1], [MISRA C++ Rule 0-1-2], [MISRA C++ Rule 0-1-9]. Justification: the integer type could be signed or not depending on the template type.*/
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
template<typename FloatType, typename IntegerType>
bool FloatToInteger(const FloatType floatNumber,
                    IntegerType &integerNumber) {

    bool ret = true;

    integerNumber = static_cast<IntegerType>(0);

    bool isDestSigned = (static_cast<IntegerType>(-1) < static_cast<IntegerType>(0));
    bool isSourcePositive = (floatNumber > static_cast<FloatType>(0.0));

    // exit if dest is unsigned and source negative.
    if ((isDestSigned) || (isSourcePositive)) {
        IntegerType max = static_cast<IntegerType>(-1);
        IntegerType min = static_cast<IntegerType>(0);

        if (isDestSigned) {
            max = Shift::LogicalRightSafeShift(max, 1u);
            min = Shift::LogicalLeftSafeShift(static_cast<IntegerType>(1), static_cast<uint8>((sizeof(IntegerType) * 8u) - 1u));
        }

        if (floatNumber >= static_cast<FloatType>(max)) {
            REPORT_ERROR(ErrorManagement::warning, "FloatToInteger: Saturation to the maximum value");
            integerNumber = max;
        }
        else if (floatNumber <= static_cast<FloatType>(min)) {
            REPORT_ERROR(ErrorManagement::warning, "FloatToInteger: Saturation to the minimum value");
            integerNumber = min;
        }
        else {

            integerNumber = static_cast<IntegerType>(floatNumber);

            if ((floatNumber - static_cast<FloatType>(integerNumber)) >= static_cast<FloatType>(0.5)) {
                //approximation
                if (integerNumber < max) {
                    integerNumber++;
                }
            }
            else {
                if ((floatNumber - static_cast<FloatType>(integerNumber)) <= static_cast<FloatType>(-0.5)) {
                    if (integerNumber > min) {
                        integerNumber--;
                    }
                }
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::warning, "FloatToInteger: Assignment of negative float to unsigned integer type; saturation to 0");
    }

    return ret;
}

/*lint -e{740} -e{826} -e{927}  [MISRA C++ Rule 5-2-6], [MISRA C++ Rule 5-2-7]. Justification: Pointer to pointer cast requested from this implementation.*/
bool FloatToIntegerGeneric(const float32 * const source,
                           const uint32 sourceBitSize,
                           uint8 * const dest,
                           const uint32 destBitSize,
                           const bool isSigned) {

    bool ret = false;

    if (destBitSize <= 8u) {
        if (isSigned) {
            int8 tempDest = 0;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<const float64*>(source)), tempDest);
            }

            *(reinterpret_cast<int8*>(dest)) = tempDest;
        }
        else {
            uint8 tempDest = 0u;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<const float64*>(source)), tempDest);
            }

            *dest = tempDest;
        }

    }
    if ((destBitSize > 8u) && (destBitSize <= 16u)) {
        if (isSigned) {
            int16 tempDest = 0;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<const float64*>(source)), tempDest);
            }
            *(reinterpret_cast<int16*>(dest)) = tempDest;
        }
        else {
            uint16 tempDest = 0u;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<const float64*>(source)), tempDest);
            }
            *(reinterpret_cast<uint16*>(dest)) = tempDest;
        }
    }
    if ((destBitSize > 16u) && (destBitSize <= 32u)) {
        if (isSigned) {
            int32 tempDest = 0;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<const float64*>(source)), tempDest);
            }
            *(reinterpret_cast<int32*>(dest)) = tempDest;
        }
        else {
            uint32 tempDest = 0u;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<const float64*>(source)), tempDest);
            }
            *(reinterpret_cast<uint32*>(dest)) = tempDest;
        }
    }
    if ((destBitSize > 32u) && (destBitSize <= 64u)) {
        if (isSigned) {

            int64 tempDest = 0;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<const float64*>(source)), tempDest);
            }
            *(reinterpret_cast<int64*>(dest)) = tempDest;
        }
        else {

            uint64 tempDest = 0u;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<const float64*>(source)), tempDest);
            }
            *(reinterpret_cast<uint64*>(dest)) = tempDest;
        }
    }
    return ret;
}

}
