/**
 * @file ClassRegistryDatabase.cpp
 * @brief Source file for class ClassRegistryDatabase
 * @date Aug 4, 2015
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
 * the class ClassRegistryDatabase (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
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
    Reset();
}

void ClassRegistryDatabase::Delete(ClassRegistryItem *p) {
    ListExtract(p);
}

void ClassRegistryDatabase::Add(ClassRegistryItem *p) {
    const ClassRegistryItem *q = static_cast<const ClassRegistryItem *>(List());
    while (q != NULL) {
        if (StringHelper::Compare(q->GetClassProperties()->GetName(), p->GetClassProperties()->GetName()) == 0) {
            ListExtract(const_cast<ClassRegistryItem *>(q));
            q = NULL;
        }
        else {
            q = static_cast<ClassRegistryItem *>(p->Next());
        }
    }

    ListAdd(p);
}

ClassRegistryItem *ClassRegistryDatabase::Find(const char8 *className) {
    ClassRegistryItem *registryItem = NULL;

    const char8 *dllPartName = StringHelper::SearchString(className, "::");
    const int32 maxSize = 128 + 1;
    char8 dllName[maxSize];
    dllName[0] = 0;

    //Check for the string dllName::className pattern
    if (dllPartName != NULL) {
        int size = dllPartName - className;
        if (size > maxSize) {
            size = maxSize;
        }
        StringHelper::CopyN(dllName, dllPartName, size);
        className = dllPartName + 2;
    }

    if (className != NULL) {
        ClassRegistryItem *p = (ClassRegistryItem *) List();
        while (p != NULL) {
            if (StringHelper::Compare(p->GetClassProperties()->GetName(), className) == 0) {
                registryItem = p;
                break;
            }
            p = static_cast<ClassRegistryItem *>(p->Next());
        }
    }

    //registryItem still not found. Try to look inside the dll (if it exists)
    if ((registryItem == NULL) && (dllName[0] != 0)) {
        uint32 fullSize = StringHelper::Length(dllName) + 5u;
        char *fullName = static_cast<char *>(Memory::Malloc(fullSize));

        LoadableLibrary *loader = new LoadableLibrary();

        uint32 i;
        bool dllOpened = false;
        //Check for all known operating system extensions.
        while (operatingSystemDLLExtensions[i] != 0) {
            Memory::Set(fullName, 0, fullSize);
            StringHelper::ConcatenateN(fullName, operatingSystemDLLExtensions[i], 4);
            dllOpened = loader->Open(fullName);
            if (dllOpened) {
                break;
            }
            i++;
        }

        //If the dll was successfully opened than it is likely that more classes were registered
        //in the database. Search again.
        if (dllOpened) {
            ClassRegistryItem *p = (ClassRegistryItem *) List();
            while (p != NULL) {
                if (StringHelper::Compare(p->GetClassProperties()->GetName(), className) == 0) {
                    registryItem = p;
                    registryItem->SetLoadableLibrary(loader);
                    break;
                }
                p = static_cast<ClassRegistryItem *>(p->Next());
            }
        }
        //Not found...
        if (registryItem == NULL) {
            delete loader;
        }
    }
    return registryItem;
}

const ClassRegistryItem * ClassRegistryDatabase::List() const{
    return static_cast<const ClassRegistryItem *>(LinkedListHolder::List());
}

Object *ClassRegistryDatabase::CreateByName(const char8 *className, Heap &heap) {
    Object *obj = NULL;

    ClassRegistryItem *classRegistryItem = Find(className);
    if (classRegistryItem != NULL) {
        if (classRegistryItem->GetObjectBuildFunction() != NULL) {
            obj = classRegistryItem->GetObjectBuildFunction()(heap);
        }
    }

    return obj;
}
