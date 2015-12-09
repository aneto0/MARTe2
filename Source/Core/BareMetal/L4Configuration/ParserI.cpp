/**
 * @file ParserI.cpp
 * @brief Source file for class ParserI
 * @date 09/dic/2015
 * @author pc
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

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ParserI.h"
#include "StaticListHolder.h"
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
            REPORT_ERROR(ErrorManagement::FatalError, "PrintErrorOnStream: Failed Printf() on parseError stream");
        }
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, format, lineNumber);
    }
}

ParserI::ParserI(StreamI &stream,
                 ConfigurationDatabase &databaseIn,
                 BufferedStreamI * const err,
                 ParserGrammar grammarIn) :
        parseError(err),
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
    InitializeTable();
}

ParserI::~ParserI() {

}

void ParserI::InitializeTable() {
    Action[0] = 0;
    Action[1] = &ParserI::End;
    Action[2] = &ParserI::GetNodeName;
    Action[3] = &ParserI::AddLeaf;
    Action[4] = &ParserI::GetTypeCast;
    Action[5] = &ParserI::CreateNode;
    Action[6] = &ParserI::AddScalar;
    Action[7] = &ParserI::EndVector;
    Action[8] = &ParserI::EndMatrix;
    Action[9] = &ParserI::BlockEnd;
}

void ParserI::Execute(int32 number) {
    (this->*Action[number])();
}

ParserGrammar ParserI::GetGrammar() const {
    return grammar;
}

void ParserI::Predict(uint16 entry) {

}

void ParserI::GetTypeCast() {
    printf("\nget type cast %s\n", tokenProducer.GetTokenData());
    typeName = tokenProducer.GetTokenData();
}

void ParserI::BlockEnd() {
    printf("\nEnd the block!\n");
    if (!database->MoveToAncestor(1u)) {
        PrintErrorOnStream("\nFailed ConfigurationDatabase::MoveToAncestor(1)! [%d]", tokenProducer.GetLineNumber(), errorStream);
        parseError.SetError();
    }
}

void ParserI::CreateNode() {
    printf("\nCreating node... %s\n", tokenProducer.GetTokenData());
    database->CreateRelative(tokenProducer.GetTokenData());

}

void ParserI::AddLeaf() {
    printf("\nAdding leaf... %s\n", nodeName.Buffer());
// use numberOfRows and numberOfColumns as dimensions # elements
// a matrix!
// a vector!
    if (numberOfDimensions == 1u) {
        //numberOfColumns = firstNumberOfColumns;
        numberOfRows = 1u;
    }
// a scalar
    if (numberOfDimensions == 0u) {
        numberOfRows = 1u;
        numberOfColumns = firstNumberOfColumns;
    }

    AnyType element;
    printf("\n%d %d %d\n", numberOfColumns, numberOfRows, 1u);
    uint32 dimSizes[3] = { numberOfColumns, numberOfRows, 1u };
    /*lint -e{613} . Justification: if (memory==NULL) ---> (ret==false) */
    bool ret = memory.SetType(element, numberOfDimensions, dimSizes);
    if (ret) {
        ret = database->Write(nodeName.Buffer(), element);
        if (!ret) {
            PrintErrorOnStream("\nFailed adding a leaf to the configuration database! [%d]", tokenProducer.GetLineNumber(), errorStream);
            parseError.SetError();
        }
    }
    else {
        PrintErrorOnStream("\nPossible empty vector or matrix! [%d]", tokenProducer.GetLineNumber(), errorStream);
        parseError.SetError();
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
    printf("\nGetting node name %s\n", tokenProducer.GetTokenData());
    nodeName = tokenProducer.GetTokenData();
}

void ParserI::AddScalar() {
    printf("\nAdding a scalar %s\n", tokenProducer.GetTokenData());
    if (tokenType == 0u) {
        tokenType = tokenProducer.GetTokenId();
    }

    if (tokenType == tokenProducer.GetTokenId()) {
        bool ret = memory.ToType(typeName.Buffer(), tokenProducer.GetTokenData());

        if (ret) {
            firstNumberOfColumns++;
        }
        else {
            PrintErrorOnStream("\nFailed read or conversion! [%d]", tokenProducer.GetLineNumber(), errorStream);
            parseError.SetError();
        }
    }
    else {
        PrintErrorOnStream("\nCannot mix different types in a vector or matrix! [%d]", tokenProducer.GetLineNumber(), errorStream);
        parseError.SetError();

    }
}

