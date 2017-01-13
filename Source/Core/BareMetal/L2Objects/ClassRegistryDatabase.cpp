/**
 * @file ClassRegistryDatabase.cpp
 * @brief Source file for class ClassRegistryDatabase
 * @date 04/08/2015
 * @author Andre Neto
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
 *
 * @details This source file contains the definition of all the methods for
 * the class ClassRegistryDatabase (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassRegistryItem.h"
#include "HeapManager.h"
#include "Object.h"
#include "DynamicCString.h"
#include "StringHelper.h"
#include "MemoryOperationsHelper.h"
#include "LoadableLibrary.h"
#include "ClassRegistryDatabase.h"
#include "ErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ClassRegistryDatabase *ClassRegistryDatabase::Instance() {
    static ClassRegistryDatabase *instance = NULL_PTR(ClassRegistryDatabase *);
    if (instance == NULL_PTR(ClassRegistryDatabase *)) {
        instance = new ClassRegistryDatabase();
        GlobalObjectsDatabase::Instance()->Add(instance, NUMBER_OF_GLOBAL_OBJECTS - 2u);
    }
    return instance;
}

ClassRegistryDatabase::ClassRegistryDatabase() {

}

ClassRegistryDatabase::~ClassRegistryDatabase() {

}

void ClassRegistryDatabase::Add(ClassRegistryItem * const p) {
    if (p != NULL) {

        if (!Lock()) {
            REPORT_ERROR(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
        }

        // add at the end
        classDatabase.ListAdd(p);

        UnLock();
    }
}

ClassRegistryItem *ClassRegistryDatabase::Find(CCString className) {
    ClassRegistryItem *registryItem = NULL_PTR(ClassRegistryItem *);
    if (!Lock()) {
        REPORT_ERROR(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
    }


    DynamicCString dllName;

//    const uint32 maxSize = 129u;
//    char8 dllName[maxSize];
//    dllName[0] = '\0';
    bool found = false;

    //Check for the string pattern dllName::className
    CCString classOnlyPartName = StringHelper::SearchString(className, "::");
    if (classOnlyPartName != NULL_PTR(char8 *)) {

        uint32 size = static_cast<uint32>(StringHelper::SearchIndex(className, "::"));

        dllName.AppendN(className,size);
        className = classOnlyPartName.GetList()+2;

    }

    if (className != NULL_PTR(char8 *)) {

        uint32 i;
        uint32 databaseSize = classDatabase.ListSize();
        for (i = 0u; i < databaseSize; i++) {
            ClassRegistryItem *p = classDatabase.ListPeek(i);
            if (p != NULL) {
                if (StringHelper::Compare(p->GetClassName(), className) == 0) {
                    registryItem = p;
                    found = true;
                    break;
                }
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ClassRegistryDatabase: NULL pointer in input");
    }

    //registryItem still not found. Try to look inside the dll (if it exists)
    /*lint -e{593} this pointer is freed by the registry item when it is destructed*/
    if (!found) {
        if (dllName.GetSize() != 0) {

            // to allow stripping failed extensions from dll name
            uint32 baseSize = dllName.GetSize();

            LoadableLibrary *loader = new LoadableLibrary();

            uint32 i = 0u;
            bool dllOpened = false;
            //Check for all known operating system extensions.
            while ((operatingSystemDLLExtensions[i] != 0) && (!dllOpened)) {
                CCString extension = operatingSystemDLLExtensions[i];

                dllName.Truncate(baseSize);
                dllName.AppendN(extension);
                dllOpened = loader->Open(dllName);
                i++;
            }

            //If the dll was successfully opened than it is likely that more classes were registered
            //in the database. Search again.
            if (dllOpened) {

                uint32 databaseSize = classDatabase.ListSize();
                for (i = 0u; (i < databaseSize) && (!found); i++) {
                    ClassRegistryItem *p = classDatabase.ListPeek(i);
                    if (p != NULL) {
                        if (StringHelper::Compare(p->GetClassName(), className) == 0) {
                            registryItem = p;
                            registryItem->SetLoadableLibrary(loader);
                            found = true;
                        }
                    }
                }
            }
            //Not found...
            if (!found) {
                delete loader;
            }
        }
    }

    UnLock();
    return registryItem;
}

ClassRegistryItem *ClassRegistryDatabase::FindTypeIdName(CCString const typeidName) {
    ClassRegistryItem *registryItem = NULL_PTR(ClassRegistryItem *);
    if (!Lock()) {
        REPORT_ERROR(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
    }
    bool found = false;
    if (typeidName != NULL) {
        uint32 i;
        uint32 databaseSize = classDatabase.ListSize();
        for (i = 0u; (i < databaseSize) && (!found); i++) {
            ClassRegistryItem *p = classDatabase.ListPeek(i);
            if (p != NULL) {
                if (StringHelper::Compare(p->GetTypeidName(), typeidName) == 0) {
                    registryItem = p;
                    found = true;
                }
            }
        }
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ClassRegistryDatabase: NULL pointer in input");
    }

    UnLock();
    return registryItem;
}

uint32 ClassRegistryDatabase::GetSize() {
    uint32 size = 0u;
    if (Lock()) {
        size = classDatabase.ListSize();
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
    }
    UnLock();
    return size;
}

const ClassRegistryItem *ClassRegistryDatabase::Peek(const uint32 &idx) {
    ClassRegistryItem *item = NULL_PTR(ClassRegistryItem *);
    if (Lock()) {
        item = classDatabase.ListPeek(idx);
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ClassRegistryDatabase: Failed FastLock()");
    }
    UnLock();
    return item;
}

CCString const ClassRegistryDatabase::GetClassName() const {
    return "ClassRegistryDatabase";
}

void ClassRegistryDatabase::CleanUp() {
    classDatabase.CleanUp();
}

bool ClassRegistryDatabase::Lock() {
    return (mux.FastLock() == ErrorManagement::NoError);
}

void ClassRegistryDatabase::UnLock() {
    mux.FastUnLock();
}

}
