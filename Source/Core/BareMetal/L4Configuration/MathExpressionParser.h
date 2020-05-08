/**
 * @file MathExpressionParser.h
 * @brief Header file for class MathExpressionParser
 * @date Apr 7, 2020
 * @author Giuseppe Ferrò, RFX
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

 * @details This header file contains the declaration of the class MathExpressionParser
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef MATHEXPRESSIONPARSER_H_
#define MATHEXPRESSIONPARSER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ParserI.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


namespace MathExpressionParserData{
	extern const ParserData parserData;
}


/**
 * @brief Parser of mathematical expressions in infix form. The expression
 *        can be converted into stack machine form or RPN.
 *
 * @details This class is a concrete class for MARTe::ParserI providing
 *          the actual lexical elements and parsing rules for interpreting
 *          a mathematical expression in infix form.
 * 
 * The parser requires a mathematical expression in the form:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ret = sin(A +B) > ((type)(C+D) * tan((bool)E + (float)15))
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * @warning Because of a still unsolved bug the expression cannot end
 *          with a string token. Temporary workaround: enclose the
 *          right-hand side between parenthesis if last element is
 *          a variable.
 * 
 * The mathematical expression must be provided to the parser at
 * construction time. The instance of the parser is then bound to that
 * specific expression. 
 * 
 * Constructor requires:
 * @li an input stream of characters containing the mathematical
 *     expression in infix form,
 * @li an output foo structured data, (TODO this is not necessary and will be removed)
 * @li an output stream of characters where the parser will write all
 *     the errors found on the input stream of characters.
 * 
 * To make the parser parse the expression, users should call the Parse()
 * method. Provided that the Parse() method was
 * called, the expression in stack machine form is then availabe as the
 * output of the GetStackMachineExpression() method.
 * 
 * All the instances of the parser use the lexical elements defined
 * in MARTe::MathGrammar and apply the parsing rules of the following
 * grammar:
 *
 * @todo insert grammar here
 *
 * Note: This grammar is written in the SLK language and refers to functions
 * declared in this parser.
 */

class DLL_API MathExpressionParser : public ParserI {

public:
    MathExpressionParser();

    virtual ~MathExpressionParser();
    
    CCString GetStackMachineExpression();

protected:
    
    /**
     * @name Math expression-specific methods
     */
    //@{
		/**
		 * @brief   Pushes an operator to the top of the #operatorStack.
		 * @details This method gets called whenever the parser hits
		 *          an operator. The operator is then stored in the 
		 *          #operatorStack and later appended by PopOperator().
		 */
    	ErrorManagement::ErrorType  PushOperator(const Token *currentToken,BufferedStreamI *errorStream);
		
		/**
		 * @brief   Pops an operator from the top of the #operatorStack.
		 * @details This method gets called whenever the parser hits
		 *          the end of an infix operation. The operation operator
		 *          previously stored by PushOperator() is then popped 
		 *          from the #operatorStack and appended to the stack
		 *          machine expression #stackMachineExpr.
		 */
    	ErrorManagement::ErrorType  PopOperator(const Token *currentToken,BufferedStreamI *errorStream);
		
		/**
		 * @brief   Pushes a typecast type to the top of the #typecastStack.
		 * @details This method gets called whenever the parser hits
		 *          a typecast operation in the syntax `(type)`.
		 *          The typecast type is then stored in the #typecastStack
		 *          and later appended by PopTypecast().
		 */
    	ErrorManagement::ErrorType  PushTypecast(const Token *currentToken,BufferedStreamI *errorStream);
		
		/**
		 * @brief   Pops a typecast type from the top of the #typecastStack.
		 * @details This method gets called whenever the parser hits
		 *          the end of a typecast operation. The typecast type
		 *          previously stored by PushTypecast() is then popped 
		 *          from the typecastStack and appended to the stack
		 *          machine expression #stackMachineExpr.
		 */
    	ErrorManagement::ErrorType  PopTypecast(const Token *currentToken,BufferedStreamI *errorStream);
		
		/**
		 * @brief   Append an encountered operand (constant or variable)
		 *          to the output expression.
		 * @details This method gets called whenever the parser hits an
		 *          operand (that is, an isolated STRING or NUMBER token).
		 *          The operand is immediatly added to the #stackMachineExpr
		 *          in the syntax required by the expression evaluator
		 *          engine (that is, as `READ STRING` for STRING
		 *          tokens and as `CONST NUMBER` for NUMBER tokens.
		 * @warning NUMBER tokens should be handled by AddOperandTypecast().
		 */
    	ErrorManagement::ErrorType  AddOperand(const Token *currentToken,BufferedStreamI *errorStream);
		
