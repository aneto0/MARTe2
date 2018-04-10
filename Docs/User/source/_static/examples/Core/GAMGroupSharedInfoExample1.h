/**
 * @file GAMGroupSharedInfoExample1.h
 * @brief Header file for class GAMGroupSharedInfoExample1
 * @date 10/04/2018
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

 * @details This header file contains the declaration of the class GAMGroupSharedInfoExample1
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef EXAMPLES_CORE_GAMGROUPSHAREDINFOEXAMPLE1_H_
#define EXAMPLES_CORE_GAMGROUPSHAREDINFOEXAMPLE1_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "Object.h"
#include "Matrix.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * @brief A MARTe Object which contains information to be shared amongst GAMs in a GAMGroup.
 */
class GAMGroupSharedInfoExample1 : public MARTe::Object {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
    GAMGroupSharedInfoExample1();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~GAMGroupSharedInfoExample1();

    /**
     * @brief Reads the Model from the configuration file.
     * @param[in] data see Object::Initialise. The parameter Model shall exist and will be read as a Matrix<uint32>.
     * @return true if the parameter Model can be read.
     */
    virtual bool Initialise(MARTe::StructuredDataI & data);


    /**
     * The configured MatrixModel.
     */
    MARTe::Matrix<MARTe::uint32> *matrixModel;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* EXAMPLES_CORE_GAMGROUPSHAREDINFO_H_ */

