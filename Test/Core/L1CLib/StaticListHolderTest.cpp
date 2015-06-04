/*
 * Copyright 2015 F4E | European Joint Undertaking for
 * ITER and the Development of Fusion Energy ('Fusion for Energy')
 *
 * Licensed under the EUPL, Version 1.1 or - as soon they
 will be approved by the European Commission - subsequent
 versions of the EUPL (the "Licence");
 * You may not use this work except in compliance with the
 Licence.
 * You may obtain a copy of the Licence at:
 *
 * http://ec.europa.eu/idabc/eupl
 *
 * Unless required by applicable law or agreed to in
 writing, software distributed under the Licence is
 distributed on an "AS IS" basis,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 express or implied.
 * See the Licence
 permissions and limitations under the Licence.
 *
 *
 **/
#include "stdio.h"
#include "StaticListHolder.h"
#include "StaticListHolderTest.h"
#include "Threads.h"
#include "Sleep.h"

bool StaticListHolderTest::ListInt32() {
    bool ok = false;
    intptr f[4];

    int32 f11 = 1;
    int32 f21 = 2;
    int32 f31 = 3;
    int32 f41 = 4;

    f[0] = *(intptr *) &f11;
    f[1] = *(intptr *) &f21;
    f[2] = *(intptr *) &f31;
    f[3] = *(intptr *) &f41;
    intptr retrivedValuePtr;

    int32 i = 0; //index used to go through the Static Table
    int32 j = 0; //index used to go through the Static Table

    int32 elementPosition = -1;

    //The constructor receives the elements size, as a multiple of intptr size. All elements must have the same size.
    StaticListHolder slh(1);

    //Protect the concurrent access to the list (this is case is single-threaded, so this is useless)
    slh.SetAccessTimeout();

    //initialize the static table
    ok = slh.ListAdd(&f[0]);
    ok &= slh.ListAdd(&f[1]);
    ok &= slh.ListAdd(&f[2]);
    ok &= slh.ListAdd(&f[3]);

    //Check size
    ok &= 4 == slh.ListSize();

    //Read and compare the values of the Static list. Searching by position
    i = 0;
    while (i < int32(slh.ListSize())) {
        if (!slh.ListPeek(&retrivedValuePtr, i)) { //read the position i
            //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
            return false;
        }
        //Compare that the Static list elements are the correct ones
        ok &= (retrivedValuePtr == f[i]);
        //CStaticAssertErrorCondition(Information, " Value position %d is: %f ", i, *(float *)&retrivedValuePtr);
        i++;
    }

    //Search element and compare the returned position with the expected position
    i = 0;
    while (i < int32(slh.ListSize())) {
        elementPosition = slh.ListFind(&f[i]);
        ok &= (elementPosition == i);
        i++;
    }

    //Extract, ListPeek, List Find and finally add the extracted element
    i = 0;
    while (i < int32(slh.ListSize())) {
        //extract value
        if (!slh.ListExtract(&retrivedValuePtr, i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to retrieve the second element on the list!");
            return false;
        }
        ok &= (retrivedValuePtr == f[i]);

        //Read and compare the values of the Static list. Searching by position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (!slh.ListPeek(&retrivedValuePtr, j)) {     //read the position i
                //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
                return false;
            }
            //Compare that the Static list elements are the correct ones
            if (i <= j) { //notice that my array f[i] is not the same than the static list
                ok &= (retrivedValuePtr == f[j + 1]);
            }
            else {
                ok &= (retrivedValuePtr == f[j]);
            }
            j++;
        }

        //Search element and compare the returned position with the expected position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (i <= j) {
                elementPosition = slh.ListFind(&f[j + 1]); //notice that my array f[i] is not the same than the static list
            }
            else {
                elementPosition = slh.ListFind(&f[j]);
            }
            ok &= (elementPosition == j);
            j++;
        }

        //add the element in the same position where it was
        if (!slh.ListAdd((const intptr *) &f[i], i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to add the second element back to the list!");
            return false;
        }
        i++;
    }

    //Delete, ListPeek, List Find and finally add the extracted element
    i = 0;
    while (i < (int32) slh.ListSize()) {
        if (!slh.ListDelete(&f[i])) {
            //CStaticAssertErrorCondition(Information, "Failed to remove value %f", *(float *)&f1);
            return false;
        }

        //Read and compare the values of the Static list. Searching by position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (!slh.ListPeek(&retrivedValuePtr, j)) { //read the position i
                //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
                return false;
            }
            //Compare that the Static list elements are the correct ones
            if (i <= j) { //notice that my array f[i] is not the same than the static list
                ok &= (retrivedValuePtr == f[j + 1]);
            }
            else {
                ok &= (retrivedValuePtr == f[j]);
            }
            j++;
        }

        //Search element and compare the returned position with the expected position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (i <= j) {
                elementPosition = slh.ListFind(&f[j + 1]); //notice that my array f[i] is not the same than the static list
            }
            else {
                elementPosition = slh.ListFind(&f[j]);
            }
            ok &= (elementPosition == j);
            j++;
        }

        //add the element in the same position where it was
        if (!slh.ListAdd((const intptr *) &f[i], i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to add the second element back to the list!");
            return false;
        }
        i++;
    }
    return ok;
}

