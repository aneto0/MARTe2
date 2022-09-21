/**
 * @file TypeConversion.cpp
 * @brief Source file for class TypeConversion
 * @date 08/11/2015
 * @author Andre' Neto
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
 * the class TypeConversion (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#define DLL_API

#include "AdvancedErrorManagement.h"
#include "AnyType.h"
#include "BitSetToInteger.h"
#include "ClassRegistryDatabase.h"
#include "Introspection.h"
#include "StreamString.h"
#include "StructuredDataI.h"
#include "TypeConversion.h"
#include "ValidateBasicType.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
extern bool StringToFloatGeneric(const char8 * const input, float32 * const number, const uint32 destBitSize);

/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
extern bool StringToIntegerGeneric(const char8* const source, uint8 * const dest, const uint32 destBitSize, const bool isSigned);

/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
extern bool IntegerToFloatGeneric(const uint8 * const source, const uint32 sourceBitSize, float32 * const dest, const uint32 destBitSize, const bool isSigned);

/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
extern bool FloatToIntegerGeneric(const float32 * const source, const uint32 sourceBitSize, uint8 * const dest, const uint32 destBitSize, const bool isSigned);

/**
 * @brief Saturate the number in input to the maximum or minimum 32-bit float number.
 * @param[in] isPositive specifies if the number must be set to the maximum 32-bit float (true) or to the minimum (false).
 * @param[in, out] number is the float number to be set.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
template<typename FloatType>
static bool MinMaxFloat(const bool isPositive, FloatType &number) {

    bool ret = false;
    if (isPositive) {
        if (sizeof(FloatType) == 4u) {
            REPORT_ERROR_STATIC(ErrorManagement::Warning, "FloatToFloat: Saturation to maximum float");
            uint32 maxFloat32Mask = 0x7f7fffffu;
            ret = MemoryOperationsHelper::Copy(&number, &maxFloat32Mask, 4u);
        }
    }
    else {
        if (sizeof(FloatType) == 4u) {
            REPORT_ERROR_STATIC(ErrorManagement::Warning, "FloatToFloat: Saturation to minimum float");
            uint32 minFloat32Mask = 0xff7fffffu;
            ret = MemoryOperationsHelper::Copy(&number, &minFloat32Mask, 4u);
        }
    }
    return ret;
}

/**
 * @brief Performs the conversion from a float number to another float number.
 * @param[in] source is the float number to be converted.
 * @param[out] destination is the converted float number in output.
 * @return true if the conversion succeeds, false otherwise.
 * @post
 *   If the size of destination type is minor than the source type one, and the conversion returns a nan or inf number,
 *   the output will be saturated at the maximum or minimum depending on the sign of the source number and a warning will
 *   be generated.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
template<typename FloatType1, typename FloatType2>
static bool FloatToFloat(const FloatType1 source, FloatType2 &destination) {

    destination = static_cast<FloatType2>(0.0);

    bool isSourceNan = IsNaN(source);
    bool isSourceInf = IsInf(source);
    bool ret = ((!isSourceNan) && (!isSourceInf));
    if (ret) {
        FloatType2 test = static_cast<FloatType2>(source);

        bool isTestNan = IsNaN(test);
        bool isTestInf = IsInf(test);
        if ((isTestNan) || (isTestInf)) {
            if (sizeof(FloatType2) < sizeof(FloatType1)) {
                ret = MinMaxFloat((source > static_cast<FloatType2>(0.0)), destination);
            }
            else {
                ret = false;
            }
        }
        else {

            destination = test;
        }
    }

    return ret;
}

/**
 * @brief Performs the conversion from integer to any type.
 * @param[out] destination is the any type in output.
 * @param[in] source is the any type representing an integer number.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetTypeDescriptor() == SignedInteger ||
 *   source.GetTypeDescriptor() == UnsignedInteger;
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool IntegerToType(const AnyType &destination, const AnyType &source) {

    void* destinationPointer = destination.GetDataPointer();
    const TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void* sourcePointer = source.GetDataPointer();
    const TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();

    bool ret = false;

    bool isSourceSignedInteger = (sourceDescriptor.type == SignedInteger);
    bool isSourceUnsignedInteger = (sourceDescriptor.type == UnsignedInteger);
    bool isSourcePointer = (sourceDescriptor.type == Pointer);

    if ((isSourceSignedInteger) || (isSourceUnsignedInteger) || (isSourcePointer)) {

        const char8 *format = (isSourcePointer) ? ("%p") : ("%d");

        if (destinationDescriptor.type == SString) {
            StreamString tempString;
            ret = tempString.PrintFormatted(format, &source);
            if (ret) {
                uint32 stringLength = static_cast<uint32>(tempString.Size());
                ret = (reinterpret_cast<StreamString*>(destinationPointer))->Write(tempString.Buffer(), stringLength);
            }
        }
        else if (destinationDescriptor.type == CArray) {
            StreamString tempString;
            ret = tempString.PrintFormatted(format, &source);
            if (ret) {
                uint32 stringLength = static_cast<uint32>(tempString.Size());
                uint32 arraySize = (destination.GetByteSize() * destination.GetNumberOfElements(0u));
                if (stringLength >= arraySize) {
                    REPORT_ERROR_STATIC(ErrorManagement::Warning, "IntegerToType: The input is too long for the output buffer.");
                    ret = StringHelper::CopyN(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer(), arraySize);
                    if (arraySize > 1u) {
                        uint32 lastCharIndex = arraySize - 1u;
                        reinterpret_cast<char8 *>(destinationPointer)[lastCharIndex] = '\0';
                    }
                }
                else {
                    ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer());
                }
            }
        }
        else if (destinationDescriptor.type == BT_CCString) {
            StreamString tempString;
            ret = tempString.PrintFormatted(format, &source);
            if (ret) {
                ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer());
            }
        }
        //lint -e{9007} no read side-effects on the type comparisons below*/
        else if (destinationDescriptor.type == BT_Boolean) {
            bool* destinationBool = reinterpret_cast<bool*>(destinationPointer);
            if ((sourceDescriptor == UnsignedInteger8Bit) || (sourceDescriptor == SignedInteger8Bit)) {
                uint8* sourceInput = reinterpret_cast<uint8 *>(sourcePointer);
                *destinationBool = (*sourceInput == 1u);
            }
            else if ((sourceDescriptor == UnsignedInteger16Bit) || (sourceDescriptor == SignedInteger16Bit)) {
                uint16* sourceInput = reinterpret_cast<uint16 *>(sourcePointer);
                *destinationBool = (*sourceInput == 1u);
            }
            else if ((sourceDescriptor == UnsignedInteger32Bit) || (sourceDescriptor == SignedInteger32Bit)) {
                uint32* sourceInput = reinterpret_cast<uint32 *>(sourcePointer);
                *destinationBool = (*sourceInput == 1u);
            }
            else if ((sourceDescriptor == UnsignedInteger64Bit) || (sourceDescriptor == SignedInteger64Bit)) {
                uint64* sourceInput = reinterpret_cast<uint64 *>(sourcePointer);
                *destinationBool = (*sourceInput == 1u);
            }
            else {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Unsupported bit size conversion from integer to boolean");
            }
            ret = true;
        }
        else if (destinationDescriptor.type == SignedInteger) {

            uint8* destinationInput = reinterpret_cast<uint8*>(destinationPointer);
            uint8* sourceInput = (sourceDescriptor.type == Pointer) ? (reinterpret_cast<uint8*>(&sourcePointer)) : (reinterpret_cast<uint8*>(sourcePointer));
            uint8 destShift = static_cast<uint8>(destination.GetBitAddress());
            uint8 sourceShift = static_cast<uint8>(source.GetBitAddress());
            if ((sourceDescriptor.type == SignedInteger)) {
                ret = BitSetToBitSet(destinationInput, destShift, static_cast<uint8>(destinationDescriptor.numberOfBits), true, sourceInput, sourceShift,
                                     static_cast<uint8>(sourceDescriptor.numberOfBits), true);
            }
            if (sourceDescriptor.type == UnsignedInteger) {
                ret = BitSetToBitSet(destinationInput, destShift, static_cast<uint8>(destinationDescriptor.numberOfBits), true, sourceInput, sourceShift,
                                     static_cast<uint8>(sourceDescriptor.numberOfBits), false);
            }
            if (sourceDescriptor.type == Pointer) {
                ret = BitSetToBitSet(destinationInput, destShift, static_cast<uint8>(destinationDescriptor.numberOfBits), true, sourceInput, sourceShift,
                                     static_cast<uint8>(sourceDescriptor.numberOfBits), false);
            }
        }
        else if (destinationDescriptor.type == UnsignedInteger) {
            uint8* destinationInput = reinterpret_cast<uint8*>(destinationPointer);
            uint8* sourceInput = (sourceDescriptor.type == Pointer) ? (reinterpret_cast<uint8*>(&sourcePointer)) : (reinterpret_cast<uint8*>(sourcePointer));
            uint8 destShift = static_cast<uint8>(destination.GetBitAddress());
            uint8 sourceShift = static_cast<uint8>(source.GetBitAddress());
            if ((sourceDescriptor.type == SignedInteger)) {
                ret = BitSetToBitSet(destinationInput, destShift, static_cast<uint8>(destinationDescriptor.numberOfBits), false, sourceInput, sourceShift,
                                     static_cast<uint8>(sourceDescriptor.numberOfBits), true);
            }
            else if (sourceDescriptor.type == UnsignedInteger) {
                ret = BitSetToBitSet(destinationInput, destShift, static_cast<uint8>(destinationDescriptor.numberOfBits), false, sourceInput, sourceShift,
                                     static_cast<uint8>(sourceDescriptor.numberOfBits), false);
            }
            else if (sourceDescriptor.type == Pointer) {
                ret = BitSetToBitSet(destinationInput, destShift, static_cast<uint8>(destinationDescriptor.numberOfBits), false, sourceInput, sourceShift,
                                     static_cast<uint8>(sourceDescriptor.numberOfBits), false);
            }
            else {//NOOP
            }
        }
        else if (destinationDescriptor.type == Float) {
            uint8* sourceInput = (sourceDescriptor.type == Pointer) ? (reinterpret_cast<uint8*>(&sourcePointer)) : (reinterpret_cast<uint8*>(sourcePointer));
            float32* destinationInput = reinterpret_cast<float32*>(destinationPointer);
            if ((sourceDescriptor.type == SignedInteger)) {
                ret = IntegerToFloatGeneric(sourceInput, source.GetBitSize(), destinationInput, destination.GetBitSize(), true);
            }
            else if (sourceDescriptor.type == UnsignedInteger) {
                ret = IntegerToFloatGeneric(sourceInput, source.GetBitSize(), destinationInput, destination.GetBitSize(), false);
            }
            else if (sourceDescriptor.type == Pointer) {
                ret = IntegerToFloatGeneric(sourceInput, source.GetBitSize(), destinationInput, destination.GetBitSize(), false);
            }
            else {//NOOP
            }
        }
        else {//NOOP
        }
    }
    return ret;
}

