/**
 * @file SlkAction.cpp
 * @brief Source file for class SlkAction
 * @date 04/dic/2015
 * @author pc
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
 * the class SlkAction (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SlkAction.h"
#include "AnyType.h"
#include "SlkParse.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
using namespace MARTe;

/**
 * @file Parser.cpp
 * @brief Source file for class Parser
 * @date 25/11/2015
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
 * the class Parser (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Parser.h"
#include "StaticListHolder.h"
#include "TypeConversion.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

struct TypeCastInfo {
    TypeDescriptor typeDes;
    const char8 *castName;
};

static const TypeCastInfo castTypes[] = { { CharString, "string" }, { SignedInteger8Bit, "int8" }, { SignedInteger16Bit, "int16" }, { SignedInteger32Bit,
        "int32" }, { SignedInteger64Bit, "int64" }, { UnsignedInteger8Bit, "uint8" }, { UnsignedInteger16Bit, "uint16" }, { UnsignedInteger32Bit, "uint32" }, {
        UnsignedInteger64Bit, "uint64" }, { Float32Bit, "float32" }, { Float64Bit, "float64" }, { CharString, static_cast<const char8*>(NULL) } };

static void PrintErrorOnStream(const char8 * const format,
                               const uint32 lineNumber,
                               BufferedStreamI * const err) {
    if (err != NULL) {
        if (!err->Printf(format, lineNumber)) {
            REPORT_ERROR(ErrorManagement::FatalError, "PrintErrorOnStream: Failed Printf() on error stream");
        }
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, format, lineNumber);
    }
}

static void SetType(const uint32 typeIndex,
                    void * const dataPointer,
                    AnyType &element,
                    const uint8 nOfDimensions) {

    if ((castTypes[typeIndex].typeDes.type == CCString) && (nOfDimensions == 0u)) {
        element = AnyType(castTypes[typeIndex].typeDes, static_cast<uint8>(0u), *static_cast<char8**>(dataPointer));
    }
    else {
        element = AnyType(castTypes[typeIndex].typeDes, static_cast<uint8>(0u), dataPointer);
    }
    element.SetNumberOfDimensions(nOfDimensions);
}

static bool ToType(const char8 * const tokenBuffer,
                   const uint32 typeIndex,
                   const uint32 granularity,
                   StaticListHolder *&memory) {

    bool ret = false;
    if (castTypes[typeIndex].typeDes == CharString) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(char8 *)), granularity);
        }
        uint32 size = StringHelper::Length(tokenBuffer) + 1u;
        char8 *cString = static_cast<char8 *>(HeapManager::Malloc(size));
        if (StringHelper::Copy(cString, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&cString));
        }
    }
    if (castTypes[typeIndex].typeDes == SignedInteger8Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(int8)), granularity);
        }
        int8 possibleInt8 = 0;
        if (TypeConvert(possibleInt8, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleInt8));
        }
    }
    if (castTypes[typeIndex].typeDes == SignedInteger16Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(int16)), granularity);
        }
        int16 possibleInt16 = 0;
        if (TypeConvert(possibleInt16, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleInt16));
        }
    }
    if (castTypes[typeIndex].typeDes == SignedInteger32Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(int32)), granularity);
        }
        int32 possibleInt32 = 0;
        if (TypeConvert(possibleInt32, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleInt32));
        }
    }
    if (castTypes[typeIndex].typeDes == SignedInteger64Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(int64)), granularity);
        }
        int64 possibleInt64 = 0;
        if (TypeConvert(possibleInt64, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleInt64));
        }
    }
    if (castTypes[typeIndex].typeDes == UnsignedInteger8Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(uint8)), granularity);
        }
        uint8 possibleUInt8 = 0u;
        if (TypeConvert(possibleUInt8, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleUInt8));
        }
    }
    if (castTypes[typeIndex].typeDes == UnsignedInteger16Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(uint16)), granularity);
        }
        uint16 possibleUInt16 = 0u;
        if (TypeConvert(possibleUInt16, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleUInt16));
        }
    }
    if (castTypes[typeIndex].typeDes == UnsignedInteger32Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(uint32)), granularity);
        }
        uint32 possibleUInt32 = 0u;
        if (TypeConvert(possibleUInt32, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleUInt32));
        }
    }
    if (castTypes[typeIndex].typeDes == UnsignedInteger64Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(uint64)), granularity);
        }
        uint64 possibleUInt64 = 0u;
        if (TypeConvert(possibleUInt64, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleUInt64));
        }
    }
    if (castTypes[typeIndex].typeDes == Float32Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(float32)), granularity);
        }
        float32 possibleFloat32 = 0.0F;
        if (TypeConvert(possibleFloat32, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleFloat32));
        }
    }
    if (castTypes[typeIndex].typeDes == Float64Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(float64)), granularity);
        }
        float64 possibleFloat64 = 0.0;
        if (TypeConvert(possibleFloat64, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleFloat64));
        }
    }

    if (!ret) {
        delete memory;
        memory = static_cast<StaticListHolder*>(NULL);
    }

    return ret;
}

SlkAction::SlkAction(StreamI &stream,
                     ConfigurationDatabase &databaseIn,
                     const char8* terminals,
                     const char8* separators,
                     BufferedStreamI * const err) :
        parseError(),
        token(stream, terminals, separators) {
    typeIndex = 0u;
    numberOfColumns = 0u;
    firstNumberOfColumns = 0u;
    numberOfRows = 0u;
    memory = static_cast<StaticListHolder*>(NULL);
    database = &databaseIn;
    initialize_table();
}

void SlkAction::predict(uint16 a) {

}

void SlkAction::GetTypeCast() {

    typeIndex = 0u;
    bool found = false;
    if (token.get() == STRING_TOKEN) {
        while ((castTypes[typeIndex].castName != NULL) && (!found)) {
            if (StringHelper::Compare(token.GetTokenData(), castTypes[typeIndex].castName) == 0) {
                found = true;
            }
            else {
                typeIndex++;
            }
        }
        if (!found) {
            typeIndex = 0u;
        }
    }
}

void SlkAction::BlockEnd() {
    printf("\nEnd the block!\n");
    database->MoveToAncestor(1u);
}

void SlkAction::CreateNode() {
    printf("\nCreating node... %s\n", token.GetTokenData());
    database->CreateRelative(token.GetTokenData());

}

void SlkAction::AddLeaf() {
    printf("\nAdding leaf... %s\n", nodeName.Buffer());
    uint32 nDimensions = 0u;
    // use numberOfRows and numberOfColumns as dimensions # elements
    // a matrix!
    if (numberOfRows > 0u) {
        nDimensions = 2u;
    }
    // a vector!
    else if (firstNumberOfColumns > 1u) {
        numberOfColumns = firstNumberOfColumns;
        numberOfRows = 1u;
        nDimensions = 1u;
    }
    // a scalar
    else {
        numberOfRows = 1u;
        numberOfColumns = firstNumberOfColumns;
    }

    printf("\nnDim=%d nRows=%d nCols=%d\n", nDimensions, numberOfRows, numberOfColumns);
    AnyType element;
    /*lint -e{613} . Justification: if (memory==NULL) ---> (ret==false) */
    printf("\n%s\n", *(const char8**)memory->GetAllocatedMemory());
    SetType(typeIndex, memory->GetAllocatedMemory(), element, nDimensions);
    element.SetNumberOfElements(0u, numberOfColumns);
    element.SetNumberOfElements(1u, numberOfRows);
    bool ret = database->Write(nodeName.Buffer(), element);

    typeIndex = 0u;
    numberOfColumns = 0u;
    firstNumberOfColumns = 0u;
    numberOfRows = 0u;
    if (memory != NULL) {
        if (castTypes[typeIndex].typeDes == CharString) {
            uint32 nElements = memory->GetSize();
            for (uint32 i = 0u; i < nElements; i++) {
                char8 * elementToFree = static_cast<char8 **>(memory->GetAllocatedMemory())[i];
                if (!HeapManager::Free(reinterpret_cast<void*&>(elementToFree))) {
                    //  TODO
                }
            }
        }
        delete memory;
        memory = static_cast<StaticListHolder*>(NULL);
    }
}