bool StaticListHolderTest::ListInt64() {
    bool ok = false;
    intptr f[4];

    int64 f11 = 4294967296;
    int64 f21 = 4294967297;
    int64 f31 = 4294967298;
    int64 f41 = 4294967299;

    f[0] = *(intptr *) &f11;
    f[1] = *(intptr *) &f21;
    f[2] = *(intptr *) &f31;
    f[3] = *(intptr *) &f41;
    intptr retrivedValuePtr;

    int32 i = 0; //index use to go through the Static Table
    int32 j = 0; //index use to go through the Static Table

    int32 elementPosition = -1;

    //The constructor receives the elements size, as a multiple of intptr size. All elements must have the same size.
    StaticListHolder slh(1);

    //Protect the concurrent access to the list (this is case is single-threaded, so this is useless)
    slh.SetAccessTimeout();

    //initialize the static table
    ok = slh.ListAdd(&f[0]);
    ok &= slh.ListAdd(&f[1]);
    ok &= slh.ListAdd(&f[2]);
    ok &= slh.ListAdd(&f[3]);

    //Check size
    ok &= 4 == slh.ListSize();

    //Read and compare the values of the Static list. Searching by position
    i = 0;
    while (i < int32(slh.ListSize())) {
        if (!slh.ListPeek(&retrivedValuePtr, i)) { //read the position i
            //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
            return false;
        }
        //Compare that the Static list elements are the correct ones
        ok &= (retrivedValuePtr == f[i]);
        //CStaticAssertErrorCondition(Information, " Value position %d is: %f ", i, *(float *)&retrivedValuePtr);
        i++;
    }

    //Search element and compare the returned position with the expected position
    i = 0;
    while (i < int32(slh.ListSize())) {
        elementPosition = slh.ListFind(&f[i]);
        ok &= (elementPosition == i);
        i++;
    }

    //Extract, ListPeek, List Find and finally add the extracted element
    i = 0;
    while (i < int32(slh.ListSize())) {
        //extract value
        if (!slh.ListExtract(&retrivedValuePtr, i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to retrieve the second element on the list!");
            return false;
        }
        ok &= (retrivedValuePtr == f[i]);

        //Read and compare the values of the Static list. Searching by position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (!slh.ListPeek(&retrivedValuePtr, j)) {     //read the position i
                //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
                return false;
            }
            //Compare that the Static list elements are the correct ones
            if (i <= j) { //notice that my array f[i] is not the same than the static list
                ok &= (retrivedValuePtr == f[j + 1]);
            }
            else {
                ok &= (retrivedValuePtr == f[j]);
            }
            j++;
        }

        //Search element and compare the returned position with the expected position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (i <= j) {
                elementPosition = slh.ListFind(&f[j + 1]); //notice that my array f[i] is not the same than the static list
            }
            else {
                elementPosition = slh.ListFind(&f[j]);
            }
            ok &= (elementPosition == j);
            j++;
        }

        //add the element in the same position where it was
        if (!slh.ListAdd((const intptr *) &f[i], i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to add the second element back to the list!");
            return false;
        }
        i++;
    }

    //Delete, ListPeek, List Find and finally add the extracted element
    i = 0;
    while (i < int32(slh.ListSize())) {
        if (!slh.ListDelete(&f[i])) {
            //CStaticAssertErrorCondition(Information, "Failed to remove value %f", *(float *)&f1);
            return false;
        }

        //Read and compare the values of the Static list. Searching by position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (!slh.ListPeek(&retrivedValuePtr, j)) { //read the position i
                //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
                return false;
            }
            //Compare that the Static list elements are the correct ones
            if (i <= j) { //notice that my array f[i] is not the same than the static list
                ok &= (retrivedValuePtr == f[j + 1]);
            }
            else {
                ok &= (retrivedValuePtr == f[j]);
            }
            j++;
        }

        //Search element and compare the returned position with the expected position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (i <= j) {
                elementPosition = slh.ListFind(&f[j + 1]); //notice that my array f[i] is not the same than the static list
            }
            else {
                elementPosition = slh.ListFind(&f[j]);
            }
            ok &= (elementPosition == j);
            j++;
        }

        //add the element in the same position where it was
        if (!slh.ListAdd((const intptr *) &f[i], i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to add the second element back to the list!");
            return false;
        }
        i++;
    }
    return ok;
}