/**
 * @brief Performs the conversion from float to any type.
 * @param[out] destination is the any type in output.
 * @param[in] source is the any type representing a float number.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetTypeDescriptor() == Float;
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool FloatToType(const AnyType &destination, const AnyType &source) {

    void* destinationPointer = destination.GetDataPointer();
    const TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void* sourcePointer = source.GetDataPointer();
    const TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();

    bool ret = false;

    if (sourceDescriptor.type == Float) {

        if (destinationDescriptor.type == SString) {
            StreamString tempString;
            ret = tempString.PrintFormatted("%E", &source);
            if (ret) {
                uint32 stringLength = static_cast<uint32>(tempString.Size());
                ret = (reinterpret_cast<StreamString*>(destinationPointer))->Write(tempString.Buffer(), stringLength);
            }
        }
        else if (destinationDescriptor.type == CArray) {
            StreamString tempString;
            ret = tempString.PrintFormatted("%E", &source);
            if (ret) {
                uint32 stringLength = static_cast<uint32>(tempString.Size());
                uint32 arraySize = (destination.GetByteSize() * destination.GetNumberOfElements(0u));
                if (stringLength >= arraySize) {
                    REPORT_ERROR_STATIC(ErrorManagement::Warning, "FloatToType: The input is too long for the output buffer.");
                    ret = StringHelper::CopyN(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer(), arraySize);
                    if (arraySize > 1u) {
                        uint32 lastCharIndex = arraySize - 1u;
                        reinterpret_cast<char8 *>(destinationPointer)[lastCharIndex] = '\0';
                    }
                }
                else {
                    ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer());
                }
            }
        }
        else if (destinationDescriptor.type == BT_CCString) {
            StreamString tempString;
            ret = tempString.PrintFormatted("%E", &source);
            if (ret) {
                ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer());
            }
        }
        else if (destinationDescriptor.type == SignedInteger) {
            ret = FloatToIntegerGeneric(reinterpret_cast<float32*>(sourcePointer), static_cast<uint8>(sourceDescriptor.numberOfBits), reinterpret_cast<uint8*>(destinationPointer),
                                        destination.GetBitSize(), true);
        }
        else if (destinationDescriptor.type == UnsignedInteger) {
            ret = FloatToIntegerGeneric(reinterpret_cast<float32*>(sourcePointer), static_cast<uint8>(sourceDescriptor.numberOfBits), reinterpret_cast<uint8*>(destinationPointer),
                                        destination.GetBitSize(), false);
        }
        else if (destinationDescriptor.type == Float) {
            if (destination.GetBitSize() == 32u) {
                if (source.GetBitSize() == 64u) {
                    ret = FloatToFloat(*(reinterpret_cast<float64*>(sourcePointer)), *(reinterpret_cast<float32*>(destinationPointer)));
                }
            }
            if (destination.GetBitSize() == 64u) {
                if (source.GetBitSize() == 32u) {
                    ret = FloatToFloat(*(reinterpret_cast<float32*>(sourcePointer)), *(reinterpret_cast<float64*>(destinationPointer)));
                }
            }
        }
        else if (destinationDescriptor.type == BT_Boolean) {
            *(reinterpret_cast<bool*>(destinationPointer)) = false;
            ret = false;
        }
        else {
            //NOOP
        }
    }
    return ret;
}
/**
 * @brief Performs the conversion from boolean to any type.
 * @param[out] destination is the any type in output.
 * @param[in] source is the any type representing a boolean.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetTypeDescriptor() == BT_Boolean;
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool BooleanToType(const AnyType &destination, const AnyType &source) {

    void* destinationPointer = destination.GetDataPointer();
    const TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void* sourcePointer = source.GetDataPointer();
    const TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();

    bool ret = false;
    if (sourceDescriptor.type == BT_Boolean) {
        bool sourceVal = *static_cast<bool *>(sourcePointer);
        if (destinationDescriptor.type == SString) {
            StreamString tempString;
            if (sourceVal) {
                tempString = "true";
            }
            else {
                tempString = "false";
            }
            uint32 stringLength = static_cast<uint32>(tempString.Size());
            ret = (reinterpret_cast<StreamString*>(destinationPointer))->Write(tempString.Buffer(), stringLength);
        }
        if (destinationDescriptor.type == CArray) {
            StreamString tempString;
            if (sourceVal) {
                tempString = "true";
            }
            else {
                tempString = "false";
            }
            uint32 stringLength = static_cast<uint32>(tempString.Size());
            uint32 arraySize = (destination.GetByteSize() * destination.GetNumberOfElements(0u));
            if (stringLength >= arraySize) {
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "BooleanToType: The input is too long for the output buffer.");
                ret = StringHelper::CopyN(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer(), arraySize);
                if (arraySize > 1u) {
                    uint32 lastCharIndex = arraySize - 1u;
                    reinterpret_cast<char8 *>(destinationPointer)[lastCharIndex] = '\0';
                }
            }
            else {
                ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer());
            }
        }
        if (destinationDescriptor.type == BT_CCString) {
            StreamString tempString;
            if (sourceVal) {
                tempString = "true";
            }
            else {
                tempString = "false";
            }
            ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer());
        }
        if (destinationDescriptor.type == SignedInteger) {
            uint32 byteSize = destination.GetBitSize();
            byteSize /= 8u;
            ret = MemoryOperationsHelper::Set(destinationPointer, '\0', byteSize);
            uint8 *destAsUInt8 = reinterpret_cast<uint8 *>(destinationPointer);
            *destAsUInt8 = (sourceVal ? 1u : 0u);
        }
        if (destinationDescriptor.type == UnsignedInteger) {
            uint32 byteSize = destination.GetBitSize();
            byteSize /= 8u;
            ret = MemoryOperationsHelper::Set(destinationPointer, '\0', byteSize);
            int8 *destAsInt8 = reinterpret_cast<int8 *>(destinationPointer);
            *destAsInt8 = (sourceVal ? 1 : 0);
        }
        if (destinationDescriptor.type == Float) {
            ret = false;
            REPORT_ERROR_STATIC(ErrorManagement::Warning, "BooleanToType: float to boolean conversion not support.");
        }
    }
    return ret;
}

/**
 * @brief Performs the conversion from string types (CCString, StreamString, CArray) to any type.
 * @param[out] destination is the any type in output.
 * @param[in] source is the any type representing a string.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetTypeDescriptor() == StreamString ||
 *   source.GetTypeDescriptor() == CCString ||
 *   source.GetTypeDescriptor() == CArray;
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool StringToType(const AnyType &destination, const AnyType &source) {

    void* destinationPointer = destination.GetDataPointer();
    const TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void* sourcePointer = source.GetDataPointer();
    const TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();

    bool ret = false;

    const char8* token = static_cast<const char8 *>(NULL);
    if (sourceDescriptor.type == BT_CCString) {
        token = reinterpret_cast<const char8*>(sourcePointer);
    }
    if (sourceDescriptor.type == CArray) {
        token = reinterpret_cast<const char8*>(sourcePointer);
    }
    if ((sourceDescriptor.type == SString)) {
        token = (reinterpret_cast<StreamString*>(sourcePointer))->Buffer();
    }

    if (token != NULL) {
        uint32 tokenLength = StringHelper::Length(token);
        if (destinationDescriptor.type == SString) {
            StreamString* tempString = reinterpret_cast<StreamString*>(destinationPointer);
            ret = tempString->Write(token, tokenLength);
        }
        else if (destinationDescriptor.type == CArray) {
            uint32 arraySize = destination.GetByteSize() * destination.GetNumberOfElements(0u);
            // case char
            if (tokenLength >= arraySize) {
                REPORT_ERROR_STATIC(ErrorManagement::Warning, "StringToType: The input is too long for the output buffer.");
                ret = StringHelper::CopyN(reinterpret_cast<char8 *>(destinationPointer), token, arraySize);
                if (arraySize > 1u) {
                    uint32 lastCharIndex = arraySize - 1u;
                    reinterpret_cast<char8 *>(destinationPointer)[lastCharIndex] = '\0';
                }
            }
            else {
                ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), token);
            }
        }
        else if (destinationDescriptor.type == BT_CCString) {
            ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), token);
        }
        else if (destinationDescriptor.type == SignedInteger) {
            ret = StringToIntegerGeneric(token, reinterpret_cast<uint8*>(destinationPointer), destination.GetBitSize(), true);
        }
        else if (destinationDescriptor.type == UnsignedInteger) {
            ret = StringToIntegerGeneric(token, reinterpret_cast<uint8*>(destinationPointer), destination.GetBitSize(), false);
        }
        else if (destinationDescriptor.type == BT_Boolean) {
            bool boolValue = false;
            const char8 *trueValues[] = {"true", "1", "yes", NULL_PTR(const char8 *)};
            const uint32 lenTrueValues[] = {4u, 1u, 3u, 0u};
            uint32 c = 0u;
            while((trueValues[c] != NULL_PTR(const char8 *)) && (!boolValue)) {
                uint32 lenToken = StringHelper::Length(token);
                boolValue = (lenToken == lenTrueValues[c]);
                if (boolValue) {
                    boolValue = (StringHelper::CompareNoCaseSensN(token, trueValues[c], lenTrueValues[c]) == 0);
                }
                c++;
            }
            bool *destinationPointerBool = reinterpret_cast<bool *>(destinationPointer);
            *destinationPointerBool = boolValue;
            ret = true;
        }
        else if (destinationDescriptor.type == Float) {
            ret = StringToFloatGeneric(token, (reinterpret_cast<float32*>(destinationPointer)), destination.GetBitSize());
        }
        else {//NOOP
        }
    }
    return ret;
}

/**
 * @brief Performs the conversion between two compatible objects.
 * @param[out] destination is the output.
 * @param[in] source is the input.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   The objects represented by \a source and \a destination must be introspectable and registered into ClassRegistryDatabase.
 */
