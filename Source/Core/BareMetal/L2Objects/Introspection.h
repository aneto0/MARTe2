/**
 * @file Introspection2.h
 * @brief Header file for class Introspection2
 * @date Nov 18, 2016
 * @author fsartori
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

 * @details This header file contains the declaration of the class Introspection2
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef L2OBJECTS_INTROSPECTION2_H_
#define L2OBJECTS_INTROSPECTION2_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

//#include "IntrospectionEntry2.h"
#include "ZeroTerminatedArray.h"
#include "ClassRegistryItemT.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

class IntrospectionEntry{

};

/**
 * @brief Groups the information about each member of a class or a structure.
 */
/*lint -e{9109} forward declaration of this class is required in other modules*/
template <class T>
class DLL_API Introspection: public ZeroTerminatedArray<IntrospectionEntry> {

    /**
     *
     */
    Introspection(IntrospectionEntry *entries,ClassRegistryItem *record):ZeroTerminatedArray<IntrospectionEntry>(entries) {
        if ()
        record->SetIntrospection(this);
    }

};

#define DECLARE_CLASS_INTROSPECTION(className, introEntryArray) \
    static MARTe::ClassProperties className ## _ ## introspection_properties( #className , typeid(className).name(), "", static_cast<MARTe::uint32>(sizeof(className))); \
    static MARTe::Introspection   className ## _ ## introspection(introEntryArray, ClassRegistryItemT<className>::GenericInstance(className ## _ ## introspection_properties));


/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/
}
#endif /* L2OBJECTS_INTROSPECTION2_H_ */
	
