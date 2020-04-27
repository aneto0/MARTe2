assignment:
   __StoreAssignment STRING = operation0 __End EOF

// *********** Comparisons **********************

operation0:
    operation1 { __PushOperator operator0 operation1 __PopOperator }

operator0:
    >
    <

// *********** Addition, subtraction ************

operation1:
    operation2 { __PushOperator operator1 operation2 __PopOperator }

operator1:
    +
    -

// *********** Multiplication, division **********

operation2:
    operation3 { __PushOperator operator2 operation3 __PopOperator }

operator2:
    *
    /

// *********** Power *******************************

operation3:
    operation4 { __PushOperator operator3 operation4 __PopOperator }

operator3:
    ^

// *********** Unary expresion ******************* (prefix operator: !, ... )

operation4:
    operation5
      __PushOperator operator4 operation5 __PopOperator

operator4:
    !

// ****** Typecast

operation5:
    operation6
    __AddOperand NUMBER      // NUMBER must be treated differently to prevent conflicts (see comments below)
    ( __PushTypecast STRING ) operation6 __PopTypecast
    ( __PushTypecast STRING ) __AddOperandTypecast NUMBER

// ********** Functions ****************************

operation6:
    ( operation0 )
    __AddOperand   STRING
    __PushOperator STRING ( operation0 ) __PopOperator

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
