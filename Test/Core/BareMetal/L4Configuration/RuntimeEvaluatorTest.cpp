/**
 * @file RuntimeEvaluator.cpp
 * @brief Source file for class PseudoCode
 * @date 04/05/2015
 * @author RFX
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
 * the class PseudoCodeTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <cstdio>
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
//bool RuntimeEvaluatorTest::TestContextConstructor() {
    //RuntimeEvaluatorTest::Context context;

    ///* Right after construction variablesMemoryPtr is NULL leading to a segmentation fault
    //uint32 variable;
    //variable = context.Variable<uint32>(0)
    //if (variable != 0) {
        //return false;
    //}*/

    ///* Right after construction codeMemoryPtr is NULL leading to a segmentation fault
    //CodeMemoryElement element;
    //context.GetPseudoCode(element);
    //if (element != 0) {
        //return false;
    //}*/

    //uint32 value = 5;
    //context.Peek<uint32>(value);
    //if (value != 5) {
        //return false;
    //}

    //return (context.startOfVariables == 0);
//}

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
            printf ("input  var %2i @%04x = %s \n",index,var->location,var->name.Buffer());
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
            printf ("output var %2i @%04x = %s\n",index,var->location,var->name.Buffer());
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

        ret = context.Execute(RuntimeEvaluator::debugMode,&debugStream,0);
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

bool RuntimeEvaluatorTest::TestGetInputVariableMemory() {
    
    bool ret;
    
    StreamString rpnCode = "READ A\n"
                           "READ B\n"
                           "ADD\n"
                           "WRITE C\n";
    
    RuntimeEvaluator evaluator(rpnCode);
    
    ret = evaluator.ExtractVariables();
    
    if (ret) {
        evaluator.SetInputVariableType("A", Float64Bit);
        evaluator.SetInputVariableType("B", Float64Bit);
    }
    
    if (ret) {
        evaluator.SetOutputVariableType("C", Float64Bit);
    }
    
    if (ret) {
        ret = evaluator.Compile();
    }
    
    char8* evaluatorMemoryAddress = reinterpret_cast<char8 *>(evaluator.GetVariablesMemory());
    uint32 numberOfVariables = 2U;
    uint32 offsets[] = { sizeof(float64), sizeof(float64) };
    StreamString names[] = {"A", "B"};
    
    if (ret) {
        uint32 i;
        uint32 offset = 0u;
        
        for (i = 0; (i < numberOfVariables) && (ret); i++) {
            ret = (evaluator.GetInputVariableMemory(i) == (evaluatorMemoryAddress + offset));
            if (ret) {
                ret = (evaluator.GetInputVariableMemory(names[i]) == (evaluatorMemoryAddress + offset));
            }
            offset += offsets[i];
        }
    }
    if (ret) {
        ret = (evaluator.GetInputVariableMemory(1000) == NULL_PTR(void*));
    }
    
    // TODO add cases where externalLocation is used
    
    return ret;
    
}

bool RuntimeEvaluatorTest::TestGetOutputVariableMemory() {
    
    bool ret;
    
    StreamString rpnCode = "READ A\n"
                           "READ B\n"
                           "ADD\n"
                           "WRITE C\n"
                           "READ C\n"
                           "WRITE D\n";
    
    RuntimeEvaluator evaluator(rpnCode);
    
    ret = evaluator.ExtractVariables();
    
    if (ret) {
        evaluator.SetInputVariableType("A", Float64Bit);
        evaluator.SetInputVariableType("B", Float64Bit);
    }
    
    if (ret) {
        evaluator.SetOutputVariableType("C", Float64Bit);
        evaluator.SetOutputVariableType("D", Float64Bit);
    }
    
    if (ret) {
        ret = evaluator.Compile();
    }
    
    char8 *evaluatorMemoryAddress = reinterpret_cast<char8 *>(evaluator.GetVariablesMemory());
    uint32 numberOfVariables = 2U;
    uint23 inputVariableMemory = sizeof(float64) + sizeof(float64);
    uint32 offsets[] = { sizeof(float64), sizeof(float64) };
    StreamString names[] = {"C", "D"};
    
    if (ret) {
        uint32 i;
        uint32 offset = 0u;
        for (i = 0; (i < numberOfVariables) && (ret); i++) {
            ret = (evaluator.GetOutputVariableMemory(i) == (evaluatorMemoryAddress + inputVariableMemory + offset));
            if (ret) {
                ret = (evaluator.GetOutputVariableMemory(names[i]) == (evaluatorMemoryAddress + inputVariableMemory  + offset));
            }
            offset += offsets[i];
        }
    }
    if (ret) {
        ret = (evaluator.GetOutputVariableMemory(1000) == NULL_PTR(void*));
    }
    
    // TODO add cases where externalLocation is used
    
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
    
    // TODO?
    // 1. no segfault if stack is not initialised (before Compile())
    // 2. no segfault if output variable is not suitable to hold the Pop()ed/Peek()ed one
    
    return ret;
    
}

