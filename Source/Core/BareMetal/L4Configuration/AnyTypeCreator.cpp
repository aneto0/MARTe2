/**
 * @file AnyTypeCreator.cpp
 * @brief Source file for class AnyTypeCreator
 * @date 07/12/2015
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
 * the class AnyTypeCreator (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AnyTypeCreator.h"
#include "TypeConversion.h"
#include "StringHelper.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/*lint -e9150 [MISRA C++ Rule 11-0-1]. Justification: Used a structure instead of a class. */
struct TypeCastInfo {
    TypeDescriptor typeDes;
    const char8 *castName;
};

static const TypeCastInfo castTypes[] = { { CharString, "string" }, { SignedInteger8Bit, "int8" }, { SignedInteger16Bit, "int16" }, { SignedInteger32Bit,
        "int32" }, { SignedInteger64Bit, "int64" }, { UnsignedInteger8Bit, "uint8" }, { UnsignedInteger16Bit, "uint16" }, { UnsignedInteger32Bit, "uint32" }, {
        UnsignedInteger64Bit, "uint64" }, { Float32Bit, "float32" }, { Float64Bit, "float64" }, { CharString, static_cast<const char8*>(NULL)}};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

AnyTypeCreator::AnyTypeCreator(const uint32 granularityIn) {
    typeIndex = 0u;
    memory = static_cast<StaticListHolder *>(NULL);
    granularity = granularityIn;
}

/*lint -e{1579} .Justification: The destructor calls an external function. */
/*lint -e{1551} Justification: Memory has to be freed in the destructor.
 * No exceptions should be thrown given that the memory is managed exclusively managed by this class.". */
AnyTypeCreator::~AnyTypeCreator() {
    CleanUp(0u);
}

void AnyTypeCreator::CleanUp(const uint32 granularityIn) {
    if (memory != NULL) {
        // in this case delete the string on heap
        if (memory->GetSize() > 0u) {
            if (castTypes[typeIndex].typeDes == CharString) {
                char8* stringElement = reinterpret_cast<char8 **>(memory->GetAllocatedMemory())[0];
                if (!HeapManager::Free(reinterpret_cast<void* &>(stringElement))) {
                    REPORT_ERROR(ErrorManagement::FatalError, "ReadMatrix: Failed HeapManager::Free()");
                }
            }
        }
        delete memory;
        typeIndex = 0u;
        memory = static_cast<StaticListHolder *>(NULL);
    }
    granularity=granularityIn;
}

AnyType AnyTypeCreator::Create(const uint8 nOfDimensions,
                               const uint32 dimensionSize[3]) const {

    AnyType element;
    bool ret = (memory != NULL);
    if (ret) {
        /*lint -e{613} .Justification: possible NULL memory is checked before entering here*/
        ret = (((dimensionSize[0] * dimensionSize[1]) * dimensionSize[2]) == (memory->GetSize()));
        if (ret) {
            uint32 dimCheck = 0u;
            for (uint32 i = 0u; i < 3u; i++) {
                if (dimensionSize[i] > 1u) {
                    dimCheck = (i + 1u);
                }
            }
            ret = (nOfDimensions >= dimCheck) && (nOfDimensions < 3u);
            if (ret) {

                if ((castTypes[typeIndex].typeDes.type == CCString) && (nOfDimensions == 0u)) {
                    /*lint -e{613} .Justification: possible NULL memory is checked before entering here*/
                    element = AnyType(castTypes[typeIndex].typeDes, static_cast<uint8>(0u), *static_cast<char8**>(memory->GetAllocatedMemory()));
                }
                else {
                    /*lint -e{613} .Justification: possible NULL memory is checked before entering here*/
                    element = AnyType(castTypes[typeIndex].typeDes, static_cast<uint8>(0u), memory->GetAllocatedMemory());
                }
                element.SetNumberOfDimensions(nOfDimensions);
                for (uint32 i = 0u; i < 3u; i++) {
                    element.SetNumberOfElements(i, dimensionSize[i]);
                }
            }
        }
    }

    return element;
}

