/**
 * @file XMLParserData.cpp
 * @brief Header file for class AnyType
 * @date Apr 24, 2020
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#include "ParserData.h"
#include "XMLParser.h"

namespace MARTe{


/**
 * name of the PaserClass ## Data
 */
namespace XMLParserData{

/*************************************************************************************************

lexical parameters

*************************************************************************************************/


static const char * separators                = "\n\r\t, ";

/**
 * One line comment begin pattern.
 */
static const char * beginOneLineComment       = "" ;

/**
 * Multiple line comment begin pattern.
 */
static const char * beginMultipleLinesComment = "<!--";

/**
 * Multiple line comment end pattern.
 */
static const char * endMultipleLinesComment   = "-->";

/**
 * Assignment operator
 */
static const char * terminals                 = "<>{}/()";

/*************************************************************************************************/


#define slk_size_t uint32

/*************************************************************************************************

slk -Cpp XMLGrammar.ll
PASTE HERE FROM SlkParse.cpp

**************************************************************************************************/

static slk_size_t Production[] = { 0u

, 3u, 11u, 12u, 24u
, 3u, 12u, 13u, 20u
, 11u, 13u, 1u, 25u, 2u, 3u, 14u, 1u, 4u, 2u, 3u, 26u
, 15u, 13u, 1u, 25u, 2u, 3u, 5u, 27u, 2u, 6u, 14u, 1u, 4u, 2u, 3u, 26u
, 10u, 13u, 1u, 28u, 2u, 3u, 18u, 1u, 4u, 2u, 3u
, 2u, 14u, 15u
, 2u, 14u, 16u
, 2u, 14u, 17u
, 3u, 15u, 29u, 19u
, 5u, 16u, 7u, 15u, 21u, 8u
, 5u, 17u, 7u, 16u, 22u, 8u
, 3u, 18u, 13u, 23u
, 2u, 19u, 2u
, 2u, 19u, 9u
, 3u, 20u, 13u, 20u
, 1u, 20u
, 3u, 21u, 15u, 21u
, 2u, 21u, 30u
, 3u, 22u, 16u, 22u
, 2u, 22u, 31u
, 3u, 23u, 13u, 23u
, 2u, 23u, 32u
, 0u };

static slk_size_t Production_row[] = { 0u
, 1u, 5u, 9u, 21u, 37u, 48u, 51u, 54u, 57u, 61u, 67u, 73u, 77u, 80u, 83u, 87u, 89u, 93u, 96u, 100u, 103u, 107u, 0u };

static slk_size_t Parse[] = {
0u, 0u, 1u, 6u, 2u, 17u, 9u, 13u, 24u, 15u, 6u, 18u, 17u, 9u, 14u, 19u, 20u, 23u, 16u, 10u, 11u, 12u, 25u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u };

static slk_size_t Parse_row[] = { 0u
, 1u, 3u, 16u, 1u, 4u, 12u, 13u, 20u, 5u, 8u, 3u, 8u, 21u, 0u };

static slk_size_t Conflict[] = {
0u, 0u, 0u, 26u, 7u, 21u, 27u, 22u, 0u, 8u, 0u, 7u, 5u, 3u, 0u, 0u, 4u, 0u, 3u, 0u, 3u };

static slk_size_t Conflict_row[] = { 0u
, 1u, 2u, 3u, 3u, 11u, 0u };


