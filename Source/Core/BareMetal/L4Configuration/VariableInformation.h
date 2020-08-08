/**
 * @file VariableInformation.h
 * @brief Header file for class VariableInformation
 * @date 08/04/2020
 * @author Filippo Sartori
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef VARIABLEINFORMATION_H_
#define VARIABLEINFORMATION_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StreamString.h"
#include "TypeCharacteristics.h"
#include "TypeDescriptor.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * type used to store 1 PseudoCode element
 */
typedef uint16 CodeMemoryElement;

/**
 * type used to address 1 PseudoCode element
 */
typedef uint16 CodeMemoryAddress;

/**
 * type used to store 1 Data element inthe stack and data area
 */
typedef uint32 DataMemoryElement;

/**
 * type used to address 1 Data element MUST be the same as a CodeMemoryElement
 */
typedef CodeMemoryElement DataMemoryAddress;

/**
 * used to mark an address to be invalid
 */
#define MAXDataMemoryAddress (TypeCharacteristics<DataMemoryAddress>::MaxValue())


/**
 *  Element for the list of variables
 */
struct VariableInformation: public LinkedListable {
    /**
     * name of the variable
     */
    StreamString name;

    /**
     * type of the variable. Only simple and numeric types are possible
     */
    TypeDescriptor type;

    /**
     * location of the variable in the data area.
     * if location is below the start of the variables then
     * it is a constant
     */
    DataMemoryAddress location;

    /**
     * where to read the variable from
     */
    void *externalLocation;

    /**
     * set when during compilation to mark that this output variable has already been written
     */
    bool variableUsed;

    /**
     * initialises data
     */
    inline VariableInformation();

    /**
    * @brief Copy constructor.
    */
    inline VariableInformation(const VariableInformation &in);
};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

inline VariableInformation::VariableInformation() : LinkedListable() {
    type = VoidType;
    location = 0u;
    externalLocation = NULL_PTR(void*);
    variableUsed = false;
}

/*lint -e{1738} LinkedListable has no copy constructor to be invoked in VariableInformation initializer list. */
inline VariableInformation::VariableInformation(const VariableInformation &in) : LinkedListable() {
    name             = in.name;
    type             = in.type;
    location         = in.location;
    /*lint -e{1554} if the variable is external the new copy created by the copy constructor is expected to point to the same place as the old one. */
    externalLocation = in.externalLocation;
    variableUsed = false;
}

} // MARTe

#endif /* VARIABLEINFORMATION_H_ */
