/**
 * @file ParentGAMGroupExample1.h
 * @brief Header file for class ParentGAMGroupExample1
 * @date 06/04/2018
 * @author Andre Neto
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

 * @details This header file contains the declaration of the class ParentGAMGroupExample1
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EXAMPLES_CORE_PARENTGAMGROUPEXAMPLE1_H_
#define EXAMPLES_CORE_PARENTGAMGROUPEXAMPLE1_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "GAM.h"
#include "GAMGroup.h"
#include "GAMGroupSharedInfoExample1.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe2Tutorial {
/**
 * @brief A GAMGroup which shares a context (containing a matrix) with many GAMs .
 *
 * +GAMExample1 = {
 *     Class = ParentGAMGroupExample1
 *     Model = {{2, 0, 0}, {0, 3, 0}, {0, 0, 4}}
 * }
 */
class ParentGAMGroupExample1: public MARTe::GAMGroup {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
ParentGAMGroupExample1    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~ParentGAMGroupExample1();

    /**
     * @brief Reads the Model from the configuration file and creates a GAMGroupSharedInfoExample1 reference to be shared with all the child GAMs.
     * @return true if the parameter Model can be read as a Matrix.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);

    /**
     * @brief Calls SetContext with the Matrix read during initialisation on all the child GAMs.
     * @return true if SetContext returns true on all the child GAMs.
     */
    virtual bool PrepareNextState(const MARTe::char8*, const MARTe::char8*);

private:

    /**
     * The context containing the Matrix to be shared.
     */
    MARTe::ReferenceT<GAMGroupSharedInfoExample1> matrixModelContext;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EXAMPLES_CORE_PARENTGAMGROUPEXAMPLE1_H_ */

