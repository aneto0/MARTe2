/**
 * @file ObjectRegistryDatabase.cpp
 * @brief Source file for class ObjectRegistryDatabase
 * @date 18/02/2016
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
 *
 * @details This source file contains the definition of all the methods for
 * the class ObjectRegistryDatabase (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ErrorType.h"
#include "ObjectRegistryDatabase.h"
#include "ReferenceContainerFilterObjectName.h"
#include "ReferenceContainerFilterReferences.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ObjectRegistryDatabase *ObjectRegistryDatabase::Instance() {

    static ObjectRegistryDatabase *instance = NULL_PTR(ObjectRegistryDatabase *);
    if (instance == NULL) {
        instance = new ObjectRegistryDatabase(); //dynamic_cast<ObjectRegistryDatabase*>(ObjectRegistryDatabase_BuildFn());
        GlobalObjectsDatabase::Instance()->Add(instance, NUMBER_OF_GLOBAL_OBJECTS - 3u);
    }
    return instance;
}

ObjectRegistryDatabase::ObjectRegistryDatabase() :
        ReferenceContainer() {
}

/*lint -e{1551} .*/
ObjectRegistryDatabase::~ObjectRegistryDatabase() {
    ReferenceContainer::CleanUp();
    // The ReferenceContainer destructor does the work
}


const char8 * const ObjectRegistryDatabase::GetClassName() const {
    return "ObjectRegistryDatabase";
}

/*lint -e{1550} */
void *ObjectRegistryDatabase::operator new(const osulong size) throw () {
    return GlobalObjectI::operator new(size);
}

/*lint -e{1550} */
void ObjectRegistryDatabase::operator delete(void * const p) {
    return GlobalObjectI::operator delete(p);
}

}
