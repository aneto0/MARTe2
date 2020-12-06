/**
 * @file TypeConversionTest.h
 * @brief Header file for class TypeConversionTest
 * @date 12/11/2015
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

 * @details This header file contains the declaration of the class TypeConversionTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TYPECONVERSIONTEST_H_
#define TYPECONVERSIONTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "TypeConversion.h"
#include "StreamString.h"
#include "stdio.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

/**
 * @brief A table node used to test the TypeConvert function between basic elements.
 */
template<typename T1, typename T2>
struct TypeToTypeTableTest {
    T1 typeToConvert;
    T2 result;
    bool go;
    bool expected;
};

/**
 * @brief A table node used to test the TypeConvert function between vectors.
 */
template<typename T1, typename T2, uint32 nElements>
struct TypeToTypeVectorTableTest {
    T1 typeToConvert[nElements];
    T2 result[nElements];
    bool go;
};

/**
 * @brief A table node used to test the TypeConvert function between matrices.
 */
template<typename T1, typename T2, uint32 nRows, uint32 nCols>
struct TypeToTypeMatrixTableTest {
    T1 typeToConvert[nRows][nCols];
    T2 result[nRows][nCols];
    bool go;
};

/**
 * @brief Tests all the TypeConversion functions.
 */
class TypeConversionTest {
public:

    /**
     * @brief Tests the type conversion between scalar types.
     */
    template<typename T1, typename T2>
    bool TestTypeConvert(const TypeToTypeTableTest<T1, T2>* table);

    /**
     * @brief Tests the type conversion between scalar types in input and a char* pointing to a C-String heap memory in output.
     */
    template<typename T1>
    bool TestTypeConvertToCCString(const TypeToTypeTableTest<T1, const char8*>* table);

    /**
     * @brief Tests the type conversion between scalar types in input and a char[] in output.
     */
    template<typename T1>
    bool TestTypeConvertCArray(const TypeToTypeTableTest<T1, const char8 *>* table);

    /**
     * @brief Tests the type conversion between char[] in input and scalar types in output.
     */
    template<typename T2>
    bool TestTypeConvertFromCArray(const TypeToTypeTableTest<const char8 *, T2>* table);

    /**
     * @brief Tests the type conversion between scalar types in input and a Vector<char> in output.
     */
    template<typename T1>
    bool TestTypeConvertToCharVector(const TypeToTypeTableTest<T1, const char8 *>* table);

    /**
     * @brief Tests the type conversion between Vector<char> in input and scalar types in output.
     */
    template<typename T2>
    bool TestTypeConvertFromCharVector(const TypeToTypeTableTest<const char8 *, T2>* table);

    /**
     * @brief Tests the type conversion between vectors.
     */
    template<typename T1, typename T2, uint32 nElements>
    bool TestTypeConvertVector(const TypeToTypeVectorTableTest<T1, T2, nElements>* table);

    /**
     * @brief Tests the type conversion between vectors in input and a char[][] in output.
     */
    template<typename T1, uint32 nElements>
    bool TestTypeConvertVectorCArray(const TypeToTypeVectorTableTest<T1, const char8*, nElements>* table);

    /**
     * @brief Tests the type conversion between char[][] in input and vectors in output.
     */
    template<typename T2, uint32 nElements>
    bool TestTypeConvertVectorFromCArray(const TypeToTypeVectorTableTest<const char8*, T2, nElements>* table);

    /**
     * @brief Tests the type conversion between vectors in input and a Matrix<char> in output.
     */
    template<typename T1, uint32 nElements>
    bool TestTypeConvertVectorToCharMatrix(const TypeToTypeVectorTableTest<T1, const char8*, nElements>* table);

    /**
     * @brief Tests the type conversion between Matrix<char> in input and vectors in output.
     */
    template<typename T2, uint32 nElements>
    bool TestTypeConvertVectorFromCharMatrix(const TypeToTypeVectorTableTest<const char8*, T2, nElements>* table);

    /**
     * @brief Tests the type conversion between vectors in input and a Vector<char*> in output.
     */
    template<typename T1, uint32 nElements>
    bool TestTypeConvertVectorToCharStarVector(const TypeToTypeVectorTableTest<T1, const char8*, nElements>* table);

    /**
     * @brief Tests the type conversion from a static matrix to a static matrix.
     */
    template<typename T1, typename T2, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixStaticToStatic(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table);

    /**
     * @brief Tests the type conversion from a static matrix to a heap matrix.
     */
    template<typename T1, typename T2, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixStaticToHeap(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table);

    /**
     * @brief Tests the type conversion from a heap matrix to a static matrix.
     */
    template<typename T1, typename T2, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixHeapToStatic(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table);