/*
 void ParserI::CreateClassLeaf() {
 printf("\nAdd The class name!\n");
 // Aggiungi un elemento alla memoria
 bool ret = memory.ToType(typeName.Buffer(), token.GetTokenData());
 if (ret) {
 AnyType element;
 uint32 dimSizes[3] = { 1u, 1u, 1u };
 // lint -e{613} . Justification: if (memory==NULL) ---> (ret==false)
 ret = memory.SetType(element, 0u, dimSizes);
 if (ret) {
 ret = database->Write("ClassName", element);
 if (!ret) {
 PrintErrorOnStream("\nFailed adding the class name leaf to the configuration database! [%d]", token.GetLineNumber(), errorStream);
 parseError.SetError();
 }
 }
 else {
 PrintErrorOnStream("\nPossible empty vector or matrix! [%d]", token.GetLineNumber(), errorStream);
 parseError.SetError();
 }
 }
 else {
 PrintErrorOnStream("\nFailed read or conversion! [%d]", token.GetLineNumber(), errorStream);
 parseError.SetError();
 }

 memory.CleanUp(1u);
 }
 */

void ParserI::EndVector() {
    printf("\nEnd Row\n");
    if (numberOfColumns == 0u) {
        numberOfColumns = firstNumberOfColumns;
    }
    else {
        if (numberOfColumns != firstNumberOfColumns) {
            PrintErrorOnStream("\nIncorrect matrix format! [%d]", tokenProducer.GetLineNumber(), errorStream);
            parseError.SetError();
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
        PrintErrorOnStream("\nFailed ConfigurationDatabase::MoveToRoot() at the end! [%d]", tokenProducer.GetLineNumber(), errorStream);
        parseError.SetError();
    }
}




bool ParserI::Parse() {
    typeName = defaultTypeName;

    while ((!parseError.IsError()) && (!parseError.IsEOF())) {
        //    SlkParse(*this, token, parseError, 0u);

        uint16 *top, *production;
        uint16 production_number, entry, symbol, token, new_token;
        int32 production_length, index, level;
        uint16 stackArray[ParserConstant::PARSE_STACK_SIZE];
        uint16 *stack = &stackArray[0];

        uint16 start_symbol = 0u;

        top = &stackArray[ParserConstant::PARSE_STACK_SIZE - 1u];
        *top = 0;
        if (!start_symbol) {
            start_symbol = GetConstant(ParserConstant::START_SYMBOL);
        }
        StackPush(start_symbol, stack, top);
        token = tokenProducer.get();
        new_token = token;

        for (symbol = StackPop(top); symbol;) {

            if (symbol >= GetConstant(ParserConstant::START_ACTION)) {
                Execute(symbol - (GetConstant(ParserConstant::START_ACTION) - 1));

            }
            else if (symbol >= GetConstant(ParserConstant::START_SYMBOL)) {
                entry = 0;
                level = 1;
                production_number = GetConditionalProduction(symbol);
                if (production_number) {
                    entry = GetPredictedEntry(tokenProducer, production_number, token, level, 1);
                }
                if (!entry) {
                    index = GetParseRow(symbol - (GetConstant(ParserConstant::START_SYMBOL) - 1));
                    index += token;
                    entry = GetParse(index);
                }
                while (entry >= GetConstant(ParserConstant::START_CONFLICT)) {
                    index = GetConflictRow(entry - (GetConstant(ParserConstant::START_CONFLICT) - 1));
                    index += tokenProducer.peek(level);
                    entry = GetConflict(index);
                    ++level;
                }
                if (entry) {
                    production = &GetProduction(GetProductionRow(entry));
                    production_length = *production - 1;
                    if (*++production == symbol) {
                        Predict(entry);
                        production += production_length;
                        for (; production_length-- > 0; --production) {
                            StackPush(*production, stack, top);
                        }
                    }
                    else {
                        new_token = parseError.no_entry(symbol, token, level - 1);
                    }
                }
                else {
                    new_token = parseError.no_entry(symbol, token, level - 1);
                }
            }
            else if (symbol > 0) {
                if (symbol == token) {
                    token = tokenProducer.get();
                    new_token = token;
                }
                else {
                    new_token = parseError.mismatch(symbol, token);
                }
            }
            if (token != new_token) {
                if (new_token) {
                    token = new_token;
                }
                if (token != GetConstant(ParserConstant::END_OF_SLK_INPUT)) {
                    continue;
                }
            }
            symbol = StackPop(top);
        }
        if (token != GetConstant(ParserConstant::END_OF_SLK_INPUT)) {
            parseError.input_left();
        }

    }

    return !parseError.IsError();
}

}
