/**
 * @file IntrospectionEntryTest.h
 * @brief Header file for class IntrospectionEntryTest
 * @date 20/01/2016
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

 * @details This header file contains the declaration of the class IntrospectionEntryTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTROSPECTIONENTRYTEST_H_
#define INTROSPECTIONENTRYTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "IntrospectionEntry.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Utility struct for IntrospectionEntry tests.
 */
struct IntrospectionEntryTestTable{
    const char8* modifiers;
    uint32 level;
    bool expected;
};


/**
 * @brief Tests all the IntrospectionEntry functions.
 */
class IntrospectionEntryTest {
public:

    /**
     * @brief Tests if the class members are initialized correctly.
     */
    bool TestConstructor(const char8* memberName, const char8 *type, const char8 *modifiers, const char8 *attributes, uint32 size, uint23 index);

    /**
     * @brief Checks if the function returns the member name set by constructor.
     */
    bool TestGetMemberName(const char8* memberName);

    /**
     * @brief Checks if the function returns the modifiers string set by constructor.
     */
    bool TestGetMemberModifiers(const char8* modifiers);

    /**
     * @brief Checks if the function returns the type name set by constructor.
     */
    bool TestGetMemberTypeName(const char8* typeName);

    /**
     * @brief Checks if the function returns the member size set by constructor.
     */
    bool TestGetMemberSize(uint32 size);

    /**
     * @brief Checks if the function returns the member index set by constructor.
     */
    bool TestGetMemberByteOffset(uint32 byteOffset);

    /**
     * @brief Checks if the function returns the TypeDescriptor related to the type name set by constructor.
     */
    bool TestGetMemberTypeDescriptor();

    /**
     * @brief Checks if the function returns the member attributes.
     */
    bool TestGetMemberAttributes(const char8 * attributes);

    /**
     * @brief Checks if the function returns true if the member type is constant and false otherwise.
     */
    bool TestIsConstant(const IntrospectionEntryTestTable *table);

    /**
     * @brief Checks if the function returns correctly the member pointer level.
     */
    bool TestGetMemberPointerLevel(const IntrospectionEntryTestTable *table);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTROSPECTIONENTRYTEST_H_ */

