/**
 * @file MathExpressionParser.h
 * @brief Header file for class MathExpressionParser
 * @date 07/04/2020
 * @author Nicolo Ferron
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

/**
 * @brief Parser of mathematical expressions in infix form which can
 *        also convert them into stack machine form or RPN.
 *
 * @details This class is a concrete class for MARTe::ParserI providing
 *          the actual lexical elements and parsing rules for interpreting
 *          a mathematical expression in infix form.
 * 
 * The parser accepts C-like mathematical expressions in the form:
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.c}
 * ret = sin(A + B) > (pow(C + D, E) * tan((bool)F + (float)15));
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * Each expression must be terminated with a comma or a semicolon.
 * Multiple expressions are allowed (provided that each is terminated
 * with a comma or semicolon). Accepted operators are the following:
 * 
 * | Operator | Meaning           |
 * | :------: | :---------------- |
 * | =        | Assignment        |
 * | &&       | AND               |
 * | \|\|     | OR                |
 * | ^        | Exclusive OR      |
 * | !        | NOT               |
 * | <        | Less than         |
 * | >        | Greater than      |
 * | <=       | Less ot equal     |
 * | >=       | Greater or equal  |
 * | ==       | Equal             |
 * | !=       | Not equal         |
 * | +        | Sum               |
 * | -        | Subtraction       |
 * | *        | Multiplication    |
 * | /        | Division          |
 * | , ; \\n  | End of expression |
 * 
 * Functions in the form `sin(x)`, `pow(x,y)` etc are supported.
 * However, the function name is passed as-is to the evaluation engine,
 * so please verify that the evaluation engine (typically MARTe::RuntimeEvaluator)
 * supports the required function.
 * 
 * The mathematical expression must be provided to the parser at
 * construction time. The instance of the parser is then bound to that
 * specific expression. 
 * 
 * Constructor requires:
 * - an input stream of characters containing the mathematical
 *   expression in infix form,
 * - an output stream of characters where the parser will write all
 *   the errors found on the input stream of characters (optional).
 * 
 * To make the parser parse the expression, users should call the Parse()
 * method. Provided that the Parse() method was called, the expression
 * in stack machine form is then availabe as the output of the
 * GetStackMachineExpression() method.
 * 
 * Example usage with the following expression:
 * `retVar = pow(sin(theta), 2) + pow(cos(theta), 2)`
 * 
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~{.cpp}
 * #include "MathExpressionParser.h"
 * 
 * StreamString expr = "retVar = pow(sin(theta), 2) + pow(cos(theta), (int64) 2);"
 * expr.Seek(0);
 * 
 * MathExpressionParser mathParser(expr);
 * bool parseOk = mathParser.Parse();
 * 
 * if (parseOk) {
 *     StreamString outputExpr;
 *     outputExpr = mathParser.GetStackMachineExpression();
 *     
 *     REPORT_ERROR(ErrorManagement::Information, "\n%s", outputExpr.Buffer());
 * }
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * 
 * This should print:
 * 
 * ~~~~~~~~~~~~
 * READ theta
 * SIN
 * CONST float64 2
 * POW
 * READ theta
 * COS
 * CONST int64 2
 * POW
 * ADD
 * WRITE retVar
 * ~~~~~~~~~~~~
 *
 * All the instances of this parser use the lexical elements defined
 * in the MathGrammar of MARTe::GrammarInfo and apply the parsing rules
 * defined in MathGrammar.ll:
 * 
 * @include MathGrammar.ll
 * 
 * @note This grammar is written in the SLK language and refers to
 * functions declared in this parser.
 * 
 */

class DLL_API MathExpressionParser : public ParserI {

public:

    /**
     * @brief Default constructor.
     * @param[in]  stream is the stream containing the expression to be parsed.
     * @param[out] err    is the stream where error messages are printed to (optional parameter).
     */
    MathExpressionParser(StreamI &stream,
                         BufferedStreamI * const err = static_cast<BufferedStreamI*>(NULL));
    
