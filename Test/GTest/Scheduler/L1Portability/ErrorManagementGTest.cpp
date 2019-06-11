/**
 * @file ErrorManagementGTest.cpp
 * @brief Source file for class ErrorManagementGTest
 * @date 25/08/2015
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
 * the class ErrorManagementGTest (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

#include <limits.h>

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "TestSupport.h"
#include "ErrorManagementMTTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/



TEST(ErrorManagementGTest,TestReportErrorFullContext) {
    ErrorManagementMTTest errorManTest;
    ASSERT_TRUE(errorManTest.TestReportErrorFullContext(ErrorManagement::Information,"Information","Error!!","FileError2", 1, "TestReportErrorFullContext",32));
    ErrorManagement::SetErrorProcessFunction (ErrorManagement::NullErrorProcessFunction);
}

TEST(ErrorManagementGTest,TestReportErrorMacroFullContext) {
    ErrorManagementMTTest errorManTest;
    ASSERT_TRUE(errorManTest.TestReportErrorMacroFullContext(ErrorManagement::Information,"Error!!","Information",32));
    ErrorManagement::SetErrorProcessFunction (ErrorManagement::NullErrorProcessFunction);
}
