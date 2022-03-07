/**
 * @file RuntimeEvaluatorFunctionClass.h
 * @brief Header file for class RuntimeEvaluatorFunctionClass
 * @date Aug 30, 2020
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef RUNTIMEEVALUATORFUNCTIONCLASS_H_
#define RUNTIMEEVALUATORFUNCTIONCLASS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CCString.h"
#include "ErrorManagement.h"
#include "Stack.h"
#include "AnyType.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * contains a database of function classes
 * a function class is a set of functions with same parameterization and same overall behavior
 * each member of the class has same mnemonic
 * different members of a class have different parameters type and therefore use the stack differently
 * READ, WRITE, CONST and CAST are
 */
namespace RuntimeEvaluatorFunctionClass{

/**
 * the record containing the important information of a function class
 */
class FunctionClassInfo{
private:
    //
    CCString name;
};


/**
 *
 */
ErrorManagement::ErrorType FindFunctionClassInfo(CCString name, FunctionClassInfo &functionClassInfo );

/**
 *
 */
ErrorManagement::ErrorType RegisterFunctionClassInfo(const FunctionClassInfo &functionClassInfo );

/**
 * parses input line
 * identifies the function class
 * sees if there is a custom function class elaboration
 * adds any auxiliary types to the stack
 *
 */
ErrorManagement::ErrorType ParseInput(CCString inputLine,Stack<AnyType> typeStack,bool & matchOutput );


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

FunctionClassInfo

} //RuntimeEvaluatorFunctionClass


} // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_PRIVATE_RUNTIMEEVALUATORFUNCTIONCLASS_H_ */
