/**
 * @file RealTimeDefaultData.cpp
 * @brief Source file for class RealTimeDefaultData
 * @date 22/02/2016
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
 * the class RealTimeDefaultData (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RealTimeDefaultData.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

RealTimeDefaultData::RealTimeDefaultData() {
}

bool RealTimeDefaultData::Verify() {
    bool ret = (path == "");

    // check myself
    if (!ret) {
        StreamString typePath = path + ".Type";
        RealTimeDataSource *dataSource = RealTimeDataSource::Instance();
        StreamString testType;
        if (dataSource->Read(typePath.Buffer(), testType)) {
            if (testType == type) {
                ret = true;
            }
        }
    }

    // here if the the self check goes fine or the path is empty
    if (ret) {
        // check the children!!!
        for (uint32 i = 0u; (i < Size()) && (ret); i++) {
            ReferenceT<RealTimeDefaultData> subnode = Get(i);
            ret = subnode->Verify();
        }
    }

    return ret;
}

bool RealTimeDefaultData::Initialise(StructuredDataI& data) {

    bool ret = ReferenceContainer::Initialise(data);

    if (ret) {
        ret = data.Read("Path", path);
    }
    if (ret) {
        ret = data.Read("Type", type);
    }
    if (ret) {
        ret = data.Read("Default", defaultValue);
    }
    if (ret) {
        ret = data.Read("IsFinalised", finalised);
    }
    return ret;
}

bool RealTimeDefaultData::MergeWithLocal(StructuredDataI & localData) {

    uint32 newItemsNumber = localData.GetNumberOfChildren();
    bool ret = ((finalised) && (newItemsNumber == 0u));

    if (!finalised) {
        ret = true;
        for (uint32 i = 0u; (i < newItemsNumber) & (ret); i++) {
            const char8 * newItemName = localData.GetChildName(i);
            for (uint32 j = 0u; j < Size(); j++) {
                Reference item = Get(i);
                if (StringHelper::Compare(item->GetName(), newItemName) == 0) {
                    ret = false;
                    //TODO Already exists a variable with the same name!!
                }
                else {
                    ReferenceT<RealTimeData> newItem;
                    newItem.Initialise(localData, false);
                    if (newItem.IsValid()) {
                        ret = Insert(newItem);
                    }
                    else{
                        ret=false;
                    }
                }
            }
        }
    }
    return ret;
}

const char8 *RealTimeDefaultData::GetPath() {
    return path.Buffer();
}

const char8 *RealTimeDefaultData::GetType() {
    return type.Buffer();
}

const char8 *RealTimeDefaultData::GetDefaultValue() {
    return defaultValue.Buffer();
}

}
