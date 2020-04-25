/**
 * @file StructuredDataParserI.cpp
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

#include "StructuredDataParserI.h"
#include "AdvancedErrorManagement.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

static void PrintErrorOnStream(CCString const format,
                               const uint32 lineNumber,
                               BufferedStreamI * const err) {
    if (err != NULL) {
        if (!err->Printf(format, lineNumber)) {
            REPORT_ERROR(ErrorManagement::FatalError, "PrintErrorOnStream: Failed Printf() on parseError stream");
        }

//TODO FIX THIS
//        REPORT_ERROR(ErrorManagement::FatalError, format, lineNumber);
    }
}


static CCString GetCurrentTokenData(Token * const token) {

    return (token != NULL)?(token->GetData()):(emptyString);
}

static uint32 GetCurrentTokenLineNumber(const Token * const token) {
    return (token != NULL)?(token->GetLineNumber()):0u;
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

StructuredDataParserI::StructuredDataParserI(StreamI &stream,StructuredDataI &databaseIn,BufferedStreamI * const err,const GrammarInfo &grammarIn,const ParserData & constantsIn):
		ParserI(stream,err,grammarIn,constantsIn),
		memory(1024*1024) {

//	parseStatus.Init();

    database = &databaseIn;
}

StructuredDataParserI::~StructuredDataParserI() {
    database=static_cast<StructuredDataI*>(NULL);
}



void StructuredDataParserI::GetTypeCast() {
	if ((currentToken != NULL_PTR(Token *)) && (ParserI::ok)){
		parseStatus.td = TypeDescriptor(currentToken->GetData());
	}
}

void StructuredDataParserI::BlockEnd() {
    if (!database->MoveToAncestor(1u)) {
        PrintErrorOnStream("\nFailed StructuredDataI::MoveToAncestor(1)! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
		ParserI::ok.fatalError = true;
    }
}

void StructuredDataParserI::CreateNode() {
    if (!database->CreateRelative(GetCurrentTokenData(currentToken))) {
        PrintErrorOnStream("\nFailed StructuredDataI::CreateRelative(*)! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
		ParserI::ok.fatalError = true;
    }
}

void StructuredDataParserI::AddLeaf() {
	Reference ref;

	if (ParserI::ok){
		ParserI::ok = memory.End();
		if (!ParserI::ok){
	        PrintErrorOnStream("\n Failed ending construction of object[%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
		}
	}

	if (ParserI::ok){
		ParserI::ok = memory.GetReference(ref);
		if (!ref.IsValid()){
			ParserI::ok.fatalError = true;
		}
		if (!ParserI::ok){
	        PrintErrorOnStream("\n Failed creating object[%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
		}
	}

	if (ParserI::ok){
		ref->SetName(parseStatus.nodeName);

		// no errors if ref is valid. which must be since we got here
        database->Write(ref);
	}

	parseStatus.Init();

}

void StructuredDataParserI::GetNodeName() {
	if ((currentToken != NULL_PTR(Token *)) && (ParserI::ok)){
		parseStatus.nodeName = currentToken->GetData().GetList();
	}
}

void StructuredDataParserI::AddScalar() {
	if ((currentToken != NULL_PTR(Token *)) && (ParserI::ok)){

		// restart parsing
		if (parseStatus.parseElStatus == ParseStatus::parseElFinished){
			memory.Start(parseStatus.td);
		}

		ParserI::ok = memory.AddElement(currentToken->GetData());
		if (!ParserI::ok){
	        PrintErrorOnStream("\n Failed adding element[%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
		}
	}
}

void StructuredDataParserI::EndVector() {
	if (ParserI::ok){
		ParserI::ok = memory.EndVector();
		if (!ParserI::ok){
	        PrintErrorOnStream("\n Failed ending vector [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
		}
	}
}

void StructuredDataParserI::EndMatrix() {
}

void StructuredDataParserI::End() {
    if (!database->MoveToRoot()) {
        PrintErrorOnStream("\nFailed StructuredDataI::MoveToRoot() at the end! [%d]", GetCurrentTokenLineNumber(currentToken), errorStream);
        ParserI::ok.fatalError = true;
    }
}

void StructuredDataParserI::ParseStatus::Init(){
	nodeName().Truncate(0);
	parseElStatus = ParseStatus::parseElFinished;
	td = ConstCharString;
}


ErrorManagement::ErrorType StructuredDataParserI::Parse() {
	ParserI::ok = ErrorManagement::NoError;
	parseStatus.Init();

	return ParserI::Parse();
}


}
