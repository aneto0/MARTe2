/**
 * @file CallRegisteredMethodLauncher.h
 * @brief Header file for class CallRegisteredMethodLauncher
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

 * @details This header file contains the declaration of the class CallRegisteredMethodLauncher
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CALLREGISTEREDMETHODLAUNCHER_H_
#define CALLREGISTEREDMETHODLAUNCHER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CString.h"
#include "Object.h"
#include "SearchFilterT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class DLL_API CallRegisteredMethodLauncher: public SearchFilterT<ClassMethodsRegistryItem> {

public:

    CallRegisteredMethodLauncher(Object *objectIn,
                                 CCString methodNameIn);

    virtual ~CallRegisteredMethodLauncher();

    virtual bool Test(ClassMethodsRegistryItem *data);

    ErrorManagement::ErrorType GetResults();

protected:

    CCString methodName;

    Object *object;

    ErrorManagement::ErrorType ret;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

inline CallRegisteredMethodLauncher::CallRegisteredMethodLauncher(Object *objectIn,
                                                                  CCString methodNameIn) {
    object = objectIn;
    methodName = methodNameIn;
}

inline CallRegisteredMethodLauncher::~CallRegisteredMethodLauncher() {

}

inline bool CallRegisteredMethodLauncher::Test(ClassMethodsRegistryItem *data) {
    ret = data->CallFunction(object, methodName.GetList());
    // the function has been found and called
    return !ret.unsupportedFeature;
}

inline ErrorManagement::ErrorType CallRegisteredMethodLauncher::GetResults() {
    return ret;
}

}

#endif /* CALLREGISTEREDMETHODLAUNCHER_H_ */
