/**
 * @file IntegerToFloat.cpp
 * @brief Source file for class IntegerToFloat
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
 * the class IntegerToFloat (public, protected, and private). Be aware that some 
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
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename IntegerType, typename FloatType>
static bool IntegerToFloat(IntegerType source,
                           FloatType &dest) {

    bool ret = true;
    dest = static_cast<FloatType>(0.0);
    FloatType test = source / static_cast<FloatType>(1.0);

    if (isNaN(test) || isInf(test)) {
        //TODO overflow or underflow
        ret = false;
    }
    else {
        dest = test;
    }

    if (((source - dest) >= 1) || ((source - dest) <= -1)) {
        //TODO loss of precision
    }

    return ret;

}

bool IntegerToFloatGeneric(uint8 *source,
                           const uint8 sourceBitSize,
                           float32 *dest,
                           const uint8 destBitSize,
                           const bool isSigned) {

    bool ret = true;

    if (ret) {
        ret = false;
        if (sourceBitSize <= 8u) {
            if (isSigned) {
                int8 newSource = *(reinterpret_cast<int8*>(source));

                if (destBitSize == 32u) {
                    ret = IntegerToFloat(newSource, *dest);
                }
                if (destBitSize == 64u) {
                    ret = IntegerToFloat(newSource, *(reinterpret_cast<float64*>(dest)));
                }
            }
            else {
                uint8 newSource = *source;

                if (destBitSize == 32u) {
                    ret = IntegerToFloat(newSource, *dest);
                }
                if (destBitSize == 64u) {
                    ret = IntegerToFloat(newSource, *(reinterpret_cast<float64*>(dest)));
                }
            }
        }
        if ((sourceBitSize > 8u) && (sourceBitSize <= 16u)) {
            if (isSigned) {
                int16 newSource = *(reinterpret_cast<int16*>(source));
                if (destBitSize == 32u) {
                    ret = IntegerToFloat(newSource, *dest);
                }
                if (destBitSize == 64u) {
                    ret = IntegerToFloat(newSource, *(reinterpret_cast<float64*>(dest)));
                }
            }
            else {
                uint16 newSource = *(reinterpret_cast<uint16*>(source));
                if (destBitSize == 32u) {
                    ret = IntegerToFloat(newSource, *dest);
                }
                if (destBitSize == 64u) {
                    ret = IntegerToFloat(newSource, *(reinterpret_cast<float64*>(dest)));
                }
            }
        }
        if ((sourceBitSize > 16u) && (sourceBitSize <= 32u)) {
            if (isSigned) {
                int32 newSource = *(reinterpret_cast<int32*>(source));
                if (destBitSize == 32u) {
                    ret = IntegerToFloat(newSource, *dest);
                }
                if (destBitSize == 64u) {
                    ret = IntegerToFloat(newSource, *(reinterpret_cast<float64*>(dest)));
                }
            }
            else {
                uint32 newSource = *(reinterpret_cast<uint32*>(source));
                if (destBitSize == 32u) {
                    ret = IntegerToFloat(newSource, *dest);
                }
                if (destBitSize == 64u) {
                    ret = IntegerToFloat(newSource, *(reinterpret_cast<float64*>(dest)));
                }
            }
        }
        if ((sourceBitSize > 32u) && (sourceBitSize <= 64u)) {
            if (isSigned) {
                int64 newSource = *(reinterpret_cast<int64*>(source));
                if (destBitSize == 32u) {
                    ret = IntegerToFloat(newSource, *dest);
                }
                if (destBitSize == 64u) {
                    ret = IntegerToFloat(newSource, *(reinterpret_cast<float64*>(dest)));
                }
            }
            else {
                uint64 newSource = *(reinterpret_cast<uint64*>(source));
                if (destBitSize == 32u) {
                    ret = IntegerToFloat(newSource, *dest);
                }
                if (destBitSize == 64u) {
                    ret = IntegerToFloat(newSource, *(reinterpret_cast<float64*>(dest)));
                }
            }
        }
    }

    return ret;
}

}
