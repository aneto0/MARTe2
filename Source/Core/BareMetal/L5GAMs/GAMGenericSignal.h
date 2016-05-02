/**
 * @file GAMGenericSignal.h
 * @brief Header file for class GAMGenericSignal
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

 * @details This header file contains the declaration of the class GAMGenericSignal
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMGENERICSIGNAL_H_
#define GAMGENERICSIGNAL_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "GAMSignalI.h"
#include "StreamString.h"
#include "StructuredDataI.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief The definition of a generic GAM input and / or output signal, which has to be read  and / or
 * write from the GAM to the DataSource.
 *
 * @details Allows to link different pieces of structured variables to different data source signals
 * using a recursive approach. Thus a GAMGenericSignal can be linked to two or more data source signals.
 *
 * @details The syntax in the configuration stream has to be:
 *
 * +GAMGenericSignal_name = {\n
 *     Class = (child of GAMSignalI) \n
 *     Path = "the path of the variable in the RealTimeDataSource" (default "")\n
 *     Type = "the variable type" (default "")\n
 *     Default = "the variable default value" (default "")\n
 *     Dimensions = "the variable dimensions" (default "")\n
 *     Operation = "the variable operation" (default "")
 *     Cycles = "how many consecutive cycles the variable must be be read or write"
 *     IsFinal = "specifies if the definition of the signal is complete"
 *     GAMGenericSignal_Name = {\n
 *         ...\n
 *     }\n
 *     ...\n
 * }\n
 *
 * and it has to be contained in the [GAMSignalsContainer] declaration.
 */
class DLL_API GAMGenericSignal: public GAMSignalI {
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
    GAMGenericSignal();

    /**
     * @see GAMSignalI::Verify(*)
     */
    virtual bool Verify();

    /**
     * @see ReferenceContainer::Initialise.
     * @details The following parameters can be specified:
     *
     *   - IsFinal = true (default = false)
     *
     * The field IsFinal is false by default but if it is defined true in \a data, then this
     * definition it is supposed to be final and cannot be merged with any local configuration data
     * to be completed.
     */
    virtual bool Initialise(StructuredDataI& data);

    /**
     * @see Object::ToStructuredData.
     * @details If true adds the leaf
     *
     *   - IsFinal = true
     *
     * to \a data.
     */
    virtual bool ToStructuredData(StructuredDataI& data);

    /**
     * @see GAMSignalI::MergeWithLocal(*)
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

#endif /* GAMGENERICSIGNAL_H_ */

