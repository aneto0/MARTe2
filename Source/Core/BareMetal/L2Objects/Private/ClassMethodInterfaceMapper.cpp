/**
 * @file ClassMethodInterfaceMapper.cpp
 * @brief Source file for class ClassMethodInterfaceMapper
 * @date 13/06/2016
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
 * the class ClassMethodInterfaceMapper (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassMethodInterfaceMapper.h"
#include "CString.h"
#include "CCString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*lint -e{1551} no exception should be thrown given that
 * before deleting the pointer is verified not to be NULL*/
ClassMethodInterfaceMapper::~ClassMethodInterfaceMapper() {
    if (caller != NULL) {
        delete caller;
    }
}

ClassMethodCaller *ClassMethodInterfaceMapper::GetMethodCaller() {
    return caller;
}

void ClassMethodInterfaceMapper::SetMethodName(CCString const & name) {
    methodName = name;
}

CCString ClassMethodInterfaceMapper::GetMethodName() const {
    return methodName;
}

}
