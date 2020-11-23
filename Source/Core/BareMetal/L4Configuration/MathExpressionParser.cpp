/**
 * @file MathExpressionParser.cpp
 * @brief Source file for class MathExpressionParser
 * @date 07/04/2020
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

,3u,23u,24u,39u ,7u,24u,45u,1u,2u,26u,25u,46u ,2u,25u,3u ,2u,25u,4u 
,3u,26u,28u,40u ,2u,27u,5u ,2u,27u,6u ,2u,27u,7u ,3u,28u,30u,41u 
,2u,29u,8u ,2u,29u,9u ,2u,29u,10u ,2u,29u,11u ,2u,29u,12u ,2u,29u,13u 
,3u,30u,32u,42u ,2u,31u,14u ,2u,31u,15u ,3u,32u,34u,43u ,2u,33u,16u 
,2u,33u,17u ,2u,34u,36u ,5u,34u,47u,31u,36u,49u ,5u,34u,47u,35u,36u,48u 
,2u,35u,18u ,2u,36u,37u ,7u,36u,19u,50u,1u,20u,37u,51u ,7u,36u,19u,50u,1u,20u,52u,21u 
,3u,36u,53u,21u ,2u,37u,38u ,8u,37u,47u,1u,19u,26u,44u,20u,48u 
,4u,38u,19u,26u,20u ,3u,38u,53u,1u ,3u,39u,24u,39u ,1u,39u ,6u,40u,47u,27u,28u,48u,40u 
,1u,40u ,6u,41u,47u,29u,30u,48u,41u ,1u,41u ,6u,42u,47u,31u,32u,48u,42u 
,1u,42u ,6u,43u,47u,33u,34u,48u,43u ,1u,43u ,4u,44u,4u,26u,44u 
,1u,44u 
,0u};

static uint32 Production_row[] = {0u

,1u,5u,13u,16u,19u,23u,26u,29u,32u,36u,39u,42u,45u,48u,51u,54u
,58u,61u,64u,68u,71u,74u,77u,83u,89u,92u,95u,103u,111u,115u,118u,127u
,132u,136u,140u,142u,149u,151u,158u,160u,167u,169u,176u,178u,183u
,0u};

static uint32 ParseArray[] = {

0u,0u,1u,2u,43u,43u,43u,43u,43u,43u,43u,43u,43u,43u,43u,43u,43u,42u,42u
,3u,4u,43u,41u,41u,41u,41u,41u,41u,41u,41u,41u,41u,41u,40u,40u,6u,7u,8u,5u
,41u,39u,39u,39u,39u,39u,38u,38u,38u,38u,38u,38u,5u,5u,9u,47u,5u,5u,39u,5u
,10u,11u,12u,13u,14u,15u,16u,9u,9u,17u,18u,9u,9u,30u,9u,19u,26u,20u,21u,16u
,16u,44u,33u,16u,16u,22u,16u,25u,19u,19u,34u,0u,19u,19u,46u,19u,29u,45u,23u,23u
,32u,0u,24u,22u,0u,22u,37u,37u,36u,36u,36u,35u,0u,0u,0u,0u,0u,0u,0u,0u
,0u,0u,0u,37u,0u,0u
};

static uint32 Parse_row[] = {0u

,1u,2u,16u,37u,30u,52u,51u,64u,54u,73u,60u,83u,68u,74u,53u,80u
,88u,102u,37u,19u,1u,76u
,0u};

static uint32 Conflict[] = {

0u,0u,48u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,26u,26u,0u,0u
,26u,26u,0u,26u,30u,30u,30u,30u,30u,30u,30u,30u,30u,30u,30u,30u,30u,30u,30u,0u
,31u,30u,26u,26u,26u,26u,26u,26u,26u,26u,26u,26u,26u,26u,26u,0u,26u,49u,27u,0u
,26u,26u,26u,26u,26u,26u,26u,26u,26u,26u,26u,26u,26u,26u,26u,0u,27u,26u,28u
};

static uint32 Conflict_row[] = {0u

,1u,20u,36u,56u
,0u};

static const uint32 Constants[] = { 23u, 22u, 0u, 46u, 50u, 45u, 54u, 4u, 512u };

static const char8 * Terminal_name[] ={"0"

,"STRING"
,"="
,";"
,","
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
    }
    
    queueSize = typecastStack.GetSize();
    for (uint32 i = 0u; i < queueSize; i++) {
        StreamString* toDelete;
        if (typecastStack.Extract(0u, toDelete)) {
            delete toDelete;
        }
    }
}

const char8* MathExpressionParser::OperatorFormatting(char8* const operatorIn) const {

    const char8* ret = operatorIn;
    
    if (StringHelper::Compare(operatorIn, "&&") == 0) {
        ret = "AND";
    }
    else if (StringHelper::Compare(operatorIn, "||") == 0) {
        ret = "OR";
    }
    else if (StringHelper::Compare(operatorIn, "^") == 0) {
        ret = "XOR";
    }
    else if (StringHelper::Compare(operatorIn, "==") == 0) {
        ret = "EQ";
    }
    else if (StringHelper::Compare(operatorIn, "!=") == 0) {
        ret = "NEQ";
    }
    else if (StringHelper::Compare(operatorIn, ">") == 0) {
        ret = "GT";
    }
    else if (StringHelper::Compare(operatorIn, "<") == 0) {
        ret = "LT";
    }
    else if (StringHelper::Compare(operatorIn, ">=") == 0) {
        ret = "GTE";
    }
    else if (StringHelper::Compare(operatorIn, "<=") == 0) {
        ret = "LTE";
    }
    else if (StringHelper::Compare(operatorIn, "+") == 0) {
        ret = "ADD";
    }
    else if (StringHelper::Compare(operatorIn, "-") == 0) {
        ret = "SUB";
    }
    else if (StringHelper::Compare(operatorIn, "*") == 0) {
        ret = "MUL";
    }
    else if (StringHelper::Compare(operatorIn, "/") == 0) {
        ret = "DIV";
    }
    else if (StringHelper::Compare(operatorIn, "!") == 0) {
        ret = "NOT";
    }
    else {
        bool ok = StringHelper::ToUpper(operatorIn);
        if (ok) {
            ret = operatorIn;
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                "OperatorFormatting(): failed to convert operator %s to uppercase.",
                operatorIn
                );
            ret = "ERR";
        }
    }
    
    return ret;
    
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
    StreamString* currentOperator = NULL_PTR(StreamString*);
    
    bool ok = operatorStack.Extract(top, currentOperator);
    
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
            "StaticList<currentOperator *>: Failed Extract() of the operator from the operator stack."
            );
    }
    
    // Write in the stack machine expression
    if (ok) {
        stackMachineExpr += OperatorFormatting(currentOperator->BufferReference());
    }
    else {
        stackMachineExpr += "ERR";
    }
    stackMachineExpr += "\n";
    
    if (currentOperator != NULL) {
        delete currentOperator;
    }
}

void MathExpressionParser::PopOperatorAlternate() {
    
    uint32 top = operatorStack.GetSize() - 1u;
    StreamString* currentOperator = NULL_PTR(StreamString*);
    
    bool ok = operatorStack.Extract(top, currentOperator);
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
            "StaticList<currentOperator *>: Failed Extract() of the operator from the operator stack."
            );
    }
    
    if (ok) {
        if (StringHelper::Compare(currentOperator->Buffer(), "+") == 0) {
            // prefix + operator is implied
        }
        else if (StringHelper::Compare(currentOperator->Buffer(), "-") == 0) {
            stackMachineExpr += "NEG\n";
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                "PopOperatorAlternat(): operator %s has no alternative form.", currentOperator->Buffer()
                );
        }
    }
    else {
        stackMachineExpr += "ERR\n";
    }
    
    if (currentOperator != NULL) {
        delete currentOperator;
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
    StreamString* currentOperator = NULL_PTR(StreamString*);
    
    bool ok = typecastStack.Extract(top, currentOperator);
    
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
            "StaticList<currentOperator *>: Failed Extract() of the typecast type from the typecast stack."
            );
    }
    
    // Write in the stack machine expression
    if (ok) {
        stackMachineExpr += "CAST ";
        stackMachineExpr += currentOperator->Buffer();
    }
    else {
        stackMachineExpr += "ERR";
    }
    stackMachineExpr += "\n";
    
    if (currentOperator != NULL) {
        delete currentOperator;
    }
}

void MathExpressionParser::AddOperand() {
    
    // Write in the stack machine expression
    if (StringHelper::Compare(currentToken->GetDescription(), "STRING") == 0) {
        stackMachineExpr += "READ ";
        stackMachineExpr += currentToken->GetData();
    }
    else if (StringHelper::Compare(currentToken->GetDescription(), "NUMBER") == 0) {
        stackMachineExpr += "CONST float64 ";
        stackMachineExpr += currentToken->GetData();
    }
    else {
        stackMachineExpr += "ERR";
    }
    stackMachineExpr += "\n";
}

void MathExpressionParser::AddOperandTypecast() {
    
    uint32 top = typecastStack.GetSize() - 1u;
    StreamString* currentOperator = NULL_PTR(StreamString*);
    
    bool ok = typecastStack.Extract(top, currentOperator);
    if (!ok) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
            "StaticList<currentOperator *>: Failed Extract() of the typecast type from the typecast stack."
            );
    }
    
    // Write in the stack machine expression
    if (ok) {
        stackMachineExpr += "CONST ";
        stackMachineExpr += currentOperator->Buffer();
        stackMachineExpr += " ";
        stackMachineExpr += currentToken->GetData();
        stackMachineExpr += "\n";
    }
    else {
        stackMachineExpr += "ERR\n";
    }

    if (currentOperator != NULL) {
        delete currentOperator;
    }
}

void MathExpressionParser::StoreAssignment() {
    
    assignmentVarName = currentToken->GetData();
}


void MathExpressionParser::PopAssignment() {
    
    // Write in the stack machine expression
    stackMachineExpr += "WRITE ";
    stackMachineExpr += assignmentVarName.Buffer();
    stackMachineExpr += "\n";
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