		/**
		 * @brief   Append an encountered constant to the output expression.
		 * @details This method gets called whenever the parser hits an
		 *          isolated NUMBER token for which store type has been
		 *          specified in the format `(type) CONSTANT` (e.g. 
		 *          `(float32) 1.52`).
		 *          The constant is immediatly added to the #stackMachineExpr
		 *          in the syntax required by the expression evaluator
		 *          engine (that is, as `READ STRING` for STRING
		 *          tokens and as `CONST type NUMBER` for NUMBER tokens.
		 * @warning NUMBER tokens should be handled by AddOperandTypecast().
		 */
    	ErrorManagement::ErrorType  AddOperandTypecast(const Token *currentToken,BufferedStreamI *errorStream);
		
        /**
         * @brief   Called when the parser hits the end of the expression.
         * @details The method executes all operations required when
         *          the parser hits the end of the mathematical
         *          expression under analysis. In particular, this
         *          method is responsible for appending the `WRITE`
         *          statement required by the expression evaluation engine.
         */
    	ErrorManagement::ErrorType  PopAssignment(const Token *currentToken,BufferedStreamI *errorStream);
        /**
         * @brief   Pops alternative forms of operators.
         * @details Alternative version of PopOperator() in case of
         *          operators that have more than one meaning (e.g.
         *          prefix `+` must be ignored, prefix `-` has a
         *          different meaning with respect to infix `-` etc).
         */
    	ErrorManagement::ErrorType  PopOperatorAlternate(const Token *currentToken,BufferedStreamI *errorStream);

		/**
		 * @brief   Stores the name of the variable before the equal sign.
		 * @details This method gets called when the left-hand side of a
		 *          mathematical expression is hit bt the parser. The STRING
		 *          token before the assignment operator is stored and
		 *          then printed at the end of the #stackMachineExpr by
		 *          the End() method. For example in:
		 *          
		 *          <pre> ret = A + B </pre>
		 * 
		 *          ret is stored, and later written at the end of the 
		 *          #stackMachineExpr as
		 * 
		 *          <pre> WRITE ret </pre>
		 */
    	ErrorManagement::ErrorType  StoreAssignment(const Token *currentToken,BufferedStreamI *errorStream);
		
		/**
		 * @brief   Called when the parser hits the end of the expression.
		 * @details The method executes all operations required when
		 *          the parser hits the end of the mathematical
		 *          expression under analysis. In particular, this
		 *          method is responsible for appending the `WRITE`
		 *          statement required by the expression evaluation engine.
		 */
    	ErrorManagement::ErrorType  End(const Token *currentToken,BufferedStreamI *errorStream);
    //@}
    
    /**
     * @brief   Translates an operator from infix mathematical syntax
     *          to the form requierd by PseudoCode.h.
     * @param[in]  operatorIn Operator in infix form (`+`, `-` ...)
     * @returns Operator in the form required by PseudoCode.h (`SUM`, `POW` ...) 
     * @details The stack machine expression is required to express
     *          operators as uppercase postfix string. This method 
     *          transforms infix operators as follows:
     * 
     *          Infix  | Stack
	 *          -----: | :-----
	 *          +      | SUM 
	 *          -      | MIN 
	 *          *      | MUL
	 *          /      | DIV
	 *          ^      | POW
	 *          sin    | SIN
	 *          tan    | TAN
	 *          ...    | ...
     */
    CCString OperatorLookupTable(CCString operatorIn);


	
	StreamString              stackMachineExpr;    //!< @brief Holds the mathematical expression in stack machine form while parsing.
	StreamString              assignmentVarName;   //!< @brief Holds the name of the variable to whom the whole expression is going to be assigned.
	
    StaticList<StreamString*> operatorStack;     //!< @brief Stack where operators are pushed to and popped from while parsing.
    StaticList<StreamString*> typecastStack;     //!< @brief Stack where typecast types are pushed to and popped from while parsing.
    
private:

	virtual ErrorManagement::ErrorType Execute(const uint32 number,const Token *currentToken,BufferedStreamI *errorStream);

    /**
     * implemented in MathExpressionParserData.cpp
     */
	void MapMethods();


	ErrorManagement::ErrorType  (MathExpressionParser::*Action[10])(const Token *,BufferedStreamI *);
    
};



}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MATHEXPRESSIONPARSER_H_ */

