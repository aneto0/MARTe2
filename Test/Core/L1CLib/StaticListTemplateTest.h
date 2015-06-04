/*
 * StaticListTemplateTest.h
 *
 *  Created on: Mar 13, 2015
 *      Author: shareuser
 */
/** 
 * @file StaticListTemplateTest.h
 * @brief Tests the StaticListTemplate class and associate functions. */
#ifndef TEST_L1CLIB_STATICLISTTEMPLATETEST_H_
#define TEST_L1CLIB_STATICLISTTEMPLATETEST_H_

#include "StaticListTemplate.h"

/** @brief Class used for tests on StaticListTemplate functions. */
//the static list arguments are variables not addresses. The template is interface which
//simplify the usage of the StaticListHolder class.
class StaticListTemplateTest {
public:
    StaticListTemplateTest() {
        successful = false;
        elementInt32In = 0;
        elementInt32Out = 0;
        elementInt64In = 0;
        elementInt64Out = 0;
        numberOfElements = 20000;
        elementPosition = -1;
    }

    StaticListTemplate<int32> sltInt32;

    StaticListTemplate<int64> sltInt64;

    /** Indicates if the test was passed */
    bool successful;

    /** It is used to introduce elements in the sltInt32 class */ 
    int32 elementInt32In;

    /** It is used to introduce elements in the sltInt64 class */
    int64 elementInt64In;

    /** It is used to save the returned elements in the sltInt32 class */
    int32 elementInt32Out;

    /** It is used to save the returned elements in the sltInt64 class */
    int64 elementInt64Out;

    /** number of elements of the list */
    uint32 numberOfElements;

    /** saves the position of the searched element */
    int32 elementPosition;

    virtual ~StaticListTemplateTest() {

    }
    /**
     * @brief Test ListAdd using int32
     * @return true when the values are added correctly
     */
    bool ListAddTestInt32();

    /**
     * @ brief Test ListAdd using int32
     * @return true when the values are added correctly
     */
    bool ListAddTestInt64();

    /**
     * @brief Test ListExtract using Int64
     *
     * In the test are used the two variants of the ListExtract; specifying the position which
     * have to be removed and not specifying it (the default is to remove the end of the list)
     *
     * @return true when the values are removed correctly from the list
     */
    bool ListExtractTestInt64();

    /**
     * @brief Test ListPeek using Int64
     *
     * In the test are used the two variants of the ListExtract; specifying the position which
     * have to be removed and not specifying it (default SLH_EndOfList).
     *
     * @return true when the elements are removed correctly and the table is reorganized properly.
     */
    bool ListPeekTest();

    /**
     * @brief Test ListDeleteTest using Int64
     *
     * Test both possibilities; argument input = value to remove or argument input = position to remove
     *
     * @return True when the function behaves as expected
     */
    bool ListDeleteTest();

    /**
     * @brief Test ListFindTest using int64
     *
     * Also was  check that with an invalid argument the function have to return -1
     *
     * @return true when the position is the expected value
     */
    bool ListFindTest();

    /**
     * @brief Test ListInsert
     * @return true when the elements are added correctly at the beginning of the list
     */
    bool ListInsertTest();
};

#endif /* TEST_L1CLIB_STATICLISTTEMPLATETEST_H_ */
