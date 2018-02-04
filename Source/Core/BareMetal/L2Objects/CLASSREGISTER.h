/**
 * @file CLASSREGISTER.h
 * @brief Header file for class CLASSREGISTER
 * @date 14/07/2016
 * @author Ivan Herrero
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
 * @details This header file contains the declaration of the class CLASSREGISTER
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef CLASSREGISTER_H_
#define CLASSREGISTER_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

#include <typeinfo>

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

/**
 * These macros are required for the application to automatically register in the ClassRegistryDatabase
 * the information associated to every class that inherits from Object.
 * These have to be defined as macros given that the new and delete functions are specific for each
 * final class that inherits from Object (new and delete are static and thus cannot be virtual).
 */
/**
 * The function GetClassPropertiesCopy has to be virtual in order to guarantee that
 * there will be a virtual member in all inherited classes.
 * This macro has to be inserted in every class that inherits from Object.
 */
/*lint -save -e9026 -e9024 -e9023 -e9141 -estring(1960,"*6-2-1*")
 * 9026: function-like macro defined.
 * 9024: '#/##' operators used in macro.
 * 9023: (Multiple use of '#/##' operators in definition of macro) an exception to this rule.
 * is only applied in the definition of the ClassRegistryItem_.
 * 9141: Disable global declaration of symbols.
 * 1960: lint is confused with the placement new and reporting a false alarm.
 */
