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
     * @return true if the name and version fields are initialised to NULL.
     */
    bool TestDefaultConstructor();

    /**
     * @brief Tests the full constructor.
     * @param[in] name the desired name.
     * @param[in] name the desired typeidName.
     * @param[in] version the desired version.
     * @return true if the attributes are set equal to inputs.
     */
    bool TestFullConstructor(const char8 *name,
                             const char8 *typeidName,
                             const char8* version);

    /**
     * @brief Tests the ClassProperties::GetName function.
     * @param[in] the desired name.
     * @return true if the name returned is correct.
     */
    bool TestGetName(const char8*name);

    /**
     * @brief Tests the ClassProperties::GetVersion function.
     * @param[in] the desired version.
     * @return true if the version returned is correct.
     */
    bool TestGetVersion(const char8*version);

    /**
     * @brief Tests the ClassProperties::GetUniqueIdentifier function.
     * @param uniqueIdentifier the unique identifier value to test.
     * @return true if after calling ClassProperties::SetUniqueIdentifier(\a uniqueIdentifier), ClassProperties::GetUniqueIdentifier() == \a uniqueIdentifier;
     */
    bool TestGetUniqueIdentifier(uint32 uniqueIdentifier);

    /**
     * @brief Tests the ClassProperties::SetUniqueIdentifier function.
     * @param uniqueIdentifier the unique identifier value to test.
     * @return true if after calling ClassProperties::SetUniqueIdentifier(\a uniqueIdentifier), ClassProperties::GetUniqueIdentifier() == \a uniqueIdentifier;
     */
    bool TestSetUniqueIdentifier(uint32 uniqueIdentifier);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSPROPERTIESTEST_H_ */

