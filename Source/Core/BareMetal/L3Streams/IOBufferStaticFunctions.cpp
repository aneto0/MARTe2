/**
 * @file IOBuffer.cpp
 * @brief Source file for class IOBuffer
 * @date 01/10/2015
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
 * the class IOBuffer (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#define DLL_API

#include "IOBufferPrivate.h"
#include "AdvancedErrorManagement.h"
#include "FormatDescriptor.h"
#include "BitSetToInteger.h"
#include "ClassRegistryDatabase.h"
#include "StructuredDataI.h"

namespace MARTe {



/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
/*                           Static implementations                          */
/*---------------------------------------------------------------------------*/





/**
 * @brief Prints the introspection related to the object in input.
 * @param[out] iobuff is the output stream buffer.
 * @param[in] parIn is the input.
 * @return true if the object is registered and introspectable, false otherwise.
 * @pre
 *   The object represented by \a parIn must be introspectable
 */
static bool PrintObjectIntrospection(IOBuffer & iobuff, const AnyType & parIn) {

    TypeDescriptor descriptor = parIn.GetTypeDescriptor();
    const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Peek(descriptor.structuredDataIdCode);

    bool ret = false;

    if (item != NULL) {
        const ClassProperties *properties = item->GetClassProperties();
        const char8* data = NULL_PTR(const char8*);
        // print the class name
        if (properties != NULL) {
            data = properties->GetName();
            AnyType printClassName[] = {data, "= {", voidAnyType};
            if (iobuff.PrintFormatted("\r\n%s %s\r\n", &printClassName[0])) {
                const Introspection *introspection = item->GetIntrospection();
                if (introspection != NULL) {
                    ret = true;
                    uint32 numberOfMembers = introspection->GetNumberOfMembers();
                    for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
                        IntrospectionEntry memberIntrospection = (*introspection)[i];
                        data = memberIntrospection.GetMemberName();
                        AnyType printMemberName[] = {data, "= {", voidAnyType};
                        if (!iobuff.PrintFormatted("    %s %s\r\n", &printMemberName[0])) {
                            ret = false;
                        }
                        if (ret) {
                            data = memberIntrospection.GetMemberTypeName();
                            AnyType printType[] = {"type =", data, voidAnyType};
                            if (!iobuff.PrintFormatted("        %s %s\r\n", &printType[0])) {
                                ret = false;
                            }
                        }
                        if (ret) {
                            data = memberIntrospection.GetMemberModifiers();
                            AnyType printModifiers[] = {"modifiers =", data, voidAnyType};
                            if (!iobuff.PrintFormatted("        %s \"%s\"\r\n", &printModifiers[0])) {
                                ret = false;
                            }
                        }
                        if (ret) {
                            data = memberIntrospection.GetMemberAttributes();
                            AnyType printAttributes[] = {"attributes =", data, voidAnyType};
                            if (!iobuff.PrintFormatted("        %s \"%s\"\r\n    }\r\n", &printAttributes[0])) {
                                ret = false;
                            }
                        }
                    }
                    AnyType printClose[] = {"}", voidAnyType};
                    if (!iobuff.PrintFormatted("%s\r\n", &printClose[0])) {
                        ret = false;
                    }
                }
                REPORT_ERROR(ErrorManagement::FatalError, "PrintObjectIntrospection: The object is not introspectable");
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "PrintObjectIntrospection: The object is not registered");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "PrintObjectIntrospection: The object is not registered");
    }
    return ret;
}

/**
 * @brief Prints a StructuredDataInterface.
 * @param[out] iobuff is the output stream buffer.
 * @param[in] structuredData is the input.
 * @return false in case of error in the StructuredDataI functions, true otherwise.
 */

