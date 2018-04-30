/**
 * @file StreamStringExample1.cpp
 * @brief Source file for class StreamStringExample1
 * @date 30/04/2018
 * @author Andre Neto
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
 * the class StreamStringExample1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "ErrorLoggerExample.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

int main(int argc, char *argv[]) {
    using namespace MARTe;
    SetErrorProcessFunction(&ErrorProcessExampleFunction);

    StreamString s1 = "Hello";
    //The cursor of s1 will be pointing at the end. As a consequence an empty string will be printed.
    REPORT_ERROR_STATIC(ErrorManagement::Information, "s1 = %s", s1);
    s1.Seek(0);
    REPORT_ERROR_STATIC(ErrorManagement::Information, "{After seek} s1 = %s", s1);
    //The buffer will return the pointer to the start of the string.
    REPORT_ERROR_STATIC(ErrorManagement::Information, "{Buffer()} s1 = %s", s1.Buffer());

    StreamString s2 = s1;
    //The cursor of s2 will also be pointing at the end. As a consequence an empty string will be printed.
    REPORT_ERROR_STATIC(ErrorManagement::Information, "s2 = %s", s2);
    s2.Seek(1);
    REPORT_ERROR_STATIC(ErrorManagement::Information, "{After seek:1} s2 = %s", s2);

    StreamString s3 = s1;
    s3.Seek(0);
    s3.Printf("%s", "Hi");
    REPORT_ERROR_STATIC(ErrorManagement::Information, "{After Printf} s3 = %s", s3.Buffer());

    //Reset the string
    s3 = "";
    s3.Printf("%s", "Hi");
    REPORT_ERROR_STATIC(ErrorManagement::Information, "{After =\"\"} s3 = %s", s3.Buffer());

    return 0;
}

