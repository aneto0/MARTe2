/**
 * @file ConfigurationParserI.cpp
 * @brief Source file for class ConfigurationParserI
 * @date 06/05/2020
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
 * the class ConfigurationParserI (public, protected, and private). Be aware that some 
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
#include "ConfigurationParserI.h"
#include "TypeConversion.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static void PrintErrorOnStream(const char8 * const format,
                               const uint32 lineNumber,
                               BufferedStreamI * const err) {
    if (err != NULL) {
        if (!err->Printf(format, lineNumber)) {
            REPORT_ERROR_STATIC(ErrorManagement::FatalError, "PrintErrorOnStream: Failed Printf() on parseError stream");
        }
    }
    
    REPORT_ERROR_STATIC(ErrorManagement::FatalError,  format, lineNumber);
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

ConfigurationParserI::ConfigurationParserI(StreamI &stream,
                               StructuredDataI &databaseIn,
                               BufferedStreamI * const err,
                               const GrammarInfo &grammarIn)
        : ParserI(stream, err, grammarIn)
        , memory(1u) {
            
    numberOfColumns = 0u;
    firstNumberOfColumns = 0u;
    numberOfRows = 0u;
    database = &databaseIn;
    tokenType = 0u;
    numberOfDimensions = 0u;
    
}

ConfigurationParserI::~ConfigurationParserI() {
    database=static_cast<StructuredDataI*>(NULL);
}

void ConfigurationParserI::GetTypeCast() {
    typeName = GetCurrentTokenData(currentToken);
}

void ConfigurationParserI::BlockEnd() {
    if (!database->MoveToAncestor(1u)) {
        PrintErrorOnStream("Failed StructuredDataI::MoveToAncestor(1)! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

void ConfigurationParserI::CreateNode() {
    if (!database->CreateRelative(GetCurrentTokenData(currentToken))) {
        PrintErrorOnStream("Failed StructuredDataI::CreateRelative(*)! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

void ConfigurationParserI::AddLeaf() {
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
            PrintErrorOnStream("Failed adding a leaf to the configuration database! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
            isError = true;
        }
    }
    else {
        PrintErrorOnStream("Possible empty vector or matrix! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
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

void ConfigurationParserI::GetNodeName() {
    nodeName = GetCurrentTokenData(currentToken);
}

void ConfigurationParserI::AddScalar() {
    if (tokenType == 0u) {
        tokenType = GetCurrentTokenId(currentToken);
    }

    if (tokenType == GetCurrentTokenId(currentToken)) {
        bool ret = memory.Add(typeName.Buffer(), GetCurrentTokenData(currentToken));

        if (ret) {
            firstNumberOfColumns++;
        }
        else {
            PrintErrorOnStream("Failed read or conversion! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
            isError = true;
        }
    }
    else {
        PrintErrorOnStream("Cannot mix different types in a vector or matrix! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

void ConfigurationParserI::EndVector() {
    if (numberOfColumns == 0u) {
        numberOfColumns = firstNumberOfColumns;
    }
    else {
        if (numberOfColumns != firstNumberOfColumns) {
            PrintErrorOnStream("Incorrect matrix format! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
            isError = true;
        }
    }
    firstNumberOfColumns = 0u;
    numberOfRows++;
    if (numberOfDimensions == 0u) {
        numberOfDimensions = 1u;
    }
}

void ConfigurationParserI::EndMatrix() {
    numberOfDimensions = 2u;
}

void ConfigurationParserI::End() {
    if (!database->MoveToRoot()) {
        PrintErrorOnStream("Failed StructuredDataI::MoveToRoot() at the end! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        isError = true;
    }
}

}
