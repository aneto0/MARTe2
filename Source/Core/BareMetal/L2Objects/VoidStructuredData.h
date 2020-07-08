/**
 * @file VoidStructuredData.h
 * @brief Header file for class AnyType
 * @date 24 Jun 2019
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

#ifndef VOIDSTRUCTUREDDATA_H_
#define VOIDSTRUCTUREDDATA_H_


/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StructuredDataI.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief dummy StructuredDataI where each function fails
 */
class VoidStructuredData: public StructuredDataI{
public:
    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Read(CCString path,const AnyType &value);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Read(CCString path,Reference &object,bool borrow=true);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType GetVariableInformation(
    		CCString 			path,
    		TypeDescriptor &	td,
			uint32 &			nOfDimensions,
			uint32 *			dimensionSizes) const;

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Write(CCString name, const AnyType &value);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Write(Reference object);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Copy(StructuredDataI &destination);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType MoveToRoot();

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType MoveToAncestor(uint32 generations);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType MoveAbsolute(CCString path);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType MoveRelative(CCString path);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType MoveToChild(const uint32 childIdx);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType GetName(DynamicCString &name);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType GetChildName(const uint32 index,DynamicCString &name);

    /**
     * @see StructuredDataI
     */
    virtual uint32 GetNumberOfChildren();

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType AddToCurrentNode(Reference node);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType CreateAbsolute(CCString path);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType CreateRelative(CCString path);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Delete(CCString path);

};


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* VOIDSTRUCTUREDDATA_H_ */