static bool PrintStructuredDataInterface(IOBuffer &iobuff,
                                         StructuredDataI * const structuredData,
                                         uint32 nodeLevel = 0u) {
    bool ret = true;
    uint32 numberOfChildren = structuredData->GetNumberOfChildren();
    for (uint32 i = 0u; (i < numberOfChildren) && (ret); i++) {
        const char8 * childName = structuredData->GetChildName(i);

        AnyType toPrint = structuredData->GetType(childName);
        if (toPrint.GetDataPointer() != NULL) {
            uint32 j;
            for (j = 0u; (j < nodeLevel) && (ret); j++) {
                AnyType noneType = voidAnyType;
                ret = (iobuff.PrintFormatted("    ", &noneType));
            }
            AnyType printLeftSide[] = {childName, "= ", voidAnyType};
            ret = (iobuff.PrintFormatted("%s %s", &printLeftSide[0]));
            if (ret) {
                AnyType printLeaf[] = {toPrint, voidAnyType};
                ret = (iobuff.PrintFormatted("%#!\r\n", &printLeaf[0]));
            }
        }
        else {
            // a node
            if (structuredData->MoveRelative(childName)) {
                uint32 j;
                for (j = 0u; (j < nodeLevel) && (ret); j++) {
                    AnyType noneType = voidAnyType;
                    ret = (iobuff.PrintFormatted("    ", &noneType));
                }
                AnyType printLeftSide[] = {childName, "= {", voidAnyType};
                ret = (iobuff.PrintFormatted("%s %s\r\n", &printLeftSide[0]));
                if (ret) {
                    nodeLevel++;
                    ret = PrintStructuredDataInterface(iobuff, structuredData, nodeLevel);
                }
                if (ret) {
                    ret = (structuredData->MoveToAncestor(1u));
                    nodeLevel--;
                }
                if (ret) {
                    for (j = 0u; (j < nodeLevel) && (ret); j++) {
                        AnyType noneType = voidAnyType;
                        ret = (iobuff.PrintFormatted("    ", &noneType));
                    }
                    AnyType printClose[] = {"}", voidAnyType};
                    ret = (iobuff.PrintFormatted("%s\r\n", &printClose[0]));
                }
            }
        }
    }

    return ret;
}

/**
 * @brief Prints an object.
 * @param[out] iobuff is the output stream buffer.
 * @param[in] is the input.
 * @return false if the object is not introspectable, true otherwise.
 * @pre
 *   The object represented by parIn must be introspectable.
 */
static bool PrintObject(IOBuffer & iobuff, const AnyType & parIn) {

    char8* dataPointer = reinterpret_cast<char8 *>(parIn.GetDataPointer());
    TypeDescriptor descriptor = parIn.GetTypeDescriptor();
    const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Peek(descriptor.structuredDataIdCode);

    bool ret = false;

    if (item != NULL) {
        const ClassProperties *properties = item->GetClassProperties();
        const char8* data = NULL_PTR(const char8*);
        // print the class name
        if (properties != NULL) {
            data = properties->GetName();
            AnyType printClassName[] = {"Class =", data, voidAnyType};
            if (iobuff.PrintFormatted("\r\n%s %s\r\n", &printClassName[0])) {
                const Introspection *introspection = item->GetIntrospection();
                if (introspection != NULL) {
                    ret = true;
                    uint32 numberOfMembers = introspection->GetNumberOfMembers();
                    for (uint32 i = 0u; (i < numberOfMembers) && (ret); i++) {
                        IntrospectionEntry memberIntrospection = (*introspection)[i];
                        // the member name
                        data = memberIntrospection.GetMemberName();

                        AnyType printMemberName[] = {data, "= ", voidAnyType};
                        if (!iobuff.PrintFormatted("%s %s", &printMemberName[0])) {
                            ret = false;
                        }
                        if (ret) {
                            uint32 byteOffset = memberIntrospection.GetMemberByteOffset();
                            TypeDescriptor memberDescriptor = memberIntrospection.GetMemberTypeDescriptor();
                            bool isMemberStructured = memberDescriptor.isStructuredData;
                            if (isMemberStructured) {
                                AnyType printOpen[] = {"{", voidAnyType};
                                if (!iobuff.PrintFormatted("%s\r\n", &printOpen[0])) {
                                    ret = false;
                                }
                            }
                            if (ret) {
                                // create the member type
                                char8* memberPointer = &dataPointer[byteOffset];
                                AnyType member(memberDescriptor, 0u, memberPointer);

                                if (memberIntrospection.GetMemberPointerLevel() > 0u) {
                                    member = AnyType(*reinterpret_cast<void**>(memberPointer));
                                }

                                if (memberDescriptor == CharString) {
                                    if (memberIntrospection.GetNumberOfDimensions() == 0u) {
                                        member = AnyType(*reinterpret_cast<char8**>(memberPointer));
                                    }
                                }

                                for (uint32 j = 0u; j < 3u; j++) {
                                    member.SetNumberOfElements(j, memberIntrospection.GetNumberOfElements(j));
                                }
                                member.SetNumberOfDimensions(memberIntrospection.GetNumberOfDimensions());
                                ret = iobuff.PrintFormatted("%!\r\n", &member);
                            }
                            if (isMemberStructured) {
                                AnyType printClose[] = {"}", voidAnyType};
                                if (!iobuff.PrintFormatted("%s\r\n", &printClose[0])) {
                                    ret = false;
                                }
                            }
                        }
                    }
                }
                else {
                    REPORT_ERROR(ErrorManagement::FatalError, "PrintObjectIntrospection: The object is not introspectable");
                }
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "PrintObjectIntrospection: The object is not registered");
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "PrintObjectIntrospection: The object is not registered");
    }
    return ret;
}

