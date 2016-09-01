/**
 * @file ClassMethodsRegistryItem.h
 * @brief Header file for class ClassMethodsRegistryItem
 * @date 07/04/2016
 * @author Filippo Sartori
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
 * @details This header file contains the declaration of the class ClassMethodsRegistryItem
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSMETHODSREGISTRYITEM_H_
#define CLASSMETHODSREGISTRYITEM_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "LinkedListable.h"
#include "ClassMethodInterfaceMapper.h"
#include "CString.h"
#include "CLASSMETHODREGISTER.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

namespace MARTe {
class ClassRegistryItem;
}

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief This class represents a registry of callable methods for a class.
 *
 * @details This class is in charge of allowing to call methods on a target
 * object, but only those methods that are registered, i.e. those that are
 * into a closed list. For that reason, each instance of this class must be
 * linked to a previously initialised list of ClassMethodInterfaceMapper
 * instances and a list of method names, being its maintenance out of the
 * scope of the class.
 *
 * Moreover, each instance of this class will register itself to a previously
 * existent ClassRegistryItem instance, calling its RegisterMethods method
 * (that's why this class derives from LinkedListable). The initialisation
 * and maintenance of this ClassRegistryItem instance is out of the scope
 * of this class, too.
 *
 * Example (registering virtual function):\n
 * Assume we have the inheritance chain A<--B<--C and the virtual function
 * A::f() reimplemented in B::f() but not in C. Registering A::f() in the
 * ClassRegistryItem of the class C, means that when calling
 * CallFunction(C* x) the implementation will be the one defined in B,
 * namely B::f() will be called because of the polymorphism.
 *
 * Example (registering non virtual function):\n
 * Assume we have the inheritance chain A<--B<--C and the non-virtual
 * function A::f() implemented also in B::f() but not in C. Registering
 * A::f() in the ClassRegistryItem of the class C, means that when calling
 * CallFunction(C* x) the implementation will be the one defined in A,
 * namely A::f(). Registering B::f() in the ClassRegistryItem of C, the
 * implementation will be the one defined in B::f() also passing a pointer
 * to C in the CallFunction. At last, registering C::f(), CallFunction(C* x)
 * will return UnsupportedFeature because C::f() has not been defined.
 *
 * @warning All the methods of the provided list have to belong to the class
 * we want to register, otherwise at the moment of calling the function,
 * through the CallFunction method, it will return ErrorManagement::unsupportedFeature.
 *
 *
 * method without argument and 1 argument
 *
 */
class DLL_API ClassMethodsRegistryItem: public LinkedListable {

public:

    /**
     * @brief Constructor.
     * @details Initialise an instance of the class with the required external
     * objects, i.e. it links the instance with the list of callable methods
     * and its method names list companion, and the ClassRegistryItem instance.
     * @param[in] cri is the ClassRegistryItem where this object has to be
     * added to.
     * @param[in] functionTable_in is the list of ClassMethodInterfaceMapper,
     * each one capable to store and call one class method.
     * @param[in] functionNames_in is a string containing all the class methods
     * names. The syntax for this string has to be "ClassName::FunctionName1,
     * ClassName::FunctionName2, ...". ClassName it is not forced to be the
     * same name of the class to be registered because can be also the name of
     * an ancestor of this class.
     */
    ClassMethodsRegistryItem(ClassRegistryItem * const cri,
                             ClassMethodInterfaceMapper * const functionTable_In,
                             const char8 * const functionNames_In);

    /**
     * @brief Destructor.
     */
    virtual ~ClassMethodsRegistryItem();

    /**
     * @brief Calls a function without arguments.
     * @param[in] context is the object which must call the class method.
     * @param[in] name is the name of the class method to be called. The name
     * has to be only the name of the function without any extra mangling.
     * @return
     * + ErrorManagement::unsupportedFeature if \a name does not match with
     * any of the names in the function names list or if the class method does
     * not belong to \a context.
     * + ErrorManagement::fatalError if the class method returns false
     * + ErrorManagemenr::NoError if the class method returns true.
     */
    ErrorManagement::ErrorType CallFunction(Object * const context,
                                            const char8 * const name);

    /**
     * @brief Calls a function with one argument.
     * @param[in] context is the object which must call the class method.
     * @param[in] name is the name of the class method to be called. The name
     * has to be only the name of the function without any extra mangling.
     * @param[in,out] ref is the class method argument.
     * @return
     * + ErrorManagement::unsupportedFeature if \a name does not match with
     * any of the names in the function names list or if the class method does
     * not belong to \a context.
     * + ErrorManagement::fatalError if the class method returns false
     * + ErrorManagemenr::NoError if the class method returns true.
     * @tparam argType is the type of the argument to pass to the class method.
     */
    template<typename argType>
    ErrorManagement::ErrorType CallFunction(Object * const context,
                                            const char8 * const name,
                                            argType ref);

private:

    /**
     * @brief Finds the class method name in the class method names list.
     * @param[in] name is the name of the class method (without any mangling)
     * to be searched in the names list.
     * @return the index of the class method pointer in the table if \a name
     * is found in the list, -1 otherwise.
     */
    int32 FindFunction(const char8 * const name,
                       const int32 minIndex);

    /**
     * @brief The pointer to the array of objects used to call class methods
     * @warning This class does not own the array, so it is not responsible
     * for the management of the array's lifecycle.
     */
    ClassMethodInterfaceMapper * const functionTable;

    /**
     * The class method names list encoded into a string.
     */
    CCString functionNames;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

template<typename argType>
ErrorManagement::ErrorType ClassMethodsRegistryItem::CallFunction(Object * context,
                                                                  const char8 *name,
                                                                  argType ref) {
    ErrorManagement::ErrorType returnValue;

    if (context == NULL) {
        returnValue.parametersError = true;
    }
    if (name == NULL) {
        returnValue.parametersError = true;
    }

    if (returnValue.ErrorsCleared()) {
        ClassMethodInterfaceMapper * fmp = NULL_PTR(ClassMethodInterfaceMapper *);
        int32 minIndex = 0;
        int32 functionIndex = 0;
        while (functionIndex >= 0) {
            returnValue = ErrorManagement::noError;
            functionIndex = FindFunction(name, minIndex);
            if (functionIndex >= 0) {
                fmp = &functionTable[functionIndex];
            }
            else {
                returnValue.unsupportedFeature = true;
            }

            if (returnValue.ErrorsCleared()) {
                /*lint -e{613} .The NULL checking has been done before entering here*/
                returnValue = fmp->Call<argType>(context, ref);
                if (returnValue.unsupportedFeature == true) {
                    // allow function overload, try again to search!!
                    minIndex = functionIndex + 1;
                }
                else {
                    //the function has been executed.. exit
                    functionIndex = -1;
                }
            }
        }
    }

    return returnValue;
}

}

#endif /* CLASSMETHODSREGISTRYITEM_H_ */

