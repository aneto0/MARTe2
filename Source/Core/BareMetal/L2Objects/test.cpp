/**
 * @file test.cpp
 * @brief Source file for class test
 * @date Aug 26, 2016
 * @author fsartori
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

 * @details This source file contains the definition of all the methods for
 * the class test (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassMethodCallerFT.h"
#include "Object.h"
#include "StreamI.h"


namespace MARTe{

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/


class Dummy: public Object{
    CLASS_REGISTER_DECLARATION()
public:

    ErrorManagement::ErrorType f1(){
        return true;
    }
    ErrorManagement::ErrorType f2(float x){
        return true;
    }
    ErrorManagement::ErrorType f2x(float &x){
        return true;
    }
    ErrorManagement::ErrorType f2xx(const float &x){
        return true;
    }
    ErrorManagement::ErrorType f2xxx(const float *x){
        return true;
    }
    ErrorManagement::ErrorType f3(float x,int i){
        return true;
    }
    ErrorManagement::ErrorType f4(float x,int i,const char *p){
        return true;
    }
    ErrorManagement::ErrorType f5(float x,int i,const char *p,bool zeta){
        return true;
    }
    ErrorManagement::ErrorType f6(StructuredDataI *si){
        return true;
    }
    ErrorManagement::ErrorType f7(ReferenceContainer &rc){
        return true;

    }
    ErrorManagement::ErrorType f8(StreamI *s){
        return true;

    }

};


void test(){
    Object *p;
    StreamI *s;
    ReferenceContainer rc;
    StructuredDataI *sI;
    XClassMethodCaller *c[] = {
            XClassMethodCallerCreate(&Dummy::f1),
            XClassMethodCallerCreate(&Dummy::f2),
            XClassMethodCallerCreate(&Dummy::f2x),
            XClassMethodCallerCreate(&Dummy::f2xx),
            XClassMethodCallerCreate(&Dummy::f2xxx),
            XClassMethodCallerCreate(&Dummy::f3),
            XClassMethodCallerCreate(&Dummy::f4),
            XClassMethodCallerCreate(&Dummy::f5),
            XClassMethodCallerCreate(&Dummy::f6),
            XClassMethodCallerCreate(&Dummy::f7),
            XClassMethodCallerCreate(&Dummy::f8)
    };


    XClassMethodInterfaceMapper cmim[] = {
            &Dummy::f1,
            &Dummy::f2,
            &Dummy::f2x,
            &Dummy::f2xx,
            &Dummy::f2xxx,
            &Dummy::f3,
            &Dummy::f4,
            &Dummy::f5,
            &Dummy::f6,
            &Dummy::f7,
            &Dummy::f8
    };

    c[0]->Call(p);
    c[1]->Call(p,s);
    c[2]->Call(p,rc);
    c[3]->Call(p,sI);
    c[4]->Call(p);



    cmim[0].GetMethod()->Call(p);
    cmim[1].GetMethod()->Call(p,s);
    cmim[2].GetMethod()->Call(p,rc);
    cmim[3].GetMethod()->Call(p,sI);
    cmim[4].GetMethod()->Call(p);

}
	
}
