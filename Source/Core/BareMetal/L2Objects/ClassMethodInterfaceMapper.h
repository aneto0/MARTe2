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

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Implementation of an object which stores and calls a class method.
 * @details The class allows to create an implicit class method caller whose
 * actual target method is inferred from the constructor (this class has 3
 * templated constructors, which allow to bind the each instance with a target
 * method). The target method can have a prototype without arguments or with
 * one argument at most (with by reference and by copy versions).
 */
class DLL_API ClassMethodInterfaceMapper: public LinkedListable {

public:

// unsupported feature trapping

    template<class className, typename argType1>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *)) {
        //    static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template<class className, typename argType1, typename argType2>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,
                                                                                      argType2 *)) {
        //    static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template<class className, typename argType1, typename argType2>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *,
                                                                                      argType2)) {
        //    static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template<class className, typename argType1, typename argType2, typename argType3>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,
                                                                                      argType2,
                                                                                      argType3 *)) {
        //    static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template<class className, typename argType1, typename argType2, typename argType3>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,
                                                                                      argType2 *,
                                                                                      argType3)) {
        //    static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template<class className, typename argType1, typename argType2, typename argType3>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *,
                                                                                      argType2,
                                                                                      argType3)) {
        //    static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,
                                                                                      argType2,
                                                                                      argType3,
                                                                                      argType4 *)) {
        //    static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,
                                                                                      argType2,
                                                                                      argType3 *,
                                                                                      argType4)) {
        //    static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1,
                                                                                      argType2 *,
                                                                                      argType3,
                                                                                      argType4)) {
        //    static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 *,
                                                                                      argType2,
                                                                                      argType3,
                                                                                      argType4)) {
        //    static_assert(false, "Do not use ClassMethodCallerCreate with method having pointer parameters!");
        caller = NULL;
    }

    // 0 params

    template<class className>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(void)) {
        caller = new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(void), void, void, void, void>(MethodPointer, 0x0000);
    }

    // 1 params

    template<class className, typename argType1>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &)) {
        caller = new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(const argType1 &), argType1, void, void, void>(MethodPointer,
                                                                                                                                            0x0000);
    }

    template<class className, typename argType1>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &)) {
        caller = new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1 &), argType1, void, void, void>(MethodPointer, 0x1000);
    }

    template<class className, typename argType1>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1)) {
        caller = new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1), argType1, void, void, void>(MethodPointer, 0x0000);
    }

    // 2 params

    template<class className, typename argType1, typename argType2>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, const argType2 &)) {
        void (className::*MethodPointer2)(argType2);
        caller = ClassMethodCallerCreate2(MethodPointer, 0x0000, MethodPointer2);
    }

    template<class className, typename argType1, typename argType2>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 &)) {
        void (className::*MethodPointer2)(argType2);
        caller = ClassMethodCallerCreate2(MethodPointer, 0x0100u, MethodPointer2);
    }

    template<class className, typename argType1, typename argType2>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2)) {
        void (className::*MethodPointer2)(argType2);
        caller = ClassMethodCallerCreate2(MethodPointer, 0x0000u, MethodPointer2);
    }

    template<class className, typename argType1, typename argType2, typename argType22>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &, argType2), uint32 mask, void (className::*MethodPointer2)(argType22)) {
        return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(const argType1 &, argType2), argType1, argType22, void, void>(MethodPointer, mask);
    }

    template<class className, typename argType1, typename argType2, typename argType22>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &, argType2), uint32 mask, void (className::*MethodPointer2)(argType22)) {
        return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1 &, argType2), argType1, argType22, void, void>(MethodPointer, mask | 0x1000);
    }

    template<class className, typename argType1, typename argType2, typename argType22>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2), uint32 mask, void (className::*MethodPointer2)(argType22)) {
        return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1, argType2), argType1, argType22, void, void>(MethodPointer, mask);
    }

    // 3 params

    template<class className, typename argType1, typename argType2, typename argType3>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, const argType3 &)) {
        void (className::*MethodPointer2)(argType3);
        caller = ClassMethodCallerCreate2(MethodPointer, 0x0000, MethodPointer2);
    }

    template<class className, typename argType1, typename argType2, typename argType3>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3 &)) {
        void (className::*MethodPointer2)(argType3);
        caller = ClassMethodCallerCreate2(MethodPointer, 0x0010, MethodPointer2);
    }

    template<class className, typename argType1, typename argType2, typename argType3>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3)) {
        void (className::*MethodPointer2)(argType3);
        caller = ClassMethodCallerCreate2(MethodPointer, 0x0000, MethodPointer2);
    }




    template<class className, typename argType1, typename argType2, typename argType3, typename argType33>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, const argType2 &, argType3),
                                                uint32 mask,
                                                void (className::*MethodPointer2)(argType33)) {
        void (className::*MethodPointer3)(argType2, argType33);
        return ClassMethodCallerCreate3(MethodPointer, mask, MethodPointer3);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType33>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 &, argType3),
                                                uint32 mask,
                                                void (className::*MethodPointer2)(argType33)) {
        void (className::*MethodPointer3)(argType2, argType33);
        return ClassMethodCallerCreate3(MethodPointer, mask | 0x0100, MethodPointer3);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType33>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3),
                                                uint32 mask,
                                                void (className::*MethodPointer2)(argType33)) {
        void (className::*MethodPointer3)(argType2, argType33);
        return ClassMethodCallerCreate3(MethodPointer, mask, MethodPointer3);
    }



    template<class className, typename argType1, typename argType2, typename argType3, typename argType22, typename argType33>
    ClassMethodCaller *ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &, argType2, argType3),
                                                uint32 mask,
                                                void (className::*MethodPointer3)(argType22, argType33)) {
        return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(const argType1 &, argType2, argType3), argType1, argType22, argType33, void>(MethodPointer, mask);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType22, typename argType33>
    ClassMethodCaller *ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &, argType2, argType3),
                                                uint32 mask,
                                                void (className::*MethodPointer3)(argType22, argType33)) {
        return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1 &, argType2,  argType3), argType1, argType22, argType33, void>(MethodPointer, mask | 0x1000);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType22, typename argType33>
    ClassMethodCaller *ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3),
                                                uint32 mask,
                                                void (className::*MethodPointer3)(argType22, argType33)) {
        return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1, argType2, argType3), argType1, argType22, argType33, void>(MethodPointer, mask);
    }

    // 4 params

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, const argType4 &)) {
        void (className::*MethodPointer2)(argType4);
        caller = ClassMethodCallerCreate2(MethodPointer, 0x0000, MethodPointer2);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4 &)) {
        void (className::*MethodPointer2)(argType4);
        caller = ClassMethodCallerCreate2(MethodPointer, 0x0001, MethodPointer2);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4>
    ClassMethodInterfaceMapper(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4)) {
        void (className::*MethodPointer2)(argType4);
        caller = ClassMethodCallerCreate2(MethodPointer, 0x0000, MethodPointer2);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType44>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, const argType3 &, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer2)(argType44)) {
        void (className::*MethodPointer3)(argType3, argType44);
        return ClassMethodCallerCreate3(MethodPointer, mask, MethodPointer3);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType44>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3 &, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer2)(argType44)) {
        void (className::*MethodPointer3)(argType3, argType44);
        return ClassMethodCallerCreate3(MethodPointer, mask | 0x0010, MethodPointer3);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType44>
    ClassMethodCaller *ClassMethodCallerCreate2(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer2)(argType44)) {
        void (className::*MethodPointer3)(argType3, argType44);
        return ClassMethodCallerCreate3(MethodPointer, mask, MethodPointer3);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType33, typename argType44>
    ClassMethodCaller *ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, const argType2 &, argType3, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer3)(argType33, argType44)) {
        void (className::*MethodPointer4)(argType2, argType33, argType44);
        return ClassMethodCallerCreate4(MethodPointer, mask, MethodPointer4);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType33, typename argType44>
    ClassMethodCaller *ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2 &, argType3, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer3)(argType33, argType44)) {
        void (className::*MethodPointer4)(argType2, argType33, argType44);
        return ClassMethodCallerCreate4(MethodPointer, mask | 0x0100, MethodPointer4);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType33, typename argType44>
    ClassMethodCaller *ClassMethodCallerCreate3(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer3)(argType33, argType44)) {
        void (className::*MethodPointer4)(argType2, argType33, argType44);
        return ClassMethodCallerCreate4(MethodPointer, mask, MethodPointer4);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType22, typename argType33, typename argType44>
    ClassMethodCaller *ClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(const argType1 &, argType2, argType3, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer4)(argType22, argType33, argType44)) {
        return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(const argType1 &, argType2, argType3, argType4), argType1, argType22, argType33, argType44>(MethodPointer, mask);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType22, typename argType33, typename argType44>
    ClassMethodCaller *ClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(argType1 &, argType2, argType3, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer4)(argType22, argType33, argType44)) {
        return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1 &, argType2, argType3, argType4), argType1, argType22, argType33, argType44>(MethodPointer, mask | 0x1000);
    }

    template<class className, typename argType1, typename argType2, typename argType3, typename argType4, typename argType22, typename argType33, typename argType44>
    ClassMethodCaller *ClassMethodCallerCreate4(ErrorManagement::ErrorType (className::*MethodPointer)(argType1, argType2, argType3, argType4),
                                                uint32 mask,
                                                void (className::*MethodPointer4)(argType22, argType33, argType44)) {
        return new ClassMethodCallerT<className, ErrorManagement::ErrorType (className::*)(argType1, argType2, argType3, argType4), argType1, argType22, argType33, argType44>(MethodPointer, mask);
    }

    /**
     *
     */
    ClassMethodCaller *GetMethod() {
        return caller;
    }

    /**
     * @brief Destructor
     */
    virtual ~ClassMethodInterfaceMapper();

    /**
     * TODO
     */
    void SetName(CCString name) {
        methodName = name;
    }

    CCString GetMethodName() {
        return methodName;
    }

private:

    /**
     * The class method caller
     */
    ClassMethodCaller *caller;

    /**
     * TODO
     */
    CCString methodName;

};

}

#endif /* CLASSMETHODINTERFACEMAPPER_H_ */

