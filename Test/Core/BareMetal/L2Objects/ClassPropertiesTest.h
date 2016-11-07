/**
 * @file ClassPropertiesTest.h
 * @brief Header file for class ClassPropertiesTest
 * @date 11/08/2015
 * @author Giuseppe Ferrò
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

 * @details This header file contains the declaration of the class ClassPropertiesTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSPROPERTIESTEST_H_
#define CLASSPROPERTIESTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests all the ClassProperties functions.
 */
#include "ClassProperties.h"
using namespace MARTe;

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Tests all the ClassProperties functions.
 */
class ClassPropertiesTest {
public:

    /**
     * @brief Tests the default constructor.
     * @return True if the name and version fields are initialised to NULL.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the full constructor.
     * @param[in] name the desired name of the class.
     * @param[in] typeidName he name of the class as returned by typeid.
     * @param[in] version the version of the class.
     * @return True if the attributes are set equal to inputs.
     */
    bool TestFullConstructor(const char8 * name,
                             const char8 * typeidName,
                             const char8 * version);

    /**
     * @brief Tests the ClassProperties::GetName function.
     * @param[in] name the desired name of the class.
     * @return True if the name returned is correct.
     */
    bool TestGetName(const char8* name);

    /**
     * @brief Test the ClassProperties::GetVersion function.
     * @param[in] version the desired version of the class.
     * @return True if the version returned is equal to the input version.
     */
    bool TestGetVersion(const char8 * version);

#if 0
    /**
     * @brief Tests the ClassProperties::GetUniqueIdentifier function.
     * @param[in] UniqueIdentifier the unique identifier value to test.
     * @return True if after calling ClassProperties::SetUniqueIdentifier(\a uniqueIdentifier), ClassProperties::GetUniqueIdentifier() == \a uniqueIdentifier;
     */
    bool TestGetUniqueIdentifier(uint32 uniqueIdentifier);

    /**
     * @brief Tests the ClassProperties::SetUniqueIdentifier function.
     * @param[in] uniqueIdentifier the unique identifier value to test.
     * @return True if after calling ClassProperties::SetUniqueIdentifier(\a uniqueIdentifier), ClassProperties::GetUniqueIdentifier() == \a uniqueIdentifier;
     */
    bool TestSetUniqueIdentifier(uint32 uniqueIdentifier);
#endif

    /**
     * @brief Tests the ClassProperties::GetTypeDescriptor function.
     * @param[in] td the TypeDescriptor value to test.
     * @return True if after calling ClassProperties::SetTypeDescriptor(\a td), ClassProperties::GetTypeDescriptor() == \a td;
     */
    bool TestGetTypeDescriptor(const TypeDescriptor &td);

    /**
     * @brief Tests the ClassProperties::SetTypeDescriptor function.
     * @param[in] td the TypeDescriptor value to test.
     * @return True if after calling ClassProperties::SetTypeDescriptor(\a td), ClassProperties::GetTypeDescriptor() == \a td;
     */
    bool TestSetTypeDescriptor(const TypeDescriptor &td);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSPROPERTIESTEST_H_ */

