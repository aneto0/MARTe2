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

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/** */
Parser::Parser() :
        stack(sizeof(TokenDataType), 64u) {
    lexicalAnalyzer.AddSeparators("\n\r\t ");
    lexicalAnalyzer.AddTerminals("{}()=*");
}

/** */
virtual Parser::~Parser() {
}

/** @param errorLevel
 0 means do not display
 1 means show actions
 2 means show productions
 0xFFFFFFFF means show all      */
bool Parser::Parse(StreamI &stream,
                   StreamI *err,
                   int32 errorLevel) {

    // the parser status in the processing of the stream
    const CDBParserState * status = &CDBPS_lvalue;
    // the name of the variable
    FString lValue;
    // the value of the variable
    FString rValue;
    // contains the array ?
    FString array;
    // count the opened parenthesys within the assignment
    int parLevel = 0;
    // counts the overall parenthesys (opened and closed)
    int parCount = 0;
    // line Counter
    int lineCount = 1;
    // the name of the variable
    FString typeValue;
    //
    CDBTYPE cdbType = CDBTYPE_Interpret;

    typeValue = "";
    static int linkCounter = 0;

    if (err && parserReportEnabled)
        err->Printf(">>START<<\n");

    bool globalRet = True;

    CDBNode *node = this;

    LexicalAnalyzer la;
    la.AddSeparators("\n\r\t ");
    la.AddTerminals("{}(),=");
    la.ChangeTokenCode("EOF", CDBTV_EOF);
    la.ChangeTokenCode("IDENT", CDBTV_Ident);
    la.ChangeTokenCode("NUMBER", CDBTV_Number);
    la.ChangeTokenCode("FLOAT", CDBTV_Float);
    la.ChangeTokenCode("ERROR", CDBTV_Error);

    LA_TokenData *latd;

    // stores the terminator
    while ((latd = la.GetToken(stream)) != NULL) {

        // if the token is the terminator, exit
        if (latd->Token() == CDBTV_EOF)
            break;

        // exit with error in case of error token
        if (latd->Token() == CDBTV_Error) {
            if (err)
                err->Printf("Line[%i] lexical error %s %s\n", latd->LineNumber(), latd->Description(), latd->Data());
            AssertErrorCondition(SyntaxError, "Line[%i] lexical error %s %s\n", latd->LineNumber(), latd->Description(), latd->Data());
            return False;
        }

        // print informations about the new node on the output stream
        if ((err != NULL) && parserReportEnabled) {
            FString location;
            node->SubTreeName(location, ".");
            err->Printf("Line[%i] {C=%i,L=%i} base=%s type=%s status=%s value=%s\n", latd->LineNumber(), parCount, parLevel, location.Buffer(),
                        latd->Description(), status->name, latd->Data());
        }

        switch (status->value) {
        // left value
        case CDBPSV_lvalue: {
            // get the new token
            switch (latd->Token()) {
            case CDBTV_Number:
            case CDBTV_Ident: {
                // if number of ident if there is a star means a link
                if (latd->Data()[0] == '*') {
                    FString name;
                    // the name of the node
                    name.Printf("link%i", linkCounter++);
                    // build the name of the link (begin after the *)
                    const char *linkTo = latd->Data() + 1;
                    // write the node
                    bool ret = node->WriteArray(name.Buffer(), &linkTo, CDBTYPE_String, NULL, 1, CDBN_CreateLinkNode, sorter);
                    globalRet = globalRet && ret;
                }
                // in this case expects an =
                else {
                    status = &CDBPS_equal;
                    lValue = latd->Data();
                }
            }
                break;

                // if there is a } (and the block is not finished) moves to the father and expects a new lvalue
            case CDBTV_CloseCurly: {
                if (parCount > 0) {
                    parCount--;
                    node = node->Father();
                    status = &CDBPS_lvalue;
                    lValue = "";
                }
                // if the block is finished...
                else {
                    if (err != NULL) {
                        err->Printf("Line[%i] unmatched %s at state %s\n", latd->LineNumber(), latd->Description(), status->name);
                    }
                    AssertErrorCondition(SyntaxError, "Line[%i] unmatched %s at state %s\n", latd->LineNumber(), latd->Description(), status->name);
                    return False;
                }

            }
                break;
            default: {
                if (err != NULL) {
                    err->Printf("Line[%i] unexpected type %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                }
                AssertErrorCondition(SyntaxError, "Line[%i] unexpected type %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                return False;
            }
            }
        }
            break;

            // expects an =
        case CDBPSV_equal: {
            // if an = is matched expects an rvalue
            if (latd->Token() == CDBTV_Equal) {
                status = &CDBPS_rvalue;
                // not assigned type at this stage
                typeValue = "";
                cdbType.containerClassName = NULL;
            }
            // else return false
            else {
                if (err != NULL) {
                    err->Printf("Line[%i] unexpected type %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                }
                AssertErrorCondition(SyntaxError, "Line[%i] unexpected type %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                status = &CDBPS_lvalue;
                return False;
            }
        }
            break;

            // expects an rvalue
        case CDBPSV_rvalue: {
            switch (latd->Token()) {
            // expects one of this token types
            // and write them as strings
            case CDBTV_Number:
            case CDBTV_Float:
            case CDBTV_Ident: {

                FString buffer;
                // encapsulate with curlies to allow parsing of strings with spaces ????????????????????????????????? NO
                buffer.Printf("\"%s\"", latd->Data());
                const char *buf = buffer.Buffer();

                bool ret = node->WriteArray(lValue.Buffer(), &buf, cdbType, NULL, 1, CDBN_CreateStringNode, sorter);
                if (!ret) {
                    if (err)
                        err->Printf("Line[%i] cannot write %s at node %s\n", latd->LineNumber(), latd->Data(), lValue.Buffer());
                    globalRet = False;
                    AssertErrorCondition(FatalError, "Line[%i] cannot write %s at node %s\n", latd->LineNumber(), latd->Data(), lValue.Buffer());
                }

                // NEXT STATUS !
                status = &CDBPS_lvalue;
            }
                break;

                // if a { is found expects a new block
            case CDBTV_OpenCurly: {
                // NEXT STATUS !
                status = &CDBPS_block;
                parCount++;
                rValue = "";
                array = "";
                array = "{\n";
                parLevel = 1;
            }
                break;

                // if a ( is found expects a type cast
            case CDBTV_OpenRound: {
                // NEXT STATUS !
                status = &CDBPS_typeCast;
            }
                break;

                // no valid rvalue found, expects again a new lvalue
            default: {
                // NEXT STATUS !
                status = &CDBPS_lvalue;

                if (err)
                    err->Printf("Line[%i] unexpected type %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                AssertErrorCondition(SyntaxError, "Line[%i] unexpected type %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                return False;
            }
            }
        }
            break;

            // if typecast (..
        case CDBPSV_typeCast: {
            switch (latd->Token()) {
            case CDBTV_Ident: {
                // a type is already defined ! Return false
                if (typeValue.Size() != 0) {
                    if (err)
                        err->Printf("Line[%i] double definition of type was %s now set to %s\n", latd->LineNumber(), typeValue.Buffer(), latd->Data());
                    AssertErrorCondition(SyntaxError, "Line[%i] double definition of type was %s now set to %s\n", latd->LineNumber(), typeValue.Buffer(),
                                         latd->Data());
                    status = &CDBPS_lvalue;
                    return False;
                }

                typeValue = latd->Data();
                cdbType.containerClassName = typeValue.Buffer();
            }
                break;

                // expects an rvalue e go on
            case CDBTV_CloseRound: {
                status = &CDBPS_rvalue;
            }
                break;

            default: {
                if (err)
                    err->Printf("Line[%i] unexpected type %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                AssertErrorCondition(SyntaxError, "Line[%i] unexpected type %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                status = &CDBPS_lvalue;
                return False;
            }
            }
        }
            break;

            // block found {...
        case CDBPSV_block: {
            switch (latd->Token()) {

            // if the token is valid create a child (or move to it if exists)
            case CDBTV_Number:
            case CDBTV_Float:
            case CDBTV_Ident: {
                // we find a link, this means we are in a GroupNode
                if (latd->Data()[0] == '*') {
                    node = node->Children(lValue.Buffer(), CDBN_SearchAndCreate | CDBN_CreateGroupNode, sorter, typeValue.Buffer());

                    typeValue = "";
                    cdbType.containerClassName = NULL;

                    if (node == NULL) {
                        if (err)
                            err->Printf("Line[%i] cannot move/create node %s\n", lineCount, lValue.Buffer());
                        AssertErrorCondition(FatalError, "Line[%i] cannot move/create node %s\n", lineCount, lValue.Buffer());
                        return False;
                    }

                    FString name;
                    name.Printf("link%i", linkCounter++);
                    const char *linkTo = latd->Data() + 1;
                    bool ret = node->WriteArray(name.Buffer(), &linkTo, CDBTYPE_String, NULL, 1, CDBN_CreateLinkNode, sorter);
                    globalRet = globalRet && ret;

                    status = &CDBPS_lvalue;
                    array = "";
                    lValue = "";
                    rValue = "";
                    parLevel = 0;
                }
                // this is not a link!
                else {
                    rValue = latd->Data();
                    status = &CDBPS_block2;
                    array += " \"";
                    array += latd->Data();
                    array += "\" ";
                }
            }
                break;

                // found { this is an array!
            case CDBTV_OpenCurly: {
                parCount++;
                parLevel++;
                array += "{\n";
                status = &CDBPS_block;
            }
                break;

            case CDBTV_CloseCurly: {
                if (parCount > 0) {
                    parCount--;
                }
                // something wrong... i am in a block and the number of total {} is 0 !
                else {
                    if (err)
                        err->Printf("Line[%i] unmatched %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                    AssertErrorCondition(SyntaxError, "Line[%i] unmatched %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                    return False;
                }
                parLevel--;
                array += "}\n";
                // close the array and write it on the node!
                if (parLevel == 0) {
                    const char *buffer = array.Buffer();
                    bool ret = node->WriteArray(lValue.Buffer(), &buffer, cdbType, NULL, 1, CDBN_CreateStringNode, sorter);
                    if (!ret) {
                        if (err)
                            err->Printf("Line[%i] cannot write %s at node %s\n", latd->LineNumber(), array.Buffer(), lValue.Buffer());
                        globalRet = False;
                        AssertErrorCondition(FatalError, "Line[%i] cannot write %s at node %s\n", latd->LineNumber(), array.Buffer(), lValue.Buffer());
                    }

                    array = "";
                    status = &CDBPS_lvalue;
                }
                else {
                    // the array is not finished, we are still in the block.
                    status = &CDBPS_block;
                }
            }
                break;

            default: {
                if (err)
                    err->Printf("Line[%i] unexpected type %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                AssertErrorCondition(SyntaxError, "Line[%i] unexpected type %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                status = &CDBPS_lvalue;
                return False;
            }
            }
        }
            break;

        // i am in a block and this is not a link
        case CDBPSV_block2: {
            // add something to the array
            switch (latd->Token()) {
            case CDBTV_Number:
            case CDBTV_Float:
            case CDBTV_Ident: {
                rValue = "";
                //array += latd->Data();
                array += " \"";
                array += latd->Data();
                array += "\" ";
                array += ' ';
            }
                break;

            // begin the array and return to block
            case CDBTV_OpenCurly: {
                parCount++;
                parLevel++;
                array += "{\n";
                status = &CDBPS_block;
            }
                break;

            // found }
            case CDBTV_CloseCurly: {

                if (parCount > 0) {
                    parCount--;
                }
                else {
                    // error inside a block the total number of {} is 0!
                    if (err)
                        err->Printf("Line[%i] unmatched %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                    AssertErrorCondition(SyntaxError, "Line[%i] unmatched %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                    return False;
                }
                parLevel--;
                // impossible?
                if (parLevel < 0) {
                    if (err)
                        err->Printf("Line[%i] unexpected Negative parLevel %i at status %s\n", latd->LineNumber(), parLevel, status->name);
                    AssertErrorCondition(SyntaxError, "Line[%i] unexpected Negative parLevel %i at status %s\n", latd->LineNumber(), parLevel, status->name);
                    return False;
                }
                array += "}\n";
                // the array is finished
                if (parLevel == 0) {

                    const char *buffer = array.Buffer();
                    bool ret = node->WriteArray(lValue.Buffer(), &buffer, cdbType, NULL, 1, CDBN_CreateStringNode, sorter);
                    if (!ret) {
                        if (err)
                            err->Printf("Line[%i] cannot write %s at node %s\n", latd->LineNumber(), array.Buffer(), lValue.Buffer());
                        AssertErrorCondition(FatalError, "Line[%i] cannot write %s at node %s\n", latd->LineNumber(), array.Buffer(), lValue.Buffer());
                        globalRet = False;
                    }

                    array = "";
                    status = &CDBPS_lvalue;
                }
                else{
                    // return to block
                    status = &CDBPS_block;
                }
            }
                break;

            // found =
            case CDBTV_Equal: {
                // if this is an element not yet assigned
                if (parLevel == 1) {

                    // find the node with the lvalue name and set status to rvalue
                    node = node->Find(lValue.Buffer(), CDBN_SearchAndCreate | CDBN_CreateGroupNode, sorter, typeValue.Buffer());

                    typeValue = "";
                    cdbType.containerClassName = NULL;

                    if (node == NULL) {
                        if (err){
                            err->Printf("Line[%i] cannot move/create node %s\n", latd->LineNumber(), lValue.Buffer());
                        }
                        AssertErrorCondition(FatalError, "Line[%i] cannot move/create node %s\n", latd->LineNumber(), lValue.Buffer());
                        return False;
                    }

                    array = "";
                    lValue = rValue;
                    rValue = "";
                    status = &CDBPS_rvalue;
                    parLevel = 0;

                }
                else {
                    // ??
                    status = &CDBPS_lvalue;
                }
            }
                break;

            default: {
                if (err)
                    err->Printf("Line[%i] unexpected type %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
                status = &CDBPS_lvalue;
                return False;
            }
            }
        }
            break;

        default: {
            if (err)
                err->Printf("Line[%i] unexpected type %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
            AssertErrorCondition(SyntaxError, "Line[%i] unexpected type %s at status %s\n", latd->LineNumber(), latd->Description(), status->name);
            return False;
        }

        }

        delete latd;
        latd = NULL;
    }

    if (parCount != 0) {
        if (err)
            err->Printf("unexpected parCount = %i. Should be 0!\n", parCount);
        AssertErrorCondition(SyntaxError, "unexpected parCount = %i. Should be 0!\n", parCount);
        return False;
    }

    if (err && parserReportEnabled)
        err->Printf(">>END<<\n");
    return globalRet;
}

/** */
const char8 *Parser::GetSymbolName(int32 symbol) {
    if (symbol >= GetStartSymbol()) {
        return GetNonTerminalName(symbol - (GetStartSymbol() - 1));
    }
    else if (symbol > 0) {
        return GetTerminalName(GetTerminal2Index(symbol));
    }
    else {
        return GetActionName(-symbol);
    }
}

CLASS_REGISTER(AnyObject, "1.0")
}
