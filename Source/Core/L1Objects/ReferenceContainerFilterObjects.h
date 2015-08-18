/**
 * @file ReferenceContainerFilterObjects.h
 * @brief Header file for class ReferenceContainerFilterObjects
 * @date 14/08/2015
 * @author aneto
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

 * @details This header file contains the declaration of the class ReferenceContainerFilterObjects
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTEROBJECTS_H_
#define SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTEROBJECTS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainerFilter.h"
#include "ReferenceContainer.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
class ReferenceContainerFilterObjects: public ReferenceContainerFilter {
public:

    ReferenceContainerFilterObjects();

    /**
     * @brief TODO
     */
    ReferenceContainerFilterObjects(const int32 &occurrenceNumber,
                                    const uint32 &mode,
                                    const char8 * const address);

    /**
     * @brief TODO
     */
    virtual ~ReferenceContainerFilterObjects();

    /**
     * @brief TODO
     */
    virtual bool Test(ReferenceContainer &previouslyFound,
                      Reference &referenceToTest);

    virtual bool IsRecursive() const;

    virtual bool IsSearchAll() const;

    virtual bool IsStorePath() const;

private:
    /**
     * TODO
     */
    char8 **addressToSearch;

    /**
     * TODO
     */
    uint32 addressNumberNodes;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_L1OBJECTS_REFERENCECONTAINERFILTEROBJECTS_H_ */

