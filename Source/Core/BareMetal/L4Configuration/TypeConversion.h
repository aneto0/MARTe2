/**
 * @file TypeConversion.h
 * @brief Header file for class TypeConversion
 * @date 28/10/2015
 * @author Andre' Neto
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

 * @details This header file contains the declaration of the class TypeConversion
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
 */

#ifndef TYPECONVERSION_H_
#define TYPECONVERSION_H_

/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "AnyType.h"
#include "Vector.h"
#include "Matrix.h"
#include "StreamI.h"
#include "StreamString.h"
#include "StringHelper.h"
#include "FormatDescriptor.h"
#include "MemoryOperationsHelper.h"

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe {

/**
 * @brief Converts between two types.
 *
 * @details The allowed basic type conversions are between the following types:\n
 *   (u)int{8,16,32,64} | float{32,64} | StreamString | char8[1:127] | char8 * (char array created on heap) | Vector<char8>;\n
 * It is possible converting vectors of these types between them if the number of elements in the two vectors is the same:\n
 *   (u)int{8,16,32,64}[] | float{32,64}[] | StreamString[] | char8[1:127][1:127] | char8 ** (memory created on heap) | Matrix<char8>;\n
 * It is possible converting matrices of these types between them if the numbers of rows and columns in the two matrices are the same:\n
 *   (u)int{8,16,32,64}[][] | float{32,64}[][] | StreamString[][] | char8[1:127][1:127][1:127] | char8 *** (memory created on heap);\n
 *
 * @details It is possible converting one class or structure to another. The classes must be introspectable and registered into the
 * ClassRegistryDatabase. The conversion between two objects will return true if they have the same number of members and if each member
 * of the source is convertible to the respective member of the destination.
 *
 * @details It is possible converting a StructuredDataI (i.e a ConfigurationDatabase) to a class or structure. The destination class must be
 * introspectable and registered into the ClassRegistryDatabase. If the StructuredDataI source represents a class introspectable and registered into
 * the ClassRegistryDatabase the members will be converted respecting the order in the class declaration, otherwise the members will be
 * converted respecting the order of the elements in the StructuredDataI source.
 *
 * @details It is possible converting a class or structure into a StructuredDataI (i.e ConfigurationDatabase). The source class must be
 * introspectable and registered into the ClassRegistryDatabase.
 *
 * @details It is possible the conversion between two StructuredDataI. See StructuredDataI::Copy().
 *
 * @param[out] destination the converted type.
 * @param[in] source the type to be converted.
 * @return true if the conversion succeeds, false otherwise.
 * on each dimension is the same. Finally, for each element, the conversion from the source
 * type to the destination must be supported and successful.
 * @pre
 *   source.GetNumberOfElements([0:2]) == destination.GetNumberOfElements([0:2]);\n
 *   In case of source or destination representing a char8 array or table, the size must be minor than 127 characters for each dimension.
 * @post
 *   destination holds the converted type of source
 */
DLL_API bool TypeConvert(const AnyType &destination,
                         const AnyType &source);

}

/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

#endif /* TYPECONVERSION_H_ */

