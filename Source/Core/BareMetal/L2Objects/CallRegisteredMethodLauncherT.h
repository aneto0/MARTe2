/**
 * @file CallRegisteredMethodLauncherT.h
 * @brief Header file for class CallRegisteredMethodLauncherT
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

 * @details This header file contains the declaration of the class CallRegisteredMethodLauncherT
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CALLREGISTEREDMETHODLAUNCHERT_H_
#define CALLREGISTEREDMETHODLAUNCHERT_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CString.h"
#include "Object.h"
#include "CallRegisteredMethodLauncher.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename argType>
class DLL_API CallRegisteredMethodLauncherT: public CallRegisteredMethodLauncher {
public:

    CallRegisteredMethodLauncherT(Object *objectIn,
                                  CCString methodNameIn,
                                  argType parametersIn);

    virtual ~CallRegisteredMethodLauncherT();

    virtual bool Test(ClassMethodsRegistryItem *data);

    ErrorManagement::ErrorType GetResults();

protected:

    argType parameters;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

inline CallRegisteredMethodLauncherT::CallRegisteredMethodLauncherT(Object *objectIn,
                              CCString methodNameIn,
                              argType parametersIn) :
        CallRegisteredMethodLauncher(objectIn, methodNameIn),
        parameters(parametersIn) {
}

inline CallRegisteredMethodLauncherT::~CallRegisteredMethodLauncherT() {

}

inline bool CallRegisteredMethodLauncherT::Test(ClassMethodsRegistryItem *data) {
    ret = data->CallFunction<argType>(object, methodName.GetList(), parameters);
    // the function has been found and called
    return !ret.unsupportedFeature;
}

inline ErrorManagement::ErrorType CallRegisteredMethodLauncherT::GetResults() {
    return ret;
}

}

#endif /* CALLREGISTEREDMETHODLAUNCHERT_H_ */
