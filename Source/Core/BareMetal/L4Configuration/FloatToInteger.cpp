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

template<typename FloatType, typename IntegerType>
bool FloatToInteger(FloatType floatNumber,
                    IntegerType &integerNumber) {

    bool ret = true;

    integerNumber = static_cast<IntegerType>(0);

    bool isSigned = (static_cast<IntegerType>(-1) < static_cast<IntegerType>(0));

    if ((isSigned) || (floatNumber > static_cast<FloatType>(0.0))) {
        IntegerType max = ~static_cast<IntegerType>(0);
        IntegerType min = static_cast<IntegerType>(0);

        if (isSigned) {
            max = Shift::LogicalRightSafeShift(max, 1u);
            min = static_cast<IntegerType>(1) << (sizeof(IntegerType) * 8u - 1u);
        }

        if (floatNumber >= max) {
            //TODO Saturation.
            integerNumber = max;
        }
        else if (floatNumber <= min) {
            //TODO Saturation
            integerNumber = min;
        }
        else {

            integerNumber = static_cast<IntegerType>(floatNumber);

            if ((floatNumber - integerNumber) >= 0.5) {
                //approximation
                if (integerNumber < max) {
                    integerNumber++;
                }
            }
            else {
                if ((floatNumber - integerNumber) <= -0.5) {
                    if (integerNumber > min) {
                        integerNumber--;
                    }
                }
            }
        }
    }

    return ret;
}

bool FloatToIntegerGeneric(float32 *source,
                           const uint8 sourceBitSize,
                           uint8 *dest,
                           const uint8 destBitSize,
                           const bool isSigned) {

    bool ret = false;

    if (destBitSize <= 8u) {
        if (isSigned) {
            int8 tempDest;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<float64*>(source)), tempDest);
            }

            *(reinterpret_cast<int8*>(dest)) = tempDest;
        }
        else {
            uint8 tempDest;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<float64*>(source)), tempDest);
            }

            *dest = tempDest;
        }

    }
    if ((destBitSize > 8u) && (destBitSize <= 16u)) {
        if (isSigned) {
            int16 tempDest;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<float64*>(source)), tempDest);
            }
            *(reinterpret_cast<int16*>(dest)) = tempDest;
        }
        else {
            uint16 tempDest;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<float64*>(source)), tempDest);
            }
            *(reinterpret_cast<uint16*>(dest)) = tempDest;
        }
    }
    if ((destBitSize > 16u) && (destBitSize <= 32u)) {
        if (isSigned) {
            int32 tempDest;
            if (sourceBitSize == 32u) {
                if (*source > 0xffffffff) {
                }
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<float64*>(source)), tempDest);
            }
            *(reinterpret_cast<int32*>(dest)) = tempDest;
        }
        else {
            uint32 tempDest;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<float64*>(source)), tempDest);
            }
            *(reinterpret_cast<uint32*>(dest)) = tempDest;
        }
    }
    if ((destBitSize > 32u) && (destBitSize <= 64u)) {
        if (isSigned) {

            int64 tempDest;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<float64*>(source)), tempDest);
            }
            *(reinterpret_cast<int64*>(dest)) = tempDest;
        }
        else {

            uint64 tempDest;
            if (sourceBitSize == 32u) {
                ret = FloatToInteger(*source, tempDest);
            }
            if (sourceBitSize == 64u) {
                ret = FloatToInteger(*(reinterpret_cast<float64*>(source)), tempDest);
            }
            *(reinterpret_cast<uint64*>(dest)) = tempDest;
        }
    }
    return ret;
}

}
