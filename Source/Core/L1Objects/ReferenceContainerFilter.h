/**
 * @file ReferenceContainerSearchFilter.h
 * @brief Header file for class ReferenceContainerSearchFilter
 * @date 13/08/2015
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

 * @details This header file contains the declaration of the class ReferenceContainerSearchFilter
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTER_H_
#define SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "Reference.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

class ReferenceContainerFilter {
public:
    /**
     * @brief Destructor. NOOP
     */
    virtual ~ReferenceContainerFilter() {

    }

    /**
     * @brief Tests if a given reference meets the searching criteria.
     * @details This function is to be implemented by the filter subclasses.
     * @param previouslyFound a container with the history of all the nodes that were previously found.
     * @param referenceToTest the Reference to be tested.
     * @return if the \a referenceToTest meets the searching criteria.
     */
    virtual bool Test(ReferenceContainer &previouslyFound, Reference &referenceToTest) = 0;
};




/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTER_H_ */
	