/**
 * @brief Prints a generic scalar AnyType object on a buffer.
 * @param[out] iobuff is the stream buffer output.
 * @param[in] parIn is the generic object to be printed.
 * @param[in] fd specifies the desired printing format.
 */
static bool PrintToStreamScalar(IOBuffer & iobuff,
                                const AnyType & parIn,
                                const FormatDescriptor &fd) {

    bool ret = true;
    // void anytype
    AnyType par = parIn;
    void* dataPointer = par.GetVariablePointer();
    if (dataPointer != NULL) {
        //if the element is structured, the print is not supported.
        bool isStructured = (par.GetTypeDescriptor()).isStructuredData;
        if (isStructured) {
            if (fd.desiredAction == PrintInfo) {
                ret = PrintObjectIntrospection(iobuff, parIn);
            }
            else {
                if (fd.desiredAction != PrintAnything) {
                    if (fd.desiredAction != PrintStruct) {
                        REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: a struct will be printed");
                    }
                }
                ret = PrintObject(iobuff, parIn);
            }
        }
        else {
            if (((par.GetTypeDescriptor()).type) == UnsignedInteger) {
                if (fd.desiredAction == PrintInfo) {
                    const char8* infoName = "Unsigned Integer";
                    AnyType info = infoName;
                    FormatDescriptor newFD = fd;
                    newFD.desiredAction = PrintString;
                    ret = PrintToStreamScalar(iobuff, info, newFD);
                }
                else {
                    if (fd.desiredAction != PrintAnything) {
                        if (fd.desiredAction != PrintInteger) {
                            REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: an unsigned integer will be printed");
                        }
                    }
                    //native unsigned integer types.
                    if (par.GetBitAddress() == 0u) {
                        switch ((par.GetTypeDescriptor()).numberOfBits) {
                            case 8u: {
                                uint8 *data = static_cast<uint8 *>(dataPointer);
                                ret = IntegerToStream(iobuff, *data, fd);
                            }
                            break;
                            case 16u: {
                                uint16 *data = static_cast<uint16 *>(dataPointer);
                                ret = IntegerToStream(iobuff, *data, fd);
                            }
                            break;
                            case 32u: {
                                uint32 *data = static_cast<uint32 *>(dataPointer);
                                ret = IntegerToStream(iobuff, *data, fd);
                            }
                            break;
                            case 64u: {
                                uint64 *data = static_cast<uint64 *>(dataPointer);
                                ret = IntegerToStream(iobuff, *data, fd);
                            }
                            break;
                            default: {
                                // use native standard integer
                                uint32 *number = static_cast<uint32 *>(dataPointer);
                                // all the remaining cases here
                                uint8 nBits = static_cast<uint8>((par.GetTypeDescriptor()).numberOfBits);
                                ret = BitSetToStream(iobuff, number, par.GetBitAddress(), nBits, false, fd);
                            }
                        }
                    }
                    else {
                        // use native standard integer
                        uint32 *number = static_cast<uint32 *>(dataPointer);
                        // all the remaining cases here
                        uint8 nBits = static_cast<uint8>((par.GetTypeDescriptor()).numberOfBits);
                        ret = BitSetToStream(iobuff, number, par.GetBitAddress(), nBits, false, fd);
                    }
                }
            }

            if (((par.GetTypeDescriptor()).type) == SignedInteger) {
                if (fd.desiredAction == PrintInfo) {
                    const char8* infoName = "Signed Integer";
                    AnyType info = infoName;
                    FormatDescriptor newFD = fd;
                    newFD.desiredAction = PrintString;
                    ret = PrintToStreamScalar(iobuff, info, newFD);
                }
                else {
                    if (fd.desiredAction != PrintAnything) {
                        if (fd.desiredAction != PrintInteger) {
                            REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: a signed integer will be printed");
                        }
                    }
                    //native signed integer types.
                    if (par.GetBitAddress() == 0u) {
                        switch ((par.GetTypeDescriptor()).numberOfBits) {
                            case 8u: {
                                int8 *data = static_cast<int8 *>(dataPointer);
                                ret = IntegerToStream(iobuff, *data, fd);
                            }
                            break;
                            case 16u: {
                                int16 *data = static_cast<int16 *>(dataPointer);
                                ret = IntegerToStream(iobuff, *data, fd);
                            }
                            break;
                            case 32u: {
                                int32 *data = static_cast<int32 *>(dataPointer);
                                ret = IntegerToStream(iobuff, *data, fd);
                            }
                            break;
                            case 64u: {
                                int64 *data = static_cast<int64 *>(dataPointer);
                                ret = IntegerToStream(iobuff, *data, fd);
                            }
                            break;
                            default: {
                                // use native standard integer
                                uint32 *number = static_cast<uint32 *>(dataPointer);
                                uint8 nBits = static_cast<uint8>((par.GetTypeDescriptor()).numberOfBits);
                                // all the remaining cases here
                                ret = BitSetToStream(iobuff, number, par.GetBitAddress(), nBits, true, fd);
                            }
                        }
                    }
                    else {
                        // use native standard integer
                        uint32 *number = static_cast<uint32 *>(dataPointer);
                        uint8 nBits = static_cast<uint8>((par.GetTypeDescriptor()).numberOfBits);
                        // all the remaining cases here
                        ret = BitSetToStream(iobuff, number, par.GetBitAddress(), nBits, true, fd);
                    }
                }
            }
            if (((par.GetTypeDescriptor()).type) == Float) {
                if (fd.desiredAction == PrintInfo) {
                    const char8* infoName = "Float";
                    AnyType info = infoName;
                    FormatDescriptor newFD = fd;
                    newFD.desiredAction = PrintString;
                    ret = PrintToStreamScalar(iobuff, info, newFD);
                }
                else {
                    if (fd.desiredAction != PrintAnything) {
                        if (fd.desiredAction != PrintFloat) {
                            REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: a float number will be printed");
                        }
                    }
                    //native float32 types. Float 128 bit is not supported.
                    switch ((par.GetTypeDescriptor()).numberOfBits) {
                        case 32u: {
                            float32 *data = static_cast<float32 *>(dataPointer);
                            ret = FloatToStream(iobuff, *data, fd);
                        }
                        break;
                        case 64u: {
                            float64 *data = static_cast<float64 *>(dataPointer);
                            ret = FloatToStream(iobuff, *data, fd);
                        }
                        break;
                        case 128u: {
                            REPORT_ERROR(ErrorManagement::UnsupportedFeature, "IOBuffer: Unsupported 128 bit floats");
                            ret = false;
                        }
                        break;
                        default: {
                            //REPORT_ERROR(ParametersError,"non standard float32 size")
                            ret = false;
                        }
                    }
                }
            }

            //pointer type.
            if (((par.GetTypeDescriptor()).type) == Pointer) {
                if (fd.desiredAction == PrintInfo) {
                    const char8* infoName = "Pointer";
                    AnyType info = infoName;
                    FormatDescriptor newFD = fd;
                    newFD.desiredAction = PrintString;
                    ret = PrintToStreamScalar(iobuff, info, newFD);
                }
                else {
                    if (fd.desiredAction != PrintAnything) {
                        if (fd.desiredAction != PrintPointer) {
                            REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: a pointer will be printed");
                        }
                    }
                    TypeDescriptor newTypeDes(par.GetTypeDescriptor().isConstant, UnsignedInteger, par.GetTypeDescriptor().numberOfBits);
                    AnyType at(newTypeDes, par.GetBitAddress(), static_cast<void *>(&dataPointer));
                    FormatDescriptor newFD = fd;
                    newFD.binaryNotation = HexNotation;
                    newFD.desiredAction = PrintInteger;
                    ret = PrintToStreamScalar(iobuff, at, newFD);
                }
            }
            //const char8* string type.
            //if in the format descriptor is specified the hex notation (%p or %x)
            //print the value of the pointer.
            if (((par.GetTypeDescriptor()).type) == BT_CCString) {
                if (fd.desiredAction == PrintInfo) {
                    const char8* infoName = "Char String";
                    AnyType info = infoName;
                    FormatDescriptor newFD = fd;
                    newFD.desiredAction = PrintString;
                    ret = PrintToStreamScalar(iobuff, info, newFD);
                }
                else {
                    if (fd.binaryNotation == HexNotation) {

                        TypeDescriptor newTypeDes(par.GetTypeDescriptor().isConstant, UnsignedInteger, par.GetTypeDescriptor().numberOfBits);
                        AnyType at(newTypeDes, par.GetBitAddress(), static_cast<void *>(&dataPointer));
                        ret = PrintToStreamScalar(iobuff, at, fd);
                    }
                    else {
                        if (fd.desiredAction != PrintAnything) {
                            if (fd.desiredAction != PrintString) {
                                REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: a string will be printed");
                            }
                        }
                        const char8 *string = static_cast<const char8 *>(dataPointer);
                        bool addQuotesOnString = fd.fullNotation;
                        ret = PrintCCString(iobuff, string, fd, addQuotesOnString);
                    }
                }
            }

            if (((par.GetTypeDescriptor()).type) == CArray) {
                if (fd.desiredAction == PrintInfo) {
                    const char8* infoName = "Char Array";
                    AnyType info = infoName;
                    FormatDescriptor newFD = fd;
                    newFD.desiredAction = PrintString;
                    ret = PrintToStreamScalar(iobuff, info, newFD);
                }
                else {
                    if (fd.desiredAction != PrintAnything) {
                        if (fd.desiredAction != PrintString) {
                            REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: a string will be printed");
                        }
                    }
                    const char8 *string = static_cast<const char8 *>(dataPointer);
                    bool addQuotesOnString = fd.fullNotation;
                    ret = PrintCCString(iobuff, string, fd, addQuotesOnString);
                }
            }
            //general stream type.
            bool isStream=(((par.GetTypeDescriptor()).type) == Stream);
            bool isSString=(((par.GetTypeDescriptor()).type) == SString);
            if ((isStream) || (isSString)) {
                if (fd.desiredAction == PrintInfo) {
                    const char8* infoName = "Stream";
                    AnyType info = infoName;
                    FormatDescriptor newFD = fd;
                    newFD.desiredAction = PrintString;
                    ret = PrintToStreamScalar(iobuff, info, newFD);
                }
                else {
                    if (fd.desiredAction != PrintAnything) {
                        if (fd.desiredAction != PrintString) {
                            REPORT_ERROR(ErrorManagement::Warning, "IOBuffer: Type mismatch: a stream will be printed");
                        }
                    }
                    StreamI * stream = static_cast<StreamI *>(dataPointer);
                    bool addQuotesOnString = fd.fullNotation;
                    ret = PrintStream(iobuff, *stream, fd, addQuotesOnString);
                }
            }

            //StructuredDataInterface.
            if (((par.GetTypeDescriptor()).type) == StructuredDataNode) {
                StructuredDataI * structuredData = static_cast<StructuredDataI *>(dataPointer);
                ret = PrintStructuredDataInterface(iobuff, structuredData);
            }
        }
    }
    else {
        ret = false;
    }
    //REPORT_ERROR(UnsupportedError,"unsupported format")
    return ret;
}

