/**
 * @file AdvancedErrorManagementTest.cpp
 * @brief Source file for class AdvancedErrorManagementTest
 * @date 12/03/2017
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
 * the class AdvancedErrorManagementTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "AdvancedErrorManagementTest.h"
#include "ErrorManagement.h"
#include "ReferenceT.h"
#include "Object.h"
#include "StreamString.h"
#include "HeapManager.h"
/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
/**
 * Latest log received
 */
static MARTe::ErrorManagement::ErrorInformation lastErrorInfo;
static MARTe::StreamString lastErrorDescription;

/**
 * Call back to check the error messages triggered by the methods below
 */
void AdvancedErrorManagementTestProcessFunction(const MARTe::ErrorManagement::ErrorInformation &errorInfo, const char * const errorDescription) {
    lastErrorInfo = errorInfo;
    lastErrorDescription = errorDescription;
}

class AdvancedErrorManagementTestHelper: public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION()AdvancedErrorManagementTestHelper() {
        SetName("AdvancedErrorManagementTestHelper.0");
    }

    bool TestLogging_NoParameters() {
        MARTe::DynamicCString logText("AdvancedErrorManagementTest::TestLogging");
        REPORT_ERROR(MARTe::ErrorManagement::Debug, logText.GetList());
        MARTe::int32 lineNumber = __LINE__ - 1;
        bool ok = (lastErrorDescription == logText);
        ok &= (lastErrorInfo.className == MARTe::CCString("AdvancedErrorManagementTestHelper"));
        ok &= (lastErrorInfo.objectName == MARTe::CCString("AdvancedErrorManagementTestHelper.0"));
        ok &= (lastErrorInfo.objectPointer != NULL_PTR(void *));
        ok &= (lastErrorInfo.hrtTime > 0u);
        ok &= (lastErrorInfo.header.isObject == true);
        ok &= (lastErrorInfo.header.errorType == MARTe::ErrorManagement::Debug);
        ok &= (lastErrorInfo.header.lineNumber == lineNumber);
        ok &= (lastErrorInfo.fileName == MARTe::CCString("AdvancedErrorManagementTest.cpp") );
        ok &= (lastErrorInfo.functionName.FindPattern(MARTe::CCString("TestLogging_NoParameters"))!= 0xFFFFFFFF);

        return ok;
    }

    bool TestLogging_Parameters() {
        MARTe::DynamicCString logText("AdvancedErrorManagementTest::TestLogging");
        REPORT_ERROR(MARTe::ErrorManagement::Debug, logText.GetList());
        MARTe::int32 lineNumber = __LINE__ - 1;
        bool ok = (lastErrorDescription == logText);
        ok &= (lastErrorInfo.className == MARTe::CCString("AdvancedErrorManagementTestHelper"));
        ok &= (lastErrorInfo.objectName == MARTe::CCString("AdvancedErrorManagementTestHelper.0"));
        ok &= (lastErrorInfo.objectPointer != NULL_PTR(void *));
        ok &= (lastErrorInfo.hrtTime > 0u);
        ok &= (lastErrorInfo.header.isObject == true);
        ok &= (lastErrorInfo.header.errorType == MARTe::ErrorManagement::Debug);
        ok &= (lastErrorInfo.header.lineNumber == lineNumber);
        ok &= (lastErrorInfo.fileName == MARTe::CCString("AdvancedErrorManagementTest.cpp") );
        ok &= (lastErrorInfo.functionName.FindPattern(MARTe::CCString("TestLogging_Parameters"))!= 0xFFFFFFFF);

        return ok;
    }

};
CLASS_REGISTER(AdvancedErrorManagementTestHelper, "1.0")

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

bool AdvancedErrorManagementTest::TestREPORT_ERROR_STATIC_NoParameters() {
    SetErrorProcessFunction(&AdvancedErrorManagementTestProcessFunction);
    MARTe::StreamString logText = "AdvancedErrorManagementTest::TestREPORT_ERROR_STATIC_Parameters 1 2 A B";
    REPORT_ERROR_STATIC(MARTe::ErrorManagement::Debug, "AdvancedErrorManagementTest::TestREPORT_ERROR_STATIC_Parameters %d %d %s %s", 1, 2, "A", "B");
    MARTe::int32 lineNumber = __LINE__ - 1;
    bool ok = (lastErrorDescription == logText);
    ok &= (lastErrorInfo.className == NULL_PTR(MARTe::char8 *));
    ok &= (lastErrorInfo.objectName == NULL_PTR(MARTe::char8 *));
    ok &= (lastErrorInfo.objectPointer == NULL_PTR(void *));
    ok &= (lastErrorInfo.hrtTime > 0u);
    ok &= (lastErrorInfo.header.isObject == false);
    ok &= (lastErrorInfo.header.errorType == MARTe::ErrorManagement::Debug);
    ok &= (lastErrorInfo.header.lineNumber == lineNumber);
    ok &= (MARTe::StringHelper::Compare(lastErrorInfo.fileName, "AdvancedErrorManagementTest.cpp") == 0);
    ok &= (MARTe::StringHelper::SearchString(lastErrorInfo.functionName, "TestREPORT_ERROR_STATIC_NoParameters") != NULL_PTR(MARTe::char8 *));

    return ok;
}

bool AdvancedErrorManagementTest::TestREPORT_ERROR_STATIC_Parameters() {
    SetErrorProcessFunction(&AdvancedErrorManagementTestProcessFunction);
    MARTe::StreamString logText = "AdvancedErrorManagementTest::TestREPORT_ERROR_STATIC_Parameters 1 2 A B";
    REPORT_ERROR_STATIC(MARTe::ErrorManagement::Debug, "AdvancedErrorManagementTest::TestREPORT_ERROR_STATIC_Parameters %d %d %s %s", 1, 2, "A", "B");
    MARTe::int32 lineNumber = __LINE__ - 1;
    bool ok = (lastErrorDescription == logText);
    ok &= (lastErrorInfo.className == NULL_PTR(MARTe::char8 *));
    ok &= (lastErrorInfo.objectName == NULL_PTR(MARTe::char8 *));
    ok &= (lastErrorInfo.objectPointer == NULL_PTR(void *));
    ok &= (lastErrorInfo.hrtTime > 0u);
    ok &= (lastErrorInfo.header.isObject == false);
    ok &= (lastErrorInfo.header.errorType == MARTe::ErrorManagement::Debug);
    ok &= (lastErrorInfo.header.lineNumber == lineNumber);
    ok &= (lastErrorInfo.fileName == MARTe::CCString("AdvancedErrorManagementTest.cpp"));
    ok &= (lastErrorInfo.functionName.FindPattern(MARTe::CCString("TestREPORT_ERROR_STATIC_Parameters"))!=0xFFFFFFFF );

    return ok;
}

bool AdvancedErrorManagementTest::TestREPORT_ERROR_NoParameters() {
    MARTe::ReferenceT<AdvancedErrorManagementTestHelper> test(MARTe::HeapManager::standardHeapId);
    return test->TestLogging_NoParameters();
}

bool AdvancedErrorManagementTest::TestREPORT_ERROR_Parameters() {

	MARTe::ReferenceT<AdvancedErrorManagementTestHelper> test(MARTe::HeapManager::standardHeapId);
    return test->TestLogging_Parameters();
}