static bool ObjectToObject(const AnyType &destination, const AnyType &source) {

    bool ret = false;
    const TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();
    const TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    const ClassRegistryItem *sourceItem = ClassRegistryDatabase::Instance()->Peek(sourceDescriptor.structuredDataIdCode);
    const ClassRegistryItem *destinationItem = ClassRegistryDatabase::Instance()->Peek(destinationDescriptor.structuredDataIdCode);
    if ((sourceItem != NULL) && (destinationItem != NULL)) {
        const Introspection *sourceIntrospection = sourceItem->GetIntrospection();
        const Introspection *destinationIntrospection = destinationItem->GetIntrospection();
        if ((sourceIntrospection != NULL) && (destinationIntrospection != NULL)) {
            if (sourceIntrospection->GetNumberOfMembers() == destinationIntrospection->GetNumberOfMembers()) {
                uint32 numberOfMembers = sourceIntrospection->GetNumberOfMembers();
                ret = true;
                for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
                    IntrospectionEntry sourceMemberIntrospection = (*sourceIntrospection)[i];
                    IntrospectionEntry destinationMemberIntrospection = (*destinationIntrospection)[i];

                    TypeDescriptor sourceMemberDescriptor = sourceMemberIntrospection.GetMemberTypeDescriptor();
                    TypeDescriptor destinationMemberDescriptor = destinationMemberIntrospection.GetMemberTypeDescriptor();

                    TypeDescriptor newSourceDescriptor = sourceMemberDescriptor;
                    // source is a pointer!
                    if (sourceMemberIntrospection.GetMemberPointerLevel() > 0u) {
                        newSourceDescriptor = TypeDescriptor(false, UnsignedInteger, static_cast<uint8>(sizeof(void*)) * 8u);
                    }

                    TypeDescriptor newDestinationDescriptor = destinationMemberDescriptor;
                    // destination is a pointer!
                    if (destinationMemberIntrospection.GetMemberPointerLevel() > 0u) {
                        newDestinationDescriptor = TypeDescriptor(false, UnsignedInteger, static_cast<uint8>(sizeof(void*)) * 8u);
                    }

                    char8* sourceMemberDataPointer = &(static_cast<char8*>(source.GetDataPointer())[sourceMemberIntrospection.GetMemberByteOffset()]);

                    AnyType newSource(newSourceDescriptor, 0u, sourceMemberDataPointer);
                    // special case char* string because is a pointer
                    if (newSourceDescriptor == CharString) {
                        if (sourceMemberIntrospection.GetNumberOfDimensions() == 0u) {
                            newSource = AnyType(*reinterpret_cast<char8**>(sourceMemberDataPointer));
                        }
                    }

                    char8* destinationMemberDataPointer = &(static_cast<char8*>(destination.GetDataPointer())[destinationMemberIntrospection.GetMemberByteOffset()]);
                    AnyType newDestination(newDestinationDescriptor, 0u, destinationMemberDataPointer);

                    // special case char* string because is a pointer
                    if (newDestinationDescriptor == CharString) {
                        if (destinationMemberIntrospection.GetNumberOfDimensions() == 0u) {
                            newDestination = AnyType(*reinterpret_cast<char8**>(destinationMemberDataPointer));
                        }
                    }

                    for (uint32 j = 0u; j < 3u; j++) {
                        newSource.SetNumberOfElements(j, sourceMemberIntrospection.GetNumberOfElements(j));
                        newDestination.SetNumberOfElements(j, destinationMemberIntrospection.GetNumberOfElements(j));
                    }
                    newSource.SetNumberOfDimensions(sourceMemberIntrospection.GetNumberOfDimensions());
                    newDestination.SetNumberOfDimensions(destinationMemberIntrospection.GetNumberOfDimensions());
                    // call the conversion recursively !
                    ret = TypeConvert(newDestination, newSource);
                    if (ret) {
                        // validate the output
                        ret = ValidateBasicType(newDestination, destinationMemberIntrospection.GetMemberAttributes());
                    }
                }
            }
            else {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "ObjectToObject: The classes does not have the same number of members");
            }
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "ObjectToObject: Introspection not found for the specified classes");
        }
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "ObjectToObject: Class not registered");
    }
    return ret;
}

