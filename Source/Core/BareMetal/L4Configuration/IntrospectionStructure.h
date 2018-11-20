/**
 * @file IntrospectionStructure.h
 * @brief Header file for class IntrospectionStructure
 * @date 09/11/2018
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

 * @details This header file contains the declaration of the class IntrospectionStructure
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTROSPECTIONSTRUCTURE_H_
#define INTROSPECTIONSTRUCTURE_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/
#include "Object.h"
#include "StreamString.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Registers a structure as defined by a StructuredDataI.
 * @details Each node is a structure member and shall have the field Type defined. The name of the Object is the name of the structure to register.
 *
 * The field NumberOfElements allows to define multi-dimensional arrays of structures.
 * The Type can be a BasicType or any structured Type (possibly also defined by another IntrospectionStructure).
 * An example of a possible collection of structures would be (names only given as examples):
 * <pre>
 *   +MyTypes = {
 *     Class = ReferenceContainer
 *     +MyStructEx1 = { //name of the structured type to register.
 *       Class = IntrospectionStructure
 *       Field1 = {
 *         Type = uint32
 *         NumberOfElements = 1
 *       }
 *       Field2 = {
 *         Type = float32
 *         NumberOfElements = {3, 2} //3x2 matrix
 *       }
 *     }
 *     +MyStructEx2 = { //name of the structured type to register.
 *       Class = IntrospectionStructure
 *       Field1 = {
 *         Type = MyStructEx1
 *         NumberOfElements = {3, 2, 1} //3x1x2 matrix of MyStructEx1
 *       }
 *     }
 *   }
 * </pre>
 */
class IntrospectionStructure: public Object {
public:
    CLASS_REGISTER_DECLARATION()
    /**
     * @brief Constructor. NOOP.
     */
IntrospectionStructure    ();

    /**
     * @brief Destructor. NOOP.
     */
    virtual ~IntrospectionStructure();

    /**
     * @brief Registers the structure as defined in the class description.
     * @param[in] data the structure to be loaded as defined above.
     * @return true if the Type is defined in all the leafs, if the Type exists and if the Type to be registered is not already registed.
     */
    virtual bool Initialise(StructuredDataI &data);

private:
    /**
     * Helper class which wraps the structure declared @ Initialise.
     */
    class ClassRegistryItemConfigurationStructureLoader : public ClassRegistryItem {
    public:
        /**
         * @brief See ClassRegistryItem::ClassRegistryItem(ClassProperties)
         */
        ClassRegistryItemConfigurationStructureLoader(StreamString typeNameIn, uint32 totalSizeIn);
        /**
         * @brief See ClassRegistryItem::~ClassRegistryItem()
         */
        virtual ~ClassRegistryItemConfigurationStructureLoader();
        /**
         * @briefs Updates the class properties.
         */
        void Update(StreamString typeNameIn, uint32 totalSizeIn);
    private:
        /**
         * The name of the type
         */
        StreamString typeName;
        /**
         * The class size
         */
        uint32 totalSize;
        /**
         * The class properties
         */
        ClassProperties cp;
    };

    /**
     * The introspection entries for the structure
     */
    IntrospectionEntry **entries;

    /**
     * The structure members.
     */
    Introspection *introMembers;

    /**
     * The number of structure members
     */
    uint32 numberOfMembers;

    /**
     * Wraps the information about each member
     */
    struct MemberInfo {
        /**
         * Member name
         */
        StreamString memberName;

        /**
         * Member type
         */
        StreamString memberType;

        /**
         * Member modifier (see IntrospectionEntry.h)
         */
        StreamString memberModifier;
    };

    /**
     * Information about the members
     */
    MemberInfo **memberInfo;
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTROSPECTIONSTRUCTURE_H_ */

