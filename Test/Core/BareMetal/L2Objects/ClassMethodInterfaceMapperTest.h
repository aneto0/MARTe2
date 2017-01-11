/**
 * @file ClassMethodInterfaceMapperTest.h
 * @brief Header file for class ClassMethodInterfaceMapperTest
 * @date 24/05/2016
 * @author Ivan Herrero
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

 * @details This header file contains the declaration of the class ClassMethodInterfaceMapperTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSMETHODINTERFACEMAPPERTEST_H_
#define CLASSMETHODINTERFACEMAPPERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Unit tests for the public methods of ClassMethodInterfaceMapperTest.
 */
class ClassMethodInterfaceMapperTest {
public:

    /**
     * @brief Default constructor.
     **/
    ClassMethodInterfaceMapperTest();

    /**
     * @brief Destructor.
     **/
    virtual ~ClassMethodInterfaceMapperTest();

    /**
     * @brief Tests the constructor with no parameters.
     */
    bool TestConstructor_VoidParameters();

    /**
     * @brief Tests the Constructor with 1 parameter in all the 3 possible combinations
     */
    bool TestConstructor_OneParameter_C();
    bool TestConstructor_OneParameter_R();
    bool TestConstructor_OneParameter_W();

    /**
     * @brief Tests the Constructor with 1 parameter that is a pointer (a feature that is not supported).
     */
    bool TestConstructor_OneParameter_Pointer();

    /**
     * @brief Tests the Constructor with 2 parameters in all the 9 possible combinations
     */
    bool TestConstructor_TwoParameters_C_C();
    bool TestConstructor_TwoParameters_C_R();
    bool TestConstructor_TwoParameters_C_W();
    bool TestConstructor_TwoParameters_R_C();
    bool TestConstructor_TwoParameters_R_R();
    bool TestConstructor_TwoParameters_R_W();
    bool TestConstructor_TwoParameters_W_C();
    bool TestConstructor_TwoParameters_W_R();
    bool TestConstructor_TwoParameters_W_W();
    /**
     * @brief Tests the Constructor with 2 parameters that are pointers (a feature that is not supported).
     */
    bool TestConstructor_TwoParameters_Pointers();

    /**
     * @brief Tests the Constructor with 3 parameters in all the 27 possible combinations
     */
    bool TestConstructor_ThreeParameters_C_C_C();
    bool TestConstructor_ThreeParameters_C_C_R();
    bool TestConstructor_ThreeParameters_C_C_W();
    bool TestConstructor_ThreeParameters_C_R_C();
    bool TestConstructor_ThreeParameters_C_R_R();
    bool TestConstructor_ThreeParameters_C_R_W();
    bool TestConstructor_ThreeParameters_C_W_C();
    bool TestConstructor_ThreeParameters_C_W_R();
    bool TestConstructor_ThreeParameters_C_W_W();
    bool TestConstructor_ThreeParameters_R_C_C();
    bool TestConstructor_ThreeParameters_R_C_R();
    bool TestConstructor_ThreeParameters_R_C_W();
    bool TestConstructor_ThreeParameters_R_R_C();
    bool TestConstructor_ThreeParameters_R_R_R();
    bool TestConstructor_ThreeParameters_R_R_W();
    bool TestConstructor_ThreeParameters_R_W_C();
    bool TestConstructor_ThreeParameters_R_W_R();
    bool TestConstructor_ThreeParameters_R_W_W();
    bool TestConstructor_ThreeParameters_W_C_C();
    bool TestConstructor_ThreeParameters_W_C_R();
    bool TestConstructor_ThreeParameters_W_C_W();
    bool TestConstructor_ThreeParameters_W_R_C();
    bool TestConstructor_ThreeParameters_W_R_R();
    bool TestConstructor_ThreeParameters_W_R_W();
    bool TestConstructor_ThreeParameters_W_W_C();
    bool TestConstructor_ThreeParameters_W_W_R();
    bool TestConstructor_ThreeParameters_W_W_W();