/**
 * @Prints a vector.
 * @param[out] iobuff is the buffer output.
 * @param[in] parIn is the input.
 * @param[in] fd specifies the print format.
 * @return true if the print of all elements succeeds, false otherwise.
 */
static bool PrintToStreamVector(IOBuffer & iobuff,
                                const AnyType & parIn,
                                const FormatDescriptor &fd) {

    TypeDescriptor descriptor = parIn.GetTypeDescriptor();
    char8* dataPointer = static_cast<char8*>(parIn.GetDataPointer());
    uint32 numberOfElements = parIn.GetNumberOfElements(0u);
    uint32 elementSize = parIn.GetByteSize();
    FormatDescriptor newFD;
    bool ret = PrintCCString(iobuff, "{ ", newFD);

    for (uint32 i = 0u; (i < numberOfElements) && (ret); i++) {
        uint32 index = i * elementSize;
        char8* scalarPointer = &dataPointer[index];
        AnyType scalar(descriptor, parIn.GetBitAddress(), scalarPointer);

        // Consider the special case of c string
        if (descriptor == CharString) {
            scalar = AnyType(*reinterpret_cast<char8**>(scalarPointer));
        }

        if (descriptor.type == Pointer) {
            void *pointerValue = *reinterpret_cast<void**>(scalarPointer);
            scalar = AnyType(PointerType, parIn.GetBitAddress(), pointerValue);
        }

        bool isSourceCArray = (descriptor.type == CArray);
        bool isSourceStaticDeclared = parIn.IsStaticDeclared();

        // Consider the special case of matrix of characters (seen as a vector)
        if ((isSourceCArray) && (!isSourceStaticDeclared)) {
            scalarPointer = reinterpret_cast<char8 **>(dataPointer)[i];
            scalar.SetDataPointer(scalarPointer);
        }

        ret = PrintToStreamScalar(iobuff, scalar, fd);
        if (ret) {
            ret = PrintCCString(iobuff, " ", newFD);
        }
    }

    ret = PrintCCString(iobuff, "} ", newFD);

    return ret;
}

