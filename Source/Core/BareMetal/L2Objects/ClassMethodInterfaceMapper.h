/**
 * @file ClassMethodInterfaceMapper.h
 * @brief Header file for class ClassMethodInterfaceMapper
 * @date 11/04/2016
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
 * @details This header file contains the declaration of the class ClassMethodInterfaceMapper
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSMETHODINTERFACEMAPPER_H_
#define CLASSMETHODINTERFACEMAPPER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ErrorType.h"
#include "ClassMethodCaller.h"
#include "ClassMethodCallerT.h"
#include "LinkedListable.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Associates any function with the type ErrorManagement::ErrorType (className::*)(argType1, ..., argType4)
 *  against a ClassMethodCaller, where argType1, ..., argType4 are parameters of any type.
 * @details The ClassMethodCallerT to be constructed is automatically selected by the provided template parameters,
 *  allowing to distinguish, for any of the parameters, between input (copy or constant reference) and output parameters.
 */
/*lint -e{9109} forward declaration of ClassMethodInterfaceMapper required in the ClassRegistryItem*/
class DLL_API ClassMethodInterfaceMapper: public LinkedListable {

public:

    /**
     * @brief Constructor with one input pointer parameter.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with one input pointer parameter.
     * @post
     *   GetMethodCaller() == NULL given that pointer parameters are not supported.
     */
    template<class className, typename argType1>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *));

    /**
     * @brief Constructor with two inputs where one of them is a pointer.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with two input pointer parameters, where one of parameters is a pointer.
     * @post
     *   GetMethodCaller() == NULL given that pointer parameters are not supported.
     */
    template<class className, typename argType1, typename argType2>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 *));

    /**
     * @brief Constructor with two inputs where one of them is a pointer.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with two input pointer parameters, where one of parameters is a pointer.
     * @post
     *   GetMethodCaller() == NULL given that pointer parameters are not supported.
     */
    template<class className, typename argType1, typename argType2>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *, argType2));

    /**
     * @brief Constructor with three inputs where one of them is a pointer.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with three input pointer parameters, where one of parameters is a pointer.
     * @post
     *   GetMethodCaller() == NULL given that pointer parameters are not supported.
     */
    template<class className, typename argType1, typename argType2, typename argType3>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3 *));

    /**
     * @brief Constructor with three inputs where one of them is a pointer.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with three input pointer parameters, where one of parameters is a pointer.
     * @post
     *   GetMethodCaller() == NULL given that pointer parameters are not supported.
     */
    template<class className, typename argType1, typename argType2, typename argType3>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 *, argType3));

    /**
     * @brief Constructor with three inputs where one of them is a pointer.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with three input pointer parameters, where one of parameters is a pointer.
     * @post
     *   GetMethodCaller() == NULL given that pointer parameters are not supported.
     */
    template<class className, typename argType1, typename argType2, typename argType3>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *, argType2, argType3));

    /**
     * @brief Constructor with four inputs where one of them is a pointer.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four input pointer parameters, where one of parameters is a pointer.
     * @post
     *   GetMethodCaller() == NULL given that pointer parameters are not supported.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4 *));

    /**
     * @brief Constructor with four inputs where one of them is a pointer.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four input pointer parameters, where one of parameters is a pointer.
     * @post
     *   GetMethodCaller() == NULL given that pointer parameters are not supported.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3 *, argType4));

    /**
     * @brief Constructor with four inputs where one of them is a pointer.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four input pointer parameters, where one of parameters is a pointer.
     * @post
     *   GetMethodCaller() == NULL given that pointer parameters are not supported.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 *, argType3, argType4));

    /**
     * @brief Constructor with four inputs where one of them is a pointer.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four input pointer parameters, where one of parameters is a pointer.
     * @post
     *   GetMethodCaller() == NULL given that pointer parameters are not supported.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *, argType2, argType3, argType4));

    /**
     * @brief Constructor with zero inputs.
     * @tparam className is the class owning the target method.
     * @param[in] MethodPointer function with void parameters.
     * @post
     *   GetMethodCaller() != NULL.
     */
    template<class className>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(void));

    /**
     * @brief Constructor with one constant reference input parameter.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with one constant reference input parameter.
     * @post
     *   GetMethodCaller() != NULL.
     */
    template<class className, typename argType1>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &));

    /**
     * @brief Constructor with one reference output parameter.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with one reference output parameter.
     * @post
     *   GetMethodCaller() != NULL.
     */
    template<class className, typename argType1>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &));

    /**
     * @brief Constructor with one input parameter that is passed by copy.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with one input parameter that is passed by copy.
     * @post
     *   GetMethodCaller() != NULL.
     */
    template<class className, typename argType1>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1));

    /**
     * @brief Constructor with two parameters where the second is a constant reference input parameter.
     * @details The other parameter can be a constant reference, a parameter passed by copy or an output parameter.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with two parameters where the second is a constant reference input parameter.
     * @post
     *   GetMethodCaller() != NULL.
     */
    template<class className, typename argType1, typename argType2>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, const argType2 &));

    /**
     * @brief Constructor with two parameters where the second parameter is an output parameter.
     * @details The other parameter can be a constant reference, a parameter passed by copy or an output parameter.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with two parameters where the second is an output parameter.
     * @post
     *   GetMethodCaller() != NULL.
     */
    template<class className, typename argType1, typename argType2>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 &));

    /**
     * @brief Constructor with two parameters where the second is a parameter passed by copy.
     * @details The other parameter can be a constant reference, a parameter passed by copy or an output parameter.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with two parameters where the second is a parameter passed by copy.
     * @post
     *   GetMethodCaller() != NULL.
     */
    template<class className, typename argType1, typename argType2>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2));

    /**
     * @brief Helper function with template magic to resolve the first parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType2Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with two parameters where the first is a constant reference parameter.
     * @param[in] mask a bit-field which identifies if the second parameter is a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer2 function with the second parameter stripped of any information regarding its modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType2Stripped>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &, argType2),
                                                uint32 mask,
                                                void (className::*MethodPointer2)(argType2Stripped));
    /**
     * @brief Helper function with template magic to resolve the first parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType2Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with two parameters where the first is an output parameter.
     * @param[in] mask a bit-field which identifies if the second parameter is a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer2 function with the second parameter stripped of any information regarding its modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType2Stripped>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &, argType2),
                                                uint32 mask,
                                                void (className::*MethodPointer2)(argType2Stripped));

    /**
     * @brief Helper function with template magic to resolve the first parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType2Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with two parameters where the first is a parameter passed by copy.
     * @param[in] mask a bit-field which identifies if the second parameter is a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer2 function with the second parameter stripped of any information regarding its modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType2Stripped>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2),
                                                uint32 mask,
                                                void (className::*MethodPointer2)(argType2Stripped));

    /**
     * @brief Constructor with three parameters where the third is a constant reference input parameter.
     * @details The other parameters can be a constant reference, a parameter passed by copy or an output parameter.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with three parameters where the third is a constant reference input parameter.
     * @post
     *   GetMethodCaller() != NULL.
     */
    template<class className, typename argType1, typename argType2, typename argType3>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, const argType3 &));

    /**
     * @brief Constructor with three parameters where the third parameter is an output parameter.
     * @details The other parameters can be a constant reference, a parameter passed by copy or an output parameter.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with three parameters where the third parameter is an output parameter.
     * @post
     *   GetMethodCaller() != NULL.
     */
    template<class className, typename argType1, typename argType2, typename argType3>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3 &));

    /**
     * @brief Constructor with three parameters where the third is a parameter passed by copy.
     * @details The other parameters can be a constant reference, a parameter passed by copy or an output parameter.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with three parameters where the third is a parameter passed by copy.
     * @post
     *   GetMethodCaller() != NULL.
     */
    template<class className, typename argType1, typename argType2, typename argType3>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3));

    /**
     * @brief Helper function with template magic to resolve the second parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType3Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with three parameters where the second is a constant reference parameter.
     * @param[in] mask a bit-field which identifies if the third parameter is a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer2 function with the third parameter stripped of any information regarding its modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType3Stripped>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, const argType2 &, argType3),
                                                uint32 mask,
                                                void (className::*MethodPointer2)(argType3Stripped));

    /**
     * @brief Helper function with template magic to resolve the second parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType3Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with three parameters where the second is an output parameter.
     * @param[in] mask a bit-field which identifies if the third parameter is a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer2 function with the third parameter stripped of any information regarding its modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType3Stripped>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 &, argType3),
                                                uint32 mask,
                                                void (className::*MethodPointer2)(argType3Stripped));

    /**
     * @brief Helper function with template magic to resolve the second parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType3Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with three parameters where the second parameter is passed by copy.
     * @param[in] mask a bit-field which identifies if the third parameter is a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer2 function with the third parameter stripped of any information regarding its modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType3Stripped>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3),
                                                uint32 mask, void (className::*MethodPointer2)(argType3Stripped));

    /**
     * @brief Helper function with template magic to resolve the first parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType2Stripped is the type of the input parameter to be passed to the target method.
     * @tparam argType3Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with three parameters where the first parameter is a constant reference.
     * @param[in] mask a bit-field which identifies if the second and third parameters are a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer3 function with the third parameter stripped of any information regarding its modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType2Stripped, typename argType3Stripped>
    ClassMethodCaller *ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &, argType2, argType3),
                                                uint32 mask, void (className::*MethodPointer3)(argType2Stripped, argType3Stripped));

    /**
     * @brief Helper function with template magic to resolve the first parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType2Stripped is the type of the input parameter to be passed to the target method.
     * @tparam argType3Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with three parameters where the first parameter is an output parameter.
     * @param[in] mask a bit-field which identifies if the second and third parameters are a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer3 function with the third parameter stripped of any information regarding its modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType2Stripped, typename argType3Stripped>
    ClassMethodCaller *ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &, argType2, argType3),
                                                uint32 mask,
                                                void (className::*MethodPointer3)(argType2Stripped, argType3Stripped));

    /**
     * @brief Helper function with template magic to resolve the first parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType2Stripped is the type of the input parameter to be passed to the target method.
     * @tparam argType3Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with three parameters where the first parameter is passed by copy.
     * @param[in] mask a bit-field which identifies if the second and third parameters are a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer3 function with the third parameter stripped of any information regarding its modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType2Stripped, typename argType3Stripped>
    ClassMethodCaller *ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3),
                                                uint32 mask,
                                                void (className::*MethodPointer3)(argType2Stripped, argType3Stripped));

    /**
     * @brief Constructor with four parameters where the forth is a constant reference input parameter.
     * @details The other parameters can be a constant reference, a parameter passed by copy or an output parameter.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four parameters where the forth is a constant reference input parameter.
     * @post
     *   GetMethodCaller() != NULL.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, const argType4 &));

    /**
     * @brief Constructor with four parameters where the forth is an output parameter.
     * @details The other parameters can be a constant reference, a parameter passed by copy or an output parameter.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four parameters where the forth is an output parameter.
     * @post
     *   GetMethodCaller() != NULL.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4 &));

    /**
     * @brief Constructor with four parameters where the forth is a parameter passed by copy.
     * @details The other parameters can be a constant reference, a parameter passed by copy or an output parameter.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four parameters where the forth is a parameter passed by copy.
     * @post
     *   GetMethodCaller() != NULL.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4));

    /**
     * @brief Helper function with template magic to resolve the third parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @tparam argType4Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four parameters where the third is a constant reference parameter.
     * @param[in] mask a bit-field which identifies if the forth parameter is a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer2 function with the forth parameter stripped of any information regarding its modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType4Stripped>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, const argType3 &, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer2)(argType4Stripped));

    /**
     * @brief Helper function with template magic to resolve the third parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @tparam argType4Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four parameters where the third is an output parameter.
     * @param[in] mask a bit-field which identifies if the forth parameter is a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer2 function with the forth parameter stripped of any information regarding its modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType4Stripped>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3 &, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer2)(argType4Stripped));

    /**
     * @brief Helper function with template magic to resolve the third parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @tparam argType4Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four parameters where the third parameter is passed by copy.
     * @param[in] mask a bit-field which identifies if the forth parameter is a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer2 function with the forth parameter stripped of any information regarding its modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType4Stripped>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer2)(argType4Stripped));

    /**
     * @brief Helper function with template magic to resolve the second parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @tparam argType3Stripped is the type of the input parameter to be passed to the target method.
     * @tparam argType4Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four parameters where the second is a constant reference parameter.
     * @param[in] mask a bit-field which identifies if the third and forth parameter are a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer3 function with the third and forth parameters stripped of any information regarding their modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType3Stripped, typename argType4Stripped>
    ClassMethodCaller *ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, const argType2 &, argType3, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer3)(argType3Stripped, argType4Stripped));

    /**
     * @brief Helper function with template magic to resolve the second parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @tparam argType3Stripped is the type of the input parameter to be passed to the target method.
     * @tparam argType4Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four parameters where the second is an output parameter.
     * @param[in] mask a bit-field which identifies if the third and forth parameter are a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer3 function with the third and forth parameters stripped of any information regarding their modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType3Stripped, typename argType4Stripped>
    ClassMethodCaller *ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 &, argType3, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer3)(argType3Stripped, argType4Stripped));

    /**
     * @brief Helper function with template magic to resolve the second parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @tparam argType3Stripped is the type of the input parameter to be passed to the target method.
     * @tparam argType4Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four parameters where the second parameter is passed by copy.
     * @param[in] mask a bit-field which identifies if the third and forth parameter are a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer3 function with the third and forth parameters stripped of any information regarding their modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType3Stripped, typename argType4Stripped>
    ClassMethodCaller *ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer3)(argType3Stripped, argType4Stripped));

    /**
     * @brief Helper function with template magic to resolve the first parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @tparam argType2Stripped is the type of the input parameter to be passed to the target method.
     * @tparam argType3Stripped is the type of the input parameter to be passed to the target method.
     * @tparam argType4Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four parameters where the first parameter is a constant reference.
     * @param[in] mask a bit-field which identifies if the second, the third and forth parameter are a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer4 function with the second, third and forth parameters stripped of any information regarding their modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType2Stripped, typename argType3Stripped, typename argType4Stripped>
    ClassMethodCaller *ClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &, argType2, argType3, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer4)(argType2Stripped, argType3Stripped, argType4Stripped));

    /**
     * @brief Helper function with template magic to resolve the first parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @tparam argType2Stripped is the type of the input parameter to be passed to the target method.
     * @tparam argType3Stripped is the type of the input parameter to be passed to the target method.
     * @tparam argType4Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four parameters where the first is an output parameter.
     * @param[in] mask a bit-field which identifies if the second, the third and forth parameter are a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer4 function with the second, third and forth parameters stripped of any information regarding their modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType2Stripped, typename argType3Stripped, typename argType4Stripped>
    ClassMethodCaller *ClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &, argType2, argType3, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer4)(argType2Stripped, argType3Stripped, argType4Stripped));

    /**
     * @brief Helper function with template magic to resolve the first parameter type without doing all the possible combinations of parameter types.
     * @tparam className is the class owning the target method.
     * @tparam argType1 is the type of the input parameter to be passed to the target method.
     * @tparam argType2 is the type of the input parameter to be passed to the target method.
     * @tparam argType3 is the type of the input parameter to be passed to the target method.
     * @tparam argType4 is the type of the input parameter to be passed to the target method.
     * @tparam argType2Stripped is the type of the input parameter to be passed to the target method.
     * @tparam argType3Stripped is the type of the input parameter to be passed to the target method.
     * @tparam argType4Stripped is the type of the input parameter to be passed to the target method.
     * @param[in] MethodPointer function with four parameters where the first passed by copy.
     * @param[in] mask a bit-field which identifies if the second, the third and forth parameter are a constant reference, a parameter passed by copy or an output parameter.
     * @param[in] MethodPointer4 function with the second, third and forth parameters stripped of any information regarding their modifiers (const and reference). This is only used to
     * help the template find this function.
     * @return a pointer to a new ClassMethodCaller instance which knows how to call the MethodPointer.
     */
    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType2Stripped, typename argType3Stripped, typename argType4Stripped>
    ClassMethodCaller *ClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3,argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer4)(argType2Stripped, argType3Stripped, argType4Stripped));

    /**
     * @brief Destructor. Frees the ClassMethodCaller instance associated to this mapper.
     */
    virtual ~ClassMethodInterfaceMapper();

    /**
     * @brief Gets the method ClassMethodCaller registered against this mapper.
     * @return the method ClassMethodCaller registered against this mapper.
     */
    ClassMethodCaller *GetMethodCaller();

    /**
     * @brief Sets the name of method registered in this mapper.
     * @param[in] name the name of method registered in this mapper.
     */
    void SetMethodName(CCString const & name);

    /**
     * @brief Gets the name of method registered in this mapper.
     * @return the name of method registered in this mapper.
     */
    CCString GetMethodName() const;

