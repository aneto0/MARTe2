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

#include "Token.h"
#include "StructuredDataI.h"
#include "StreamString.h"
#include "AnyTypeCreator.h"
#include "LexicalAnalyzer.h"
#include "ParserI.h"

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

class MathExpressionParser {
public:
    MathExpressionParser(StreamI &stream,
                         StructuredDataI &databaseIn,
                         BufferedStreamI * const err = static_cast<BufferedStreamI*>(NULL));

    virtual ~MathExpressionParser();

    bool Parse();
    
    StreamString GetStackMachineExpression();

    GrammarInfo GetGrammarInfo() const;

protected:
    
    /**
     * @name Methods called by the parser
     */
    //@{
		/**
		 * @brief   Pushes an operator to the top of the operator stack.
		 * @details This method gets called whenever the parser hits
		 *          an operator. The operator is then stored in the 
		 *          operatorStack and later appended by PopOperator().
		 */
		virtual void PushOperator();
		
		/**
		 * @brief   Pops an operator from the top of the operator stack.
		 * @details This method gets called whenever the parser hits
		 *          the end of an infix operation. The operation operator
		 *          previously stored by PushOperator() is then popped 
		 *          from the operatorStack and appended to the stack
		 *          machine expression StackMachineExpr.
		 */
		virtual void PopOperator();
		
		/**
		 * @brief   Pushes a typecast type to the top of the typecastStack.
		 * @details This method gets called whenever the parser hits
		 *          a typecast operation in the syntax <pre>(type)</pre>.
		 *          The typecast type is then stored in the typecastStack
		 *          and later appended by PopTypecast().
		 */
		virtual void PushTypecast();
		
		/**
		 * @brief   Pops a typecast type from the top of the typecastStack.
		 * @details This method gets called whenever the parser hits
		 *          the end of a typecast operation. The typecast type
		 *          previously stored by PushTypecast() is then popped 
		 *          from the typecastStack and appended to the stack
		 *          machine expression StackMachineExpr.
		 */
		virtual void PopTypecast();
		
		/**
		 * @brief   Append an encountered operand (constant or variable)
		 *          to the output expression.
		 * @details This method gets called whenever the parser hits an
		 *          operand (that is, an isolated STRING or NUMBER token).
		 *          The operand is immediatly added to the StackMachineExpr
		 *          in the syntax required by the expression evaluator
		 *          engine (that is, as <pre>READ STRING</per> for STRING
		 *          tokens and as <pre>CONST NUMBER</pre> for NUMBER tokens.
		 * @warning NUMBER tokens should be handled by AddOperandTypecast().
		 */
		virtual void AddOperand();
		
		/**
		 * @brief   Append an encountered constant to the output expression.
		 * @details This method gets called whenever the parser hits an
		 *          isolated NUMBER token for which store type has been
		 *          specified in the format <pre> (float32) 1.52 .
		 *          The consant is immediatly added to the StackMachineExpr
		 *          in the syntax required by the expression evaluator
		 *          engine (that is, as <pre>READ STRING</per> for STRING
		 *          tokens and as <pre>CONST NUMBER</pre> for NUMBER tokens.
		 * @warning NUMBER tokens should be handled by AddOperandTypecast().
		 */
		virtual void AddOperandTypecast();
		
		/**
		 * @brief   Stores the name of the variable before the equal sign.
		 * @details This method gets called when the left-hand side of a
		 *          mathematical expression is hit bt the parser. The STRING
		 *          token befor the assignment operator is stored and
		 *          then printed at the end of the StackMachineExpr by
		 *          the End() method. For example in:
		 *          
		 *          <pre> ret = A + B </pre>
		 * 
		 *          ret is stored, and later written in the end of the 
		 *          stack machine expression as
		 * 
		 *          <pre> WRITE ret </pre>
		 */
		virtual void StoreAssignment();
		
		/**
		 * @brief   Called when the parser hits the end of the expression.
		 * @details The method executes all operations required when
		 *          the parser hits the end of the mathematical
		 *          expression under analysis. In particular, this
		 *          method is responsible for appending the <pre>WRITE</pre>
		 *          statement required by the expression evaluation engine.
		 */
		virtual void End();
    //@}
    
    /**
     * @brief   Translates an operator from infix mathematical syntax
     *          to the form requierd by PseudoCode.h .
     * @details The stack machine expression is required to express
     *          operators as uppercase postfix string. This method 
     *          transforms <pre>+</pre> in <pre>SUM</pre>, <pre>sin</pre>
     *          in <pre>SIN</pre> and so on.
     */
    const char8* OperatorLookupTable(const char8* operatorIn);

    virtual uint32 &GetProduction(const uint32 index) const;

    virtual uint32 GetProductionRow(const uint32 index) const;

    virtual uint32 GetParse(const uint32 index) const;

    virtual uint32 GetParseRow(const uint32 index) const;

    virtual uint32 GetConflict(const uint32 index) const;

    virtual uint32 GetConflictRow(const uint32 index) const;

    virtual uint32 GetConstant(const uint32 index) const;

    virtual const char8 *GetSymbolName(const uint32 symbol) const;

    virtual uint32 GetNextTokenType();

    virtual uint32 PeekNextTokenType(const uint32 position);

    inline void StackPush(const uint32 symbol,
                          const uint32 * const stack,
                          uint32* &top) const;

    inline uint32 StackPop(uint32* &top) const;

    virtual void Execute(const uint32 number);

    Token *currentToken;
    
    StreamString StackMachineExpr;
    
    StaticList<StreamString*> operatorStack;
    
    StaticList<StreamString*> typecastStack;

private:

    StreamString typeName;

    StreamString assignmentVarName;

    StructuredDataI *database;

    bool isError;

    LexicalAnalyzer tokenProducer;
    //ExpressionAnalyzer tokenProducer;
    
    uint32 numberOfColumns;

    uint32 firstNumberOfColumns;

    uint32 numberOfRows;

    AnyTypeCreator memory;

    BufferedStreamI *errorStream;

    uint32 tokenType;

    uint8 numberOfDimensions;

    GrammarInfo grammar;

    void (MathExpressionParser::*Action[10])(void);

    //char8 operatorStack[512];

    //uint32 stackTop;
    
};



}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


namespace MARTe {

void MathExpressionParser::StackPush(const uint32 symbol,
                        const uint32 * const stack,
                        uint32 *&top) const {
    /*lint -e{946} [MISRA C++ Rule 5-0-15], [MISRA C++ Rule 5-0-17]. Justification: stack implementation requires operational applied to pointer. */
    if (top > stack) {
        /*lint -e{165} , [MISRA C++ Rule 5-0-18]. Justification: stack implementation requires subtraction applied to pointer. */
        top--;
        *top = symbol;
    }
}

uint32 MathExpressionParser::StackPop(uint32 * &top) const {
    uint32 ret = 0u;
    if (*top != 0u) {
        ret = *top;
        top++;
    }
    return ret;
}

}
#endif /* MATHEXPRESSIONPARSER_H_ */

