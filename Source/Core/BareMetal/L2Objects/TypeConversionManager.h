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
#include "TypeDescriptor.h"
#include "GlobalObjectI.h"
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
class DLL_API TypeConversionManager: public GlobalObjectI{

public:

	/**
     * @brief allow access to optimal functor for data conversion
	 * The operator must be destroyed at the end of its use
	 */
	const TypeConversionOperatorI *GetOperator(const TypeDescriptor &destTd,const TypeDescriptor &sourceTd) const ;

    /**
     * @brief Singleton access to the database.
     * @return a pointer to the database.
     */
    static TypeConversionManager &Instance();

    /**
    * @brief Destructor.
    */
    virtual ~TypeConversionManager();

    /**
     * @see Object::GetClassName
     * @return "TypeConversionManager".
     */
    virtual CCString  GetClassName() const;

    /**
     * @brief allows registering a factory to the manager
     * @details. factory must be a pointer to a static object.
     * @return true if registration successful.
     */
    bool Register(TypeConversionFactoryI *factory);

private:

    /**
     * Allows registering up to 32 factories
     */
    SimpleStaticListT<TypeConversionFactoryI*,MaximumNumberOfFactories> factories;

    /**
     * @brief Default constructor.
     */
    /*lint -e{1704} private constructor for singleton implementation*/
    TypeConversionManager();

    /**
     * @brief Disallow the usage of new.
     * @param[in] size the size of the object.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The new operator must wrap GlobalObjectsI::new(*) */
    static void *operator new(const osulong size) throw ();

    /**
     * @brief Frees the memory area pointed by \a p previously allocated on the StandardHeap.
     * @param[in] p is the pointer to be freed.
     */
    /*lint -e{1511} [MISRA C++ Rule 2-10-2]. Justification: The new operator must wrap GlobalObjectsI::delete(*) */
    static void operator delete(void * const p);
};



/*---------------------------------------------------------------------------*/
/*                        Inline method definitions                          */
/*---------------------------------------------------------------------------*/

} // MARTe

#endif /* TYPECONVERTERMANAGER_H_ */
