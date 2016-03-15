/**
 * @file RealTimeDataDefI.h
 * @brief Header file for class RealTimeDataDefI
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

 * @details This header file contains the declaration of the class RealTimeDataDefI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef REALTIMEDATADEFI_H_
#define REALTIMEDATADEFI_H_

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
 * @brief The interface of a real time data definition structure.
 * @details Maps a structure to the RealTimeDataSource.
 */
class RealTimeDataDefI: public ReferenceContainer {

public:

    /**
     * @Constructor
     * @post
     *   GetType() == "" &&
     *   GetPath() == "";
     */
    RealTimeDataDefI();

    /**
     * @brief Merges the global definition (initialised using Initialise(*) function) with the
     * informations stored in the local StructuredData in input.
     * @param[in] localData is the local StructuredData.
     * @return true if there are not conflicts between the local and the global definitions.
     */
    virtual bool MergeWithLocal(StructuredDataI &localData)=0;

    /**
     * @brief Checks if the definition is consistent with the introspection of a registered structure
     * if the field type is specified.
     * @return true if the type is unspecified or if the specified type definition is consistent
     * with the introspection of a registered structure.
     */
    virtual bool Verify()=0;

    /**
     * @brief retrieves the variable address in the RealTimeDataSource.
     * @return the variable address in the RealTimeDataSource.
     */
    const char8 *GetPath();

    /**
     * @brief Retrieves the variable type.
     * @return the variable type.
     */
    const char8 *GetType();

    /**
     * @brief Retrieves the variable default value.
     * @return the variable default value.
     */
    const char8 *GetDefaultValue();

    /**
     * @brief Initialises the container and reads the variable address and type from the StructuredData
     * in input.
     */
    virtual bool Initialise(StructuredDataI &data);

    void SetPath(const char8 * newPath);


    uint8 GetNumberOfDimensions() const;


    uint32 GetNumberOfElements(uint32 dimension);

protected:

    /**
     * The address of the variable in the RealTimeDataSource
     */
    StreamString path;

    /**
     * The variable type
     */
    StreamString type;

    uint8 numberOfDimensions;

    uint32 numberOfElements[3];


    /**
     * The default value
     */
    StreamString defaultValue;
};
}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* SOURCE_CORE_BAREMETAL_L5GAMS_REALTIMEDATADEFI_H_ */

