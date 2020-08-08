/**
 * @file RuntimeEvaluatorFunctionTest.cpp
 * @brief Source file for class RuntimeEvaluatorFunctionTest
 * @date 06/05/2020
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
 * the class RuntimeEvaluatorFunctionTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RuntimeEvaluatorFunctionTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool RuntimeEvaluatorFunctionTest::TestRegisterFunction() {

    TypeDescriptor types[] = {UnsignedInteger8Bit, Float64Bit};
    bool ok;
    RuntimeEvaluator context("");

    RuntimeEvaluatorFunction functionUT("Test", 1, 1, types, MockFunction);

    uint32 initialAvailableFunctions = availableFunctions;
    RegisterFunction(functionUT);

    ok = (availableFunctions == initialAvailableFunctions + 1);

    RuntimeEvaluatorFunction lastFunctionRecord = functionRecords[availableFunctions - 1];

    ok &= (lastFunctionRecord.GetName() == "Test");

    Vector<TypeDescriptor> inputs = lastFunctionRecord.GetInputTypes();
    ok &= (inputs.GetNumberOfElements() == 1);
    ok &= (inputs[0] == UnsignedInteger8Bit);

    Vector<TypeDescriptor> outputs = lastFunctionRecord.GetOutputTypes();
    ok &= (outputs.GetNumberOfElements() == 1);
    ok &= (outputs[0] == Float64Bit);

    context.runtimeError = ErrorManagement::FatalError;
    lastFunctionRecord.ExecuteFunction(context);
    ok &= (context.runtimeError.ErrorsCleared());

    //Prevent that functions registered in this test affect following tests
    availableFunctions = initialAvailableFunctions;

    return ok;
}

bool RuntimeEvaluatorFunctionTest::TestRegisterFunctionMaxFunctions() {

    TypeDescriptor types[] = {UnsignedInteger8Bit, Float64Bit};
    bool ok;

    uint32 initialAvailableFunctions = availableFunctions;

    for (uint32 i = initialAvailableFunctions; i <= maxFunctions; ++i) {
        StreamString functionName;
        functionName.Printf("Test%u", i);
        RuntimeEvaluatorFunction functionUT(functionName.Buffer(), 1, 1, types, MockFunction);
        RegisterFunction(functionUT);
    }

    ok = (availableFunctions == maxFunctions);

    for (uint32 i = initialAvailableFunctions; i < maxFunctions; ++i) {
        StreamString expectedfunctionName;
        expectedfunctionName.Printf("Test%u", i);
        ok &= (expectedfunctionName == functionRecords[i].GetName());
    }

    //Prevent that functions registered in this test affect following tests
    availableFunctions = initialAvailableFunctions;

    return ok;
}

bool RuntimeEvaluatorFunctionTest::TestFindPCodeAndUpdateTypeStack(const CCString functionName, bool expectedReturn, CodeMemoryElement expectedCode) {

    bool ok;
    CodeMemoryElement code;
    DataMemoryAddress dataStackSize;
    StaticStack<TypeDescriptor,32> typeStack;

    ok = (FindPCodeAndUpdateTypeStack(code, functionName, typeStack, false, dataStackSize) == expectedReturn);
    if (expectedReturn) {
        ok &= (code == expectedCode);
    }

    return ok;
}

bool RuntimeEvaluatorFunctionTest::TestDefaultConstructor() {

    RuntimeEvaluatorFunction functionUT;
    bool ok;

    ok = (functionUT.GetName() == "");
    ok &= (functionUT.GetInputTypes().GetNumberOfElements() == 0);
    ok &= (functionUT.GetOutputTypes().GetNumberOfElements() == 0);

    return ok;
}

bool RuntimeEvaluatorFunctionTest::TestFullConstructor() {

    TypeDescriptor types[] = {UnsignedInteger8Bit, Float64Bit};
    bool ok;
    RuntimeEvaluator context("");

    RuntimeEvaluatorFunction functionUT("Test", 1, 1, types, MockFunction);

    ok = (functionUT.GetName() == "Test");

    Vector<TypeDescriptor> inputs = functionUT.GetInputTypes();
    ok &= (inputs.GetNumberOfElements() == 1);
    ok &= (inputs[0] == UnsignedInteger8Bit);

    Vector<TypeDescriptor> outputs = functionUT.GetOutputTypes();
    ok &= (outputs.GetNumberOfElements() == 1);
    ok &= (outputs[0] == Float64Bit);

    context.runtimeError = ErrorManagement::FatalError;
    functionUT.ExecuteFunction(context);
    ok &= (context.runtimeError.ErrorsCleared());

    return ok;
}

bool RuntimeEvaluatorFunctionTest::TestGetName() {

    bool ok;

    RuntimeEvaluatorFunction functionUT("MyFunction", 0, 0, NULL_PTR(TypeDescriptor*), NULL_PTR(Function));

    ok = (functionUT.GetName() == "MyFunction");

    return ok;
}

bool RuntimeEvaluatorFunctionTest::TestGetTypes(uint16 numberOfInputs, uint16 numberOfOutputs, TypeDescriptor* types) {

    bool ok;

    RuntimeEvaluatorFunction functionUT("Test", numberOfInputs, numberOfOutputs, types, NULL_PTR(Function));

    Vector<TypeDescriptor> inputTypes = functionUT.GetInputTypes();
    ok = (inputTypes.GetNumberOfElements() == numberOfInputs);
    for (uint16 i = 0; (ok) && (i < numberOfInputs); ++i) {
        ok &= (inputTypes[i] == types[i]);
    }

    Vector<TypeDescriptor> outputTypes = functionUT.GetOutputTypes();
    ok &= (outputTypes.GetNumberOfElements() == numberOfOutputs);
    for (uint16 i = 0; (ok) && (i < numberOfOutputs); ++i) {
        ok &= (outputTypes[i] == types[numberOfInputs + i]);
    }

    return ok;
}

bool RuntimeEvaluatorFunctionTest::TestFunctionTypes(CCString functionName, uint8 numberOfInputs, uint8 numberOfOutputs) {
    bool ok = true;

    for (uint32 i = 0; (i < availableFunctions) && (ok); ++i) {
        RuntimeEvaluatorFunction functionRecord = functionRecords[i];
        Vector<TypeDescriptor> inputTypes = functionRecord.GetInputTypes();
        Vector<TypeDescriptor> outputTypes = functionRecord.GetOutputTypes();

        if ((functionRecord.GetName() == functionName.GetList()) &&
            (inputTypes.GetNumberOfElements() == numberOfInputs) &&
            (outputTypes.GetNumberOfElements() == numberOfOutputs)) {
            CCString foundName;

            ok &= FindTypesInCdb(foundName, inputTypes, outputTypes, expectedFunctionTypesCdb);
            if (ok) {
                expectedFunctionTypesCdb.Delete(foundName);
            }
        }
    }

    ok &= (expectedFunctionTypesCdb.GetNumberOfChildren() == 0);

    return ok;

}

bool RuntimeEvaluatorFunctionTest::FindTypesInCdb(CCString &foundName, Vector<TypeDescriptor> &inputTypes, Vector<TypeDescriptor> &outputTypes, ConfigurationDatabase &typesCdb) {

    bool found = false, moved = false;

    for (uint32 i= 0; (i < typesCdb.GetNumberOfChildren()) && (!found); ++i) {
        bool equals = true;

        typesCdb.MoveToChild(i);
        foundName = typesCdb.GetName();
        moved = typesCdb.MoveRelative("Inputs");

        for (uint32 j = 0; (equals) && (j < inputTypes.GetNumberOfElements()); ++j) {
            StreamString argumentName, typeName;
            argumentName.Printf("arg%i", j + 1);
            typesCdb.Read(argumentName.Buffer(), typeName);
            equals &= (inputTypes[j] == TypeDescriptor::GetTypeDescriptorFromTypeName(typeName.Buffer()));
        }

        if (moved){
            typesCdb.MoveToAncestor(1);
        }
        moved = typesCdb.MoveRelative("Outputs");

        for (uint32 j = 0; (equals) && (j < outputTypes.GetNumberOfElements()); ++j) {
            StreamString argumentName, typeName;
            argumentName.Printf("arg%i", j + 1);
            typesCdb.Read(argumentName.Buffer(), typeName);
            equals &= (outputTypes[j] == TypeDescriptor::GetTypeDescriptorFromTypeName(typeName.Buffer()));
        }

        found = equals;
        typesCdb.MoveToRoot();
    }

    return found;
}

void RuntimeEvaluatorFunctionTest::AddExpectedFunction1In(StreamString inputTypeName) {

    StreamString inputs[] = {inputTypeName};
    StreamString outputs[] = {};
    AddExpectedFunction(1, inputs, 0, outputs);

}

void RuntimeEvaluatorFunctionTest::AddExpectedFunction1Out(StreamString outputTypeName) {

    StreamString inputs[] = {};
    StreamString outputs[] = {outputTypeName};
    AddExpectedFunction(0, inputs, 1, outputs);

}

void RuntimeEvaluatorFunctionTest::AddExpectedFunction1In1Out(StreamString inputTypeName, StreamString outputTypeName) {

    StreamString inputs[] = {inputTypeName};
    StreamString outputs[] = {outputTypeName};
    AddExpectedFunction(1, inputs, 1, outputs);

}

void RuntimeEvaluatorFunctionTest::AddExpectedFunction2In1Out(StreamString input1TypeName, StreamString input2TypeName, StreamString outputTypeName) {

    StreamString inputs[] = {input1TypeName, input2TypeName};
    StreamString outputs[] = {outputTypeName};
    AddExpectedFunction(2, inputs, 1, outputs);

}

void RuntimeEvaluatorFunctionTest::AddExpectedFunction1In2Out(StreamString inputTypeName, StreamString output1TypeName, StreamString output2TypeName) {

    StreamString inputs[] = {inputTypeName};
    StreamString outputs[] = {output1TypeName, output2TypeName};
    AddExpectedFunction(1, inputs, 2, outputs);

}

void RuntimeEvaluatorFunctionTest::AddExpectedFunctionCombinedInputs(StreamString input1TypeName, StreamString input2TypeName, StreamString outputTypeName) {

    StreamString inputs1[] = {input1TypeName, input2TypeName};
    StreamString inputs2[] = {input2TypeName, input1TypeName};
    StreamString outputs[] = {outputTypeName};
    AddExpectedFunction(2, inputs1, 1, outputs);
    AddExpectedFunction(2, inputs2, 1, outputs);

}

void RuntimeEvaluatorFunctionTest::AddExpectedFunction(uint8 numberOfInputs, StreamString inputs[], uint8 numberOfOutputs,  StreamString outputs[]) {
    uint32 lastExistentFunction = expectedFunctionTypesCdb.GetNumberOfChildren();
    StreamString functionName;

    functionName.Printf("Function%i", lastExistentFunction + 1);
    expectedFunctionTypesCdb.CreateRelative(functionName.Buffer());

    if (numberOfInputs > 0) {
        expectedFunctionTypesCdb.CreateRelative("Inputs");
        for (uint8 i = 0; i < numberOfInputs; ++i) {
            StreamString argName;
            argName.Printf("arg%i", i + 1);
            expectedFunctionTypesCdb.Write(argName.Buffer(), inputs[i]);
        }
        expectedFunctionTypesCdb.MoveToAncestor(1);
    }

    if (numberOfOutputs > 0) {
        expectedFunctionTypesCdb.CreateRelative("Outputs");
        for (uint8 i = 0; i < numberOfOutputs; ++i) {
            StreamString argName;
            argName.Printf("arg%i", i + 1);
            expectedFunctionTypesCdb.Write(argName.Buffer(), outputs[i]);
        }
    }

    expectedFunctionTypesCdb.MoveToRoot();
}

bool RuntimeEvaluatorFunctionTest::TestTryConsume(RuntimeEvaluatorFunction &functionRecordUT, CCString inputName, StaticStack<TypeDescriptor,32> &typeStack, bool matchOutput, bool expectedRet, DataMemoryAddress initialDataStackSize, DataMemoryAddress expectedDataStackSize) {

    bool ok = true;

    DataMemoryAddress dataStackSize = initialDataStackSize;
    TypeDescriptor type, initialType;
    Vector<TypeDescriptor> functionInputTypes = functionRecordUT.GetInputTypes();
    Vector<TypeDescriptor> functionOutputTypes = functionRecordUT.GetOutputTypes();
    StaticStack<TypeDescriptor,32> initialTypeStack;

    for (uint32 i = typeStack.GetSize(); i > 0; --i) {
        typeStack.Peek(i - 1, type);
        initialTypeStack.Push(type);
    }

    ok &= (expectedRet == functionRecordUT.TryConsume(inputName, typeStack, matchOutput, dataStackSize));
    if (expectedRet) {
        ok &= (dataStackSize == expectedDataStackSize);

        if (matchOutput){
            initialTypeStack.Pop(initialType);
        }

        ok &= (typeStack.GetSize() == initialTypeStack.GetSize() - functionInputTypes.GetNumberOfElements() + functionOutputTypes.GetNumberOfElements());


        for (uint32 i = 0; ((ok) && (i < functionOutputTypes.GetNumberOfElements())); ++i){
            typeStack.Pop(type);
            ok &= (type == functionOutputTypes[functionOutputTypes.GetNumberOfElements() - i - 1]);
        }

        for (uint32 i = functionInputTypes.GetNumberOfElements(); ((ok) && (typeStack.GetSize() > 0)); ++i) {

            initialTypeStack.Peek(i, initialType);
            typeStack.Pop(type);
            ok &= ((initialType == type));
        }

    } else {
        ok &= (dataStackSize == expectedDataStackSize);

        ok &= (typeStack.GetSize() == initialTypeStack.GetSize());
        while ((ok) && (initialTypeStack.GetSize() > 0)) {

            initialTypeStack.Pop(initialType);
            typeStack.Pop(type);
            ok &= ((initialType == type));
        }
    }

    return ok;
}

bool RuntimeEvaluatorFunctionTest::TestFunctionExecution(RuntimeEvaluator &context, ErrorManagement::ErrorType expectedReturn) {

    ErrorManagement::ErrorType ret;
    bool ok = true;

    ret = context.Execute(RuntimeEvaluator::fastMode);

    if (expectedReturn == ErrorManagement::NoError) {
        ok &= (ret.ErrorsCleared());
    } else {
        ok &= ret.Contains(expectedReturn);
    }

    return ok;
}

bool RuntimeEvaluatorFunctionTest::PrepareContext(RuntimeEvaluator &context, TypeDescriptor inputType, TypeDescriptor outputType, void *inputExternalLocation, void *outputExternalLocation) {

    ErrorManagement::ErrorType ret;
    VariableInformation *var;

    ret = context.ExtractVariables();

    if ((ret) && (inputType != InvalidType)){
        for (uint32 i = 0; context.BrowseInputVariable(i,var); ++i) {
            var->type = inputType;
            if (inputExternalLocation != NULL) {
                var->externalLocation = inputExternalLocation;
            }
        }
    }

    if ((ret) && (outputType != InvalidType)){
        for (uint32 i = 0; context.BrowseOutputVariable(i,var); ++i) {
            var->type = outputType;
            if (outputExternalLocation != NULL) {
                var->externalLocation = outputExternalLocation;
            }
        }
    }

    if (ret) {
        ret = context.Compile();
    }

    return ret;
}

bool RuntimeEvaluatorFunctionTest::PrepareContext(RuntimeEvaluator &context, TypeDescriptor input1Type, TypeDescriptor input2Type, TypeDescriptor outputType) {

    ErrorManagement::ErrorType ret;
    VariableInformation *var;

    ret = context.ExtractVariables();

    if ((ret) && (input1Type != InvalidType) && (context.BrowseInputVariable(0,var))) {
        var->type = input1Type;
    }

    if ((ret) && (input2Type != InvalidType) && (context.BrowseInputVariable(1,var))) {
        var->type = input2Type;
    }

    if ((ret) && (outputType != InvalidType)){
        for (uint32 i = 0; context.BrowseOutputVariable(i,var); ++i) {
            var->type = outputType;
        }
    }

    if (ret) {
        ret = context.Compile();
    }

    return ret;
}

void MockFunction(RuntimeEvaluator &evaluator) {
    evaluator.runtimeError = false;
}
