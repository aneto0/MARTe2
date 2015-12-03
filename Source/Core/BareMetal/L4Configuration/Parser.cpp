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
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/


namespace MARTe {

struct TypeCastInfo {
    TypeDescriptor typeDes;
    const char8 *castName;
};

static const TypeCastInfo castTypes[] = { { ConstCharString, "string" }, { SignedInteger8Bit, "int8" }, { SignedInteger16Bit, "int16" }, { SignedInteger32Bit,
        "int32" }, { SignedInteger64Bit, "int64" }, { UnsignedInteger8Bit, "uint8" }, { UnsignedInteger16Bit, "uint16" }, { UnsignedInteger32Bit, "uint32" }, {
        UnsignedInteger64Bit, "uint64" }, { Float32Bit, "float32" }, { Float64Bit, "float64" }, { ConstCharString, static_cast<const char8*>(NULL) } };



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

static void SetType(const uint32 typeIndex,
                    void * const dataPointer,
                    AnyType &element,
                    const uint8 nOfDimensions) {

    if ((castTypes[typeIndex].typeDes.type == CCString) && (nOfDimensions == 0u)) {
        element = AnyType(castTypes[typeIndex].typeDes, static_cast<uint8>(0u), *static_cast<char8**>(dataPointer));
    }
    else {
        element = AnyType(castTypes[typeIndex].typeDes, static_cast<uint8>(0u), dataPointer);
    }
    element.SetNumberOfDimensions(nOfDimensions);
}

static bool CheckCloseBlock(Token* &token,
                            LexicalAnalyzer &lexicalAnalyzer,
                            ConfigurationDatabase &database,
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

static bool ToType(const char8 * const tokenBuffer,
                   const uint32 typeIndex,
                   const uint32 granularity,
                   StaticListHolder *&memory) {

    bool ret = false;
    if (castTypes[typeIndex].typeDes == ConstCharString) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(char8 *)), granularity);
        }
        uint32 size = StringHelper::Length(tokenBuffer) + 1u;
        char8 *cString = static_cast<char8 *>(HeapManager::Malloc(size));
        if (StringHelper::Copy(cString, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&cString));
        }
    }
    if (castTypes[typeIndex].typeDes == SignedInteger8Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(int8)), granularity);
        }
        int8 possibleInt8 = 0;
        if (TypeConvert(possibleInt8, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleInt8));
        }
    }
    if (castTypes[typeIndex].typeDes == SignedInteger16Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(int16)), granularity);
        }
        int16 possibleInt16 = 0;
        if (TypeConvert(possibleInt16, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleInt16));
        }
    }
    if (castTypes[typeIndex].typeDes == SignedInteger32Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(int32)), granularity);
        }
        int32 possibleInt32 = 0;
        if (TypeConvert(possibleInt32, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleInt32));
        }
    }
    if (castTypes[typeIndex].typeDes == SignedInteger64Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(int64)), granularity);
        }
        int64 possibleInt64 = 0;
        if (TypeConvert(possibleInt64, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleInt64));
        }
    }
    if (castTypes[typeIndex].typeDes == UnsignedInteger8Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(uint8)), granularity);
        }
        uint8 possibleUInt8 = 0u;
        if (TypeConvert(possibleUInt8, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleUInt8));
        }
    }
    if (castTypes[typeIndex].typeDes == UnsignedInteger16Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(uint16)), granularity);
        }
        uint16 possibleUInt16 = 0u;
        if (TypeConvert(possibleUInt16, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleUInt16));
        }
    }
    if (castTypes[typeIndex].typeDes == UnsignedInteger32Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(uint32)), granularity);
        }
        uint32 possibleUInt32 = 0u;
        if (TypeConvert(possibleUInt32, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleUInt32));
        }
    }
    if (castTypes[typeIndex].typeDes == UnsignedInteger64Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(uint64)), granularity);
        }
        uint64 possibleUInt64 = 0u;
        if (TypeConvert(possibleUInt64, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleUInt64));
        }
    }
    if (castTypes[typeIndex].typeDes == Float32Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(float32)), granularity);
        }
        float32 possibleFloat32 = 0.0F;
        if (TypeConvert(possibleFloat32, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleFloat32));
        }
    }
    if (castTypes[typeIndex].typeDes == Float64Bit) {
        if (memory == NULL) {
            memory = new StaticListHolder(static_cast<uint32>(sizeof(float64)), granularity);
        }
        float64 possibleFloat64 = 0.0;
        if (TypeConvert(possibleFloat64, tokenBuffer)) {
            ret = memory->Add(reinterpret_cast<void*>(&possibleFloat64));
        }
    }

    if (!ret) {
        delete memory;
        memory = static_cast<StaticListHolder*>(NULL);
    }

    return ret;
}