    /**
     * @brief Destructor.
     */
    virtual ~MathExpressionParser();
    
    /**
     * @brief   Returns the input expression in stack machine form.
     * @returns the input expression in stack machine form.
     * @pre     Parse() method must have been called.
     */
    StreamString GetStackMachineExpression() const;

protected:
    
    /**
     * @name    Parsing methods
     * @brief   Methods invoked by the parser during parsing.
     * @details These methods manage the three stacks of the class:
     *          #operatorStack, #typecastStack and #assignmentVarName.
     *          Each time the parser hits an element it pushes it to
     *          the appropriate stack and is then responsible for popping
     *          it in the right order into the #stackMachineExpression
     *          so that the resulting expression is in RPN form.
     *          The order in which push and pop methods are called is
     *          specified in MathGrammar.ll
     */
    //@{
        /**
         * @brief   Pushes an operator to the top of the #operatorStack.
         * @details This method gets called whenever the parser hits
         *          an operator. The operator is then stored in the 
         *          #operatorStack and later appended by PopOperator().
         */
        virtual void PushOperator();
        
        /**
         * @brief   Pops an operator from the top of the #operatorStack.
         * @details This method gets called whenever the parser hits
         *          the end of an infix operation. The operation operator
         *          previously stored by PushOperator() is then popped 
         *          from the #operatorStack and appended to the stack
         *          machine expression #stackMachineExpr.
         */
        virtual void PopOperator();
        
        /**
         * @brief   Pops alternative forms of operators.
         * @details Alternative version of PopOperator() in case of
         *          operators that have more than one meaning (e.g.
         *          in `+ A - B` prefix `+` must be ignored, in `- A - B`
         *          prefix `-` has a different meaning with respect
         *          to infix `-` etc).
         */
        virtual void PopOperatorAlternate();
        
        /**
         * @brief   Pushes a typecast type to the top of the #typecastStack.
         * @details This method gets called whenever the parser hits
         *          a typecast operation in the syntax `(type)`.
         *          The typecast type is then stored in the #typecastStack
         *          and later appended by PopTypecast().
         */
        virtual void PushTypecast();
        
        /**
         * @brief   Pops a typecast type from the top of the #typecastStack.
         * @details This method gets called whenever the parser hits
         *          the end of a typecast operation. The typecast type
         *          previously stored by PushTypecast() is then popped 
         *          from the typecastStack and appended to the stack
         *          machine expression #stackMachineExpr.
         */
        virtual void PopTypecast();
        
        /**
         * @brief   Appends an encountered operand (constant or variable)
         *          to the output expression.
         * @details This method gets called whenever the parser hits an
         *          operand (that is, an isolated `STRING` or `NUMBER` token).
         *          The operand is immediately added to the #stackMachineExpr
         *          in the syntax required by the expression evaluator
         *          engine (that is, as `READ STRING` for `STRING`
         *          tokens and as `CONST float64 NUMBER` for `NUMBER` tokens).
         * @warning Note that when no type is specified for `NUMBER`
         *          token, `float64` type is assumed.
         */
        virtual void AddOperand();
        
        /**
         * @brief   Append an encountered constant to the output expression.
         * @details This method gets called whenever the parser hits an
         *          isolated NUMBER token for which store type has been
         *          specified in the input expression with the syntax
         *          `(type) CONSTANT` (e.g. `(float32) 1.52`).
         *          The constant is immediately added to the #stackMachineExpr
         *          in the syntax required by the expression evaluator
         *          engine (that is `CONST type NUMBER`), with `type`
         *          retrieved from the #typecastStack.
         */
        virtual void AddOperandTypecast();
        
        /**
         * @brief   Stores the name of the variable before the equal sign.
         * @details This method gets called when the left-hand side of a
         *          mathematical expression is hit bt the parser. The STRING
         *          token before the assignment operator is stored and
         *          then printed at the end of the #stackMachineExpr by
         *          the End() method. For example in:
         *          
         *          `ret = A + B`
         * 
         *          ret is stored, and later written at the end of the 
         *          #stackMachineExpr as
         * 
         *          `WRITE ret`
         */
        virtual void StoreAssignment();
        
