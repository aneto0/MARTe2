/**
 * @file GAMGroupSharedInfoExample1.cpp
 * @brief Source file for class GAMGroupSharedInfoExample1
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

 * @details This source file contains the definition of all the methods for
 * the class GAMGroupSharedInfoExample1 (public, protected, and private). Be aware that some
 * methods, such as those inline could be defined on the header file, instead.
 */

/*---------------------------------------------------------------------------*/
/*                         Standard header includes                          */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                         Project header includes                           */
/*---------------------------------------------------------------------------*/
#include "AdvancedErrorManagement.h"
#include "GAMGroupSharedInfoExample1.h"

/*---------------------------------------------------------------------------*/
/*                           Static definitions                              */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Method definitions                              */
/*---------------------------------------------------------------------------*/

GAMGroupSharedInfoExample1::GAMGroupSharedInfoExample1() {
    matrixModel = NULL_PTR(MARTe::Matrix<MARTe::uint32> *);
}

GAMGroupSharedInfoExample1::~GAMGroupSharedInfoExample1() {
    if (matrixModel != NULL_PTR(MARTe::Matrix<MARTe::uint32> *)) {
        delete matrixModel;
    }
}

bool GAMGroupSharedInfoExample1::Initialise(MARTe::StructuredDataI & data) {
    using namespace MARTe;
    bool ok = Object::Initialise(data);
    AnyType arrayDescription = data.GetType("Model");
    if (ok) {
        ok = arrayDescription.GetDataPointer() != NULL_PTR(void *);
    }
    if (!ok) {
        REPORT_ERROR(ErrorManagement::ParametersError, "Model parameter was not set");
    }
    uint32 numberOfDimensions = 0u;
    if (ok) {
        numberOfDimensions = arrayDescription.GetNumberOfDimensions();
        ok = (numberOfDimensions == 2u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "Model is not a Matrix");
        }
    }
    uint32 modelNumberOfRows;
    uint32 modelNumberOfCols;
    if (ok) {
        modelNumberOfRows = arrayDescription.GetNumberOfElements(1u);
        modelNumberOfCols = arrayDescription.GetNumberOfElements(0u);
        ok = (modelNumberOfRows > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "No rows defined in the matrix");
        }
    }
    if (ok) {
        ok = (modelNumberOfCols > 0u);
        if (!ok) {
            REPORT_ERROR(ErrorManagement::ParametersError, "No columns defined in the matrix");
        }
    }
    if (ok) {
        matrixModel = new Matrix<uint32>(modelNumberOfRows, modelNumberOfCols);
        ok = data.Read("Model", *matrixModel);
        if (ok) {
           // matrixModel.Copy(matrixModelRead);
            REPORT_ERROR(ErrorManagement::Information, "Model set to %u", *matrixModel);
        }
        else {
            REPORT_ERROR(ErrorManagement::ParametersError, "Could not read the Model");
        }
    }
    return ok;
}

CLASS_REGISTER(GAMGroupSharedInfoExample1, "")