bool StaticListHolderTest::ListFloat() {
    bool ok = false;
    intptr f[4];

    float f11 = 1.1; //3456789101112131.1;//1.1;
    float f21 = 2.2; //3456789101112131.2;//2.2;
    float f31 = 3.3; //3456789101112131.3;//3.3;
    float f41 = 4.4; //3456789101112131.4;//4.4;

    f[0] = *(intptr *) &f11;
    f[1] = *(intptr *) &f21;
    f[2] = *(intptr *) &f31;
    f[3] = *(intptr *) &f41;
    intptr retrivedValuePtr;

    int32 i = 0; //index use to go through the Static Table
    int32 j = 0; //index use to go through the Static Table

    int32 elementPosition = -1;

    //The constructor receives the elements size, as a multiple of intptr size. All elements must have the same size.
    StaticListHolder slh(1);

    //Protect the concurrent access to the list (this is case is single-threaded, so this is useless)
    slh.SetAccessTimeout();

    //initialize the static table
    ok = slh.ListAdd(&f[0]);
    ok &= slh.ListAdd(&f[1]);
    ok &= slh.ListAdd(&f[2]);
    ok &= slh.ListAdd(&f[3]);

    //Check size
    ok &= 4 == slh.ListSize();

    //Read and compare the values of the Static list. Searching by position
    i = 0;
    while (i < int32(slh.ListSize())) {
        if (!slh.ListPeek(&retrivedValuePtr, i)) { //read the position i
            //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
            return false;
        }
        //Compare that the Static list elements are the correct ones
        ok &= (retrivedValuePtr == f[i]);
        //CStaticAssertErrorCondition(Information, " Value position %d is: %f ", i, *(float *)&retrivedValuePtr);
        i++;
    }

    //Search element and compare the returned position with the expected position
    i = 0;
    while (i < (int32) (slh.ListSize())) {
        elementPosition = slh.ListFind(&f[i]);
        ok &= (elementPosition == i);
        i++;
    }

    //Extract, ListPeek, List Find and finally add the extracted element
    i = 0;
    while (i < (int32) slh.ListSize()) {
        //extract value
        if (!slh.ListExtract(&retrivedValuePtr, i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to retrieve the second element on the list!");
            return false;
        }
        ok &= (retrivedValuePtr == f[i]);

        //Read and compare the values of the Static list. Searching by position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (!slh.ListPeek(&retrivedValuePtr, j)) {     //read the position i
                //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
                return false;
            }
            //Compare that the Static list elements are the correct ones
            if (i <= j) { //notice that my array f[i] is not the same than the static list
                ok &= (retrivedValuePtr == f[j + 1]);
            }
            else {
                ok &= (retrivedValuePtr == f[j]);
            }
            j++;
        }

        //Search element and compare the returned position with the expected position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (i <= j) {
                elementPosition = slh.ListFind(&f[j + 1]); //notice that my array f[i] is not the same than the static list
            }
            else {
                elementPosition = slh.ListFind(&f[j]);
            }
            ok &= (elementPosition == j);
            j++;
        }

        //add the element in the same position where it was
        if (!slh.ListAdd((const intptr *) &f[i], i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to add the second element back to the list!");
            return false;
        }
        i++;
    }

    //Delete, ListPeek, List Find and finally add the extracted element
    i = 0;
    while (i < (int32) slh.ListSize()) {
        if (!slh.ListDelete(&f[i])) {
            //CStaticAssertErrorCondition(Information, "Failed to remove value %f", *(float *)&f1);
            return false;
        }

        //Read and compare the values of the Static list. Searching by position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (!slh.ListPeek(&retrivedValuePtr, j)) { //read the position i
                //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
                return false;
            }
            //Compare that the Static list elements are the correct ones
            if (i <= j) { //notice that my array f[i] is not the same than the static list
                ok &= (retrivedValuePtr == f[j + 1]);
            }
            else {
                ok &= (retrivedValuePtr == f[j]);
            }
            j++;
        }

        //Search element and compare the returned position with the expected position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (i <= j) {
                elementPosition = slh.ListFind(&f[j + 1]); //notice that my array f[i] is not the same than the static list
            }
            else {
                elementPosition = slh.ListFind(&f[j]);
            }
            ok &= (elementPosition == j);
            j++;
        }

        //add the element in the same position where it was
        if (!slh.ListAdd((const intptr *) &f[i], i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to add the second element back to the list!");
            return false;
        }
        i++;
    }
    return ok;
}

