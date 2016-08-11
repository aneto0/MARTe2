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
 *
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

#include "CallRegisteredMethodLauncher.h"
#include "ClassProperties.h"
#include "ClassRegistryDatabase.h"
#include "ClassRegistryItem.h"
#include "ErrorManagement.h"
#include "Introspection.h"
#include "LoadableLibrary.h"
#include "ObjectBuilder.h"
#include "SearchFilterT.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

// TODO remove LCOV_EXCL_START
ClassRegistryItem::ClassRegistryItem(ClassProperties &classProperties_in) :
        LinkedListable(),
        classProperties(classProperties_in),
        classMethods() {
    numberOfInstances = 0;
    loadableLibrary = NULL_PTR(LoadableLibrary *);
    objectBuilder = NULL_PTR(ObjectBuilder *);
    introspection = NULL_PTR(Introspection *);
}

ClassRegistryItem *ClassRegistryItem::Instance(ClassRegistryItem *&instance,
                                               ClassProperties &classProperties_in) {
    ClassRegistryDatabase* crd = ClassRegistryDatabase::Instance();

    if ((crd != NULL_PTR(ClassRegistryDatabase*)) && (instance == NULL_PTR(ClassRegistryItem*))) {

        instance = new ClassRegistryItem(classProperties_in);
        crd->Add(instance);
    }

    return instance;
}

void ClassRegistryItem::SetObjectBuilder(const ObjectBuilder * const objectBuilderIn) {
    objectBuilder = objectBuilderIn;
}

const ObjectBuilder *ClassRegistryItem::GetObjectBuilder() const {
    return objectBuilder;
}

/*lint -e{1551} no exception should be thrown as loadableLibrary is properly initialised and
 * before deleting it is verified if the pointer is NULL*/
ClassRegistryItem::~ClassRegistryItem() {
    if (loadableLibrary != NULL_PTR(LoadableLibrary *)) {
        delete loadableLibrary;
    }

    loadableLibrary = NULL_PTR(LoadableLibrary *);
    introspection = NULL_PTR(Introspection *);
    objectBuilder = NULL_PTR(ObjectBuilder *);
}

void ClassRegistryItem::GetClassPropertiesCopy(ClassProperties &destination) const {
    destination = classProperties;
}

const ClassProperties *ClassRegistryItem::GetClassProperties() const {
    return &classProperties;
}

void ClassRegistryItem::SetIntrospection(const Introspection * const introspectionIn) {
    introspection = introspectionIn;
}

const Introspection * ClassRegistryItem::GetIntrospection() const {
    return introspection;
}

const LoadableLibrary *ClassRegistryItem::GetLoadableLibrary() const {
    return loadableLibrary;
}

void ClassRegistryItem::SetLoadableLibrary(const LoadableLibrary * const loadLibrary) {
    this->loadableLibrary = loadLibrary;
}

void ClassRegistryItem::RegisterMethods(ClassMethodsRegistryItem * const classMethodRecord) {
    classMethods.ListAdd(classMethodRecord);
}

void ClassRegistryItem::IncrementNumberOfInstances() {
    Atomic::Increment(&numberOfInstances);
}

void ClassRegistryItem::DecrementNumberOfInstances() {
    Atomic::Decrement(&numberOfInstances);
}

uint32 ClassRegistryItem::GetNumberOfInstances() const {
    return static_cast<uint32>(numberOfInstances);
}

void ClassRegistryItem::SetUniqueIdentifier(const ClassUID &uid) {
    classProperties.SetUniqueIdentifier(uid);
}

ErrorManagement::ErrorType ClassRegistryItem::CallRegisteredMethod(Object * const object,
                                                                   CCString methodName) {
    ErrorManagement::ErrorType ret;

    if (object == NULL_PTR(Object*)) {
        ret.parametersError = true;
    }

    if (methodName.GetList() == NULL_PTR(char8*)) {
        ret.parametersError = true;
    }

    if (ret.ErrorsCleared()) {
        /*
         * The launcher is passed as a filter to the ListSearch method of the
         * classMethods list, which will execute the Test method of the launcher
         * for each registered class method available in classMethods. Assuming
         * that the launcher's Test method will try, each time it is executed,
         * to call the target method methodName, the ListSearch method will
         * finish as soon as a successful call happens, or it will return an
         * unsupported feature error.
         */
        CallRegisteredMethodLauncher launcher(object, methodName);
        if (classMethods.ListSearch(&launcher) != NULL) {
            ret = launcher.GetResults();
        }
        else {
            ret.unsupportedFeature = true;
        }
    }
    return ret;
}

}