    /**
     * @brief Tests the type conversion from a heap matrix to a heap matrix.
     */
    template<typename T1, typename T2, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixHeapToHeap(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table);

    /**
     * @brief Tests the type conversion from a static matrix to a char[][][].
     */
    template<typename T1, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixStaticToStaticCArray(const TypeToTypeMatrixTableTest<T1, const char8*, nRows, nCols>* table);

    /**
     * @brief Tests the type conversion from a char[][][] to static matrix.
     */
    template<typename T2, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixStaticToStaticFromCArray(const TypeToTypeMatrixTableTest<const char8*, T2, nRows, nCols>* table);

    /**
     * @brief Tests the type conversion from a heap matrix to a char[][][].
     */
    template<typename T1, uint32 nElements>
    bool TestTypeConvertVectorStaticToHeapMatrixCArray(const TypeToTypeVectorTableTest<T1, const char8*, nElements>* table);

    /**
     * @brief Tests the type conversion from a char[][][] to heap matrix.
     */
    template<typename T2, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixStaticToHeapFromCArray(const TypeToTypeMatrixTableTest<const char8*, T2, nRows, nCols>* table);

    /**
     * @brief Tests the type conversion from a static matrix to a Matrix<char*> on heap.
     */
    template<typename T1, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixStaticToHeapCCString(const TypeToTypeMatrixTableTest<T1, const char8*, nRows, nCols>* table);

    /**
     * @brief Tests the type conversion from a static matrix to a static Matrix<char*>.
     */
    template<typename T1, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixStaticToStaticCCString(const TypeToTypeMatrixTableTest<T1, const char8*, nRows, nCols>* table);

    /**
     * @brief Tests the type conversion from a heap matrix to a Matrix<char*> on heap.
     */
    template<typename T1, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixHeapToHeapCCString(const TypeToTypeMatrixTableTest<T1, const char8*, nRows, nCols>* table);

    /**
     * @brief Tests the type conversion from a heap matrix to a static Matrix<char*>.
     */
    template<typename T1, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixHeapToStaticCCString(const TypeToTypeMatrixTableTest<T1, const char8*, nRows, nCols>* table);

    /**
     * @brief Tests the type conversion from a scalar pointer
     */
    template<typename T1, typename T2>
    bool TestTypeConvertPointer(const TypeToTypeTableTest<T1, T2>* table);

    /**
     * @brief Tests the type conversion from a vector of pointers
     */
    template<typename T1, typename T2, uint32 nElements>
    bool TestTypeConvertPointerVector(const TypeToTypeVectorTableTest<T1, T2, nElements>* table);

    /**
     * @brief Tests the type conversion from a matrix of pointers
     */
    template<typename T1, typename T2, uint32 nRows, uint32 nCols>
    bool TestTypeConvertPointerMatrix(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table);

    /**
     * @brief Tests the conversion from a char[] to a char*.
     */
    bool TestCArrayToCCStringScalar();

    /**
     * @brief Tests the conversion from a char[][] to a Vector<char*>
     */
    bool TestCArrayToCCStringVector();

    /**
     * @brief Tests the conversion from a char[][][] to a Matrix<char*>
     */
    bool TestCArrayToCCStringMatrix();

    /**
     * @brief Tests the conversion from an integer to a char[] truncating the string in output
     * because of the too small array size.
     */
    bool TestIntegerToCArrayScalar_Trunc();

    /**
     * @brief Tests the conversion from a float to a char[] truncating the string in output
     * because of the too small array size.
     */
    bool TestFloatToCArrayScalar_Trunc();

    /**
     * @brief Tests the conversion from a const char* to a char[] truncating the string in output
     * because of the too small array size.
     */
    bool TestCCStringToCArrayScalar_Trunc();

    /**
     * @brief Tests the conversion from a Vector<const char*> to a char[][] truncating the string in output
     * because of the too small array size.
     */
    bool TestCCStringToCArrayVector_Trunc();

    /**
     * @brief Tests the conversion from a Matrix<const char*> to a char[][][] truncating the string in output
     * because of the too small array size.
     */
    bool TestCCStringToCArrayMatrix_Trunc();

    /**
     * @brief Tests the conversion between two structured objects.
     */
    bool TestObjectToObject();

    /**
     * @brief Tests the conversion between two structured objects.
     */
    bool TestObjectToObject_Reverse();

    /**
     * @brief Tests if the conversion fails if the source class does not provide its introspection.
     */
    bool TestObjectToObject_ErrorNoSourceIntrospection();

    /**
     * @brief Tests if the conversion fails if the destination class does not provide its introspection.
     */
    bool TestObjectToObject_ErrorNoDestIntrospection();

