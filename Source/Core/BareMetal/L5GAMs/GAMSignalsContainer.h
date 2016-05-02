/**
 * @file GAMSignalsContainer.h
 * @brief Header file for class GAMSignalsContainer
 * @date 11/04/2016
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

 * @details This header file contains the declaration of the class GAMSignalsContainer
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMSIGNALSCONTAINER_H_
#define GAMSIGNALSCONTAINER_H_

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
 * @details It defines an input or output (or both) collection of GAM data. It is possible
 * integrate the declaration in the global configuration StructuredDataI with a
 * local declaration (if defined inside the GAM) to add new RealTimeDataDefI into this
 * collection or specify locally if it is for input - output.
 *
 * @details The syntax in the configuration stream has to be:
 *
 * +RealTimeDataDefContainer_name = {\n
 *     Class = GAMSignalsContainer\n
 *     IsFinal = true (false by default)\n
 *     IsInput = true (false by default)\n
 *     IsOutput = true (false by default)\n
 *     ...\n
 * }\n
 *
 * and it has to be contained in the [GAM] declaration.
 */
class DLL_API GAMSignalsContainer: public ReferenceContainer {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     * @post
     *   IsFinal() == false &&
     *   IsInput() == false &&
     *   IsOutput() == false;
     */
    GAMSignalsContainer();

    /**
     * @brief Calls RealTimeDataDefI::Verify(*) for each item in the container
     */
    bool Verify();

    /**
     * @see ReferenceContainer::Initialise(*)
     * from the StructuredData if the container is final defined (complete definition) or not.
     * @details The following fields can be specified:
     *
     *   -IsInput = true (false by default)
     *   -IsOutput = true (false by default)
     *   -IsFinal = true (false by default)
     *
     * @details If after the initialisation this definition is not an input neither an output definition
     * a warning will be generated.
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Merges definitions with the local StructuredData.
     * @details If the definition from the global StructuredData is not complete,
     * completes the definition using the local StructuredData. It is possible setting
     * from local data the input - output attributes or adding new RealTimeDataDefI
     * definitions. All the definitions contained will be also eventually completed
     * calling RealTimeDataDefI::MergeWithLocal(*).
     * @param[in] localData is the local StructuredData.
     * @return true if the merge succeeds with no conflicts, false otherwise.
     */
    bool MergeWithLocal(StructuredDataI & localData);

    /**
     * @brief Checks if the definitions represent GAM input variables.
     * @return true if the definitions represent GAM input variables.
     */
    bool IsInput() const;

    /**
     * @brief Checks if the definitions represent GAM output variables.
     * @return true if the definitions represent GAM output variables.
     */
    bool IsOutput() const;

    /**
     * @see Object::ToStructuredData.
     * @details If true the following leafs will be added to \data:
     *
     *   -IsInput = true
     *   -IsOutput = true
     *   -IsFinal = true
     *
     */
    virtual bool ToStructuredData(StructuredDataI & data);


private:

    /**
     * Specifies if the definition is complete
     */
    bool final;

    /**
     * Denotes if the definitions are for input variables.
     */
    bool isInput;

    /**
     * Denotes if the definitions are for output variables.
     */
    bool isOutput;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* GAMSIGNALSCONTAINER_H_ */

