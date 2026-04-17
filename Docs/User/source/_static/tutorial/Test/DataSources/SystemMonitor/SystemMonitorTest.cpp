/**                                                                             
 * @file SystemMonitorTest.cpp                                                                
 * @brief Source file for class SystemMonitorTest                                        
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
 * the class SystemMonitorTest (public, protected, and private). Be aware that some      
 * methods, such as those inline could be defined on the header file, instead.  
 */                                                                             

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "SystemMonitorTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace Tutorial {

SystemMonitorTest::SystemMonitorTest() {
    using namespace MARTe;
}

SystemMonitorTest::~SystemMonitorTest() {
    using namespace MARTe;
}

bool SystemMonitorTest::TestInitialise() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool SystemMonitorTest::TestSetConfiguredDatabase() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool SystemMonitorTest::TestSynchronise() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool SystemMonitorTest::TestAllocateMemory() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool SystemMonitorTest::TestGetSignalMemoryBuffer() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool SystemMonitorTest::TestGetBrokerName() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

bool SystemMonitorTest::TestPrepareNextState() {
    using namespace MARTe;
    bool ok = true;
    return ok;
}

}
