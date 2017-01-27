/**
 * @file ErrorTypeTest.h
 * @brief Header file for class ErrorTypeTest
 * @date 08/09/2016
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class ErrorTypeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef ERRORTYPETEST_H_
#define ERRORTYPETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
class ErrorTypeTest {
public:

    /**
     * @brief Tests the ErrorType Constructor.
     */
    bool TestConstructor_Default();

    /**
     * @brief Tests the ErrorType Constructor initialised to false.
     */
    bool TestConstructor_False();

    /**
     * @brief Tests the ErrorType Constructor initialised with a bit set.
     */
    bool TestConstructor_BitSet();

    /**
     * @brief Tests the ErrorsCleared function.
     */
    bool TestErrorsCleared();

    /**
     * @brief Tests the operator bool.
     */
    bool TestOperatorBool();

    /**
     * @brief Tests the operator ErrorIntegerFormat.
     */
    bool TestOperatorErrorIntegerFormat();

    /**
     * @brief Tests the equal operator.
     */
    bool TestOperatorEquals();

    /**
     * @brief Tests the not equal operator.
     */
    bool TestOperatorNotEqual();

    /**
     * @brief Tests the assignment operator.
     */
    bool TestOperatorAssignment();

    /**
     * @brief Tests the SetError method.
     */
    bool TestSetError();

    /**
     * @brief Tests the ClearError method.
     */
    bool TestClearError();

    /**
     * @brief Tests the Contains method.
     */
    bool TestContains();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* ERRORTYPETEST_H_ */

