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

extern bool StringToFloat(const char8 * const input,
                          float32 &number);

extern bool StringToFloat(const char8 * const input,
                          float64 &number);

bool StringToIntegerGeneric(const char8* source,
                            uint8 *dest,
                            const uint8 destBitSize,
                            const bool isSigned);

extern bool IntegerToFloatGeneric(uint8 *source,
                                  const uint8 sourceBitSize,
                                  float32 *dest,
                                  const uint8 destBitSize,
                                  const bool isSigned);

extern bool FloatToIntegerGeneric(float32 *source,
                                  const uint8 sourceBitSize,
                                  uint8 *dest,
                                  const uint8 destBitSize,
                                  const bool isSigned);

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
            if (destinationDescriptor.type == CArray) {
                String tempString;
                ret = tempString.PrintFormatted("%d", &source);
                if (ret) {
                    uint32 stringLength = static_cast<uint32>(tempString.Size()) + 1u;
                    uint32 arraySize = static_cast<uint32>(destinationDescriptor.numberOfBits) / 8u;
                    if (stringLength > arraySize) {
                        //TODO warning clip string
                        ret = StringHelper::CopyN(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer(), arraySize);
                        reinterpret_cast<char8 *>(destinationPointer)[arraySize] = '\0';
                    }
                    else {
                        ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer());
                    }
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
            if (destinationDescriptor.type == CArray) {
                String tempString;
                ret = tempString.PrintFormatted("%E", &source);
                if (ret) {
                    uint32 stringLength = static_cast<uint32>(tempString.Size()) + 1u;
                    uint32 arraySize = static_cast<uint32>(destinationDescriptor.numberOfBits) / 8u;
                    if (stringLength > arraySize) {
                        //TODO warning clip string
                        ret = StringHelper::CopyN(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer(), arraySize);
                        reinterpret_cast<char8 *>(destinationPointer)[arraySize] = '\0';
                    }
                    else {
                        ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), tempString.Buffer());
                    }
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

        if ((sourceDescriptor.type == CCString) || (sourceDescriptor.type == SString) || (sourceDescriptor.type == CArray)) {

            const char8* token = static_cast<const char8 *>(NULL);
            if(sourceDescriptor.type == CCString) {
                if(source.GetNumberOfDimensions() == 0u) {
                    token=reinterpret_cast<const char8*>(sourcePointer);
                }
                else {
                    token=*reinterpret_cast<const char8**>(sourcePointer);
                }
            }
            if(sourceDescriptor.type == CArray) {
                token=reinterpret_cast<const char8*>(sourcePointer);
            }
            if((sourceDescriptor.type == SString)) {
                token=(reinterpret_cast<String*>(sourcePointer))->Buffer();
            }
            uint32 tokenLength=StringHelper::Length(token);
            if(destinationDescriptor.type==SString) {
                String* tempString=reinterpret_cast<String*>(destinationPointer);
                ret=tempString->Write(token, tokenLength);
            }
            if (destinationDescriptor.type == CArray) {
                uint32 arraySize = static_cast<uint32>(destinationDescriptor.numberOfBits) / 8u;
                if (tokenLength > arraySize) {
                    //TODO warning clip string
                    ret = StringHelper::CopyN(reinterpret_cast<char8 *>(destinationPointer), token, arraySize);
                    reinterpret_cast<char8 *>(destinationPointer)[arraySize] = '\0';
                }
                else {
                    ret = StringHelper::Copy(reinterpret_cast<char8 *>(destinationPointer), token);
                }
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
        uint32 sourceElementByteSize = static_cast<uint32>(source.GetTypeDescriptor().numberOfBits) / 8u;
        uint32 destinationElementByteSize = static_cast<uint32>(destination.GetTypeDescriptor().numberOfBits) / 8u;
        uint32 sourceIndex = idx * sourceElementByteSize;
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

        char8* sourceArray = reinterpret_cast<char8 *>(sourcePointer);
        char8* destinationArray = reinterpret_cast<char8 *>(destinationPointer);

        uint32 sourceElementByteSize = static_cast<uint32>(sourceDescriptor.numberOfBits) / 8u;
        uint32 destinationElementByteSize = static_cast<uint32>(destinationDescriptor.numberOfBits) / 8u;

        uint32 sourceRowIndex = ((r * numberOfColumns) * sourceElementByteSize);
        AnyType sourceRow(sourceDescriptor, source.GetBitAddress(), &sourceArray[sourceRowIndex]);
        sourceRow.SetNumberOfDimensions(1u);
        sourceRow.SetNumberOfElements(0u, numberOfColumns);
        sourceRow.SetStaticDeclared(true);

        uint32 destinationRowIndex = ((r * numberOfColumns) * destinationElementByteSize);
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

        char8* sourceArray = reinterpret_cast<char8 *>(sourcePointer);
        char8* destinationArray = reinterpret_cast<char8 **>(destinationPointer)[r];

        uint32 sourceElementByteSize = static_cast<uint32>(sourceDescriptor.numberOfBits) / 8u;
        uint32 destinationElementByteSize = static_cast<uint32>(destinationDescriptor.numberOfBits) / 8u;

        uint32 sourceRowIndex = ((r * numberOfColumns) * sourceElementByteSize);
        AnyType sourceRow(sourceDescriptor, source.GetBitAddress(), &sourceArray[sourceRowIndex]);
        sourceRow.SetNumberOfDimensions(1u);
        sourceRow.SetNumberOfElements(0u, numberOfColumns);
        sourceRow.SetStaticDeclared(true);

        AnyType destinationRow(destinationDescriptor, destination.GetBitAddress(), destinationArray);
        destinationRow.SetNumberOfDimensions(1u);
        destinationRow.SetNumberOfElements(0u, numberOfColumns);
        destinationRow.SetStaticDeclared(false);

        if (!VectorBasicTypeConvert(destinationRow, sourceRow)) {
            ok = false;
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
        char8* sourceArray = reinterpret_cast<char8 **>(sourcePointer)[r];
        char8* destinationArray = reinterpret_cast<char8 *>(destinationPointer);

        AnyType sourceRow(sourceDescriptor, source.GetBitAddress(), sourceArray);
        sourceRow.SetNumberOfDimensions(1u);
        sourceRow.SetNumberOfElements(0u, numberOfColumns);
        sourceRow.SetStaticDeclared(false);

        uint32 destinationElementByteSize = static_cast<uint32>(destinationDescriptor.numberOfBits) / 8u;
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
        char8* sourceArray = reinterpret_cast<char8 **>(sourcePointer)[r];
        char8* destinationArray = reinterpret_cast<char8 **>(destinationPointer)[r];

        AnyType sourceRow(sourceDescriptor, source.GetBitAddress(), sourceArray);
        sourceRow.SetNumberOfDimensions(1u);
        sourceRow.SetNumberOfElements(0u, numberOfColumns);
        sourceRow.SetStaticDeclared(false);

        AnyType destinationRow(destinationDescriptor, destination.GetBitAddress(), destinationArray);
        destinationRow.SetNumberOfDimensions(1u);
        destinationRow.SetNumberOfElements(0u, numberOfColumns);
        destinationRow.SetStaticDeclared(false);

        if (!VectorBasicTypeConvert(destinationRow, sourceRow)) {
            ok = false;
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

    bool ok = false;

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

    bool ok = (!destination.GetTypeDescriptor().isConstant);
    if (ok) {

        //Source and destination dimensions must be the same
        ok = (destination.GetNumberOfDimensions() == source.GetNumberOfDimensions());
        //The number of elements in all dimensions must be the same
        for (uint32 i = 0; ok && (i < 3u); i++) {
            ok = (destination.GetNumberOfElements(i) == source.GetNumberOfElements(i));
        }

        if (ok) {
            if (source.GetNumberOfDimensions() == 0u) {
                ok = ScalarBasicTypeConvert(destination, source); //PointBasicTypeConvert(destination, source);
            }
            else if (source.GetNumberOfDimensions() == 1u) {
                ok = VectorBasicTypeConvert(destination, source);
            }
            else if (source.GetNumberOfDimensions() == 2u) {
                ok = MatrixBasicTypeConvert(destination, source);
            }
        }
    }

    return ok;
}
}
