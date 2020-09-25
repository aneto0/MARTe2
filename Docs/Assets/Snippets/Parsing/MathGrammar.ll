system:
    assignment { assignment }


assignment:
   __StoreAssignment STRING = logic_expr delimitator __PopAssignment
   
delimitator:
    ;
    ,


logic_expr:
    comparison_expr { __PushOperator logic_op comparison_expr __PopOperator }

logic_op:
    ||
    &&
    ^


comparison_expr:
    addition_expr { __PushOperator comp_op addition_expr __PopOperator }

comp_op:
    >
    <
    >=
    <=
    ==
    !=


addition_expr:
    multiplication_expr { __PushOperator add_op multiplication_expr __PopOperator }

add_op:
    +
    -


multiplication_expr:
    unary_expr { __PushOperator mul_op unary_expr __PopOperator }

mul_op:
    *
    /


unary_expr:
    typecast_expr
    __PushOperator add_op   typecast_expr __PopOperatorAlternate
    __PushOperator unary_op typecast_expr __PopOperator

unary_op:
    !


typecast_expr:
    function_expr
    ( __PushTypecast STRING ) function_expr __PopTypecast
    ( __PushTypecast STRING ) __AddOperandTypecast NUMBER    // NUMBER token must be treated differently (see below)
    __AddOperand NUMBER


function_expr:
    primary_expr
    __PushOperator STRING ( logic_expr { , logic_expr } ) __PopOperator


primary_expr:
    ( logic_expr )
    __AddOperand   STRING


/*
 * NUMBER must be treated differently. If treated just like STRING the following:
 * (float) 10.5
 * is seen as
 * CONST 10.5
 * CAST float
 * but we want typecasts of constants to be translated in
 * CONST float 10.5
 * since we want the second behaviour for numbers and the first for
 * variables, the two tokens must be handled in different productions
 */
