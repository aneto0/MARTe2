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

#include "CallRegisteredMethodLauncher.h"
#include "CString.h"

/*---------------------------------------------------------------------------*/
/*                         Forward declarations                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
    class ClassMethodsRegistryItem;
    class Object;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Template version of CallRegisteredMethodLauncher
 *
 * @details This class derives from CallRegisteredMethodLauncher inheriting
 * all of its features, but adding the ability to pass an argument to the
 * called method, instead of assuming that the registered method is a method
 * without arguments.
 *
 * Like CallRegisteredMethodLauncher, it needs to be bound to a target object
 * of class Object and it also needs the name of the registered method to be
 * called on that object. Besides, it also needs to be bound to the argument
 * of the method to be called.
 *
 * Like CallRegisteredMethodLauncher, this class is also a filter and is
 * expected to be used like that, i.e. the only way to actually call the
 * registered method is indirectly through the Test method. In other words,
 * each time that an iterator calls the Test method, it will try to call the
 * registered method, storing the status of the call which will be available
 * through the method GetResults.
 *
 * @tparam argType Is the expected type of the argument to be passed to the
 * called method.
 */
template<typename argType>
class DLL_API CallRegisteredMethodLauncherT: public CallRegisteredMethodLauncher {
public:

    /**
     * @brief Constructor
     * @param[in] objectIn The target object of the call
     * @param[in] methodNameIn The name of the registered method to call
     * @param[in] parametersIn The argument to be passed to the registered method when called
     */
    CallRegisteredMethodLauncherT(Object *objectIn,
                                  CCString methodNameIn,
                                  argType parametersIn);

    /**
     * @brief Destructor
     */
    virtual ~CallRegisteredMethodLauncherT();

    /**
     * @brief LinkedListable searching callback function.
     * @details This function is called for every element in the list being searched (i.e. traversed).
     * @param[in] data the current LinkedListable element to be tested.
     * @return true if \a data meets the search criteria.
     */
    virtual bool Test(ClassMethodsRegistryItem *data);

    /**
     * @brief Gets the status of the last call
     * @return An ErrorType value with the status of the last call
     */
    ErrorManagement::ErrorType GetResults();

protected:

    /**
     * The argument to be passed to the registered method when called.
     */
    argType parameters;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename argType>
inline CallRegisteredMethodLauncherT<argType>::CallRegisteredMethodLauncherT(Object *objectIn,
                                                                    CCString methodNameIn,
                                                                    argType parametersIn) :
        CallRegisteredMethodLauncher(objectIn, methodNameIn),
        parameters(parametersIn) {
}

template<typename argType>
inline CallRegisteredMethodLauncherT<argType>::~CallRegisteredMethodLauncherT() {

}

template<typename argType>
inline bool CallRegisteredMethodLauncherT<argType>::Test(ClassMethodsRegistryItem *data) {
    ret = data->CallFunction<argType>(object, methodName.GetList(), parameters);
    // the function has been found and called
    return !ret.unsupportedFeature;
}

template<typename argType>
inline ErrorManagement::ErrorType CallRegisteredMethodLauncherT<argType>::GetResults() {
    return ret;
}

}

#endif /* CALLREGISTEREDMETHODLAUNCHERT_H_ */
