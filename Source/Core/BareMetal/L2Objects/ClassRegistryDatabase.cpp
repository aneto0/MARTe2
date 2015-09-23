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

 * @details This source file contains the definition of all the methods for
 * the class ClassRegistryDatabase (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ClassRegistryItem.h"
#include "Object.h"
#include "HeapManager.h"
#include "StringHelper.h"
#include "MemoryOperationsHelper.h"
#include "LoadableLibrary.h"
#include "ClassRegistryItem.h"
#include "ClassRegistryDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

ClassRegistryDatabase *ClassRegistryDatabase::Instance() {
    static ClassRegistryDatabase instance;
    return &instance;
}

ClassRegistryDatabase::ClassRegistryDatabase() :
        classDatabase(32u) {
    classUniqueIdentifier = 0u;
}

ClassRegistryDatabase::~ClassRegistryDatabase() {
}

void ClassRegistryDatabase::Add(ClassRegistryItem * const p) {
    if (mux.FastLock() == ErrorManagement::NoError) {
        p->SetUniqueIdentifier(classUniqueIdentifier);
        if (classDatabase.Add(classUniqueIdentifier, p)) {
            classUniqueIdentifier++;
        }
    }
    mux.FastUnLock();
}

const ClassRegistryItem *ClassRegistryDatabase::Find(const char8 *className) {
    ClassRegistryItem *registryItem = NULL_PTR(ClassRegistryItem *);
    if (mux.FastLock() == ErrorManagement::NoError) {
        const uint32 maxSize = 129u;
        char8 dllName[maxSize];
        dllName[0] = '\0';
        bool found = false;

        //Check for the string pattern dllName::className
        const char8 *classOnlyPartName = StringHelper::SearchString(className, "::");
        if (classOnlyPartName != NULL) {
            uint32 size = static_cast<uint32>(StringHelper::SearchIndex(className, "::"));
            if (size > (maxSize - 1u)) {
                size = (maxSize - 1u);
            }
            if (StringHelper::CopyN(&(dllName[0]), className, size)) {
                dllName[size] = '\0';
                className = &classOnlyPartName[2];
            }
        }

        if (className != NULL) {
            ClassRegistryItem *p;
            uint32 i;
            uint32 databaseSize = classDatabase.GetSize();
            for (i = 0u; i < databaseSize; i++) {
                if (classDatabase.Peek(i, p)) {
                    const ClassProperties *classProperties = p->GetClassProperties();
                    if (classProperties != NULL_PTR(ClassProperties *)) {
                        if (StringHelper::Compare(classProperties->GetName(), className) == 0) {
                            registryItem = p;
                            found = true;
                            break;
                        }
                    }
                }
            }
        }

        //registryItem still not found. Try to look inside the dll (if it exists)
        /*lint -e{593} this pointer is freed by the registry item when it is destructed*/
        if (!found) {
            if (dllName[0] != '\0') {

                uint32 fullSize = StringHelper::Length(&(dllName[0])) + 5u;
                /*lint -e{925} pointer to pointer required due to Malloc implementation*/
                char8 *fullName = static_cast<char8 *>(HeapManager::Malloc(fullSize));

                LoadableLibrary *loader = new LoadableLibrary();

                uint32 i = 0u;
                bool dllOpened = false;
                //Check for all known operating system extensions.
                while (operatingSystemDLLExtensions[i] != 0) {
                    if (MemoryOperationsHelper::Set(fullName, '\0', fullSize)) {
                        const char8 *extension = operatingSystemDLLExtensions[i];
                        if (StringHelper::ConcatenateN(fullName, extension, 4u)) {
                            dllOpened = loader->Open(fullName);
                            if (dllOpened) {
                                break;
                            }
                            i++;
                        }
                    }
                }

                //If the dll was successfully opened than it is likely that more classes were registered
                //in the database. Search again.
                if (dllOpened) {
                    ClassRegistryItem *p;
                    uint32 databaseSize = classDatabase.GetSize();
                    for (i = 0u; (i < databaseSize) && (!found); i++) {
                        if (classDatabase.Peek(i, p)) {
                            const ClassProperties *classProperties = p->GetClassProperties();
                            if (classProperties != NULL_PTR(ClassProperties *)) {
                                if (StringHelper::Compare(classProperties->GetName(), className) == 0) {
                                    registryItem = p;
                                    registryItem->SetLoadableLibrary(loader);
                                    found = true;
                                }
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
    }
    mux.FastUnLock();
    return registryItem;
}

const ClassRegistryItem *ClassRegistryDatabase::FindTypeIdName(const char8 * const typeidName) {
    ClassRegistryItem *registryItem = NULL_PTR(ClassRegistryItem *);
    if (mux.FastLock() == ErrorManagement::NoError) {
        bool found = false;
        if (typeidName != NULL) {
            ClassRegistryItem *p;
            uint32 i;
            uint32 databaseSize = classDatabase.GetSize();
            for (i = 0u; (i < databaseSize) && (!found); i++) {
                if (classDatabase.Peek(i, p)) {
                    const ClassProperties *classProperties = p->GetClassProperties();
                    if (classProperties != NULL_PTR(ClassProperties *)) {
                        if (StringHelper::Compare(classProperties->GetTypeIdName(), typeidName) == 0) {
                            registryItem = p;
                            found = true;
                        }
                    }
                }
            }
        }
    }
    mux.FastUnLock();
    return registryItem;
}

uint32 ClassRegistryDatabase::GetSize() {
    uint32 size = 0u;
    if (mux.FastLock() == ErrorManagement::NoError) {
        size = classDatabase.GetSize();
    }
    mux.FastUnLock();
    return size;
}

const ClassRegistryItem *ClassRegistryDatabase::Peek(const uint32 &idx) {
    ClassRegistryItem *item = NULL_PTR(ClassRegistryItem *);
    if (mux.FastLock() == ErrorManagement::NoError) {
        if(!classDatabase.Peek(idx, item)){
            item = NULL_PTR(ClassRegistryItem *);
        }
    }
    mux.FastUnLock();
    return item;
}

}
