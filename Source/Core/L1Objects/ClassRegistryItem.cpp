/**
 * @file ClassRegistryItem.cpp
 * @brief Source file for class ClassRegistryItem
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
 * the class ClassRegistryItem (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ClassRegistryDatabase.h"
#include "ClassRegistryItem.h"
#include "FastPollingMutexSem.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
static FastPollingMutexSem classRegistryItemMuxSem;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
ClassRegistryItem::ClassRegistryItem(const ClassProperties &clProperties,
                                     ObjectBuildFn *objBuildFn) {
    numberOfInstances = 0;
    classProperties = clProperties;
    loadableLibrary = NULL;
    objectBuildFn = objBuildFn;
    ClassRegistryDatabase::Instance().Add(this);
}

ClassRegistryItem::~ClassRegistryItem() {
    const LoadableLibrary *loader = loadableLibrary;
    ClassRegistryDatabase::Instance().Delete(this);
    if (loader != NULL) {
        delete loader;
    }
    loadableLibrary = NULL;
}

void ClassRegistryItem::GetClassPropertiesCopy(ClassProperties &destination) const {
    destination = classProperties;
}

const ClassProperties *ClassRegistryItem::GetClassProperties() const {
    return &classProperties;
}

void ClassRegistryItem::IncrementNumberOfInstances() {
    classRegistryItemMuxSem.FastLock();
    numberOfInstances++;
    classRegistryItemMuxSem.FastUnLock();
}

void ClassRegistryItem::DecrementNumberOfInstances() {
    classRegistryItemMuxSem.FastLock();
    numberOfInstances--;
    classRegistryItemMuxSem.FastUnLock();
}

uint32 ClassRegistryItem::GetNumberOfInstances() {
    return numberOfInstances;
}

Heap *ClassRegistryItem::GetHeap() {
    return &heap;
}

void ClassRegistryItem::SetHeap(const Heap& h) {
    heap = h;
}

const LoadableLibrary *ClassRegistryItem::GetLoadableLibrary() const {
    return loadableLibrary;
}

void ClassRegistryItem::SetLoadableLibrary(const LoadableLibrary *lLibrary) {
    this->loadableLibrary = lLibrary;
}

ObjectBuildFn *ClassRegistryItem::GetObjectBuildFunction() const {
    return objectBuildFn;
}

