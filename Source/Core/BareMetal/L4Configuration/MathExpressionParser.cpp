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

#include "MathExpressionParser.h"
#include "AdvancedErrorManagement.h" // DEBUG remove this

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static uint32 Production[] = {0u

,3u,24u,25u,40u ,7u,25u,46u,1u,2u,27u,26u,47u ,2u,26u,3u ,2u,26u,4u 
,3u,27u,29u,41u ,2u,28u,5u ,2u,28u,6u ,2u,28u,7u ,2u,28u,8u 
,2u,28u,9u ,3u,29u,31u,42u ,2u,30u,10u ,2u,30u,11u ,2u,30u,12u 
,2u,30u,13u ,2u,30u,14u ,3u,31u,33u,43u ,2u,32u,15u ,2u,32u,16u 
,3u,33u,35u,44u ,2u,34u,17u ,2u,34u,18u ,2u,35u,38u ,5u,35u,48u,37u,38u,50u 
,5u,35u,48u,36u,38u,49u ,2u,36u,19u ,2u,37u,15u ,2u,37u,16u ,2u,38u,39u 
,7u,38u,20u,51u,1u,21u,39u,52u ,3u,38u,53u,22u ,7u,38u,20u,51u,1u,21u,54u,22u 
,4u,39u,20u,27u,21u ,3u,39u,53u,1u ,8u,39u,48u,1u,20u,27u,45u,21u,49u 
,3u,40u,25u,40u ,1u,40u ,6u,41u,48u,28u,29u,49u,41u ,1u,41u ,6u,42u,48u,30u,31u,49u,42u 
,1u,42u ,6u,43u,48u,32u,33u,49u,43u ,1u,43u ,6u,44u,48u,34u,35u,49u,44u 
,1u,44u ,4u,45u,4u,27u,45u ,1u,45u 
,0u};

static uint32 Production_row[] = {0u

,1u,5u,13u,16u,19u,23u,26u,29u,32u,35u,38u,42u,45u,48u,51u,54u
,57u,61u,64u,67u,71u,74u,77u,80u,86u,92u,95u,98u,101u,104u,112u,116u
,124u,129u,133u,142u,146u,148u,155u,157u,164u,166u,173u,175u,182u,184u,189u
,0u};

static uint32 ParseArray[] = {

0u,0u,1u,2u,45u,45u,45u,45u,45u,45u,45u,45u,45u,45u,45u,45u,45u,45u,44u
,44u,3u,4u,45u,43u,43u,43u,43u,43u,43u,43u,43u,43u,43u,43u,43u,42u,42u,18u,19u
,21u,22u,43u,41u,41u,41u,41u,41u,41u,41u,40u,40u,40u,40u,40u,5u,6u,7u,8u,9u
,10u,41u,39u,39u,38u,38u,38u,38u,38u,5u,5u,11u,49u,5u,5u,26u,5u,27u,28u,17u
,39u,0u,36u,0u,0u,11u,11u,20u,0u,11u,11u,33u,11u,17u,17u,23u,0u,17u,17u,0u
,17u,20u,20u,29u,37u,20u,20u,0u,20u,24u,24u,46u,0u,25u,23u,0u,23u,12u,13u,14u
,15u,16u,48u,0u,31u,0u,0u,0u,47u,0u,0u
};

static uint32 Parse_row[] = {0u

,1u,2u,17u,53u,50u,69u,106u,77u,22u,85u,22u,93u,55u,61u,101u,70u
,80u,58u,39u,20u,1u,106u
,0u};

static uint32 Conflict[] = {

0u,0u,50u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,0u,29u,29u,0u
,0u,29u,29u,0u,29u,34u,34u,34u,34u,34u,34u,34u,34u,34u,34u,34u,34u,34u,34u,34u
,34u,0u,35u,34u,29u,29u,29u,29u,29u,29u,29u,29u,29u,29u,29u,29u,29u,29u,0u,29u
,51u,30u,0u,29u,29u,29u,29u,29u,29u,29u,29u,29u,29u,29u,29u,29u,29u,29u,29u,0u
,30u,29u,32u
};