    /**
     * @brief Tests if the conversion fails if the classes are not compatible.
     */
    bool TestObjectToObject_NoCompatibility();

    /**
     * @brief Tests the read of a structured object from a ConfigurationDatabase.
     */
    bool TestStructuredDataToObject_SourceIntrospection();

    /**
     * @brief Tests the read of a structured object from a ConfigurationDatabase with arrays of structures.
     */
    bool TestStructuredDataToObject_SourceIntrospectionArray();

    /**
     * @brief Tests the read of a structured object from a ConfigurationDatabase.
     */
    bool TestStructuredDataToObject_NoSourceIntrospection();

    /**
     * @brief Tests if the conversion fails if the destination class does not provide its introspection.
     */
    bool TestStructuredDataToObject_ErrorNoDestIntrospection();

    /**
     * @brief Tests if the conversion fails if the classes are not compatible.
     */
    bool TestStructuredDataToObject_NoCompatibility();

    /**
     * @brief Tests the write of a structured object on a ConfigurationDatabase.
     */
    bool TestObjectToStructuredData();

    /**
     * @brief Tests the write of a structured object with arrays of structures on a ConfigurationDatabase.
     */
    bool TestObjectArrayToStructuredData();

    /**
     * @brief Tests the if the functions clones the source ConfigurationDatabase into the destination ConfigurationDatabase.
     */
    bool TestStructuredDataToStructuredData();

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
template<typename T1, typename T2>
bool TypeConversionTest::TestTypeConvert(const TypeToTypeTableTest<T1, T2>* table) {

    uint32 i = 0;
    while (table[i].go) {

        T2 element;
        T2 result = table[i].result;
        T1 toConvert = table[i].typeToConvert;

        bool ret = TypeConvert(element, toConvert);
        if (element != result) {
            printf("\n%d \n", i);

            return false;
        }
        if (ret != table[i].expected) {
            printf("\n%d \n", i);

            return false;
        }

        i++;
    }
    return true;

}

template<typename T1, typename T2>
bool TypeConversionTest::TestTypeConvertPointer(const TypeToTypeTableTest<T1, T2>* table) {
    uint32 i = 0;
    while (table[i].go) {

        T2 element;
        const void* toConvert = &table[i].typeToConvert;

        TypeConvert(element, toConvert);
        uintp test = (uintp) &table[i].typeToConvert;
        T2 element2;
        TypeConvert(element2, test);
        if (element != element2) {
            printf("\n%d\n",i);
            return false;
        }


        i++;
    }
    return true;

}

template<typename T1, typename T2, uint32 nElements>
bool TypeConversionTest::TestTypeConvertPointerVector(const TypeToTypeVectorTableTest<T1, T2, nElements>* table){

    uint32 i = 0;
    while (table[i].go) {
        T2 element[nElements];
        T2 element2[nElements];
        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        const void* toConvert[nElements];
        uintp test[nElements];
        for (uint32 j = 0; j < nElements; j++) {
            // to avoid const errors
            toConvert[j] = &table[i].typeToConvert[j];
            test[j]=(uintp)&table[i].typeToConvert[j];
        }

        TypeConvert(element, toConvert);
        TypeConvert(element2, test);

        for (uint32 j = 0; j < nElements; j++) {
            //  T2 result=table[i].result[j];
            if (element[j] != element2[j]) {

                return false;
            }
        }

        i++;
    }
    return true;

}

template<typename T1, typename T2, uint32 nRows, uint32 nCols>
bool TypeConversionTest::TestTypeConvertPointerMatrix(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table){
    uint32 i = 0;
    while (table[i].go) {
        T2 element[nRows][nCols];
        T2 element2[nRows][nCols];
        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        const void* toConvert[nRows][nCols];
        uintp test[nRows][nCols];
        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {
                // to avoid const errors
                toConvert[j][k] = &table[i].typeToConvert[j][k];
                test[j][k] = (uintp)&table[i].typeToConvert[j][k];
            }
        }

        TypeConvert(element, toConvert);
        TypeConvert(element2, test);


        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {

                //  T2 result=table[i].result[j];
                if (element[j][k] != element2[j][k]) {
                    //printf("\n%d %d %d\n", toConvert[j][k], element[j][k], table[i].result[j][k]);
                    //printf("\n%s %s %d\n", element[j].Buffer(), result.Buffer(), j);

                    return false;
                }
            }
        }

        i++;
    }
    return true;
}

