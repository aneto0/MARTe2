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

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

class FunctionTypes {
public:
    FunctionTypes(const TypeDescriptor &inputType, const TypeDescriptor &outputType);
    FunctionTypes(PseudoCode::FunctionRecord &functionRecord);
    bool operator==(FunctionTypes *functionTypes);

private:
    Vector<TypeDescriptor> inputTypes;
    Vector<TypeDescriptor> outputTypes;

};

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
     * @brief Tests the registed Read functionRecords.
     */
    bool TestFunctionRecordTypes(CCString functionName, StaticList<FunctionTypes*> &expectedTypes);

    /**
     * @brief Checks if types provided in functionRecord are within provided expectedTypes.
     */
    bool RemoveFunctionRecordTypesFromList(PseudoCode::FunctionRecord &functionRecord, StaticList<FunctionTypes*> &expectedTypes);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* PSEUDOCODEFUNCTIONSTEST_H_ */