private:

    /**
     * The class method caller
     */
    ClassMethodCaller *caller;

    /**
     * The name of the mapper.
     */
    CCString methodName;

/*lint -e{1712} This class is only used by CLASSMETHODREGISTER macro to register new methods.*/
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
namespace MARTe {

template<class className, typename argType1>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *)) {
    //static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
    caller = NULL;
}

template<class className, typename argType1, typename argType2>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 *)) {
    //static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
    caller = NULL;
}

template<class className, typename argType1, typename argType2>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *, argType2)) {
    //static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
    caller = NULL;
}

template<class className, typename argType1, typename argType2, typename argType3>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3 *)) {
    //static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
    caller = NULL;
}

template<class className, typename argType1, typename argType2, typename argType3>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 *, argType3)) {
    //static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
    caller = NULL;
}

template<class className, typename argType1, typename argType2, typename argType3>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *, argType2, argType3)) {
    //static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
    caller = NULL;
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4 *)) {
    //static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
    caller = NULL;
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3 *, argType4)) {
    //static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
    caller = NULL;
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 *, argType3, argType4)) {
    //static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
    caller = NULL;
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *, argType2, argType3, argType4)) {
    //static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
    caller = NULL;
}


template<class className>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(void)) {
    caller = new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(void), void, void, void, void>(MethodPointer, 0x0000);
}