/**
 * @Prints a matrix.
 * @param[out] iobuff is the buffer output.
 * @param[in] parIn is the input.
 * @param[in] fd specifies the print format.
 * @return true if the print of all elements succeeds, false otherwise.
 */
static bool PrintToStreamMatrix(IOBuffer & iobuff,
                                const AnyType & parIn,
                                const FormatDescriptor &fd) {

    TypeDescriptor descriptor = parIn.GetTypeDescriptor();
    char8* dataPointer = static_cast<char8*>(parIn.GetDataPointer());
    uint32 numberOfRows = parIn.GetNumberOfElements(1u);
    uint32 numberOfColumns = parIn.GetNumberOfElements(0u);
    uint32 elementSize = parIn.GetByteSize();
    bool isStaticDeclared = parIn.IsStaticDeclared();
    FormatDescriptor newFD;
    bool ret = PrintCCString(iobuff, "{ ", newFD);
    for (uint32 i = 0u; (i < numberOfRows) && (ret); i++) {

        if (ret) {
            char8* vectorPointer = NULL_PTR(char8*);
            if (isStaticDeclared) {
                uint32 index = (i * numberOfColumns) * elementSize;
                vectorPointer = &dataPointer[index];
            }
            else {
                vectorPointer = reinterpret_cast<char8**>(dataPointer)[i];
            }
            if (vectorPointer != NULL) {
                AnyType vector(descriptor, parIn.GetBitAddress(), vectorPointer);
                vector.SetNumberOfDimensions(1u);
                vector.SetNumberOfElements(0u, numberOfColumns);
                vector.SetStaticDeclared(parIn.IsStaticDeclared());
                ret = PrintToStreamVector(iobuff, vector, fd);
            }
        }

    }
    ret = PrintCCString(iobuff, "} ", newFD);
    return ret;
}