static uint32 Conflict_row[] = {0u

,1u,21u,38u,59u
,0u};

static const uint32 Constants[] = { 24u, 23u, 0u, 48u, 52u, 46u, 55u, 4u, 512u };

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
                                           StructuredDataI &databaseIn,
                                           BufferedStreamI * const err) :
    ParserI(stream, databaseIn, err, MathGrammar) {
        
        //tokenProducer.TokenizeInput();
        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Acquired terminals:: %s", &(MathGrammar.assignment));
        REPORT_ERROR_STATIC(ErrorManagement::Debug, "Acquired string: %s", (dynamic_cast<StreamString&>(stream)).Buffer());
        
        Action[0] = static_cast<void (MathExpressionParser::*)(void)>(NULL);
    Action [ 1 ] = &MathExpressionParser::StoreAssignment;
    Action [ 2 ] = &MathExpressionParser::PopAssignment;
    Action [ 3 ] = &MathExpressionParser::PushOperator;
    Action [ 4 ] = &MathExpressionParser::PopOperator;
    Action [ 5 ] = &MathExpressionParser::PopOperatorAlternate;
    Action [ 6 ] = &MathExpressionParser::PushTypecast;
    Action [ 7 ] = &MathExpressionParser::PopTypecast;
    Action [ 8 ] = &MathExpressionParser::AddOperand;
    Action [ 9 ] = &MathExpressionParser::AddOperandTypecast;
}

/*lint -e{1551} Justification: Memory has to be freed in the destructor.
 * No exceptions should be thrown given that the memory is managed exclusively by this class.". */
MathExpressionParser::~MathExpressionParser() {
    uint32 queueSize = operatorStack.GetSize();
    for (uint32 i = 0u; i < queueSize; i++) {
        StreamString* toDelete;
        if (operatorStack.Extract(0u, toDelete)) {
            delete toDelete;
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "StaticList<operat *>: Failed Extract() during garbage collection.");
        }
    }
    
    queueSize = typecastStack.GetSize();
    for (uint32 i = 0u; i < queueSize; i++) {
        StreamString* toDelete;
        if (typecastStack.Extract(0u, toDelete)) {
            delete toDelete;
        }
        else {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "StaticList<operat *>: Failed Extract() during garbage collection.");
        }
    }
}

const char8* MathExpressionParser::OperatorLookupTable(const char8* const operatorIn) const {

    const char8* ret = operatorIn;
    
    if (StringHelper::Compare(operatorIn, "+") == 0) {
        ret = "SUM";
    }
    else if (StringHelper::Compare(operatorIn, "*") == 0) {
        ret = "MUL";
    }
    else if (StringHelper::Compare(operatorIn, "/") == 0) {
        ret = "DIV";
    }
    else if (StringHelper::Compare(operatorIn, "-") == 0) {
        ret = "MIN";
    }
    else if (StringHelper::Compare(operatorIn, "!") == 0) {
        ret = "FACT";
    }
    else if (StringHelper::Compare(operatorIn, "&") == 0) {
        ret = "BAND";
    }
    else if (StringHelper::Compare(operatorIn, "|") == 0) {
        ret = "BOR";
    }
    else if (StringHelper::Compare(operatorIn, "&&") == 0) {
        ret = "AND";
    }
    else if (StringHelper::Compare(operatorIn, "||") == 0) {
        ret = "OR";
    }
    else if (StringHelper::Compare(operatorIn, "^") == 0) {
        ret = "XOR";
    }
    else if (StringHelper::Compare(operatorIn, "<") == 0) {
        ret = "LT";
    }
    else if (StringHelper::Compare(operatorIn, ">") == 0) {
        ret = "GT"; 
    }
    else if (StringHelper::Compare(operatorIn, "<=") == 0) {
        ret = "LE";
    }
    else if (StringHelper::Compare(operatorIn, ">=") == 0) {
        ret = "GE"; 
    }
    else if (StringHelper::Compare(operatorIn, "==") == 0) {
        ret = "EQ"; 
    }
    else
    {
        char8* uppercaseOp = StringHelper::StringDup(operatorIn);
        bool ok = StringHelper::ToUpper(uppercaseOp);
        if (!ok) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "OperatorLookupTable(): failed to convert operator %s to uppercase.", operatorIn);
            ret = "ERR";
        }
        else {
            ret = uppercaseOp;
        }
    
    }
    
    return ret;
    
} // OperatorLookupTable()

