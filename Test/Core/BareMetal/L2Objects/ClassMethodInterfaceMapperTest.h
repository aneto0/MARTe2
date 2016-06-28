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

class ClassMethodInterfaceMapperTest {
//TODO Add the macro DLL_API to the class declaration (i.e. class DLL_API ClassMethodCallerTest)
public:
    ClassMethodInterfaceMapperTest();
    virtual ~ClassMethodInterfaceMapperTest();
    bool TestDefaultConstructor();
    bool TestConstructorForMethodWithNoArguments();
    bool TestConstructorForMethodWith1ArgumentByCopy();
    bool TestConstructorForMethodWith1ArgumentByRef();
    bool TestCall();
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSMETHODINTERFACEMAPPERTEST_H_ */

