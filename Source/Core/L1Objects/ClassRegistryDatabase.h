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
#include "LinkedListHolder.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/
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
class ClassRegistryDatabase {

public:
    /**
     * @brief Singleton access to the database.
     * @return a reference to the database.
     */
    static ClassRegistryDatabase &Instance();

    /**
     * @brief Destructor. Removes all the elements hold by the database.
     */
    ~ClassRegistryDatabase();

    /**
     * @brief Removes an element from the database.
     * @details This method should only be called by the
     * ClassRegistryItem destructor.
     * @param[in] p the element to be removed.
     * @return true if the element was successfully removed from the database.
     */
    bool Delete(ClassRegistryItem * const p);

    /**
     * @brief Adds an element to the database.
     * @details This method should only be called by the
     * ClassRegistryItem constructor.
     * @param[in] p the element to be added.
     */
    void Add(ClassRegistryItem * const p);

    /**
     * @brief Returns the ClassRegistryItem associated to the class with name \a className.
     * @details The returned pointer will be valid as long as it exists in the database.
     * @param[in] className the name of the class to be searched.
     * @return a pointer to the ClassRegisteredItem or NULL if the \a className could not be found.
     */
    ClassRegistryItem *Find(const char8 *className);

    /**
     * @brief Returns an access point to the database root.
     * @return a pointer to the database root which can be used to scan the database.
     */
    ClassRegistryItem *List();

    /**
     * @brief Returns the number of classes registered in the database.
     * @return the number of classes registered in the database.
     */
    uint32 Size() const;

    /**
     * @brief Returns the element at position idx.
     * @param idx the index of the ClassRegistryItem to be retrieved.
     * @return the ClassRegistryItem at position idx or NULL if there is no element at that position.
     */
    const ClassRegistryItem *ElementAt(const uint32 &idx);
private:
    /**
     * @brief Private Constructor.
     */
    /*lint -e{1704} private constructor for singleton implementation*/
    ClassRegistryDatabase();

    /**
     * The database is implemented as a LinkedListHolder
     */
    LinkedListHolder classDatabase;
};

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
#endif /* CLASSREGISTRYDATABASE_H_ */
