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

#include "ConfigurationDatabase.h"
#include "ReferenceContainer.h"
#include "StandardParser.h"
#include "StreamString.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
namespace MARTe {

/**
 * @brief The definition of a GAM input (output) variable, which has to be read (written)
 * by the GAM from (to) the DataSource.
 *
 * @details A class which implements this interface knows how to map a GAM signal to its relative DataSourceSignalI signal.
 */
class DLL_API GAMSignalI: public ReferenceContainer {

public:

    /**
     * @Constructor
     * @post
     *   GetType() == "" &&
     *   GetPath() == "" &&
     *   GetNumberOfDimensions () == 0 &&
     *   GetNumberOfElements(*) == 1 &&
     *   IsSync() == false
     */
    GAMSignalI();

    /**
     * @brief Merges the global definition (initialised using Initialise(*) function) with external
     * information provided with \a localData.
     * @details During the configuration phase the GAM will call this function and provide a StructuredDataI
     *  with extra information to be merged with any information related to this signal.
     * This can be used, for example, to complement static information imposed by the GAM (e.g. Type) with dynamic information
     *  which is available in the Initialise phase (e.g. number of elements)
     * If a field was already provided during the Initialise phase this shall not be overridden by the new field value in \a localData.
     * @param[in] localData is the local StructuredData.
     * @return true if there are not conflicts between the local and the global definitions and if the signal allows merging.
     */
    virtual bool MergeWithLocal(StructuredDataI &localData)=0;

    /**
     * @brief Checks the type defined in GetType() is consistent with a registered introspection type.
     * @return true if the type is unspecified or if the specified type definition is consistent
     * with the introspection of a registered structure.
     */
    virtual bool Verify()=0;

    /**
     * @brief Gets the address of the DataSourceSignalI object producing/consuming this signal.
     * @return the variable address to the DataSourceSignalI.
     */
    const char8 *GetPath();

    /**
     * @brief Gets the variable type.
     * @return the variable type.
     */
    const char8 *GetType();

    /**
     * @brief Gets the variable default value.
     * @return the variable default value.
     */
    const char8 *GetDefaultValue();

    /**
     * @brief Gets the string used to configure the interface of this signal with the data source.
     * @details The "Operation" field allows to access to sub-elements of a multi-dimensional signal. Its syntax is specified
     *  by the final class implementing this interface.
     * @return the "Operation" field.
     */
    const char8 *GetOperation();

    /**
     * @brief Initialises the container and reads the variable address and type from the \a data.
     * @details The following fields can be specified:
     *
     *   - Path = "the address of the DataSourceSignalI object producing/consuming this signal" (default "")\n
     *   - Type = "the variable type" (default "")\n
     *   - Default = "the variable default value" (default "")\n
     *   - Dimensions = "the variable dimensions" (default "")\n
     *   - Operation = "the read-write operation configuration parameters" (default "")\n
     *   - Cycles = "how many subsequent cycles the variable must read or write"
     *
     * The "Dimensions" parameter follows the same format of the operation string in IntrospectionEntry. In particular in this
     * case Dimensions = "[n]" denotes an array with n elements, "Dimensions" = "[n][m]" denotes a matrix with n rows and m columns.\n
     * The "Operation" string allows to perform . The most common syntax for this variable is the expression of a nx2 matrix specifying in each row
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

#endif /* GAMSIGNALI_H_ */