template<class className, typename argType1>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &)) {
    caller = new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(const argType1 &), argType1, void, void, void>(MethodPointer, 0x0000);
}

template<class className, typename argType1>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &)) {
    caller = new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1 &), argType1, void, void, void>(MethodPointer, 0x1000);
}

template<class className, typename argType1>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1)) {
    caller = new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1), argType1, void, void, void>(MethodPointer, 0x0000);
}


template<class className, typename argType1, typename argType2>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, const argType2 &)) {
    void (className::*MethodPointer2)(argType2);
    caller = ClassMethodCallerCreate2(MethodPointer, 0x0000, MethodPointer2);
}

template<class className, typename argType1, typename argType2>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 &)) {
    void (className::*MethodPointer2)(argType2);
    caller = ClassMethodCallerCreate2(MethodPointer, 0x0100U, MethodPointer2);
}

template<class className, typename argType1, typename argType2>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2)) {
    void (className::*MethodPointer2)(argType2);
    caller = ClassMethodCallerCreate2(MethodPointer, 0x0000U, MethodPointer2);
}

template<class className, typename argType1, typename argType2, typename argType2Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &, argType2),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer2)(argType2Stripped)) {
    return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(const argType1 &, argType2), argType1, argType2Stripped, void, void>(MethodPointer, mask);
}

