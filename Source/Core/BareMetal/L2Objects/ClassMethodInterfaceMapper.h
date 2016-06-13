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

#include "ReturnType.h"
#include "Object.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {
/*
 class ClassMethodCaller {

 public:

 virtual ~ClassMethodCaller() {
 }
 virtual ReturnType Call(Object * context) {
 return ReturnType(false);
 }
 ;
 virtual ReturnType Call(Object * context,
 int x) {
 return ReturnType(false);
 }
 ;

 virtual ReturnType Call(Object * context,
 ReferenceContainer & x) {
 return ReturnType(false);
 }
 ;

 };

 template<class X, class Y>
 class ClassMethodCallerCT: public ClassMethodCaller {
 public:
 bool (X::*pFun)(Y ref);

 ClassMethodCallerCT(bool (X::*f)(Y ref)) {
 pFun = f;
 }

 virtual ReturnType Call(Object * context,
 Y ref) {
 ReturnType fr(true);

 X *actualContext = dynamic_cast<X *>(context);
 if (actualContext == NULL_PTR(X *)) {
 fr.error.notUnsupportedFeature = false;
 }

 if (fr.AllOk()) {
 fr.error.functionReturn = (actualContext->*pFun)(ref);
 }
 return fr;
 }
 };
 /*
 /**
 * TODO
 * */
class ClassMethodInterfaceMapper {


public:

    /**
     * TODO
     * */
    ClassMethodInterfaceMapper() {
        caller = NULL;};

        template <class C,typename T>
        ClassMethodInterfaceMapper(bool (C::*f)(T ref)) {
            caller = new ClassMethodCallerCT<C,T>(f);
        };

        template <typename T>
        ReturnType Call(Object *context,T ref) {
            ReturnType fr(true);
            fr.error.notUnsupportedFeature = false;
            if (caller != NULL ) fr = caller->Call(context,ref);
            return fr;
        };

        /**
         * TODO
         * */
        virtual ~ClassMethodInterfaceMapper() {
            if (caller != NULL) {
                delete caller;
            }
        };

    private:
        ClassMethodCaller *caller;

    };

    /*---------------------------------------------------------------------------*/
    /*                        Inline method definitions                          */
    /*---------------------------------------------------------------------------*/

}

class ClassMethodInterfaceMapper {
};
#endif /* CLASSMETHODINTERFACEMAPPER_H_ */

