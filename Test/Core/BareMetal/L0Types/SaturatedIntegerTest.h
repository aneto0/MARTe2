/**
 * @file SaturatedIntegerTest.h
 * @brief Header file for class AnyType
 * @date 28 May 2019
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef TEST_CORE_BAREMETAL_L0TYPES_SATURATEDINTEGERTEST_H_
#define TEST_CORE_BAREMETAL_L0TYPES_SATURATEDINTEGERTEST_H_

#include "CompilerTypes.h"
#include "SaturatedInteger.h"

using namespace MARTe;
/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace SaturatedIntegerTest{


/**
 * @brief Performs a full test of the SaturatedInteger type.
 * @param[in] testSet is a set of 1-64 bits. Each bit correspond to a subset to be executed
 * @return True if the selected tests are successful. False otherwise.
 */
bool Test(uint64 testSelection,bool show=false);

}

#endif /* TEST_CORE_BAREMETAL_L0TYPES_SATURATEDINTEGERTEST_H_ */
