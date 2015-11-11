/**
 * @file TimeStampTest.cpp
 * @brief Source file for class TimeStampTest
 * @date 11/11/2015
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

 * @details This source file contains the definition of all the methods for
 * the class TimeStampTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TimeStampTest.h"
#include "stdio.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

using namespace MARTe;

bool TimeStampTest::TestConstructor() {
    TimeStamp ts;

    if (ts.GetMicroseconds() != 0) {
        return false;
    }

    if (ts.GetSeconds() != 0) {
        return false;
    }

    if (ts.GetMinutes() != 0) {
        return false;
    }

    if (ts.GetHour() != 0) {
        return false;
    }

    if (ts.GetDay() != 0) {
        return false;
    }

    if (ts.GetMonth() != 0) {
        return false;
    }
    if (ts.GetYear() != 1900) {
        return false;
    }

    return true;
}

bool TimeStampTest::TestSetMicroSeconds(uint32 input){

    TimeStamp ts;

    ts.SetMicroseconds(input);

    return ts.GetMicroseconds()==input%1000000;

}

bool TimeStampTest::TestGetMicroSeconds(uint32 input){
    return TestSetMicroSeconds(input);
}

bool TimeStampTest::TestSetSeconds(uint32 input){

    TimeStamp ts;

    ts.SetSeconds(input);

    return ts.GetSeconds()==input%60;

}

bool TimeStampTest::TestGetSeconds(uint32 input){
    return TestSetSeconds(input);

}

bool TimeStampTest::TestSetMinutes(uint32 input){

    TimeStamp ts;

    ts.SetMinutes(input);

    return ts.GetMinutes()==input%60;

}

bool TimeStampTest::TestGetMinutes(uint32 input){
    return TestSetMinutes(input);
}

bool TimeStampTest::TestSetHour(uint32 input){

    TimeStamp ts;

    ts.SetHour(input);

    return ts.GetHour()==input%24;

}

bool TimeStampTest::TestGetHour(uint32 input){
    return TestSetHour(input);
}

bool TimeStampTest::TestSetDay(uint32 input){

    TimeStamp ts;

    ts.SetDay(input);

    return (ts.GetDay()==input%31);

}

bool TimeStampTest::TestGetDay(uint32 input){
    return TestSetDay(input);
}

bool TimeStampTest::TestSetMonth(uint32 input){

    TimeStamp ts;

    ts.SetMonth(input);

    return ts.GetMonth()==input%12;

}

bool TimeStampTest::TestGetMonth(uint32 input){
    return TestSetMonth(input);
}

bool TimeStampTest::TestSetYear(uint32 input){

    TimeStamp ts;

    ts.SetYear(input);

    return ts.GetYear()>=1900;

}

bool TimeStampTest::TestGetYear(uint32 input){
    return TestSetYear(input);
}


bool TimeStampTest::TestConvertFromEpoch(){

    TimeStamp ts;

    ts.ConvertFromEpoch(0);
    if (ts.GetMicroseconds() != 0) {
        return false;
    }

    if (ts.GetSeconds() != 0) {
        return false;
    }

    if (ts.GetMinutes() != 0) {
        return false;
    }

    if (ts.GetHour() != 1) {
        return false;
    }

    if (ts.GetDay() != 0) {
        return false;
    }

    if (ts.GetMonth() != 0) {
        return false;
    }
    if (ts.GetYear() != 1970) {
        return false;
    }

    return true;
}
