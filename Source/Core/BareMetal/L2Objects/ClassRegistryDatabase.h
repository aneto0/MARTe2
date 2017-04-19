/**
 * @file ClassRegistryDatabase.h
 * @brief Header file for class ClassRegistryDatabase
 * @date 04/08/2015
 * @author Andre Neto
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
 *
 * @details This header file contains the declaration of the class ClassRegistryDatabase
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSREGISTRYDATABASE_H_
#define CLASSREGISTRYDATABASE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "ClassRegistryItem.h"
#include "GlobalObjectsDatabase.h"
#include "FastPollingMutexSem.h"
#include "StaticList.h"
#include "FractionalInteger.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Database of framework base classes.
 * @details Most of the framework user classes inherit from Object. As a
 * consequence, they have the property of being automatically instantiated
 * and managed by the framework.
 *
 * Every class that inherits from Object will be described by a
 * ClassRegistryItem and automatically added to a ClassRegistryDatabase. This
 * database can then be used to retrieve information about the registered classes.
 */
class DLL_API ClassRegistryDatabase: public GlobalObjectI {

public:

    /**
     * @brief Singleton access to the database.
     * @return a reference to the database.
     */
    static ClassRegistryDatabase *Instance();

    /**
     * @brief Destructor. Removes all the elements hold by the database.
     */
    virtual                  ~ClassRegistryDatabase();
    /**
     * @brief Adds an element to the database.
     * @details This method should only be called by the ClassRegistryItem constructor.
     * After adding the element to the database the ClassRegistryItem unique identifier value is set
     * to the position at which it was added to the database.
     * @param[in] p the element to be added.
     */
    void                     Add(ClassRegistryItem * const p);

    /**
     * @brief Returns the ClassRegistryItem associated to the class with name \a className.
     * @details The returned pointer will be valid as long as it exists in the database.
     * @param[in] className the name of the class to be searched.
     * @return a pointer to the ClassRegisteredItem or NULL if the \a className could not be found.
     */
    ClassRegistryItem       *Find(CCString className);

    /**
     * @brief Returns the ClassRegistryItem associated to the class with given  \a targetTd.
     * @details The returned pointer will be valid as long as it exists in the database.
     * @param[in] targetTd the typedescriptor of the class to be searched.
     * @return a pointer to the ClassRegisteredItem or NULL if the \a targetTd could not be found.
     */
    ClassRegistryItem       *Find(const TypeDescriptor &targetTd);

    /**
     * @brief Returns the ClassRegistryItem associated to the class with typeid(class).name() equal to \a typeidName.
     * @details The returned pointer will be valid as long as it exists in the database.
     * @param[in] typeidName the typeid().name() of the class to be searched.
     * @return a pointer to the ClassRegisteredItem or NULL if the \a className could not be found.
     */
    ClassRegistryItem       *FindTypeIdName(CCString const typeidName);

    /**
     * @brief Returns the number of classes registered in the database.
     * @return the number of classes registered in the database.
     */
    uint32                  GetSize();

    /**
     * @brief Returns the ClassRegistryItem at position \a idx.
     * @param[in] idx the index of the ClassRegistryItem to be retrieved.
     * @return the ClassRegistryItem at position \a idx or NULL if no element exists at that position.
     *
     * @pre idx>=0 && position < GetSize()
     */
    const ClassRegistryItem *Peek(const uint32 &idx);

    /**
     * @brief Returns "ClassRegistryDatabase"
     * @return "ClassRegistryDatabase".
     */
    virtual CCString const GetClassName() const;

    /**
     * @brief Cleanup the database.
     */
    void CleanUp();

protected:

    /**
     * @brief Locks the internal spin-lock mutex semaphore.
     */
    bool Lock();

    /**
     * @brief Unlocks the internal spin-lock mutex semaphore.
     */
    void UnLock();

    /**
     * @brief Private Constructor.
     */
    /*lint -e{1704} private constructor for singleton implementation*/
    ClassRegistryDatabase();

private:

    /**
     * TODO
     * Locks database and searches using given filter
     */
    ClassRegistryItem *ClassRegistryDatabase::Find(  SearchFilterT<ClassRegistryItem> &  finder);


    /**
     * The database is implemented as a StaticList.
     * The destructor of the list will clean its elements.
     */
    LinkedListHolderT<ClassRegistryItem>  classDatabase;

    /**
     * Protects the concurrent access to the database
     */
    FastPollingMutexSem                   mux;

};

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* CLASSREGISTRYDATABASE_H_ */
