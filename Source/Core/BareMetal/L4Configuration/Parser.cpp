/**
 * @file Parser.cpp
 * @brief Source file for class Parser
 * @date 25/11/2015
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
 * the class Parser (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "Parser.h"
#include "StaticListHolder.h"
#include "TypeConversion.h"
#include "AdvancedErrorManagement.h"
#include "AnyTypeCreator.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static void PrintErrorOnStream(const char8 * const format,
                               const uint32 lineNumber,
                               BufferedStreamI * const err) {
    if (err != NULL) {
        if (!err->Printf(format, lineNumber)) {
            REPORT_ERROR(ErrorManagement::FatalError, "PrintErrorOnStream: Failed Printf() on error stream");
        }
        REPORT_ERROR_PARAMETERS(ErrorManagement::FatalError, format, lineNumber);
    }
}


/**
 * @brief Determines if the next expression is an l-value.
 * @param[in, out] token is a pointer to the token returned by the lexical analyzer.
 * @param[in] lexicalAnalyzer is the lexical analyzer used to read tokens from the stream.
 * @param[in] assignmentTerminator is the assign operator terminal.
 * @param[in] varName is the name of the l-value if found.
 * @return true if the next expression is an l-value, false otherwise.
 */
static bool isLValue(Token* &token,
                     LexicalAnalyzer &lexicalAnalyzer,
                     const char8 assignmentTerminator,
                     StreamString &varName) {

    bool ret = true;
    varName = static_cast<const char8*>(NULL);

    Token* storeToken = token;

    if (token->GetId() != STRING_TOKEN) {
        ret = false;
    }
    else {
        const char8* possibleBlockName = token->GetData();
        token = lexicalAnalyzer.PeekToken(0u);
        if (token->GetId() != TERMINAL_TOKEN) {
            ret = false;
        }
        else {
            if (token->GetData()[0] != assignmentTerminator) {
                ret = false;
            }
            else {
                varName = possibleBlockName;
                // success! Get the next two tokens
                token = lexicalAnalyzer.GetToken();
                token = lexicalAnalyzer.GetToken();
            }
        }
    }
    // in case of failure reset the token
    if (!ret) {
        token = storeToken;
    }

    return ret;
}


/**
 * @brief Checks that the total number of block terminals is not negative after reading a variable.
 * If the block is successfully closed, moves the database to the parent node.
 * @param[in, out] token is a pointer to the token returned by the lexical analyzer.
 * @param[in] lexicalAnalyzer is the lexical analyzer used to read tokens from the stream.
 * @param[in] closedBlockTerminal is the close block terminal character.
 * @param[in, out] totalNumberBlockTerminals is the total number of block terminals.
 * @return false if the totalNumberBlockTerminals becomes negative.
 */
static bool CheckCloseBlock(Token* &token,
                            LexicalAnalyzer &lexicalAnalyzer,
                            StructuredDataI &database,
                            const char8 closedBlockTerminal,
                            int32 &totalNumberBlockTerminals) {
    bool ok = true;
    while ((token->GetId() == TERMINAL_TOKEN) && (ok)) {
        char8 terminal = *reinterpret_cast<const char8*>(token->GetData());
        if (terminal == closedBlockTerminal) {
            totalNumberBlockTerminals--;
            ok = (totalNumberBlockTerminals >= 0);
            if (ok) {
                //?? could fail ?
                ok = database.MoveToAncestor(1u);
            }
        }
        token = lexicalAnalyzer.GetToken();
    }
    return ok;
}


/**
 * @brief Reading the type cast string, returns the type identifier for the token storing.
 * @param[in, out] token is a pointer to the token returned by the lexical analyzer.
 * @param[in] lexicalAnalyzer is the lexical analyzer used to read tokens from the stream.
 * @param[in] terminalCloseTypeCast is the close terminal character of the type cast expression.
 * @param[out] typeIndex is the type identifier.
 * @param[out] err is the stream to print eventual error messages.
 * @return true if the type cast expression is read correctly, false otherwise.
 */
