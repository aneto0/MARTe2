/**
 * @file JsonParserData.cpp
 * @brief Header file for class AnyType
 * @date Apr 25, 2020
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



#include "JsonParser.h"
#include "ParserData.h"

namespace MARTe{


/**
 * name of the PaserClass ## Data
 */
namespace JsonParserData{

#define slk_size_t uint32


/*************************************************************************************************

slk -Cpp JsonGrammar.ll
PASTE HERE FROM SlkParse.cpp

**************************************************************************************************/

static uint32 Production[] = { 0u

, 3u, 9u, 10u, 22u, 3u, 10u, 11u, 18u, 6u, 11u, 23u, 1u, 2u, 12u, 24u, 5u, 11u, 25u, 1u, 2u, 16u, 2u, 12u, 13u, 2u, 12u, 14u, 2u, 12u, 15u, 3u, 13u, 26u, 17u, 5u, 14u, 3u, 13u, 19u, 4u, 5u, 15u, 3u, 14u,
        20u, 4u, 5u, 16u, 5u, 11u, 21u, 6u, 2u, 17u, 1u, 2u, 17u, 7u, 3u, 18u, 11u, 18u, 1u, 18u, 3u, 19u, 13u, 19u, 2u, 19u, 27u, 3u, 20u, 14u, 20u, 2u, 20u, 28u, 3u, 21u, 11u, 21u, 2u, 21u, 29u, 0u };

static uint32 Production_row[] = { 0u

, 1u, 5u, 9u, 16u, 22u, 25u, 28u, 31u, 35u, 41u, 47u, 53u, 56u, 59u, 63u, 65u, 69u, 72u, 76u, 79u, 83u, 0u};

static uint32 Parse[] = {

0u, 0u, 5u, 16u, 23u, 8u, 17u, 12u, 5u, 16u, 14u, 8u, 1u, 13u, 18u, 19u, 20u, 15u, 2u, 22u, 9u, 21u, 10u, 11u, 0u, 0u, 0u, 0u };

static uint32 Parse_row[] = { 0u

, 11u, 17u, 18u, 1u, 4u, 17u, 19u, 18u, 6u, 9u, 2u, 11u, 15u, 0u };

static uint32 Conflict[] = {

0u, 0u, 6u, 24u, 7u, 3u, 0u, 3u, 6u, 4u, 0u, 3u };

static uint32 Conflict_row[] = { 0u
, 1u, 1u, 4u, 0u };

 #define START_SYMBOL 9
 #define END_OF_SLK_INPUT_ 8
 #define START_STATE 0
 #define START_CONFLICT 22
 #define END_CONFLICT 25
 #define START_ACTION 22
 #define END_ACTION 30
 #define TOTAL_CONFLICTS 3
 #define PARSE_STACK_SIZE 512


/*************************************************************************************************

PASTE HERE FROM SlkString.cpp
replace char * with const char *

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

,"STRING"
,":"
,"["
,"]"
,"{"
,"}"
,"NUMBER"
,"END_OF_SLK_INPUT"
};

static const char * Action_name[] ={"0"

,"__End"
,"__GetNodeName"
,"__AddLeaf"
,"__CreateNode"
,"__AddScalar"
,"__EndVector"
,"__EndMatrix"
,"__BlockEnd"
};

static const char * Production_name[] ={"0"

,"expression --> cdbFile __End"
,"cdbFile --> expressions expressions_*"
,"expressions --> __GetNodeName STRING : variables __AddLeaf"
,"expressions --> __CreateNode STRING : block"
,"variables --> scalar"
,"variables --> vector"
,"variables --> matrix"
,"scalar --> __AddScalar token"
,"vector --> [ scalar scalar_* ]"
,"matrix --> [ vector vector_* ]"
,"block --> { expressions expressions_2_* }"
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

lexical parameters

*************************************************************************************************/

static inline uint32 findId(CCString symbol){
	uint32 ret = 0;
	while((ret < END_OF_SLK_INPUT_) && (symbol != Terminal_name[ret])){
		ret++;
	}

	return ret;
}



static const RegularExpression::PatternInformation rules[]={
	    {"//!*[^\n]\n"                                                                       ,"line comment"     , 0       				,true},
	    {"/\\*!*?\\a\\*/"                                                                    ,"multiline comment", 0       				,true},
	    {"+[ \r\n\t,;]"                                                                      ,"separator"        , 0	         		,true},
	    {":"                                                                                 ,": OPERATOR"       , findId(":")          ,false},
	    {"\\["                                                                               ,"open ["           , findId("[")          ,false},
	    {"\\]"                                                                               ,"close ]"          , findId("]")          ,false},
	    {"\\("                                                                               ,"open("            , findId("(")          ,false},
	    {"\\)"                                                                               ,"close)"           , findId(")")          ,false},
	    {"[\\w_]*[\\d\\w_]"                                                                  ,"identifier"	     , findId("STRING")     ,false},
		{"\"*[^\"]\""                                                                        ,"string"		     , findId("STRING")		,false},
	    {"($BODY(+\\d)$FRACTION(?(.*\\d))|$FRACTION(.*\\d))?([eE]!$EXP(?[+\\-]{1,5}\\d))"    ,"number"		     , findId("NUMBER")     ,false},
		{"\\"                                                                                ,"EOF"              , findId("END_OF_SLK_INPUT"),false},
		RegularExpression::emptyPattern
//		{emptyString												 ,emptyString	     , 0					,false}
};

#if 0

static const char * separators                = "\n\r\t, ";

/**
 * One line comment begin pattern.
 */
static const char * beginOneLineComment       = "" ;

/**
 * Multiple line comment begin pattern.
 */
static const char * beginMultipleLinesComment = "";

/**
 * Multiple line comment end pattern.
 */
static const char * endMultipleLinesComment   = "";

/**
 * Assignment operator
 */
static const char * terminals                 = ":{}[]";


#endif

/*************************************************************************************************

This macro is to be called in a C++ file to create the data structure used to initialise a Parser
use the name of the final class instead of SlkAction

*************************************************************************************************/

/**
 * exported for use in the parser
 */
extern const ParserData parserData(
#if 0
		separators,
		beginOneLineComment,
		beginMultipleLinesComment,
		endMultipleLinesComment,
		terminals,
#endif
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
		sizeof(Production_name)/sizeof(char*),
		ZeroTerminatedArray<const RegularExpression::PatternInformation>(&rules[0])
);



} //JsonParserData

#define SlkAction JsonParser

void JsonParser::MapMethods(){

	Action[0] = static_cast<ErrorManagement::ErrorType  (JsonParser::*)(const Token *,BufferedStreamI *)>(NULL);
/*************************************************************************************************

	PASTE HERE FROM SlkString.cpp
	use the name of the final class instead of SlkAction

*************************************************************************************************/

    Action [ 1 ] = &SlkAction::End;
    Action [ 2 ] = &SlkAction::GetNodeName;
    Action [ 3 ] = &SlkAction::AddLeaf;
    Action [ 4 ] = &SlkAction::CreateNode;
    Action [ 5 ] = &SlkAction::AddScalar;
    Action [ 6 ] = &SlkAction::EndVector;
    Action [ 7 ] = &SlkAction::EndMatrix;
    Action [ 8 ] = &SlkAction::BlockEnd;


/*************************************************************************************************/

}


} //MARTe