template<typename T1>
bool TypeConversionTest::TestTypeConvertToCCString(const TypeToTypeTableTest<T1, const char8*>* table) {

    uint32 i = 0;
    char8 *element = (char8*) HeapManager::Malloc(64);

    while (table[i].go) {

        T1 toConvert = table[i].typeToConvert;

        bool ret = TypeConvert(element, toConvert);

        if (StringHelper::Compare(element, table[i].result) != 0) {
            //  printf("\n%s %s %d\n", element.Buffer(), result.Buffer(), i);

            return false;
        }
        if (ret != table[i].expected) {
            printf("\n%d\n", i);
            return false;
        }

        i++;
    }

    HeapManager::Free((void*&)element);
    return true;

}
template<typename T1, typename T2, uint32 nElements>
bool TypeConversionTest::TestTypeConvertVector(const TypeToTypeVectorTableTest<T1, T2, nElements>* table) {

    uint32 i = 0;
    while (table[i].go) {
        T2 element[nElements];
        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        T1 toConvert[nElements];
        for (uint32 j = 0; j < nElements; j++) {
            // to avoid const errors
            toConvert[j] = table[i].typeToConvert[j];
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nElements; j++) {

            //  T2 result=table[i].result[j];
            if (element[j] != table[i].result[j]) {
                //printf("\n%s %s %d\n", element[j].Buffer(), result.Buffer(), j);

                return false;
            }

        }

        i++;
    }
    return true;

}

template<typename T1, typename T2, uint32 nRows, uint32 nCols>
bool TypeConversionTest::TestTypeConvertMatrixStaticToStatic(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table) {
    uint32 i = 0;
    while (table[i].go) {
        T2 element[nRows][nCols];
        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        T1 toConvert[nRows][nCols];
        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {
                // to avoid const errors
                toConvert[j][k] = table[i].typeToConvert[j][k];
            }
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {

                //  T2 result=table[i].result[j];
                if (element[j][k] != table[i].result[j][k]) {
                    //printf("\n%d %d %d\n", toConvert[j][k], element[j][k], table[i].result[j][k]);
                    //printf("\n%s %s %d\n", element[j].Buffer(), result.Buffer(), j);

                    return false;
                }
            }
        }

        i++;
    }
    return true;

}

template<typename T1, typename T2, uint32 nRows, uint32 nCols>
bool TypeConversionTest::TestTypeConvertMatrixStaticToHeap(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table) {
    uint32 i = 0;

    T2** matrix = new T2*[nRows];

    for (uint32 j = 0; j < nRows; j++) {
        matrix[j] = new T2[nCols];
    }
    Matrix<T2> element(matrix, nRows, nCols);

    bool ret = true;
    while (table[i].go && ret) {

        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        T1 toConvert[nRows][nCols];
        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {
                // to avoid const errors
                toConvert[j][k] = table[i].typeToConvert[j][k];
            }
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {

                //  T2 result=table[i].result[j];
                if (element[j][k] != table[i].result[j][k]) {
                    //printf("\n%d %d %d\n", toConvert[j][k], element[j][k], table[i].result[j][k]);
                    //printf("\n%s %s %d\n", element[j].Buffer(), result.Buffer(), j);

                    ret = false;
                }
            }
        }

        i++;
    }

    for (uint32 j = 0; j < nRows; j++) {
        delete[] matrix[j];
    }

    delete[] matrix;

    return ret;

}

template<typename T1, typename T2, uint32 nRows, uint32 nCols>
bool TypeConversionTest::TestTypeConvertMatrixHeapToStatic(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table) {
    uint32 i = 0;

    T1** matrix = new T1*[nRows];

    for (uint32 j = 0; j < nRows; j++) {
        matrix[j] = new T1[nCols];
    }
    Matrix<T1> toConvert(matrix, nRows, nCols);

    bool ret = true;
    while (table[i].go && ret) {

        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        T2 element[nRows][nCols];
        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {
                // to avoid const errors
                toConvert[j][k] = table[i].typeToConvert[j][k];
            }
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {

                //  T2 result=table[i].result[j];
                if (element[j][k] != table[i].result[j][k]) {
                    //printf("\n%d %d %d\n", toConvert[j][k], element[j][k], table[i].result[j][k]);
                    //printf("\n%s %s %d\n", element[j].Buffer(), result.Buffer(), j);

                    ret = false;
                }
            }
        }

        i++;
    }

    for (uint32 j = 0; j < nRows; j++) {
        delete[] matrix[j];
    }

    delete[] matrix;

    return ret;

}