bool RuntimeEvaluatorTest::TestExtractVariables(CCString rpnCode) {

    
    StreamString RPNCodeStream(rpnCode);
    
    RuntimeEvaluator context(RPNCodeStream);

    ErrorManagement::ErrorType ret;
    ErrorManagement::ErrorType fatalError(ErrorManagement::FatalError);

    
    ret = context.ExtractVariables();
    
    return ret;
}

bool RuntimeEvaluatorTest::TestError(CCString rpnCode,
                                     ErrorManagement::ErrorType expectedError
                                    ) {
    
    StreamString RPNCodeStream(rpnCode);
    
    RuntimeEvaluator evaluator(RPNCodeStream);
    
    ErrorManagement::ErrorType ok;
    ErrorManagement::ErrorType ret = ErrorManagement::NoError;
    
    // 1. try ExtractVariabler()
    
    ok = evaluator.ExtractVariables();
    if (!ok) {
        ret = ErrorManagement::SyntaxError;
        printf("Failed at ExtractVariables().\n");
    }
    
    // 2. try Compile()
    
    if (ok) {
        uint32 index = 0U;
        VariableInformation *var;

        while(evaluator.BrowseInputVariable(index,var)) {
            evaluator.SetInputVariableType(index, Float64Bit);
            index++;
        }
        
    }
    
    if (ok) {
        ok = evaluator.Compile();
        if (!ok) {
            ret = ErrorManagement::InitialisationError;
            printf("Failed at Compile().\n");
        }
    }
    
    // 3. try Execute()
    
    if (ok){
        uint32 index = 0;
        VariableInformation *var;
        
        while(evaluator.BrowseInputVariable(index,var)){
            float64 *x = (float64*)evaluator.GetInputVariableMemory(index);
            *x = 1.0;
            index++;
        }
    }
    
    if (ok) {
        ok = evaluator.Execute();
        if (!ok) {
            ret = ErrorManagement::FatalError;
            printf("Failed at Execute().\n");
        }
    }
    
    if (ret == ErrorManagement::NoError) {
        
        printf("NoError, but should generate one.\n");
        
        // usually generates a segfault
        //StreamString retVar = "ret";
        //float64* x = (float64*)(evaluator.GetOutputVariableMemory(retVar));
        //printf("Result: %d\n", *x);
    }
    
    ok = (ret == expectedError);
    
    return ok;
    
}

