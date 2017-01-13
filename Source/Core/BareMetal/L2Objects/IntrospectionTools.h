/**
 * @file IntrospectionTools.h
 * @brief Header file for class IntrospectionTools
 * @date Jan 11, 2017
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

 * @details This header file contains the declaration of the class IntrospectionTools
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef INTROSPECTIONTOOLS_H_
#define INTROSPECTIONTOOLS_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CCString.h"
#include "StructuredDataI.h"
#include "ClassRegistryItem.h"
#include "Object.h"


/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Extracts the data of an input object and puts it into an object
 * which implements the StructuredDataI interface.
 * @details The returned StructuredDataI object is a tree which contains
 * a leaf for each one of the class's members.
 * If any of the members is of complex type, then a new node is created
 * instead of a leaf and a subtree is created. This happens recursively
 * for all members.
 * @param[in] ptr The pointer to the object whose data is expected to be
 * extracted.
 * @param[in] cri The ClassRegistryItem of the actual class of *ptr
 * @param[out] data The holder for the tree that contains the extracted
 */
bool ConvertDataToStructuredData(void const * const ptr,
                                 const ClassRegistryItem *cri,
                                 StructuredDataI& data);


/**
 * @brief Extracts the data of an input object and puts it into an object
 * which implements the StructuredDataI interface.
 * @details The returned StructuredDataI object is a tree which contains a
 * single node which represents the *ptr's data. This node is named with
 * the name of the object parameter and has a leaf with the className
 * parameter as a value, plus a leaf for each one of the class's members.
 * If any of the members is of complex type, then a new node is created
 * instead of a leaf and a subtree is created. This happens recursively
 * for all members.
 * @param[in] object The pointer to the object whose data is expected to be
 * extracted.
 * @param[out] data The holder for the tree that contains the extracted
 */
bool ConvertObjectToStructuredData(Object const & object,
                                 StructuredDataI& data);

/**
 * @brief Extracts the meta-data of an input object and puts it into an object
 * which implements the StructuredDataI interface.
 * @details The returned StructuredDataI object is a tree which contains
 * a leaf for each one of the class's members.
 * If any of the members is of complex type, then a new node is created
 * instead of a leaf and a subtree is created. This happens recursively
 * for all members.
 * @param[in] ptr The pointer to the object whose data is expected to be
 * extracted.
 * @param[in] cri The ClassRegistryItem of the actual class of *ptr
 * @param[out] data The holder for the tree that contains the extracted
 */
bool ConvertMetadataToStructuredData(void const * const ptr,
                                 const ClassRegistryItem *cri,
                                 StructuredDataI& data,
                                 const int32 recursionLevel = -1);


/**
 * @brief Extracts the meta-data of an input object and puts it into an object
 * which implements the StructuredDataI interface.
 * @details The returned StructuredDataI object is a tree which contains a
 * single node which represents the *ptr's data. This node is named with
 * the name of the object parameter and has a leaf with the className
 * parameter as a value, plus a leaf for each one of the class's members.
 * If any of the members is of complex type, then a new node is created
 * instead of a leaf and a subtree is created. This happens recursively
 * for all members.
 * @param[in] object The pointer to the object whose data is expected to be
 * extracted.
 * @param[out] data The holder for the tree that contains the extracted
 */
bool ConvertObjectMetadataToStructuredData(Object const & object,
                                 StructuredDataI& data,
                                 const int32 recursionLevel = -1);

#if 0
/**
 * @brief Extracts the metadata of an input object and puts it into an
 * object which implements the StructuredDataI interface.
 * @details The returned StructuredDataI object is a tree which contains
 * a single node which represents the *ptr's metadata.  This node is
 * named with the className parameter and has a sub node for each one of
 * the class's members. Those sub nodes, which represent each one a member,
 * have leaves for the metadata of the member, i.e. the type, the
 * modifiers, the attributes, the size, and the pointer to the member's
 * position. This happens recursively for all members, as deep as the
 * recursionlevel parameter allows.
 * @param[in] ptr The pointer to the object whose metadata is expected to
 * be extracted.
 * @param[in] className The name of the class of the object pointed by ptr.
 * @param[out] data The holder for the tree that contains the extracted
 * metadata of the object.
 * @param[in] level The level of recursion, hence the depth of the tree.
 */
bool ConvertMetadataToStructuredData(void * const ptr,
                                     CCString const className,
                                     StructuredDataI &data,
                                     const int32 recursionLevel = -1);

#endif


}

#endif /* INTROSPECTIONTOOLS_H_ */
	