static bool SetTypeCast(Token* &token,
                 LexicalAnalyzer &lexicalAnalyzer,
                 const char8 terminalCloseTypeCast,
                 uint32 &typeIndex,
                 BufferedStreamI * const err) {

    typeIndex = 0u;
    bool found = false;
    if (token->GetId() == STRING_TOKEN) {
        while ((castTypes[typeIndex].castName != NULL) && (!found)) {
            if (StringHelper::Compare(token->GetData(), castTypes[typeIndex].castName) == 0) {
                found = true;
            }
            else {
                typeIndex++;
            }
        }
        if (!found) {
            PrintErrorOnStream("\nSpecified type not found, automatic cast to string! [%d]", token->GetLineNumber(), err);
            typeIndex = 0u;
        }
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

static bool ReadScalar(Token* &token,
                       LexicalAnalyzer &lexicalAnalyzer,
                       ConfigurationDatabase &database,
                       const char8 * const name,
                       const uint32 typeIndex) {

    StaticListHolder *memory = static_cast<StaticListHolder *>(NULL);
    bool ret = ToType(token->GetData(), typeIndex, 1u, memory);
    if (ret) {
        AnyType element;
        /*lint -e{613} . Justification: if (memory==NULL) ---> (ret==false) */
        SetType(typeIndex, memory->GetAllocatedMemory(), element, 0u);
        ret = database.Write(name, element);
        // read the new token
        token = lexicalAnalyzer.GetToken();
    }
    if (memory != NULL) {
        // in this case delete the string on heap
        if (memory->GetSize() > 0u) {
            if (castTypes[typeIndex].typeDes == ConstCharString) {
                char8* stringElement = reinterpret_cast<char8 **>(memory)[0];
                if (!HeapManager::Free(reinterpret_cast<void* &>(stringElement))) {
                    REPORT_ERROR(ErrorManagement::FatalError, "ReadMatrix: Failed HeapManager::Free()");
                }
            }
        }
        delete memory;
    }

    return ret;
}

static bool ReadVector(Token* &token,
                       LexicalAnalyzer &lexicalAnalyzer,
                       ConfigurationDatabase &database,
                       const char8 vectorCloseTerminal,
                       const char8 * const name,
                       const uint32 typeIndex,
                       BufferedStreamI * const err) {

    uint32 tokenType = 0u;

    StaticListHolder *memory = static_cast<StaticListHolder *>(NULL);

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
                if (!ToType(token->GetData(), typeIndex, 4u, memory)) {
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
        if (memory == NULL) {
            PrintErrorOnStream("\nEmpty vector! [%d]", token->GetLineNumber(), err);
            ok = false;
        }
    }
    // serialize the element
    if (ok) {
        AnyType element;
        /*lint -e{613} . Justification: if (memory==NULL) ---> (ok==false) */
        SetType(typeIndex, memory->GetAllocatedMemory(), element, static_cast<uint8>(1u));
        element.SetNumberOfElements(0u, numberOfElements);
        ok = database.Write(name, element);
        // read the new token
        token = lexicalAnalyzer.GetToken();
    }
    if (memory != NULL) {
        if (castTypes[typeIndex].typeDes == ConstCharString) {
            uint32 memorySize = memory->GetSize();
            for (uint32 i = 0u; i < memorySize; i++) {
                char8* elementToFree = reinterpret_cast<char8 **>(memory->GetAllocatedMemory())[i];
                if (!HeapManager::Free(reinterpret_cast<void* &>(elementToFree))) {
                    REPORT_ERROR(ErrorManagement::FatalError, "ReadMatrix: Failed HeapManager::Free()");
                }
            }
        }
        delete memory;
    }
    return ok;
}

static bool ReadMatrix(Token* &token,
                       LexicalAnalyzer &lexicalAnalyzer,
                       ConfigurationDatabase &database,
                       const char8 vectorOpenTerminal,
                       const char8 vectorCloseTerminal,
                       const char8 matrixCloseTerminal,
                       const char8 * const name,
                       const uint32 typeIndex,
                       BufferedStreamI * const err) {

    uint32 tokenType = 0u;

    StaticListHolder *memory = static_cast<StaticListHolder *>(NULL);

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
            // take the type at the beginning
            tokenType = token->GetId();
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
                    if (!ToType(token->GetData(), typeIndex, 16u, memory)) {
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
        if (memory == NULL) {
            PrintErrorOnStream("\nEmpty matrix! [%d]", token->GetLineNumber(), err);
            ok = false;
        }
    }

    // serialize the element
    if (ok) {
        AnyType element;
        /*lint -e{613} . Justification: if (memory==NULL) ---> (ok==false) */
        SetType(typeIndex, memory->GetAllocatedMemory(), element, static_cast<uint8>(2u));
        element.SetNumberOfElements(0u, numberOfColumns);
        element.SetNumberOfElements(1u, numberOfRows);
        ok = database.Write(name, element);

        // read the new token
        token = lexicalAnalyzer.GetToken();
    }

    if (memory != NULL) {
        uint32 memorySize = memory->GetSize();
        if (castTypes[typeIndex].typeDes == ConstCharString) {
            for (uint32 i = 0u; i < memorySize; i++) {
                char8* elementToFree = reinterpret_cast<char8 **>(memory->GetAllocatedMemory())[i];
                if (!HeapManager::Free(reinterpret_cast<void* &>(elementToFree))) {
                    REPORT_ERROR(ErrorManagement::FatalError, "ReadMatrix: Failed HeapManager::Free()");
                }
            }
        }
        delete memory;
    }
    return ok;
}

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
Parser::Parser(const ParserGrammatic &grammaticIn) :
        Object() {
    grammatic = grammaticIn;
}

ParserGrammatic Parser::GetGrammatic() const {
    return grammatic;
}

Parser::~Parser() {
}

bool Parser::Parse(StreamI &stream,
                   ConfigurationDatabase &database,
                   BufferedStreamI * const err) const {

    LexicalAnalyzer lexicalAnalyzer(stream, &grammatic.assignment, grammatic.separators);

    char8 genericTerminal = '\0';
    StreamString varName = "";
    StreamString nodeName = "";
    // loop here

    Token *token = lexicalAnalyzer.GetToken();

    //Index of types table
    // default is CString. See castTypes on top
    uint32 typeIndex = 0u;

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
            if (!isLValue(token, lexicalAnalyzer, grammatic.assignment, varName)) {
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
                if (genericTerminal == grammatic.openBlock) {
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
                if ((genericTerminal == grammatic.openMatrix) || (genericTerminal == grammatic.openVector)) {
                    token = lexicalAnalyzer.GetToken();
                    status = VARIABLE;
                }
                else if (genericTerminal == grammatic.openTypeCast) {
                    token = lexicalAnalyzer.GetToken();
                    // next has to be scalar or matrix
                    if (SetTypeCast(token, lexicalAnalyzer, grammatic.closeTypeCast, typeIndex, err)) {
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
            if ((genericTerminal == grammatic.openMatrix) || (genericTerminal == grammatic.openVector)) {
                if (IsVector(token, genericTerminal, grammatic.openVector, grammatic.openMatrix)) {
                    if (ReadVector(token, lexicalAnalyzer, database, grammatic.closeVector, varName.Buffer(), typeIndex, err)) {
                        if (CheckCloseBlock(token, lexicalAnalyzer, database, grammatic.closeBlock, totalBlockTerminals)) {
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
                    if (ReadMatrix(token, lexicalAnalyzer, database, grammatic.openVector, grammatic.closeVector, grammatic.closeMatrix, varName.Buffer(),
                                   typeIndex, err)) {
                        if (CheckCloseBlock(token, lexicalAnalyzer, database, grammatic.closeBlock, totalBlockTerminals)) {
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
                if (ReadScalar(token, lexicalAnalyzer, database, varName.Buffer(), typeIndex)) {
                    if (CheckCloseBlock(token, lexicalAnalyzer, database, grammatic.closeBlock, totalBlockTerminals)) {
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
            typeIndex = 0u;
        }
    }

    // database created correctly! Move to Root
    if (ok) {
        if (!database.MoveToRoot()) {
            REPORT_ERROR(ErrorManagement::FatalError, "Parse: Failed ConfigurationDatabase::MoveToRoot at the end of parsing");
        }
    }

    return ok;
}

CLASS_REGISTER(Parser, "1.0")
}
