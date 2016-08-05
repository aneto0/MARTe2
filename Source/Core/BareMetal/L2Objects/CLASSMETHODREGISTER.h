/**
 * @file CLASSMETHODREGISTER.h
 * @brief Header file for class CLASSMETHODREGISTER
 * @date 20/07/2016
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
 * @details This header file contains the declaration of the class CLASSMETHODREGISTER
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASS_METHOD_REGISTER_H_
#define CLASS_METHOD_REGISTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Macro used to register automatically a list of class methods in the ClassRegistryItem (and in the ClassRegistryDatabase).
 * @details Passing a list of class method pointers, this macro automatically creates the list of function names. To find the function
 * in the list it is only necessary that the function name (without mangling) is enclosed between a ':' and a ',' or '\0'. For instance
 * putting "(bool (*)())A::f1(),B::f2()" the find function will match the functions f1 and f2. It is possible to register two different
 * functions with the same name (if a class overloads a method).
 * @param[in] C is the class name.
 * @param[in] ... list of class methods.
 */
#define CLASS_METHOD_REGISTER(C,...)\
    static MARTe::ClassMethodInterfaceMapper C ## __ClassMethodsInterfaceMapper[] = {__VA_ARGS__}; \
    static /*const*/ MARTe::ClassMethodsRegistryItem C ## __ClassMethodsRegistryItem(C::GetClassRegistryItem_Static(),C ## __ClassMethodsInterfaceMapper,#__VA_ARGS__);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASS_METHOD_REGISTER_H_ */

