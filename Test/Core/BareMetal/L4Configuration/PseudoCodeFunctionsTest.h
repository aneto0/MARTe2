/**
 * @file PseudoCodeFunctionsTest.h
 * @brief Header file for class PseudoCodeFunctionsTest
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

 * @details This header file contains the declaration of the class
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef PSEUDOCODEFUNCTIONSTEST_H_
#define PSEUDOCODEFUNCTIONSTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "PseudoCodeFunctions.h"
#include "PseudoCode.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the PseudoCodeFunctions functions.
 */
class PseudoCodeFunctionsTest {

public:

    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests types of registered functionRecords.
     */
    template<uint8 numberOfFunctions, uint8 numberOfInputs, uint8 numberOfOutputs>
    bool TestFunctionRecordTypes(CCString functionName, TypeDescriptor expectedInputTypes[][numberOfInputs], TypeDescriptor expectedOutputTypes[][numberOfOutputs]);

    /**
     * @brief Tests TryConsume.
     */
    bool TestTryConsume(PseudoCode::FunctionRecord functionRecordUT, CCString inputName, StaticStack<TypeDescriptor,32> &inputTypeStack, bool expectedRet, PseudoCode::DataMemoryAddress expectedDataStackSize);

private:
    /**
     * @brief Checks if types provided are within types lists.
     */
    template<uint8 numberOfInputs, uint8 numberOfOutputs>
    bool FindTypesInLists(uint8 &foundIndex, Vector<TypeDescriptor> inputTypes, Vector<TypeDescriptor> outputTypes, StaticList<TypeDescriptor*> &inputTypesList, StaticList<TypeDescriptor*> &outputTypesList);
};


void MockFunction(PseudoCode::Context &context);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<uint8 numberOfFunctions, uint8 numberOfInputs, uint8 numberOfOutputs>
bool PseudoCodeFunctionsTest::TestFunctionRecordTypes(CCString functionName, TypeDescriptor expectedInputTypes[][numberOfInputs], TypeDescriptor expectedOutputTypes[][numberOfOutputs]) {

    StaticList<TypeDescriptor*> expectedInputTypesList;
    StaticList<TypeDescriptor*> expectedOutputTypesList;
    bool ok = true;

    for (uint8 i=0; i < numberOfFunctions; ++i) {
        expectedInputTypesList.Add(expectedInputTypes[i]);
        expectedOutputTypesList.Add(expectedOutputTypes[i]);
    }

    for (uint32 i = 0; (i < PseudoCode::availableFunctions) && (ok); ++i) {
        PseudoCode::FunctionRecord functionRecord = PseudoCode::functionRecords[i];
        Vector<TypeDescriptor> inputTypes = functionRecord.GetInputTypes();
        Vector<TypeDescriptor> outputTypes = functionRecord.GetOutputTypes();

        if ((functionRecord.GetName() == functionName.GetList()) &&
            (inputTypes.GetNumberOfElements() == numberOfInputs) &&
            (outputTypes.GetNumberOfElements() == numberOfOutputs)) {
            uint8 foundIndex;

            ok &= FindTypesInLists<numberOfInputs, numberOfOutputs>(foundIndex, inputTypes, outputTypes, expectedInputTypesList, expectedOutputTypesList);
            if (ok) {
                expectedInputTypesList.Remove(foundIndex);
                expectedOutputTypesList.Remove(foundIndex);
            }
        }
    }

    ok &= (expectedInputTypesList.GetSize() == 0);
    ok &= (expectedOutputTypesList.GetSize() == 0);

    return ok;

}

template<uint8 numberOfInputs, uint8 numberOfOutputs>
bool PseudoCodeFunctionsTest::FindTypesInLists(uint8 &foundIndex, Vector<TypeDescriptor> inputTypes, Vector<TypeDescriptor> outputTypes, StaticList<TypeDescriptor*> &inputTypesList, StaticList<TypeDescriptor*> &outputTypesList) {

    bool found = false;

    for (foundIndex = 0; (foundIndex < inputTypesList.GetSize()) && (!found); ++foundIndex) {
        bool equals = true;

        for (uint32 i = 0; (equals) && (i < inputTypes.GetNumberOfElements()); ++i) {
            equals &= (inputTypes[i] == inputTypesList[foundIndex][i]);
        }
        for (uint32 i = 0; (equals) && (i < outputTypes.GetNumberOfElements()); ++i) {
            equals &= (outputTypes[i] == outputTypesList[foundIndex][i]);
        }

        found = equals;
    }

    --foundIndex;
    return found;
}

#endif /* PSEUDOCODEFUNCTIONSTEST_H_ */

