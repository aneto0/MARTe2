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
#include "AdvancedErrorManagement.h"
#include "TypeConversion.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static void PrintErrorOnStream(const char8 *const format,
        const uint32 lineNumber,
        BufferedStreamI *const err) {
    if (err != NULL) {
        if (!err->Printf(format, lineNumber)) {
            REPORT_ERROR_STATIC(
                    ErrorManagement::FatalError,
                    "PrintErrorOnStream: Failed Printf() on parseError stream");
        }
    }

    REPORT_ERROR_STATIC(ErrorManagement::FatalError, "at line: %d", lineNumber);
}

static uint32 GetCurrentTokenLineNumber(const Token *const token) {
    return (token != NULL) ? (token->GetLineNumber()) : 0u;
}

} // namespace MARTe

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

ParserI::ParserI(StreamI &stream, BufferedStreamI *const err,
        const GrammarInfo &grammarIn)
    : tokenProducer(stream, &grammarIn.assignment, grammarIn.separators,
            grammarIn.beginOneLineComment,
            grammarIn.beginMultipleLinesComment,
            grammarIn.endMultipleLinesComment, grammarIn.keywords) {

    errorStream = err;
    grammar = grammarIn;
    currentToken = static_cast<Token *>(NULL);
    isError = false;

    uint64 pos = stream.Position();
    bool ok = stream.Seek(0ull);
    if (ok) {
        (void)stream.Seek(pos);
    }
}

ParserI::~ParserI() {
    currentToken = static_cast<Token *>(NULL);
    errorStream = static_cast<BufferedStreamI *>(NULL);
}

uint32 ParserI::GetNextTokenType() {
    uint32 ret = 0u;

    currentToken = tokenProducer.GetToken();

    uint32 endTokendId = GetConstant(
            ParserConstant::START_SYMBOL); // StringHelper::Length(terminals)+2u;
    const char8 *toCompare = static_cast<const char8 *>(NULL);

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

    Token *tok = tokenProducer.PeekToken(position);
    uint32 endTokendId = GetConstant(ParserConstant::START_SYMBOL);
    const char8 *toCompare = static_cast<const char8 *>(NULL);

    if (tok->GetId() == TERMINAL_TOKEN) {
        toCompare = tok->GetData();
    } else {
        toCompare = tok->GetDescription();
    }
    for (uint32 i = 0u; i < endTokendId; i++) {
        if (StringHelper::Compare(toCompare, GetSymbolName(i)) == 0) {
            ret = i;
        }
    }

    return ret;
}

GrammarInfo ParserI::GetGrammarInfo() const { return grammar; }

bool ParserI::Parse() {

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

            } else if (symbol >= GetConstant(ParserConstant::START_SYMBOL)) {
                uint32 level = 0u; // before was 1

                uint32 index = GetParseRow(
                        symbol - (GetConstant(ParserConstant::START_SYMBOL) - 1u));
                index += token;
                uint32 entry = GetParse(index);
                while (entry >= GetConstant(ParserConstant::START_CONFLICT)) {
                    index = GetConflictRow(
                            entry - (GetConstant(ParserConstant::START_CONFLICT) - 1u));
                    index += PeekNextTokenType(level);
                    entry = GetConflict(index);
                    ++level;
                }
                if (entry > 0u) {
                    uint32 *production = &GetProduction(GetProductionRow(entry));
                    uint32 production_length = *production - 1u;
                    production = &production[1];
                    /*lint -e{415} [MISRA C++ Rule 5-0-16]. Justification: Remove the
                     * warning "Likely access of out-of-bounds pointer"*/
                    if (*production == symbol) {
                        /*lint -e{661} [MISRA C++ Rule 5-0-16]. Justification: Remove the
                         * warning "Likely access of out-of-bounds pointer"*/
                        for (; production_length > 0u; production_length--) {
                            /*lint -e{662} [MISRA C++ Rule 5-0-16]. Justification: Remove the
                             * warning "Likely access of out-of-bounds pointer"*/
                            uint32 toPush = production[production_length];
                            StackPush(toPush, stack, top);
                        }
                    } else {
                        (token == 0u) ? (isEOF = true) : (isError = true);
                        if (isError) {
                            PrintErrorOnStream("Syntax error 1. Invalid token on line [%d].",
                                    GetCurrentTokenLineNumber(currentToken),
                                    errorStream);
                        }
                        new_token = GetConstant(ParserConstant::END_OF_SLK_INPUT);
                    }
                } else {
                    (token == 0u) ? (isEOF = true) : (isError = true);
                    if (isError) {
                        PrintErrorOnStream("Syntax error 2. Invalid token on line [%d].",
                                GetCurrentTokenLineNumber(currentToken),
                                errorStream);
                    }
                    new_token = GetConstant(ParserConstant::END_OF_SLK_INPUT);
                }
            } else {
                if (symbol > 0u) {
                    if (symbol == token) {
                        token = GetNextTokenType();
                        new_token = token;
                    } else {
                        isError = true;
                        PrintErrorOnStream(
                                "Syntax error 3. Invalid expression on line [%d].",
                                GetCurrentTokenLineNumber(currentToken), errorStream);
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
            PrintErrorOnStream(
                    "\nEOF found with tokens on internal parser stack! [%d]",
                    GetCurrentTokenLineNumber(currentToken), errorStream);
            isError = true;
        }
    }

    return !isError;
}

} // namespace MARTe
