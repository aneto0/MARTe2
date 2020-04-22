// Works with three levels of precedence and with assignment statement
// Also typecast works http://hypertextbookshop.com/transPL/Contents/01_Topics/03_Parsing/05_Section_5/02_page_2_-_LL(1)_Table_Conflicts.html (it's quite ugly although)
// version07: works with functions (typecast not tested yet), provided that all the expression is enclosed between
//            parenthesis (maybe because operation4 expects something after a STRING token, and if no parenthesis is
//            given then ret = A + B results in STRING EOF which operation4 can't handle) 
// version08: works with functions and typecasts, but with the same problem as version07

assignment:
   __StoreAssignment STRING = operation0 __End

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

// *********** Unary expression ******************* (prefix operator: sin, cos...)

operation4:
    operation5
//    __PushOperator STRING operation5 __PopOperator

// ********** Typecast ****************************

operation5:
    ( more_op_5
    operand

more_op_5:
    __PushTypecast STRING ) typecast_operand
    operation0 )

typecast_operand:
                       ( operation0 ) __PopTypecast
    __AddOperand         STRING       __PopTypecast
    __AddOperandTypecast NUMBER

operand:
    __AddOperand STRING
    __AddOperand NUMBER
    __ImHere __AddOperand STRING STRING

//operation5:
//    [ ( types ) ] primary

//types:
//    float32
//    bool
//    ...

//operation5:
//    primary

//primary:
//    ( operation0 )
//      operand

//operand:
//    __AddOperand STRING
//    __AddOperand NUMBER
//    __AddOperandTypecast ( STRING ) NUMBER 

