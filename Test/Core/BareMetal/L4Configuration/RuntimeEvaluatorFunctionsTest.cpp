/**
 * @file RuntimeEvaluatorFunctionsTest.cpp
 * @brief Source file for class RuntimeEvaluatorFunctionsTest
 * @date 06/05/2020
 * @author Didac Magrinya
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
 * the class RuntimeEvaluatorFunctionsTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "RuntimeEvaluatorFunctionsTest.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool RuntimeEvaluatorFunctionsTest::TestDefaultConstructor() {

    RuntimeEvaluatorFunctions functionUT;
    bool ok;

    ok = (functionUT.GetName() == "");
    ok &= (functionUT.GetInputTypes().GetNumberOfElements() == 0);
    ok &= (functionUT.GetOutputTypes().GetNumberOfElements() == 0);

    return ok;
}

bool RuntimeEvaluatorFunctionsTest::ParseFunctions(StreamString &expectedFunctionTypes) {
    bool ok = true;

    expectedFunctionTypes.Seek(0LLU);
    StandardParser parser(expectedFunctionTypes, expectedFunctionTypesCdb, NULL);
    ok = parser.Parse();

    return ok;

}

bool RuntimeEvaluatorFunctionsTest::TestFunctionTypes(CCString functionName, uint8 numberOfInputs, uint8 numberOfOutputs) {
    bool ok = true;

    for (uint32 i = 0; (i < availableFunctions) && (ok); ++i) {
        RuntimeEvaluatorFunctions functionRecord = functionRecords[i];
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

bool RuntimeEvaluatorFunctionsTest::FindTypesInCdb(CCString &foundName, Vector<TypeDescriptor> &inputTypes, Vector<TypeDescriptor> &outputTypes, ConfigurationDatabase &typesCdb) {

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

void RuntimeEvaluatorFunctionsTest::AddFunctionsWithOnlyInputs(uint8 numberOfFunctions, StreamString typeName) {
    uint32 lastExistentFunction = expectedFunctionTypesCdb.GetNumberOfChildren();

    for (uint32 i = lastExistentFunction + 1; i <= lastExistentFunction + numberOfFunctions; ++i) {
        StreamString functionName;

        functionName.Printf("Function%i.Inputs", i);
        expectedFunctionTypesCdb.CreateRelative(functionName.Buffer());

        expectedFunctionTypesCdb.Write("arg1", typeName);
        expectedFunctionTypesCdb.MoveToRoot();
    }


}

void RuntimeEvaluatorFunctionsTest::AddFunctionsWithInputCombination(StreamString input1TypeName, StreamString input2TypeName, StreamString outputTypeName) {

    StreamString inputs1[] = {input1TypeName, input2TypeName};
    StreamString inputs2[] = {input2TypeName, input1TypeName};
    StreamString outputs[] = {outputTypeName};
    AddFunction(2, inputs1, 1, outputs);
    AddFunction(2, inputs2, 1, outputs);

}

void RuntimeEvaluatorFunctionsTest::AddFunction(uint8 numberOfInputs, StreamString inputs[], uint8 numberOfOutputs,  StreamString outputs[]) {
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

bool RuntimeEvaluatorFunctionsTest::TestTryConsume(RuntimeEvaluatorFunctions &functionRecordUT, CCString inputName, StaticStack<TypeDescriptor,32> &typeStack, bool matchOutput, bool expectedRet, DataMemoryAddress initialDataStackSize, DataMemoryAddress expectedDataStackSize) {

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

void MockFunction(RuntimeEvaluator &evaluator) {
    ((void)0);
}