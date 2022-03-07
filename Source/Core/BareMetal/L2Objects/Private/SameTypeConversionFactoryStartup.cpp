/**
 * @file SameTypeConversionFactory.cpp
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


#include "MemoryOperators.h"
#include "TypeConversionFactoryI.h"
#include "TypeConversionManager.h"
#include "StartupManager.h"


namespace MARTe{

/**
 * @brief simple copy functor
 */
class CopyTCO: public TypeConversionOperatorI{
public:

	/**
	 * @brief constructor
	 */
	CopyTCO(uint32 dataSizeIn,bool isCompare);

	/**
	 */
	virtual ~CopyTCO();

	/**
	 * @brief data conversion method
	 */
	virtual ErrorManagement::ErrorType Convert(uint8 *dest, const uint8 *source,uint32 numberOfElements) const;

private:
	/*
	 * @brief size of one element
	 */
	uint32 dataSize;

	/*
	 * @brief if comparison not copy
	 */
	const bool compare;

	/**
	 * @brief disable its use
	 */
	void operator=(const CopyTCO &src){}

};


class SameTypeConversionFactory: public TypeConversionFactoryI{

public:

	/**
 	 * @brief Default constructor. NOOP.
 	 */
	SameTypeConversionFactory();

    /**
     * @brief Default destructor.
     */
    virtual ~SameTypeConversionFactory();

	/**
     * @brief allow access to optimal functor for data conversion
	 *
	 */
	TypeConversionOperatorI *GetOperator(VariableDescriptor &destVd,VariableDescriptor &sourceVd,bool isCompare);

private:

} sameTypeConversionFactory;


INSTALL_STARTUP_MANAGER_INITIALISATION_ENTRY(SameTypeConversionFactory,("TCMService",emptyString),("TCMDataBase",emptyString))

ErrorManagement::ErrorType SameTypeConversionFactoryStartup::Init(){
	ErrorManagement::ErrorType ret;

	ret.initialisationError = !TypeConversionManager::Register(&sameTypeConversionFactory);

	return ret;
}

ErrorManagement::ErrorType SameTypeConversionFactoryStartup::Finish(){
	ErrorManagement::ErrorType ret;

	TypeConversionManager::Clean();

	return ret;
}


/************************************************************************************************/
/*   IMPLEMENTATION */
/************************************************************************************************/


SameTypeConversionFactory::SameTypeConversionFactory(){
}

SameTypeConversionFactory::~SameTypeConversionFactory(){
}

TypeConversionOperatorI *SameTypeConversionFactory::GetOperator(VariableDescriptor &destVd,VariableDescriptor &sourceVd,bool isCompare){
	TypeConversionOperatorI *tco = NULL_PTR(TypeConversionOperatorI *);

	// sets the sourceTd dataIsConstant to be false
	// this way if the test passes we are also sure that we can write to
	// also the test works whatever the const status of the source
	TypeDescriptor sourceTd = sourceVd.GetFinalTypeDescriptor();
    TypeDescriptor destTd   = destVd.GetFinalTypeDescriptor();
    sourceTd.SetDataConstant(false);

	//compare source and dest
	if (destTd.SameTypeAndSizeAs(sourceTd)){
		if (destTd.IsBasicType() || destTd.IsStructuredData()){
			tco = new CopyTCO(sourceTd.StorageSize(),isCompare);
		}
	}

	return tco;
}


CopyTCO::CopyTCO(uint32 dataSizeIn,bool isCompare):compare(isCompare){
	dataSize = dataSizeIn;
}


CopyTCO::~CopyTCO(){

}

ErrorManagement::ErrorType CopyTCO::Convert(uint8 *dest, const uint8 *source,uint32 numberOfElements) const{
	ErrorManagement::ErrorType  ok;
	uint32 toCopySize = dataSize * numberOfElements;
	if (!compare){
		ok = Memory::Copy(dest,source,toCopySize);
	} else {
		// compare returns true if the same or comparisonFailure if different
		ok = Memory::Compare(dest,source,toCopySize);

	}
	return ok;
}


} //MARTe
