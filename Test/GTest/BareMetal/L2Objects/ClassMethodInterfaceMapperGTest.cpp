/**
 * @file ClassMethodInterfaceMapperGTest.cpp
 * @brief Source file for class ClassMethodInterfaceMapperGTest
 * @date 25/05/2016
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

 * @details This source file contains the definition of all the methods for
 * the class ClassMethodInterfaceMapperGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "ClassMethodInterfaceMapperTest.h"
#include "MARTe2UTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestGetMethod) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestGetMethodCaller());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestGetMethodName) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestGetMethodName());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestSetMethodName) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestSetMethodName());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_OneParameter_Pointer) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_OneParameter_Pointer());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_VoidParameters) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_VoidParameters());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperTest,TestConstructor_OneParameter_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_OneParameter_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperTest,TestConstructor_OneParameter_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_OneParameter_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperTest,TestConstructor_OneParameter_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_OneParameter_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_TwoParameters_C_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_TwoParameters_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_TwoParameters_C_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_TwoParameters_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_TwoParameters_C_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_TwoParameters_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_TwoParameters_R_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_TwoParameters_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_TwoParameters_R_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_TwoParameters_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_TwoParameters_R_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_TwoParameters_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_TwoParameters_W_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_TwoParameters_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_TwoParameters_W_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_TwoParameters_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_TwoParameters_W_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_TwoParameters_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_TwoParameters_Pointers) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_TwoParameters_Pointers());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_C_C_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_C_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_C_C_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_C_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_C_C_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_C_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_C_R_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_C_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_C_R_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_C_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_C_R_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_C_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_C_W_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_C_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_C_W_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_C_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_C_W_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_C_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_R_C_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_R_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_R_C_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_R_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_R_C_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_R_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_R_R_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_R_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_R_R_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_R_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_R_R_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_R_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_R_W_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_R_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_R_W_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_R_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_R_W_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_R_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_W_C_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_W_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_W_C_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_W_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_W_C_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_W_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_W_R_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_W_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_W_R_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_W_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_W_R_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_W_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_W_W_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_W_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_W_W_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_W_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_ThreeParameters_W_W_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_ThreeParameters_W_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_C_C_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_C_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_C_C_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_C_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_C_C_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_C_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_C_R_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_C_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_C_R_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_C_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_C_R_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_C_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_C_W_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_C_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_C_W_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_C_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_C_W_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_C_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_R_C_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_R_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_R_C_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_R_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_R_C_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_R_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_R_R_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_R_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_R_R_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_R_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_R_R_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_R_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_R_W_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_R_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_R_W_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_R_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_R_W_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_R_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_W_C_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_W_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_W_C_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_W_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_W_C_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_W_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_W_R_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_W_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_W_R_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_W_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_W_R_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_W_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_W_W_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_W_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_W_W_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_W_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_C_W_W_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_C_W_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_C_C_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_C_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_C_C_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_C_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_C_C_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_C_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_C_R_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_C_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_C_R_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_C_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_C_R_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_C_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_C_W_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_C_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_C_W_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_C_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_C_W_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_C_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_R_C_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_R_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_R_C_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_R_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_R_C_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_R_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_R_R_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_R_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_R_R_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_R_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_R_R_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_R_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_R_W_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_R_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_R_W_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_R_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_R_W_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_R_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_W_C_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_W_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_W_C_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_W_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_W_C_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_W_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_W_R_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_W_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_W_R_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_W_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_W_R_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_W_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_W_W_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_W_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_W_W_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_W_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_R_W_W_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_R_W_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_C_C_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_C_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_C_C_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_C_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_C_C_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_C_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_C_R_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_C_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_C_R_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_C_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_C_R_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_C_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_C_W_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_C_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_C_W_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_C_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_C_W_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_C_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_R_C_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_R_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_R_C_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_R_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_R_C_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_R_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_R_R_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_R_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_R_R_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_R_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_R_R_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_R_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_R_W_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_R_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_R_W_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_R_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_R_W_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_R_W_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_W_C_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_W_C_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_W_C_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_W_C_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_W_C_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_W_C_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_W_R_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_W_R_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_W_R_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_W_R_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_W_R_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_W_R_W());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_W_W_C) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_W_W_C());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_W_W_R) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_W_W_R());
}

TEST(BareMetal_L2Objects_ClassMethodInterfaceMapperGTest,TestConstructor_FourParameters_W_W_W_W) {
    ClassMethodInterfaceMapperTest target;
    ASSERT_TRUE(target.TestConstructor_FourParameters_W_W_W_W());
}