/**
 * @Recursive support function for the StructuredDataToObject for the serialisation of arrays of structures
 * @param[in] destinationMemberIntrospection IntrospectionEntry of the member representing the structure.
 * @param[in] memberName the name of the member to which the format [][][] will be appended
 * @param[in] sourcePointer StructuredDataI from where the array of structure is read from
 * @param[in] source the original AnyType for the TypeConvert
 * @param[in] newDestinationDescriptor the structure TypeDescriptor
 * @param[in] destinationMemberDataPointer  the pointer to the source of the array of structures i.e. address of [0]...[0] 
 * @param[in] dim the number of dimensions
 * @param[out] offset the current offset with respect to the destinationMemberDataPointer
 * @param[in] memberSize the size of the structure
 * @return true if the conversion succeeds, false otherwise.
 */
static bool RecurseStructureToObject(IntrospectionEntry &destinationMemberIntrospection, const char8 * const memberName, StructuredDataI * const sourcePointer, const AnyType &source, TypeDescriptor &newDestinationDescriptor, char8 * const destinationMemberDataPointer, const uint32 dim, const uint32 nOfDimensions, uint32 &offset, const uint32 memberSize) {
    uint32 nOfElements = destinationMemberIntrospection.GetNumberOfElements(dim);
    bool ret = (sourcePointer != NULL_PTR(StructuredDataI *));
    /*lint -e{850} n is not modified by the Printf*/
    for (uint32 n=0u; (n<nOfElements) && (ret); n++) {
        StreamString fullMemberNameIdx = memberName;
        (void)fullMemberNameIdx.Printf("[%d]", n);
        if (dim == (nOfDimensions - 1u)) {
            ret = sourcePointer->MoveRelative(fullMemberNameIdx.Buffer());
            if (ret) {
                uint32 ptrOffset = (offset * memberSize);
                void *fullSourceMemberDataPointer = &destinationMemberDataPointer[ptrOffset];
                offset++;
                AnyType newDestination(newDestinationDescriptor, 0u, fullSourceMemberDataPointer);
                ret = TypeConvert(newDestination, source);
                if (!sourcePointer->MoveToAncestor(1u)) {
                    ret = false;
                }
            }
        }
        else {
            ret = RecurseStructureToObject(destinationMemberIntrospection, fullMemberNameIdx.Buffer(), sourcePointer, source, newDestinationDescriptor, destinationMemberDataPointer, dim + 1u, nOfDimensions, offset, memberSize);
        }
    }
    return ret;
}

/**
 * @brief Performs the conversion from a StructuredDataI to a compatible object.
 * @details If the structure represented by the StructuredDataI node is introspectable its members
 * will be searched by name, otherwise they will be considered as they are sorted inside the node.
 * @param[out] destination is the output.
 * @param[in] source is the input.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   At least the object represented by \a destination must be introspectable.
 */
static bool StructuredDataToObject(const AnyType &destination, const AnyType &source) {

    bool ret = false;
    StructuredDataI* sourcePointer = reinterpret_cast<StructuredDataI*>(source.GetDataPointer());
    const TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    StreamString sourceStructName;
    bool isSourceIntrospection = true;
    const ClassRegistryItem *sourceItem = NULL_PTR(const ClassRegistryItem *);
    uint32 beginIndex = 0u;
    // if the Class field does not exists, the introspection for the source does not exists
    if (!sourcePointer->Read("Class", sourceStructName)) {
        isSourceIntrospection = false;
    }
    else {
        // Assume that Class is the first field
        beginIndex = 1u;
        sourceItem = ClassRegistryDatabase::Instance()->Find(sourceStructName.Buffer());
        isSourceIntrospection = (sourceItem != NULL);
    }
    const ClassRegistryItem *destinationItem = ClassRegistryDatabase::Instance()->Peek(destinationDescriptor.structuredDataIdCode);

    if (destinationItem != NULL) {
        const Introspection *sourceIntrospection = NULL_PTR(const Introspection *);
        if (isSourceIntrospection) {
            if (sourceItem != NULL) {
                sourceIntrospection = sourceItem->GetIntrospection();
            }
            isSourceIntrospection = (sourceIntrospection != NULL);
        }
        const Introspection *destinationIntrospection = destinationItem->GetIntrospection();
        if (destinationIntrospection != NULL) {
            if (isSourceIntrospection) {
                // if there is the introspection get the number of members
                beginIndex = 0u;
            }
            uint32 numberOfMembers = destinationIntrospection->GetNumberOfMembers();
            uint32 cdbIdx = 0u;
            //No longer checked since with structured arrays, the number of elements in the CDB != number of elements in the struct (as the struct as the array represented in a compact format)
            //if (numberOfMembers == destinationIntrospection->GetNumberOfMembers()) {
            ret = true;
            for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
                IntrospectionEntry destinationMemberIntrospection = (*destinationIntrospection)[i];

                TypeDescriptor destinationMemberDescriptor = destinationMemberIntrospection.GetMemberTypeDescriptor();

                TypeDescriptor newDestinationDescriptor = destinationMemberDescriptor;
                // destination is a pointer!
                if (destinationMemberIntrospection.GetMemberPointerLevel() > 0u) {
                    newDestinationDescriptor = TypeDescriptor(false, UnsignedInteger, static_cast<uint8>(sizeof(void*)) * 8u);
                }

                char8* destinationMemberDataPointer = &(static_cast<char8*>(destination.GetDataPointer())[destinationMemberIntrospection.GetMemberByteOffset()]);
                AnyType newDestination(newDestinationDescriptor, 0u, destinationMemberDataPointer);

                // special case char* string because is a pointer
                if (newDestinationDescriptor == CharString) {
                    if (destinationMemberIntrospection.GetNumberOfDimensions() == 0u) {
                        newDestination = AnyType(*reinterpret_cast<char8**>(destinationMemberDataPointer));
                    }
                }

                for (uint32 j = 0u; j < 3u; j++) {
                    newDestination.SetNumberOfElements(j, destinationMemberIntrospection.GetNumberOfElements(j));
                }
                newDestination.SetNumberOfDimensions(destinationMemberIntrospection.GetNumberOfDimensions());

                AnyType newSource;
                const char8* childName = NULL_PTR(const char8*);
                // get the member AnyType from the database if the member is a basic type!
                if (isSourceIntrospection) {
                    // if the introspection for the source class exists find the member name
                    /*lint -e{613} isSourceIntrospection => sourceIntrospection != NULL*/
                    IntrospectionEntry sourceMemberIntrospection = (*sourceIntrospection)[i];
                    childName = sourceMemberIntrospection.GetMemberName();
                    newSource = sourcePointer->GetType(childName);
                }
                else {
                    childName = sourcePointer->GetChildName(cdbIdx + beginIndex);
                    newSource = sourcePointer->GetType(childName);
                }

                if (newSource.GetDataPointer() == NULL) {
                    // could be a structured node!
                    uint32 nOfDimensions = destinationMemberIntrospection.GetNumberOfDimensions();
                    if (nOfDimensions == 1u) {
                        if (destinationMemberIntrospection.GetNumberOfElements(0u) == 1u) {
                            nOfDimensions = 0u;
                        }
                    }
                    if (nOfDimensions > 0u) {
                        uint32 offset = 0u;
                        ret = RecurseStructureToObject(destinationMemberIntrospection, destinationMemberIntrospection.GetMemberName(), sourcePointer, source, newDestinationDescriptor, destinationMemberDataPointer, 0u, nOfDimensions, offset, destinationMemberIntrospection.GetMemberSize());
                        cdbIdx += offset;
                        if (!ret) {
                            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "RecurseStructureToObject failed for member %s", destinationMemberIntrospection.GetMemberName());
                        }
                    }
                    else {
                        cdbIdx++;
                        ret = sourcePointer->MoveRelative(childName);
                        // call the conversion recursively !
                        if (ret) {
                            ret = TypeConvert(newDestination, source);
                            if (!ret) {
                                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "TypeConvert failed for member %s", destinationMemberIntrospection.GetMemberName());
                            }
                        }

                        if (!sourcePointer->MoveToAncestor(1u)) {
                            ret = false;
                        }
                    }
                }
                // could be a leaf!
                else {
                    cdbIdx++;
                    // call the conversion recursively !
                    ret = TypeConvert(newDestination, newSource);
                    if (ret) {
                        // validate the output
                        ret = ValidateBasicType(newDestination, destinationMemberIntrospection.GetMemberAttributes());
                        if (!ret) {
                            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "ValidateBasicType failed for member %s", destinationMemberIntrospection.GetMemberName());
                        }
                    }
                    else {
                        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "TypeConvert (leaf) failed for member %s", destinationMemberIntrospection.GetMemberName());
                    }
                }
            }
            //No longer verified as it would invalidate the way arrays of structures are represented
            /*}
            else {
                REPORT_ERROR_STATIC(ErrorManagement::FatalError, "StructuredDataToObject: The classes do not have the same number of members");
            }*/
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "StructuredDataToObject: Introspection not found for the specified classes");
        }
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "StructuredDataToObject: Class not registered");
    }

    return ret;
}

