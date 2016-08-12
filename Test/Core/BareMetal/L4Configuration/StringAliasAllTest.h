/**
 * @file StringAliasAllTest.h
 * @brief Header file for class StringAliasAllTest
 * @date 12/ago/2016
 * @author pc
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

 * @details This header file contains the declaration of the class StringAliasAllTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STRINGALIASALLTEST_H_
#define STRINGALIASALLTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StringAliasAll.h"
#include "StandardParser.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief Utility for Validate tests
 */
template<typename T>
struct StringAliasAllTestTable {
    const char8 * attributes;
    const char8 * source;
    T dest;
    bool expected;
};

/**
 * @brief Utility for Validate tests
 */
template<typename T, uint32 nElements>
struct StringAliasAllTestTableVector {
    const char8 * attributes;
    const char8 * source;
    T dest[nElements];
    bool expected;
};

/**
 * @brief Utility for Validate tests
 */
template<typename T, uint32 nRows, uint32 nCols>
struct StringAliasAllTestTableMatrix {
    const char8 * attributes;
    const char8 * source;
    T dest[nRows][nCols];
    bool expected;
};

class StringAliasAllTest {
public:

    /**
     * @brief Tests the Validate with a scalar AnyType
     */
    template<typename T>
    bool TestSourceToDestination(const StringAliasAllTestTable<T>* table);

    /**
     * @brief Tests the Validate with a scalar AnyType
     */
    template<typename T, uint32 nElements>
    bool TestSourceToDestinationVector(const StringAliasAllTestTableVector<T, nElements>* table);


};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename T>
bool StringAliasAllTest::TestSourceToDestination(const StringAliasAllTestTable<T>* table) {

    uint32 i = 0;

    while (table[i].attributes != NULL) {
        StringAliasAll aliasMaker;
        StreamString conf = table[i].attributes;
        conf.Seek(0ull);
        ConfigurationDatabase cdb;
        StandardParser parser(conf, cdb);
        parser.Parse();
        cdb.MoveToRoot();
        printf("#children = %d", cdb.GetNumberOfChildren());
        if (!aliasMaker.Initialise(cdb)) {
            return false;
        }

        AnyType source(table[i].source);
        AnyObject valAO;
        if (aliasMaker.SourceToDestination(source, valAO) != table[i].expected) {
            printf("\n%d\n", i);
            return false;
        }
        if (table[i].expected) {
            T val;
            TypeConvert(val, valAO.GetType());
            T dest = table[i].dest;
            if (dest != val) {
                return false;
            }
        }

        i++;
    }

    return true;
}

template<typename T, uint32 nElements>
bool StringAliasAllTest::TestSourceToDestinationVector(const StringAliasAllTestTableVector<T, nElements>* table) {

    uint32 i = 0;

    while (table[i].attributes != NULL) {
        StringAliasAll aliasMaker;
        StreamString conf = table[i].attributes;
        conf.Seek(0ull);
        ConfigurationDatabase cdb;
        StandardParser parser(conf, cdb);
        parser.Parse();
        cdb.MoveToRoot();
        printf("#children = %d", cdb.GetNumberOfChildren());
        if (!aliasMaker.Initialise(cdb)) {
            return false;
        }

        AnyType source(table[i].source);
        AnyObject valAO;
        if (aliasMaker.SourceToDestination(source, valAO) != table[i].expected) {
            printf("\n%d\n", i);
            return false;
        }
        if (table[i].expected) {
            T val[nElements];
            TypeConvert(val, valAO.GetType());

            for (uint32 j = 0u; j < nElements; j++) {
                T dest = table[i].dest[j];
                if (dest != val[j]) {
                    return false;
                }
            }
        }

        i++;
    }


    return true;
}

#endif /* STRINGALIASALLTEST_H_ */

