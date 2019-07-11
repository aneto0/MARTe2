/**
 * @file AnyTypeTest.h
 * @brief Header file for class AnyTypeTest
 * @date 31/08/2015
 * @author Llorenç Capellà
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

 * @details This header file contains the declaration of the class AnyTypeTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TEST_CORE_L0TYPEDEV_ANYTYPETEST_H_
#define TEST_CORE_L0TYPEDEV_ANYTYPETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AnyType.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace AnyTypeTest {


    /**
     * @brief Generic type to Anytype conversion test
     */
    template<typename T>
    inline bool ATVerify(T initValue,const TypeDescriptor &expectedTD,CCString modifiers=emptyString);

    /**
     * @brief Test AnyType default constructor
     * @return true if GetDataPointer() == NULL && GetBitAddress() == 0 && GetTypeDescriptor() == {false, false, 0, 0}
     */
    bool TestAnyType_Void();

    /**
     * @brief Test AnyType constructor using anyType as an input.
     * @return true if GetDataPointer() == NULL && GetBitAddress() == 0 && GetTypeDescriptor() == {false, false, 0, 0}
     */
    bool TestAnyType_AnyTypeVoid();


    /**
     * @brief Tests the AnyType constructor with an Object variable.
     */
    bool TestAnyType_Object();

    /**
     * @brief Tests the AnyType constructor with a const Object variable.
     */
    bool TestAnyType_ConstObject();

   /**
     * @brief Tests if the function sets the data pointer.
     */
    bool TestSetDataPointer();

    /**
     * @brief Tests if the function returns the data pointer.
     */
    bool TestGetDataPointer();

    /**
     * @brief Tests if the function returns the type descriptor.
     */
    bool TestGetTypeDescriptor();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template <typename T>
bool AnyTypeTest::ATVerify(T initValue,const TypeDescriptor &expectedTD,CCString modifiers){
	bool retVal=true;
	T variable = initValue;
	AnyType anytype(variable);
    const VariableDescriptor &vd= anytype.GetFullVariableDescriptor();
    const TypeDescriptor     &td= vd.GetFinalTypeDescriptor();
	retVal = (anytype.GetVariablePointer() == &variable);
	if (modifiers.GetSize()>0){
		retVal &= (modifiers.IsSameAs(vd.GetModifiers()));
	}
	retVal &= (td.SameAs(expectedTD));
	return retVal;
}

#endif /* TEST_CORE_L0TYPEDEV_ANYTYPETEST_H_ */

