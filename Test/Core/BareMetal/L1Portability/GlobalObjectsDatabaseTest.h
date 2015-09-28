/**
 * @file GlobalObjectsDatabaseTest.h
 * @brief Header file for class GlobalObjectsDatabaseTest
 * @date 26/09/2015
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class GlobalObjectsDatabaseTest
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GLOBALOBJECTSDATABASETEST_H_
#define GLOBALOBJECTSDATABASETEST_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GlobalObjectsDatabase.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class GlobalObjectsDatabaseTest {

public:

    /**
     * @brief Tests that an instance of the GlobalObjectsDatabase exists.
     */
    bool TestInstance();

    /**
     * @brief Tests the GlobalObjectsDatabase::Peek function by searching for the ClassRegistryDatabase global instance.
     */
    bool TestPeek();

    /**
     * @brief Tests the an instance of the StandardHeap exists.
     */
    bool TestGetStandardHeap();
};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GLOBALOBJECTSDATABASETEST_H_ */