/**
 * @Recursive support function for the ObjectToStructuredData for the serialisation of arrays of structures
 * @param[in] sourceMemberIntrospection IntrospectionEntry of the member representing the structure.
 * @param[in] memberName the name of the member to which the format [][][] will be appended
 * @param[in] destinationPointer StructuredDataI to serialise the array of structure
 * @param[in] destination the original AnyType for the TypeConvert
 * @param[in] newSourceDescriptor the structure TypeDescriptor
 * @param[in] sourceMemberDataPointer the pointer to the source of the array of structures i.e. address of [0]...[0] 
 * @param[in] dim the number of dimensions
 * @param[out] offset the current offset with respect to the sourceMemberDataPointer
 * @param[in] memberSize the size of the structure
 * @return true if the conversion succeeds, false otherwise.
 */
static bool RecurseStructureToStructuredData(IntrospectionEntry &sourceMemberIntrospection, const char8 * const memberName, StructuredDataI * const destinationPointer, const AnyType &destination, TypeDescriptor &newSourceDescriptor, char8 * const sourceMemberDataPointer, const uint32 dim, const uint32 nOfDimensions, uint32 &offset, const uint32 memberSize) {
    uint32 nOfElements = sourceMemberIntrospection.GetNumberOfElements(dim);
    bool ret = (destinationPointer != NULL_PTR(StructuredDataI *));
    /*lint -e{850} n is not modified by the Printf*/
    for (uint32 n=0u; (n<nOfElements) && (ret); n++) {
        StreamString fullMemberNameIdx = memberName;
        (void)fullMemberNameIdx.Printf("[%d]", n);
        if (dim == (nOfDimensions - 1u)) {
            ret = destinationPointer->CreateRelative(fullMemberNameIdx.Buffer());
            if (ret) {
                uint32 ptrOffset = (offset * memberSize);
                void *fullSourceMemberDataPointer = &sourceMemberDataPointer[ptrOffset];
                offset++;
                AnyType newSource(newSourceDescriptor, 0u, fullSourceMemberDataPointer);
                ret = TypeConvert(destination, newSource);
                if (!destinationPointer->MoveToAncestor(1u)) {
                    ret = false;
                }
            }
        }
        else {
            ret = RecurseStructureToStructuredData(sourceMemberIntrospection, fullMemberNameIdx.Buffer(), destinationPointer, destination, newSourceDescriptor, sourceMemberDataPointer, dim + 1u, nOfDimensions, offset, memberSize);
        }
    }
    return ret;
}


/**
 * @Performs the conversion from an object to a StructuredDataI node.
 * @param[out] destination is the output.
 * @param[in] source is the input.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   The object represented by source must be introspectable.
 */
static bool ObjectToStructuredData(const AnyType &destination, const AnyType &source) {

    bool ret = false;
    const TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();
    const ClassRegistryItem *sourceItem = ClassRegistryDatabase::Instance()->Peek(sourceDescriptor.structuredDataIdCode);

    StructuredDataI* destinationPointer = reinterpret_cast<StructuredDataI*>(destination.GetDataPointer());

    if (sourceItem != NULL) {
        const Introspection *sourceIntrospection = sourceItem->GetIntrospection();
        if (sourceIntrospection != NULL) {
            if (destinationPointer->Write("Class", sourceItem->GetClassProperties()->GetName())) {
                uint32 numberOfMembers = sourceIntrospection->GetNumberOfMembers();
                ret = true;
                for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
                    IntrospectionEntry sourceMemberIntrospection = (*sourceIntrospection)[i];

                    TypeDescriptor sourceMemberDescriptor = sourceMemberIntrospection.GetMemberTypeDescriptor();

                    TypeDescriptor newSourceDescriptor = sourceMemberDescriptor;
                    // source is a pointer!
                    if (sourceMemberIntrospection.GetMemberPointerLevel() > 0u) {
                        newSourceDescriptor = TypeDescriptor(false, UnsignedInteger, static_cast<uint8>(sizeof(void*)) * 8u);
                    }

                    char8* sourceMemberDataPointer = &(static_cast<char8*>(source.GetDataPointer())[sourceMemberIntrospection.GetMemberByteOffset()]);
                    AnyType newSource(newSourceDescriptor, 0u, sourceMemberDataPointer);
                    // special case char* string because is a pointer
                    if (newSourceDescriptor == CharString) {
                        if (sourceMemberIntrospection.GetNumberOfDimensions() == 0u) {
                            newSource = AnyType(*reinterpret_cast<char8**>(sourceMemberDataPointer));
                        }
                    }
                    for (uint32 j = 0u; j < 3u; j++) {
                        newSource.SetNumberOfElements(j, sourceMemberIntrospection.GetNumberOfElements(j));
                    }
                    newSource.SetNumberOfDimensions(sourceMemberIntrospection.GetNumberOfDimensions());

                    bool isNewSourceStructured = newSourceDescriptor.isStructuredData;
                    if (isNewSourceStructured) {
                        uint32 nOfDimensions = sourceMemberIntrospection.GetNumberOfDimensions();
                        if (nOfDimensions == 1u) {
                            if (sourceMemberIntrospection.GetNumberOfElements(0u) == 1u) {
                                nOfDimensions = 0u;
                            }
                        }
                        if (nOfDimensions > 0u) {
                            uint32 offset = 0u;
                            ret = RecurseStructureToStructuredData(sourceMemberIntrospection, sourceMemberIntrospection.GetMemberName(), destinationPointer, destination, newSourceDescriptor, sourceMemberDataPointer, 0u, nOfDimensions, offset, sourceMemberIntrospection.GetMemberSize());
                        }
                        else {
                            // structured data again! Create a node and go recursively
                            ret = destinationPointer->CreateRelative(sourceMemberIntrospection.GetMemberName());
                            if (ret) {
                                ret = TypeConvert(destination, newSource);
                                if (!destinationPointer->MoveToAncestor(1u)) {
                                    ret = false;
                                }
                            }
                        }
                    }
                    else {
                        // in this case only write
                        ret = destinationPointer->Write(sourceMemberIntrospection.GetMemberName(), newSource);
                    }
                }
            }
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "ObjectToStructuredData: Introspection not found for the specified class");
        }
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "ObjectToStructuredData: Class not registered");
    }

    return ret;
}

