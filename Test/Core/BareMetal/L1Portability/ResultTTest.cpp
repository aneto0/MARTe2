/**
 * @file ResultTTest.cpp
 * @brief Source file for class ResultTTest
 * @date 02/12/2015
 * @author Giuseppe ferrò
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
 * the class ResultTTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ResultTTest.h"
#include "StringHelper.h"
#include "StreamString.h"
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;
void NullErrorProcessFunction(const ErrorManagement::ErrorInformation &errorInfo,
                              const char8 * const errorDescription) {
}

ErrorManagement::ErrorProcessFunctionType errorMessageProcessFunction = &NullErrorProcessFunction;

bool ResultTTest::TestConstructor(){
    uint32 t=10;
    ResultT<uint32> test(t);
    return test.GetValue() == 10;
}

bool ResultTTest::TestCopyConstructor(){
    uint32 t=10;
    ResultT<uint32> test(t);
    ResultT<uint32> copy(test);
    return copy.GetValue() == 10;
}



bool ResultTTest::TestDestructor(){
    uint32 t=10;
    ResultT<uint32> *test = new ResultT<uint32>(t);
    bool ok = test->GetValue() == 10;
    delete test;
    return ok;
}

bool ResultTTest::TestSetError(){
    float32 t=1.5;
    ResultT<float32> test(t);
    test.SetError(ErrorManagement::FatalError, "Failed", ERR_ARGS);
    return !((bool)test);
}
    
bool ResultTTest::TestPrintError(){
    StreamString c="Custom Error";
    ResultT<StreamString> test(c);
    test.SetError(ErrorManagement::FatalError, c.Buffer(), ERR_ARGS);
    test.PrintError();
    return test.GetValue() == c;
}

bool ResultTTest::TestGetValue(){
    const char8 *expected[] = {"One", "Two", "Three"};
    Vector<StreamString> c(3);
    for(uint32 i=0u; i<3u; i++){
        c[i] = expected[i];
    }
    ResultT< Vector<StreamString> > test(c);

    bool ok = true;

    Vector<StreamString> res = test.GetValue();
    for(uint32 i=0u; i<3u; i++){
        ok &= (res[i] == expected[i]);
    }

    if(ok){
        //by reference
        test.GetValue()[0] = "Four"; 
        ok = (test.GetValue()[0] == "Four"); 
    }

    if(ok){
        uint32 typeint = 3;
        ResultT<AnyType> anyt(typeint);
        ok = *(uint32*)(anyt.GetValue().GetDataPointer()) == typeint;
    }
    
    return ok;
}
  
bool ResultTTest::TestBoolOperator(){
    float32 t=1.5;
    ResultT<float32> test(t);
    bool ok = (bool)test;

    if(ok){
        test.SetError(ErrorManagement::FatalError, "Failed", ERR_ARGS);
        ok == !((bool)test);
    }

    return ok;
}


bool ResultTTest::TestAssignOperator_Type(){
    uint32 t=10;
    ResultT<uint32> test(t);

    bool ok = test.GetValue() == t;
    ok &= (bool)test;
    test.SetError(ErrorManagement::FatalError, "Failed", ERR_ARGS);
    ok &= !((bool)test);
    if(ok){
        uint32 t2 = 20;
        test = t2;
        ok = (test.GetValue() == t2);
        ok &= (bool) test;
    }
 
    return ok;
}

bool ResultTTest::TestAssignOperator_Result(){
    uint32 t1=10;
    ResultT<uint32> test1(t1);

    uint32 t2=20;
    ResultT<uint32> test2(t2);

    bool ok = test1.GetValue() != test2.GetValue();
    if(ok){
        test1 = test2;
        ok = test1.GetValue() == test2.GetValue();
        ok &= (bool)test1;
        ok &= (bool)test2;
        test1.SetError(ErrorManagement::FatalError, "Failed", ERR_ARGS);
        ok &= !((bool)test1);
        ok &= (bool)test2;
        test2 = test1;
        ok &= !((bool)test2);
    }
 
    return ok;
}

    
bool ResultTTest::TestEqualOperator_Type(){
    uint32 t1=1;
    ResultT<uint32> test1(t1); 
    uint32 t2=1;

    bool ok = test1 == t2;
    test1.SetError(ErrorManagement::FatalError, "Failed", ERR_ARGS);
    ok &= test1 == t2;

    return ok;
}

bool ResultTTest::TestEqualOperator_Result(){
    uint32 t1=1;
    ResultT<uint32> test1(t1); 
    uint32 t2=1;
    ResultT<uint32> test2(t2); 

    bool ok = test1 == test2;
    if(ok){
        test1.SetError(ErrorManagement::FatalError, "Failed", ERR_ARGS);
        ok = !(test1 == test2);
        test2.SetError(ErrorManagement::FatalError, "Failed 2", ERR_ARGS);
        ok &= !(test1 == test2);
        test2.SetError(ErrorManagement::FatalError, "Failed", ERR_ARGS);
        ok &= (test1 == test2);
    }
    return ok;
}

bool ResultTTest::TestDifferentOperator_Type(){
    uint32 t1=1;
    ResultT<uint32> test1(t1); 
    uint32 t2=2;

    return test1 != t2;
}

bool ResultTTest::TestDifferentOperator_Result(){
    uint32 t1=1;
    ResultT<uint32> test1(t1); 
    uint32 t2=2;
    ResultT<uint32> test2(t2); 

    bool ok = test1 != test2;
    if(ok){
        test2 = t1;
        ok = !(test1 != test2);
        test1.SetError(ErrorManagement::FatalError, "Failed", ERR_ARGS);
        ok = (test1 != test2);
        test2.SetError(ErrorManagement::FatalError, "Failed 2", ERR_ARGS);
        ok &= (test1 != test2);
        test2.SetError(ErrorManagement::FatalError, "Failed", ERR_ARGS);
        ok &= !(test1 != test2);
    }
    return ok;
}

