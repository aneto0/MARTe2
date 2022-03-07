/**
 * @file StructuredDataParser.cpp
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

#include "StructuredDataParser.h"
#include "AdvancedErrorManagement.h"


/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace StandardParserData{
	extern const ParserData parserData;
	extern void MapMethods(StructuredDataParser::ParserMethod *Action);
}

namespace XMLParserData{
	extern const ParserData parserData;
	extern void MapMethods(StructuredDataParser::ParserMethod *Action);
}

namespace JsonParserData{
	extern const ParserData parserData;
	extern void MapMethods(StructuredDataParser::ParserMethod *Action);
}

/**
 *
 */
struct ParseStatus{

    /**
     * The type name.
     */
    TypeDescriptor 			td;

    /**
     * The StructuredData node or leaf name.
     */
    DynamicCString 			nodeName;

    /**
     * Status of a variable element parsing
     */
    enum {
    	parseElFinished,
		parseElStarted
    }						parseElStatus;

    void Init();

} parseStatus;

StructuredDataParser::StructuredDataParser(StructuredDataI & databaseIn,StructuredDataParserFormats syntax):
//StructuredDataParser(StructuredDataI &databaseIn,const ParserData & constantsIn,StructuredDataParserMethod *ActionMapIn):
		ParserI((syntax==XMLParser)?XMLParserData::parserData : (syntax==JsonParser)?JsonParserData::parserData : StandardParserData::parserData),
		memory(1024*1024)
		{
	parseStatus.Init();
    database = &databaseIn;

    if (syntax == XMLParser){
    	XMLParserData::MapMethods(&ActionMap[0]);
    } else
    if (syntax == JsonParser){
    	JsonParserData::MapMethods(&ActionMap[0]);
    } else {
    	StandardParserData::MapMethods(&ActionMap[0]);
    }

}

StructuredDataParser::~StructuredDataParser() {
    database=static_cast<StructuredDataI*>(NULL);
}

ErrorManagement::ErrorType StructuredDataParser::Execute(const uint32 number,const Token *currentToken,DebugStream &debugStream ) {
    return (this->*ActionMap[number])(currentToken,debugStream);

}


ErrorManagement::ErrorType StructuredDataParser::GetTypeCast(const Token *currentToken,DebugStream &debugStream) {
	ErrorManagement::ErrorType ret;

	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(debugStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){
		parseStatus.td = TypeDescriptor(currentToken->GetData());
	}
	return ret;
}

ErrorManagement::ErrorType StructuredDataParser::BlockEnd(const Token *currentToken,DebugStream &debugStream) {
	ErrorManagement::ErrorType ret;

	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(debugStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){
		ret = database->MoveToAncestor(1u);
        PARSER_ERROR_REPORT(debugStream,ret,"Failed StructuredDataI::MoveToAncestor(1)! [%d]", currentToken->GetLineNumber());
    }
	return ret;
}

ErrorManagement::ErrorType  StructuredDataParser::CreateNode(const Token *currentToken,DebugStream &debugStream) {
	ErrorManagement::ErrorType ret;

	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(debugStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){
		ret.fatalError =  database->CreateRelative(currentToken->GetData());
    	PARSER_ERROR_REPORT(debugStream,ret,"Failed StructuredDataI::CreateRelative(*)! [%d]", currentToken->GetLineNumber());
    }
	return ret;
}

ErrorManagement::ErrorType  StructuredDataParser::AddLeaf(const Token *currentToken,DebugStream &debugStream) {
	ErrorManagement::ErrorType ret;
	Reference ref;

	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(debugStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){
		ret  = memory.End();
		PARSER_ERROR_REPORT(debugStream,ret,"Failed ending construction of object [%d]", currentToken->GetLineNumber());
	}

	if (ret){
		ret = memory.GetReference(ref);
		PARSER_ERROR_REPORT(debugStream,ret,"obtaining a reference from Memory [%d]", currentToken->GetLineNumber());
	}

	if (ret){
		ret.fatalError = !ref.IsValid();
		PARSER_ERROR_REPORT(debugStream,ret,"invalid reference from Memory [%d]", currentToken->GetLineNumber());
	}

	if (ret){
		ref->SetName(parseStatus.nodeName);
/*
		{  // TODO remove
			DynamicCString dcs;
			AnyType at;
			ref.ToAnyType(at);
			at.CopyTo(dcs);
			printf("dcs=%s\n",dcs.GetList());
		}
*/
		// no errors if ref is valid. which must be since we got here
        ret = database->Write(ref);
		PARSER_ERROR_REPORT(debugStream,ret,"writing %s [%d]", parseStatus.nodeName, currentToken->GetLineNumber());
	}

	parseStatus.Init();
	return ret;
}

ErrorManagement::ErrorType  StructuredDataParser::GetNodeName(const Token *currentToken,DebugStream &debugStream) {
	ErrorManagement::ErrorType ret;

	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(debugStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){
		parseStatus.nodeName = currentToken->GetData().GetList();
	}
	return ret;
}

ErrorManagement::ErrorType  StructuredDataParser::AddScalar(const Token *currentToken,DebugStream &debugStream) {
	ErrorManagement::ErrorType ret;

	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(debugStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){

		// restart parsing
		if (parseStatus.parseElStatus == ParseStatus::parseElFinished){
			memory.Start(parseStatus.td);
		}

		ret = memory.AddElement(currentToken->GetData());
		PARSER_ERROR_REPORT(debugStream,ret," Failed adding element[%d]",currentToken->GetLineNumber());
	}
	return ret;
}

ErrorManagement::ErrorType  StructuredDataParser::EndVector(const Token *currentToken,DebugStream &debugStream) {
	ErrorManagement::ErrorType ret;
	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(debugStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){
		ret = memory.EndVector();
		PARSER_ERROR_REPORT(debugStream,ret,"Failed ending vector [%d]", currentToken->GetLineNumber());
	}
	return ret;
}

ErrorManagement::ErrorType  StructuredDataParser::EndMatrix(const Token *currentToken,DebugStream &debugStream) {
	ErrorManagement::ErrorType ret;
	return ret;
}

ErrorManagement::ErrorType  StructuredDataParser::End(const Token *currentToken,DebugStream &debugStream) {

	ErrorManagement::ErrorType ret;
	ret.parametersError = (currentToken == NULL_PTR(Token *));
	PARSER_ERROR_REPORT(debugStream,ret,"currentToken = NULL [%d]", currentToken->GetLineNumber());

	if (ret){
		ret = database->MoveToRoot();
		PARSER_ERROR_REPORT(debugStream,ret,"Failed StructuredDataI::MoveToRoot() at the end! [%d]", currentToken->GetLineNumber());
    }
	return ret;
}

void ParseStatus::Init(){
	nodeName().Truncate(0);
	parseElStatus = ParseStatus::parseElFinished;
	td = ConstCharString;
}


ErrorManagement::ErrorType StructuredDataParser::Parse(
		StreamI &			stream,
        BufferedStreamI * 	const err,
		uint32 				debugLevel) {

	parseStatus.Init();
	return ParserI::Parse(stream,err,debugLevel);
}


}