template<typename T1, typename T2, uint32 nRows, uint32 nCols>
bool TypeConversionTest::TestTypeConvertMatrixHeapToHeap(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table) {
    uint32 i = 0;

    T1** input = new T1*[nRows];

    for (uint32 j = 0; j < nRows; j++) {
        input[j] = new T1[nCols];
    }
    Matrix<T1> toConvert(input, nRows, nCols);

    T2** output = new T2*[nRows];

    for (uint32 j = 0; j < nRows; j++) {
        output[j] = new T2[nCols];
    }
    Matrix<T2> element(output, nRows, nCols);

    bool ret = true;
    while (table[i].go && ret) {

        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {
                // to avoid const errors
                toConvert[j][k] = table[i].typeToConvert[j][k];
            }
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {

                //  T2 result=table[i].result[j];
                if (element[j][k] != table[i].result[j][k]) {
                    //printf("\n%d %d %d\n", toConvert[j][k], element[j][k], table[i].result[j][k]);
                    //printf("\n%s %s %d\n", element[j].Buffer(), result.Buffer(), j);

                    return false;
                }
            }
        }

        i++;
    }

    for (uint32 j = 0; j < nRows; j++) {
        delete[] input[j];
    }

    delete[] input;

    for (uint32 j = 0; j < nRows; j++) {
        delete[] output[j];
    }

    delete[] output;

    return ret;

}

template<typename T1>
bool TypeConversionTest::TestTypeConvertCArray(const TypeToTypeTableTest<T1, const char8*>* table) {
    uint32 i = 0;
    while (table[i].go) {

        char8 outBuffer[32];

        T1 toConvert = table[i].typeToConvert;

        bool ret = TypeConvert(outBuffer, toConvert);

        if (StringHelper::Compare(outBuffer, table[i].result) != 0) {

            return false;
        }

        if (ret != table[i].expected) {

            printf("\n%d\n", i);
            return false;
        }

        i++;
    }
    return true;

}

template<typename T2>
bool TypeConversionTest::TestTypeConvertFromCArray(const TypeToTypeTableTest<const char8 *, T2>* table) {
    uint32 i = 0;
    while (table[i].go) {

        char8 toConvert[127];

        StringHelper::Copy(toConvert, table[i].typeToConvert);

        T2 element;

        bool ret = TypeConvert(element, toConvert);

        if (element != table[i].result) {
            return false;
        }

        if (ret != table[i].expected) {

            printf("\n%d\n", i);
            return false;
        }

//        printf("\n%s %s %d\n", outBuffer, table[i].result, i);

        i++;
    }
    return true;

}

template<typename T1>
bool TypeConversionTest::TestTypeConvertToCharVector(const TypeToTypeTableTest<T1, const char8 *>* table) {
    uint32 i = 0;
    while (table[i].go) {

        Vector<char8> outBuffer(32);

        T1 toConvert = table[i].typeToConvert;

        bool ret = TypeConvert(outBuffer, toConvert);

        if (StringHelper::Compare(&outBuffer[0], table[i].result) != 0) {

            return false;
        }

        if (ret != table[i].expected) {

            printf("\n%d\n", i);
            return false;
        }

        i++;
    }
    return true;
}

template<typename T2>
bool TypeConversionTest::TestTypeConvertFromCharVector(const TypeToTypeTableTest<const char8 *, T2>* table) {
    uint32 i = 0;
    while (table[i].go) {

        Vector<char8> toConvert(127);

        StringHelper::Copy(&toConvert[0], table[i].typeToConvert);

        T2 element;

        bool ret = TypeConvert(element, toConvert);

        if (element != table[i].result) {
            return false;
        }

        if (ret != table[i].expected) {

            printf("\n%d\n", i);
            return false;
        }

//        printf("\n%s %s %d\n", outBuffer, table[i].result, i);

        i++;
    }
    return true;
}

template<typename T1, uint32 nElements>
bool TypeConversionTest::TestTypeConvertVectorCArray(const TypeToTypeVectorTableTest<T1, const char8*, nElements>* table) {
    uint32 i = 0;
    while (table[i].go) {
        const uint32 bufferSize = 64;
        char8 element[nElements][bufferSize];

        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        T1 toConvert[nElements];
        for (uint32 j = 0; j < nElements; j++) {
            // to avoid const errors
            toConvert[j] = table[i].typeToConvert[j];
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nElements; j++) {

            //  T2 result=table[i].result[j];
            if (StringHelper::Compare(&(element[j][0]), table[i].result[j]) != 0) {
                printf("\n%s %s %d\n", &(element[j][0]), table[i].result[j], j);

                return false;
            }

        }

        i++;
    }
    return true;

}

