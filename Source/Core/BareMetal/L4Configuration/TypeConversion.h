/**
 * @file TypeConversion.h
 * @brief Header file for class TypeConversion
 * @date Oct 28, 2015
 * @author aneto
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

 * @details This header file contains the declaration of the class TypeConversion
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TYPECONVERSION_H_
#define TYPECONVERSION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"
#include "Vector.h"
#include "Matrix.h"
#include "StreamI.h"
#include "StreamString.h"
#include "StringHelper.h"
#include "FormatDescriptor.h"
#include "MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

#if 0
static bool TypeConvert(const AnyType &destination,
        const AnyType &source) {
    bool ok = true;
    uint32 nOfDimensions = source.GetNumberOfDimensions();
    uint32 copySize = 0u;
    uint32 totalElements = 1u;

    uint32 i;
    for (i = 0u; i < nOfDimensions; i++) {
        if (source.GetNumberOfElements(i) != 0u) {
            totalElements *= source.GetNumberOfElements(i);
        }
    }

    //TODO must check if the dimensions and the number of elements are the same in source and destination...
    if (destination.GetTypeDescriptor() == source.GetTypeDescriptor()) {
        uint32 typeSize = source.GetTypeDescriptor().numberOfBits / 8u;

        if (source.GetTypeDescriptor().type == Stream) {
            copySize = source.GetNumberOfElements(2u);
        }
        else if ((nOfDimensions == 0u) && (source.GetTypeDescriptor().type == CCString)) {
            copySize = (StringHelper::Length(static_cast<const char *>(source.GetDataPointer())) + 1u);
        }
        else {
            copySize = typeSize * totalElements;
        }

        if (source.GetTypeDescriptor().type == Stream) {
            StreamI *stream = static_cast<StreamI *>(destination.GetDataPointer());
            ok = stream->Seek(0u);
            if (ok) {
                stream->Write(static_cast<char8 *>(source.GetDataPointer()), copySize);
            }
        }
        //TODO should we support tables of strings?
        else if ((nOfDimensions == 1u) && (source.GetTypeDescriptor().type == CCString)) {
            uint32 j;
            char **destArray = static_cast<char **>(destination.GetDataPointer());
            const char **sourceArray = static_cast<const char **>(source.GetDataPointer());
            //TODO .GetNumberOfElements(0u) should walk all the dimension
            for (j = 0; j < source.GetNumberOfElements(0u); j++) {
                copySize = (StringHelper::Length(sourceArray[j]) + 1u);
                if (!MemoryOperationsHelper::Copy(static_cast<void *>(destArray[j]), static_cast<const void *>(sourceArray[j]), copySize)) {
                    //TODO
                }
            }
        }
        else {
            if (!MemoryOperationsHelper::Copy(destination.GetDataPointer(), source.GetDataPointer(), copySize)) {
                ok = false;
            }
        }
    }
    else {
        ok = false;
    }

    return ok;
}
#endif

static bool BasicTypeConvert(const AnyType &destination,
                             const AnyType &source) {
    return false;
}

static bool TypeConvert(const AnyType &destination,
                        const AnyType &source) {
    bool ok = true;
    uint32 copySize = 0u;
    uint32 totalCopySize = 0u;

    //Source and destination dimensions must be the same
    ok = (destination.GetNumberOfDimensions() == source.GetNumberOfDimensions());
    //The number of elements in all dimensions must be the same
    uint32 i;
    for (i = 0; ok && (i < 3u); i++) {
        ok = (destination.GetNumberOfElements(i) == source.GetNumberOfElements(i));
    }

    if (ok) {
        if (source.GetNumberOfDimensions() == 0u) {
            if ((source.GetTypeDescriptor().type == StreamStringType) || (source.GetTypeDescriptor().type == CCString)) {
                copySize = (StringHelper::Length(static_cast<char *>(source.GetDataPointer())) + 1u);
            }
            else {
                copySize = source.GetTypeDescriptor().numberOfBits / 8u;
            }

            if (destination.GetTypeDescriptor().type == StreamStringType) {
                StreamString *stream = static_cast<StreamString *>(destination.GetDataPointer());
                stream->Write(static_cast<char8 *>(source.GetDataPointer()), copySize);
            }
            else {
                ok = MemoryOperationsHelper::Copy(destination.GetDataPointer(), source.GetDataPointer(), copySize);
            }
        }
        else if(source.GetNumberOfDimensions() == 1u) {
            VectorCore vecToDeser(destination);
            vecToDeser.SetDataPointer(destination.GetDataPointer());
            ok = vecToDeser.Deserialize(source);
        }
        else if(source.GetNumberOfDimensions() == 2u) {
            MatrixCore matToDeser(destination);
            matToDeser.SetDataPointer(destination.GetDataPointer());
            ok = matToDeser.Deserialize(source);
        }
    }
#if 0
        uint32 c = 0u;
        uint32 r = 0u;
        uint32 z = 0u;
        uint32 numberOfColumns = source.GetNumberOfElements(0u);
        uint32 numberOfRows = source.GetNumberOfElements(1u);
        uint32 numberOfZ = source.GetNumberOfElements(2u);
        if (numberOfColumns == 0u) {
            numberOfColumns = 1u;
        }
        if (numberOfRows == 0u) {
            numberOfRows = 1u;
        }
        if (numberOfZ == 0u) {
            numberOfZ = 1u;
        }

        void **copyArray = static_cast<void **>(source.GetDataPointer());

        for (z = 0u; ok && (z < numberOfZ); z++) {
            for (r = 0u; ok && (r < numberOfRows); r++) {
                for (c = 0u; ok && (c < numberOfColumns); c++) {

                    uint32 idx = c + (r * numberOfColumns) + (z * numberOfRows * numberOfColumns);

                    if ((source.GetTypeDescriptor().type == StreamStringType) || (source.GetTypeDescriptor().type == CCString)) {
                        const char *srcArray = static_cast<const char *>(copyArray[idx]);
                        copySize = (StringHelper::Length(srcArray) + 1u);
                    }
                    else {
                        copySize = source.GetTypeDescriptor().numberOfBits / 8u;
                    }

                    ok = (copySize > 0u);

                    if (ok) {
                        if (source.GetTypeDescriptor().type == StreamStringType) {
                            const char *srcArray = static_cast<const char *>(copyArray[idx]);
                            if (destination.GetTypeDescriptor().type == StreamStringType) {
                                if (destination.IsStaticDeclared() || (destination.GetNumberOfDimensions() < 2u)) {
                                    StreamString *streamDest = static_cast<StreamString *>(destination.GetDataPointer());
                                    streamDest += idx;
                                    ok = streamDest->Write(srcArray, copySize);
                                }
                                else {
                                    StreamString **streamDest = static_cast<StreamString **>(destination.GetDataPointer());
                                    ok = streamDest[r][c].Write(srcArray, copySize);
                                }
                            }
                            else if (destination.GetTypeDescriptor().type == CCString) {
                                if (destination.GetNumberOfDimensions() < 2u) {
                                    //If the destination size is not sufficient this is going to have an ugly end...
                                    char *destArray = static_cast<char *>(destination.GetDataPointer());
                                    destArray += (idx * sizeof(char *));
                                    ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                                }
                                else if (destination.IsStaticDeclared()) {
                                    char *destArray = reinterpret_cast<char **>(destination.GetDataPointer())[idx];
                                    ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                                }
                                else {
                                    char **destArray = static_cast<char **>(destination.GetDataPointer());
                                    ok = MemoryOperationsHelper::Copy(&destArray[r][c], srcArray, copySize);
                                }
                            }
                            else {
                                StreamString sourceStreamString = srcArray;
                                ok = BasicTypeConvert(destination, sourceStreamString);
                            }
                        }
                        else if (source.GetTypeDescriptor().type == CCString) {
                            const char *srcArray = static_cast<const char *>(copyArray[idx]);
                            if (destination.GetTypeDescriptor().type == CCString) {
                                if (destination.GetNumberOfDimensions() < 2u) {
                                    //If the destination size is not sufficient this is going to have an ugly end...
                                    char *destArray = static_cast<char *>(destination.GetDataPointer());
                                    destArray += (idx * sizeof(char *));
                                    ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                                }
                                else if (destination.IsStaticDeclared()) {
                                    char *destArray = reinterpret_cast<char **>(destination.GetDataPointer())[idx];
                                    ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                                }
                                else {
                                    char **destArray = static_cast<char **>(destination.GetDataPointer());
                                    ok = MemoryOperationsHelper::Copy(&destArray[r][c], srcArray, copySize);
                                    printf("-->%s\n", srcArray);
                                    printf("-->[%d][%d]=%s\n", r, c, destArray[r][c]);
                                }
                            }
                            else if (destination.GetTypeDescriptor().type == StreamStringType) {
                                if (destination.IsStaticDeclared() || (destination.GetNumberOfDimensions() < 2u)) {
                                    StreamString *streamDest = static_cast<StreamString *>(destination.GetDataPointer());
                                    streamDest += idx;
                                    ok = streamDest->Write(srcArray, copySize);
                                }
                                else {
                                    StreamString **streamDest = static_cast<StreamString **>(destination.GetDataPointer());
                                    ok = streamDest[r][c].Write(srcArray, copySize);
                                }
                            }
                            else {
                                AnyType sourceCCString(srcArray);
                                ok = BasicTypeConvert(destination, sourceCCString);
                            }
                        }
                        else {
                            void *srcArray = copyArray[idx];
                            void *destArray = static_cast<char *>(destination.GetDataPointer()) + totalCopySize;
                            if (source.GetTypeDescriptor() == destination.GetTypeDescriptor()) {
                                ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                            }
                            else {
                                AnyType sourceBasicType(source);
                                AnyType destinationBasicType(destination);
                                sourceBasicType.SetDataPointer(srcArray);
                                destinationBasicType.SetDataPointer(destArray);
                                ok = BasicTypeConvert(destinationBasicType, sourceBasicType);
                            }
                        }
                        totalCopySize += copySize;
                    }
                }
            }
        }
    }
#endif
        return ok;
    }

static bool TypeConvert(AnyType &destination,
        const AnyType& source,
        const FormatDescriptor &conversionFormat) {
    return false;
}

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TYPECONVERSION_H_ */