static bool SetTypeCast(Token* &token,
                        LexicalAnalyzer &lexicalAnalyzer,
                        const char8 terminalCloseTypeCast,
                        StreamString &type,
                        BufferedStreamI * const err) {

    type = "string";
    if (token->GetId() == STRING_TOKEN) {
        type = token->GetData();
        token = lexicalAnalyzer.GetToken();
    }

    bool ret = true;
    if (token->GetId() != TERMINAL_TOKEN) {
        PrintErrorOnStream("\nExpected type cast close terminal! [%d]", token->GetLineNumber(), err);
        ret = false;
    }
    else {
        char8 terminal = token->GetData()[0];
        if (terminal != terminalCloseTypeCast) {
            PrintErrorOnStream("\nInvalid terminal! [%d]", token->GetLineNumber(), err);
            ret = false;
        }
    }
    return ret;

}


/**
 * @brief Reads a scalar.
 * @param[in, out] token is a pointer to the token returned by the lexical analyzer.
 * @param[in] lexicalAnalyzer is the lexical analyzer used to read tokens from the stream.
 * @param[in, out] database is the StructuredDataI where the scalar will be stored.
 * @param[in] name is the name of the vector variable.
 * @param[in] typeIndex represents the type the token will be converted to.
 * @return true if the scalar variable expression is read correctly, false otherwise.
 */
static bool ReadScalar(Token* &token,
                       LexicalAnalyzer &lexicalAnalyzer,
                       StructuredDataI &database,
                       const char8 * const name,
                       const char8 * const type) {

    AnyTypeCreator scalarCreator(1u);
    bool ret = (scalarCreator.ToType(type, token->GetData()));
    if (ret) {
        AnyType element;
        uint32 dimSizes[3] = { 1u, 1u, 1u };
        /*lint -e{613} . Justification: if (memory==NULL) ---> (ret==false) */
        scalarCreator.SetType(element, 0u, dimSizes);
        ret = database.Write(name, element);
        // read the new token
        token = lexicalAnalyzer.GetToken();
    }

    return ret;
}


/**
 * @brief Reads a vector.
 * @param[in, out] token is a pointer to the token returned by the lexical analyzer.
 * @param[in] lexicalAnalyzer is the lexical analyzer used to read tokens from the stream.
 * @param[in, out] database is the StructuredDataI where the vector will be stored.
 * @param[in] vectorCloseTerminal is the close vector terminal character.
 * @param[in] name is the name of the vector variable.
 * @param[in] typeIndex represents the type the token will be converted to.
 * @param[out] err is the stream used to print eventual error messages.
 * @return true if the vector variable expression is read correctly, false otherwise.
 */
static bool ReadVector(Token* &token,
                       LexicalAnalyzer &lexicalAnalyzer,
                       StructuredDataI &database,
                       const char8 vectorCloseTerminal,
                       const char8 * const name,
                       const char8 * const type,
                       BufferedStreamI * const err) {

    uint32 tokenType = 0u;

    AnyTypeCreator vectorCreator(4u);

    bool ok = true;
    uint32 numberOfElements = 0u;

    while (ok) {
        // take the type at the beginning
        tokenType = token->GetId();
        bool isString = (token->GetId() == STRING_TOKEN);
        bool isNumber = (token->GetId() == NUMBER_TOKEN);
        bool continueRead = (isString) || (isNumber);
        // check if a token is not end or terminal
        while ((continueRead) && (ok)) {
            if (token->GetId() != tokenType) {
                // can be the vector terminator... go to END_VECTOR
                continueRead = false;
            }
            else {
                if (!vectorCreator.ToType(type, token->GetData())) {
                    PrintErrorOnStream("\nFailed type conversion! [%d]", token->GetLineNumber(), err);
                    ok = false;
                }
                else {
                    numberOfElements++;
                    token = lexicalAnalyzer.GetToken();
                }
            }
        }
        if (ok) {
            if (token->GetId() != TERMINAL_TOKEN) {
                PrintErrorOnStream("\nInvalid token! [%d]", token->GetLineNumber(), err);
                ok = false;
            }
            else {
                char8 terminal = token->GetData()[0];
                if (terminal != vectorCloseTerminal) {
                    PrintErrorOnStream("\nInvalid terminal! [%d]", token->GetLineNumber(), err);
                    ok = false;
                }
                else {
                    // done!
                    break;
                }
            }
        }
    }

    // be sure that the vector is not empty!
    if (ok) {
        if (vectorCreator.GetSize() == 0u) {
            PrintErrorOnStream("\nEmpty vector! [%d]", token->GetLineNumber(), err);
            ok = false;
        }
    }
    // serialize the element
    if (ok) {
        AnyType element;
        uint32 dimSizes[3] = { numberOfElements, 1u, 1u };
        /*lint -e{613} . Justification: if (memory==NULL) ---> (ok==false) */
        vectorCreator.SetType(element, 1u, dimSizes);
        ok = database.Write(name, element);
        // read the new token
        token = lexicalAnalyzer.GetToken();
    }
    return ok;
}


