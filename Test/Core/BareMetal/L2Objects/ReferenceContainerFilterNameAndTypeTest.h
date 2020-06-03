/**
 * @file ReferenceContainerFilterNameAndTypeTest.h
 * @brief Header file for class ReferenceContainerFilterNameAndTypeTest
 * @date 27/08/2018
 * @author Giuseppe Ferro
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

 * @details This header file contains the declaration of the class ReferenceContainerFilterNameAndTypeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REFERENCECONTAINERFILTERNAMEANDTYPETEST_H_
#define REFERENCECONTAINERFILTERNAMEANDTYPETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainerFilterNameAndType.h"
using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief Tests the ReferenceContainerFilterNameAndType public methods.
 */
class ReferenceContainerFilterNameAndTypeTest {
public:
    /**
     * @brief Constructor. NOOP.
     */
    ReferenceContainerFilterNameAndTypeTest();
    
    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ReferenceContainerFilterNameAndTypeTest();
    
    /**
     * @brief Tests the default constructor.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the set constructor.
     */
    bool TestSetConstructor();

    /**
     * @brief Tests the copy constructor.
     */
    bool TestCopyConstructor();
    
    /**
     * @brief Tests the Test method.
     */
    bool TestTest();

    /**
     * @brief Tests the TestGetRemainedAddrIndex method.
     */
    bool TestGetRemainedAddrIndex();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REFERENCECONTAINERFILTERNAMEANDTYPETEST_H_ */

