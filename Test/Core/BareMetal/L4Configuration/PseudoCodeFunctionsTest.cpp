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

bool PseudoCodeFunctionsTest::TestFunctionRecordTypes(CCString functionName, StaticList<FunctionTypes *> &expectedTypes) {

    bool ok = true;

    for (uint32 i = 0; (i < PseudoCode::availableFunctions) && (ok); ++i) {
        PseudoCode::FunctionRecord functionRecord = PseudoCode::functionRecords[i];

        if (functionRecord.GetName() == functionName.GetList()) {
            ok &= RemoveFunctionRecordTypesFromList(functionRecord, expectedTypes);
        }
    }

    ok &= (expectedTypes.GetSize() == 0);

    return ok;

}

bool PseudoCodeFunctionsTest::RemoveFunctionRecordTypesFromList(PseudoCode::FunctionRecord &functionRecord, StaticList<FunctionTypes*> &expectedTypes) {

    FunctionTypes functionTypes(functionRecord);
    bool removed = false;

    for (uint32 i = 0; (i < expectedTypes.GetSize()) && (!removed); ++i) {
        if (functionTypes == expectedTypes[i]) {
            removed = true;
            expectedTypes.Remove(i);
        }
    }

    return removed;
}

FunctionTypes::FunctionTypes(const TypeDescriptor &inputType, const TypeDescriptor &outputType) :
    inputTypes(1),
    outputTypes(1) {
    inputTypes[0] = inputType;
    outputTypes[0] = outputType;
}

FunctionTypes::FunctionTypes(PseudoCode::FunctionRecord &functionRecord) :
    inputTypes(static_cast<TypeDescriptor*>(functionRecord.GetInputTypes().GetDataPointer()), functionRecord.GetInputTypes().GetNumberOfElements()),
    outputTypes(static_cast<TypeDescriptor*>(functionRecord.GetOutputTypes().GetDataPointer()), functionRecord.GetOutputTypes().GetNumberOfElements()) {
}

bool FunctionTypes::operator==(FunctionTypes *functionTypes) {

    bool equals = true;

    equals &= inputTypes.GetNumberOfElements() == functionTypes->inputTypes.GetNumberOfElements();
    equals &= outputTypes.GetNumberOfElements() == functionTypes->outputTypes.GetNumberOfElements();
    for (uint32 i = 0; (equals) && (i < inputTypes.GetNumberOfElements()); ++i) {
        equals &= (inputTypes[i] == functionTypes->inputTypes[i]);
    }
    for (uint32 i = 0; (equals) && (i < outputTypes.GetNumberOfElements()); ++i) {
        equals &= (outputTypes[i] == functionTypes->outputTypes[i]);
    }

    return equals;
}
