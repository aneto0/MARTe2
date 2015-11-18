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
#include "TypeConversion.h"
#include "AnyType.h"
#include "BitSetToInteger.h"
#include "String.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

extern bool StringToInteger(const char8 * const input,
                            uint8 &number);

extern bool StringToInteger(const char8 * const input,
                            uint16 &number);

extern bool StringToInteger(const char8 * const input,
                            uint32 &number);

extern bool StringToInteger(const char8 * const input,
                            uint64 &number);

extern bool StringToInteger(const char8 * const input,
                            int8 &number);

extern bool StringToInteger(const char8 * const input,
                            int16 &number);

extern bool StringToInteger(const char8 * const input,
                            int32 &number);

extern bool StringToInteger(const char8 * const input,
                            int64 &number);

extern bool StringToFloat(const char8 * const input,
                          float32 &number);

extern bool StringToFloat(const char8 * const input,
                          float64 &number);

bool StringToIntegerGeneric(const char8* source,
                            uint8 *dest,
                            const uint8 destBitSize,
                            const bool isSigned) {

    bool ret = false;
    if (destBitSize <= 8u) {
        if (isSigned) {
            int8 tempDest;
            ret = StringToInteger(source, tempDest);
            *(reinterpret_cast<int8*>(dest)) = tempDest;
        }
        else {
            uint8 tempDest;
            ret = StringToInteger(source, tempDest);
            *dest = tempDest;
        }
    }
    if ((destBitSize > 8u) && (destBitSize <= 16u)) {
        if (isSigned) {
            int16 tempDest;
            ret = StringToInteger(source, tempDest);
            *(reinterpret_cast<int16*>(dest)) = tempDest;
        }
        else {
            uint16 tempDest;
            ret = StringToInteger(source, tempDest);
            *(reinterpret_cast<uint16*>(dest)) = tempDest;
        }
    }
    if ((destBitSize > 16u) && (destBitSize <= 32u)) {

        if (isSigned) {
            int32 tempDest;
            ret = StringToInteger(source, tempDest);
            *(reinterpret_cast<int32*>(dest)) = tempDest;
        }
        else {
            uint32 tempDest;
            ret = StringToInteger(source, tempDest);
            *(reinterpret_cast<uint32*>(dest)) = tempDest;
        }
    }
    if ((destBitSize > 32u) && (destBitSize <= 64u)) {
        if (isSigned) {
            int64 tempDest;
            ret = StringToInteger(source, tempDest);
            *(reinterpret_cast<int64*>(dest)) = tempDest;
        }
        else {
            uint64 tempDest;
            ret = StringToInteger(source, tempDest);
            *(reinterpret_cast<uint64*>(dest)) = tempDest;
        }
    }
    return ret;
}

