/**
 * @file ClassProperties.cpp
 * @brief Source file for class ClassProperties
 * @date 05/08/2015
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
 * the class ClassProperties (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */
#define DLL_API
/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ClassProperties.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ClassProperties::ClassProperties() {
    className = static_cast<const char8 *>(NULL);
    typeidClassName = static_cast<const char8 *>(NULL);
    classVersion = static_cast<const char8 *>(NULL);
    uniqueIdentifier = 0u;
}

ClassProperties::ClassProperties(const char8 * const cName,
                                 const char8 * const typeidName,
                                 const char8 * const cVersion) {
    className = cName;
    typeidClassName = typeidName;
    classVersion = cVersion;
    uniqueIdentifier = 0u;
}

const char8* ClassProperties::GetName() const {
    return className;
}

const char8* ClassProperties::GetTypeIdName() const {
    return typeidClassName;
}

const char8* ClassProperties::GetVersion() const {
    return classVersion;
}

uint32 ClassProperties::GetUniqueIdentifier() const{
    return uniqueIdentifier;
}

void ClassProperties::SetUniqueIdentifier(const uint32 &uid){
    uniqueIdentifier = uid;
}

}