/**
 * @brief Performs the conversion from a StructuredDataI node to a StructuredDataI node.
 * @param[out] destination is the output.
 * @param[in] source is the input.
 * @return true if the conversion succeeds, false otherwise.
 */
static bool StructuredDataToStructuredData(const AnyType &destination, const AnyType &source) {
    StructuredDataI* sourcePointer = reinterpret_cast<StructuredDataI*>(source.GetDataPointer());
    StructuredDataI* destinationPointer = reinterpret_cast<StructuredDataI*>(destination.GetDataPointer());

    return sourcePointer->Copy(*destinationPointer);
}

/**
 * @brief Performs the basic type conversion.
 * @param[out] destination is the converted type in output.
 * @param[in] source is the type to be converted.
 * @return true if the conversion succeeds, false otherwise.
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool ScalarBasicTypeConvert(const AnyType &destination, const AnyType &source) {

    void* destinationPointer = destination.GetDataPointer();
    const TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void* sourcePointer = source.GetDataPointer();
    const TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();

    bool ret = false;

    bool isSourceStructured = sourceDescriptor.isStructuredData;
    bool isDestinationStructured = destinationDescriptor.isStructuredData;

    // case source is a StructuredDataNode
    if (sourceDescriptor == StructuredDataInterfaceType) {

        if (isDestinationStructured) {
            ret = StructuredDataToObject(destination, source);
        }
        else if (destinationDescriptor == StructuredDataInterfaceType) {
            ret = StructuredDataToStructuredData(destination, source);
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "ScalarBasicTypeConvert: No known conversion StructuredDataI to basic type. Use StructuredDataI::Read(*)");
        }
    }

    // case source is a StructuredDataNode
    else if (destinationDescriptor == StructuredDataInterfaceType) {

        if (isSourceStructured) {
            ret = ObjectToStructuredData(destination, source);
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "ScalarBasicTypeConvert: No known conversion from basic type to StructuredDataI. Use StructuredDataI::Write(*)");
        }
    }
    else if ((isSourceStructured) && (isDestinationStructured)) {

        ret = ObjectToObject(destination, source);
    }
    // source and descriptor are not StructuredDataNode
    else if ((!isSourceStructured) && (!isDestinationStructured)) {

        if (sourceDescriptor == destinationDescriptor) {
            if (sourceDescriptor.type == SString) {
                StreamString *stringSource = reinterpret_cast<StreamString*>(sourcePointer);
                StreamString *stringDestination = reinterpret_cast<StreamString*>(destinationPointer);
                uint32 stringLength = static_cast<uint32>(stringSource->Size());
                ret = stringDestination->Write(stringSource->Buffer(), stringLength);
            }
            else if (sourceDescriptor.type == BT_CCString) {
                ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), reinterpret_cast<const char8 *>(sourcePointer));
            }
            else if (sourceDescriptor.type == CArray) {
                uint32 copySize = source.GetByteSize();
                if (source.GetNumberOfDimensions() > 0u) {
                    copySize *= source.GetNumberOfElements(0u);
                }
                ret = MemoryOperationsHelper::Copy(destinationPointer, sourcePointer, copySize);
            }
            else if (sourceDescriptor.type == Pointer) {
                REPORT_ERROR_STATIC(ErrorManagement::UnsupportedFeature, "ScalarBasicTypeConvert: Conversion to pointer unsupported. Try to cast the pointer to uintp");
            }
            else {
                uint32 copySize = source.GetByteSize();
                ret = MemoryOperationsHelper::Copy(destinationPointer, sourcePointer, copySize);
            }
        }
        else {
            bool isSourceSignedInteger = (sourceDescriptor.type == SignedInteger);
            bool isSourceUnsignedInteger = (sourceDescriptor.type == UnsignedInteger);
            bool isSourcePointer = (sourceDescriptor.type == Pointer);

            if ((isSourceSignedInteger) || (isSourceUnsignedInteger) || (isSourcePointer)) {
                ret = IntegerToType(destination, source);
            }
            else if (sourceDescriptor.type == BT_Boolean) {
                ret = BooleanToType(destination, source);
            }
            else if (sourceDescriptor.type == Float) {
                ret = FloatToType(destination, source);
            }
            else {
                ret = StringToType(destination, source);
            }

        }
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "ScalarBasicTypeConvert: No known conversion between types in input");
    }

    return ret;
}

/**
 * @brief Performs the conversion from a vector to a vector.
 * @param[out] destination is the converted vector in output.
 * @param[in] source is the vector to be converted.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetNumberOfDimension() == 1 &&
 *   destination.GetNumberOfDimension() == 1 &&
 *   source.GetNumberOfElements([0:2]) == destination.GetNumberOfElements([0:2]);
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool VectorBasicTypeConvert(const AnyType &destination, const AnyType &source) {
    bool ok = true;
    uint32 numberOfElements = source.GetNumberOfElements(0u);
    //special case of char8[]
    if (source.GetTypeDescriptor() == Character8Bit) {
        numberOfElements = source.GetNumberOfElements(1u);
    }
    // Assume that the number of dimensions is equal
    for (uint32 idx = 0u; (idx < numberOfElements); idx++) {
        uint32 sourceElementByteSize = static_cast<uint32>(source.GetByteSize());
        uint32 sourceIndex = idx * sourceElementByteSize;
        uint32 destinationElementByteSize = static_cast<uint32>(destination.GetByteSize());
        uint32 destinationIndex = idx * destinationElementByteSize;

        char8 *sourceArray = reinterpret_cast<char8 *>(source.GetDataPointer());

        bool isSourceCArray = (source.GetTypeDescriptor().type == CArray);
        bool isSourceStaticDeclared = source.IsStaticDeclared();

        // Consider the special case of matrix of characters
        if ((isSourceCArray) && (!isSourceStaticDeclared)) {
            sourceArray = reinterpret_cast<char8 **>(source.GetDataPointer())[idx];
            sourceIndex = 0u;
        }

        char8 *destinationArray = reinterpret_cast<char8 *>(destination.GetDataPointer());

        bool isDestinationCArray = (destination.GetTypeDescriptor().type == CArray);
        bool isDestinationStaticDeclared = destination.IsStaticDeclared();

        // Consider the special case of matrix of characters
        if ((isDestinationCArray) && (!isDestinationStaticDeclared)) {
            destinationArray = reinterpret_cast<char8 **>(destination.GetDataPointer())[idx];
            destinationIndex = 0u;
        }

        char8 *newSourcePointer = &sourceArray[sourceIndex];
        AnyType elementSource(source.GetTypeDescriptor(), source.GetBitAddress(), newSourcePointer);
        // special case of c-string
        if (source.GetTypeDescriptor() == CharString) {
            elementSource = AnyType(*reinterpret_cast<char8**>(newSourcePointer));
        }
        // special case of pointer
        else if (source.GetTypeDescriptor().type == Pointer) {
            elementSource = AnyType(*reinterpret_cast<void**>(newSourcePointer));
        }
        else if (source.GetTypeDescriptor() == Character8Bit) {
            uint32 srcArrayIdx = sourceIndex * source.GetNumberOfElements(0u);
            newSourcePointer = &sourceArray[srcArrayIdx];
            elementSource = AnyType(Character8Bit, 0u, newSourcePointer);
            elementSource.SetNumberOfDimensions(1u);
            elementSource.SetNumberOfElements(0u, source.GetNumberOfElements(0u));
        }
        else {//NOOP
        }

        char8 *newDestinationPointer = &destinationArray[destinationIndex];
        AnyType elementDestination(destination.GetTypeDescriptor(), destination.GetBitAddress(), newDestinationPointer);
        // special case of c-string
        if (destination.GetTypeDescriptor() == CharString) {
            elementDestination = AnyType(*reinterpret_cast<char8**>(newDestinationPointer));
        }
        else if (destination.GetTypeDescriptor() == Character8Bit) {
            uint32 destinationArrayIdx = destinationIndex * destination.GetNumberOfElements(0u);
            newDestinationPointer = &destinationArray[destinationArrayIdx];
            elementDestination = AnyType(Character8Bit, 0u, newDestinationPointer);
            elementDestination.SetNumberOfDimensions(1u);
            elementDestination.SetNumberOfElements(0u, destination.GetNumberOfElements(0u));
        }
        else {//NOOP
        }

        if (!ScalarBasicTypeConvert(elementDestination, elementSource)) {
            ok = false;
        }

    }

    return ok;
}

/**
 * @brief Performs the conversion from a static matrix to a static matrix.
 * @param[out] destination is the converted matrix in output.
 * @param[in] source is the matrix to be converted.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetNumberOfDimension() == 2 &&
 *   destination.GetNumberOfDimension() == 2 &&
 *   source.GetNumberOfElements([0:2]) == destination.GetNumberOfElements([0:2]);
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool StaticToStaticMatrix(const AnyType &destination, const AnyType &source) {

    uint32 numberOfRows = destination.GetNumberOfElements(1u);
    uint32 numberOfColumns = destination.GetNumberOfElements(0u);
    TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();
    TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void *sourcePointer = source.GetDataPointer();
    void *destinationPointer = destination.GetDataPointer();

    bool ok = true;
    for (uint32 r = 0u; (r < numberOfRows); r++) {

        char8* sourceArray = reinterpret_cast<char8 *>(sourcePointer);
        char8* destinationArray = reinterpret_cast<char8 *>(destinationPointer);

        uint32 sourceElementByteSize = static_cast<uint32>(source.GetByteSize());
        //Treat the char8[] differently
        if (source.GetTypeDescriptor() == Character8Bit) {
            sourceElementByteSize *= source.GetNumberOfElements(2u);
        }

        uint32 destinationElementByteSize = static_cast<uint32>(destination.GetByteSize());
        //Treat the char8[] differently
        if (destination.GetTypeDescriptor() == Character8Bit) {
            destinationElementByteSize *= destination.GetNumberOfElements(2u);
        }

        uint32 sourceRowIndex = ((r * numberOfColumns) * sourceElementByteSize);
        AnyType sourceRow(sourceDescriptor, source.GetBitAddress(), &sourceArray[sourceRowIndex]);
        sourceRow.SetNumberOfDimensions(1u);
        //Treat the char8[] differently
        if (source.GetTypeDescriptor() == Character8Bit) {
            sourceRow.SetNumberOfElements(0u, source.GetNumberOfElements(2u));
            sourceRow.SetNumberOfElements(1u, numberOfColumns);
        }
        else {
            sourceRow.SetNumberOfElements(0u, numberOfColumns);
        }
        sourceRow.SetStaticDeclared(true);

        uint32 destinationRowIndex = ((r * numberOfColumns) * destinationElementByteSize);
        AnyType destinationRow(destinationDescriptor, destination.GetBitAddress(), &destinationArray[destinationRowIndex]);
        destinationRow.SetNumberOfDimensions(1u);
        //Treat the char8[] differently
        if (destination.GetTypeDescriptor() == Character8Bit) {
            destinationRow.SetNumberOfElements(0u, destination.GetNumberOfElements(2u));
            destinationRow.SetNumberOfElements(1u, numberOfColumns);
        }
        else {
            destinationRow.SetNumberOfElements(0u, numberOfColumns);
        }
        destinationRow.SetStaticDeclared(true);

        if (!VectorBasicTypeConvert(destinationRow, sourceRow)) {
            ok = false;
        }
    }
    return ok;
}

/**
 * @brief Performs the conversion from a matrix allocated on heap to a static matrix.
 * @param[out] destination is the converted matrix in output.
 * @param[in] source is the matrix to be converted.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetNumberOfDimension() == 2 &&
 *   destination.GetNumberOfDimension() == 2 &&
 *   source.GetNumberOfElements([0:2]) == destination.GetNumberOfElements([0:2]);
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool StaticToHeapMatrix(const AnyType &destination, const AnyType &source) {
    uint32 numberOfRows = destination.GetNumberOfElements(1u);
    uint32 numberOfColumns = destination.GetNumberOfElements(0u);
    TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();
    TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void *sourcePointer = source.GetDataPointer();
    void *destinationPointer = destination.GetDataPointer();

    bool ok = true;
    for (uint32 r = 0u; (r < numberOfRows); r++) {

        char8* sourceArray = reinterpret_cast<char8 *>(sourcePointer);
        char8* destinationArray = reinterpret_cast<char8 **>(destinationPointer)[r];

        uint32 sourceElementByteSize = static_cast<uint32>(source.GetByteSize());
        //Treat the char8[] differently
        if (source.GetTypeDescriptor() == Character8Bit) {
            sourceElementByteSize *= source.GetNumberOfElements(2u);
        }

        uint32 sourceRowIndex = ((r * numberOfColumns) * sourceElementByteSize);
        AnyType sourceRow(sourceDescriptor, source.GetBitAddress(), &sourceArray[sourceRowIndex]);
        //Treat the char8[] differently
        if (source.GetTypeDescriptor() == Character8Bit) {
            sourceRow.SetNumberOfElements(0u, source.GetNumberOfElements(2u));
            sourceRow.SetNumberOfElements(1u, numberOfColumns);
        }
        else {
            sourceRow.SetNumberOfElements(0u, numberOfColumns);
        }
        sourceRow.SetStaticDeclared(true);

        AnyType destinationRow(destinationDescriptor, destination.GetBitAddress(), destinationArray);
        destinationRow.SetNumberOfDimensions(1u);
        //Treat the char8[] differently
        if (destination.GetTypeDescriptor() == Character8Bit) {
            destinationRow.SetNumberOfElements(0u, destination.GetNumberOfElements(2u));
            destinationRow.SetNumberOfElements(1u, numberOfColumns);
        }
        else {
            destinationRow.SetNumberOfElements(0u, numberOfColumns);
        }
        destinationRow.SetStaticDeclared(false);

        if (!VectorBasicTypeConvert(destinationRow, sourceRow)) {
            ok = false;
        }
    }
    return ok;
}

/**
 * @brief Performs the conversion from a static matrix to a matrix allocated on heap.
 * @param[out] destination is the converted matrix in output.
 * @param[in] source is the matrix to be converted.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetNumberOfDimension() == 2 &&
 *   destination.GetNumberOfDimension() == 2 &&
 *   source.GetNumberOfElements([0:2]) == destination.GetNumberOfElements([0:2]);
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool HeapToStaticMatrix(const AnyType &destination, const AnyType &source) {
    uint32 numberOfRows = destination.GetNumberOfElements(1u);
    uint32 numberOfColumns = destination.GetNumberOfElements(0u);
    TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();
    TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void *sourcePointer = source.GetDataPointer();
    void *destinationPointer = destination.GetDataPointer();

    bool ok = true;
    for (uint32 r = 0u; (r < numberOfRows); r++) {
        char8* sourceArray = reinterpret_cast<char8 **>(sourcePointer)[r];
        char8* destinationArray = reinterpret_cast<char8 *>(destinationPointer);

        AnyType sourceRow(sourceDescriptor, source.GetBitAddress(), sourceArray);
        sourceRow.SetNumberOfDimensions(1u);
        //Treat the char8[] differently
        if (source.GetTypeDescriptor() == Character8Bit) {
            sourceRow.SetNumberOfElements(0u, source.GetNumberOfElements(2u));
            sourceRow.SetNumberOfElements(1u, numberOfColumns);
        }
        else {
            sourceRow.SetNumberOfElements(0u, numberOfColumns);
        }
        sourceRow.SetStaticDeclared(false);

        uint32 destinationElementByteSize = static_cast<uint32>(destination.GetByteSize());
        uint32 destinationRowIndex = r * numberOfColumns * destinationElementByteSize;
        AnyType destinationRow(destinationDescriptor, destination.GetBitAddress(), &destinationArray[destinationRowIndex]);
        destinationRow.SetNumberOfDimensions(1u);
        destinationRow.SetNumberOfElements(0u, numberOfColumns);
        destinationRow.SetStaticDeclared(true);

        if (!VectorBasicTypeConvert(destinationRow, sourceRow)) {
            ok = false;
        }
    }
    return ok;
}

/**
 * @brief Performs the conversion from a matrix allocated on heap to a matrix allocated on heap.
 * @param[out] destination is the converted matrix in output.
 * @param[in] source is the matrix to be converted.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetNumberOfDimension() == 2 &&
 *   destination.GetNumberOfDimension() == 2 &&
 *   source.GetNumberOfElements([0:2]) == destination.GetNumberOfElements([0:2]);
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool HeapToHeapMatrix(const AnyType &destination, const AnyType &source) {
    uint32 numberOfRows = destination.GetNumberOfElements(1u);
    uint32 numberOfColumns = destination.GetNumberOfElements(0u);
    TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();
    TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void *sourcePointer = source.GetDataPointer();
    void *destinationPointer = destination.GetDataPointer();

    bool ok = true;
    for (uint32 r = 0u; (r < numberOfRows); r++) {
        char8* sourceArray = reinterpret_cast<char8 **>(sourcePointer)[r];
        char8* destinationArray = reinterpret_cast<char8 **>(destinationPointer)[r];

        AnyType sourceRow(sourceDescriptor, source.GetBitAddress(), sourceArray);
        sourceRow.SetNumberOfDimensions(1u);
        //Treat the char8[] differently
        if (source.GetTypeDescriptor() == Character8Bit) {
            sourceRow.SetNumberOfElements(0u, source.GetNumberOfElements(2u));
            sourceRow.SetNumberOfElements(1u, numberOfColumns);
        }
        else {
            sourceRow.SetNumberOfElements(0u, numberOfColumns);
        }
        sourceRow.SetStaticDeclared(false);

        AnyType destinationRow(destinationDescriptor, destination.GetBitAddress(), destinationArray);
        destinationRow.SetNumberOfDimensions(1u);
        //Treat the char8[] differently
        if (destination.GetTypeDescriptor() == Character8Bit) {
            destinationRow.SetNumberOfElements(0u, destination.GetNumberOfElements(2u));
            destinationRow.SetNumberOfElements(1u, numberOfColumns);
        }
        else {
            destinationRow.SetNumberOfElements(0u, numberOfColumns);
        }
        destinationRow.SetStaticDeclared(false);

        if (!VectorBasicTypeConvert(destinationRow, sourceRow)) {
            ok = false;
        }
    }
    return ok;
}

/**
 * @brief Calls the correct matrix conversion function depending on the source and destination memory type (static or heap).
 * @param[out] destination is the converted matrix in output.
 * @param[in] source is the matrix to be converted.
 * @return true if the conversion succeeds, false otherwise.
 * @pre
 *   source.GetNumberOfDimension() == 1 &&
 *   destination.GetNumberOfDimension() == 1 &&
 *   source.GetNumberOfElements([0:2]) == destination.GetNumberOfElements([0:2]);
 */