template<typename T1, uint32 nElements>
bool TypeConversionTest::TestTypeConvertVectorToCharMatrix(const TypeToTypeVectorTableTest<T1, const char8*, nElements>* table) {
    uint32 i = 0;
    while (table[i].go) {
        const uint32 bufferSize = 64;

        Matrix<char8> element(nElements, bufferSize);
        //char8 element[nElements][bufferSize];

        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        T1 toConvert[nElements];
        for (uint32 j = 0; j < nElements; j++) {
            // to avoid const errors
            toConvert[j] = table[i].typeToConvert[j];
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nElements; j++) {

            //  T2 result=table[i].result[j];
            if (StringHelper::Compare(&(element[j][0]), table[i].result[j]) != 0) {
                printf("\n%s %s %d\n", &(element[j][0]), table[i].result[j], j);

                return false;
            }

        }

        i++;
    }
    return true;

}

template<typename T2, uint32 nElements>
bool TypeConversionTest::TestTypeConvertVectorFromCArray(const TypeToTypeVectorTableTest<const char8*, T2, nElements>* table) {
    uint32 i = 0;
    while (table[i].go) {
        const uint32 bufferSize = 64;
        char8 toConvert[nElements][bufferSize];

        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        T2 element[nElements];
        for (uint32 j = 0; j < nElements; j++) {
            // to avoid const errors
            StringHelper::Copy(&toConvert[j][0], table[i].typeToConvert[j]);
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nElements; j++) {

            //  T2 result=table[i].result[j];
            if (element[j] != table[i].result[j]) {
                //printf("\n%s %s %d\n", &(element[j][0]), table[i].result[j], j);

                return false;
            }

        }

        i++;
    }
    return true;
}

template<typename T2, uint32 nElements>
bool TypeConversionTest::TestTypeConvertVectorFromCharMatrix(const TypeToTypeVectorTableTest<const char8*, T2, nElements>* table) {
    uint32 i = 0;
    while (table[i].go) {
        const uint32 bufferSize = 64;

        Matrix<char8> toConvert(nElements, bufferSize);
        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        T2 element[nElements];
        for (uint32 j = 0; j < nElements; j++) {
            // to avoid const errors
            StringHelper::Copy(&toConvert[j][0], table[i].typeToConvert[j]);
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nElements; j++) {

            //  T2 result=table[i].result[j];
            if (element[j] != table[i].result[j]) {
                //printf("\n%s %s %d\n", &(element[j][0]), table[i].result[j], j);

                return false;
            }

        }

        i++;
    }
    return true;
}

template<typename T1, uint32 nElements>
bool TypeConversionTest::TestTypeConvertVectorToCharStarVector(const TypeToTypeVectorTableTest<T1, const char8*, nElements>* table) {
    uint32 i = 0;
    Vector<char8 *> element(nElements);
    const uint32 bufferSize = 127;

    for (uint32 j = 0; j < nElements; j++) {
        element[j] = (char8*) HeapManager::Malloc(bufferSize);
    }

    bool ret = true;
    while (table[i].go && ret) {

        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        T1 toConvert[nElements];
        for (uint32 j = 0; j < nElements; j++) {
            // to avoid const errors
            toConvert[j] = table[i].typeToConvert[j];
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nElements; j++) {

            //  T2 result=table[i].result[j];
            if (StringHelper::Compare(element[j], table[i].result[j]) != 0) {
                printf("\n%s %s %d\n", (element[j]), table[i].result[j], j);

                ret = false;
            }

        }

        i++;
    }
    for (uint32 j = 0; j < nElements; j++) {
        HeapManager::Free((void*&) element[j]);
        // to avoid const errors

    }
    return ret;

}

template<typename T1, uint32 nRows, uint32 nCols>
bool TypeConversionTest::TestTypeConvertMatrixStaticToStaticCArray(const TypeToTypeMatrixTableTest<T1, const char8*, nRows, nCols>* table) {
    uint32 i = 0;
    while (table[i].go) {
        const uint32 bufferSize = 64;
        char8 element[nRows][nCols][bufferSize];
        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        T1 toConvert[nRows][nCols];
        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {
                // to avoid const errors
                toConvert[j][k] = table[i].typeToConvert[j][k];
            }
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {

                //  T2 result=table[i].result[j];
                if (StringHelper::Compare(&(element[j][k][0]), table[i].result[j][k]) != 0) {
                    //printf("\n%d %d %d\n", toConvert[j][k], element[j][k], table[i].result[j][k]);
                    //printf("\n%s %s %d\n", element[j].Buffer(), result.Buffer(), j);

                    return false;
                }
            }
        }

        i++;
    }
    return true;

}