bool RuntimeEvaluatorTest::TestExpression(CCString rpnCode, float64 valueArray[]) {

    
    StreamString RPNCodeStream(rpnCode);
    
    RuntimeEvaluator context(RPNCodeStream);

    ErrorManagement::ErrorType ret;
    ErrorManagement::ErrorType fatalError(ErrorManagement::FatalError);

    
    ret = context.ExtractVariables();

    if (ret){
        uint32 index = 0U;
        VariableInformation *var;

        //ASSIGN TYPE TO VARIABLES
        while(context.BrowseInputVariable(index,var)) {
            context.SetInputVariableType(index, Float32Bit);
            index++;
        }

        index = 0;
        while(context.BrowseOutputVariable(index,var)){
            context.SetOutputVariableType(index, Float32Bit);
            index++;
        }
    }
    
    if (ret){
        //COMPILE
        RPNCodeStream.Seek(0u);
        ret = context.Compile();
    }
    
    //if (ret){
        //ret = (context.startOfVariables == 2) ? ret : fatalError;
        //ret = (context.dataMemory.GetNumberOfElements() == 13) ? ret : fatalError;
        //ret = (context.codeMemory.GetSize() == 45) ? ret : fatalError;
        //ret = (context.stack.GetNumberOfElements() == 4) ? ret : fatalError;
    //}

    if (ret){
        //ASSIGN VALUES TO VARIABLES
        uint32 index = 0;
        VariableInformation *var;
        
        while(context.BrowseInputVariable(index,var)){
            
            // one way to retrieve the address
            //float *x  = reinterpret_cast<float *>(&context.dataMemory[var->location]);
            
            // one more way (by var name)
            //float *x = (float*)context.GetInputVariableMemory(var->name);
            
            
            // yet another way (by var index, fastest way)
            float64 *x = (float64*)context.GetInputVariableMemory(index);
            
            *x = valueArray[index];
            
            index++;
        }
    }
    
    if (ret){
        uint32 index = 0U;
        VariableInformation *var;
        
        printf ("--- Allocation ---\n");
        //AFTER VAR ALLOCATION
        while(context.BrowseInputVariable(index,var)){
            printf ("input  var %2i %s = %f \n",index,var->name.Buffer(),*((float64*)context.GetInputVariableMemory(index)));
            index++;
        }

        index = 0;
        while(context.BrowseOutputVariable(index,var)){
            printf ("output var %2i %s = %f \n",index,var->name.Buffer(),*((float64*)context.GetInputVariableMemory(index)));
            index++;
        }
    }

    if (ret){
        //DECOMPILE
        StreamString RPNCodeRev;
        ret = context.DeCompile(RPNCodeRev, false);

        if ((ret) && (RPNCodeRev != rpnCode.GetList())){
            ret = fatalError;
            printf ("decompilation error");
        }
    }


    if (ret){
        //DEBUG MODE EXECUTION
        //StreamString debugStream, debugLine, expectedDebugLine;
        StreamString debugStream;
        //char8 terminator;
        //StreamString expectedDebugStream (expectedDebug);

        ret = context.Execute(RuntimeEvaluator::debugMode,&debugStream,0);
        debugStream.Seek(0u);
        //expectedDebugStream.Seek(0u);
        
        printf ("debug stream:\n%s\n", debugStream.Buffer());
        
        //while (ret && debugStream.GetToken(debugLine, "\n", terminator, "\n\r")){
            //expectedDebugStream.GetToken(expectedDebugLine, "\n", terminator, "\n\r");
            //if (debugLine != expectedDebugLine){
                //ret = fatalError;
                //printf ("debug error");
            //} else{
                //debugLine = "";
                //expectedDebugLine = "";
            //}
        //}
    }
    
    if (ret){
        uint32 index = 0U;
        VariableInformation *var;
        
        printf (rpnCode);
        printf ("--- Results ---\n");
        //EXECUTE() RESULTS
        while(context.BrowseInputVariable(index,var)){
            printf ("input  var %2i %s = %f \n",index,var->name.Buffer(),*((float64*)context.GetInputVariableMemory(index)));
            index++;
        }

        index = 0;
        while(context.BrowseOutputVariable(index,var)){
            printf ("output var %2i %s = %f \n",index,var->name.Buffer(),*((float64*)context.GetInputVariableMemory(index)));
            index++;
        }
    }

    return ret;
}

/*---------------------------------------------------------------------------*/
/*    ↑ N                   CODE LINE DO NOT CROSS                     D ↓   */
/*---------------------------------------------------------------------------*/

bool RuntimeEvaluatorTest::TestConstructor() {
    RuntimeEvaluator context("");
    bool ok = true;

    /*
    // Right after construction variablesMemoryPtr is NULL leading to a segmentation fault
    uint32 variable;
    variable = context.Variable<uint32>(0);
    ok &= (variable == 0);

    // Right after construction codeMemoryPtr is NULL leading to a segmentation fault
    CodeMemoryElement element;
    element = context.GetPseudoCode();
    ok &= (element == 0);
    */

    uint32 value = 5;
    context.Peek(value);
    ok &= (value == 5);

    return ok;
}

bool RuntimeEvaluatorTest::TestExtractVariables(CCString rpnCode, ErrorManagement::ErrorType expectedError) {
    RuntimeEvaluator context(rpnCode);
    bool ok = true;
    uint32 variableIndex = 0;
    VariableInformation *var;

    ok &= (context.ExtractVariables() == expectedError);

    while (ok && context.BrowseInputVariable(variableIndex++, var)) {
        ok &= RemoveMatchingVariable(var, expectedInputVariables);
    }

    ok &= (expectedInputVariables.ListSize() == 0);

    return ok;
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

void RuntimeEvaluatorTest::AddExpectedInput(CCString name, TypeDescriptor type, DataMemoryAddress location, void *externalLocation, bool variableUsed) {
    VariableInformation * var = new VariableInformation();

    var->name = name;
    var->type = type;
    var->location = location;
    var->externalLocation = externalLocation;
    var->variableUsed = variableUsed;

    expectedInputVariables.ListAdd(var);
}