        /**
         * @brief   Called when the parser hits the end of the expression.
         * @details The method executes all operations required when
         *          the parser hits the end of the mathematical
         *          expression under analysis. In particular, this
         *          method is responsible for appending the `WRITE`
         *          statement required by the expression evaluation engine.
         */
        virtual void PopAssignment();
    //@}
    
    /**
     * @brief   Translates an operator from infix mathematical syntax
     *          to the form requierd by PseudoCode.h.
     * @param[in]  operatorIn Operator in infix form (`+`, `&&`, `sin()`, ...)
     * @returns Operator in the form required by PseudoCode.h 
     * @details The stack machine expression is required to express
     *          functions as uppercase postfix string. This method 
     *          transforms function names to uppercase, and operators
     *          in a corresponding uppercase string according to the 
     *          following table:
     *          
     *          | Operator | String |
     *          | :------: | :----- |
     *          | &&       | AND    |
     *          | \|\|     | OR     |
     *          | ^        | XOR    |
     *          | !        | NOT    |
     *          | <        | LT     |
     *          | >        | GT     |
     *          | <=       | LTE    |
     *          | >=       | GTE    |
     *          | ==       | EQ     |
     *          | !=       | NEQ    |
     *          | +        | ADD    |
     *          | -        | SUB    |
     *          | *        | MUL    |
     *          | /        | DIV    |
     *          
     *          For example, the infix expression `y = sin(x) + cos(x)`
     *          is translated as:
     *          
     * ~~~~~~~~~~~~~~~~~
     *          READ x
     *          SIN
     *          READ x
     *          COS
     *          ADD
     *          WRITE y
     * ~~~~~~~~~~~~~~~~~
     * 
     */
    const char8* OperatorFormatting(char8* const operatorIn) const;
    
    /**
     * @name Table navigation methods
     * @see  MARTe::ParserI
     */
    //@{
    /**
     * @see ParserI::GetProduction(*).
     */
    virtual uint32 &GetProduction(const uint32 index) const;
    
    /**
     * @see ParserI::GetProductionRow(*).
     */
    virtual uint32 GetProductionRow(const uint32 index) const;
    
    /**
     * @see ParserI::GetParse(*).
     */
    virtual uint32 GetParse(const uint32 index) const;
    
    /**
     * @see ParserI::GetParseRow(*).
     */
    virtual uint32 GetParseRow(const uint32 index) const;
    
    /**
     * @see ParserI::GetConflict(*).
     */
    virtual uint32 GetConflict(const uint32 index) const;
    
    /**
     * @see ParserI::GetConflictRow(*).
     */
    virtual uint32 GetConflictRow(const uint32 index) const;
    
    /**
     * @see ParserI::GetConstant(*).
     */
    virtual uint32 GetConstant(const uint32 index) const;
    
    /**
     * @see ParserI::GetSymbolName(*).
     */
    virtual const char8 *GetSymbolName(const uint32 symbol) const;
    
    /**
     * @see ParserI::Execute(*).
     */
    virtual void Execute(const uint32 number);
    //@}
    
    /**
     * @name Stacks
     */
    //@{
        StreamString              assignmentVarName;   //!< @brief Holds the name of the variable to whom the whole expression is going to be assigned.
        StaticList<StreamString*> operatorStack;       //!< @brief Stack where operators are pushed to and popped from while parsing.
        StaticList<StreamString*> typecastStack;       //!< @brief Stack where typecast types are pushed to and popped from while parsing.
    //@}
    
    /**
     * @brief Holds the mathematical expression in stack machine form while parsing.
     */
    StreamString              stackMachineExpr;
    
private:
    
    /**
     * The array of functions needed by the parser.
     */
    void (MathExpressionParser::*Action[10])(void);
    
};



}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* MATHEXPRESSIONPARSER_H_ */