bool StaticListHolderTest::ListDouble() {
    bool ok = false;
    intptr f[4];

    double f11 = 3456789101112131.1;
    double f21 = 3456789101112132.2;
    double f31 = 3456789101112133.3;
    double f41 = 3456789101112134.4;

    f[0] = *(intptr *) &f11;
    f[1] = *(intptr *) &f21;
    f[2] = *(intptr *) &f31;
    f[3] = *(intptr *) &f41;
    intptr retrivedValuePtr;

    int32 i = 0; //index use to go through the Static Table
    int32 j = 0; //index use to go through the Static Table

    int32 elementPosition = -1;

    //The constructor receives the elements size, as a multiple of intptr size. All elements must have the same size.
    StaticListHolder slh(1);

    //Protect the concurrent access to the list (this is case is single-threaded, so this is useless)
    slh.SetAccessTimeout();

    //initialize the static table
    ok = slh.ListAdd(&f[0]);
    ok &= slh.ListAdd(&f[1]);
    ok &= slh.ListAdd(&f[2]);
    ok &= slh.ListAdd(&f[3]);

    //Check size
    ok &= 4 == slh.ListSize();

    //Read and compare the values of the Static list. Searching by position
    i = 0;
    while (i < int32(slh.ListSize())) {
        if (!slh.ListPeek(&retrivedValuePtr, i)) { //read the position i
            //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
            return false;
        }
        //Compare that the Static list elements are the correct ones
        ok &= (retrivedValuePtr == f[i]);
        //CStaticAssertErrorCondition(Information, " Value position %d is: %f ", i, *(float *)&retrivedValuePtr);
        i++;
    }

    //Search element and compare the returned position with the expected position
    i = 0;
    while (i < int32(slh.ListSize())) {
        elementPosition = slh.ListFind(&f[i]);
        ok &= (elementPosition == i);
        i++;
    }

    //Extract, ListPeek, List Find and finally add the extracted element
    i = 0;
    while (i < (int32) slh.ListSize()) {
        //extract value
        if (!slh.ListExtract(&retrivedValuePtr, i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to retrieve the second element on the list!");
            return false;
        }
        ok &= (retrivedValuePtr == f[i]);

        //Read and compare the values of the Static list. Searching by position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (!slh.ListPeek(&retrivedValuePtr, j)) {     //read the position i
                //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
                return false;
            }
            //Compare that the Static list elements are the correct ones
            if (i <= j) { //notice that my array f[i] is not the same than the static list
                ok &= (retrivedValuePtr == f[j + 1]);
            }
            else {
                ok &= (retrivedValuePtr == f[j]);
            }
            j++;
        }

        //Search element and compare the returned position with the expected position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (i <= j) {
                elementPosition = slh.ListFind(&f[j + 1]); //notice that my array f[i] is not the same than the static list
            }
            else {
                elementPosition = slh.ListFind(&f[j]);
            }
            ok &= (elementPosition == j);
            j++;
        }

        //add the element in the same position where it was
        if (!slh.ListAdd((const intptr *) &f[i], i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to add the second element back to the list!");
            return false;
        }
        i++;
    }

    //Delete, ListPeek, List Find and finally add the extracted element
    i = 0;
    while (i < (int32) slh.ListSize()) {
        if (!slh.ListDelete(&f[i])) {
            //CStaticAssertErrorCondition(Information, "Failed to remove value %f", *(float *)&f1);
            return false;
        }

        //Read and compare the values of the Static list. Searching by position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (!slh.ListPeek(&retrivedValuePtr, j)) { //read the position i
                //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
                return false;
            }
            //Compare that the Static list elements are the correct ones
            if (i <= j) { //notice that my array f[i] is not the same than the static list
                ok &= (retrivedValuePtr == f[j + 1]);
            }
            else {
                ok &= (retrivedValuePtr == f[j]);
            }
            j++;
        }

        //Search element and compare the returned position with the expected position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (i <= j) {
                elementPosition = slh.ListFind(&f[j + 1]); //notice that my array f[i] is not the same than the static list
            }
            else {
                elementPosition = slh.ListFind(&f[j]);
            }
            ok &= (elementPosition == j);
            j++;
        }

        //add the element in the same position where it was
        if (!slh.ListAdd((const intptr *) &f[i], i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to add the second element back to the list!");
            return false;
        }
        i++;
    }
    return ok;
}

