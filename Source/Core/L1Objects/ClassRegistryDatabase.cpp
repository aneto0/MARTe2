/**
 * @file ClassRegistryDatabase.cpp
 * @brief Source file for class ClassRegistryDatabase
 * @date Aug 4, 2015
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
#include "Memory.h"
#include "StringHelper.h"
#include "LoadableLibrary.h"
#include "ClassRegistryItem.h"
#include "ClassRegistryDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
ClassRegistryDatabase &ClassRegistryDatabase::Instance() {
    static ClassRegistryDatabase instance;
    return instance;
}

ClassRegistryDatabase::ClassRegistryDatabase() {
}

ClassRegistryDatabase::~ClassRegistryDatabase() {
    /*lint -e{1551} no exception can be thrown by this method.*/
    classDatabase.Reset();
}

bool ClassRegistryDatabase::Delete(ClassRegistryItem * const p) {
    return classDatabase.ListExtract(p);
}

/*lint -e{929} pointer to pointer conversion required to dynamic_cast to the correct type*/
void ClassRegistryDatabase::Add(ClassRegistryItem * const p) {
    ClassRegistryItem *q = dynamic_cast<ClassRegistryItem *>(classDatabase.List());
    while (q != NULL) {
        if (StringHelper::Compare(q->GetClassProperties()->GetName(), p->GetClassProperties()->GetName()) == 0) {
            if (classDatabase.ListExtract(q)) {
                q = static_cast<ClassRegistryItem *>(NULL);
            }
        }
        else {
            q = dynamic_cast<ClassRegistryItem *>(q->Next());
        }
    }

    classDatabase.ListAdd(p);
}

/*lint -e{929} -e{925} the current implementation of the LinkedListable requires pointer to pointer casting
 * i.e. downcasting is necessary.*/
ClassRegistryItem *ClassRegistryDatabase::Find(const char8 *className) {
    ClassRegistryItem *registryItem = NULL_PTR(ClassRegistryItem *);

    const uint32 maxSize = 129u;
    char8 dllName[maxSize];
    dllName[0] = '\0';

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
        ClassRegistryItem *p = List();
        while (p != NULL) {
            if (StringHelper::Compare(p->GetClassProperties()->GetName(), className) == 0) {
                registryItem = p;
                break;
            }
            p = dynamic_cast<ClassRegistryItem *>(p->Next());
        }
    }

    //registryItem still not found. Try to look inside the dll (if it exists)
    /*lint -e{593} this pointer is freed by the registry item when it is destructed*/
    if ((registryItem == NULL_PTR(ClassRegistryItem *)) && (dllName[0] != '\0')) {
        uint32 fullSize = StringHelper::Length(&(dllName[0])) + 5u;
        char8 *fullName = static_cast<char8 *>(Memory::Malloc(fullSize));

        LoadableLibrary *loader = new LoadableLibrary();

        uint32 i = 0u;
        bool dllOpened = false;
        //Check for all known operating system extensions.
        while (operatingSystemDLLExtensions[i] != 0) {
            if (Memory::Set(fullName, '\0', fullSize)) {
                const char8 *extension = operatingSystemDLLExtensions[i];
                if (StringHelper::ConcatenateN(fullName, extension, 4u) != NULL_PTR(char8 *)) {
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
            ClassRegistryItem *p = dynamic_cast<ClassRegistryItem *>(List());
            while (p != NULL) {
                if (StringHelper::Compare(p->GetClassProperties()->GetName(), className) == 0) {
                    registryItem = p;
                    registryItem->SetLoadableLibrary(loader);
                    break;
                }
                p = dynamic_cast<ClassRegistryItem *>(p->Next());
            }
        }
        //Not found...
        if (registryItem == NULL) {
            delete loader;
        }
    }
    return registryItem;
}

/*lint -e{929} the current implementation of the LinkedListable requires downcasting.*/
ClassRegistryItem * ClassRegistryDatabase::List() {
    return dynamic_cast<ClassRegistryItem *>(classDatabase.List());
}

uint32 ClassRegistryDatabase::Size() const {
    return classDatabase.ListSize();
}

/*lint -e{929} the current implementation of the LinkedListable requires downcasting.*/
const ClassRegistryItem *ClassRegistryDatabase::ElementAt(const uint32 &idx) {
    return dynamic_cast<ClassRegistryItem *>(classDatabase.ListPeek(idx));
}
