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

Runtime evaluator
=================

The runtime evaluator (see :vcisdoxygencl:`RuntimeEvaluator`) is an
engine to evaluate scalar mathematical expressions at runtime.

Summary
-------

RuntimeEvaluator takes an expression in stack machine form at
construction time and is then capable of compiling and executing it.
The expression can be directly supplied in stack machine form, or it
be derived from an infix expression (a mathematical expression
in the usual form) by using :vcisdoxygencl:`MathExpressionParser`.

The RuntimeEvaluator supports scalar operands of any type.

Usage
-----

The steps required to use the evaluator are the following:
- the evaluator is instantiated and fed with the expression it will be required to evaluate
- the evaluator internal variable database is initialised by calling the ExtractVariable method
- variable properties are set by using the APIs
- the expression is compiled by calling the Compile() method
- the expression is executed by calling the Execute() method

Instantiating the evaluator
~~~~~~~~~~~~~~~~~~~~~~~~~~~

The expression must first be fed to the RuntimeEvaluator at
construction time, either typed directly:

.. code-block:: c++

    StreamString rpnCode = "READ A\n"
                           "READ B\n"
                           "ADD\n"
                           "WRITE ret\n"
                           ;

    RuntimeEvaluator expression(rpnCode);


or by converting an infix expression (e.g. via the :vcisdoxygencl:`MathExpressionParser`):

.. code-block:: c++

    StreamString infixExpr = "ret = A + B;"

    MathExpressionParser parser(infixExpr);
    parser.Parse();

    RuntimeEvaluator expression(parser.GetStackMachineExpression());


Initialising the evaluator
~~~~~~~~~~~~~~~~~~~~~~~~~~

First of all, the RuntimeEvaluator must know what variables are contained
in the expression. This is done by calling the ``ExtractVariables()``
method. After that, variable properties can be set by using
RuntimeEvaluator variable managing APIs.

.. code-block:: c++

    ret = expression.ExtractVariables();


Setting up variables
~~~~~~~~~~~~~~~~~~~~

Variable properties must then be set. Variables properties are:
- type (can be one of MARTe2 supported types: ``Unsigned32Bit``, ``Float64Bit`` etc.)
- location (the memory location where the variable value will be held)

Types *must* be set for every input variable by using the
``SetInputVariableType`` and ``SetOutputVaribleType`` methods:

.. code-block:: c++

    ret = expression.SetInputVariableType("theta", Float64Bit);


Output variable types *can* be set with SetOutputVariableType if needed.
However, this is not compulsory as RuntimeEvaluator will assign them
the output type of the last operation.

Memory locations *can* be set by using the SetInputVariableMemory and
SetOutputVariableMemory methods.

.. code-block:: c++

    float64 y;
    ret = expression.SetOutputVariableMemory("y", &y);


- In case an external location for a variable is set, the variable
  is considered *external* and any modification of its value will
  happen to the specified memory address.
- In case an external location for a variable is not set, the variable
  is considered *internal* (this is the default behavior).
  The RuntimeEvaluator will be responsible for allocating space for all the
  internal variables. The memory location of internal variables will
  be available after compilation by calling the GetInputVariableMemory and
  GetOutputVariableMemory methods.

Compiling
~~~~~~~~~

After setting up the desired properties for variables, the expression
can be compiled:

.. code-block:: c++

    ret = expression.Compile();


From now on, variable values can be set and the expression can be
executed.

Executing
~~~~~~~~~

Before executing, values of each variable can be updated. External
variable values are updated simply by updating the memory they have
been set to follow:

.. code-block:: c++

    float64 theta = 5.0;
    ret = expression.SetInputVariableMemory("theta", &theta);
    ret = expression.Compile();
    theta = 10.0;


Internal variables can be modified by retrieving a pointer to them:

.. code-block:: c++

    ret = expression.SetInputVariableType("theta", Float64Bit);

    float64* ptr;
    ptr = (float64*)expression.GetInputVariableMemory("theta");

    *ptr = 10.0;


Each time the expression is executed, all output variables
are updated. If they have been set external, their values are
directly available:

.. code-block:: c++

    float64 y;
    expression.SetOutputVariableMemory("y", &y);
    ...
    expression.Compile();
    ...
    expression.Execute();
    REPORT_ERROR(ErrorManagement::Information, "Value of y is: %f", y);


if they are internal, ``GetOutputVariableMemory`` shall be used to
retrieve their addresses and obtain the final value.

Variable values can also be updated in the middle of the expression:

.. code-block:: c++

    StreamString rpnCode = "READ A\n"      // first
                           "READ B\n"
                           "ADD\n"
                           "WRITE A\n"     // output variable A is set
                           "READ A\n"      // second
                           "WRITE ret\n";


In the example above, the former `READ A` command reads from the
input variable A, while the latter reads from the output variable A
that was just set by the `WRITE A` command.

Variable values can be updated any time, and when invoked the
``Execute()`` method will recalculate the value of output variable
based on the current value of input variables.

Further details
---------------

:vcisdoxygencl:`RuntimeEvaluator` is a stack machine that reads instructions from
a stack machine expression and executes them in a last in-first out
fashion. All operations rely on an internal stack: operands are
retrieved from the stack and results are placed in the stack.

Working principle
~~~~~~~~~~~~~~~~~

