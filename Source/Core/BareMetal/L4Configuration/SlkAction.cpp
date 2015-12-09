/**
 * @file SlkAction.cpp
 * @brief Source file for class SlkAction
 * @date 04/dic/2015
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
 * the class SlkAction (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SlkAction.h"
#include "AnyType.h"
#include "SlkParse.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

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

SlkAction::SlkAction(StreamI &stream,
                     ConfigurationDatabase &databaseIn,
                     BufferedStreamI * const err,
                     ParserGrammar grammarIn) :
        parseError(err),
        token(stream, &grammarIn.assignment, grammarIn.separators, grammarIn.beginOneLineComment, grammarIn.beginMultipleLinesComment,
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
    initialize_table();
}

SlkAction::~SlkAction() {

}

ParserGrammar SlkAction::GetGrammar() const {
    return grammar;
}

void SlkAction::predict(uint16 a) {

}

void SlkAction::GetTypeCast() {
    printf("\nget type cast %s\n", token.GetTokenData());
    typeName = token.GetTokenData();
}

void SlkAction::BlockEnd() {
    printf("\nEnd the block!\n");
    if (!database->MoveToAncestor(1u)) {
        PrintErrorOnStream("\nFailed ConfigurationDatabase::MoveToAncestor(1)! [%d]", token.GetLineNumber(), errorStream);
        parseError.SetError();
    }
}

void SlkAction::CreateNode() {
    printf("\nCreating node... %s\n", token.GetTokenData());
    database->CreateRelative(token.GetTokenData());

}

void SlkAction::AddLeaf() {
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
            PrintErrorOnStream("\nFailed adding a leaf to the configuration database! [%d]", token.GetLineNumber(), errorStream);
            parseError.SetError();
        }
    }
    else {
        PrintErrorOnStream("\nPossible empty vector or matrix! [%d]", token.GetLineNumber(), errorStream);
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

void SlkAction::GetNodeName() {
    printf("\nGetting node name %s\n", token.GetTokenData());
    nodeName = token.GetTokenData();
}

void SlkAction::AddScalar() {
    printf("\nAdding a scalar %s\n", token.GetTokenData());
    if (tokenType == 0u) {
        tokenType = token.GetTokenId();
    }

    if (tokenType == token.GetTokenId()) {
        bool ret = memory.ToType(typeName.Buffer(), token.GetTokenData());

        if (ret) {
            firstNumberOfColumns++;
        }
        else {
            PrintErrorOnStream("\nFailed read or conversion! [%d]", token.GetLineNumber(), errorStream);
            parseError.SetError();
        }
    }
    else {
        PrintErrorOnStream("\nCannot mix different types in a vector or matrix! [%d]", token.GetLineNumber(), errorStream);
        parseError.SetError();

    }
}

void SlkAction::CreateClassLeaf() {
    printf("\nAdd The class name!\n");
// Aggiungi un elemento alla memoria
    bool ret = memory.ToType(typeName.Buffer(), token.GetTokenData());
    if (ret) {
        AnyType element;
        uint32 dimSizes[3] = { 1u, 1u, 1u };
        /*lint -e{613} . Justification: if (memory==NULL) ---> (ret==false) */
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

void SlkAction::EndVector() {
    printf("\nEnd Row\n");
    if (numberOfColumns == 0u) {
        numberOfColumns = firstNumberOfColumns;
    }
    else {
        if (numberOfColumns != firstNumberOfColumns) {
            PrintErrorOnStream("\nIncorrect matrix format! [%d]", token.GetLineNumber(), errorStream);
            parseError.SetError();
        }
    }
    firstNumberOfColumns = 0u;
    numberOfRows++;
    if (numberOfDimensions == 0u) {
        numberOfDimensions = 1u;
    }
}

void SlkAction::EndMatrix() {
    numberOfDimensions = 2u;
}

void SlkAction::End() {
    if (!database->MoveToRoot()) {
        PrintErrorOnStream("\nFailed ConfigurationDatabase::MoveToRoot() at the end! [%d]", token.GetLineNumber(), errorStream);
        parseError.SetError();
    }
}

bool SlkAction::Parse() {
    typeName = defaultTypeName;

    while ((!parseError.IsError()) && (!parseError.IsEOF())) {
        SlkParse(*this, token, parseError, 0u);
    }

    return !parseError.IsError();
}

