/**
 * @file GAMGroup.h
 * @brief Header file for class GAMGroup
 * @date 24/02/2016
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

 * @details This header file contains the declaration of the class GAMGroup
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMGROUP_H_
#define GAMGROUP_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "StatefulI.h"
#include "ReferenceT.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief A group of GAMs sharing the same context.
 * @details This class allows GAMs to share a common context. GAMs which are referenced
 *  by a GAMGroup will have their SetContext method called.
 *
 * @details The syntax in the configuration stream has to be:
 *
 * +GAMGroup_name = {\n
 *    Class = GAMGroup\n
 *    GAM_name = {\n
 *        Class = GAM\n
 *        ...\n
 *    }\n
 *    ...\n
 * }
 *
 * and it has to be contained in the [RealTimeApplication].+Functions.[?ReferenceContainer?] declaration.
 */
class GAMGroup: public ReferenceContainer , public StatefulI {
public:

    /**
     * @brief Constructor. NOOP
     */
    GAMGroup();


    virtual bool Initialise(StructuredDataI & data);


    /**
     * @brief Destructor. NOOP
     */
    virtual ~GAMGroup();

    /**
     * @see ReferenceContainer::Purge()
     */
    virtual void Purge(ReferenceContainer &purgeList);

protected:

    /**
     * @brief Sets the context on all the GAMs that belong to this GAMGroup.
     * @param[in] context The context to be set on all GAMs.
     * @return true if GAM::SetContext returns true for all the GAMs.
     */
    virtual bool SetContext(ConstReference context);

    /**
     * All the GAMs that belong to this GAMGroup
     */
    ReferenceContainer GAMs;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMGROUP_H_ */

