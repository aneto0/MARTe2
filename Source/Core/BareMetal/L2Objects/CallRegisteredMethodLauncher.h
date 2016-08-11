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
 *
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

#include "ClassMethodsRegistryItem.h"
#include "CString.h"
#include "ErrorType.h"
#include "SearchFilterT.h"

/*---------------------------------------------------------------------------*/
/*                         Forward declarations                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class Object;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Call registered method launcher
 *
 * @details This class represents an object that behaves as a launcher for
 * calling registered methods of a target object. It needs to be bound with
 * a target object of class Object and it also needs the name of the
 * registered method to be called on that object. It must be noted that the
 * registered method has to be one without arguments.
 *
 * This class is also a filter and is expected to be used like that, i.e. the
 * only way to actually call the registered method is indirectly through the
 * Test method. In other words, each time that an iterator calls the Test
 * method, it will try to call the registered method, storing the status of
 * the call which will be available through the method GetResults.
 */
class DLL_API CallRegisteredMethodLauncher: public SearchFilterT<ClassMethodsRegistryItem> {

public:

    /**
     * @brief Constructor
     * @param[in] objectIn The target object of the call
     * @param[in] methodNameIn The name of the registered method to call
     */
    CallRegisteredMethodLauncher(Object* const objectIn,
                                 const CCString& methodNameIn);

    /**
     * @brief Destructor
     */
    virtual ~CallRegisteredMethodLauncher();

    /**
     * @brief LinkedListable searching callback function.
     * @details This function is called for every element in the list being
     * searched (i.e. traversed).
     * @param[in] data the current LinkedListable element to be tested.
     * @return true if \a data meets the search criteria.
     */
    virtual bool Test(ClassMethodsRegistryItem * const data);

    /**
     * @brief Gets the status of the last call
     * @return An ErrorType value with the status of the last call
     */
    ErrorManagement::ErrorType GetResults();

protected:

    /**
     * The name of the registered method to call
     */
    CCString methodName;

    /**
     * The pointer to the target object of the call
     */
    Object *object;

    /**
     * The status of the last call
     */
    ErrorManagement::ErrorType ret;
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

inline ErrorManagement::ErrorType CallRegisteredMethodLauncher::GetResults() {
    return ret;
}

}

#endif /* CALLREGISTEREDMETHODLAUNCHER_H_ */
