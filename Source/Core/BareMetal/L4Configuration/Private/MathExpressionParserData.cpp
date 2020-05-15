/**
 * @file MathExpressionParseData.cpp
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

#include "MathExpressionParser.h"
#include "ParserData.h"
#include "RegularExpression.h"

namespace MARTe{



/*************************************************************************************************/

/**
 * name of the PaserClass ## Data
 */
namespace MathExpressionParserData{

#define slk_size_t uint32


/*************************************************************************************************

slk -Cpp MathExpressionGrammar.ll
PASTE HERE FROM SlkParse.cpp

**************************************************************************************************/

static slk_size_t Production[] = {0

,3,25,26,41 ,7,26,47,1,2,28,27,48 ,2,27,3 ,2,27,4
,3,28,30,42 ,2,29,5 ,2,29,6 ,2,29,7 ,2,29,8
,2,29,9 ,3,30,32,43 ,2,31,10 ,2,31,11 ,2,31,12
,2,31,13 ,2,31,14 ,2,31,15 ,3,32,34,44 ,2,33,16
,2,33,17 ,3,34,36,45 ,2,35,18 ,2,35,19 ,2,36,38
,5,36,49,33,38,51 ,5,36,49,37,38,50 ,2,37,20 ,2,38,39
,7,38,21,52,1,22,39,53 ,7,38,21,52,1,22,54,23
,3,38,55,23 ,2,39,40 ,8,39,49,1,21,28,46,22,50
,4,40,21,28,22 ,3,40,55,1 ,3,41,26,41 ,1,41 ,6,42,49,29,30,50,42
,1,42 ,6,43,49,31,32,50,43 ,1,43 ,6,44,49,33,34,50,44
,1,44 ,6,45,49,35,36,50,45 ,1,45 ,4,46,4,28,46
,1,46
,0};

static slk_size_t Production_row[] = {0

,1,5,13,16,19,23,26,29,32,35,38,42,45,48,51,54
,57,60,64,67,70,74,77,80,83,89,95,98,101,109,117,121
,124,133,138,142,146,148,155,157,164,166,173,175,182,184,189
,0};

static slk_size_t Parse[] = {

0,0,1,2,45,45,45,45,45,45,45,45,45,45,45,45,45,45,45
,44,44,3,4,45,43,43,43,43,43,43,43,43,43,43,43,43,43,42,42
,19,20,22,23,43,41,41,41,41,41,41,41,40,40,40,40,40,40,5,6
,7,8,9,10,41,39,39,38,38,38,38,38,11,5,5,27,0,5,5,0
,5,36,0,0,39,0,18,11,11,49,28,11,11,35,11,12,13,14,15,16
,17,18,18,21,37,18,18,0,18,32,48,24,31,34,46,0,0,0,21,21
,0,0,21,21,0,21,25,25,0,0,26,24,47,24,0
};

static slk_size_t Parse_row[] = {0

,1,2,18,56,53,70,84,84,23,101,23,109,54,88,87,91
,79,61,41,21,1,109
,0};

static slk_size_t Conflict[] = {

0,0,50,0,0,0,0,0,0,0,0,0,0,0,0,0,0,28,28
,0,0,28,28,0,28,32,32,32,32,32,32,32,32,32,32,32,32,32,32
,32,32,32,0,33,32,28,28,28,28,28,28,28,28,28,28,28,28,28,28
,28,0,28,51,29,0,28,28,28,28,28,28,28,28,28,28,28,28,28,28
,28,28,28,0,29,28,30
};

static slk_size_t Conflict_row[] = {0

,1,22,40,62
,0};

#define GET_CONDITIONAL_PRODUCTION( symbol )  0
#define get_predicted_entry( a,b,c,d,e )  0

#define START_SYMBOL 25
#define END_OF_SLK_INPUT_ 24
#define START_STATE 0
#define START_CONFLICT 48
#define END_CONFLICT 52
#define START_ACTION 47
#define END_ACTION 56
#define TOTAL_CONFLICTS 4
#define PARSE_STACK_SIZE 512


/*************************************************************************************************

PASTE HERE FROM SlkString.cpp

*************************************************************************************************/

static const char * Nonterminal_name[] ={"0"

,"system"
,"assignment"
,"delimitator"
,"logic_expr"
,"logic_op"
,"comparison_expr"
,"comp_op"
,"addition_expr"
,"add_op"
,"multiplication_expr"
,"mul_op"
,"unary_expr"
,"unary_op"
,"typecast_expr"
,"function_expr"
,"primary_expr"
,"assignment_*"
,"logic_op_comparison_expr_*"
,"comp_op_addition_expr_*"
,"add_op_multiplication_expr_*"
,"mul_op_unary_expr_*"
,",_logic_expr_*"
};

static const char * Terminal_name[] ={"0"

,"STRING"
,"="
,";"
,","
,"&"
,"|"
,"||"
,"&&"
,"^"
,">"
,"<"
,">="
,"<="
,"=="
,"!="
,"+"
,"-"
,"*"
,"/"
,"!"
,"("
,")"
,"NUMBER"
,"END_OF_SLK_INPUT"
};

static const char * Action_name[] ={"0"

,"__StoreAssignment"
,"__PopAssignment"
,"__PushOperator"
,"__PopOperator"
,"__PopOperatorAlternate"
,"__PushTypecast"
,"__PopTypecast"
,"__AddOperandTypecast"
,"__AddOperand"
};

static const char * Production_name[] ={"0"

,"system --> assignment assignment_*"
,"assignment --> __StoreAssignment STRING = logic_expr delimitator __PopAssignment"
,"delimitator --> ;"
,"delimitator --> ,"
,"logic_expr --> comparison_expr logic_op_comparison_expr_*"
,"logic_op --> &"
,"logic_op --> |"
,"logic_op --> ||"
,"logic_op --> &&"
,"logic_op --> ^"
,"comparison_expr --> addition_expr comp_op_addition_expr_*"
,"comp_op --> >"
,"comp_op --> <"
,"comp_op --> >="
,"comp_op --> <="
,"comp_op --> =="
,"comp_op --> !="
,"addition_expr --> multiplication_expr add_op_multiplication_expr_*"
,"add_op --> +"
,"add_op --> -"
,"multiplication_expr --> unary_expr mul_op_unary_expr_*"
,"mul_op --> *"
,"mul_op --> /"
,"unary_expr --> typecast_expr"
,"unary_expr --> __PushOperator add_op typecast_expr __PopOperatorAlternate"
,"unary_expr --> __PushOperator unary_op typecast_expr __PopOperator"
,"unary_op --> !"
,"typecast_expr --> function_expr"
,"typecast_expr --> ( __PushTypecast STRING ) function_expr __PopTypecast"
,"typecast_expr --> ( __PushTypecast STRING ) __AddOperandTypecast NUMBER"
,"typecast_expr --> __AddOperand NUMBER"
,"function_expr --> primary_expr"
,"function_expr --> __PushOperator STRING ( logic_expr ,_logic_expr_* ) __PopOperator"
,"primary_expr --> ( logic_expr )"
,"primary_expr --> __AddOperand STRING"
,"assignment_* --> assignment assignment_*"
,"assignment_* -->"
,"logic_op_comparison_expr_* --> __PushOperator logic_op comparison_expr __PopOperator logic_op_comparison_expr_*"
,"logic_op_comparison_expr_* -->"
,"comp_op_addition_expr_* --> __PushOperator comp_op addition_expr __PopOperator comp_op_addition_expr_*"
,"comp_op_addition_expr_* -->"
,"add_op_multiplication_expr_* --> __PushOperator add_op multiplication_expr __PopOperator add_op_multiplication_expr_*"
,"add_op_multiplication_expr_* -->"
,"mul_op_unary_expr_* --> __PushOperator mul_op unary_expr __PopOperator mul_op_unary_expr_*"
,"mul_op_unary_expr_* -->"
,",_logic_expr_* --> , logic_expr ,_logic_expr_*"
,",_logic_expr_* -->"
};


/*************************************************************************************************/

/*************************************************************************************************

lexical parameters
ID field filled from the SLK constants

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
	    {"+[ \r\n\t]"                                                                        ,"separator"        , 0	         		,true},
	    {"&&"                                                                                ,"AND operator"     , findId("&&")         ,false},
	    {"\\|\\|"                                                                            ,"OR operator"      , findId("||")         ,false},
	    {"\\|"                                                                               ,"OR operator"      , findId("|")          ,false},
	    {"&"                                                                                 ,"AND operator"     , findId("&")          ,false},
	    {"\\^"                                                                               ,"XOR operator"     , findId("^")          ,false},
	    {"<="                                                                                ,"LTE operator"     , findId("<=")         ,false},
	    {">="                                                                                ,"GTE operator"     , findId(">=")         ,false},
	    {"\\+"                                                                               ,"SUM operator"     , findId("+")          ,false},
	    {"\\-"                                                                               ,"SUB operator"     , findId("-")          ,false},
	    {"\\*"                                                                               ,"MUL operator"     , findId("*")          ,false},
	    {"/"                                                                                 ,"DIV operator"     , findId("/")          ,false},
	    {"\\!"                                                                               ,"NOT operator"     , findId("!")          ,false},
	    {"=="                                                                                ,"EQ  operator"     , findId("==")         ,false},
	    {"!="                                                                                ,"NEQ  operator"    , findId("==")         ,false},
	    {">"                                                                                 ,"GT  operator"     , findId(">")          ,false},
	    {"<"                                                                                 ,"LT  operator"     , findId("<")          ,false},
	    {","                                                                                 ,"COMMA"            , findId(",")          ,false},
	    {";"                                                                                 ,"SEMICOLON"        , findId(";")          ,false},
	    {"\\("                                                                               ,"open("            , findId("(")          ,false},
	    {"\\)"                                                                               ,"close)"           , findId(")")          ,false},
	    {"[\\w_]*[\\d\\w_]"                                                                  ,"STRING"   	     , findId("STRING")     ,false},
		{"\"*[^\"]\""                                                                        ,"STRING"		     , findId("STRING")		,false},
	    {"($BODY(+\\d)$FRACTION(?(.*\\d))|$FRACTION(.*\\d))?([eE]!$EXP(?[+\\-]{1,5}\\d))"    ,"NUMBER"		     , findId("NUMBER")     ,false},
		{"\\"                                                                                ,"EOF"              , findId("END_OF_SLK_INPUT"),false},
		RegularExpression::emptyPattern
//		{emptyString												 ,emptyString	     , 0					,false}
};
#if 0
static const char * separators                = "\n\r\t ";

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
static const char * terminals                 = "=+/-*()><^!;,";

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

} //MathExpressionParserData

#define SlkAction MathExpressionParser

void MathExpressionParser::MapMethods(){

	Action[0] = static_cast<ErrorManagement::ErrorType  (MathExpressionParser::*)(const Token *,DebugStream &)>(NULL);
/*************************************************************************************************

	PASTE HERE FROM SlkString.cpp
	use the name of the final class instead of SlkAction

*************************************************************************************************/

    Action [ 1 ] = &SlkAction::StoreAssignment;
    Action [ 2 ] = &SlkAction::PopAssignment;
    Action [ 3 ] = &SlkAction::PushOperator;
    Action [ 4 ] = &SlkAction::PopOperator;
    Action [ 5 ] = &SlkAction::PopOperatorAlternate;
    Action [ 6 ] = &SlkAction::PushTypecast;
    Action [ 7 ] = &SlkAction::PopTypecast;
    Action [ 8 ] = &SlkAction::AddOperandTypecast;
    Action [ 9 ] = &SlkAction::AddOperand;

/*************************************************************************************************/

}


} //MARTe

