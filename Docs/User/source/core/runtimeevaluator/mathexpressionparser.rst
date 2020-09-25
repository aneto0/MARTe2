.. date: 30/06/2020
   copyright: Copyright 2017 F4E | European Joint Undertaking for ITER and
   the Development of Fusion Energy ('Fusion for Energy').
   Licensed under the EUPL, Version 1.1 or - as soon they will be approved
   by the European Commission - subsequent versions of the EUPL (the "Licence")
   You may not use this work except in compliance with the Licence.
   You may obtain a copy of the Licence at: http://ec.europa.eu/idabc/eupl
   warning: Unless required by applicable law or agreed to in writing, 
   software distributed under the Licence is distributed on an "AS IS"
   basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
   or implied. See the Licence permissions and limitations under the Licence.

Parser of mathematical expressions
==================================

The :vcisdoxygencl:`MathExpressionParser` class is an implementation
of the :vcisdoxygencl:`ParserI` interface that can read systems of
mathematical expressions in infix form and transform them in a
machine-readable form. The current implementation converts the infix
form into a stack machine instruction set very similar to reverse Polish
notation (RPN).

The expression in RPN-like syntax can then be executed by the
:doc:`runtime evaluator </core/runtimeevaluator/evaluator>`.

Syntax
------

The parser accepts C-like mathematical expressions in the form:

.. code-block:: c

    y1 = sin(A + B) > (pow(C + D, E)tan((bool)F + (float)15));
    y2 = 2*y1;

Each expression must be terminated with a comma or a semicolon.
Multiple expressions are allowed (provided that each is terminated
with a comma or semicolon). Accepted operators are the following:

======== =================
Operator Meaning          
======== =================
``=``    Assignment       
``&&``   AND              
``||``   OR               
``^``    Exclusive OR     
``!``    NOT              
``<``    Less than        
``>``    Greater than     
``<=``   Less ot equal    
``>=``   Greater or equal 
``==``   Equal            
``!=``   Not equal        
``+``    Sum              
``-``    Subtraction      
``*``    Multiplication   
``/``    Division         
``, ;``  End of expression
======== =================

Functions in the form ``sin(x)``, ``pow(x,y)`` etc are supported.
However, the function name is passed as-is to the evaluation engine,
so please verify that the evaluation engine (typically
:vcisdoxygencl:`RuntimeEvaluator`) supports the required function.

The mathematical expression must be provided to the parser at
construction time. The instance of the parser is then bound to that
specific expression. 

The constructor requires:

- an input stream of characters containing the mathematical expression in infix form,
- an output stream of characters where the parser will write all the errors found on the input stream of characters (optional).

To make the parser parse the expression, users should call the Parse()
method. Provided that the Parse() method was called, the expression
in stack machine form is then availabe as the output of the
GetStackMachineExpression() method.

Example usage with the following expression:
`retVar = pow(sin(theta), 2) + pow(cos(theta), 2)`

.. code-block:: c++

    #include "MathExpressionParser.h"
    
    StreamString expr = "retVar = pow(sin(theta), 2) + pow(cos(theta), (int64) 2);"
    expr.Seek(0);
    
    MathExpressionParser mathParser(expr);
    bool parseOk = mathParser.Parse();
    
    if (parseOk) {
        StreamString outputExpr;
        outputExpr = mathParser.GetStackMachineExpression();
        
        REPORT_ERROR(ErrorManagement::Information, "\n%s", outputExpr.Buffer());
    }

This prints:

.. code-block:: c++

    READ theta
    SIN
    CONST float64 2
    POW
    READ theta
    COS
    CONST int64 2
    POW
    ADD
    WRITE retVar

All the instances of this parser use the lexical elements defined
in the MathGrammar of :vcisdoxygencl:`GrammarInfo` and apply the parsing
rules defined in MathGrammar.ll:

.. literalinclude:: ../../../../Assets/Snippets/Parsing/MathGrammar.ll
   :caption: Math grammar used by MathExpressionParser
   :linenos:  

.. hint::
    This grammar is written in the SLK language and refers to
    functions declared in :vcisdoxygencl:`MathExpressionParser`.