bool StaticListHolderTest::SlhSize2() {
    bool ok = false;
    intptr *f[4];
    struct humanoide {
        const char * name; //not more than 7 characters
        double tall;
    };
    humanoide f11 = { "Juanca", 1.78 };
    humanoide f21 = { "Manolo", 1.78 };
    humanoide f31 = { "Josefa", 1.68 };
    humanoide f41 = { "Josefa", 2.08 }; //It is the same name in order to try to trick the Static list

    f[0] = (intptr *) &f11;
    f[1] = (intptr *) &f21;
    f[2] = (intptr *) &f31;
    f[3] = (intptr *) &f41;

    humanoide retrivedValue;
    intptr *retrivedValuePtr = (intptr *) &retrivedValue;

    int32 i = 0; //index used to go through the Static Table
    int32 j = 0; //index used to go through the Static Table

    int32 elementPosition = -1;

    //The constructor receives the elements size, as a multiple of intptr size. All elements must have the same size.
    StaticListHolder slh(2);

    //Protect the concurrent access to the list (this is case is single-threaded, so this is useless)
    slh.SetAccessTimeout();

    //initialize the static table
    ok = slh.ListAdd(f[0]);
    ok &= slh.ListAdd(f[1]);
    ok &= slh.ListAdd(f[2]);
    ok &= slh.ListAdd(f[3]);

    //Check size
    ok &= 4 == slh.ListSize();

    //Read and compare the values of the Static list. Searching by position
    i = 0;

    while (i < int32(slh.ListSize())) {
        if (!slh.ListPeek(retrivedValuePtr, i)) { //read the position i
            //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
            return false;
        }
        //Compare that the Static list elements are the correct ones
        ok &= (*retrivedValuePtr == *f[i]); //comparing the first 8 bytes (the field "name")
        ok &= (*(retrivedValuePtr + 1) == *(f[i] + 1)); //comparing the next 8 bytes (the field "tall")
        //printf("\n Using retrivedValuePtr: the name is %s the tall is %lf\n", *retrivedValuePtr);
        i++;
    }

    //Search element and compare the returned position with the expected position
    i = 0;
    while (i < int32(slh.ListSize())) {
        elementPosition = slh.ListFind(f[i]);
        ok &= (elementPosition == i);
        i++;
    }

    //Extract, ListPeek, List Find and finally add the extracted element
    i = 0;
    while (i < int32(slh.ListSize())) {
        //extract value
        if (!slh.ListExtract(retrivedValuePtr, i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to retrieve the second element on the list!");
            return false;
        }
        ok &= (*retrivedValuePtr == *f[i]);
        ok &= (*(retrivedValuePtr + 1) == *(f[i] + 1));

        //Read and compare the values of the Static list. Searching by position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (!slh.ListPeek(retrivedValuePtr, j)) {      //read the position i
                //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
                return false;
            }
            //Compare that the Static list elements are the correct ones
            if (i <= j) { //notice that my array f[i] is not the same than the static list
                ok &= (*retrivedValuePtr == *f[j + 1]);
                ok &= (*(retrivedValuePtr + 1) == *(f[j + 1] + 1));
            }
            else {
                ok &= (*retrivedValuePtr == *f[j]);
                ok &= (*(retrivedValuePtr + 1) == *(f[j] + 1));
            }
            j++;
        }

        //Search element and compare the returned position with the expected position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (i <= j) {
                elementPosition = slh.ListFind(f[j + 1]); //notice that my array f[i] is not the same than the static list
            }
            else {
                elementPosition = slh.ListFind(f[j]);
            }
            ok &= (elementPosition == j);
            j++;
        }

        //add the element in the same position where it was
        if (!slh.ListAdd((const intptr *) f[i], i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to add the second element back to the list!");
            return false;
        }
        i++;
    }

    //Delete, ListPeek, List Find and finally add the extracted element
    i = 0;
    while (i < (int32) slh.ListSize()) {
        if (!slh.ListDelete(f[i])) {
            //CStaticAssertErrorCondition(Information, "Failed to remove value %f", *(float *)&f1);
            return false;
        }

        //Read and compare the values of the Static list. Searching by position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (!slh.ListPeek(retrivedValuePtr, j)) { //read the position i
                //CStaticAssertErrorCondition(FatalError, " Failed to peek the second element on the list!");
                return false;
            }
            //Compare that the Static list elements are the correct ones
            if (i <= j) { //notice that my array f[i] is not the same than the static list
                ok &= (*retrivedValuePtr == *f[j + 1]);
                ok &= (*(retrivedValuePtr + 1) == *(f[j + 1] + 1));
            }
            else {
                ok &= (*retrivedValuePtr == *f[j]);
                ok &= (*(retrivedValuePtr + 1) == *(f[j] + 1));
            }
            j++;
        }

        //Search element and compare the returned position with the expected position
        j = 0;
        while (j < int32(slh.ListSize())) {
            if (i <= j) {
                elementPosition = slh.ListFind(f[j + 1]); //notice that my array f[i] is not the same than the static list
            }
            else {
                elementPosition = slh.ListFind(f[j]);
            }
            ok &= (elementPosition == j);
            j++;
        }

        //add the element in the same position where it was
        if (!slh.ListAdd((const intptr *) f[i], i)) {
            //CStaticAssertErrorCondition(FatalError, "Failed to add the second element back to the list!");
            return false;
        }
        i++;
    }
    return ok;
}
void FullFillList(StaticListHolderTest &tt) {
    int32 i = 0;
    intptr *iptr = NULL;
    //waiting the post from the main.
    tt.mutexSem.Lock();
    tt.counter++;
    tt.mutexSem.UnLock();
    tt.eventSem.Wait();
    while (i < tt.numberOfElements) {
        iptr = (intptr *) &i;
        tt.successful &= tt.slhr.ListAdd(iptr);
        //printf("\n i %d\n",i);
        i++;
    }
    tt.mutexSem.Lock();
    tt.counter--;
    tt.mutexSem.UnLock();
    return;
}

