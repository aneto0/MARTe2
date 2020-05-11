/**
 * @file PseudoCodeFunctionsTest.cpp
 * @brief Source file for class PseudoCodeFunctionsTest
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
 * the class PseudoCodeFunctionsTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "PseudoCodeFunctionsTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool PseudoCodeFunctionsTest::TestDefaultConstructor() {

    PseudoCode::FunctionRecord functionRecordUT;
    bool ok;

    ok = (functionRecordUT.GetName() == "");
    ok &= (functionRecordUT.GetInputTypes().GetNumberOfElements() == 0);
    ok &= (functionRecordUT.GetOutputTypes().GetNumberOfElements() == 0);

    return ok;
}

bool PseudoCodeFunctionsTest::TestTryConsume(PseudoCode::FunctionRecord &functionRecordUT, CCString inputName, StaticStack<TypeDescriptor,32> &typeStack, bool matchOutput, bool expectedRet, PseudoCode::DataMemoryAddress initialDataStackSize, PseudoCode::DataMemoryAddress expectedDataStackSize) {

    bool ok = true;

    PseudoCode::DataMemoryAddress dataStackSize = initialDataStackSize;
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

void MockFunction(PseudoCode::Context &context) {
    ((void)0);
}
