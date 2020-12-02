/**
 * @file RuntimeEvaluatorExample1.cpp
 * @brief Source file for class RuntimeEvaluatorExample1
 * @date 01/07/2020
 * @author Nicolo Ferron
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
 * the class LoggerServiceExample1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ErrorLoggerExample.h"
#include "MathExpressionParser.h"
#include "RuntimeEvaluator.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(int argc, char **argv) {
    using namespace MARTe;
    // Enables the REPORT_ERROR macro
    SetErrorProcessFunction(&ErrorProcessExampleFunction);
    
    bool ok;
    StreamString expression, stackMachineExpr;
    float64 F, m1, m2, G;
    float64 sunMass   = 1.988e+30;
    float64 earthMass = 5.972e+24;
    float64 moonMass  = 7.346e+22;
    RuntimeEvaluator *evaluator = NULL_PTR(RuntimeEvaluator *);

    expression = "F = G*((m1*m2)/pow(d, 2));";
    
    REPORT_ERROR_STATIC(ErrorManagement::Information,
        "Expression:\n%s", expression.Buffer());
    
    // RuntimeEvaluator accepts expressions in stack machine instructions
    expression.Seek(0u);
    MathExpressionParser mathParser(expression);
    ok = mathParser.Parse();
    if (ok) {
        stackMachineExpr = mathParser.GetStackMachineExpression();
        
        REPORT_ERROR_STATIC(ErrorManagement::Information,
            "Expression in stack machine instruction form:\n%s", stackMachineExpr.Buffer());
    }
    
    // Instantiate the evaluator object
    if (ok) {
        evaluator = new RuntimeEvaluator(stackMachineExpr);
        ok = evaluator->ExtractVariables();
    }
    
    if (ok) {
        // Types of all variables must be specified
        ok &= evaluator->SetOutputVariableType("F", Float64Bit);
        
        ok &= evaluator->SetInputVariableType("G",  Float64Bit);
        ok &= evaluator->SetInputVariableType("m1", Float64Bit);
        ok &= evaluator->SetInputVariableType("m2", Float64Bit);
        ok &= evaluator->SetInputVariableType("d",  Float64Bit);
        
        // A variable can be set to an external location
        ok &= evaluator->SetOutputVariableMemory("F", &F);
        
        ok &= evaluator->SetInputVariableMemory("G",  &G);
        ok &= evaluator->SetInputVariableMemory("m1", &m1);
        ok &= evaluator->SetInputVariableMemory("m2", &m2);
        // Notice that address of variable d is not set and will have to be retrieved
    }
    
    // Now the expression can be compiled
    if (ok) {
        ok = evaluator->Compile();
    }
    
    // From now on, values can be updated at any time
    if (ok) {
        // External variables (G, m1, m2)
        G  = 6.674e-11;
        m1 = earthMass;
        m2 = sunMass;
        
        // Internal variable (d)
        *((float64*)evaluator->GetInputVariableMemory("d")) = 1.496e+11;
    }
    
    // The expession is executed and the value of F becomes available
    if (ok) {
        ok = evaluator->Execute();
    }
    
    if (ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Information,
            "Avg. gravitational force between Sun and Earth is: %.4e N.", F);
    }
    
    // Values can be changed and the expression evaluated again
    if (ok) {
        m2 = moonMass;
        *((float64*)evaluator->GetInputVariableMemory("d")) = 3.85e+8;
    }
    
    if (ok) {
        ok = evaluator->Execute();
    }
    
    if (ok) {
        REPORT_ERROR_STATIC(ErrorManagement::Information,
            "Avg. gravitational force between Moon and Earth is: %.4e N.", F);
    }
    
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError, "Evaluation failed.");
    }
    
    return 0;
}