bool AnyTypeCreator::Add(const char8 * const type,
                         const char8 * const data) {

    bool ret = false;

    // try to insert an element with a different size
    if (memory != NULL) {
        if(StringHelper::Compare(type, castTypes[typeIndex].castName) != 0) {
            REPORT_ERROR(ErrorManagement::FatalError, "ToType: Type mismatch!");
        }
    }
    else {
        typeIndex = 0u;
        while ((castTypes[typeIndex].castName != NULL) && (!ret)) {
            if (StringHelper::Compare(type, castTypes[typeIndex].castName) == 0) {
                ret = true;
            }
            else {
                typeIndex++;
            }
        }
    }

    if (!ret) {
        REPORT_ERROR(ErrorManagement::Warning, "ToType: Type not found; automatic cast to C-string");
    }

    ret = false;
    if (castTypes[typeIndex].typeDes == CharString) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(char8 *)), granularity);
        }
        uint32 size = StringHelper::Length(data) + 1u;
        char8 *cString = static_cast<char8 *>(HeapManager::Malloc(size));
        if (StringHelper::Copy(cString, data)) {
            ret = memory->Add(reinterpret_cast<void*>(&cString));
        }
    }
    else if (castTypes[typeIndex].typeDes == SignedInteger8Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(int8)), granularity);
        }
        int8 possibleInt8 = 0;
        if (TypeConvert(possibleInt8, data)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleInt8));
        }
    }
    else if (castTypes[typeIndex].typeDes == SignedInteger16Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(int16)), granularity);
        }
        int16 possibleInt16 = 0;
        if (TypeConvert(possibleInt16, data)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleInt16));
        }
    }
    else if (castTypes[typeIndex].typeDes == SignedInteger32Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(int32)), granularity);
        }
        int32 possibleInt32 = 0;
        if (TypeConvert(possibleInt32, data)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleInt32));
        }
    }
    else if (castTypes[typeIndex].typeDes == SignedInteger64Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(int64)), granularity);
        }
        int64 possibleInt64 = 0;
        if (TypeConvert(possibleInt64, data)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleInt64));
        }
    }
    else if (castTypes[typeIndex].typeDes == UnsignedInteger8Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(uint8)), granularity);
        }
        uint8 possibleUInt8 = 0u;
        if (TypeConvert(possibleUInt8, data)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleUInt8));
        }
    }
    else if (castTypes[typeIndex].typeDes == UnsignedInteger16Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(uint16)), granularity);
        }
        uint16 possibleUInt16 = 0u;
        if (TypeConvert(possibleUInt16, data)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleUInt16));
        }
    }
    else if (castTypes[typeIndex].typeDes == UnsignedInteger32Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(uint32)), granularity);
        }
        uint32 possibleUInt32 = 0u;
        if (TypeConvert(possibleUInt32, data)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleUInt32));
        }
    }
    else if (castTypes[typeIndex].typeDes == UnsignedInteger64Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(uint64)), granularity);
        }
        uint64 possibleUInt64 = 0u;
        if (TypeConvert(possibleUInt64, data)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleUInt64));
        }
    }
    else if (castTypes[typeIndex].typeDes == Float32Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(float32)), granularity);
        }
        float32 possibleFloat32 = 0.0F;
        if (TypeConvert(possibleFloat32, data)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleFloat32));
        }
    }
    else if (castTypes[typeIndex].typeDes == Float64Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(float64)), granularity);
        }
        float64 possibleFloat64 = 0.0;
        if (TypeConvert(possibleFloat64, data)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleFloat64));
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ToType: Invalid type in input!");
    }

    if (!ret) {
        REPORT_ERROR(ErrorManagement::FatalError, "ToType: Failed conversion to the specified type!");
    }

    return ret;
}

uint32 AnyTypeCreator::GetSize() const {
    return (memory == NULL)?(0u):(memory->GetSize());
}

uint32 AnyTypeCreator::GetGranularity() const {
    return granularity;
}

}
