/**
 * @file RealTimeDataSource.cpp
 * @brief Source file for class RealTimeDataSource
 * @date 09/mar/2016
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
 * the class RealTimeDataSource (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDataSource.h"
#include "ReferenceContainerFilterObjectName.h"
#include "RealTimeDataDefContainer.h"
#include "AdvancedErrorManagement.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

static bool AllocatePrivate(ReferenceT<ReferenceContainer> container,
                            MemoryArea &memory) {
    bool ret = true;
    uint32 numberOfNodes = container->Size();
    for (uint32 i = 0u; (i < numberOfNodes) && (ret); i++) {
        ReferenceT<ReferenceContainer> subContainer = container->Get(i);
        ret = subContainer.IsValid();
        if (ret) {
            ReferenceT<BasicRealTimeDataSourceDef> def = subContainer;
            if (def.IsValid()) {
                ret = def->Allocate(memory);
            }
            else {
                ret = AllocatePrivate(subContainer, memory);
            }
        }
        else{
            REPORT_ERROR(ErrorManagement::FatalError, "RealTimeDataSource must contain a ReferenceContainer");
        }
    }
    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

RealTimeDataSource::RealTimeDataSource() :
        ReferenceContainer() {
}

bool RealTimeDataSource::Allocate() {
    return AllocatePrivate(ReferenceT<ReferenceContainer>(this), memory);
}

bool RealTimeDataSource::Initialise(StructuredDataI & data) {
    bool ret = ReferenceContainer::Initialise(data);
    if (ret) {
        if (data.Read("HeapName", heapName)) {
            memory.SetHeapName(heapName.Buffer());
        }
    }
    return ret;
}

CLASS_REGISTER(RealTimeDataSource, "1.0")

}