/*lint -e{1573} [MISRA C++ Rule 14-5-1]. Justification: MARTe::HighResolutionTimerCalibrator is not a possible argument for this function template.*/
static bool MatrixBasicTypeConvert(const AnyType &destination, const AnyType &source) {

    bool isSourceStatic = source.IsStaticDeclared();
    bool isDestinationStatic = destination.IsStaticDeclared();

    bool ok = false;

    if ((isSourceStatic) && (isDestinationStatic)) {
        ok = StaticToStaticMatrix(destination, source);
    }

    else if ((isSourceStatic) && (!isDestinationStatic)) {
        ok = StaticToHeapMatrix(destination, source);
    }

    else if ((!isSourceStatic) && (isDestinationStatic)) {
        ok = HeapToStaticMatrix(destination, source);
    }

    else if ((!isSourceStatic) && (!isDestinationStatic)) {
        ok = HeapToHeapMatrix(destination, source);
    }
    else {//NOOP
    }
    return ok;
}

}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

bool TypeConvert(const AnyType &destination, const AnyType &source) {

    bool ok = true;
    if (static_cast<bool>(destination.GetTypeDescriptor().isConstant)) {
        ok = false;
    }
    if (ok) {

        //Source and destination dimensions must be the same
        uint8 destinationNumberOfDimensions = destination.GetNumberOfDimensions();
        uint8 sourceNumberOfDimensions = source.GetNumberOfDimensions();
        bool destinationIsCArray = (destination.GetTypeDescriptor() == Character8Bit);
        bool sourceIsCArray = (source.GetTypeDescriptor() == Character8Bit);

        //Give the char8 strings a special treatment
        if (destinationIsCArray) {
            if (destination.GetNumberOfDimensions() > 0u) {
                destinationNumberOfDimensions -= 1u;
            }
        }
        if (sourceIsCArray) {
            if (source.GetNumberOfDimensions() > 0u) {
                sourceNumberOfDimensions -= 1u;
            }
        }
        ok = (destinationNumberOfDimensions == sourceNumberOfDimensions);
        /*if (!ok) {
            //Beware that by enabling this, there will be a lot of noise driven by attempts of reading arrays (e.g. if (!data.Read("MyArray", myarray")) {...
            REPORT_ERROR_STATIC(ErrorManagement::Debug, "Dimension mismatch. Destination: %d Source %d", destinationNumberOfDimensions, sourceNumberOfDimensions);
        }*/
        //The number of elements in all dimensions must be the same
        //Again, give the char8 strings a special treatment
        if (destinationIsCArray) {
            for (uint32 i = 0u; ok && (i < destinationNumberOfDimensions); i++) {
                uint32 destinationNumberOfElements = 0u;
                uint32 sourceNumberOfElements = 0u;
                //Horrible asymmetry between the way matrices and vectors of char8 are handled in the AnyType
                if (destinationNumberOfDimensions == 1u) {
                    destinationNumberOfElements = destination.GetNumberOfElements(i + 1u);
                }
                else {
                    destinationNumberOfElements = destination.GetNumberOfElements(i);
                }
                if ((sourceIsCArray) && (destinationNumberOfDimensions == 1u)) {
                    sourceNumberOfElements = source.GetNumberOfElements(i + 1u);
                }
                else {
                    sourceNumberOfElements = source.GetNumberOfElements(i);
                }
                ok = (destinationNumberOfElements == sourceNumberOfElements);
            }
        }
        else if (sourceIsCArray) {
            for (uint32 i = 0u; ok && (i < destinationNumberOfDimensions); i++) {
                uint32 destinationNumberOfElements = destination.GetNumberOfElements(i);
                uint32 sourceNumberOfElements = 0u;
                if (sourceNumberOfDimensions == 1u) {
                    sourceNumberOfElements = source.GetNumberOfElements(i + 1u);
                }
                else {
                    sourceNumberOfElements = source.GetNumberOfElements(i);
                }
                ok = (destinationNumberOfElements == sourceNumberOfElements);
            }
        }
        else {
            for (uint32 i = 0u; ok && (i < destinationNumberOfDimensions); i++) {
                uint32 destinationNumberOfElements = destination.GetNumberOfElements(i);
                uint32 sourceNumberOfElements = source.GetNumberOfElements(i);
                ok = (destinationNumberOfElements == sourceNumberOfElements);
            }
        }
        if (ok) {
            if (sourceNumberOfDimensions == 0u) {
                ok = ScalarBasicTypeConvert(destination, source);
            }
            else if (sourceNumberOfDimensions == 1u) {
                ok = VectorBasicTypeConvert(destination, source);
            }
            else if (sourceNumberOfDimensions == 2u) {
                ok = MatrixBasicTypeConvert(destination, source);
            }
            else {//NOOP
            }
        }
    }
    return ok;
}
}
