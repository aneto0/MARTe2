/**
 * @file AnyTypeStructuredData.h
 * @brief Header file for class AnyType
 * @date 5 Sep 2017
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

#ifndef ANYTYPESTRUCTUREDDATA_H_
#define ANYTYPESTRUCTUREDDATA_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "StructuredDataI.h"
#include "AnyType.h"
#include "Stack.h"


/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * It allows accessing AnyType via a StructuredDataI interface
 * If the data type is structured a vector or is a pointer, it allows browsing into it.
 * There a number of conventions:
 *  address '*'allows deferencing a pointer
 *  address [N] allows choosing the Nth element of a Vector or a ZeroTerm array (if within range)
 *  .nnn allows choosing a member of a structure named .nnn (. needed only when combining addresses)
 */
class AnyTypeStructuredData: public StructuredDataI{

public:
	/**
	 * First method of instantiation.
	 * Any variable can be used.
	 */
	template <class T >
	AnyTypeStructuredData(T &data);

    AnyTypeStructuredData(const AnyTypeStructuredData *data);

	/**
	 * virtual destructor
	 */
	virtual ~AnyTypeStructuredData();

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
            CCString            path,
            TypeDescriptor &    td,
            uint32 &            nOfDimensions,
            uint32 *            dimensionSizes) const ;
/**
 * require write access!
 */

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Write(Reference object);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Write(CCString name, const AnyType &value);

    /**
     * @see StructuredDataI
     */
    virtual ErrorManagement::ErrorType Copy(StructuredDataI &destination);

    /**
     * Affect the current position
     */

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
     * Read path information
     */

    /**
     * @see StructuredDataI
     */
    virtual CCString GetName();

    /**
     * @see StructuredDataI
     */
    virtual CCString GetChildName(const uint32 childIdx);

    /**
     * @see StructuredDataI
     */
    virtual uint32 GetNumberOfChildren();


/**
 *  Affect path
 */

    /**
     * @see StructuredDataI
     * Unsupported
     */
    virtual ErrorManagement::ErrorType AddToCurrentNode(Reference node);

    /**
     * @see StructuredDataI
     * Unsupported
     */
    virtual ErrorManagement::ErrorType CreateAbsolute(CCString path);

    /**
     * @see StructuredDataI
     * Unsupported
     */
    virtual ErrorManagement::ErrorType CreateRelative(CCString path);

    /**
     * @see StructuredDataI
     * Unsupported
     */
    virtual ErrorManagement::ErrorType Delete(CCString path);

private:
    /**
     * allow obtaining the AnyType corresponding to a given level of dereferencing;
     */
    ErrorManagement::ErrorType Move1Step(CCString name) ;

    /**
     * allow obtaining the AnyType corresponding to a given level of dereferenciation;
     */
    ErrorManagement::ErrorType Access(CCString name,AnyType &atRef) const;

    /**
     * allow obtaining the AnyType corresponding to accessing the n-th element at this level;
     */
    ErrorManagement::ErrorType Access(uint32 index,AnyType &atRef,DynamicCString &name) const;

    /**
     *
     */
    ErrorManagement::ErrorType IsLeaf(const AnyType &at,bool &isLeaf) const;


    // start position of each of the substructures addressed by the current path
    Stack<AnyType> referencesHistory;

    //
    Stack<uint32> pathLengthHistory;

    //
    DynamicCString navigationPath;

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/




}; // MARTe

#endif /* SOURCE_CORE_BAREMETAL_L2OBJECTS_ANYTYPESTRUCTUREDDATA_H_ */