 #define START_SYMBOL 11
 #define END_OF_SLK_INPUT_ 10
 #define START_STATE 0
 #define START_CONFLICT 23
 #define END_CONFLICT 28
 #define START_ACTION 24
 #define END_ACTION 33
 #define TOTAL_CONFLICTS 5
 #define PARSE_STACK_SIZE 512


/*************************************************************************************************

PASTE HERE FROM SlkString.cpp

*************************************************************************************************/

static const char * Nonterminal_name[] ={"0"

,"expression"
,"cdbFile"
,"expressions"
,"variables"
,"scalar"
,"vector"
,"matrix"
,"block"
,"token"
,"expressions_*"
,"scalar_*"
,"vector_*"
,"expressions_2_*"
};

static const char * Terminal_name[] ={"0"

,"<"
,"STRING"
,">"
,"/"
,"("
,")"
,"{"
,"}"
,"NUMBER"
,"END_OF_SLK_INPUT"
};

static const char * Action_name[] ={"0"

,"__End"
,"__GetNodeName"
,"__AddLeaf"
,"__GetTypeCast"
,"__CreateNode"
,"__AddScalar"
,"__EndVector"
,"__EndMatrix"
,"__BlockEnd"
};

static const char * Production_name[] ={"0"

,"expression --> cdbFile __End"
,"cdbFile --> expressions expressions_*"
,"expressions --> < __GetNodeName STRING > variables < / STRING > __AddLeaf"
,"expressions --> < __GetNodeName STRING > ( __GetTypeCast STRING ) variables < / STRING > __AddLeaf"
,"expressions --> < __CreateNode STRING > block < / STRING >"
,"variables --> scalar"
,"variables --> vector"
,"variables --> matrix"
,"scalar --> __AddScalar token"
,"vector --> { scalar scalar_* }"
,"matrix --> { vector vector_* }"
,"block --> expressions expressions_2_*"
,"token --> STRING"
,"token --> NUMBER"
,"expressions_* --> expressions expressions_*"
,"expressions_* -->"
,"scalar_* --> scalar scalar_*"
,"scalar_* --> __EndVector"
,"vector_* --> vector vector_*"
,"vector_* --> __EndMatrix"
,"expressions_2_* --> expressions expressions_2_*"
,"expressions_2_* --> __BlockEnd"
};

/*************************************************************************************************/




/*************************************************************************************************

This macro is to be called in a C++ file to create the data structure used to initialise a Parser
use the name of the final class instead of SlkAction

*************************************************************************************************/

/**
 * exported for use in the parser
 */
extern const ParserData parserData(
		separators,
		beginOneLineComment,
		beginMultipleLinesComment,
		endMultipleLinesComment,
		terminals,
		START_SYMBOL,
		END_OF_SLK_INPUT_,
		START_STATE,
		START_CONFLICT,
		END_CONFLICT,
		START_ACTION,
		END_ACTION,
		TOTAL_CONFLICTS,
		PARSE_STACK_SIZE,
		Production,
		Production_row,
		Parse,
		Parse_row,
		Conflict,
		Conflict_row,
		Nonterminal_name,
		sizeof(Nonterminal_name)/sizeof(char*),
		Terminal_name,
		sizeof(Terminal_name)/sizeof(char*),
		Action_name,
		sizeof(Action_name)/sizeof(char*),
		Production_name,
		sizeof(Production_name)/sizeof(char*)
);


} //XMLParserData

#define SlkAction XMLParser

void XMLParser::MapMethods(){

	Action[0] = static_cast<ErrorManagement::ErrorType  (XMLParser::*)(const Token *,BufferedStreamI *)>(NULL);
/*************************************************************************************************

	PASTE HERE FROM SlkTables.cpp
	use the name of the final class instead of SlkAction

*************************************************************************************************/

	Action [ 1 ] = &SlkAction::End;
	Action [ 2 ] = &SlkAction::GetNodeName;
	Action [ 3 ] = &SlkAction::AddLeaf;
	Action [ 4 ] = &SlkAction::GetTypeCast;
	Action [ 5 ] = &SlkAction::CreateNode;
	Action [ 6 ] = &SlkAction::AddScalar;
	Action [ 7 ] = &SlkAction::EndVector;
	Action [ 8 ] = &SlkAction::EndMatrix;
	Action [ 9 ] = &SlkAction::BlockEnd;


/*************************************************************************************************/

}


} //MARTe

