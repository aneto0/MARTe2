/**
 * @file MathExpressionParser.cpp
 * @brief Source file for class MathExpressionParser
 * @date Apr 7, 2020
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
 * the class MathExpressionParser (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "AdvancedErrorManagement.h"
#include "MathExpressionParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static uint32 Production[] = {0u

,3u,25u,26u,41u ,7u,26u,47u,1u,2u,28u,27u,48u ,2u,27u,3u ,2u,27u,4u 
,3u,28u,30u,42u ,2u,29u,5u ,2u,29u,6u ,2u,29u,7u ,2u,29u,8u 
,2u,29u,9u ,3u,30u,32u,43u ,2u,31u,10u ,2u,31u,11u ,2u,31u,12u 
,2u,31u,13u ,2u,31u,14u ,2u,31u,15u ,3u,32u,34u,44u ,2u,33u,16u 
,2u,33u,17u ,3u,34u,36u,45u ,2u,35u,18u ,2u,35u,19u ,2u,36u,38u 
,5u,36u,49u,33u,38u,51u ,5u,36u,49u,37u,38u,50u ,2u,37u,20u ,2u,38u,39u 
,7u,38u,21u,52u,1u,22u,39u,53u ,7u,38u,21u,52u,1u,22u,54u,23u 
,3u,38u,55u,23u ,2u,39u,40u ,8u,39u,49u,1u,21u,28u,46u,22u,50u 
,4u,40u,21u,28u,22u ,3u,40u,55u,1u ,3u,41u,26u,41u ,1u,41u ,6u,42u,49u,29u,30u,50u,42u 
,1u,42u ,6u,43u,49u,31u,32u,50u,43u ,1u,43u ,6u,44u,49u,33u,34u,50u,44u 
,1u,44u ,6u,45u,49u,35u,36u,50u,45u ,1u,45u ,4u,46u,4u,28u,46u 
,1u,46u 
,0u};

static uint32 Production_row[] = {0u

,1u,5u,13u,16u,19u,23u,26u,29u,32u,35u,38u,42u,45u,48u,51u,54u
,57u,60u,64u,67u,70u,74u,77u,80u,83u,89u,95u,98u,101u,109u,117u,121u
,124u,133u,138u,142u,146u,148u,155u,157u,164u,166u,173u,175u,182u,184u,189u
,0u};

static uint32 ParseArray[] = {

0u,0u,1u,2u,45u,45u,45u,45u,45u,45u,45u,45u,45u,45u,45u,45u,45u,45u,45u
,44u,44u,3u,4u,45u,43u,43u,43u,43u,43u,43u,43u,43u,43u,43u,43u,43u,43u,42u,42u
,19u,20u,22u,23u,43u,41u,41u,41u,41u,41u,41u,41u,40u,40u,40u,40u,40u,40u,5u,6u
,7u,8u,9u,10u,41u,39u,39u,38u,38u,38u,38u,38u,11u,5u,5u,27u,0u,5u,5u,0u
,5u,36u,0u,0u,39u,0u,18u,11u,11u,49u,28u,11u,11u,35u,11u,12u,13u,14u,15u,16u
,17u,18u,18u,21u,37u,18u,18u,0u,18u,32u,48u,24u,31u,34u,46u,0u,0u,0u,21u,21u
,0u,0u,21u,21u,0u,21u,25u,25u,0u,0u,26u,24u,47u,24u,0u
};

static uint32 Parse_row[] = {0u

,1u,2u,18u,56u,53u,70u,84u,84u,23u,101u,23u,109u,54u,88u,87u,91u
,79u,61u,41u,21u,1u,109u
,0u};

static uint32 Conflict[] = {

0u,0u,50u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,28u,28u
,0u,0u,28u,28u,0u,28u,32u,32u,32u,32u,32u,32u,32u,32u,32u,32u,32u,32u,32u,32u
,32u,32u,32u,0u,33u,32u,28u,28u,28u,28u,28u,28u,28u,28u,28u,28u,28u,28u,28u,28u
,28u,0u,28u,51u,29u,0u,28u,28u,28u,28u,28u,28u,28u,28u,28u,28u,28u,28u,28u,28u
,28u,28u,28u,0u,29u,28u,30u
};

static uint32 Conflict_row[] = {0u

,1u,22u,40u,62u
,0u};

static const uint32 Constants[] = { 25u, 24u, 0u, 48u, 52u, 47u, 56u, 4u, 512u };

static const char8 * Terminal_name[] ={"0"

,"STRING"
,"="
,";"
,","
,"&"
,"|"
,"||"
,"&&"
,"^"
,">"
,"<"
,">="
,"<="
,"=="
,"!="
,"+"
,"-"
,"*"
,"/"
,"!"
,"("
,")"
,"NUMBER"
,"END_OF_SLK_INPUT"
};

static const char8 *GetTerminalName(const uint32 symbol) {
    return Terminal_name[symbol];
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

MathExpressionParser::MathExpressionParser(StreamI &stream,
                                           BufferedStreamI * const err) :
    ParserI(stream, err, MathGrammar) {
        
    Action[0] = static_cast<void (MathExpressionParser::*)(void)>(NULL);
    Action [ 1 ] = &MathExpressionParser::StoreAssignment;
    Action [ 2 ] = &MathExpressionParser::PopAssignment;
    Action [ 3 ] = &MathExpressionParser::PushOperator;
    Action [ 4 ] = &MathExpressionParser::PopOperator;
    Action [ 5 ] = &MathExpressionParser::PopOperatorAlternate;
    Action [ 6 ] = &MathExpressionParser::PushTypecast;
    Action [ 7 ] = &MathExpressionParser::PopTypecast;
    Action [ 8 ] = &MathExpressionParser::AddOperandTypecast;
    Action [ 9 ] = &MathExpressionParser::AddOperand;
}

/*lint -e{1551} Justification: Memory has to be freed in the destructor.
 * No exceptions should be thrown given that the memory is managed exclusively by this class. */