/**
 * @brief Reads a matrix.
 * @param[in, out] token is a pointer to the token returned by the lexical analyzer.
 * @param[in] lexicalAnalyzer is the lexical analyzer used to read tokens from the stream.
 * @param[in, out] database is the StructuredDataI where the matrix will be stored.
 * @param[in] vectorOpenTerminal is the open vector terminal character.
 * @param[in] vectorCloseTerminal is the close vector terminal character.
 * @param[in] matrixOpenTerminal is the close matrix terminal character.
 * @param[in] name is the name of the matrix variable.
 * @param[in] typeIndex represents the type the token will be converted to.
 * @param[out] err is the stream used to print eventual error messages.
 * @return true if the matrix variable expression is read correctly, false otherwise.
 */
static bool ReadMatrix(Token* &token,
                       LexicalAnalyzer &lexicalAnalyzer,
                       StructuredDataI &database,
                       const char8 vectorOpenTerminal,
                       const char8 vectorCloseTerminal,
                       const char8 matrixCloseTerminal,
                       const char8 * const name,
                       const char8* const type,
                       BufferedStreamI * const err) {

    uint32 tokenType = 0u;
    AnyTypeCreator matrixCreator(16u);

    bool ok = true;
    uint32 numberOfColumns = 0u;
    uint32 numberOfRows = 0u;
    uint32 testNumberOfColumns = numberOfColumns;

    const uint32 BEGIN_VECTOR = 0u;
    const uint32 READ = 1u;
    const uint32 END_VECTOR = 2u;

    uint32 status = BEGIN_VECTOR;

    while (ok) {
        if (status == BEGIN_VECTOR) {
            if (token->GetId() != TERMINAL_TOKEN) {
                PrintErrorOnStream("\nExpected open vector or close matrix terminals! [%d]", token->GetLineNumber(), err);
                ok = false;
            }
            else {
                char8 terminal = token->GetData()[0];
                if (terminal == matrixCloseTerminal) {
                    // done!!!
                    break;
                }
                else if (terminal != vectorOpenTerminal) {
                    PrintErrorOnStream("\nExpected open vector or close matrix terminals! [%d]", token->GetLineNumber(), err);
                    ok = false;
                }
                else {
                    status = READ;
                    token = lexicalAnalyzer.GetToken();
                }
            }
        }

        if (status == READ) {
            if (tokenType == 0u) {
                // take the type at the beginning
                tokenType = token->GetId();
            }
            bool isString = (token->GetId() == STRING_TOKEN);
            bool isNumber = (token->GetId() == NUMBER_TOKEN);
            bool continueRead = (isString) || (isNumber);
            testNumberOfColumns = 0u;
            // check if a token is not end or terminal
            while ((continueRead) && (ok)) {
                if (token->GetId() != tokenType) {
                    // can be the vector terminator... go to END_VECTOR
                    continueRead = false;
                }
                else {
                    if (!matrixCreator.ToType(type, token->GetData())) {
                        PrintErrorOnStream("\nFailed type conversion! [%d]", token->GetLineNumber(), err);
                        ok = false;
                    }
                    else {
                        if (numberOfRows == 0u) {
                            numberOfColumns++;
                            testNumberOfColumns++;
                        }
                        else {
                            testNumberOfColumns++;
                        }
                        token = lexicalAnalyzer.GetToken();
                    }
                }
            }
            if (ok) {
                status = END_VECTOR;
            }
        }

        if (status == END_VECTOR) {
            if (token->GetId() != TERMINAL_TOKEN) {
                PrintErrorOnStream("\nInvalid token! [%d]", token->GetLineNumber(), err);
                ok = false;
            }
            else {
                char8 terminal = token->GetData()[0];
                if (terminal != vectorCloseTerminal) {
                    PrintErrorOnStream("\nInvalid terminal! [%d]", token->GetLineNumber(), err);
                    ok = false;
                }
                else {
                    if (testNumberOfColumns != numberOfColumns) {
                        PrintErrorOnStream("\nIncorrect matrix format! [%d]", token->GetLineNumber(), err);
                        ok = false;
                    }
                    else {
                        numberOfRows++;
                        token = lexicalAnalyzer.GetToken();
                        status = BEGIN_VECTOR;
                    }
                }
            }
        }
    }

    //be sure that the matrix is not empty!
    if (ok) {
        if (matrixCreator.GetSize() == 0u) {
            PrintErrorOnStream("\nEmpty matrix! [%d]", token->GetLineNumber(), err);
            ok = false;
        }
    }

    // serialize the element
    if (ok) {
        AnyType element;
        uint32 dimSizes[3] = { numberOfColumns, numberOfRows, 1u };
        /*lint -e{613} . Justification: if (memory==NULL) ---> (ok==false) */
        matrixCreator.SetType(element, 2u, dimSizes);
        ok = database.Write(name, element);

        // read the new token
        token = lexicalAnalyzer.GetToken();
    }
    return ok;
}

