/**                                                                              
 * @file SystemMonitorSolTest.h                                                                 
 * @brief Header file for class SystemMonitorSolTest                                         
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
 * software distributed under the Licence is distributed on an  'AS IS'          
 * basis, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express           
 * or implied. See the Licence permissions and limitations under the Licence.    

 * @details This header file contains the declaration of the class SystemMonitorSolTest      
 * with all of its public, protected and private members. It may also include    
 * definitions for inline methods which need to be visible to the compiler.      
 */                                                                              

#ifndef _TUTORIAL_SYSTEMMONITORSOL_TEST_H_
#define _TUTORIAL_SYSTEMMONITORSOL_TEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "DataSourceI.h"
#include "SystemMonitorSol.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace Tutorial {

/**
 * @brief TODO.
 *
 * @details TODO.
 */

class SystemMonitorSolTest {
public:
    SystemMonitorSolTest();

    virtual ~SystemMonitorSolTest();

    /**
     * @brief Tests the Initialise method.
     */
    bool TestInitialise();

    /**
     * @brief Tests the SetConfiguredDatabase method.
     */
    bool TestSetConfiguredDatabase();

    /**
     * @brief Tests the Synchronise method.
     */
    bool TestSynchronise();

    /**
     * @brief Tests the AllocateMemory method.
     */
    bool TestAllocateMemory();

    /**
     * @brief Tests the GetSignalMemoryBuffer method.
     */
    bool TestGetSignalMemoryBuffer();

    /**
     * @brief Tests the GetBrokerName method.
     */
    bool TestGetBrokerName();

    /**
     * @brief Tests the PrepareNextState method.
     */
    bool TestPrepareNextState();

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* _TUTORIAL_SYSTEMMONITORSOL_TEST_H_ */

