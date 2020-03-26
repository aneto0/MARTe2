/**
 * @file TypeConversionManager.cpp
 * @brief Header file for class AnyType
 * @date 11 Nov 2017
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


#define DLL_API

#include "TypeConversionManager.h"


namespace MARTe{

namespace TypeConversionManager{

    /**
     * Allows registering up to 32 factories
     */
    SimpleStaticListT<TypeConversionFactoryI*,MaximumNumberOfFactories> factories;

    const TypeConversionOperatorI *GetOperator(const TypeDescriptor &destTd,const TypeDescriptor &sourceTd,bool isCompare) {
    	uint32 ix = 0;
    	TypeConversionOperatorI *tco = NULL_PTR(TypeConversionOperatorI *);
    	while ( (ix < factories.NumberOfUsedElements()) && (tco == NULL)){
    		TypeConversionFactoryI *tcf = factories.Get(ix);
    		tco = tcf->GetOperator(destTd,sourceTd,isCompare);
    		ix++;
    	}

    	return tco;
    }

    bool Register(TypeConversionFactoryI *factory){
    	bool ret = false;
    	uint32 n = factories.NumberOfUsedElements();
    	if (n < factories.NumberOfElements()){
    		ret = factories.Add(factory);
    	}
    	return ret;
    }

    void Clean(){
    	for (uint32 i = 0; i < factories.NumberOfUsedElements();i++){
    		factories[i] = NULL_PTR(TypeConversionFactoryI*);
    	}
    }


//const FormatDescriptor TypeConversionOperatorI::format;


} //TypeConversionManager
} //MARTe