/**
 * @brief Prints a generic scalar AnyType object on a buffer.
 * @param[out] iobuff is the stream buffer output.
 * @param[in] parIn is the generic object to be printed.
 * @param[in] fd specifies the desired printing format.
 */
static bool PrintToStream(IOBuffer & iobuff,
                          const AnyType & parIn,
                          const FormatDescriptor &fd) {

    bool ret = false;
    AnyType par = parIn;
    void* dataPointer = par.GetDataPointer();
    if (dataPointer != NULL) {
        if (parIn.GetNumberOfDimensions() == 2u) {
            ret = PrintToStreamMatrix(iobuff, parIn, fd);
        }
        else if (parIn.GetNumberOfDimensions() == 1u) {
            ret = PrintToStreamVector(iobuff, parIn, fd);
        }
        else if (parIn.GetNumberOfDimensions() == 0u) {
            ret = PrintToStreamScalar(iobuff, parIn, fd);
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "PrintToStream: Print of type with dimension > 2 not supported");
        }
    }
    return ret;
}
/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
#if 0
bool IOBuffer::GetToken(
						char8 * const outputBuffer,
                        const char8 * const terminator,
                        uint32 outputBufferSize,
                        char8 &saveTerminator,
                        const char8 * skipCharacters) {

    bool ret = true;
    bool quit = false;
// need space for trailing 0
    outputBufferSize--;

    if (skipCharacters == NULL) {
        skipCharacters = terminator;
    }

    uint32 tokenSize = 0u;
    while (!quit) {
        char8 c;
        if (!GetC(c)) {

            // 0 terminated string
            outputBuffer[tokenSize] = '\0';

            saveTerminator = '\0';

            //
            if (tokenSize == 0u) {
                ret = false;
                quit = true;
            }
            else {
                quit = true;
            }
        }
        else {

            bool isTerminator = (StringHelper::SearchChar(terminator, c) != NULL);
            bool isSkip = (StringHelper::SearchChar(skipCharacters, c) != NULL);
            if ((isTerminator) || (c == '\0')) {

                // quit only if some data was read, otw just skip separator block
                if ((tokenSize != 0u) || (!isSkip)) {
                    // 0 terminated string
                    outputBuffer[tokenSize] = '\0';

                    saveTerminator = c;

                    quit = true;
                }
            }
            //isTerminator is false here
            else {
                if (!isSkip) {

                    outputBuffer[tokenSize] = c;
                    tokenSize++;
                    if (tokenSize >= outputBufferSize) {
                        // 0 terminated string
                        outputBuffer[tokenSize] = '\0';

                        saveTerminator = c;

                        quit = true;
                    }
                }
            }
        }
    }

    return ret;
}
#endif



