/**
 * @file StaticListTest.cpp
 * @brief Source file for class StaticListTest
 * @date 14/09/2015
 * @author Ivan Herrero
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
 * the class StaticListTest (public, protected, and private). Be aware that some 
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/

#include "StaticListTest.h"
#include "GeneralDefinitions.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

namespace {

//TODO: Remove these data for explicit instantiations
uint32 uint32Values[] = {3,7,2,5,45,3,6,7,47,3,74,5,32,91,8,66,9};
int32 intValues[] = {3,7,2,5,45,3,6,7,47,3,74,5,32,91,8,66,9};
int32* intpValues[] = {&intValues[0], &intValues[1], &intValues[2], &intValues[3]};

}

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

namespace Lists {

//TODO: Remove these explicit instantiations
template class StaticListTest<uint32,10,uint32Values,sizeof(uint32Values)>;
template bool StaticListTest<uint32,10,uint32Values,sizeof(uint32Values)>::TestAddOnEmptyList<10>(void);

template class StaticListTest<int32,10,intValues,sizeof(intValues)>;
template class StaticListTest<int32*,10,intpValues,sizeof(intpValues)>;

}