template<typename T2, uint32 nRows, uint32 nCols>
bool TypeConversionTest::TestTypeConvertMatrixStaticToStaticFromCArray(const TypeToTypeMatrixTableTest<const char8*, T2, nRows, nCols>* table) {
    uint32 i = 0;
    while (table[i].go) {
        const uint32 bufferSize = 64;
        char8 toConvert[nRows][nCols][bufferSize];
        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        T2 element[nRows][nCols];
        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {
                StringHelper::Copy(&(toConvert[j][k][0]), table[i].typeToConvert[j][k]);

            }
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {

                //  T2 result=table[i].result[j];
                if (element[j][k] != table[i].result[j][k]) {
                    //printf("\n%d %d %d\n", toConvert[j][k], element[j][k], table[i].result[j][k]);
                    //printf("\n%s %s %d\n", element[j].Buffer(), result.Buffer(), j);

                    return false;
                }
            }
        }

        i++;
    }
    return true;

}

template<typename T1, uint32 nElements>
bool TypeConversionTest::TestTypeConvertVectorStaticToHeapMatrixCArray(const TypeToTypeVectorTableTest<T1, const char8*, nElements>* table) {
    uint32 i = 0;

    char8** matrix = new char8*[nElements];
    const char bufferSize = 64;

    for (uint32 j = 0; j < nElements; j++) {
        matrix[j] = new char8[bufferSize];
    }

    Matrix<char8> element(matrix, nElements, bufferSize);

    bool ret = true;
    while (table[i].go && ret) {

        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        T1 toConvert[nElements];
        for (uint32 j = 0; j < nElements; j++) {
            // to avoid const errors
            toConvert[j] = table[i].typeToConvert[j];
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nElements; j++) {

            //  T2 result=table[i].result[j];
            if (StringHelper::Compare(&(element[j][0]), table[i].result[j]) != 0) {
                // printf("\n%d %d %d\n", toConvert[j][k], element[j][k], table[i].result[j][k]);
                //printf("\n%s %s %d\n", element[j].Buffer(), result.Buffer(), j);

                ret = false;
            }
        }

        i++;
    }

    for (uint32 j = 0; j < nElements; j++) {
        delete[] matrix[j];
    }

    delete[] matrix;

    return ret;
}

template<typename T2, uint32 nRows, uint32 nCols>
bool TypeConversionTest::TestTypeConvertMatrixStaticToHeapFromCArray(const TypeToTypeMatrixTableTest<const char8*, T2, nRows, nCols>* table) {
    uint32 i = 0;

    T2** matrix = new T2*[nRows];

    for (uint32 j = 0; j < nRows; j++) {
        matrix[j] = new T2[nCols];
    }
    Matrix<T2> element(matrix, nRows, nCols);

    bool ret = true;
    while (table[i].go && ret) {

        const uint32 bufferSize = 64;
        char8 toConvert[nRows][nCols][bufferSize];
        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {
                // to avoid const errors
                StringHelper::Copy(&(toConvert[j][k][0]), table[i].typeToConvert[j][k]);
            }
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {

                //  T2 result=table[i].result[j];
                if (element[j][k] != table[i].result[j][k]) {
                    //printf("\n%d %d %d\n", toConvert[j][k], element[j][k], table[i].result[j][k]);
                    //printf("\n%s %s %d\n", element[j].Buffer(), result.Buffer(), j);

                    ret = false;
                }
            }
        }

        i++;
    }

    for (uint32 j = 0; j < nRows; j++) {
        delete[] matrix[j];
    }

    delete[] matrix;

    return ret;
}

template<typename T1, uint32 nRows, uint32 nCols>
bool TypeConversionTest::TestTypeConvertMatrixStaticToHeapCCString(const TypeToTypeMatrixTableTest<T1, const char8*, nRows, nCols>* table) {
    uint32 i = 0;

    Matrix<char8 *> element(nRows, nCols);

    for (uint32 j = 0; j < nRows; j++) {
        for (uint32 k = 0; k < nCols; k++) {
            element[j][k] = (char8*) HeapManager::Malloc(64);
        }
    }

    bool ret = true;
    while (table[i].go && ret) {

        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        T1 toConvert[nRows][nCols];
        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {
                toConvert[j][k] = table[i].typeToConvert[j][k];
            }
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {

                //  T2 result=table[i].result[j];
                if (StringHelper::Compare(element[j][k], table[i].result[j][k]) != 0) {
                    //printf("\n%d %d %d\n", toConvert[j][k], element[j][k], table[i].result[j][k]);
                    //printf("\n%s %s %d\n", element[j].Buffer(), result.Buffer(), j);

                    ret = false;
                }
            }
        }

        i++;
    }

    for (uint32 j = 0; j < nRows; j++) {
        for (uint32 k = 0; k < nCols; k++) {
            HeapManager::Free((void*&) element[j][k]);
        }
    }

    return ret;

}