template<class className, typename argType1, typename argType2, typename argType2Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &, argType2),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer2)(argType2Stripped)) {
    return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1 &,argType2), argType1, argType2Stripped, void, void>(MethodPointer, mask | 0x1000);
}

template<class className, typename argType1, typename argType2, typename argType2Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer2)(argType2Stripped)) {
    return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1, argType2), argType1, argType2Stripped, void, void>(MethodPointer, mask);
}


template<class className, typename argType1, typename argType2, typename argType3>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, const argType3 &)) {
    void (className::*MethodPointer2)(argType3);
    caller = ClassMethodCallerCreate2(MethodPointer, 0x0000, MethodPointer2);
}

template<class className, typename argType1, typename argType2, typename argType3>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3 &)) {
    void (className::*MethodPointer2)(argType3);
    caller = ClassMethodCallerCreate2(MethodPointer, 0x0010, MethodPointer2);
}

template<class className, typename argType1, typename argType2, typename argType3>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3)) {
    void (className::*MethodPointer2)(argType3);
    caller = ClassMethodCallerCreate2(MethodPointer, 0x0000, MethodPointer2);
}


template<class className, typename argType1, typename argType2, typename argType3, typename argType3Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, const argType2 &, argType3),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer2)(argType3Stripped)) {
    void (className::*MethodPointer3)(argType2, argType3Stripped);
    return ClassMethodCallerCreate3(MethodPointer, mask, MethodPointer3);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType3Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 &, argType3),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer2)(argType3Stripped)) {
    void (className::*MethodPointer3)(argType2, argType3Stripped);
    return ClassMethodCallerCreate3(MethodPointer, mask | 0x0100, MethodPointer3);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType3Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer2)(argType3Stripped)) {
    void (className::*MethodPointer3)(argType2, argType3Stripped);
    return ClassMethodCallerCreate3(MethodPointer, mask, MethodPointer3);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType2Stripped, typename argType3Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &, argType2, argType3),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer3)(argType2Stripped, argType3Stripped)) {
    return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(const argType1 &, argType2, argType3), argType1, argType2Stripped, argType3Stripped, void>(MethodPointer, mask);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType2Stripped, typename argType3Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &, argType2, argType3),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer3)(argType2Stripped, argType3Stripped)) {
    return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1 &, argType2, argType3), argType1, argType2Stripped, argType3Stripped, void>(MethodPointer, mask | 0x1000);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType2Stripped, typename argType3Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer3)(argType2Stripped, argType3Stripped)) {
    return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1, argType2, argType3), argType1, argType2Stripped, argType3Stripped, void>(MethodPointer, mask);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, const argType4 &)) {
    void (className::*MethodPointer2)(argType4);
    caller = ClassMethodCallerCreate2(MethodPointer, 0x0000, MethodPointer2);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4 &)) {
    void (className::*MethodPointer2)(argType4);
    caller = ClassMethodCallerCreate2(MethodPointer, 0x0001, MethodPointer2);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
ClassMethodInterfaceMapper::ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4)) {
    void (className::*MethodPointer2)(argType4);
    caller = ClassMethodCallerCreate2(MethodPointer, 0x0000, MethodPointer2);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType4Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, const argType3 &, argType4),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer2)(argType4Stripped)) {
    void (className::*MethodPointer3)(argType3, argType4Stripped);
    return ClassMethodCallerCreate3(MethodPointer, mask, MethodPointer3);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType4Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3 &, argType4),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer2)(argType4Stripped)) {
    void (className::*MethodPointer3)(argType3, argType4Stripped);
    return ClassMethodCallerCreate3(MethodPointer, mask | 0x0010, MethodPointer3);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType4Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer2)(argType4Stripped)) {
    void (className::*MethodPointer3)(argType3, argType4Stripped);
    return ClassMethodCallerCreate3(MethodPointer, mask, MethodPointer3);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType3Stripped, typename argType4Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, const argType2 &, argType3, argType4),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer3)(argType3Stripped, argType4Stripped)) {
    void (className::*MethodPointer4)(argType2, argType3Stripped, argType4Stripped);
    return ClassMethodCallerCreate4(MethodPointer, mask, MethodPointer4);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType3Stripped, typename argType4Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 &, argType3, argType4),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer3)(argType3Stripped, argType4Stripped)) {
    void (className::*MethodPointer4)(argType2, argType3Stripped, argType4Stripped);
    return ClassMethodCallerCreate4(MethodPointer, mask | 0x0100, MethodPointer4);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType3Stripped, typename argType4Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer3)(argType3Stripped, argType4Stripped)) {
    void (className::*MethodPointer4)(argType2, argType3Stripped, argType4Stripped);
    return ClassMethodCallerCreate4(MethodPointer, mask, MethodPointer4);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType2Stripped, typename argType3Stripped, typename argType4Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &, argType2, argType3, argType4),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer4)(argType2Stripped, argType3Stripped, argType4Stripped)) {
    return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(const argType1 &, argType2, argType3, argType4), argType1, argType2Stripped, argType3Stripped, argType4Stripped>(MethodPointer, mask);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType2Stripped, typename argType3Stripped, typename argType4Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &, argType2, argType3, argType4),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer4)(argType2Stripped, argType3Stripped, argType4Stripped)) {
    return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1 &, argType2, argType3, argType4), argType1, argType2Stripped, argType3Stripped, argType4Stripped>(MethodPointer, mask | 0x1000);
}

template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType2Stripped, typename argType3Stripped, typename argType4Stripped>
ClassMethodCaller *ClassMethodInterfaceMapper::ClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4),
                                                                        uint32 mask,
                                                                        void (className::*MethodPointer4)(argType2Stripped, argType3Stripped, argType4Stripped)) {

    return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1, argType2, argType3, argType4), argType1, argType2Stripped, argType3Stripped, argType4Stripped>(MethodPointer, mask);
}


}

#endif /* CLASSMETHODINTERFACEMAPPER_H_ */