void MathExpressionParser::PopAssignment()
{
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "WRITE %s", assignmentVarName.Buffer());
    REPORT_ERROR_STATIC(ErrorManagement::Information, "END");
    
    // Write in the stack machine expression
    stackMachineExpr += "WRITE ";
    stackMachineExpr += assignmentVarName.Buffer();
    stackMachineExpr += "\n";
}

/*lint -e{429} . Justification: the allocated memory is freed by the class destructor. */
void MathExpressionParser::PushOperator()
{
    StreamString* operat = new StreamString(currentToken->GetData());
    
    if (!operatorStack.Add(operat)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                            "StaticList<operat *>: Failed Add() of the operator to the operator stack."
                            );
    }
}

void MathExpressionParser::PopOperator()
{
    uint32 top = operatorStack.GetSize() - 1u;
    StreamString* operat;
    
    if (!operatorStack.Extract(top, operat)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                            "StaticList<operat *>: Failed Extract() of the operator from the operator stack."
                            );
    }
    
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Add Operator %s", operat->Buffer());
    
    // Write in the stack machine expression
    stackMachineExpr += OperatorLookupTable(operat->Buffer());
    stackMachineExpr += "\n";
}

void MathExpressionParser::PopOperatorAlternate()
{
    uint32 top = operatorStack.GetSize() - 1u;
    StreamString* operat;
    
    if (!operatorStack.Extract(top, operat)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                            "StaticList<operat *>: Failed Extract() of the operator from the operator stack."
                            );
    }

    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Add Operator %s", operat->Buffer());
    
    if (StringHelper::Compare(operat->Buffer(), "+") == 0) {
        // nothing
    }
    else if (StringHelper::Compare(operat->Buffer(), "-") == 0) {
        stackMachineExpr += "NEG\n";
    }
    else {
        stackMachineExpr += "ERR\n";
    }
    
}

/*lint -e{429} . Justification: the allocated memory is freed by the class destructor. */
void MathExpressionParser::PushTypecast()
{
    StreamString* operat = new StreamString(currentToken->GetData());
    
    if (!typecastStack.Add(operat)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                            "StaticList<operat *>: Failed Add() of the typecast type to the typecast stack."
                            );
    }
}

void MathExpressionParser::PopTypecast()
{
    uint32 top = typecastStack.GetSize() - 1u;
    StreamString* operat;
    
    if (!typecastStack.Extract(top, operat)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                            "StaticList<operat *>: Failed Extract() of the typecast type from the typecast stack."
                            );
    }
    
    // Write in the stack machine expression
    stackMachineExpr += "CAST ";
    stackMachineExpr += operat->Buffer();
    stackMachineExpr += "\n";
}

void MathExpressionParser::AddOperand()
{
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "Add Operand  %s", currentToken->GetData());

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

void MathExpressionParser::AddOperandTypecast()
{
    uint32 top = typecastStack.GetSize() - 1u;
    StreamString* operat;
    
    if (!typecastStack.Extract(top, operat)) {
        REPORT_ERROR_STATIC(ErrorManagement::FatalError,
                            "StaticList<operat *>: Failed Extract() of the typecast type from the typecast stack."
                            );
    }
    
    // Write in the stack machine expression
    stackMachineExpr += "CONST ";
    stackMachineExpr += operat->Buffer();
    stackMachineExpr += " ";
    stackMachineExpr += currentToken->GetData();
    stackMachineExpr += "\n";
}

void MathExpressionParser::StoreAssignment()
{
    REPORT_ERROR_STATIC(ErrorManagement::Debug, "StoreAssignment %s", currentToken->GetData());
    
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
