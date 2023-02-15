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
#include "ConfigurationDatabase.h"
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
     * Allow to control the behaviour of the defualt destructor. default value false, but if desired the static variale can be set to 1.
     */
    static bool allowDestructor;

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

    /**
     * @brief Registers a structure as defined by a StructuredDataI tree.
     * @details The StructuredDataI is recursively traversed and an equivalent structure constructed based on the leaf types.
     * Each node will be registered as an individual structure. If the node contains a leaf named \a typeIdentifier,
     * this will be used as the type of the structure to be associated to that node.
     *
     * Example:
     * <pre>
     *   A1 = {
     *     c1 = 2
     *     A2 = {
     *       d2 = 2.0
     *     }
     *     A3[0] = {
     *       f1 = -1
     *     }
     *     A3[1] = {
     *       f1 = 2
     *     }
     *   }
     * </pre>
     * Is registered as:
     * <pre>
     *   struct A2 {
     *     float32 d2;
     *   };
     *   struct A3 {
     *     int32 f1;
     *   };
     *   struct A1 {
     *     uint32 c2;
     *     A2 A2;
     *     A3 A3[2];
     *   };
     * </pre>
     *
     * If typeIdentifier had been set to e.g. RegisterType:
     *
     * <pre>
     *   A1 = {
     *     RegisterType = MainStruct
     *     c1 = 2
     *     A2 = {
     *       RegisterType = SubStruct
     *       d2 = 2.0
     *     }
     *     A3[0] = {
     *       RegisterType = SubStruct2
     *       f1 = -1
     *     }
     *     A3[1] = {
     *       f1 = 2
     *     }
     *   }
     * </pre>
     * Would be registered as:
     * <pre>
     *   struct SubStruct {
     *     float32 d2;
     *   };
     *   struct MainStruct {
     *     uint32 c2;
     *     SubStruct A2;
     *     SubStruct2 A3[2];
     *   };
     * </pre>
     *
     * @param[in] in the structure to be registered, already moved inside the parent node to be registered
     * (i.e. in.GetName() should return the type of the parent structure to register).
     * @param[in] typeIdentifier a keyword which allows to detect type definitions inside the nodes.
     * @return true if the structure can be successfully registered.
     */
    static bool RegisterStructuredDataI(StructuredDataI &in, const char8 * const typeIdentifier = NULL_PTR(const char8 * const));

private:
    /**
     * @brief Recursive implementation of RegisterStructuredDataI.
     * @param[in] in see RegisterStructuredDataI.
     * @param[out] typesCDB holds the list of types found while traversing the \a in StructuredDataI.
     * @param[in] typeIdentifier see typeIdentifier.
     * @return see RegisterStructuredDataI.
     */
    static bool RegisterStructuredDataIPriv(StructuredDataI &in, ConfigurationDatabase &typesCDB, const char8 * const typeIdentifier = NULL_PTR(const char8 * const));

    /**
     * Helper class which wraps the structure declared @ Initialise.
     */
    /*lint -e{1790} ClassRegistryItem is not a true virtual parent. This is just a private wrapper.*/
    class ClassRegistryItemConfigurationStructureLoader : public ClassRegistryItem {
    public:
        /**
         * @brief See ClassRegistryItem::ClassRegistryItem(ClassProperties)
         */
        ClassRegistryItemConfigurationStructureLoader(StreamString typeNameIn, uint32 totalSizeIn);
        /**
         * @brief See ClassRegistryItem::~ClassRegistryItem()
         */
        ~ClassRegistryItemConfigurationStructureLoader();
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
        /*lint -e{9150} the struct MemberInfo is effectively a POD*/
    };

    /**
     * Information about the members
     */
    MemberInfo **memberInfo;
    /*lint -e{1712} default constructor not required ClassRegistryItemConfigurationStructureLoaderas as it is just a wrapper*/
};
}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* INTROSPECTIONSTRUCTURE_H_ */

