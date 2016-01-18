/**
 * @file Introspection.h
 * @brief Header file for class Introspection
 * @date 05/08/2015
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

 * @details This header file contains the declaration of the class Introspection
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTROSPECTION_H_
#define INTROSPECTION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "IntrospectionEntry.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * This macro has to be inserted in every unit file due to add an introspection class to the ClassRegistryDatabase.
 * The definition of the  _## className ## ClassRegistryItem variable will
 * instantiate a new ClassRegistryItem for every unit file compiled in the application.
 * Upon instantiation each ClassRegistryItem will automatically add itself to the ClassRegistryDatabase.
 */
#define INTROSPECTION_CLASS_REGISTER(className,ver,introspection)                                                      \
    /*                                                                                                                 \
     * Class properties of this class type. One instance per class type automatically instantiated at the start        \
     * of an application or loading of a loadable library.                                                             \
     * e.g. static ClassProperties MyClassTypeClassProperties_("MyClassType", typeid(MyClassType).name(), "1.0");      \
     */                                                                                                                \
    static MARTe::ClassProperties className ## ClassProperties_( #className , typeid(className).name(), ver);                 \
    /*                                                                                                                 \
     * Class registry item of this class type. One instance per class type automatically instantiated at the start     \
     * of an application or loading of a loadable library. It will automatically add the class type to the             \
     * ClassRegistryDatabase.                                                                                          \
     * e.g. static ClassRegistryItem MyClassTypeClassRegistryItem_( MyClassTypeClassProperties_, &MyClassTypeBuildFn_);\
     */                                                                                                                \
    static MARTe::ClassRegistryItem className ## ClassRegistryItem_( className ## ClassProperties_, introspection);




/**
 * @brief Describes the class
 */
class  Introspection {

public:

    Introspection(IntrospectionEntry **);

private:

    /**
     * An array of pointer to the class member's descriptors.
     * The array must be zero-terminated.
     */
    IntrospectionEntry ** fields;

};



}
/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#define DECLARE_CLASS_MEMBER(className, memberName, memberByteOffset, member, modifierString ) \
    static const ClassStructureItem className ## _ ## memberName ## _classStructureItem =      \
    {                                                                                          \
        #memberName,                                                                           \
        className.member,                                                                      \
        memberByteOffset,                                                                      \
        modifierString                                                                         \
    };

#define introspectionMemberIndex(className, fieldName) \
    &(((className *)0)->fieldName)


#define introspectionMemberSize(className, fieldName) \
    sizeof(((className *)0)->fieldName)


#endif /* INTROSPECTION_H_ */

