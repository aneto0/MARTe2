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

#include "GlobalObjectI.h"
#include "SimpleStaticListT.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * declared and implemented in level2
 * Here only to allow defining pointers to it
 * */
class ClassRegistryItem;

/**
 * Just to provide the information of size at level 1!
 * information duplicated in *cri
 */
struct ClassRegistryBrief{

	/**
	 * pointer to ClassRegistryItem
	 */
	class ClassRegistryItem  *cri;

	/**
	 * sizeof ()
	 */
    uint32                    sizeOfClass;

    /**
     *
     */
    ClassRegistryBrief(){
    	cri = NULL_PTR(ClassRegistryItem *);
    	sizeOfClass = 0;
    }
};


/**
 * @brief ordered array of pointers to class information records
 * @details Used inside Level2 ClassRegistryDatabase. Singleton use.
 * Implemented actually in level2 as part of the ClassRegistryDatabase.
 */
class DLL_API ClassRegistryIndex: public GlobalObjectI {

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
	typedef SimpleStaticListT<ClassRegistryBrief,IndexCellSize> ClassRegistryIndexCell;

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
    inline ClassRegistryBrief *operator[] (uint32 classRegistrationNo);

    /**
     * @brief Access to basic information of a class
     * @return NULL if index is outside range
     *
     */
    uint32 GetClassSize (uint32 classRegistrationNo);

    /**
     * @brief Access to full information of a class
     * @return NULL if index is outside range
     *
     */
    ClassRegistryItem * GetClassRegistryItem (uint32 classRegistrationNo);

    /**
     * Adds one new ClassRegistryItem to the database.
     * Does not move memory, may allocate some.
     * The access is not protected as it is assumed to be called only
     * during initial initialisation of static objects.
     * @return the index number of the object. Returns 0xFFFFFFFF in case of error
     */
    uint32 Add(ClassRegistryItem *cri,uint32 size);

private:
    /**
     * @brief Returns free slots at the current index. If zero adds a new
     * ClassRegistryIndexCell and returns its size
     * @retun zero if no new slot could be created or no existing one were available
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


}
#endif /* CLASSREGISTRYINDEX_H_ */
