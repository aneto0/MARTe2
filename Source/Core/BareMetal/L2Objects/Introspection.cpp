/**
 * @file Introspection.cpp
 * @brief Source file for class Introspection
 * @date 18/12/2016
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
 * the class Introspection (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Introspection.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

Introspection::Introspection() :
        fields() {
    classSize = 0u;
    introMemLocation = NULL_PTR(void *);
}

Introspection::Introspection(const IntrospectionEntry ** const introspectionListIn,
                             const uint32 classSizeIn) :
        fields(introspectionListIn) {
    classSize = classSizeIn;
    introMemLocation = NULL_PTR(void *);
}

const IntrospectionEntry Introspection::operator[](const uint32 index) const {
    return (fields[index] == NULL)?(InvalidIntrospectionEntry):(*fields[index]);
}

uint32 Introspection::GetNumberOfMembers() const {
    return fields.GetSize();
}

uint32 Introspection::GetClassSize() const {
    return classSize;
}

const IntrospectionEntry **Introspection::GetFields() {
    return fields.GetList();
}

void *Introspection::GetIntrospectionEntryMemory() {
    return introMemLocation;
}

void Introspection::SetIntrospectionEntryMemory(void * const mem) {
    introMemLocation = mem;
}
}
