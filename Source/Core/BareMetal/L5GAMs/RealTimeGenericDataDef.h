/**
 * @file RealTimeGenericDataDef.h
 * @brief Header file for class RealTimeGenericDataDef
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

 * @details This header file contains the declaration of the class RealTimeGenericDataDef
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEGENERICDATADEF_H_
#define REALTIMEGENERICDATADEF_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "RealTimeDataDefI.h"
#include "StreamString.h"
#include "StructuredDataI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief Generic mapping of a structure to the RealTimeDataSource.
 */
class DLL_API RealTimeGenericDataDef: public RealTimeDataDefI {
public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Constructor
     * @post
     *   GetDefaultValue() == "" &&
     *   GetType() == "" &&
     *   GetPath() == "" &&
     *   IsFinal == false;
     */
    RealTimeGenericDataDef();

    /**
     * @see RealTimeDataDefI::Verify(*)
     */
    virtual bool Verify();

    /**
     * @brief Initialises the container form StructuredData and for each sub-type definition
     * reads the default value and the final flag (specifying if the type is complete or not).
     * @details The following parameters can be specified:
     *
     *   - IsFinal = true (default false)
     *
     * The field IsFinal is false by default but if it is defined true in \a data, then this
     * definition it is supposed to be final and cannot be merged with any local configuration data
     * to be completed.
     */
    virtual bool Initialise(StructuredDataI& data);

    /**
     * @brief Converts this object in a StructuredData.
     * @param[in] data contains the configuration data.
     * @return false in case of errors, true otherwise.
     */
    virtual bool ToStructuredData(StructuredDataI& data);

    /**
     * @see RealTimeDataDefI::MergeWithLocal(*)
     */
    virtual bool MergeWithLocal(StructuredDataI & localData);


private:

    /**
     * Specifies if the definition is complete
     */
    bool final;

};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/


#endif /* REALTIMEGENERICDATADEF_H_ */