template<typename IntegerType, typename FloatType>
bool IntegerToFloat(IntegerType source,
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

template<typename FloatType>
static bool MinMaxFloat(bool isPositive,
                        uint8 numberOfBits,
                        FloatType &number) {

    bool ret = false;
    if (isPositive) {
        if (numberOfBits == 4u) {
            uint32 maxFloat32Mask = 0x7f7fffffu;
            MemoryOperationsHelper::Copy(&number, &maxFloat32Mask, 4u);
            ret = true;
        }
    }
    else {
        if (numberOfBits == 4u) {
            uint32 maxFloat32Mask = 0xff7fffffu;
            MemoryOperationsHelper::Copy(&number, &maxFloat32Mask, 4u);
            ret = true;
        }
    }
    return ret;
}

template<typename FloatType1, typename FloatType2>
bool FloatToFloat(FloatType1 source,
                  FloatType2 &destination) {

    destination = static_cast<FloatType2>(0.0);

    bool ret = ((!isNaN(source)) && (!isInf(source)));
    if (ret) {
        FloatType2 test = static_cast<FloatType2>(source);

        if ((isNaN(test)) || (isInf(test))) {
            if (sizeof(FloatType2) < sizeof(FloatType1)) {
                ret = MinMaxFloat((source > 0.0), sizeof(FloatType2), destination);
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
 * @brief BasicType conversion between two scalar types.
 * @param[out] destination the converted type.
 * @param[in] source the type to be converted.
 * @return true if the conversion is both types are indeed scalar and if the
 * conversion is successful.
 */
static bool ScalarBasicTypeConvert(const AnyType &destination,
                                   const AnyType &source) {

    void* destinationPointer = destination.GetDataPointer();
    const TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void* sourcePointer = source.GetDataPointer();
    const TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();

    bool ret = false;

    if (sourceDescriptor == destinationDescriptor) {
        if (sourceDescriptor.type == SString) {
            String *stringSource = reinterpret_cast<String*>(sourcePointer);
            String *stringDestination = reinterpret_cast<String*>(destinationPointer);
            uint32 stringLength = static_cast<uint32>(stringSource->Size());
            ret = stringDestination->Write(stringSource->Buffer(), stringLength);
        }
        else {
            uint32 copySize = static_cast<uint32>(sourceDescriptor.numberOfBits) / 8u;
            ret = MemoryOperationsHelper::Copy(destinationPointer, sourcePointer, copySize);
        }
    }
    else {

        if ((sourceDescriptor.type == SignedInteger) || (sourceDescriptor.type == UnsignedInteger)) {

            if (destinationDescriptor.type == SString) {
                String tempString;
                ret = tempString.PrintFormatted("%d", &source);
                if (ret) {
                    uint32 stringLength = static_cast<uint32>(tempString.Size());
                    ret = (reinterpret_cast<String*>(destinationPointer))->Write(tempString.Buffer(), stringLength);
                }
            }
            if (destinationDescriptor.type == SignedInteger) {
                if ((sourceDescriptor.type == SignedInteger)) {
                    uint8* destinationInput = reinterpret_cast<uint8*>(destinationPointer);
                    uint8* sourceInput = reinterpret_cast<uint8*>(sourcePointer);
                    uint8 destShift = static_cast<uint8>(destination.GetBitAddress());
                    uint8 sourceShift = static_cast<uint8>(source.GetBitAddress());
                    ret = BitSetToBitSet(destinationInput, destShift, static_cast<uint8>(destinationDescriptor.numberOfBits), true, sourceInput, sourceShift,
                                         static_cast<uint8>(sourceDescriptor.numberOfBits), true);

                }
                if (sourceDescriptor.type == UnsignedInteger) {
                    uint8* destinationInput = reinterpret_cast<uint8*>(destinationPointer);
                    uint8* sourceInput = reinterpret_cast<uint8*>(sourcePointer);
                    uint8 destShift = static_cast<uint8>(destination.GetBitAddress());
                    uint8 sourceShift = static_cast<uint8>(source.GetBitAddress());
                    ret = BitSetToBitSet(destinationInput, destShift, static_cast<uint8>(destinationDescriptor.numberOfBits), true, sourceInput, sourceShift,
                                         static_cast<uint8>(sourceDescriptor.numberOfBits), false);
                }
            }
            if (destinationDescriptor.type == UnsignedInteger) {
                if ((sourceDescriptor.type == SignedInteger)) {
                    uint8* destinationInput = reinterpret_cast<uint8*>(destinationPointer);
                    uint8* sourceInput = reinterpret_cast<uint8*>(sourcePointer);
                    uint8 destShift = static_cast<uint8>(destination.GetBitAddress());
                    uint8 sourceShift = static_cast<uint8>(source.GetBitAddress());
                    ret = BitSetToBitSet(destinationInput, destShift, static_cast<uint8>(destinationDescriptor.numberOfBits), false, sourceInput, sourceShift,
                                         static_cast<uint8>(sourceDescriptor.numberOfBits), true);
                }
                if (sourceDescriptor.type == UnsignedInteger) {
                    uint8* destinationInput = reinterpret_cast<uint8*>(destinationPointer);
                    uint8* sourceInput = reinterpret_cast<uint8*>(sourcePointer);
                    uint8 destShift = static_cast<uint8>(destination.GetBitAddress());
                    uint8 sourceShift = static_cast<uint8>(source.GetBitAddress());
                    ret = BitSetToBitSet(destinationInput, destShift, static_cast<uint8>(destinationDescriptor.numberOfBits), false, sourceInput, sourceShift,
                                         static_cast<uint8>(sourceDescriptor.numberOfBits), false);
                }
            }

            if (destinationDescriptor.type == Float) {
                if ((sourceDescriptor.type == SignedInteger)) {
                    uint8* sourceInput = reinterpret_cast<uint8*>(sourcePointer);
                    float32* destinationInput = reinterpret_cast<float32*>(destinationPointer);
                    ret = IntegerToFloatGeneric(sourceInput, static_cast<uint8>(sourceDescriptor.numberOfBits), destinationInput,
                                                static_cast<uint8>(destinationDescriptor.numberOfBits), true);
                }
                if (sourceDescriptor.type == UnsignedInteger) {
                    uint8* sourceInput = reinterpret_cast<uint8*>(sourcePointer);
                    float32* destinationInput = reinterpret_cast<float32*>(destinationPointer);
                    ret = IntegerToFloatGeneric(sourceInput, static_cast<uint8>(sourceDescriptor.numberOfBits), destinationInput,
                                                static_cast<uint8>(destinationDescriptor.numberOfBits), false);
                }
            }
        }

        if (sourceDescriptor.type == Float) {

            if (destinationDescriptor.type == SString) {
                String tempString;
                ret = tempString.PrintFormatted("%E", &source);
                if (ret) {
                    *(reinterpret_cast<String*>(destinationPointer)) = tempString;
                    ret = (*(reinterpret_cast<String*>(destinationPointer))) == tempString;
                }
            }
            if (destinationDescriptor.type == SignedInteger) {
                ret = FloatToIntegerGeneric(reinterpret_cast<float32*>(sourcePointer), static_cast<uint8>(sourceDescriptor.numberOfBits),
                                            reinterpret_cast<uint8*>(destinationPointer), static_cast<uint8>(destinationDescriptor.numberOfBits), true);
            }
            if (destinationDescriptor.type == UnsignedInteger) {
                ret = FloatToIntegerGeneric(reinterpret_cast<float32*>(sourcePointer), static_cast<uint8>(sourceDescriptor.numberOfBits),
                                            reinterpret_cast<uint8*>(destinationPointer), static_cast<uint8>(destinationDescriptor.numberOfBits), false);
            }
            if (destinationDescriptor.type == Float) {
                if (destinationDescriptor.numberOfBits == 32u) {
                    if (sourceDescriptor.numberOfBits == 64u) {
                        ret = FloatToFloat(*(reinterpret_cast<float64*>(sourcePointer)), *(reinterpret_cast<float32*>(destinationPointer)));
                    }
                }
                if (destinationDescriptor.numberOfBits == 64u) {
                    if (sourceDescriptor.numberOfBits == 32u) {
                        ret = FloatToFloat(*(reinterpret_cast<float32*>(sourcePointer)), *(reinterpret_cast<float64*>(destinationPointer)));
                    }
                }
            }
        }

        if ((sourceDescriptor.type == CCString) || (sourceDescriptor.type == SString)) {
            const char8* token = static_cast<const char8 *>(NULL);
            if(sourceDescriptor.type == CCString) {
                if(source.GetNumberOfDimensions() == 0u) {
                    token=reinterpret_cast<const char8*>(sourcePointer);
                }
                else {
                    token=*reinterpret_cast<const char8**>(sourcePointer);
                }
            }
            else {
                token=(reinterpret_cast<String*>(sourcePointer))->Buffer();
            }
            uint32 tokenLength=StringHelper::Length(token);
            if(destinationDescriptor.type==SString) {
                String* tempString=reinterpret_cast<String*>(destinationPointer);
                ret=tempString->Write(token, tokenLength);
            }
            if(destinationDescriptor.type==SignedInteger) {
                ret=StringToIntegerGeneric(token,
                        reinterpret_cast<uint8*>(destinationPointer),
                        static_cast<uint8>(destinationDescriptor.numberOfBits),true);

            }
            if(destinationDescriptor.type==UnsignedInteger) {

                ret=StringToIntegerGeneric(token,
                        reinterpret_cast<uint8*>(destinationPointer),
                        static_cast<uint8>(destinationDescriptor.numberOfBits),false);
            }
            if(destinationDescriptor.type==Float) {
                if(destinationDescriptor.numberOfBits==32u) {
                    ret=StringToFloat(token,*(reinterpret_cast<float32*>(destinationPointer)));
                }
                if(destinationDescriptor.numberOfBits==64u) {
                    ret=StringToFloat(token,*(reinterpret_cast<float64*>(destinationPointer)));
                }
            }
        }
    }

    return ret;
}

/**
 * TODO
 */
static bool VectorBasicTypeConvert(const AnyType &destination,
                                   const AnyType &source) {
    uint32 idx = 0u;
    uint32 numberOfElements = source.GetNumberOfElements(0u);

    // Assume that the number of dimensions is equal
    bool ok = true;

    for (idx = 0; (idx < numberOfElements); idx++) {

        char8 *sourceArray = static_cast<char8 *>(source.GetDataPointer());
        char8 *destinationArray = static_cast<char8 *>(destination.GetDataPointer());
        uint32 sourceElementByteSize = static_cast<uint32>(source.GetTypeDescriptor().numberOfBits) / 8u;
        uint32 destinationElementByteSize = static_cast<uint32>(destination.GetTypeDescriptor().numberOfBits) / 8u;

        uint32 sourceIndex = idx * sourceElementByteSize;
        uint32 destinationIndex = idx * destinationElementByteSize;

        AnyType elementSource(source);
        elementSource.SetDataPointer(&sourceArray[sourceIndex]);
        AnyType elementDestination(destination);
        elementDestination.SetDataPointer(&destinationArray[destinationIndex]);

        if (!ScalarBasicTypeConvert(elementDestination, elementSource)) {
            ok = false;
        }

    }
    return ok;
}

static bool StaticToStaticMatrix(const AnyType &destination,
                                 const AnyType &source) {

    uint32 numberOfRows = destination.GetNumberOfElements(1u);
    uint32 numberOfColumns = destination.GetNumberOfElements(0u);
    TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();
    TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void *sourcePointer = source.GetDataPointer();
    void *destinationPointer = destination.GetDataPointer();

    bool ok = true;
    for (uint32 r = 0u; (r < numberOfRows); r++) {

        for (uint32 c = 0u; (c < numberOfColumns); c++) {
            char8* sourceArray = reinterpret_cast<char8 *>(sourcePointer);
            char8* destinationArray = reinterpret_cast<char8 *>(destinationPointer);

            uint32 sourceElementByteSize = static_cast<uint32>(sourceDescriptor.numberOfBits) / 8u;
            uint32 destinationElementByteSize = static_cast<uint32>(destinationDescriptor.numberOfBits) / 8u;
            uint32 sourceIndex = ((r * numberOfColumns) + c) * sourceElementByteSize;
            uint32 destinationIndex = ((r * numberOfColumns) + c) * destinationElementByteSize;

            AnyType elementSource(source);
            elementSource.SetDataPointer(&sourceArray[sourceIndex]);
            AnyType elementDestination(destination);
            elementDestination.SetDataPointer(&destinationArray[destinationIndex]);

            if (!ScalarBasicTypeConvert(elementDestination, elementSource)) {
                ok = false;
            }
        }
    }
    return ok;
}

static bool StaticToHeapMatrix(const AnyType &destination,
                               const AnyType &source) {
    uint32 numberOfRows = destination.GetNumberOfElements(1u);
    uint32 numberOfColumns = destination.GetNumberOfElements(0u);
    TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();
    TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void *sourcePointer = source.GetDataPointer();
    void *destinationPointer = destination.GetDataPointer();

    bool ok = true;
    for (uint32 r = 0u; (r < numberOfRows); r++) {

        for (uint32 c = 0u; (c < numberOfColumns); c++) {
            char8* sourceArray = reinterpret_cast<char8 *>(sourcePointer);
            char8* destinationArray = reinterpret_cast<char8 **>(destinationPointer)[r];

            uint32 sourceElementByteSize = static_cast<uint32>(sourceDescriptor.numberOfBits) / 8u;
            uint32 destinationElementByteSize = static_cast<uint32>(destinationDescriptor.numberOfBits) / 8u;
            uint32 sourceIndex = (r * numberOfColumns + c) * sourceElementByteSize;
            uint32 destinationIndex = c * destinationElementByteSize;

            AnyType elementSource(source);
            elementSource.SetDataPointer(&sourceArray[sourceIndex]);
            AnyType elementDestination(destination);
            elementDestination.SetDataPointer(&destinationArray[destinationIndex]);

            if (!ScalarBasicTypeConvert(elementDestination, elementSource)) {
                ok = false;
            }
        }
    }
    return ok;
}

static bool HeapToStaticMatrix(const AnyType &destination,
                               const AnyType &source) {
    uint32 numberOfRows = destination.GetNumberOfElements(1u);
    uint32 numberOfColumns = destination.GetNumberOfElements(0u);
    TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();
    TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void *sourcePointer = source.GetDataPointer();
    void *destinationPointer = destination.GetDataPointer();

    bool ok = true;
    for (uint32 r = 0u; (r < numberOfRows); r++) {

        for (uint32 c = 0u; (c < numberOfColumns); c++) {
            char8* sourceArray = reinterpret_cast<char8 **>(sourcePointer)[r];
            char8* destinationArray = reinterpret_cast<char8 *>(destinationPointer);

            uint32 sourceElementByteSize = static_cast<uint32>(sourceDescriptor.numberOfBits) / 8u;
            uint32 destinationElementByteSize = static_cast<uint32>(destinationDescriptor.numberOfBits) / 8u;
            uint32 sourceIndex = c * sourceElementByteSize;
            uint32 destinationIndex = (r * numberOfColumns + c) * destinationElementByteSize;

            AnyType elementSource(source);
            elementSource.SetDataPointer(&sourceArray[sourceIndex]);
            AnyType elementDestination(destination);
            elementDestination.SetDataPointer(&destinationArray[destinationIndex]);

            if (!ScalarBasicTypeConvert(elementDestination, elementSource)) {
                ok = false;
            }
        }
    }
    return ok;
}

static bool HeapToHeapMatrix(const AnyType &destination,
                             const AnyType &source) {
    uint32 numberOfRows = destination.GetNumberOfElements(1u);
    uint32 numberOfColumns = destination.GetNumberOfElements(0u);
    TypeDescriptor sourceDescriptor = source.GetTypeDescriptor();
    TypeDescriptor destinationDescriptor = destination.GetTypeDescriptor();
    void *sourcePointer = source.GetDataPointer();
    void *destinationPointer = destination.GetDataPointer();

    bool ok = true;
    for (uint32 r = 0u; (r < numberOfRows); r++) {

        for (uint32 c = 0u; (c < numberOfColumns); c++) {
            char8* sourceArray = reinterpret_cast<char8 **>(sourcePointer)[r];
            char8* destinationArray = reinterpret_cast<char8 **>(destinationPointer)[r];

            uint32 sourceElementByteSize = static_cast<uint32>(sourceDescriptor.numberOfBits) / 8u;
            uint32 destinationElementByteSize = static_cast<uint32>(destinationDescriptor.numberOfBits) / 8u;
            uint32 sourceIndex = c * sourceElementByteSize;
            uint32 destinationIndex = c * destinationElementByteSize;

            AnyType elementSource(source);
            elementSource.SetDataPointer(&sourceArray[sourceIndex]);
            AnyType elementDestination(destination);
            elementDestination.SetDataPointer(&destinationArray[destinationIndex]);

            if (!ScalarBasicTypeConvert(elementDestination, elementSource)) {
                ok = false;
            }
        }
    }
    return ok;
}

/**
 * TODO
 */
static bool MatrixBasicTypeConvert(const AnyType &destination,
                                   const AnyType &source) {

    bool isSourceStatic = source.IsStaticDeclared();
    bool isDestinationStatic = destination.IsStaticDeclared();

    bool ok = true;

    if ((isSourceStatic) && (isDestinationStatic)) {
        ok = StaticToStaticMatrix(destination, source);
    }

    if ((isSourceStatic) && (!isDestinationStatic)) {
        ok = StaticToHeapMatrix(destination, source);
    }

    if ((!isSourceStatic) && (isDestinationStatic)) {
        ok = HeapToStaticMatrix(destination, source);
    }

    if ((!isSourceStatic) && (!isDestinationStatic)) {
        ok = HeapToHeapMatrix(destination, source);
    }

    return ok;
}

}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
bool TypeConvert(const AnyType &destination,
                 const AnyType &source) {
    bool ok = true;

    //Source and destination dimensions must be the same
    ok = (destination.GetNumberOfDimensions() == source.GetNumberOfDimensions());
    //The number of elements in all dimensions must be the same
    uint32 i;
    for (i = 0; ok && (i < 3u); i++) {
        ok = (destination.GetNumberOfElements(i) == source.GetNumberOfElements(i));
    }

    if (ok) {
        if (source.GetNumberOfDimensions() == 0u) {
            ok = ScalarBasicTypeConvert(destination, source);                    //PointBasicTypeConvert(destination, source);
        }
        else if (source.GetNumberOfDimensions() == 1u) {
            ok = VectorBasicTypeConvert(destination, source);
        }
        else if (source.GetNumberOfDimensions() == 2u) {
            ok = MatrixBasicTypeConvert(destination, source);
        }
    }

    return ok;
}
}
