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

,3,24,25,40 ,7,25,46,1,2,27,26,47 ,2,26,3 ,2,26,4
,3,27,29,41 ,2,28,5 ,2,28,6 ,2,28,7 ,2,28,8
,2,28,9 ,3,29,31,42 ,2,30,10 ,2,30,11 ,2,30,12
,2,30,13 ,2,30,14 ,3,31,33,43 ,2,32,15 ,2,32,16
,3,33,35,44 ,2,34,17 ,2,34,18 ,2,35,37 ,5,35,48,32,37,50
,5,35,48,36,37,49 ,2,36,19 ,2,37,38 ,7,37,20,51,1,21,38,52
,7,37,20,51,1,21,53,22 ,3,37,54,22 ,2,38,39 ,8,38,48,1,20,27,45,21,49
,4,39,20,27,21 ,3,39,54,1 ,3,40,25,40 ,1,40 ,6,41,48,28,29,49,41
,1,41 ,6,42,48,30,31,49,42 ,1,42 ,6,43,48,32,33,49,43
,1,43 ,6,44,48,34,35,49,44 ,1,44 ,4,45,4,27,45
,1,45
,0};

static slk_size_t Production_row[] = {0

,1,5,13,16,19,23,26,29,32,35,38,42,45,48,51,54
,57,61,64,67,71,74,77,80,86,92,95,98,106,114,118,121
,130,135,139,143,145,152,154,161,163,170,172,179,181,186
,0};

static slk_size_t Parse[] = {

0,0,1,2,44,44,44,44,44,44,44,44,44,44,44,44,44,44,43
,43,3,4,44,42,42,42,42,42,42,42,42,42,42,42,42,41,41,18,19
,21,22,42,40,40,40,40,40,40,40,39,39,39,39,39,5,6,7,8,9
,10,40,38,38,37,37,37,37,37,5,5,11,48,5,5,26,5,34,0,17
,38,0,35,0,0,11,11,20,0,11,11,31,11,17,17,23,33,17,17,0
,17,20,20,27,36,20,20,0,20,24,24,45,0,25,23,0,23,12,13,14
,15,16,47,0,30,0,0,0,46,0,0
};

static slk_size_t Parse_row[] = {0

,1,2,17,53,50,69,106,77,22,85,22,93,55,101,70,75
,80,58,39,20,1,106
,0};

static slk_size_t Conflict[] = {

0,0,49,0,0,0,0,0,0,0,0,0,0,0,0,0,27,27,0
,0,27,27,0,27,31,31,31,31,31,31,31,31,31,31,31,31,31,31,31
,31,0,32,31,27,27,27,27,27,27,27,27,27,27,27,27,27,27,0,27
,50,28,0,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,27,0
,28,27,29
};

static slk_size_t Conflict_row[] = {0

,1,21,38,59
,0};

#define GET_CONDITIONAL_PRODUCTION( symbol )  0
#define get_predicted_entry( a,b,c,d,e )  0

#define START_SYMBOL 24
#define END_OF_SLK_INPUT_ 23
#define START_STATE 0
#define START_CONFLICT 47
#define END_CONFLICT 51
#define START_ACTION 46
#define END_ACTION 55
#define TOTAL_CONFLICTS 4
#define PARSE_STACK_SIZE 512


/*************************************************************************************************

PASTE HERE FROM SlkString.cpp

*************************************************************************************************/

const static char * Nonterminal_name[] ={"0"

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

const static char * Terminal_name[] ={"0"
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

const static char * Action_name[] ={"0"

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

const static char * Production_name[] ={"0"

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
	    {"="                                                                                 ,"EQ  operator"     , findId("=")          ,false},
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

} //MathExpressionParserData

#define SlkAction MathExpressionParser

void MathExpressionParser::MapMethods(){

	Action[0] = static_cast<ErrorManagement::ErrorType  (MathExpressionParser::*)(const Token *,BufferedStreamI *)>(NULL);
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

