/**
 * @file RuntimeEvaluatorTest.cpp
 * @brief Source file for class RuntimeEvaluatorTest
 * @date 04/05/2020
 * @author Nicolo Ferron
 * @author Dídac Magriñá
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
 * the class RuntimeEvaluatorTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RuntimeEvaluatorTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool RuntimeEvaluatorTest::TestIntegration() {

    CCString rpnCode=
            "READ A\n"
            "READ B\n"
            "SUB\n"
            "CONST float32 3.140000\n"
            "MUL\n"
            "WRITE C\n"
            "READ C\n"
            "CONST float32 0.500000\n"
            "POW\n"
            "TAN\n"
            "CAST int32\n"
            "WRITE D\n"
            "READ D\n"
            "READ C\n"
            "LOG\n"
            "CAST int32\n"
            "GT\n"
            "WRITE E\n"
            "READ N1\n"
            "READ N2\n"
            "SUB\n"
            "WRITE TEMP\n"
            "READ TEMP\n"
            "READ TEMP\n"
            "READ N3\n"
            "READ N4\n"
            "DIV\n"
            "ADD\n"
            "WRITE N5\n"
            "READ N5\n"
            "CAST int64\n"
            "GT\n"
            "WRITE F\n"
    ;

    CCString expectedDebug=
            "[line]-[stackPtr]-[codePtr]::[CODE] stack-in => stack-out\n"
            "1 - 0 - 0 :: READ  A => ((float32)2.000000)\n"
            "2 - 1 - 2 :: READ  B => ((float32)1.000000)\n"
            "3 - 2 - 4 :: SUB ((float32)1.000000,(float32)2.000000) => ((float32)1.000000)\n"
            "4 - 1 - 5 :: READ  float32 3.140000 => ((float32)3.140000)\n"
            "5 - 2 - 7 :: MUL ((float32)3.140000,(float32)1.000000) => ((float32)3.140000)\n"
            "6 - 1 - 8 :: WRITE  C((float32)3.140000)\n"
            "7 - 0 - 10 :: READ  C => ((float32)3.140000)\n"
            "8 - 1 - 12 :: READ  float32 0.500000 => ((float32)0.500000)\n"
            "9 - 2 - 14 :: POW ((float32)0.500000,(float32)3.140000) => ((float32)1.772004)\n"
            "10 - 1 - 15 :: TAN ((float32)1.772004) => ((float32)-4.902726)\n"
            "11 - 1 - 16 :: CAST ((float32)-4.902726) => ((int32)-4)\n"
            "12 - 1 - 17 :: WRITE  D((int32)-4)\n"
            "13 - 0 - 19 :: READ  D => ((int32)-4)\n"
            "14 - 1 - 21 :: READ  C => ((float32)3.140000)\n"
            "15 - 2 - 23 :: LOG ((float32)3.140000) => ((float32)1.144223)\n"
            "16 - 2 - 24 :: CAST ((float32)1.144223) => ((int32)1)\n"
            "17 - 2 - 25 :: GT ((int32)1,(int32)-4) => ((uint8)0)\n"
            "18 - 1 - 26 :: WRITE  E((uint8)0)\n"
            "19 - 0 - 28 :: READ  N1 => ((int8)22)\n"
            "20 - 1 - 30 :: READ  N2 => ((int8)31)\n"
            "21 - 2 - 32 :: SUB ((int8)31,(int8)22) => ((int32)-9)\n"
            "22 - 1 - 33 :: WRITE  TEMP((int32)-9)\n"
            "23 - 0 - 35 :: READ  TEMP => ((int32)-9)\n"
            "24 - 1 - 37 :: READ  TEMP => ((int32)-9)\n"
            "25 - 2 - 39 :: READ  N3 => ((int8)127)\n"
            "26 - 3 - 41 :: READ  N4 => ((int8)13)\n"
            "27 - 4 - 43 :: DIV ((int8)13,(int8)127) => ((int32)9)\n"
            "28 - 3 - 44 :: ADD ((int32)9,(int32)-9) => ((int32)0)\n"
            "29 - 2 - 45 :: WRITE  N5((int32)0)\n"
            "30 - 1 - 47 :: READ  N5 => ((int8)0)\n"
            "31 - 2 - 49 :: CAST ((int8)0) => ((int64)0)\n"
            "32 - 3 - 50 :: GT ((int64)0,(int32)-9) => ((uint8)0)\n"
            "33 - 1 - 51 :: WRITE  F((uint8)0)\n"
            "0 - 53 :: END\n"
    ;
    
    StreamString RPNCodeStream(rpnCode);
    RPNCodeStream.Seek(0u);

    RuntimeEvaluator context(RPNCodeStream);

    ErrorManagement::ErrorType ret;
    ErrorManagement::ErrorType fatalError(ErrorManagement::FatalError);

    
    ret = context.ExtractVariables();

    if (ret){
        uint32 index = 0U;
        VariableInformation *var;

        //VAR SCAN RESULT
        while(context.BrowseInputVariable(index,var)){
            index++;
            if (var->name == "A"){
                var->type = Float32Bit;
            }
            if (var->name == "B"){
                var->type = Float32Bit;
            }
            if (var->name == "N1"){
                var->type = SignedInteger8Bit;
            }
            if (var->name == "N2"){
                var->type = SignedInteger8Bit;
            }
            if (var->name == "N3"){
                var->type = SignedInteger8Bit;
            }
            if (var->name == "N4"){
                var->type = SignedInteger8Bit;
            }
        }

        index = 0;
        while(context.BrowseOutputVariable(index,var)){
            index++;
            if (var->name == "C"){
                var->type = Float32Bit;
            }
            if (var->name == "D"){
                var->type = SignedInteger32Bit;
            }
            if (var->name == "E"){
                var->type = UnsignedInteger8Bit;
            }
            if (var->name == "F"){
                var->type = UnsignedInteger8Bit;
            }
            if (var->name == "N5"){
                var->type = SignedInteger8Bit;
            }
        }
    }

    if (ret){
        //COMPILE
        RPNCodeStream.Seek(0u);
        ret = context.Compile();
    }

    if (ret){
        //ASSIGN INPUTS
        uint32 index = 0;
        VariableInformation *var;

        while(context.BrowseInputVariable(index,var)){
            if (var->name == "A"){
                float *x  = (float*)context.GetInputVariableMemory(index);
                *x = 2.0;
            }
            if (var->name == "B"){
                float *x  = (float*)context.GetInputVariableMemory(index);
                *x = 1.0;
            }
            if (var->name == "N1"){
                int8 *x  = (int8*)context.GetInputVariableMemory(index);
                *x = 22;
            }
            if (var->name == "N2"){
                int8 *x  = (int8*)context.GetInputVariableMemory(index);
                *x = 31;
            }
            if (var->name == "N3"){
                int8 *x  = (int8*)context.GetInputVariableMemory(index);
                *x = 127;
            }
            if (var->name == "N4"){
                int8 *x  = (int8*)context.GetInputVariableMemory(index);
                *x = 13;
            }
            index++;
        }
    }

    if (ret){
        //VAR ALLOCATION RESULT
        uint32 index = 0;
        VariableInformation *var;

        while(context.BrowseInputVariable(index,var)){
            //printf ("input  var %2i @%04x = %s \n",index,var->location,var->name.Buffer());
            index++;
        }

        index = 0;
        while(context.BrowseOutputVariable(index,var)){
            //printf ("output var %2i @%04x = %s\n",index,var->location,var->name.Buffer());
            index++;
        }
    }

    if (ret){
        //DECOMPILE
        StreamString RPNCodeRev;
        ret = context.DeCompile(RPNCodeRev,false);

        if ((ret) && (RPNCodeRev != rpnCode.GetList())){
            ret = fatalError;
        }
    }


    if (ret){
        //DEBUG MODE EXECUTION
        StreamString debugStream, debugLine, expectedDebugLine;
        char8 terminator;
        StreamString expectedDebugStream (expectedDebug);

        ret = context.Execute(RuntimeEvaluator::debugMode,&debugStream);
        debugStream.Seek(0u);
        expectedDebugStream.Seek(0u);

        while (ret && debugStream.GetToken(debugLine, "\n", terminator, "\n\r")){
            expectedDebugStream.GetToken(expectedDebugLine, "\n", terminator, "\n\r");
            if (debugLine != expectedDebugLine){
                ret = fatalError;
            } else{
                debugLine = "";
                expectedDebugLine = "";
            }
        }
    }

    return ret;
}

bool RuntimeEvaluatorTest::TestConstructor() {
    RuntimeEvaluator context("");
    bool ok = true;

    //Check that stackPtr is null
    uint32 value = 5;
    context.Peek(value);
    ok &= (value == 0);

    return ok;
}

bool RuntimeEvaluatorTest::TestGetPseudoCode() {

    bool ok = false;
    StreamString rpnCode = "CONST uint8 2\n"
                           "WRITE A\n";

    RuntimeEvaluator evaluator(rpnCode);

    ok = (evaluator.ExtractVariables() == ErrorManagement::NoError);

    ok &= evaluator.SetOutputVariableType("A", UnsignedInteger8Bit);

    ok &= (evaluator.Compile() == ErrorManagement::NoError);

    ok &= (evaluator.GetPseudoCode() == 8);  //READ OPERATION MEMORY POSITION
    ok &= (evaluator.GetPseudoCode() == 0);  //CONSTANT MEMORY POSITION
    ok &= (evaluator.GetPseudoCode() == 18); //WRITE OPERATION MEMORY POSITION

    return ok;
}

bool RuntimeEvaluatorTest::TestBrowseInputVariable(RuntimeEvaluator &evaluator, uint32 index, ErrorManagement::ErrorType expectedError, CCString expectedName) {

    bool ok = false;
    VariableInformation *var;

    ok = (expectedError == evaluator.BrowseInputVariable(index, var));

    if (expectedError) {
        ok &= (var->name == expectedName.GetList());
    }

    return ok;
}

bool RuntimeEvaluatorTest::TestBrowseOutputVariable(RuntimeEvaluator &evaluator, uint32 index, ErrorManagement::ErrorType expectedError, CCString expectedName) {

    bool ok = false;
    VariableInformation *var;

    ok = (expectedError == evaluator.BrowseOutputVariable(index, var));

    if (expectedError) {
        ok &= (var->name == expectedName.GetList());
    }

    return ok;
}

bool RuntimeEvaluatorTest::TestGetInputVariableMemory() {
    
    bool ret;
    
    StreamString rpnCode = "READ A\n"
                           "READ B\n"
                           "ADD\n"
                           "WRITE retValue1\n"
                           "READ C\n"
                           "READ D\n"
                           "ADD\n"
                           "WRITE retValue2\n"
                           "READ E\n"
                           "READ F\n"
                           "ADD\n"
                           "WRITE retValue3\n";
    
    RuntimeEvaluator evaluator(rpnCode);
    
    ret = evaluator.ExtractVariables();
    
    float64 B;
    float64 D;
    uint32  E;
    bool  isExternal[]        = { false,           true,        false,           true,        true,        false           };
    void* externalAddresses[] = { NULL_PTR(void*), (void*)(&B), NULL_PTR(void*), (void*)(&D), (void*)(&E), NULL_PTR(void*) };
    
    if (ret) {
        ret &= evaluator.SetInputVariableType("A", Float64Bit);
        ret &= evaluator.SetInputVariableType("B", Float64Bit);
        ret &= evaluator.SetInputVariableType("C", Float32Bit);
        ret &= evaluator.SetInputVariableType("D", Float32Bit);
        ret &= evaluator.SetInputVariableType("E", UnsignedInteger32Bit);
        ret &= evaluator.SetInputVariableType("F", UnsignedInteger32Bit);
        
        ret &= evaluator.SetInputVariableMemory("B", &B);
        ret &= evaluator.SetInputVariableMemory("D", &D);
        ret &= evaluator.SetInputVariableMemory("E", &E);
    }
    
    if (ret) {
        ret = evaluator.Compile();
    }
    
    if (ret) {
        
        uint32 index = 0U;
        VariableInformation* var;
        
        void *ptr1, *ptr2, *ptr3;
        
        while (evaluator.BrowseInputVariable(index, var) && ret) {
            
            if (isExternal[index]) {
                ptr1 = externalAddresses[index];
            }
            else {
                ptr1 = &(evaluator.Variable<void*>(var->location));
            }
            ptr2 = evaluator.GetInputVariableMemory(index);
            ptr3 = evaluator.GetInputVariableMemory(var->name);
            
            ret = (ptr1 == ptr2 && ptr2 == ptr3);
            
            index++;
        }
    }
    
    if (ret) {
        ret = (evaluator.GetInputVariableMemory(1000) == NULL_PTR(void*));
    }
    
    return ret;
    
}

bool RuntimeEvaluatorTest::TestGetOutputVariableMemory() {
    
    bool ret;
    
    StreamString rpnCode = "READ A\n"
                           "READ B\n"
                           "ADD\n"
                           "WRITE C\n"
                           "READ C\n"
                           "WRITE D\n"
                           "READ D\n"
                           "READ E\n"
                           "ADD\n"
                           "WRITE F\n"
                           "READ G\n"
                           "READ H\n"
                           "ADD\n"
                           "WRITE I\n"
                           "READ I\n"
                           "WRITE J\n";
    
    RuntimeEvaluator evaluator(rpnCode);
    
    ret = evaluator.ExtractVariables();
    
    float64 B;
    float64 D;
    int32   J;
    
    if (ret) {
        ret &= evaluator.SetInputVariableType("A", Float64Bit);
        ret &= evaluator.SetInputVariableType("B", Float64Bit);
        ret &= evaluator.SetInputVariableType("E", Float64Bit);
        ret &= evaluator.SetInputVariableType("G", SignedInteger32Bit);
        ret &= evaluator.SetInputVariableType("H", SignedInteger32Bit);
        
        ret &= evaluator.SetInputVariableMemory("B", &B);
    }
    
    if (ret) {
        ret &= evaluator.SetOutputVariableType("C", Float64Bit);
        ret &= evaluator.SetOutputVariableType("D", Float64Bit);
        ret &= evaluator.SetOutputVariableType("F", Float64Bit);
        // I not explicitly declared and thus internal
        ret &= evaluator.SetOutputVariableType("J", SignedInteger32Bit);
        
        ret &= evaluator.SetOutputVariableMemory("D", &D);
        ret &= evaluator.SetOutputVariableMemory("J", &J);
    }
    
    if (ret) {
        ret = evaluator.Compile();
    }
    
    bool isExternal[]         = {false, true,        false, false, true        };
    void* externalAddresses[] = {NULL,  (void*)(&D), NULL,  NULL,  (void*)(&J) };
    
    if (ret) {
        
        uint32 index = 0U;
        VariableInformation* var;
        
        void *ptr1, *ptr2, *ptr3;
        
        while (evaluator.BrowseOutputVariable(index, var) && ret) {
            
            if (isExternal[index]) {
                ptr1 = externalAddresses[index];
            }
            else {
                ptr1 = &(evaluator.Variable<void*>(var->location));
            }
            ptr2 = evaluator.GetOutputVariableMemory(index);
            ptr3 = evaluator.GetOutputVariableMemory(var->name);
            
            ret = (ptr1 == ptr2 && ptr2 == ptr3);
            
            index++;
        }
    }
    
    if (ret) {
        ret = (evaluator.GetOutputVariableMemory(1000) == NULL_PTR(void*));
    }
    
    return ret;
    
}

bool RuntimeEvaluatorTest::TestSetVariableType() {
    
    bool ret = false;
    
    StreamString rpnCode = "READ A\n"
                           "READ B\n"
                           "ADD\n"
                           "WRITE C\n"
                           "READ C\n"
                           "WRITE D\n";
    
    RuntimeEvaluator evaluator(rpnCode);
    
    ret = evaluator.ExtractVariables();
    
    if (ret) {
        ret = evaluator.SetInputVariableType("A", UnsignedInteger32Bit);
        if (ret) {
            ret = evaluator.SetInputVariableType(1u, UnsignedInteger64Bit);
        }
    }
    
    if (ret) {
        ret = evaluator.SetOutputVariableType(0u, UnsignedInteger64Bit);
        if (ret) {
            ret = evaluator.SetOutputVariableType("D", UnsignedInteger64Bit);
        }
    }
    
    if (ret) {
        ret = !evaluator.SetInputVariableType("Z", UnsignedInteger64Bit);
        if (ret) {
            ret = !evaluator.SetOutputVariableType("Z", UnsignedInteger64Bit);
        }
    }
    
    if (ret) {
        ret = evaluator.Compile();
    }

    TypeDescriptor  inTypes[] = {UnsignedInteger32Bit, UnsignedInteger64Bit};
    TypeDescriptor outTypes[] = {UnsignedInteger64Bit, UnsignedInteger64Bit};
    
    if (ret) {
        uint32 index = 0U;
        VariableInformation *var;

        while(evaluator.BrowseInputVariable(index,var) && ret) {
            ret = (var->type == inTypes[index]);
            index++;
        }
        
        index = 0U;
        
        while(evaluator.BrowseOutputVariable(index,var) && ret) {
            ret = (var->type == outTypes[index]);
            index++;
        }
    }
    
    return ret;
}

bool RuntimeEvaluatorTest::TestSetVariableMemory() {
    
    bool ret = false;
    
    StreamString rpnCode = "READ A\n"
                           "READ B\n"
                           "ADD\n"
                           "WRITE C\n"
                           "READ C\n"
                           "WRITE D\n";
    
    RuntimeEvaluator evaluator(rpnCode);
    
    ret = evaluator.ExtractVariables();
    
    uint32 A;
    uint64 B;
    uint64 C;
    uint64 D;
    
    if (ret) {
        ret &= evaluator.SetInputVariableType("A", UnsignedInteger32Bit);
        ret &= evaluator.SetInputVariableType("B", UnsignedInteger64Bit);
        
        ret &= evaluator.SetInputVariableMemory("A", &A);
        ret &= evaluator.SetInputVariableMemory(1u, &B);
    }
    
    if (ret) {
        ret &= evaluator.SetOutputVariableType("C", UnsignedInteger64Bit);
        ret &= evaluator.SetOutputVariableType("D", UnsignedInteger64Bit);
        
        ret &= evaluator.SetOutputVariableMemory(0u, &C);
        ret &= evaluator.SetOutputVariableMemory("D", &D);
    }
    
    if (ret) {
        ret = evaluator.Compile();
    }

    void*  inAddresses[] = {(void*)(&A), (void*)(&B)};
    void* outAddresses[] = {(void*)(&C), (void*)(&D)};
    
    if (ret) {
        uint32 index = 0U;
        VariableInformation *var;

        while(evaluator.BrowseInputVariable(index,var) && ret) {
            ret = (var->externalLocation == inAddresses[index]);
            index++;
        }
        
        index = 0U;
        
        while(evaluator.BrowseOutputVariable(index,var) && ret) {
            ret = (var->externalLocation == outAddresses[index]);
            index++;
        }
    }
    
    return ret;
}

bool RuntimeEvaluatorTest::TestVariable() {

    bool ok = false;
    StreamString rpnCode = "CONST uint8 2\n"
                           "CONST int64 5\n"
                           "ADD\n"
                           "WRITE A\n";

    RuntimeEvaluator evaluator(rpnCode);

    ok = (evaluator.ExtractVariables() == ErrorManagement::NoError);

    ok &= evaluator.SetOutputVariableType("A", SignedInteger64Bit);

    ok &= (evaluator.Compile() == ErrorManagement::NoError);

    ok &= (evaluator.Variable<uint8>(0) == 2);
    ok &= (evaluator.Variable<int64>(1) == 5);

    return ok;
}

bool RuntimeEvaluatorTest::TestPushPopPeek() {
    
    bool ret = false;
    
    // add variables to increase the size of the stack
    StreamString rpnCode = "READ A\n"
                           "READ B\n"
                           "READ C\n"
                           "READ D\n"
                           "READ E\n"
                           "READ F\n"
                           "READ G\n"
                           "READ H\n"
                           "READ I\n"
                           "READ J\n"
                           "READ K\n"
                           "READ L\n"
                           "ADD\n"
                           "ADD\n"
                           "ADD\n"
                           "ADD\n"
                           "ADD\n"
                           "ADD\n"
                           "ADD\n"
                           "ADD\n"
                           "ADD\n"
                           "ADD\n"
                           "ADD\n"
                           "WRITE I\n"
                           "";
    
    RuntimeEvaluator evaluator(rpnCode);
    
    ret = evaluator.ExtractVariables();
    
    if (ret) {
        ret = evaluator.SetInputVariableType("A", Float32Bit);
        ret = evaluator.SetInputVariableType("B", Float32Bit);
        ret = evaluator.SetInputVariableType("C", Float32Bit);
        ret = evaluator.SetInputVariableType("D", Float32Bit);
        ret = evaluator.SetInputVariableType("E", Float32Bit);
        ret = evaluator.SetInputVariableType("F", Float32Bit);
        ret = evaluator.SetInputVariableType("G", Float32Bit);
        ret = evaluator.SetInputVariableType("H", Float32Bit);
        ret = evaluator.SetInputVariableType("H", Float32Bit);
        ret = evaluator.SetInputVariableType("I", Float32Bit);
        ret = evaluator.SetInputVariableType("J", Float32Bit);
        ret = evaluator.SetInputVariableType("K", Float32Bit);
        ret = evaluator.SetInputVariableType("L", Float32Bit);
    }
    
    if (ret) {
        ret = evaluator.Compile();
    }
    
    // unsigned integers 
    uint8  pushVarUint8,  peekVarUint8,  popVarUint8;
    uint16 pushVarUint16, peekVarUint16, popVarUint16;
    uint32 pushVarUint32, peekVarUint32, popVarUint32;
    uint64 pushVarUint64, peekVarUint64, popVarUint64;
    
    pushVarUint8  = 255;
    pushVarUint16 = 65535;
    pushVarUint32 = 4294967295;
    pushVarUint64 = 18446744073709551615ul;
    
    evaluator.Push(pushVarUint8);
    evaluator.Push(pushVarUint16);
    evaluator.Push(pushVarUint32);
    evaluator.Push(pushVarUint64);
    
    if (ret) {
        evaluator.Peek(peekVarUint64);
        evaluator.Pop(popVarUint64);
        ret = ( (popVarUint64 == pushVarUint64) && (peekVarUint64 == pushVarUint64) );
    }
    if (ret) {
        evaluator.Peek(peekVarUint32);
        evaluator.Pop(popVarUint32);
        ret = ( (popVarUint32 == pushVarUint32) && (peekVarUint32 == pushVarUint32) );
    }
    if (ret) {
        evaluator.Peek(peekVarUint16);
        evaluator.Pop(popVarUint16);
        ret = ( (popVarUint16 == pushVarUint16) && (peekVarUint16 == pushVarUint16) );
    }
    if (ret) {
        evaluator.Peek(peekVarUint8);
        evaluator.Pop(popVarUint8);
        ret = ( (popVarUint8 == pushVarUint8) && (peekVarUint8 == pushVarUint8) );
    }
    
    // signed integers
    int8   pushVarInt8,  peekVarInt8,  popVarInt8;
    int16  pushVarInt16, peekVarInt16, popVarInt16;
    int32  pushVarInt32, peekVarInt32, popVarInt32;
    int64  pushVarInt64, peekVarInt64, popVarInt64;
    
    pushVarInt8  = -127;
    pushVarInt16 = 32767;
    pushVarInt32 = -2147483647;
    pushVarInt64 = 9223372036854775807;
    
    evaluator.Push(pushVarInt8);
    evaluator.Push(pushVarInt16);
    evaluator.Push(pushVarInt32);
    evaluator.Push(pushVarInt64);
    
    if (ret) {
        evaluator.Peek(peekVarInt64);
        evaluator.Pop(popVarInt64);
        ret = ( (popVarInt64 == pushVarInt64) && (peekVarInt64 == pushVarInt64) );
    }
    if (ret) {
        evaluator.Peek(peekVarInt32);
        evaluator.Pop(popVarInt32);
        ret = ( (popVarInt32 == pushVarInt32) && (peekVarInt32 == pushVarInt32) );
    }
    if (ret) {
        evaluator.Peek(peekVarInt16);
        evaluator.Pop(popVarInt16);
        ret = ( (popVarInt16 == pushVarInt16) && (peekVarInt16 == pushVarInt16) );
    }
    if (ret) {
        evaluator.Peek(peekVarInt8);
        evaluator.Pop(popVarInt8);
        ret = ( (popVarInt8 == pushVarInt8) && (peekVarInt8 == pushVarInt8) );
    }
    
    // floats
    float32 pushVarFloat32, peekVarFloat32, popVarFloat32;
    float64 pushVarFloat64, peekVarFloat64, popVarFloat64;
    
    pushVarFloat32 = -340282346638528859811704183484516925440.0;
    pushVarFloat64 = 340282346638528859811704183484516925440.0;
    
    evaluator.Push(pushVarFloat32);
    evaluator.Push(pushVarFloat64);
    
    if (ret) {
        evaluator.Peek(peekVarFloat64);
        evaluator.Pop(popVarFloat64);
        ret = ( (popVarFloat64 == pushVarFloat64) && (peekVarFloat64 == pushVarFloat64) );
    }
    if (ret) {
        evaluator.Peek(peekVarFloat32);
        evaluator.Pop(popVarFloat32);
        ret = ( (popVarFloat32 == pushVarFloat32) && (peekVarFloat32 == pushVarFloat32) );
    }
    
    // mixed types
    evaluator.Push(pushVarFloat64);
    evaluator.Push(pushVarInt32);
    evaluator.Push(pushVarUint64);
    evaluator.Push(pushVarUint8);
    evaluator.Push(pushVarFloat32);
    evaluator.Push(pushVarInt32);
    evaluator.Push(pushVarInt16);
    evaluator.Push(pushVarFloat32);
    
    if (ret) {
        evaluator.Peek(peekVarFloat32);
        evaluator.Pop(popVarFloat32);
        ret = ( (popVarFloat32 == pushVarFloat32) && (peekVarFloat32 == pushVarFloat32) );
    }
    if (ret) {
        evaluator.Peek(peekVarInt16);
        evaluator.Pop(popVarInt16);
        ret = ( (popVarInt16 == pushVarInt16) && (peekVarInt16 == pushVarInt16) );
    }
    if (ret) {
        evaluator.Peek(peekVarInt32);
        evaluator.Pop(popVarInt32);
        ret = ( (popVarInt32 == pushVarInt32) && (peekVarInt32 == pushVarInt32) );
    }
    if (ret) {
        evaluator.Peek(peekVarFloat32);
        evaluator.Pop(popVarFloat32);
        ret = ( (popVarFloat32 == pushVarFloat32) && (peekVarFloat32 == pushVarFloat32) );
    }
    if (ret) {
        evaluator.Peek(peekVarUint8);
        evaluator.Pop(popVarUint8);
        ret = ( (popVarUint8 == pushVarUint8) && (peekVarUint8 == pushVarUint8) );
    }
    if (ret) {
        evaluator.Peek(peekVarUint64);
        evaluator.Pop(popVarUint64);
        ret = ( (popVarUint64 == pushVarUint64) && (peekVarUint64 == pushVarUint64) );
    }
    if (ret) {
        evaluator.Peek(peekVarInt32);
        evaluator.Pop(popVarInt32);
        ret = ( (popVarInt32 == pushVarInt32) && (peekVarInt32 == pushVarInt32) );
    }
    if (ret) {
        evaluator.Peek(peekVarFloat64);
        evaluator.Pop(popVarFloat64);
        ret = ( (popVarFloat64 == pushVarFloat64) && (peekVarFloat64 == pushVarFloat64) );
    }
    
    return ret;
    
}

bool RuntimeEvaluatorTest::TestExecute(CCString rpnCode, ErrorManagement::ErrorType expectedError, RuntimeEvaluator::executionMode mode) {
    
    bool ok = false;
    
    RuntimeEvaluator context(rpnCode);
    
    ok = (context.ExtractVariables() == ErrorManagement::NoError);
    
    VariableListElement* element;
    
    for (uint32 i = 0; i < usedInputVariables.GetSize() && ok; i++) {
        
        ok = usedInputVariables.Peek(i, element);
        ok = context.SetInputVariableType(element->var.name, element->var.type);
        if (ok && (element->var.externalLocation != NULL)) {
            ok = context.SetInputVariableMemory(element->var.name, element->var.externalLocation);
        }
        
    }
    
    for (uint32 i = 0; i < usedOutputVariables.GetSize() && ok; i++) {
        
        ok = usedOutputVariables.Peek(i, element);
        ok = context.SetOutputVariableType(element->var.name, element->var.type);
        if (ok && (element->var.externalLocation != NULL)) {
            ok = context.SetOutputVariableMemory(element->var.name, element->var.externalLocation);
        }
        
    }
    
    if (ok) {
        ok = (context.Compile() == ErrorManagement::NoError);
    }
    
    for (uint32 i = 0; i < usedInputVariables.GetSize() && ok; i++) {
        
        ok = usedInputVariables.Peek(i, element);
        if (ok && (element->var.externalLocation == NULL)) {
            *((float32*)context.GetInputVariableMemory(element->var.name)) = (float32)(element->expectedValue);
        }
        
    }
    
    ErrorManagement::ErrorType ret = context.Execute(mode);
    
    ok = (ret == expectedError);
    
    if (expectedError == ErrorManagement::NoError) {
        for (uint32 i = 0; i < usedOutputVariables.GetSize() && ok; i++) {
            
            ok = usedOutputVariables.Peek(i, element);
            
            float32 expectedValue = (float32)(element->expectedValue);
            float32 actualValue   = *((float32*)context.GetOutputVariableMemory(element->var.name));
            ok = (expectedValue == actualValue);
            
        }
    }
    
    return ok;
    
}

bool RuntimeEvaluatorTest::TestExtractVariables(CCString rpnCode, ErrorManagement::ErrorType expectedError) {
    RuntimeEvaluator evaluator(rpnCode);
    bool ok = true;
    VariableInformation *var;

    ok &= (evaluator.ExtractVariables() == expectedError);

    if (expectedError) {
        for (uint32 i = 0; (ok) && (evaluator.BrowseInputVariable(i, var)); ++i) {
            ok &= RemoveMatchingVariable(var, expectedInputVariables);
        }

        ok &= (expectedInputVariables.ListSize() == 0);

        for (uint32 i = 0; (ok) && (evaluator.BrowseOutputVariable(i, var)); ++i) {
            ok &= RemoveMatchingVariable(var, expectedOutputVariables);
        }

        ok &= (expectedOutputVariables.ListSize() == 0);
    }

    return ok;
}

bool RuntimeEvaluatorTest::TestCompile(RuntimeEvaluator &evaluator, ErrorManagement::ErrorType expectedError, uint32 expectedDataSize) {
    bool ok = true;
    VariableInformation *var;

    ok &= (evaluator.Compile() == expectedError);

    if (expectedError) {
        for (uint32 i = 0; (ok) && (evaluator.BrowseInputVariable(i, var)); ++i) {
            ok &= RemoveMatchingVariable(var, expectedInputVariables);
        }

        ok &= (expectedInputVariables.ListSize() == 0);

        for (uint32 i = 0; (ok) && (evaluator.BrowseOutputVariable(i, var)); ++i) {
            ok &= RemoveMatchingVariable(var, expectedOutputVariables);
        }

        ok &= (expectedOutputVariables.ListSize() == 0);

        for (uint32 i = 0; (ok) && (i < expectedCodeMemory.GetNumberOfChildren()); ++i){
            StreamString type;
            CodeMemoryElement location, expectedLocation;

            location = evaluator.GetPseudoCode();
            expectedCodeMemory.MoveToChild(i);
            expectedCodeMemory.Read("Type", type);

            if (type == "Function") {
                ok &= RecordMatchesExpectedFunction(functionRecords[location]);

            } else if (type == "Variable") {
                expectedCodeMemory.Read("Location", expectedLocation);
                ok &= (expectedLocation == location);
            }

            expectedCodeMemory.MoveToRoot();
        }

        ok &= (evaluator.dataMemory.GetNumberOfElements() == expectedDataSize);
    }

    return ok;
}

void RuntimeEvaluatorTest::SetTestInputVariable(CCString name, TypeDescriptor type, void *externalLocation, float64 expectedVarValue) {
    SetTestVariable(usedInputVariables, name, type, externalLocation, expectedVarValue);
}

void RuntimeEvaluatorTest::SetTestOutputVariable(CCString name, TypeDescriptor type, void *externalLocation, float64 expectedVarValue) {
    SetTestVariable(usedOutputVariables, name, type, externalLocation, expectedVarValue);
}

void RuntimeEvaluatorTest::SetTestVariable(StaticList<VariableListElement*>& list, CCString name, TypeDescriptor type, void *externalLocation, float64 expectedVarValue) {
    
    VariableListElement* element = new VariableListElement();
    
    element->var.name = name;
    element->var.type = type;
    element->var.externalLocation = externalLocation;
    
    element->expectedValue = expectedVarValue;
    
    list.Add(element);
}

void RuntimeEvaluatorTest::AddExpectedInputVariable(CCString name, TypeDescriptor type, DataMemoryAddress location, void *externalLocation, bool variableUsed) {
    AddExpectedVariable(expectedInputVariables, name, type, location, externalLocation, variableUsed);
}

void RuntimeEvaluatorTest::AddExpectedOutputVariable(CCString name, TypeDescriptor type, DataMemoryAddress location, void *externalLocation, bool variableUsed) {
    AddExpectedVariable(expectedOutputVariables, name, type, location, externalLocation, variableUsed);
}

void RuntimeEvaluatorTest::AddExpectedFunctionInMemory(StreamString name, StreamString inputType, StreamString outputType) {
    uint32 lastChildren = expectedCodeMemory.GetNumberOfChildren();
    StreamString functionName;

    functionName.Printf("Element%i", lastChildren);
    expectedCodeMemory.CreateRelative(functionName.Buffer());

    expectedCodeMemory.Write("Type", "Function");

    expectedCodeMemory.Write("Name", name);

    if (inputType != "void"){
        expectedCodeMemory.Write("Input", inputType);
    }

    if (outputType != "void"){
        expectedCodeMemory.Write("Output", outputType);
    }

    expectedCodeMemory.MoveToRoot();
}

void RuntimeEvaluatorTest::AddExpectedVariableInMemory(CodeMemoryElement location) {
    uint32 lastChildren = expectedCodeMemory.GetNumberOfChildren();
    StreamString functionName;

    functionName.Printf("Element%i", lastChildren);
    expectedCodeMemory.CreateRelative(functionName.Buffer());

    expectedCodeMemory.Write("Type", "Variable");

    expectedCodeMemory.Write("Location", location);

    expectedCodeMemory.MoveToRoot();
}

bool RuntimeEvaluatorTest::RemoveMatchingVariable(const VariableInformation *var, LinkedListHolderT<VariableInformation> &varList) {
    bool removed = false;

    for (uint32 i = 0; (!removed) && (i < varList.ListSize()); ++i) {
        if (VariablesMatch(var, varList.ListPeek(i))) {
            varList.ListExtract(i);
            removed = true;
        }
    }

    return removed;
}

bool RuntimeEvaluatorTest::VariablesMatch(const VariableInformation *var1, const VariableInformation *var2) {
    bool match = true;

    match &= (var1->name == var2->name);
    match &= (var1->type == var2->type);
    match &= (var1->location == var2->location);
    match &= (var1->externalLocation == var2->externalLocation);
    match &= (var1->variableUsed == var2->variableUsed);

    return match;
}

void RuntimeEvaluatorTest::AddExpectedVariable(LinkedListHolderT<VariableInformation> &varList, CCString name, TypeDescriptor type, DataMemoryAddress location, void *externalLocation, bool variableUsed) {
    VariableInformation * var = new VariableInformation();

    var->name = name;
    var->type = type;
    var->location = location;
    var->externalLocation = externalLocation;
    var->variableUsed = variableUsed;

    varList.ListAdd(var);
}

bool RuntimeEvaluatorTest::RecordMatchesExpectedFunction(RuntimeEvaluatorFunction &functionRecord) {
    bool match = true;
    StreamString expectedName, expectedInput, expectedOutput;
    Vector<TypeDescriptor> inputs, outputs;

    expectedCodeMemory.Read("Name", expectedName);
    match &= (functionRecord.GetName() == expectedName);

    inputs = functionRecord.GetInputTypes();
    if (expectedCodeMemory.Read("Input", expectedInput)) {
        match &= (inputs.GetNumberOfElements() == 1);
        match &= (expectedInput == TypeDescriptor::GetTypeNameFromTypeDescriptor(inputs[0]));
    } else {
        match &= (inputs.GetNumberOfElements() == 0);
    }

    outputs = functionRecord.GetOutputTypes();
    if (expectedCodeMemory.Read("Output", expectedOutput)) {
        match &= (outputs.GetNumberOfElements() == 1);
        match &= (expectedOutput == TypeDescriptor::GetTypeNameFromTypeDescriptor(outputs[0]));
    } else {
        match &= (outputs.GetNumberOfElements() == 0);
    }

    return match;
}

void MockRead(RuntimeEvaluator &context) {
    float32 variableHolder = 0.F;
    context.Push(variableHolder);
    context.Push(variableHolder);
    context.Push(variableHolder);
}

void MockWrite(RuntimeEvaluator &context) {
    float32 variableHolder = 0.F;
    context.Pop(variableHolder);
    context.Pop(variableHolder);
    context.Pop(variableHolder);
}

void MockExecutionError(RuntimeEvaluator &context) {
    context.runtimeError.syntaxError = true;
}

void MockOutOfRangeExecutionError(RuntimeEvaluator &context) {
    context.runtimeError.outOfRange = true;
}
