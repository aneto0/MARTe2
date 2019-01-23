/**
 * @file StaticListHolder.h
 * @brief array of registered classes - used by ClassRegistryDatabase
 * @date 31/08/2015
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

 * @details This header file contains the declaration of the class StaticListHolder
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */
#ifndef CLASSREGISTRYINDEX_H_
#define CLASSREGISTRYINDEX_H_




/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "TypeDescriptor.h"
#include "SimpleStaticListT.h"
#include "CCString.h"
#include "ClassRegistryItemI.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class ClassRegistryItem;

/**
 * @brief ordered array of pointers to class information records
 * @details Used inside Level2 ClassRegistryDatabase. Singleton use.
 * Implemented actually in level2 as part of the ClassRegistryDatabase.
 */
class DLL_API ClassRegistryIndex {

public:

	/**
	 * the number of bits addressing the first index
	 */
	static const uint32 IndexBits = 8;

	/**
	 * number of elements on the first index
	 */
	static const uint32 IndexSize = (1 << IndexBits);

	/**
	 * the number of bits addressing the second index
	 */
	static const uint32 IndexCellBits = 8;

	/**
	 * number of elements on the second index
	 */
	static const uint32 IndexCellSize = (1 << IndexCellBits);

	/**
	 * number of elements on the second index
	 */
	static const uint32 IndexCellMask = (IndexCellSize - 1);

	/**
	 * number of elements on the second index
	 */
	static const uint32 MaxIndexSize = (IndexSize * IndexCellSize);

	/**
	 * rename a template class
	 */
	typedef SimpleStaticListT<ClassRegistryItemI *,IndexCellSize> ClassRegistryIndexCell;

    /**
     * @brief Singleton access to the database.
     * @return a reference to the database.
     */
    static ClassRegistryIndex *Instance();


    /**
     * @brief Destructor. Removes all the elements hold by the database.
     */
    virtual                  ~ClassRegistryIndex();

    /**
     * @brief Allows browsing the database.
     * @return NULL if index is outside range
     *
     */
    ClassRegistryItemI *operator[] (uint32 classRegistrationNo);

    /**
     * @brief Access to basic information of a class
     * @return NULL if index is outside range
     *
     */
    uint32 GetClassSize (uint32 classRegistrationNo);

    /**
     * @brief Access to basic information of a class
     * @return NULL if index is outside range
     *
     */
    TypeDescriptor GetTypeDescriptor(CCString name);

    /**
     * @brief Access to full information of a class
     * @return NULL if index is outside range
     *
     */
    inline ClassRegistryItem * GetClassRegistryItem (uint32 classRegistrationNo);

    /**
     * @brief Adds one new ClassRegistryItem to the database.
     * @details This method should only be called by the ClassRegistryItem constructor.
     * After adding the element to the database the ClassRegistryItem unique identifier value is set
     * to the position at which it was added to the database.
     * Does not move memory, may allocate some.
     * The access is not protected as it is assumed to be called only
     * during initial initialisation of static objects.
     * @param[in] cri the element to be added.
     * @return the index number of the object. Returns 0xFFFFFFFF in case of error
     */
    uint32 Add(ClassRegistryItemI * const crii,uint32 size);

    /**
     * @return number of classes in database
     */
    uint32 NumberOfRegisteredClasses();

private:

    /**
     * @brief private constructor: use Instance();
     */
    ClassRegistryIndex();

    /**
     * @brief disable copy constructor;
     */
    inline ClassRegistryIndex(const ClassRegistryIndex &);

    /**
     * @brief Returns free slots at the current index. If zero adds a new
     * ClassRegistryIndexCell and returns its size
     * @return zero if no new slot could be created or no existing one were available
     */
    uint32 AllocateFreeSlots();

    /**
     * The actual database
     */
    SimpleStaticListT<ClassRegistryIndexCell*,IndexSize> index;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

ClassRegistryIndex::ClassRegistryIndex(const ClassRegistryIndex &){}

ClassRegistryItem *  ClassRegistryIndex::GetClassRegistryItem (uint32 classRegistrationNo){
	ClassRegistryItemI *crii =  (*this)[classRegistrationNo];
	return crii->GetBasePtr();
}


}
#endif /* CLASSREGISTRYINDEX_H_ */