bool StaticListHolderTest::SetAccessTimeoutTest() {
    int32 numberOfThreads = 10; //number of threads you want generate
    int32 i = 0; //index to count the number of threads generated
    successful = true;
    counter = 0;
    TID id[numberOfThreads];
    eventSem.Reset();
    bool finalSuccess;

    //Protect the concurrent access to the list (this is case is single-threaded, so this is useless)
    slhr.SetAccessTimeout();

    //Start threads
    while (i < numberOfThreads) {
        id[i] = Threads::BeginThread((ThreadFunctionType) FullFillList, this);
        i++;
    }
    //waiting the initialization threads
    while (counter < numberOfThreads) {
        SleepMSec(1);
    }

    //Sleep just to make sure
    SleepMSec(1);

    //The threads can run their code. Now they are synchronized
    eventSem.Post();

    //Waiting the threads to finish.
    while (counter > 0) {
        SleepMSec(1);
    }
    //Just to make sure the threads finished their job
    SleepMSec(1);

    //Save true if all elements are added correctly
    finalSuccess = successful;

    //Check if the size of the table is as expected
    finalSuccess &= ((int32) slhr.ListSize()
            == numberOfThreads * numberOfElements);
    eventSem.Reset();

    //UnProtect the concurrent access to the list
    slhr.SetAccessTimeout(0);

    //Start threads
    i = 0;
    while (i < numberOfThreads) {
        id[i] = Threads::BeginThread((ThreadFunctionType) FullFillList, this);
        i++;
    }

    //waiting the initialization threads
    while (counter < numberOfThreads) {
        SleepMSec(1);
    }

    //Sleep just to make sure
    SleepMSec(1);

    //The threads can run their code. Now they are synchronized
    eventSem.Post();

    //Waiting the threads to finish.
    while (counter > 0) {
        SleepMSec(1);
    }

    //Just to make sure the threads finished their job
    SleepMSec(1);

    //successful is expected to fail because the static list is not-->
    //-->protected and several threads try to access it at the same time
    finalSuccess = finalSuccess && !successful;

    //Check that the size of the list is not as expected
    finalSuccess &= !((int32) slhr.ListSize()
            == 2 * numberOfElements * numberOfThreads);
    return finalSuccess;
}

