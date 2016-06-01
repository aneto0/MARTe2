/**
 * @file GAMSignalI.h
 * @brief Header file for class GAMSignalI
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

 * @details This header file contains the declaration of the class GAMSignalI
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef GAMSIGNALI_H_
#define GAMSIGNALI_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

#include "ReferenceContainer.h"
#include "StreamString.h"
#include "StandardParser.h"
#include "ConfigurationDatabase.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief The definition of a GAM input and / or output variable, which has to be read and / or
 * write by the GAM to the DataSource.
 *
 * @details Maps a GAM signal to its relative data source signals. Some methods are pure virtual
 * and must be implemented by the specific type of definitions inheriting from this class.
 */
class DLL_API GAMSignalI: public ReferenceContainer {

public:

    /**
     * @Constructor
     * @post
     *   GetType() == "" &&
     *   GetPath() == "";
     */
    GAMSignalI();

    virtual ~GAMSignalI();

    /**
     * @brief Merges the global definition (initialised using Initialise(*) function) with the
     * informations stored in the local StructuredData in input. It is possible completing structured
     * type definitions or setting from local data the Path, Type, Default and Operation fields. If
     * a defined field in the global one is going to be overridden by a local field, the global definition
     * will be considered valid.
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
     * @brief Retrieves the string used to configure the interface of this signal with the data source.
     * @return the "Operation" field.
     */
    const char8 *GetOperation();


    /**
     * @brief Initialises the container and reads the variable address and type from the StructuredData
     * in input.
     * @details The following fields can be specified:
     *
     *   - Path = "the path of the variable in the RealTimeDataSource" (default "")\n
     *   - Type = "the variable type" (default "")\n
     *   - Default = "the variable default value" (default "")\n
     *   - Dimensions = "the variable dimensions" (default "")\n
     *   - Operation = "the read-write operation configuration parameters" (default "")\n
     *   - Cycles = "how many subsequent cycles the variable must be be read or write"
     *
     * The "Dimensions" parameter follows the same format of the operation string in IntrospectionEntry. In particular in this
     * case Dimensions = "[n]" denotes an array with n elements, "Dimensions" = "[n][m]" denotes a matrix with n rows and m columns.\n
     * The "Operation" string will be delivered to the DataSource to make it able to provide the DataSourceBroker capable to perform
     * the desired IO operation. The most common syntax for this variable is the expression of a nx2 matrix specifying in each row
     * the begin and the end indexes of the block to be read (write) from (to) the RealTimeDataSourceSignal. An example is
     *
     *     Operation = "{{0,2},{9,9}}"
     *
     * which means that the reader (writer) must read (write) the first three elements of the DataSourceSignal block and then the 10-th element
     * to (from) the GAM signal variable.
     * Obviously in this case the GAMSignal must have a dimension of at least four elements (this will be checked during the configuration
     * of the DataSourceBroker).With this syntax a MemoryMapInputReader (MemoryMapOutputWriter) can be returned from the DataSource (if supported).
     * Most of DataSources (like DynamicDataBuffer) supports this syntax for this operation type.\n
     * The "Cycles" field can specify if the variable has to be read (write) in synchronous or asynchronous mode. In particular if it is equal to zero the
     * signal will be read (write) from (to) the data source asynchronously. Conversely, if this field is greater than zero the the specified number
     * of cycles of the signal will be read (write) synchronously from the data source and stored in memory. For instance, suppose that the goal is to
     * read three cycles of the integer X. The signal returned will be the array { X(1) , X(2) , X(3) } where X(i) denotes the integer read at the i-th cycle.\n
     * It is possible combine "Cycles" with "Operation" and "Samples" field (the latter available only for GAMSampledSignal) to define a specific IO
     * operation.
     *
     * @param[in] data contains the configuration data.
     * @return false in case of errors, true otherwise.
     */
    virtual bool Initialise(StructuredDataI &data);

    /**
     * @brief Sets the path to the variable in the RealTimeDataSource
     * @param[in] newPath is the path to the data source to be set.
     */
    void SetPath(const char8 * const newPath);

    /**
     * @brief Retrieves the number of cycles such that this variable has to be read (write) from (to) a data source.
     * @return the number of cycles.
     */
    uint32 GetCycles() const;


    /**
     * @brief Retrieves the number of dimensions (0=scalar, 1=vector, 2=matrix).
     * @return the number of dimensions.
     */
    uint8 GetNumberOfDimensions() const;

    /**
     * @brief Retrieves the number of elements related to a specific dimension.
     * @param[in] dimension is the dimension to be checked.
     * @return the number of elements of the dimension \a dimension.
     * @pre
     *   dimension < 3
     */
    uint32 GetNumberOfElements(const uint32 dimension) const;

protected:

    /**
     * The address of the variable in the RealTimeDataSource
     */
    StreamString path;

    /**
     * The signal type
     */
    StreamString type;

    /**
     * The signal dimensions as a string
     */
    StreamString dimensions;

    /**
     * The operation
     */
    StreamString operation;

    /**
     * The number of cycles
     */
    uint32 cycles;

    /**
     * The number of dimensions of this variable
     */
    uint8 numberOfDimensions;

    /**
     * The number of elements for each dimension
     * of this variable.
     */
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

#endif /* SOURCE_CORE_BAREMETAL_L5GAMS_GAMSIGNALI_H_ */

