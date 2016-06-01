/**
 * @file DataSourceI.cpp
 * @brief Source file for class DataSourceI
 * @date 18/04/2016
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
 * the class DataSourceI (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "ConfigurationDatabase.h"
#include <DataSourceI.h>
#include <GAMDataSource.h>
#include "GAM.h"
#include "GAMSignalsContainer.h"
#include "MemoryMapInputReader.h"
#include "MemoryMapInputReader.h"
#include "MemoryMapOutputWriter.h"
#include "ReferenceContainerFilterObjectName.h"
#include "StandardParser.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static bool Allocate(ReferenceT<DataSourceSignalI> dataSourceSignal,
                     MemoryArea &memory) {
    bool ret = dataSourceSignal.IsValid();
    if (ret) {
        TypeDescriptor typeDes = TypeDescriptor::GetTypeDescriptorFromTypeName(dataSourceSignal->GetType());
        uint32 varSize = 0u;
        // structured type
        if (typeDes == InvalidType) {
            const ClassRegistryItem *item = ClassRegistryDatabase::Instance()->Find(dataSourceSignal->GetType());
            ret = (item != NULL);
            if (ret) {
                /*lint -e{613} NULL pointer checking done before entering here */
                const ClassProperties *properties = item->GetClassProperties();
                ret = (properties != NULL);
                if (ret) {
                    varSize = properties->GetSize();
                }
                else {
                    REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The type %s does not provide ClassProperties", dataSourceSignal->GetType())
                }
            }
            else {
                REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, "The type %s is not registered", dataSourceSignal->GetType())
            }
        }
        // basic type
        else {
            varSize = (static_cast<uint32>(typeDes.numberOfBits) + 7u) / 8u;
            // consider the multi - dimensional
            varSize *= dataSourceSignal->GetNumberOfElements();
        }

        // consider the number of samples per cycle
        varSize *= dataSourceSignal->GetNumberOfSamples();

        // allocate the memory
        if (ret) {
            /*lint -e{613} NULL pointer checking done before entering here */
            //TODO likely that these offsets have to be set in DataSourceSignalI
            uint32 offset = 0;
            ret = memory.Add(varSize, offset);
            //ret = memory->Add(varSize, dataSourceSignal->bufferPtrOffset[0]);
            if (ret) {
                /*lint -e{613} NULL pointer checking done before entering here */
                ret = memory.Add(varSize, offset);
                //ret = memory->Add(varSize, bufferPtrOffset[1]);
            }
        }
    }
    return ret;
}

static bool AllocatePrivate(ReferenceT<ReferenceContainer> container,
                            MemoryArea &memory) {
    bool ret = true;
    uint32 numberOfNodes = container->Size();
    for (uint32 i = 0u; (i < numberOfNodes) && (ret); i++) {
        ReferenceT<ReferenceContainer> subContainer = container->Get(i);
        ret = subContainer.IsValid();
        if (ret) {
            ReferenceT<DataSourceSignalI> def = subContainer;
            if (def.IsValid()) {
                ret = Allocate(def, memory);
            }
            else {
                ret = AllocatePrivate(subContainer, memory);
            }
        }
        else {
            REPORT_ERROR(ErrorManagement::FatalError, "DataSourceI must contain an object inheriting from ReferenceContainer");
        }
    }
    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

DataSourceI::DataSourceI() :
        ReferenceContainer() {
}

bool DataSourceI::Allocate() {
    return AllocatePrivate(ReferenceT<ReferenceContainer>(this), memory);
}

bool DataSourceI::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        if (data.Read("HeapName", heapName)) {
            memory.SetHeapName(heapName.Buffer());
        }
    }
    return ret;
}

}
