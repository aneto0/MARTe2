/**
 * @file ClassRegistryItem.cpp
 * @brief Source file for class ClassRegistryItem
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
 * the class ClassRegistryItem (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ClassRegistryDatabase.h"
#include "ClassRegistryItem.h"
#include "FastPollingMutexSem.h"
#include "ErrorManagement.h"

namespace MARTe {
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/*lint -e{9141} global declaration but only used to support the class implementation.
 * The symbol is not exported (static). This could also be replaced by an anonymous namespace.
 */
static FastPollingMutexSem classRegistryItemMuxSem;

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
//LCOV_EXCL_START
ClassRegistryItem::ClassRegistryItem() :
        classProperties() {
    numberOfInstances = 0u;
    loadableLibrary = NULL_PTR(LoadableLibrary *);
    objectBuildFn = NULL_PTR(ObjectBuildFn *);
}
//LCOV_EXCL_STOP

ClassRegistryItem::ClassRegistryItem(const ClassProperties &clProperties,
                                     const ObjectBuildFn * const objBuildFn) {
    numberOfInstances = 0u;
    classProperties = clProperties;
    loadableLibrary = NULL_PTR(LoadableLibrary *);
    objectBuildFn = objBuildFn;
    ClassRegistryDatabase::Instance()->Add(this);
}

/*lint -e{1551} no exception should be thrown as loadableLibrary is properly initialised and
 * before deleting it is verified if the pointer is NULL*/
ClassRegistryItem::~ClassRegistryItem() {
    if (loadableLibrary != NULL_PTR(LoadableLibrary *)) {
        delete loadableLibrary;
    }
    loadableLibrary = NULL_PTR(LoadableLibrary *);
}

void ClassRegistryItem::GetClassPropertiesCopy(ClassProperties &destination) const {
    destination = classProperties;
}

const ClassProperties *ClassRegistryItem::GetClassProperties() const {
    return &classProperties;
}

void ClassRegistryItem::IncrementNumberOfInstances() {
    if (classRegistryItemMuxSem.FastLock() == ErrorManagement::NoError) {
        numberOfInstances++;
    }
    classRegistryItemMuxSem.FastUnLock();
}

void ClassRegistryItem::DecrementNumberOfInstances() {
    if (classRegistryItemMuxSem.FastLock() == ErrorManagement::NoError) {
        numberOfInstances--;
    }
    else {
        REPORT_ERROR(ErrorManagement::FatalError, "ClassRegistryItem: Failed FastLock()");
    }
    classRegistryItemMuxSem.FastUnLock();
}

uint32 ClassRegistryItem::GetNumberOfInstances() const {
    return numberOfInstances;
}

const LoadableLibrary *ClassRegistryItem::GetLoadableLibrary() const {
    return loadableLibrary;
}

void ClassRegistryItem::SetLoadableLibrary(const LoadableLibrary * const loadLibrary) {
    this->loadableLibrary = loadLibrary;
}

const ObjectBuildFn *ClassRegistryItem::GetObjectBuildFunction() const {
    return objectBuildFn;
}

void ClassRegistryItem::SetUniqueIdentifier(const ClassUID &uid) {
    classProperties.SetUniqueIdentifier(uid);
}

}