MathExpressionParser::~MathExpressionParser() {
    
    uint32 queueSize = operatorStack.GetSize();
    for (uint32 i = 0u; i < queueSize; i++) {
        StreamString* toDelete;
        if (operatorStack.Extract(0u, toDelete)) {
            delete toDelete;
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                "StaticList<currentOperator *>: Failed Extract() during garbage collection."
                );
        }
    }
    
    queueSize = typecastStack.GetSize();
    for (uint32 i = 0u; i < queueSize; i++) {
        StreamString* toDelete;
        if (typecastStack.Extract(0u, toDelete)) {
            delete toDelete;
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                "StaticList<currentOperator *>: Failed Extract() during garbage collection."
                );
        }
    }
}

const char8* MathExpressionParser::OperatorFormatting(const char8* const operatorIn) const {

    const char8* ret = operatorIn;
    
    char8* uppercaseOp = StringHelper::StringDup(operatorIn);
    bool ok = StringHelper::ToUpper(uppercaseOp);
    if (ok) {
        ret = uppercaseOp;
    }
    else {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
            "OperatorFormatting(): failed to convert operator %s to uppercase.",
            operatorIn
            );
        ret = "ERR";
    }
    
    return ret;
    
}

void MathExpressionParser::PopAssignment() {
    
    // Write in the stack machine expression
    stackMachineExpr += "WRITE ";
    stackMachineExpr += assignmentVarName.Buffer();
    stackMachineExpr += "\n";
}

/*lint -e{429} . Justification: the allocated memory is freed by the class destructor. */
void MathExpressionParser::PushOperator() {
    
    StreamString* currentOperator = new StreamString(currentToken->GetData());
    
    if (!operatorStack.Add(currentOperator)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
            "StaticList<currentOperator *>: Failed Add() of the operator to the operator stack."
            );
    }
}

void MathExpressionParser::PopOperator() {
    
    uint32 top = operatorStack.GetSize() - 1u;
    StreamString* currentOperator;
    
    if (!operatorStack.Extract(top, currentOperator)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
            "StaticList<currentOperator *>: Failed Extract() of the operator from the operator stack."
            );
    }
    
    // Write in the stack machine expression
    stackMachineExpr += OperatorFormatting(currentOperator->Buffer());
    stackMachineExpr += "\n";
}

