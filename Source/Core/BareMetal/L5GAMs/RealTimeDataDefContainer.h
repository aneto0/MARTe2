/**
 * @file RealTimeDataDefContainer.h
 * @brief Header file for class RealTimeDataDefContainer
 * @date 25/02/2016
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

 * @details This header file contains the declaration of the class RealTimeDataDefContainer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEDATADEFCONTAINER_H_
#define REALTIMEDATADEFCONTAINER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "ReferenceContainer.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Collects a set of RealTimeDataDefI
 * @details It defines the IO GAM interface with respect to the RealTimeDataSource.
 */
class DLL_API RealTimeDataDefContainer: public ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     * @post
     *   IsFinal() == false;
     */
    RealTimeDataDefContainer();

    /**
     * @brief Calls RealTimeDataDefI::Verify(*) for each item in the container
     */
    bool Verify();

    /**
     * @brief Inserts all the RealTimeDataDefI items in the container and reads
     * from the StructuredData if the container is final defined (complete definition) or not.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Merges definitions with the local StructuredData.
     * @details If the definition from the global StructuredData is not complete,
     * completes the definition using the local StructuredData.
     * @param[in] localData is the local StructuredData.
     * @return true if the merge succeeds with no conflicts, false otherwise.
     */
    bool MergeWithLocal(StructuredDataI & localData);

    bool IsInput() const;

    bool IsOutput() const;

    virtual bool ToStructuredData(StructuredDataI & data);


private:

    /**
     * Specifies if the definition is complete
     */
    bool final;

    // initialised from cdb
    bool isInput;

    // initialised from cdb
    bool isOutput;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* REALTIMEDATADEFCONTAINER_H_ */

