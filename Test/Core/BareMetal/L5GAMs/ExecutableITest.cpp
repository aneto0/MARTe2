/**
 * @file ExecutableITest.cpp
 * @brief Source file for class ExecutableITest
 * @date 26/03/2022
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
 * the class ExecutableITest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "ExecutableI.h"
#include "ExecutableITest.h"
#include "ConfigurationDatabase.h"
#include "DataSourceI.h"
#include "ErrorManagement.h"
#include "GAMSchedulerI.h"
#include "ObjectRegistryDatabase.h"
#include "RealTimeApplication.h"
#include "StandardParser.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/
class ExecutableITester: public ExecutableI {
public:
    ExecutableITester() {
    }

    virtual bool Execute() {
        return true;
    }
};

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/
bool ExecutableITest::TestConstructor() {
    ExecutableITester tester;
    bool ok = tester.IsEnabled();
    if (ok) {
        ok = (tester.GetTimingSignalAddress() == NULL_PTR(uint32 *));
    }
    return ok;
}

bool ExecutableITest::TestSetEnabled() {
    ExecutableITester tester;
    tester.SetEnabled(true);
    bool ret = tester.IsEnabled();
    if (ret) {
        tester.SetEnabled(false);
        ret = !tester.IsEnabled();
    }
    if (ret) {
        tester.SetEnabled(true);
        ret = tester.IsEnabled();
    }
    return ret;
}

bool ExecutableITest::TestIsEnabled() {
    return TestSetEnabled();
}

bool ExecutableITest::TestSetTimingSignalAddress() {
    ExecutableITester tester;
    uint32 *ptr = reinterpret_cast<uint32 *>(0x01ABCDEF);
    tester.SetTimingSignalAddress(ptr);
    bool ok = (tester.GetTimingSignalAddress() == ptr);
    if (ok) {
        ptr = reinterpret_cast<uint32 *>(0xFEDCBA10);
        tester.SetTimingSignalAddress(ptr);
        ok = (tester.GetTimingSignalAddress() == ptr);
    }

    return ok;
}

bool ExecutableITest::TestGetTimingSignalAddress() {
    return TestSetTimingSignalAddress();
}