template<typename T1, uint32 nRows, uint32 nCols>
bool TypeConversionTest::TestTypeConvertMatrixStaticToStaticCCString(const TypeToTypeMatrixTableTest<T1, const char8*, nRows, nCols>* table) {
    uint32 i = 0;

    char8 *matrix[nRows][nCols];
    Matrix<char8 *> element(matrix);

    for (uint32 j = 0; j < nRows; j++) {
        for (uint32 k = 0; k < nCols; k++) {
            element[j][k] = (char8*) HeapManager::Malloc(64);
        }
    }

    bool ret = true;
    while (table[i].go && ret) {

        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        T1 toConvert[nRows][nCols];
        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {
                toConvert[j][k] = table[i].typeToConvert[j][k];
            }
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {

                //  T2 result=table[i].result[j];
                if (StringHelper::Compare(element[j][k], table[i].result[j][k]) != 0) {
                    //printf("\n%d %d %d\n", toConvert[j][k], element[j][k], table[i].result[j][k]);
                    //printf("\n%s %s %d\n", element[j].Buffer(), result.Buffer(), j);

                    ret = false;
                }
            }
        }

        i++;
    }

    for (uint32 j = 0; j < nRows; j++) {
        for (uint32 k = 0; k < nCols; k++) {
            HeapManager::Free((void*&) element[j][k]);
        }
    }

    return ret;
}

template<typename T1, uint32 nRows, uint32 nCols>
bool TypeConversionTest::TestTypeConvertMatrixHeapToHeapCCString(const TypeToTypeMatrixTableTest<T1, const char8*, nRows, nCols>* table) {
    uint32 i = 0;

    T1** input = new T1*[nRows];

    for (uint32 j = 0; j < nRows; j++) {
        input[j] = new T1[nCols];
    }
    Matrix<T1> toConvert(input, nRows, nCols);

    Matrix<char8 *> element(nRows, nCols);
    for (uint32 j = 0; j < nRows; j++) {
        for (uint32 k = 0; k < nCols; k++) {
            element[j][k] = (char8*) HeapManager::Malloc(64);
        }
    }

    bool ret = true;

    while (table[i].go && ret) {

        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {
                // to avoid const errors
                toConvert[j][k] = table[i].typeToConvert[j][k];
            }
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {

                //  T2 result=table[i].result[j];
                if (StringHelper::Compare(element[j][k], table[i].result[j][k]) != 0) {
                    //printf("\n%d %d %d\n", toConvert[j][k], element[j][k], table[i].result[j][k]);
                    //printf("\n%s %s %d\n", element[j].Buffer(), result.Buffer(), j);

                    ret = false;
                }
            }
        }

        i++;
    }

    for (uint32 j = 0; j < nRows; j++) {
        for (uint32 k = 0; k < nCols; k++) {
            HeapManager::Free((void*&) element[j][k]);
        }
    }

    for (uint32 j = 0; j < nRows; j++) {
        delete[] input[j];
    }

    delete[] input;

    return ret;
}

template<typename T1, uint32 nRows, uint32 nCols>
bool TypeConversionTest::TestTypeConvertMatrixHeapToStaticCCString(const TypeToTypeMatrixTableTest<T1, const char8*, nRows, nCols>* table) {
    uint32 i = 0;

    T1** input = new T1*[nRows];

    for (uint32 j = 0; j < nRows; j++) {
        input[j] = new T1[nCols];
    }
    Matrix<T1> toConvert(input, nRows, nCols);

    char8 *matrix[nRows][nCols];

    Matrix<char8 *> element(matrix);

    for (uint32 j = 0; j < nRows; j++) {
        for (uint32 k = 0; k < nCols; k++) {
            element[j][k] = (char8*) HeapManager::Malloc(64);
        }
    }

    bool ret = true;
    while (table[i].go && ret) {

        //T2 result = table[i].result;
        //T1 toConvert = table[i].typeToConvert;
        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {

                // to avoid const errors
                toConvert[j][k] = table[i].typeToConvert[j][k];
            }
        }

        TypeConvert(element, toConvert);

        for (uint32 j = 0; j < nRows; j++) {
            for (uint32 k = 0; k < nCols; k++) {

                //  T2 result=table[i].result[j];
                if (StringHelper::Compare(element[j][k], table[i].result[j][k]) != 0) {
                    //printf("\n%d %d %d\n", toConvert[j][k], element[j][k], table[i].result[j][k]);
                    //printf("\n%s %s %d\n", element[j].Buffer(), result.Buffer(), j);

                    ret = false;
                }
            }
        }

        i++;
    }

    for (uint32 j = 0; j < nRows; j++) {
        for (uint32 k = 0; k < nCols; k++) {
            HeapManager::Free((void*&) element[j][k]);
        }
    }

    for (uint32 j = 0; j < nRows; j++) {
        delete[] input[j];
    }

    delete[] input;

    return ret;
}

#endif /* TYPECONVERSIONTEST_H_ */

