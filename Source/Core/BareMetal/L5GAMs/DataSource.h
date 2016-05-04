/**
 * @file DataSource.h
 * @brief Header file for class DataSource
 * @date 18/04/2016
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

 * @details This header file contains the declaration of the class DataSource
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef DATASOURCE_H_
#define DATASOURCE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "MemoryArea.h"
#include "StaticList.h"
#include "ReferenceContainer.h"
#include "GAMSignalI.h"
#include "ReferenceT.h"
#include "ReferenceContainer.h"
/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{


/**
 * @brief Memory container for the exchange of signal data.
 * @details This class provides the memory area where all signals interchanged between GAMs exist.
 * It can also be sub-classed to provide a uniform mechanism of sharing the same memory area between
 *  DataSourceSignalI that are inserted as a child of this container.
 *
 * @details The syntax in the configuration stream shall be:
 *
 * +DataSource_name = {\n
 *     Class = DataSource\n
 *     HeapName = "the heap name" (default NULL, i.e StandardHeap will be used)
 *     ...\n
 * }\n
 *
 * and it can only exist as a child of the [RealTimeApplication].+Data declaration.
 */
class DLL_API DataSource: public ReferenceContainer {

public:
    CLASS_REGISTER_DECLARATION()

    /**
     * @brief Initialises the ReferenceContainer.
     */
    DataSource();


    /**
     * @brief Configures the DataSource against the input configuration \a data.
     * @details The following fields can be specified in \a data:
     *
     *   HeapName = "the name of the heap memory"
     *
     * By default (if not specified) the StandardHeap will be used to manage the heap memory.
     * @return true if the ReferenceContainer::Initialise succeeds
     * @post
     *    if HeapName exits:
     *      memory.SetHeapName(heapName.Buffer())
     */
    virtual bool Initialise(StructuredDataI & data);

    /**
     * @brief Recursively calls DataSourceSignalI::Allocate
     * @details During the initialisation of a RealTimeApplication this method is called and allows
     * to share the memory of this DataSource with all of its children (provided that they inherit from DataSourceSignalI).
     * @return true if DataSourceSignalI::Allocate returns true for all the DataSourceSignalI in this container.
     * @pre
     *    All the children of this container inherit either from DataSourceSignalI or from ReferenceContainer.
     */
    virtual bool Allocate();


private:


    /**
     * The heap name
     */
    StreamString heapName;

    /**
     * The memory of the data source.
     */
    MemoryArea memory;

};

}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* DATASOURCE_H_ */

