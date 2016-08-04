/**
 * @file CallRegisteredMethodLauncher.cpp
 * @brief Source file for class CallRegisteredMethodLauncher
 * @date 11/07/2016
 * @author Ivan Herrero
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
 * the class CallRegisteredMethodLauncher (public, protected, and private). Be aware that some 
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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

CallRegisteredMethodLauncher::CallRegisteredMethodLauncher(Object* const objectIn,
                                                           const CCString& methodNameIn) {
    object = objectIn;
    methodName = methodNameIn;
}

CallRegisteredMethodLauncher::~CallRegisteredMethodLauncher() {
    /*lint -e{1540} the memory of the object pointed by the pointer member
     * 'object' is managed by the clients of this class*/
}

bool CallRegisteredMethodLauncher::Test(ClassMethodsRegistryItem * const data) {
    ret = data->CallFunction(object, methodName.GetList());
    // the function has been found and called
    return !ret.unsupportedFeature;
}

}

