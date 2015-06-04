/*
 * StaticListTemplateTest.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: shareuser
 */

#include "StaticListTemplateTest.h"
#include "stdio.h"

bool StaticListTemplateTest::ListAddTestInt32() {
    int32 i; //index List
    successful = true;
    numberOfElements = 66;
    //Fulfill the table
    i = 0;
    while (((uint32)i) < numberOfElements) {
        elementInt32In = i;
        successful &= sltInt32.ListAdd(elementInt32In);
        i++;
    }
    successful &= sltInt32.ListSize() == numberOfElements;
    return successful;
}

bool StaticListTemplateTest::ListAddTestInt64() {
    int32 i; //index List
    successful = true;
    numberOfElements = 66;
    //Fulfill the table
    //elementInPtr = &i;
    i = 0;
    while (((uint32)i) < numberOfElements) {
        elementInt64In = (int64) i;
        //add at the end of the list
        successful &= sltInt64.ListAdd(elementInt64In);
        i++;
    }
    //check the size of the list
    successful &= sltInt64.ListSize() == numberOfElements;

    //add to a specific position
    successful &= sltInt64.ListAdd(elementInt64In, 2);
    return successful;

    //add to a specific position (-1 = end atthe list
    successful &= sltInt64.ListAdd(elementInt64In, -1);

    //add to a specific position (0 = end atthe list
    successful &= sltInt64.ListAdd(elementInt64In, 0);
    return successful;
}

bool StaticListTemplateTest::ListExtractTestInt64() {
    int32 i; //index List
    successful = true;
    numberOfElements = 64;
    //Fulfill the table
    i = 0;
    while (((uint32)i) < numberOfElements) {
        elementInt64In = (int64) i;
        successful &= sltInt64.ListAdd(elementInt64In);
        i++;
    }
    successful &= sltInt64.ListSize() == numberOfElements;

    //Extract elements
    i = 0;
    while (((uint32)i) < numberOfElements) {
        successful &= sltInt64.ListExtract(elementInt64Out);
        successful &= (numberOfElements - i - 1 == elementInt64Out);
        i++;
    }
    successful &= (sltInt64.ListSize() == 0);

    //Fulfill the table (again)
    i = 0;
    while (((uint32)i) < numberOfElements) {
        elementInt64In = (int64) i;
        successful &= sltInt64.ListAdd(elementInt64In);
        i++;
    }
    successful &= sltInt64.ListSize() == numberOfElements;

    //extract from position 4
    successful &= sltInt64.ListExtract(elementInt64Out, 4);
    // check the value extracted
    successful &= elementInt64Out == 4;

    //extract from impossible position. sltInt64.ListExtract should return false
    successful &= !sltInt64.ListExtract(elementInt64Out, numberOfElements + 10);

    return successful;
}

bool StaticListTemplateTest::ListPeekTest() {
    int32 i; //index List
    successful = true;
    numberOfElements = 64;
    //Fulfill the List
    i = 0;
    while (((uint32)i) < numberOfElements) {
        elementInt64In = (int64) i;
        successful &= sltInt64.ListAdd(elementInt64In);
        i++;
    }

    //Read the list
    i = 0;
    while (((uint32)i) < numberOfElements) {
        //Check the returned value
        successful &= sltInt64.ListPeek(elementInt64Out, i);
        //check that the element in the position i is as expected
        successful &= elementInt64Out == (int64) i;
        i++;
    }

    //check ListPeek without position arguments (default SLH_EndOfList)
    successful &= sltInt64.ListPeek(elementInt64Out);
    //check that the element in the last position is as expected
    successful &= elementInt64Out == numberOfElements - 1;
    return successful;
}

bool StaticListTemplateTest::ListDeleteTest() {
    int32 i; //index List
    successful = true;
    numberOfElements = 64;
    //Fulfill the List
    i = 0;
    while (((uint32)i) < numberOfElements) {
        elementInt64In = (int64) i;
        successful &= sltInt64.ListAdd(elementInt64In);
        i++;
    }
    //Delete the element which value is 1
    elementInt64In = 1;
    successful &= sltInt64.ListDelete(elementInt64In);
    //Check the value of the position 1
    successful &= sltInt64.ListPeek(elementInt64Out, (int32) elementInt64In);
    //Check that now in the position i there is the value i + 1 (because the other element was deletes before
    successful &= elementInt64Out == elementInt64In + 1;

    //add the value numberOfElements in the position 1
    elementInt64In = (int64) numberOfElements;
    i = 1;
    successful &= sltInt64.ListAdd(elementInt64In, i);
    //Check that the value was added
    successful &= sltInt64.ListPeek(elementInt64Out, i);
    successful &= elementInt64Out == elementInt64In;
    successful &= elementInt64Out == (int64) numberOfElements;
    //Delete the element in the position 1
    i = 1;
    successful &= sltInt64.ListDelete(i);
    //Read the value in the position i = 1
    successful &= sltInt64.ListPeek(elementInt64Out, i);
    //Check that the value in the position i = 1 is 2 because the value numberOfElements in position 1 was deleted
    successful &= elementInt64Out == (int64) (i + 1);
    return successful;
}

bool StaticListTemplateTest::ListFindTest() {
    int32 i; //index List
    successful = true;
    numberOfElements = 64;
    //Fulfill the List
    i = 0;
    while (((uint32)i) < numberOfElements) {
        elementInt64In = (int64) i;
        successful &= sltInt64.ListAdd(elementInt64In);
        i++;
    }
    //select a value of the list
    elementInt64In = 10;
    // Find the elementInt64In value and return the position
    elementPosition = sltInt64.ListFind(elementInt64In);
    successful &= elementPosition == (int) elementInt64In;

    //Try to find an element which is not on the list
    //select a value of the list
    elementInt64In = numberOfElements * 2;
    // Find the elementInt64In value and return the position
    elementPosition = sltInt64.ListFind(elementInt64In);
    successful &= elementPosition == -1;

    return successful;
}

bool StaticListTemplateTest::ListInsertTest() {
    int32 i; //index List
    successful = true;
    numberOfElements = 64;
    //Fulfill the List add at the beginning of the table
    i = 0;
    while (((uint32)i) < numberOfElements) {
        elementInt64In = (int64) i;
        successful &= sltInt64.ListInsert(elementInt64In);
        i++;
    }

    //Read the list
    i = 0;
    while (((uint32)i) < numberOfElements) {
        //Check the returned value
        successful &= sltInt64.ListPeek(elementInt64Out, i);
        //check that the element in the position i is as expected
        successful &= elementInt64Out == (int64) (numberOfElements - 1 - i);
        i++;
    }
    return successful;
}

