system:
    assignment { assignment }

// *********** Complete assignment **********************

assignment:
   __StoreAssignment STRING = operation0 delimitator __PopAssignment
   
delimitator:
    ;
    ,

// *************** Logic operations

operation0:
    operation1 { __PushOperator logic_op operation1 __PopOperator }

logic_op:
    &
    |
    ||
    &&
    ^

// *********** Comparisons **********************

operation1:
    operation2 { __PushOperator comp_op operation2 __PopOperator }

comp_op:
    >
    <
    >=
    <=
    ==

// *********** Addition, subtraction ************

operation2:
    operation3 { __PushOperator add_op operation3 __PopOperator }

add_op:
    +
    -

// *********** Multiplication, division **********

operation3:
    operation5 { __PushOperator mul_op operation5 __PopOperator }

mul_op:
    *
    /


// *********** Unary expresion ******************* (prefix operator: !, ... )

operation5:
    operation6
    __PushOperator unary_op_alt operation6 __PopOperatorAlternate
    __PushOperator unary_op     operation6 __PopOperator

unary_op:
    !
    
unary_op_alt:
    +
    -

// *********** Typecast *******************

operation6:
    operation7
    ( __PushTypecast STRING ) operation7 __PopTypecast
    // NUMBER must be treated differently (see below)
    __AddOperand NUMBER
    ( __PushTypecast STRING ) __AddOperandTypecast NUMBER

// ********** Functions ****************************

operation7:
    ( operation0 )
    __AddOperand   STRING
    __PushOperator STRING ( operation0 { , operation0 } ) __PopOperator






// NUMBER must be treated differently.
// If treated just like STRING the following:
// (float) 10.5
// is seen as
// CONST 10.5
// CAST float
// but we want typecasts of constants to be translated in
// CONST float 10.5
// since we want the second behaviour for numbers and the first for
// variables, the two tokens must be treated differently in the productions
