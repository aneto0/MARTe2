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
#include "String.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

template<typename T1, typename T2>
struct TypeToTypeTableTest {
    T1 typeToConvert;
    T2 result;
    bool go;
    bool expected;
};

template<typename T1, typename T2, uint32 nElements>
struct TypeToTypeVectorTableTest {
    T1 typeToConvert[nElements];
    T2 result[nElements];
    bool go;
};

template<typename T1, typename T2, uint32 nRows, uint32 nCols>
struct TypeToTypeMatrixTableTest {
    T1 typeToConvert[nRows][nCols];
    T2 result[nRows][nCols];
    bool go;
};

class TypeConversionTest {
public:
    template<typename T1, typename T2>
    bool TestTypeConvert(const TypeToTypeTableTest<T1, T2>* table);

    template<typename T1>
    bool TestTypeConvertCArray(const TypeToTypeTableTest<T1, const char8 *>* table);

    template<typename T2>
    bool TestTypeConvertFromCArray(const TypeToTypeTableTest<const char8 *, T2>* table);

    template<typename T1, typename T2, uint32 nElements>
    bool TestTypeConvertVector(const TypeToTypeVectorTableTest<T1, T2, nElements>* table);

    template<typename T1, uint32 nElements>
    bool TestTypeConvertVectorCArray(const TypeToTypeVectorTableTest<T1, const char8*, nElements>* table);

    template<typename T2, uint32 nElements>
    bool TestTypeConvertVectorFromCArray(const TypeToTypeVectorTableTest<const char8*, T2, nElements>* table);

    template<typename T1, typename T2, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixStaticToStatic(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table);

    template<typename T1, typename T2, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixStaticToHeap(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table);

    template<typename T1, typename T2, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixHeapToStatic(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table);

    template<typename T1, typename T2, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixHeapToHeap(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table);

    template<typename T1, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixStaticToStaticCArray(const TypeToTypeMatrixTableTest<T1, const char8*, nRows, nCols>* table);

    template<typename T2, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixStaticToStaticFromCArray(const TypeToTypeMatrixTableTest<const char8*, T2, nRows, nCols>* table);

    template<typename T1, uint32 nElements>
    bool TestTypeConvertVectorStaticToHeapMatrixCArray(const TypeToTypeVectorTableTest<T1, const char8*, nElements>* table);

    template<typename T2, uint32 nRows, uint32 nCols>
    bool TestTypeConvertMatrixStaticToHeapFromCArray(const TypeToTypeMatrixTableTest<const char8*, T2, nRows, nCols>* table);
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
            //  printf("\n%s %s %d\n", element.Buffer(), result.Buffer(), i);

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

    while (table[i].go) {

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

    for (uint32 j = 0; j < nRows; j++) {
        delete[] matrix[j];
    }

    delete[] matrix;

    return true;

}

template<typename T1, typename T2, uint32 nRows, uint32 nCols>
bool TypeConversionTest::TestTypeConvertMatrixHeapToStatic(const TypeToTypeMatrixTableTest<T1, T2, nRows, nCols>* table) {
    uint32 i = 0;

    T1** matrix = new T1*[nRows];

    for (uint32 j = 0; j < nRows; j++) {
        matrix[j] = new T1[nCols];
    }
    Matrix<T1> toConvert(matrix, nRows, nCols);

    while (table[i].go) {

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

                    return false;
                }
            }
        }

        i++;
    }

    for (uint32 j = 0; j < nRows; j++) {
        delete[] matrix[j];
    }

    delete[] matrix;

    return true;

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

    while (table[i].go) {

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

    return true;

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

        char8 toConvert[128];

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

    while (table[i].go) {

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

                return false;
            }
        }

        i++;
    }

    for (uint32 j = 0; j < nElements; j++) {
        delete[] matrix[j];
    }

    delete[] matrix;

    return true;
}

template<typename T2, uint32 nRows, uint32 nCols>
bool TypeConversionTest::TestTypeConvertMatrixStaticToHeapFromCArray(const TypeToTypeMatrixTableTest<const char8*, T2, nRows, nCols>* table) {
    uint32 i = 0;

    T2** matrix = new T2*[nRows];

    for (uint32 j = 0; j < nRows; j++) {
        matrix[j] = new T2[nCols];
    }
    Matrix<T2> element(matrix, nRows, nCols);

    while (table[i].go) {

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

                    return false;
                }
            }
        }

        i++;
    }

    for (uint32 j = 0; j < nRows; j++) {
        delete[] matrix[j];
    }

    delete[] matrix;

    return true;
}

#endif /* TYPECONVERSIONTEST_H_ */

