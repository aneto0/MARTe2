/**
 * @file RuntimeEvaluator.h
 * @brief Header file for class PseudoCodeTest
 * @date 04/05/2020
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

 * @details This header file contains the declaration of the class PseudoCodeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef RUNTIMEEVALUATORTEST_H_
#define RUNTIMEEVALUATORTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RuntimeEvaluator.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Tests all the PseudoCode functions
 */
class RuntimeEvaluatorTest {
public:

    /**
     * @brief Tests the constructor of Context class.
     */
    bool TestContextConstructor();

    /**
     * @brief TODO
     */
    bool TestIntegration();
    
    /**
     * @brief TODO
     */
    bool TestError2();
    
    /**
     * @brief     Tests RuntimeEvaluator in case of errors.
     * @details   Checks that RuntimeEvaluator fails in case it is fed
     *            with wrong inputs and that it fails at the right stage
     *            in the evaluation sequence.
     * @param[in] rpnCode       the input RPN code.
     * @param[in] expectedError the expected error, that is:
     *            - `SyntaxError` if RuntimeEvaluator is expected
     *              to fail at RuntimeEvaluator::ExtractVariables(),
     *            - `InitializationError` if RuntimeEvaluator is expected
     *              to fail at RuntimeEvaluator::Compile(),
     *            - `FatalError` if RuntimeEvaluator is expected
     *              to fail at RuntimeEvaluator::Execute().
     * @returns   `true` if RuntimeEvaluator fails at the correct stage.
     */
    bool TestError(CCString rpnCode,
                                         ErrorManagement::ErrorType expectedError
                                        );
    
    /**
     * @brief     Tests RuntimeEvaluator initialization.
     * @details   Before compiling RuntimeEvaluator requires the
     *            RuntimeEvaluator::ExtractVariables() method to be
     *            called. This test checks if RuntimeEvaluator::ExtractVariables()
     *            can process the input RPN code.
     * @param[in] rpnCode the input RPN code.
     * @returns   `true` if RuntimeEvaluator::ExtractVariables() succeed.
     */
    bool TestExtractVariables(CCString rpnCode);
    
    /**
     * @brief TODO
     */
    bool TestExpression(CCString rpnCode, float64 valueArray[]);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* RUNTIMEEVALUATORTEST_H_ */
