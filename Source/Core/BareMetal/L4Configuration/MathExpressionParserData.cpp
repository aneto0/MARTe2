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

#if 1

#include "SimpleGrammar4.inc"

#else
namespace MARTe{


/*************************************************************************************************

lexical parameters

*************************************************************************************************/


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
static const char * terminals                 = "=+/-*()><^!";

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

,6,12,22,1,2,16,23 ,2,13,3 ,2,13,4 ,2,14,5
,2,14,6 ,2,15,7 ,3,16,17,20 ,3,17,18,21 ,5,18,24,15,19,25
,2,18,19 ,7,18,8,26,1,9,19,27 ,3,19,28,1 ,3,19,28,10
,4,19,8,16,9 ,5,20,24,13,16,25 ,1,20 ,5,21,24,14,17,25
,1,21
,0};

static slk_size_t Production_row[] = {0

,1,8,11,14,17,20,23,27,31,37,40,48,52,56,61,67
,69,75
,0};

static slk_size_t Parse[] = {

0,0,7,1,18,18,17,17,7,7,18,7,18,8,2,3,6,10,0
,8,8,0,8,9,19,12,10,15,15,4,5,0,14,16,13,16
};

static slk_size_t Parse_row[] = {0

,2,11,24,9,1,12,16,24,24,1
,0};

static slk_size_t Conflict[] = {

0,0,20,0,10,10,10,10,10,10,21,10,11,0,10,10,10,10,0
,11,10,11
};

static slk_size_t Conflict_row[] = {0

,1,1,11
,0};

#define GET_CONDITIONAL_PRODUCTION( symbol )  0
#define get_predicted_entry( a,b,c,d,e )  0

#define START_SYMBOL 12
#define END_OF_SLK_INPUT_ 11
#define START_STATE 0
#define START_CONFLICT 19
#define END_CONFLICT 22
#define START_ACTION 22
#define END_ACTION 29
#define TOTAL_CONFLICTS 3
#define PARSE_STACK_SIZE 512



/*************************************************************************************************

PASTE HERE FROM SlkString.cpp

*************************************************************************************************/

static const char * Nonterminal_name[] ={"0"

,"assignment"
,"level1_operator"
,"level2_operator"
,"prefix_operator"
,"full_expression"
,"level2_expression"
,"full_element"
,"basic_element"
,"level1_operator_full_expression_opt"
,"level2_operator_level2_expression_opt"
};

static const char * Terminal_name[] ={"0"

,"STRING"
,"="
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
,"__End"
,"__PushOperator"
,"__PopOperator"
,"__PushTypecast"
,"__PopTypecast"
,"__AddOperand"
};

static const char * Production_name[] ={"0"

,"assignment --> __StoreAssignment STRING = full_expression __End"
,"level1_operator --> +"
,"level1_operator --> -"
,"level2_operator --> *"
,"level2_operator --> /"
,"prefix_operator --> !"
,"full_expression --> level2_expression level1_operator_full_expression_opt"
,"level2_expression --> full_element level2_operator_level2_expression_opt"
,"full_element --> __PushOperator prefix_operator basic_element __PopOperator"
,"full_element --> basic_element"
,"full_element --> ( __PushTypecast STRING ) basic_element __PopTypecast"
,"basic_element --> __AddOperand STRING"
,"basic_element --> __AddOperand NUMBER"
,"basic_element --> ( full_expression )"
,"level1_operator_full_expression_opt --> __PushOperator level1_operator full_expression __PopOperator"
,"level1_operator_full_expression_opt -->"
,"level2_operator_level2_expression_opt --> __PushOperator level2_operator level2_expression __PopOperator"
,"level2_operator_level2_expression_opt -->"
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


} //MathExpressionParserData

#define SlkAction MathExpressionParser

void MathExpressionParser::MapMethods(){

	Action[0] = static_cast<void (MathExpressionParser::*)(const Token *,BufferedStreamI *)>(NULL);
/*************************************************************************************************

	PASTE HERE FROM SlkString.cpp
	use the name of the final class instead of SlkAction

*************************************************************************************************/

    Action [ 1 ] = &SlkAction::StoreAssignment;
    Action [ 2 ] = &SlkAction::End;
    Action [ 3 ] = &SlkAction::PushOperator;
    Action [ 4 ] = &SlkAction::PopOperator;
    Action [ 5 ] = &SlkAction::PushTypecast;
    Action [ 6 ] = &SlkAction::PopTypecast;
    Action [ 7 ] = &SlkAction::AddOperand;



/*************************************************************************************************/

}


} //MARTe

#endif