bool StaticListHolderTest::ListAddTest() {
    intptr *f[7] = { NULL };

    int32 f11 = 1;
    int32 f21 = 2;
    int32 f31 = 2;
    int32 f41 = 4;
    int32 f51 = 2;
    int32 f61 = 6;
    int32 f71 = 7;

    f[0] = (intptr *) &f11;
    f[1] = (intptr *) &f21;
    f[2] = (intptr *) &f31;
    f[3] = (intptr *) &f41;
    f[4] = (intptr *) &f51;
    f[5] = (intptr *) &f61;
    f[6] = (intptr *) &f71;

    //Protect the concurrent access to the list (this is case is single-threaded, so this is useless)
    slhr.SetAccessTimeout();

    //initialize the static table
    successful = slhr.ListAdd(f[0]);
    successful &= slhr.ListAdd(f[1]);
    successful &= slhr.ListAdd(f[2]);
    successful &= slhr.ListAdd(f[3]);
    //add at the beginning
    successful &= slhr.ListAdd(f[4], 0);
    //add at the end
    successful &= slhr.ListAdd(f[5], -1);
    //add to strange position (should return false) it is impossible
    successful &= !slhr.ListAdd(f[6], 12);
    //add to strange position (should return false) it is impossible
    successful &= !slhr.ListAdd(f[6], -12);
    return successful;
}

bool StaticListHolderTest::AddFirstPosition() {
    numberOfElements = 65 * 2;
    successful = true;
    int32 i = 0;
    elementIntroduced = (intptr *) &i;
    //Fulfill the Static list
    while (i < numberOfElements) {
        successful &= slhr.ListAdd(elementIntroduced, 0);
        i++;
    }
    //Read static list
    i = 0;    //the larger number is the first element
    while (i < numberOfElements) {
        successful &= slhr.ListPeek(elementReturnedPtr, i);
        successful &= (numberOfElements - 1 - i == elementReturned);
        i++;
    }
    return successful;
}

bool StaticListHolderTest::ListExtractTest() {
    int32 i = 0;
    successful = true;
    numberOfElements = 10;
    //Fulfill the list
    i = 0;
    elementIntroduced = (intptr *) &i;
    while (i < numberOfElements) {
        successful &= slhr.ListAdd(elementIntroduced);
        i++;
    }

    //extract all elements
    i = 0;
    while (i < numberOfElements) {
        successful &= slhr.ListExtract(elementReturnedPtr);
        successful &= (elementReturned == numberOfElements - 1 - i);
        i++;
    }
    successful &= slhr.ListSize() == 0;

    //Try to extract from an empty list
    successful &= !slhr.ListExtract(elementReturnedPtr);

    //Fulfill the list again
    i = 0;
    elementIntroduced = (intptr *) &i;
    while (i < numberOfElements) {
        successful &= slhr.ListAdd(elementIntroduced);
        i++;
    }

    //extract from specific position
    successful &= slhr.ListExtract(elementReturnedPtr, 1);

    //extract from non existing position
    successful &= !slhr.ListExtract(elementReturnedPtr, numberOfElements + 10);

    return successful;
}