void SlkAction::GetNodeName() {
    printf("\nGetting node name %s\n", token.GetTokenData());
    nodeName = token.GetTokenData();
}

void SlkAction::AddScalar() {
    printf("\nAdding a scalar %s\n",token.GetTokenData());
    bool ret = ToType(token.GetTokenData(), typeIndex, 1u, memory);

    if (ret) {
        firstNumberOfColumns++;
    }
    else {
        printf("\nAdd scalar error!\n");
        parseError.SetError();
    }

}

void SlkAction::CreateClassLeaf() {
    printf("\nAdd The class name!\n");
    // Aggiungi un elemento alla memoria
    bool ret = ToType(token.GetTokenData(), 0u, 1u, memory);
    if (ret) {
        AnyType element;
        /*lint -e{613} . Justification: if (memory==NULL) ---> (ret==false) */
        SetType(typeIndex, memory->GetAllocatedMemory(), element, 0u);
        ret = database->Write("ClassName", element);
    }
    if (memory != NULL) {
        char8 * elementToFree = static_cast<char8 **>(memory->GetAllocatedMemory())[0];
        if (!HeapManager::Free(reinterpret_cast<void*&>(elementToFree))) {
            //  TODO
        }
        delete memory;
        memory = static_cast<StaticListHolder*>(NULL);
    }
}

void SlkAction::EndRow() {
    printf("\nEnd Row\n");
    if (numberOfColumns == 0u) {
        numberOfColumns = firstNumberOfColumns;
    }
    else {
        if (numberOfColumns != firstNumberOfColumns) {
            // TODO malformed matrix
            parseError.SetError();
        }
    }
    firstNumberOfColumns = 0u;
    numberOfRows++;
}

void SlkAction::End() {
    database->MoveToRoot();
}

void SlkAction::Parse() {

    while (!parseError.IsError()) {
        SlkParse(*this, token, parseError, 0u);
    }

}