    /**
     * @brief Tests the Constructor with 4 parameters in all the 8 possible combinations
     */
    bool TestConstructor_FourParameters_C_C_C_C();
    bool TestConstructor_FourParameters_C_C_C_R();
    bool TestConstructor_FourParameters_C_C_C_W();
    bool TestConstructor_FourParameters_C_C_R_C();
    bool TestConstructor_FourParameters_C_C_R_R();
    bool TestConstructor_FourParameters_C_C_R_W();
    bool TestConstructor_FourParameters_C_C_W_C();
    bool TestConstructor_FourParameters_C_C_W_R();
    bool TestConstructor_FourParameters_C_C_W_W();
    bool TestConstructor_FourParameters_C_R_C_C();
    bool TestConstructor_FourParameters_C_R_C_R();
    bool TestConstructor_FourParameters_C_R_C_W();
    bool TestConstructor_FourParameters_C_R_R_C();
    bool TestConstructor_FourParameters_C_R_R_R();
    bool TestConstructor_FourParameters_C_R_R_W();
    bool TestConstructor_FourParameters_C_R_W_C();
    bool TestConstructor_FourParameters_C_R_W_R();
    bool TestConstructor_FourParameters_C_R_W_W();
    bool TestConstructor_FourParameters_C_W_C_C();
    bool TestConstructor_FourParameters_C_W_C_R();
    bool TestConstructor_FourParameters_C_W_C_W();
    bool TestConstructor_FourParameters_C_W_R_C();
    bool TestConstructor_FourParameters_C_W_R_R();
    bool TestConstructor_FourParameters_C_W_R_W();
    bool TestConstructor_FourParameters_C_W_W_C();
    bool TestConstructor_FourParameters_C_W_W_R();
    bool TestConstructor_FourParameters_C_W_W_W();
    bool TestConstructor_FourParameters_R_C_C_C();
    bool TestConstructor_FourParameters_R_C_C_R();
    bool TestConstructor_FourParameters_R_C_C_W();
    bool TestConstructor_FourParameters_R_C_R_C();
    bool TestConstructor_FourParameters_R_C_R_R();
    bool TestConstructor_FourParameters_R_C_R_W();
    bool TestConstructor_FourParameters_R_C_W_C();
    bool TestConstructor_FourParameters_R_C_W_R();
    bool TestConstructor_FourParameters_R_C_W_W();
    bool TestConstructor_FourParameters_R_R_C_C();
    bool TestConstructor_FourParameters_R_R_C_R();
    bool TestConstructor_FourParameters_R_R_C_W();
    bool TestConstructor_FourParameters_R_R_R_C();
    bool TestConstructor_FourParameters_R_R_R_R();
    bool TestConstructor_FourParameters_R_R_R_W();
    bool TestConstructor_FourParameters_R_R_W_C();
    bool TestConstructor_FourParameters_R_R_W_R();
    bool TestConstructor_FourParameters_R_R_W_W();
    bool TestConstructor_FourParameters_R_W_C_C();
    bool TestConstructor_FourParameters_R_W_C_R();
    bool TestConstructor_FourParameters_R_W_C_W();
    bool TestConstructor_FourParameters_R_W_R_C();
    bool TestConstructor_FourParameters_R_W_R_R();
    bool TestConstructor_FourParameters_R_W_R_W();
    bool TestConstructor_FourParameters_R_W_W_C();
    bool TestConstructor_FourParameters_R_W_W_R();
    bool TestConstructor_FourParameters_R_W_W_W();
    bool TestConstructor_FourParameters_W_C_C_C();
    bool TestConstructor_FourParameters_W_C_C_R();
    bool TestConstructor_FourParameters_W_C_C_W();
    bool TestConstructor_FourParameters_W_C_R_C();
    bool TestConstructor_FourParameters_W_C_R_R();
    bool TestConstructor_FourParameters_W_C_R_W();
    bool TestConstructor_FourParameters_W_C_W_C();
    bool TestConstructor_FourParameters_W_C_W_R();
    bool TestConstructor_FourParameters_W_C_W_W();
    bool TestConstructor_FourParameters_W_R_C_C();
    bool TestConstructor_FourParameters_W_R_C_R();
    bool TestConstructor_FourParameters_W_R_C_W();
    bool TestConstructor_FourParameters_W_R_R_C();
    bool TestConstructor_FourParameters_W_R_R_R();
    bool TestConstructor_FourParameters_W_R_R_W();
    bool TestConstructor_FourParameters_W_R_W_C();
    bool TestConstructor_FourParameters_W_R_W_R();
    bool TestConstructor_FourParameters_W_R_W_W();
    bool TestConstructor_FourParameters_W_W_C_C();
    bool TestConstructor_FourParameters_W_W_C_R();
    bool TestConstructor_FourParameters_W_W_C_W();
    bool TestConstructor_FourParameters_W_W_R_C();
    bool TestConstructor_FourParameters_W_W_R_R();
    bool TestConstructor_FourParameters_W_W_R_W();
    bool TestConstructor_FourParameters_W_W_W_C();
    bool TestConstructor_FourParameters_W_W_W_R();
    bool TestConstructor_FourParameters_W_W_W_W();

    /**
     * @brief Tests the GetMethod function
     */
    bool TestGetMethodCaller();

    /**
     * @brief Tests the GetMethodName function
     */
    bool TestGetMethodName();

    /**
     * @brief Tests the SetMethodName function
     */
    bool TestSetMethodName();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSMETHODINTERFACEMAPPERTEST_H_ */

