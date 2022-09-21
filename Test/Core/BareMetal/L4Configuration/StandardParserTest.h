/**
 * @file StandardParserTest.h
 * @brief Header file for class StandardParserTest
 * @date 09/12/2015
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

 * @details This header file contains the declaration of the class StandardParserTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STANDARDPARSERTEST_H_
#define STANDARDPARSERTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include "StandardParser.h"
#include "ConfigurationDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief A table used for type cast parsing tests on scalars.
 */
template<typename T>
struct TypeCastTableTest {
    const char8 *configString;
    const char8 *varName;
    T expectedResult;
    bool expectedRet;
};

/**
 * @brief A table used for type cast parsing tests on vectors.
 */
template<typename T, uint32 nElements>
struct TypeCastTableTestVector {
    const char8 *configString;
    const char8 *varName;
    T expectedResult[nElements];
    bool expectedRet;
};

/**
 * @brief A table used for type cast parsing tests on matrices.
 */
template<typename T, uint32 nRows, uint32 nCols>
struct TypeCastTableTestMatrix {
    const char8 *configString;
    const char8 *varName;
    T expectedResult[nRows][nCols];
    bool expectedRet;
};


class StandardParserTest {

public:
    /**
     * @brief Tests the constructor.
     */
    bool TestConstructor();

    /**
     * @brief Tests if the function returns the Json grammar.
     */
    bool TestGetGrammarInfo();

    /**
     * @brief Tests the parsing of scalar variables.
     */
    bool TestParseScalar();

    /**
     * @brief Tests the parsing of strings.
     */
    bool TestParseString();

    /**
     * @brief Tests the parsing of vector variables.
     */
    bool TestParseVector();

    /**
     * @brief Tests the parsing of matrix variables.
     */
    bool TestParseMatrix();

    /**
     * @brief Tests the parsing of an input with a big number of blocks.
     */
    bool TestNestedBlocks();

    /**
     * @brief Tests if the Parse function returns false in case of errors in the input stream.
     */
    bool TestParseErrors(const char8 *configStringIn);

    /**
     * @brief Tests if passing a bad type in the type cast expression the automatic cast to string will be used.
     */
    bool TestStandardCast();

    /**
     * @brief Test the parsing of math expressions.
     */
    bool TestGetExprCast();

    /**
     * @brief Test the parsing of math expressions with an invalid expression.
     */
    bool TestGetExprCast_InvalidExpression();

    /**
     * @brief Test the parsing of math expressions with an invalid output type.
     */
    bool TestGetExprCast_InvalidOutput();

    /**
     * @brief Test the parsing of math expressions forcing a compilation error.
     */
    bool TestGetExprCast_FailCompile();

    /**
     * @brief Test the parsing of math expressions with a variable that does not exist.
     */
    bool TestGetExprCast_FailVariableNotFound();

    /**
     * @brief Test the parsing of math expressions with a variable path that does not exist.
     */
    bool TestGetExprCast_FailVariablePathNotFound();

    /**
     * @brief Tests the type cast parsing with scalar variables.
     */
    template<typename T>
    bool TestTypeCast_Scalar(const TypeCastTableTest<T> *table);

    /**
     * @brief Tests the type cast parsing with vector variables.
     */
    template<typename T, uint32 nElements>
    bool TestTypeCast_Vector(const TypeCastTableTestVector<T, nElements> *table);

    /**
     * @brief Tests the type cast parsing with matrix variables.
     */
    template<typename T, uint32 nRows, uint32 nCols>
    bool TestTypeCast_Matrix(const TypeCastTableTestMatrix<T, nRows, nCols> *table);
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/



template<typename T>
bool StandardParserTest::TestTypeCast_Scalar(const TypeCastTableTest<T> *table) {
    ConfigurationDatabase database;
    StreamString errors;

    uint32 i = 0;
    while (table[i].configString != NULL) {
        StreamString configString = table[i].configString;

        configString.Seek(0);
        StandardParser myParser(configString, database, &errors);
        bool ret = myParser.Parse();

        if (table[i].expectedRet != ret) {
            //printf("failed ret %d\n", i);
            return false;
        }
        if (table[i].expectedRet) {

            T output;
            if (!database.Read(table[i].varName, output)) {
                //printf("failed read %d\n", i);
                return false;
            }
            if (output != table[i].expectedResult) {
                //printf("\nfailed result %d\n", i);
                return false;
            }
        }
        else {
            //printf("\nerrors=%s, %d\n", errors.Buffer(), i);
        }

        i++;
    }
    return true;
}

template<typename T, uint32 nElements>
bool StandardParserTest::TestTypeCast_Vector(const TypeCastTableTestVector<T, nElements> *table) {
    ConfigurationDatabase database;
    StreamString errors;

    uint32 i = 0;
    while (table[i].configString != NULL) {
        StreamString configString = table[i].configString;

        configString.Seek(0);
        StandardParser myParser(configString, database, &errors);
        bool ret = myParser.Parse();

        if (table[i].expectedRet != ret) {
            //printf("failed ret %d\n", i);
            return false;
        }
        if (table[i].expectedRet) {

            T output[nElements];
            if (!database.Read(table[i].varName, output)) {
                //printf("failed read %d\n", i);
                return false;
            }
            for (uint32 h = 0; h < nElements; h++) {
                if (table[i].expectedResult[h] != output[h]) {
                    //printf("\nfailed result %d %d\n", i, h);
                    return false;
                }
            }

        }
        else {
            //printf("\nerrors=%s, %d\n", errors.Buffer(), i);
        }

        i++;
    }
    return true;
}

template<typename T, uint32 nRows, uint32 nCols>
bool StandardParserTest::TestTypeCast_Matrix(const TypeCastTableTestMatrix<T, nRows, nCols> *table) {
    ConfigurationDatabase database;
    StreamString errors;

    uint32 i = 0;
    while (table[i].configString != NULL) {
        StreamString configString = table[i].configString;

        configString.Seek(0);
        StandardParser myParser(configString, database, &errors);
        bool ret = myParser.Parse();

        if (table[i].expectedRet != ret) {
            //printf("failed ret %d\n", i);
            return false;
        }
        if (table[i].expectedRet) {

            T output[nRows][nCols];
            if (!database.Read(table[i].varName, output)) {
                //printf("failed read %d\n", i);
                return false;
            }
            for (uint32 h = 0; h < nRows; h++) {
                for (uint32 k = 0; k < nCols; k++) {
                    if (table[i].expectedResult[h][k] != output[h][k]) {
                        //printf("\nfailed result %d %d %d\n", i, h, k);
                        return false;
                    }
                }
            }

        }
        else {
            //printf("\nerrors=%s, %d\n", errors.Buffer(), i);
        }

        i++;
    }
    return true;
}


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* STANDARDPARSERTEST_H_ */