#define CLASS_REGISTER_DECLARATION()                                                                                   \
    /*                                                                                                                 \
     * Collects all the class informations (Introspection, ClassProperties, ...) and the build function due to         \
     * make new instances of the class in runtime                                                                      \
     */                                                                                                                \
     /*lint -e{1516} This function will be redeclared in descendants */                                                \
     virtual MARTe::ClassRegistryItem * GetClassRegistryItem() const ;                                                 \
    /*                                                                                                                 \
     * Retrieves the ClassRegistryItem static attribute                                                                \
     */                                                                                                                \
     /*lint -e{1511} This function will be redeclared in descendants */                                                \
     static MARTe::ClassRegistryItem * GetClassRegistryItem_Static()  ;                                                \
    /*                                                                                                                 \
     * @brief Allocates a new instance of the class type in the provided heap. Note that this                          \
     * is automatically called by the compiler (given that we use placement new).                                      \
     * Note that the selected heap might be different for each type of class.                                          \
     * @param[in, out] destination the destination where to copy the class properties to.                              \
     */                                                                                                                \
     /*lint -e{1511} This function will be redeclared in descendants */                                                \
     static void * operator new(const MARTe::osulong size, MARTe::HeapI *heap = static_cast<MARTe::HeapI *>(NULL));    \
    /*                                                                                                                 \
     * @brief Delete the object.                                                                                       \
     * @details Will delegate the deleting of the object to the correct heap. Note that the delete function            \
     * cannot call non-static members and as a consequence the heap variable must have global                          \
     * scope in the unit file (but is not exported) (see CLASS_REGISTER).                                              \
     * @param[in] p the pointer to the object to be deleted.                                                           \
     */                                                                                                                \
     /*lint -e{1511} This function will be redeclared in descendants */                                                \
      static void operator delete(void *p);

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#if 0
    /*                                                                                                                 \
     * Class properties of this class type. One instance per class type automatically instantiated at the start        \
     * of an application or loading of a loadable library.                                                             \
     * e.g. static ClassProperties MyClassTypeClassProperties_("MyClassType", typeid(MyClassType).name(), "1.0");      \
     */                                                                                                                \
    MARTe::ClassProperties className::classProperties( #className , typeid(className).name(), ver, static_cast<MARTe::uint32>(sizeof(className)));
#endif




/**
 * This macro has to be inserted in every unit file.
 * The definition of the  _## className ## ClassRegistryItem variable will
 * instantiate a new ClassRegistryItem for every unit file compiled in the application.
 * Upon instantiation each ClassRegistryItem will automatically add itself to the ClassRegistryDatabase.
 * When required the ClassRegistryItem will instantiate new objects by asking the ClassRegistryItem
 * for the build function related to the class it is representing.
 * Note that new and delete are static functions and as a consequence they cannot call member
 * functions in the class (or its derived classes). Because their implementation is specific to the final
 * class they had to be written as part of the macro as well.
 */
#define CLASS_REGISTER(className,ver)                                                                                  \
                                                                                                                       \
    /*                                                                                                                 \
     */                                                                                                                \
    MARTe::ClassRegistryItem * className::GetClassRegistryItem_Static() {                                              \
	return MARTe::ClassRegistryItem::Instance<className>();                                                       \
    }                                                                                                                  \
    /*                                                                                                                 \
     */                                                                                                                \
    MARTe::ClassRegistryItem * className::GetClassRegistryItem() const {                                               \
        return MARTe::ClassRegistryItem::Instance<className>();                                                       \
    }                                                                                                                  \
    /*                                                                                                                 \
     * e.g. void *MyClassType::operator new(const size_t size, Heap &heap);                                            \
     */                                                                                                                \
    void * className::operator new(const MARTe::osulong size, MARTe::HeapI *heap) {                              \
        void *obj = NULL_PTR(void *);                                                                                  \
        if (heap != NULL) {                                                                                            \
            obj = heap->Malloc(static_cast<MARTe::uint32>(size));                                                      \
        } else {                                                                                                       \
            obj = MARTe::HeapManager::Malloc(static_cast<MARTe::uint32>(size));                                        \
        }                                                                                                              \
        GetClassRegistryItem_Static()->IncrementNumberOfInstances();                                                   \
        return obj;                                                                                                    \
    }                                                                                                                  \
    /*                                                                                                                 \
     * e.g. void *MyClassType::operator delete(void *p);                                                               \
     */                                                                                                                \
    void className::operator delete(void *p) {                                                                         \
        bool ok = MARTe::HeapManager::Free(p);                                                                         \
        if(!ok){                                                                                                       \
            /* TODO error here */                                                                                      \
        }                                                                                                              \
        GetClassRegistryItem_Static()->DecrementNumberOfInstances();                                                   \
    }


#define TEMPLATE_CLASS_REGISTER(className,ver,tempDecl)                                                      \
                                                                                                                       \
    /*                                                                                                                 \
     */                                                                                                                \
	template <tempDecl>                                                                                                \
    MARTe::ClassRegistryItem * className::GetClassRegistryItem_Static() {                                   \
	return MARTe::ClassRegistryItem::Instance<className>();                                                            \
    }                                                                                                                  \
    /*                                                                                                                 \
     */                                                                                                                \
	template <tempDecl>                                                                                                \
    MARTe::ClassRegistryItem * className::GetClassRegistryItem() const {                                    \
        return MARTe::ClassRegistryItem::Instance<className>();                                                        \
    }                                                                                                                  \
    /*                                                                                                                 \
     * e.g. void *MyClassType::operator new(const size_t size, Heap &heap);                                            \
     */                                                                                                                \
	template <tempDecl>                                                                                                \
    void * className::operator new(const MARTe::osulong size, MARTe::HeapI *heap) {                           \
        void *obj = NULL_PTR(void *);                                                                                  \
        if (heap != NULL) {                                                                                            \
            obj = heap->Malloc(static_cast<MARTe::uint32>(size));                                                      \
        } else {                                                                                                       \
            obj = MARTe::HeapManager::Malloc(static_cast<MARTe::uint32>(size));                                        \
        }                                                                                                              \
        GetClassRegistryItem_Static()->IncrementNumberOfInstances();                                                   \
        return obj;                                                                                                    \
    }                                                                                                                  \
    /*                                                                                                                 \
     * e.g. void *MyClassType::operator delete(void *p);                                                               \
     */                                                                                                                \
	template <tempDecl>                                                                                                \
    void className::operator delete(void *p) {                                                              \
        bool ok = MARTe::HeapManager::Free(p);                                                                         \
        if(!ok){                                                                                                       \
            /* TODO error here */                                                                                      \
        }                                                                                                              \
        GetClassRegistryItem_Static()->DecrementNumberOfInstances();                                                   \
    }
/*lint -restore */

#endif /* CLASSREGISTER_H_ */