void MathExpressionParser::PopOperatorAlternate() {
    
    uint32 top = operatorStack.GetSize() - 1u;
    StreamString* currentOperator;
    
    if (!operatorStack.Extract(top, currentOperator)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
            "StaticList<currentOperator *>: Failed Extract() of the operator from the operator stack."
            );
    }
    
    if (StringHelper::Compare(currentOperator->Buffer(), "+") == 0) {
        // nothing
    }
    else if (StringHelper::Compare(currentOperator->Buffer(), "-") == 0) {
        stackMachineExpr += "NEG\n";
    }
    else {
        stackMachineExpr += "ERR\n";
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
            "PopOperatorAlternat(): operator %s has no alternative form.", currentOperator->Buffer()
            );
    }
    
}

/*lint -e{429} . Justification: the allocated memory is freed by the class destructor. */
void MathExpressionParser::PushTypecast() {
    
    StreamString* currentOperator = new StreamString(currentToken->GetData());
    
    if (!typecastStack.Add(currentOperator)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
            "StaticList<currentOperator *>: Failed Add() of the typecast type to the typecast stack."
            );
    }
}

void MathExpressionParser::PopTypecast() {
    
    uint32 top = typecastStack.GetSize() - 1u;
    StreamString* currentOperator;
    
    if (!typecastStack.Extract(top, currentOperator)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
            "StaticList<currentOperator *>: Failed Extract() of the typecast type from the typecast stack."
            );
    }
    
    // Write in the stack machine expression
    stackMachineExpr += "CAST ";
    stackMachineExpr += currentOperator->Buffer();
    stackMachineExpr += "\n";
}

void MathExpressionParser::AddOperand() {
    
    // Write in the stack machine expression
    if (StringHelper::Compare(currentToken->GetDescription(), "STRING") == 0) {
        stackMachineExpr += "READ ";
    }
    else if (StringHelper::Compare(currentToken->GetDescription(), "NUMBER") == 0) {
        stackMachineExpr += "CONST ";
    }
    else {
        stackMachineExpr += "ERR ";
    }
    stackMachineExpr += currentToken->GetData();
    stackMachineExpr += "\n";
}

void MathExpressionParser::AddOperandTypecast() {
    
    uint32 top = typecastStack.GetSize() - 1u;
    StreamString* currentOperator;
    
    if (!typecastStack.Extract(top, currentOperator)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
            "StaticList<currentOperator *>: Failed Extract() of the typecast type from the typecast stack."
            );
    }
    
    // Write in the stack machine expression
    stackMachineExpr += "CONST ";
    stackMachineExpr += currentOperator->Buffer();
    stackMachineExpr += " ";
    stackMachineExpr += currentToken->GetData();
    stackMachineExpr += "\n";
}

void MathExpressionParser::StoreAssignment() {
    
    assignmentVarName = currentToken->GetData();
}

void MathExpressionParser::Execute(const uint32 number) {
    (this->*Action[number])();
}

const char8 *MathExpressionParser::GetSymbolName(const uint32 symbol) const {
    const char8 *symbolName = static_cast<const char8 *>(NULL);

    if((symbol > 0u) && (symbol < Constants[ParserConstant::START_SYMBOL])) {
        symbolName=GetTerminalName(symbol);
    }
    else {
        symbolName="not a symbol";
    }
    return symbolName;
}

uint32 &MathExpressionParser::GetProduction(const uint32 index) const {
    return Production[index];
}

uint32 MathExpressionParser::GetProductionRow(const uint32 index) const {
    return Production_row[index];
}

uint32 MathExpressionParser::GetParse(const uint32 index) const {
    return ParseArray[index];
}

uint32 MathExpressionParser::GetParseRow(const uint32 index) const {
    return Parse_row[index];
}

uint32 MathExpressionParser::GetConflict(const uint32 index) const {
    return Conflict[index];
}

uint32 MathExpressionParser::GetConflictRow(const uint32 index) const {
    return Conflict_row[index];
}

uint32 MathExpressionParser::GetConstant(const uint32 index) const {
    return Constants[index];
}

StreamString MathExpressionParser::GetStackMachineExpression() const {

    return stackMachineExpr;
}

}
