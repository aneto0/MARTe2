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
#include "ConversionPrivate.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Converts an integer number to a float number.
 * @param[in] source is the integer number to be converted.
 * @param[out] dest is the converted float number in output.
 * @return false if the conversion fails, true otherwise.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
template<typename IntegerType, typename FloatType>
static bool IntegerToFloatT(const IntegerType source,
                           FloatType &dest) {

    bool ret = true;
    dest = static_cast<FloatType>(0.0);
    FloatType test = static_cast<FloatType>(source);

    bool isTestNan = (IsNaN(test));
    bool isTestInf = (IsInf(test));
    if ((isTestNan) || (isTestInf)) {
        REPORT_ERROR(ErrorManagement::FatalError, "IntegerToFloat: Conversion Failed, the result is NaN or Inf");
        ret = false;
    }
    else {
        dest = test;
    }

    return ret;

}

bool NumberToNumber(uint8  source,float  &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber(uint16 source,float  &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber(uint32 source,float  &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber(uint64 source,float  &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber(uint8  source,double &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber(uint16 source,double &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber(uint32 source,double &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber(uint64 source,double &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber( int8  source,float  &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber( int16 source,float  &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber( int32 source,float  &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber( int64 source,float  &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber( int8  source,double &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber( int16 source,double &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber( int32 source,double &dest){	return IntegerToFloatT(source,dest); }
bool NumberToNumber( int64 source,double &dest){	return IntegerToFloatT(source,dest); }



#if 0
/**
 * @brief Reinterprets the generic source and destination pointers in input recognizing the source integer type and the destination
 * float type by the bit size.
 * @param[in] source is a pointer to the integer number to be converted.
 * @param[in] sourceBitSize is the size in bit of the integer type in input.
 * @param[in,out] dest is a pointer to the float number in output.
 * @param[in] isSigned specifies if the integer type is signed.
 * @return true if the conversion succeeds, false otherwise.
 */
/*lint -e{740} -e{826} -e{927}  [MISRA C++ Rule 5-2-6], [MISRA C++ Rule 5-2-7]. Justification: Pointer to pointer cast requested from this implementation.*/
bool IntegerToFloatGeneric(const uint8 * const source,
                           const uint32 sourceBitSize,
                           float32 * const dest,
                           const uint32 destBitSize,
                           const bool isSigned) {

    bool ret = true;
    if (ret) {
        ret = false;
        if (sourceBitSize <= 8u) {
            if (isSigned) {
                int8 newSource = *(reinterpret_cast<const int8*>(source));

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
                int16 newSource = *(reinterpret_cast<const int16*>(source));
                if (destBitSize == 32u) {
                    ret = IntegerToFloat(newSource, *dest);
                }
                if (destBitSize == 64u) {
                    ret = IntegerToFloat(newSource, *(reinterpret_cast<float64*>(dest)));
                }
            }
            else {
                uint16 newSource = *(reinterpret_cast<const uint16*>(source));
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
                int32 newSource = *(reinterpret_cast<const int32*>(source));
                if (destBitSize == 32u) {
                    ret = IntegerToFloat(newSource, *dest);
                }
                if (destBitSize == 64u) {
                    ret = IntegerToFloat(newSource, *(reinterpret_cast<float64*>(dest)));
                }
            }
            else {
                uint32 newSource = *(reinterpret_cast<const uint32*>(source));
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
                int64 newSource = *(reinterpret_cast<const int64*>(source));
                if (destBitSize == 32u) {
                    ret = IntegerToFloat(newSource, *dest);
                }
                if (destBitSize == 64u) {
                    ret = IntegerToFloat(newSource, *(reinterpret_cast<float64*>(dest)));
                }
            }
            else {
                uint64 newSource = *(reinterpret_cast<const uint64*>(source));
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

#endif
}
