/**
 * @file StatisticsHelperTTest.h
 * @brief Header file for class StatisticsHelperTTest
 * @date 21/04/2017
 * @author Andre' Torres
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

 * @details This header file contains the declaration of the class StatisticsHelperTTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef STATISTICSHELPERTTEST_H_
#define STATISTICSHELPERTTEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "StatisticsHelperT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

template<typename Type>
class StatisticsHelperTTest {
public:

    bool testConstructor(const uint32 windowSize);

    bool testReset(const uint32 windowSize);

    bool testPushSample(const uint32 windowSize);

    bool testGetSample_1(const uint32 windowSize);

    bool testGetSample_2(const uint32 windowSize);

    bool testGetSize(const uint32 windowSize);

    bool testGetCounter(const uint32 windowSize);

    bool testGetAvg(const uint32 windowSize,const Type val);

    bool testGetMax(const uint32 windowSize);

    bool testGetMin(const uint32 windowSize);

    bool testGetRms();

    bool testGetStd();

    bool testGetSum(const uint32 windowSize);

};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

template<typename Type>
bool StatisticsHelperTTest<Type>::testConstructor(const uint32 windowSize) {
    StatisticsHelperT<Type> myStatisticsHelper(windowSize);
    bool ok = (myStatisticsHelper.GetSize() == windowSize);
    if (ok) {
        ok = (myStatisticsHelper.GetCounter() == 0);
    }
    return ok;
}

template<typename Type>
bool StatisticsHelperTTest<Type>::testReset(const uint32 windowSize) {
    StatisticsHelperT<Type> myStatisticsHelper(windowSize);
    Type mySample = 0;
    myStatisticsHelper.PushSample(mySample);
    bool ok = (myStatisticsHelper.GetCounter() == 1);
    if (ok) {
        ok = myStatisticsHelper.Reset();
    }
    ok = (myStatisticsHelper.GetCounter() == 0);
    return ok;
}

template<typename Type>
bool StatisticsHelperTTest<Type>::testPushSample(const uint32 windowSize) {
    StatisticsHelperT<Type> myStatisticsHelper(windowSize);
    Type mySample[windowSize];
    bool ok = true;
    for (uint32 i = 0; (i < windowSize) && (ok); i++) {
        myStatisticsHelper.PushSample(mySample[i]);
        ok = (myStatisticsHelper.GetCounter() == i + 1);
    }
    return ok;
}

template<typename Type>
bool StatisticsHelperTTest<Type>::testGetSample_1(const uint32 windowSize) {
    StatisticsHelperT<Type> myStatisticsHelper(windowSize);
    Type mySample = static_cast<Type>(0);
    bool ok = true;
    for (uint32 i = 0; (i < 2u * windowSize) && (true); i++) {
        mySample = static_cast<Type>(i);
        ok = myStatisticsHelper.PushSample(mySample);
        if (true) {
            ok = (myStatisticsHelper.GetSample() == mySample);
        }
    }
    return ok;
}

template<typename Type>
bool StatisticsHelperTTest<Type>::testGetSample_2(const uint32 windowSize) {
    StatisticsHelperT<Type> myStatisticsHelper(windowSize);
    Type mySample = static_cast<Type>(0);
    bool ok = true;
    for (uint32 i = 0; (i < windowSize) && (ok); i++) {
        mySample = static_cast<Type>(i);
        myStatisticsHelper.PushSample(mySample);
        if (ok) {
            ok = (myStatisticsHelper.GetSample(i) == mySample);
        }
    }
    ok = (myStatisticsHelper.GetSample(windowSize) == static_cast<Type>(0));
    return ok;
}

template<typename Type>
bool StatisticsHelperTTest<Type>::testGetSize(const uint32 windowSize) {
    StatisticsHelperT<Type> myStatisticsHelper(windowSize);
    bool ok = (myStatisticsHelper.GetSize() == windowSize);
    return ok;
}

template<typename Type>
bool StatisticsHelperTTest<Type>::testGetCounter(const uint32 windowSize) {
    StatisticsHelperT<Type> myStatisticsHelper(windowSize);
    Type mySample;
    bool ok = true;
    for (uint32 i = 0; (i < 2u * windowSize) && (ok); i++) {
        mySample = static_cast<Type>(i);
        ok = myStatisticsHelper.PushSample(mySample);
        if (ok) {
            ok = (myStatisticsHelper.GetCounter() == ((i < windowSize) ? i+1u : windowSize));
        }
    }
    return ok;
}

template<typename Type>
bool StatisticsHelperTTest<Type>::testGetAvg(const uint32 windowSize, const Type val) {
    StatisticsHelperT<Type> myStatisticsHelper(windowSize);
    Type mySample = static_cast<Type>(val);
    bool ok = true;
    for (uint32 i = 0; (i < windowSize) && (ok); i++) {
        ok = myStatisticsHelper.PushSample(mySample);
    }
    return (myStatisticsHelper.GetAvg()==val);
}

template<typename Type>
bool StatisticsHelperTTest<Type>::testGetMax(const uint32 windowSize) {
    StatisticsHelperT<Type> myStatisticsHelper(windowSize);
    Type mySample = static_cast<Type>(0);
    bool ok = true;
    for (uint32 i = 0; (i < windowSize) && (ok); i++) {
        mySample = static_cast<Type>(i);
        ok = myStatisticsHelper.PushSample(mySample);
    }
    return (myStatisticsHelper.GetMax()==static_cast<Type>(windowSize-1));
}

template<typename Type>
bool StatisticsHelperTTest<Type>::testGetMin(const uint32 windowSize) {
    StatisticsHelperT<Type> myStatisticsHelper(windowSize);
    Type mySample = static_cast<Type>(0);
    bool ok = true;
    for (uint32 i = 0; (i < windowSize) && (ok); i++) {
        mySample = static_cast<Type>(i);
        ok = myStatisticsHelper.PushSample(mySample);
    }
    return (myStatisticsHelper.GetMin()==0);
}

template<typename Type>
bool StatisticsHelperTTest<Type>::testGetRms() {
    StatisticsHelperT<Type> myStatisticsHelper(4);
    // 6224 = 3.5
    myStatisticsHelper.PushSample(static_cast<Type>(6));
    myStatisticsHelper.PushSample(static_cast<Type>(2));
    myStatisticsHelper.PushSample(static_cast<Type>(2));
    myStatisticsHelper.PushSample(static_cast<Type>(4));
    return (myStatisticsHelper.GetStd()== static_cast<Type>(3.5));
}


template<typename Type>
bool StatisticsHelperTTest<Type>::testGetStd() {
    StatisticsHelperT<Type> myStatisticsHelper(4);
    // 6224 = 1.6583123951777
    myStatisticsHelper.PushSample(static_cast<Type>(0));
    myStatisticsHelper.PushSample(static_cast<Type>(1));
    myStatisticsHelper.PushSample(static_cast<Type>(4));
    myStatisticsHelper.PushSample(static_cast<Type>(9));
    return (myStatisticsHelper.GetStd()== static_cast<Type>(3.5));
}

template<typename Type>
bool StatisticsHelperTTest<Type>::testGetSum(const uint32 windowSize) {
    StatisticsHelperT<Type> myStatisticsHelper(windowSize);
    Type mySample = static_cast<Type>(0);
    Type sum = static_cast<Type>(0);
    bool ok = true;
    for (uint32 i = 0; (i < windowSize) && (ok); i++) {
        mySample = static_cast<Type>(i);
        sum=sum+static_cast<Type>(i);
        ok = myStatisticsHelper.PushSample(mySample);
    }
    return (myStatisticsHelper.GetSum()==sum);
}



} /*namespace MARTe*/
#endif /* STATISTICSHELPERTTEST_H_ */