/**
 * @brief Determines if the next variable is a matrix or a vector.
 * @param[in] token is the token to be checked.
 * @param[in] terminal is the previous token found (an open vector terminal or open matrix terminal).
 * @param[in] vectorOpenTerminal is the open vector terminal character.
 * @param[in] vectorOpenTerminal is the open matrix terminal character.
 * @return true if a vector variable is matched, false if it is a matrix.
 */
static bool IsVector(Token * const &token,
                     const char8 terminal,
                     const char8 vectorOpenTerminal,
                     const char8 matrixOpenTerminal) {

    bool ret = false;
    if (terminal == vectorOpenTerminal) {
        if (terminal == matrixOpenTerminal) {
            if (token->GetId() == TERMINAL_TOKEN) {
                char8 readTerminal = token->GetData()[0];
                // this is a matrix for sure!
                ret = (readTerminal != vectorOpenTerminal);
            }
            else {
                ret = true;
            }
        }
        else {
            ret = true;
        }
    }
    return ret;
}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
Parser::Parser(const ParserGrammar &grammarIn) :
        Object() {
    grammar = grammarIn;
}

ParserGrammar Parser::GetGrammar() const {
    return grammar;
}

Parser::~Parser() {
}

bool Parser::Parse(StreamI &stream,
                   StructuredDataI &database,
                   BufferedStreamI * const err) const {

    LexicalAnalyzer lexicalAnalyzer(stream, &grammar.assignment, grammar.separators);

    char8 genericTerminal = '\0';
    StreamString varName = "";
    StreamString nodeName = "";
    // default is CString.
    StreamString typeName = "string";

    // loop here

    Token *token = lexicalAnalyzer.GetToken();

    int32 totalBlockTerminals = 0;

    const uint32 LVALUE = 0u;
    const uint32 BLOCK = 1u;
    const uint32 RVALUE = 2u;
    const uint32 VARIABLE = 3u;

    uint32 status = LVALUE;

    bool ok = true;
    while (ok) {

        if (token->GetId() == EOF_TOKEN) {
            if (totalBlockTerminals != 0) {
                PrintErrorOnStream("\nUnmatched close block terminals! [%d]", token->GetLineNumber(), err);
                ok = false;
            }
            // done!!
            break;
        }

        if (token->GetId() == ERROR_TOKEN) {
            PrintErrorOnStream("\nInvalid token! [%d]", token->GetLineNumber(), err);
            ok = false;
        }

        // Read the lvalue ( string = )
        if (status == LVALUE) {
            if (!isLValue(token, lexicalAnalyzer, grammar.assignment, varName)) {
                // block checking... this is not a block
                if (genericTerminal != '\0') {
                    status = VARIABLE;
                }
                else {
                    // not an LValue without a previous block. This is something like:
                    //a=10
                    //ab1234
                    PrintErrorOnStream("\nExpected lvalue! [%d]", token->GetLineNumber(), err);
                    ok = false;
                }
            }
            else {
                // it is a block!
                if (genericTerminal != '\0') {
                    if (database.CreateRelative(nodeName.Buffer())) {
                        totalBlockTerminals++;
                        genericTerminal = '\0';
                        nodeName = "";
                    }
                    else {
                        ok = false;
                        PrintErrorOnStream("\nFailed new node creation! [%d]", token->GetLineNumber(), err);
                    }
                }
                if (ok) {
                    // check if a block follows
                    status = BLOCK;
                }
            }
        }

        if (status == BLOCK) {
            if (token->GetId() == TERMINAL_TOKEN) {
                // a terminal found! block or matrix
                genericTerminal = *(token->GetData());
                if (genericTerminal == grammar.openBlock) {
                    nodeName = varName;
                    token = lexicalAnalyzer.GetToken();
                    status = LVALUE;
                }
                else {
                    // could be a vector or matrix
                    status = RVALUE;
                }
            }
            else {
                // could be a scalar
                status = RVALUE;
            }
        }

        if (status == RVALUE) {
            // an lvalue found!
            // create a node in the CGDB using varName
            bool isString = (token->GetId() == STRING_TOKEN);
            bool isNumber = (token->GetId() == NUMBER_TOKEN);

            bool isVariable = (isString) || (isNumber);

            if (isVariable) {
                status = VARIABLE;
            }
            if (token->GetId() == TERMINAL_TOKEN) {

                // a terminal found! block or matrix
                genericTerminal = *(token->GetData());
                if ((genericTerminal == grammar.openMatrix) || (genericTerminal == grammar.openVector)) {
                    token = lexicalAnalyzer.GetToken();
                    status = VARIABLE;
                }
                else if (genericTerminal == grammar.openTypeCast) {
                    token = lexicalAnalyzer.GetToken();
                    // next has to be scalar or matrix
                    if (SetTypeCast(token, lexicalAnalyzer, grammar.closeTypeCast, typeName, err)) {
                        token = lexicalAnalyzer.GetToken();
                        status = RVALUE;
                    }
                    else {
                        PrintErrorOnStream("\nError in type cast expression \"(type)\"! [%d]", token->GetLineNumber(), err);
                        ok = false;
                    }
                }
                else {
                    PrintErrorOnStream("\nUnexpected Terminal! [%d]", token->GetLineNumber(), err);
                    ok = false;
                }
            }
        }

        if (status == VARIABLE) {
            if ((genericTerminal == grammar.openMatrix) || (genericTerminal == grammar.openVector)) {
                if (IsVector(token, genericTerminal, grammar.openVector, grammar.openMatrix)) {
                    if (ReadVector(token, lexicalAnalyzer, database, grammar.closeVector, varName.Buffer(), typeName.Buffer(), err)) {
                        if (CheckCloseBlock(token, lexicalAnalyzer, database, grammar.closeBlock, totalBlockTerminals)) {
                            status = LVALUE;
                        }
                        else {
                            PrintErrorOnStream("\nToo many closed block terminals! [%d]", token->GetLineNumber(), err);
                            ok = false;
                        }
                    }
                    else {
                        PrintErrorOnStream("\nFailed read vector operation! [%d]", token->GetLineNumber(), err);
                        ok = false;
                    }
                }
                else {
                    if (ReadMatrix(token, lexicalAnalyzer, database, grammar.openVector, grammar.closeVector, grammar.closeMatrix, varName.Buffer(),
                                   typeName.Buffer(), err)) {
                        if (CheckCloseBlock(token, lexicalAnalyzer, database, grammar.closeBlock, totalBlockTerminals)) {
                            status = LVALUE;
                        }
                        else {
                            PrintErrorOnStream("\nToo many closed block terminals! [%d]", token->GetLineNumber(), err);
                            ok = false;
                        }
                    }
                    else {
                        PrintErrorOnStream("\nFailed read matrix operation! [%d]", token->GetLineNumber(), err);
                        ok = false;
                    }
                }
            }
            else {
                // this is a scalar!!
                if (ReadScalar(token, lexicalAnalyzer, database, varName.Buffer(), typeName.Buffer())) {
                    if (CheckCloseBlock(token, lexicalAnalyzer, database, grammar.closeBlock, totalBlockTerminals)) {
                        status = LVALUE;
                    }
                    else {
                        PrintErrorOnStream("\nToo many closed block terminals! [%d]", token->GetLineNumber(), err);
                        ok = false;
                    }
                }
                else {
                    PrintErrorOnStream("\nFailed read scalar operation! [%d]", token->GetLineNumber(), err);
                    ok = false;
                }
            }
            genericTerminal = '\0';
            // reset the default type
            typeName = "string";
        }
    }

    // database created correctly! Move to Root
    if (ok) {
        if (!database.MoveToRoot()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Parse: Failed StructuredDataI::MoveToRoot at the end of parsing");
        }
    }

    return ok;
}

CLASS_REGISTER(Parser, "1.0")
}
