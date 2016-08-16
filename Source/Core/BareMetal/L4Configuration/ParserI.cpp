/**
 * @file ParserI.cpp
 * @brief Source file for class ParserI
 * @date 09/12/2015
 * @author Giuseppe Ferrò
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
 * the class ParserI (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ParserI.h"
#include "TypeConversion.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static void PrintErrorOnStream(const char8 * const format,
                               const uint32 lineNumber,
                               BufferedStreamI * const err) {
    if (err != NULL) {
        if (!err->Printf(format, lineNumber)) {
            REPORT_ERROR(ErrorManagement::fatalError, "PrintErrorOnStream: Failed Printf() on parseError stream");
        }
        REPORT_ERROR_PARAMETERS(ErrorManagement::fatalError, format, lineNumber)
    }
}

static const char8* GetCurrentTokenData(Token * const token) {

    return (token != NULL)?(token->GetData()):(static_cast<const char8*>(NULL));
}

static uint32 GetCurrentTokenId(const Token * const token) {
    return (token != NULL)?(token->GetId()):(ERROR_TOKEN);
}

static uint32 GetCurrentTokenLineNumber(const Token * const token) {
    return (token != NULL)?(token->GetLineNumber()):0u;
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ParserI::ParserI(StreamI &stream,
                 StructuredDataI &databaseIn,
                 BufferedStreamI * const err,
                 const GrammarInfo &grammarIn) :
        tokenProducer(stream, &grammarIn.assignment, grammarIn.separators, grammarIn.beginOneLineComment, grammarIn.beginMultipleLinesComment,
                      grammarIn.endMultipleLinesComment),
        memory(1u) {
    numberOfColumns = 0u;
    firstNumberOfColumns = 0u;
    numberOfRows = 0u;
    database = &databaseIn;
    errorStream = err;
    tokenType = 0u;
    numberOfDimensions = 0u;
    grammar = grammarIn;
    currentToken = static_cast<Token*>(NULL);
    isError = false;
}

ParserI::~ParserI() {
    currentToken = static_cast<Token*>(NULL);
    errorStream=static_cast<BufferedStreamI*>(NULL);
    database=static_cast<StructuredDataI*>(NULL);
}

uint32 ParserI::GetNextTokenType() {
    uint32 ret = 0u;

    currentToken = tokenProducer.GetToken();

    uint32 endTokendId = GetConstant(ParserConstant::START_SYMBOL); //StringHelper::Length(terminals)+2u;
    const char8* toCompare = static_cast<const char8 *>(NULL);

    // if it is a terminal use the data
    if (currentToken->GetId() == TERMINAL_TOKEN) {
        toCompare = currentToken->GetData();
    }
    // otherwise use the description
    else {
        toCompare = currentToken->GetDescription();
    }
    // return the slk token number
    for (uint32 i = 0u; i < endTokendId; i++) {
        if (StringHelper::Compare(toCompare, GetSymbolName(i)) == 0) {
            ret = i;
        }
    }
    return ret;
}

uint32 ParserI::PeekNextTokenType(const uint32 position) {
    uint32 ret = 0u;

    Token* tok = tokenProducer.PeekToken(position);
    uint32 endTokendId = GetConstant(ParserConstant::START_SYMBOL);
    const char8* toCompare = static_cast<const char8 *>(NULL);

    if (tok->GetId() == TERMINAL_TOKEN) {
        toCompare = tok->GetData();
    }
    else {
        toCompare = tok->GetDescription();
    }
    for (uint32 i = 0u; i < endTokendId; i++) {
        if (StringHelper::Compare(toCompare, GetSymbolName(i)) == 0) {
            ret = i;
        }
    }

    return ret;
}

GrammarInfo ParserI::GetGrammarInfo() const {
    return grammar;
}

void ParserI::GetTypeCast() {
    typeName = GetCurrentTokenData(currentToken);
}

void ParserI::BlockEnd() {
    if (!database->MoveToAncestor(1u)) {
        PrintErrorOnStream("\nFailed StructuredDataI::MoveToAncestor(1)! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

void ParserI::CreateNode() {
    if (!database->CreateRelative(GetCurrentTokenData(currentToken))) {
        PrintErrorOnStream("\nFailed StructuredDataI::CreateRelative(*)! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

void ParserI::AddLeaf() {
    // use numberOfRows and numberOfColumns as dimensions # elements
    if (numberOfDimensions == 1u) {
        numberOfRows = 1u;
    }
    // a scalar
    if (numberOfDimensions == 0u) {
        numberOfRows = 1u;
        numberOfColumns = firstNumberOfColumns;
    }

    ;
    uint32 dimSizes[3] = { numberOfColumns, numberOfRows, 1u };
    /*lint -e{613} . Justification: if (memory==NULL) ---> (ret==false) */
    AnyType element = memory.Create(numberOfDimensions, &dimSizes[0]);
    bool ret = (element.GetDataPointer() != NULL);
    if (ret) {
        ret = database->Write(nodeName.Buffer(), element);
        if (!ret) {
            PrintErrorOnStream("\nFailed adding a leaf to the configuration database! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
            isError = true;
        }
    }
    else {
        PrintErrorOnStream("\nPossible empty vector or matrix! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
    typeName = defaultTypeName;
    numberOfColumns = 0u;
    firstNumberOfColumns = 0u;
    numberOfRows = 0u;
    tokenType = 0u;
    numberOfDimensions = 0u;
    memory.CleanUp(1u);
}

void ParserI::GetNodeName() {
    nodeName = GetCurrentTokenData(currentToken);
}

void ParserI::AddScalar() {
    if (tokenType == 0u) {
        tokenType = GetCurrentTokenId(currentToken);
    }

    if (tokenType == GetCurrentTokenId(currentToken)) {
        bool ret = memory.Add(typeName.Buffer(), GetCurrentTokenData(currentToken));

        if (ret) {
            firstNumberOfColumns++;
        }
        else {
            PrintErrorOnStream("\nFailed read or conversion! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
            isError = true;
        }
    }
    else {
        PrintErrorOnStream("\nCannot mix different types in a vector or matrix! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

void ParserI::EndVector() {
    if (numberOfColumns == 0u) {
        numberOfColumns = firstNumberOfColumns;
    }
    else {
        if (numberOfColumns != firstNumberOfColumns) {
            PrintErrorOnStream("\nIncorrect matrix format! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
            isError = true;
        }
    }
    firstNumberOfColumns = 0u;
    numberOfRows++;
    if (numberOfDimensions == 0u) {
        numberOfDimensions = 1u;
    }
}

void ParserI::EndMatrix() {
    numberOfDimensions = 2u;
}

void ParserI::End() {
    if (!database->MoveToRoot()) {
        PrintErrorOnStream("\nFailed StructuredDataI::MoveToRoot() at the end! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

bool ParserI::Parse() {
    typeName = defaultTypeName;

    bool isEOF = false;

    while ((!isError) && (!isEOF)) {

        uint32 stackArray[ParserConstant::PARSE_STACK_SIZE];
        uint32 *stack = &stackArray[0];

        uint32 *top = &stackArray[ParserConstant::PARSE_STACK_SIZE - 1u];
        *top = 0u;
        uint32 start_symbol = GetConstant(ParserConstant::START_SYMBOL);

        StackPush(start_symbol, stack, top);
        uint32 token = GetNextTokenType();
        uint32 new_token = token;

        for (uint32 symbol = StackPop(top); (symbol > 0u) && (!isError);) {
            if (symbol >= GetConstant(ParserConstant::START_ACTION)) {
                Execute(symbol - (GetConstant(ParserConstant::START_ACTION) - 1u));

            }
            else if (symbol >= GetConstant(ParserConstant::START_SYMBOL)) {
                uint32 level = 0u; // before was 1

                uint32 index = GetParseRow(symbol - (GetConstant(ParserConstant::START_SYMBOL) - 1u));
                index += token;
                uint32 entry = GetParse(index);
                while (entry >= GetConstant(ParserConstant::START_CONFLICT)) {
                    index = GetConflictRow(entry - (GetConstant(ParserConstant::START_CONFLICT) - 1u));
                    index += PeekNextTokenType(level);
                    entry = GetConflict(index);
                    ++level;
                }
                if (entry > 0u) {
                    uint32 *production = &GetProduction(GetProductionRow(entry));
                    uint32 production_length = *production - 1u;
                    production = &production[1];
                    /*lint -e{415} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                    if (*production == symbol) {
                        /*lint -e{661} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                        for (; production_length > 0u; production_length--) {
                            /*lint -e{662} [MISRA C++ Rule 5-0-16]. Justification: Remove the warning "Likely access of out-of-bounds pointer"*/
                            uint32 toPush = production[production_length];
                            StackPush(toPush, stack, top);
                        }
                    }
                    else {
                        (token == 0u) ? (isEOF = true) : (isError = true);
                        if (isError) {
                            PrintErrorOnStream("\nInvalid Token! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
                        }
                        new_token = GetConstant(ParserConstant::END_OF_SLK_INPUT);
                    }
                }
                else {
                    (token == 0u) ? (isEOF = true) : (isError = true);
                    if (isError) {
                        PrintErrorOnStream("\nInvalid Token! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
                    }
                    new_token = GetConstant(ParserConstant::END_OF_SLK_INPUT);
                }
            }
            else {
                if (symbol > 0u) {
                    if (symbol == token) {
                        token = GetNextTokenType();
                        new_token = token;
                    }
                    else {
                        isError = true;
                        PrintErrorOnStream("\nInvalid Expression! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
                        new_token = GetConstant(ParserConstant::END_OF_SLK_INPUT);
                    }
                }
            }
            if (token != new_token) {
                if (new_token > 0u) {
                    token = new_token;
                }
                if (token != GetConstant(ParserConstant::END_OF_SLK_INPUT)) {
                    continue;
                }
            }
            symbol = StackPop(top);
        }
        if (token != GetConstant(ParserConstant::END_OF_SLK_INPUT)) {
            PrintErrorOnStream("\nEOF found with tokens on internal parser stack! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
            isError = true;
        }
    }

    return !isError;
}

}
