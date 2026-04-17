/**                                                                             
 * @file SystemMonitorSolTest.cpp                                                                
 * @brief Source file for class SystemMonitorSolTest                                        
 * @date 17/04/2026                                                                
 * @author cabrian                                                            
 *                                                                              
 * @copyright Copyright 2026 F4E | European Joint Undertaking for ITER and      
 * the Development of Fusion Energy ('Fusion for Energy').                      
 * Licensed under the EUPL, Version 1.1 or - as soon they will be approved      
 * by the European Commission - subsequent versions of the EUPL (the 'Licence') 
 * You may not use this work except in compliance with the Licence.             
 * You may obtain a copy of the Licence at: https://eupl.eu/                    
 *                                                                              
 * @warning Unless required by applicable law or agreed to in writing,          
 * software distributed under the Licence is distributed on an 'AS IS'          
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express          
 * or implied. See the Licence permissions and limitations under the Licence.   

 * @details This source file contains the definition of all the methods for     
 * the class SystemMonitorSolTest (public, protected, and private). Be aware that some      
 * methods, such as those inline could be defined on the header file, instead.  
 */                                                                             

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SystemMonitorSolTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace Tutorial {

SystemMonitorSolTest::SystemMonitorSolTest() {
    using namespace MARTe;
}

SystemMonitorSolTest::~SystemMonitorSolTest() {
    using namespace MARTe;
}

bool SystemMonitorSolTest::TestInitialise() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool SystemMonitorSolTest::TestSetConfiguredDatabase() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool SystemMonitorSolTest::TestSynchronise() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool SystemMonitorSolTest::TestAllocateMemory() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool SystemMonitorSolTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool SystemMonitorSolTest::TestGetBrokerName() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool SystemMonitorSolTest::TestPrepareNextState() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

}
