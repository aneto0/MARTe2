// Works with three levels of precedence and with assignment statement
// Also typecast works http://hypertextbookshop.com/transPL/Contents/01_Topics/03_Parsing/05_Section_5/02_page_2_-_LL(1)_Table_Conflicts.html (it's quite ugly although)
// version07: works with functions (typecast not tested yet), provided that all the expression is enclosed between
//            parenthesis (maybe because operation4 expects something after a STRING token, and if no parenthesis is
//            given then ret = A + B results in STRING EOF which operation4 can't handle) 
// version08: works with functions and typecasts, but with the same problem as version07
// version10: everything works, a little tidier than version09
// version11: tidied up, there's a problem when a typecast is followed by an operation
// version12: solved typecast problem, added multiargument function


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

// *********** Comparisons **********************

operation1:
    operation2 { __PushOperator comp_op operation2 __PopOperator }

comp_op:
    >
    <

// *********** Addition, subtraction ************

operation2:
    operation3 { __PushOperator add_op operation3 __PopOperator }

add_op:
    +
    -

// *********** Multiplication, division **********

operation3:
    operation4 { __PushOperator mul_op operation4 __PopOperator }

mul_op:
    *
    /

// *********** Power *******************************

operation4:
    operation5 { __PushOperator pow_op operation5 __PopOperator }

pow_op:
    ^

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

// ****** Typecast

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
