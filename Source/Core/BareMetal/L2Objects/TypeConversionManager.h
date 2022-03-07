/**
 * @file TypeConversionmanager.h
 * @brief Header file for class AnyType
 * @date 10 Nov 2017
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

 * @details This header file contains the declaration of the class AnyType
 * with all of its public, protected and private members. It may also include
 * definitions for inline methods which need to be visible to the compiler.
*/

#ifndef TYPECONVERSIONMANAGER_H_
#define TYPECONVERSIONMANAGER_H_



/*---------------------------------------------------------------------------*/
/*                        Standard header includes                           */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                        Project header includes                            */
/*---------------------------------------------------------------------------*/

#include "CompilerTypes.h"
#include "VariableDescriptor.h"
#include "SimpleStaticListT.h"
#include "TypeConversionFactoryI.h"

/*---------------------------------------------------------------------------*/
/*                          Forward declarations                             */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*                           Class declaration                               */
/*---------------------------------------------------------------------------*/

namespace MARTe{

/**
 * @brief limits the number of factories that can be registered
 */
static const uint32 MaximumNumberOfFactories = 32;

/**
 * @brief manager for data conversion functors
 */
namespace TypeConversionManager{

	/**
     * @brief allow access to optimal functor for data conversion
     * The operator must be destroyed at the end of its use
     * @param[in,out] destTd is the type of the destination variable to write to.
     * @param[in,out] sourceTd is the type of the source variable to write from.
	 */
	const TypeConversionOperatorI *GetOperator(const TypeDescriptor &destTd,const TypeDescriptor &sourceTd,bool isCompare) ;

    /**
     * @brief allow access to optimal functor for data conversion
     * The operator must be destroyed at the end of its use
     * @param[in,out] destVd is the type of the destination variable to write to.
     * as output a potentially different VariableDescriptor is returned corresponding
     * to the part of the variable structure the copy cannot handle
     * for instance if the operator can handle a vector of variables, in case of an array of
     * integers as input the output will be  a vector of generic arrays
     * AnnAnn(int32) --> Ann(generic array)
     * @param[in,out] sourceVd is the type of the source variable to write from.
     * as output a potentially different VariableDescriptor is returned corresponding
     * to the part of the variable structure the copy cannot handle
     * for instance if the operator can handle a vector of variables, in case of an array of
     * integers as input the output will be  a vector of generic arrays
     * AnnAnn(int32) --> Ann(generic array)
     */
    const TypeConversionOperatorI *GetOperator(VariableDescriptor &destVd,VariableDescriptor &sourceVd,bool isCompare) ;

    /**
     * @brief allows registering a factory to the manager
     * @details. factory must be a pointer to a static object.
     * @return true if registration successful.
     */
    bool Register(TypeConversionFactoryI *factory);

    /**
     * @brief removes all the TypeConversionOperatorI
     */
    void Clean();

};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* TYPECONVERTERMANAGER_H_ */
