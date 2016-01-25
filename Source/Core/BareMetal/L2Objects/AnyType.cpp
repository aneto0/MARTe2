/**
 * @file AnyType.cpp
 * @brief Source file for class AnyType
 * @date Nov 12, 2015
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

 * @details This source file contains the definition of all the methods for
 * the class AnyType (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AnyType.h"
#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {
AnyType::AnyType(Object &obj) {
    Init();
    dataPointer = static_cast<void *>(&obj);
    bitAddress = 0u;
/*
    ClassRegistryDatabase *classDatabase = ClassRegistryDatabase::Instance();
    const char8 *className = obj.GetClassProperties()->GetName();
    const ClassRegistryItem *classItem = classDatabase->Find(className);*/

    ClassRegistryDatabase *classDatabase = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *classItem = classDatabase->FindTypeIdName(typeid(obj).name());

    if (classItem != NULL_PTR(ClassRegistryItem *)) {
        dataDescriptor.isStructuredData = true;
        dataDescriptor.isConstant = false;
        dataDescriptor.structuredDataIdCode = static_cast<uint14>(classItem->GetClassProperties()->GetUniqueIdentifier());
    }
}

AnyType::AnyType(const Object &obj) {
    Init();
    dataPointer = static_cast<void *>(const_cast<Object *>(&obj));
    bitAddress = 0u;
/*
    ClassRegistryDatabase *classDatabase = ClassRegistryDatabase::Instance();
    const char8 *className = obj.GetClassProperties()->GetName();
    const ClassRegistryItem *classItem = classDatabase->Find(className);*/

    ClassRegistryDatabase *classDatabase = ClassRegistryDatabase::Instance();
    const ClassRegistryItem *classItem = classDatabase->FindTypeIdName(typeid(obj).name());
    if (classItem != NULL_PTR(ClassRegistryItem *)) {
        dataDescriptor.isStructuredData = true;
        dataDescriptor.isConstant = true;
        dataDescriptor.structuredDataIdCode = static_cast<uint14>(classItem->GetClassProperties()->GetUniqueIdentifier());
    }
}



}