The RuntimeEvaluator scans the input stack machine code and the variable
types. Combination of code and types during ``Compile()`` produces
a list of calls to functions with specific types (the "pseudocode")
that will be executed during ``Execute()``.
Functions that will be called must be present in the ``functionRecords``
array, an array that holds all the available functions that
RuntimeEvaluator can call. ``functionRecords`` is an array of
:vcisdoxygencl:`RuntimeEvaluatorFunction` objects.

When the RuntimeEvaluator executes an operation, it actually calls the
corresponding function in ``functionRecords``, or better calls the
``RuntimeEvaluatorFunction::ExecuteFunction()`` method of that function
and passes itself to the method as the argument.
The operation is then executed by :vcisdoxygencl:`RuntimeEvaluatorFunction`, which is
responsible for managing RuntimeEvaluator internal stack by using
``Pop()``, ``Push()`` and ``Peek()`` methods. See
:vcisdoxygencl:`RuntimeEvaluatorFunction` documentation for further details.

Supported operators
~~~~~~~~~~~~~~~~~~~

This is a table of all supported operators:

==================== ========================================================================
 Operator             Meaning                                                                
==================== ========================================================================
 ``READ  var``        Pushes the value of variable `var` from memory to the top of the stack 
 ``WRITE var``        Pops the top of the stack and writes its value to variable `var`       
 ``CONST type val``   Push a constant of value `val` and type `type` to the top of the stack 
 ``CAST type``        Casts the top of the stack to type `type`                              
 ``AND``              AND operation between top two elements of the stack                    
 ``OR``               OR operation between top two elements of the stack                     
 ``XOR``              XOR operation between top two elements of the stack                    
 ``GT``               Greater than operation between top two elements of the stack           
 ``LT``               Less than operation between top two elements of the stack              
 ``GTE``              Greater or equal operation between top two elements of the stack       
 ``LTE``              Less or equal operation between top two elements of the stack          
 ``EQ``               Equal operation between top two elements of the stack                  
 ``NEQ``              Not equal operation between top two elements of the stack              
 ``ADD``              Sum between top two elements of the stack                              
 ``SUB``              Subtraction between top two elements of the stack                      
 ``MUL``              Multiplication between top two elements of the stack                   
 ``DIV``              Division between top two elements of the stack                         
 ``SIN``              Sine operation on the top of the stack                                 
 ``COS``              Cosine operation on the top of the stack                               
 ``POW``              Power operation between top two elements of the stack                  
==================== ========================================================================

.. hint::
      Comparison of floating-point types may often be implementation-dependent.
      The `EQ` and `NEQ` operators use the SafeMath::IsEqual()
      instead of the standard `==` operator in order to be safely
      portable: this implementation of the comparison operation takes
      into account the floating-point operand granularity
      (machine epsilon) to achieve a safely portable equality check.

Adding new functions
~~~~~~~~~~~~~~~~~~~~

New operations can be made available to the RuntimeEvaluator by
adding a RuntimeEvaluatorFunction to the ``functionRecords`` as follows:

.. code-block:: c++

    void NewOperation(RuntimeEvaluator &evaluator) {
        float32 x1,x2,x3;
        evaluator.Pop(x1);
        evaluator.Pop(x2);
        x3 = x2 + x1;
        evaluator.Push(x3);
    }

    TypeDescriptor types[] = {Float32Bit, Float32Bit, Float32Bit};
    RuntimeEvaluatorFunction newAdd("NEWADD", 2, 1, types, NewOperation);
    RegisterFunction(newAdd);


The function above pops two ``float32`` element from the stack, sums them and then pushes the result to the stack. Upon calling the ``RegisterFunction`` function, the function itself becomes available to the RuntimeEvaluator by using the command ``NEWADD``.

Examples
--------

Example usage with the following expression:
``y = pow(sin(theta), 2) + pow(cos(theta), 2)``

(for a more extendive example see below).

.. code-block:: c++

    #include "RuntimeEvaluator.h"

    bool ret;

    StreamString rpnCode = "READ theta\n"
                           "SIN\n"
                           "CONST int64 2\n"
                           "POW\n"
                           "READ theta\n"
                           "COS\n"
                           "CONST int64 2\n"
                           "POW\n"
                           "ADD\n"
                           "WRITE y\n"
    ;

    RuntimeEvaluator expression(rpnCode);

    ret = expression.ExtractVariables();

    float64 theta;
    float64 y;

    if (ret) {
        ret &= expression.SetInputVariableType("theta", Float64Bit);
        ret &= expression.SetInputVariableMemory("theta", &theta);
    }

    if (ret) {
        ret &= expression.SetOutputVariableType("y", Float64Bit);
        ret &= expression.SetOutputVariableMemory("y", &y);
    }

    if (ret) {
        ret = expression.Compile();
    }

    // now variable values can be set
    theta = 3.14;

    if (ret) {
        ret = expression.Execute();
    }

    // result is now available
    if (y == 1.0) {
        REPORT_ERROR(ErrorManagement::Information, "OK!");
    }

The following example shows how to use RuntimeEvaluator to perform runtime calculations. 

.. literalinclude:: /_static/examples/Core/RuntimeEvaluatorExample1.cpp
   :language: c++   
   :caption: Runtime evaluator example (RuntimeEvaluatorExample1)
   :linenos:

Instructions on how to compile and execute the example can be found :doc:`here </core/examples>`.
