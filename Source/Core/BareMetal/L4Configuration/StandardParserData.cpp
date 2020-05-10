/**
 * @file StandardParserData.cpp
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


#include "ParserData.h"
#include "StandardParser.h"

namespace MARTe{

/**
 * name of the PaserClass ## Data
 */
namespace StandardParserData{



/*************************************************************************************************/


#define slk_size_t uint32


/*************************************************************************************************

slk -Cpp StandardGrammar.ll
PASTE HERE FROM SlkParse.cpp

**************************************************************************************************/

static uint32 Production[] = { 0u

, 3u, 9u, 10u, 22u, 3u, 10u, 11u, 18u, 6u, 11u, 23u, 1u, 2u, 12u, 24u, 10u, 11u, 23u, 1u, 2u, 3u, 25u, 1u, 4u, 12u, 24u, 5u, 11u, 26u, 1u, 2u, 16u, 2u, 12u, 13u, 2u, 12u, 14u, 2u, 12u, 15u, 3u, 13u, 27u,
        17u, 5u, 14u, 5u, 13u, 19u, 6u, 5u, 15u, 5u, 14u, 20u, 6u, 5u, 16u, 5u, 11u, 21u, 6u, 2u, 17u, 1u, 2u, 17u, 7u, 3u, 18u, 11u, 18u, 1u, 18u, 3u, 19u, 13u, 19u, 2u, 19u, 28u, 3u, 20u, 14u, 20u, 2u,
        20u, 29u, 3u, 21u, 11u, 21u, 2u, 21u, 30u
        , 0u };

static uint32 Production_row[] = { 0u

, 1u, 5u, 9u, 16u, 27u, 33u, 36u, 39u, 42u, 46u, 52u, 58u, 64u, 67u, 70u, 74u, 76u, 80u, 83u, 87u, 90u, 94u, 0u };

static uint32 Parse[] = {

0u, 0u, 6u, 1u, 17u, 9u, 24u, 13u, 6u, 18u, 17u, 9u, 15u, 14u, 19u, 20u, 21u, 2u, 23u, 16u, 10u, 22u, 11u, 12u, 0u, 0u, 0u };

static uint32 Parse_row[] = { 0u

, 2u, 16u, 17u, 1u, 4u, 15u, 17u, 18u, 6u, 11u, 3u, 9u, 15u, 0u };

static uint32 Conflict[] = {

0u, 0u, 7u, 25u, 0u, 3u, 8u, 4u, 7u, 26u, 27u, 3u, 3u, 5u, 3u, 0u, 3u, 3u, 3u };

static uint32 Conflict_row[] = { 0u

, 1u, 1u, 4u, 9u, 11u, 0u };


//static const uint32 Constants[] = { 9u, 8u, 0u, 23u, 28u, 22u, 31u, 5u };
#define START_SYMBOL 9
#define END_OF_SLK_INPUT_ 8
#define START_STATE 0
#define START_CONFLICT 23
#define END_CONFLICT 28
#define START_ACTION 22
#define END_ACTION 31
#define TOTAL_CONFLICTS 5
#define PARSE_STACK_SIZE 512


/*************************************************************************************************

PASTE HERE FROM SlkString.cpp
Change all char * to const char *

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
,"="
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
,"expressions --> __GetNodeName STRING = variables __AddLeaf"
,"expressions --> __GetNodeName STRING = ( __GetTypeCast STRING ) variables __AddLeaf"
,"expressions --> __CreateNode STRING = block"
,"variables --> scalar"
,"variables --> vector"
,"variables --> matrix"
,"scalar --> __AddScalar token"
,"vector --> { scalar scalar_* }"
,"matrix --> { vector vector_* }"
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


/*
 */
static const RegularExpression::PatternInformation rules[]={
	    {"//!*[^\n]\n"                                                                       ,"line comment"     , 0       				,true},
	    {"/\\*!*?\\a\\*/"                                                                    ,"multiline comment", 0       				,true},
	    {"+[ \r\n\t,;]"                                                                      ,"separator"        , 0	         		,true},
	    {"\\{"                                                                               ,"open {"           , findId("{")          ,false},
	    {"\\}"                                                                               ,"close }"          , findId("}")          ,false},
	    {"="                                                                                 ,"EQ  operator"     , findId("=")          ,false},
	    {"\\("                                                                               ,"open("            , findId("(")          ,false},
	    {"\\)"                                                                               ,"close)"           , findId(")")          ,false},
	    {"[\\w_]*[\\d\\w_]"                                                                  ,"identifier"	     , findId("STRING")     ,false},
		{"\"*[^\"]\""                                                                        ,"string"		     , findId("STRING")		,false},
	    {"($BODY(+\\d)$FRACTION(?(.*\\d))|$FRACTION(.*\\d))?([eE]!$EXP(?[+\\-]{1,5}\\d))"    ,"number"		     , findId("NUMBER")     ,false},
		{"\\"                                                                                ,"EOF"              , findId("END_OF_SLK_INPUT"),false},
		RegularExpression::emptyPattern
//		{emptyString												 ,emptyString	     , 0					,false}
};


static const char * separators                = "\n\r\t, ";

/**
 * One line comment begin pattern.
 */
static const char * beginOneLineComment       = "//" ;

/**
 * Multiple line comment begin pattern.
 */
static const char * beginMultipleLinesComment = "/*";

/**
 * Multiple line comment end pattern.
 */
static const char * endMultipleLinesComment   = "*/";

/**
 * Assignment operator
 */
static const char * terminals 				  = "{}()";



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
		sizeof(Production_name)/sizeof(char*),
		ZeroTerminatedArray<const RegularExpression::PatternInformation>(&rules[0])
);


} //StandardParserData

#define SlkAction StandardParser

void StandardParser::MapMethods(){

	Action[0] = static_cast<ErrorManagement::ErrorType  (StandardParser::*)(const Token *,BufferedStreamI *)>(0);
/*************************************************************************************************

	PASTE HERE FROM SlkString.cpp
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

