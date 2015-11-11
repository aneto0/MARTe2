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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
/**
 * @brief BasicType conversion between two scalar types.
 * @param[out] destination the converted type.
 * @param[in] source the type to be converted.
 * @return true if the conversion is both types are indeed scalar and if the
 * conversion is successful.
 */
static bool ScalarBasicTypeConvert(const AnyType &destination,
                                   const AnyType &source) {
    return false;
}

/**
 * TODO
 */
static bool VectorBasicTypeConvert(const AnyType &destination,
                                   const AnyType &source) {
    uint32 copySize = 0u;
    uint32 totalCopySize = 0u;
    uint32 idx = 0u;
    uint32 numberOfElements = source.GetNumberOfElements(0u);

    //Check that the dimensions are correct
    bool ok = (source.GetNumberOfDimensions() == destination.GetNumberOfDimensions());
    if (ok) {
        ok = (numberOfElements == destination.GetNumberOfElements(0u));
    }
    if (ok) {
        //The deserialisation assumes that source dataPointer is an array of pointers to each individual element value.
        void **sourceArray = static_cast<void **>(source.GetDataPointer());

        for (idx = 0; ok && (idx < numberOfElements); idx++) {
            //Get the source element size (must treat strings and CCString specially...
            //From Stream
            if (source.GetTypeDescriptor().type == SString) {
                String *streamSrc = static_cast<String *>(source.GetDataPointer());
                streamSrc += idx;
                copySize = streamSrc->Size() + 1u;
            }
            //From CCString
            else if (source.GetTypeDescriptor().type == CCString) {
                const char8 *srcArray = static_cast<const char8 *>(sourceArray[idx]);
                copySize = (StringHelper::Length(srcArray) + 1u);
            }
            //From any other BasicType
            else {
                copySize = source.GetTypeDescriptor().numberOfBits / 8u;
            }

            ok = (copySize > 0u);

            if (ok) {
                //Do the actual conversion, again must treat strings and CCString specially...
                //From String
                if (source.GetTypeDescriptor().type == SString) {
                    String *streamSrc = static_cast<String *>(source.GetDataPointer());
                    streamSrc += idx;
                    const char8 *srcArray = streamSrc->Buffer();
                    if (destination.GetTypeDescriptor().type == SString) {
                        String *streamDest = static_cast<String *>(destination.GetDataPointer());
                        streamDest += idx;
                        ok = streamDest->Write(srcArray, copySize);
                    }
                    else if (destination.GetTypeDescriptor().type == CCString) {
                        //If the destination size is not sufficient this is going to have an ugly end...
                        char8 *destArray = static_cast<char8 *>(destination.GetDataPointer());
                        destArray += (idx * sizeof(char8 *));
                        ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                    }
                    else {
                        String sourceStreamString = srcArray;
                        ok = ScalarBasicTypeConvert(destination, sourceStreamString);
                    }
                }
                //From CCString
                else if (source.GetTypeDescriptor().type == CCString) {
                    const char8 *srcArray = static_cast<const char8 *>(sourceArray[idx]);
                    if (destination.GetTypeDescriptor().type == CCString) {
                        //If the destination size is not sufficient this is going to have an ugly end...
                        char8 *destArray = static_cast<char8 *>(destination.GetDataPointer());
                        destArray += (idx * sizeof(char8 *));
                        ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                    }
                    else if (destination.GetTypeDescriptor().type == SString) {
                        String *streamDest = static_cast<String *>(destination.GetDataPointer());
                        streamDest += idx;
                        ok = streamDest->Write(srcArray, copySize);
                    }
                    else {
                        AnyType sourceCCString(srcArray);
                        ok = ScalarBasicTypeConvert(destination, sourceCCString);
                    }
                }
                //From any other BasicType
                else {
                    void *srcArray = sourceArray[idx];
                    void *destArray = static_cast<char8 *>(destination.GetDataPointer()) + totalCopySize;
                    //Source and destination are the same. Easy.
                    if (source.GetTypeDescriptor() == destination.GetTypeDescriptor()) {
                        ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                    }
                    else {
                        AnyType sourceBasicType(source);
                        sourceBasicType.SetDataPointer(srcArray);
                        AnyType destinationBasicType(destination);
                        destinationBasicType.SetDataPointer(destArray);
                        ok = ScalarBasicTypeConvert(destinationBasicType, sourceBasicType);
                    }
                }
                totalCopySize += copySize;
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
    uint32 copySize = 0u;
    uint32 totalCopySize = 0u;

    //The deserialisation assumes that the source dataPointer is an array of pointers to the element values.
    void **sourceArray = static_cast<void **>(source.GetDataPointer());

    uint32 r = 0u;
    uint32 c = 0u;
    uint32 idx = 0u;
    uint32 numberOfRows = destination.GetNumberOfElements(1u);
    uint32 numberOfColumns = destination.GetNumberOfElements(0u);

    //Check that the dimensions are correct
    bool ok = (source.GetNumberOfDimensions() == destination.GetNumberOfDimensions());
    if (ok) {
        ok = (numberOfRows == source.GetNumberOfElements(1u));
    }
    if (ok) {
        ok = (numberOfColumns == source.GetNumberOfElements(0u));
    }
    if (ok) {
        for (r = 0; ok && (r < numberOfRows); r++) {
            for (c = 0; ok && (c < numberOfColumns); c++) {
                idx = c + r * numberOfColumns;
                //Compute the size
                //From String
                if ((source.GetTypeDescriptor().type == SString)) {
                    String *sourceStream = NULL;
                    if (source.IsStaticDeclared()) {
                        sourceStream = static_cast<String *>(source.GetDataPointer());
                        sourceStream += idx;
                    }
                    else {
                        sourceStream = &static_cast<String **>(source.GetDataPointer())[r][c];
                    }
                    copySize = sourceStream->Size() + 1u;
                }
                //From CCString
                else if (source.GetTypeDescriptor().type == CCString) {
                    const char8 *srcArray = static_cast<const char8 *>(sourceArray[idx]);
                    copySize = (StringHelper::Length(srcArray) + 1u);
                }
                //From any other BasicType
                else {
                    copySize = source.GetTypeDescriptor().numberOfBits / 8u;
                }

                ok = (copySize > 0u);

                if (ok) {
                    //Perform the actual conversion
                    //From String
                    if (source.GetTypeDescriptor().type == SString) {
                        String *sourceStream = NULL;
                        if (source.IsStaticDeclared()) {
                            sourceStream = static_cast<String *>(source.GetDataPointer());
                            sourceStream += idx;
                        }
                        else {
                            sourceStream = &static_cast<String **>(source.GetDataPointer())[r][c];
                        }
                        const char8 *srcArray = sourceStream->Buffer();
                        //To String
                        if (destination.GetTypeDescriptor().type == SString) {
                            String *destStream = NULL;
                            if (destination.IsStaticDeclared()) {
                                destStream = static_cast<String *>(destination.GetDataPointer());
                                destStream += idx;
                            }
                            else {
                                destStream = &static_cast<String **>(destination.GetDataPointer())[r][c];
                            }
                            destStream->Write(srcArray, copySize);
                        }
                        //To CCString
                        else if (destination.GetTypeDescriptor().type == CCString) {
                            char8 *destArray = NULL_PTR(char8 *);
                            if (destination.IsStaticDeclared()) {
                                destArray = static_cast<char8 **>(destination.GetDataPointer())[idx];
                            }
                            else {
                                destArray = static_cast<char8 ***>(destination.GetDataPointer())[r][c];
                            }
                            ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                        }
                        //To any other BasicType
                        else {
                            String sourceStreamString = srcArray;
                            ok = ScalarBasicTypeConvert(destination, sourceStreamString);
                        }
                    }
                    //From CCString
                    else if (source.GetTypeDescriptor().type == CCString) {
                        const char8 *srcArray = static_cast<const char8 *>(sourceArray[idx]);
                        //To CCString
                        if (destination.GetTypeDescriptor().type == CCString) {
                            char8 *destArray = NULL_PTR(char8 *);
                            if (destination.IsStaticDeclared()) {
                                destArray = static_cast<char8 **>(destination.GetDataPointer())[idx];
                            }
                            else {
                                destArray = static_cast<char8 ***>(destination.GetDataPointer())[r][c];
                            }
                            ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                        }
                        //To String
                        else if (destination.GetTypeDescriptor().type == SString) {
                            String *stream = NULL;
                            if (destination.IsStaticDeclared()) {
                                stream = static_cast<String *>(destination.GetDataPointer());
                                stream += idx;
                            }
                            else {
                                stream = &static_cast<String **>(destination.GetDataPointer())[r][c];
                            }
                            stream->Write(srcArray, copySize);
                        }
                        else {
                            AnyType sourceCCString(srcArray);
                            ok = ScalarBasicTypeConvert(destination, sourceCCString);
                        }
                    }
                    else {
                        void *srcArray = sourceArray[idx];
                        void *destArray = static_cast<char8 *>(destination.GetDataPointer()) + totalCopySize;
                        if (source.GetTypeDescriptor() == destination.GetTypeDescriptor()) {
                            ok = MemoryOperationsHelper::Copy(destArray, srcArray, copySize);
                        }
                        else {
                            AnyType sourceBasicType(source);
                            sourceBasicType.SetDataPointer(srcArray);
                            AnyType destinationBasicType(destination);
                            destinationBasicType.SetDataPointer(destArray);
                            ok = ScalarBasicTypeConvert(destinationBasicType, sourceBasicType);
                        }
                    }
                    totalCopySize += copySize;
                }
            }
        }
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
    uint32 copySize = 0u;

    //Source and destination dimensions must be the same
    ok = (destination.GetNumberOfDimensions() == source.GetNumberOfDimensions());
    //The number of elements in all dimensions must be the same
    uint32 i;
    for (i = 0; ok && (i < 3u); i++) {
        ok = (destination.GetNumberOfElements(i) == source.GetNumberOfElements(i));
    }

    if (ok) {
        if (source.GetNumberOfDimensions() == 0u) {
            //From Stream
            if (source.GetTypeDescriptor().type == SString) {
                String *stream = static_cast<String *>(source.GetDataPointer());
                char8 *srcString = static_cast<char8 *>(stream->BufferReference());
                copySize = stream->Size() + 1u;

                //To Stream
                if (destination.GetTypeDescriptor().type == SString) {
                    String *stream = static_cast<String *>(destination.GetDataPointer());
                    ok = stream->Write(srcString, copySize);
                } //To CCString
                else if (destination.GetTypeDescriptor().type == CCString) {
                    ok = MemoryOperationsHelper::Copy(destination.GetDataPointer(), source.GetDataPointer(), copySize);
                } //To another BasicType
                else {
                    ok = ScalarBasicTypeConvert(destination, source);
                }
            } //From CCString
            else if (source.GetTypeDescriptor().type == CCString) {
                char8 *srcString = static_cast<char8 *>(source.GetDataPointer());
                copySize = (StringHelper::Length(srcString) + 1u);

                //To Stream
                if (destination.GetTypeDescriptor().type == SString) {
                    String *stream = static_cast<String *>(destination.GetDataPointer());
                    ok = stream->Write(srcString, copySize);
                } //To CString
                else if (destination.GetTypeDescriptor().type == CCString) {
                    ok = MemoryOperationsHelper::Copy(destination.GetDataPointer(), source.GetDataPointer(), copySize);
                } //To another BasicType
                else {
                    String sourceString = srcString;
                    ok = ScalarBasicTypeConvert(destination, sourceString);
                }
            } //From a BasicType
            else { //Destination and source are the same
                if (source.GetTypeDescriptor() == destination.GetTypeDescriptor()) {
                    copySize = source.GetTypeDescriptor().numberOfBits / 8u;
                    ok = MemoryOperationsHelper::Copy(destination.GetDataPointer(), source.GetDataPointer(), copySize);
                }
                else {
                    ok = ScalarBasicTypeConvert(destination, source);
                }
            }
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