bool IOBuffer::GetToken(
    		IOBuffer &inputBuffer,
    		IOBuffer & outputBuffer,
            CCString  terminator,
            char8 &saveTerminator,
            CCString skipCharacters){


    if (skipCharacters == NULL) {
        skipCharacters = terminator;
    }

    bool ret = true;
    bool quit = false;
    uint32 tokenSize = 0u;
    while (!quit) {
        char8 c;
        if (!inputBuffer.GetC(c)) {

            saveTerminator = '\0';

            //
            if (tokenSize == 0u) {
                ret = false;
                quit = true;
            }
            else {
                quit = true;
            }
        }
        else {

            bool isTerminator = (StringHelper::SearchChar(terminator, c) != NULL);
            bool isSkip = (StringHelper::SearchChar(skipCharacters, c) != NULL);
            if ((isTerminator) || (c == '\0')) {
                // quit only if some data was read, otw just skip separator block
                if ((tokenSize != 0u) || (!isSkip)) {

                    saveTerminator = c;

                    quit = true;
                }
            }
            else {
                if (!isSkip) {
                    if (!outputBuffer.PutC(c)) {
                        ret = false;
                        quit = true;
                    }
                    tokenSize++;
                }
            }
        }
    }

    return ret;
}

bool IOBuffer::SkipTokens(IOBuffer &iob, uint32 count, CCString terminator){

    bool ret = true;
    uint32 tokenSize = 0u;
    while (count > 0u) {
        char8 c;
        if (!iob.GetC(c)) {
            ret = false;
            //quit
            count = 0u;
        }
        else {
            //
            if ((StringHelper::SearchChar(terminator, c) != NULL) || (c == '\0')) {
                // quit only if some data was read, otherwise just skip separator block
                if (tokenSize != 0u) {
                    tokenSize = 0u;
                    count--;
                }
            }
            else {
                tokenSize++;
            }
        }
    }

    return ret;
}

bool IOBuffer::PrintFormatted(IOBuffer &iob, CCString format, const AnyType pars[]){

    bool ret = true;
    bool quit = false;
    // indicates active parameter
    int32 parsIndex = 0;
    // checks silly parameter
    if (format != NULL) {

        // loops through parameters
        while (!quit) {
            // scans for % and in the meantime prints what it encounters
            while ((format[0] != '\0') && (format[0] != '%')) {
                if (!iob.PutC(format[0])) {
                    ret = false;
                    quit = true;
                }
                format = &format[1];
            }
            if (ret) {

                // end of format
                if (format[0] == '\0') {
                    quit = true;
                }
                else {

                    // consume %
                    format = &format[1];

                    // if not end then %
                    // keep on parsing format to build a FormatDescriptor
                    FormatDescriptor fd;
                    if (!fd.InitialiseFromString(format)) {
                        ret = false;
                        quit = true;
                    }
                    else {

                        // if void simply skip and continue
                        if (!pars[parsIndex].IsVoid()) {
                            // use it to process parameters
                            if (!PrintToStream(iob, pars[parsIndex], fd)) {
                                ret = false;
                                quit = true;
                            }
                            parsIndex++;
                        }
                    }
                }
            }
        }
    }
    else {
        ret = false;
    }
    // never comes here!
    return ret;
}


}
