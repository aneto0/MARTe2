/**                                                                             
 * @file SystemMonitorSolGTest.cpp                                                                
 * @brief Source file for class SystemMonitorSolGTest                                        
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
 * the class SystemMonitorSolGTest (public, protected, and private). Be aware that some      
 * methods, such as those inline could be defined on the header file, instead.  
 */                                                                             

#define DLL_API

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/
#include <limits.h>
#include "gtest/gtest.h"
/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "SystemMonitorSolTest.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace Tutorial {

TEST(SystemMonitorSolGTest, TestInitialise) {
    SystemMonitorSolTest test;
    ASSERT_TRUE(test.TestInitialise());
}

TEST(SystemMonitorSolGTest, TestSetConfiguredDatabase) {
    SystemMonitorSolTest test;
    ASSERT_TRUE(test.TestSetConfiguredDatabase());
}

TEST(SystemMonitorSolGTest, TestSynchronise) {
    SystemMonitorSolTest test;
    ASSERT_TRUE(test.TestSynchronise());
}

TEST(SystemMonitorSolGTest, TestAllocateMemory) {
    SystemMonitorSolTest test;
    ASSERT_TRUE(test.TestAllocateMemory());
}

TEST(SystemMonitorSolGTest, TestGetSignalMemoryBuffer) {
    SystemMonitorSolTest test;
    ASSERT_TRUE(test.TestGetSignalMemoryBuffer());
}

TEST(SystemMonitorSolGTest, TestGetBrokerName) {
    SystemMonitorSolTest test;
    ASSERT_TRUE(test.TestGetBrokerName());
}

TEST(SystemMonitorSolGTest, TestPrepareNextState) {
    SystemMonitorSolTest test;
    ASSERT_TRUE(test.TestPrepareNextState());
}

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

