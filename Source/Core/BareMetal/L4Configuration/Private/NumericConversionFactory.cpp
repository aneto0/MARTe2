/**
 * @file NumericConversionFactory.cpp
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

#include "TypeConversionFactoryI.h"
#include "TypeConversionManager.h"
#include "StreamString.h"
#include "BitSetToInteger.h"
#include "CompositeErrorManagement.h"

namespace MARTe{

/**
 * @brief copies to strings
 */
template <typename Tsource,typename Tdest>
class Number2NumberTCO: public TypeConversionOperatorI{
public:

	/**
	 * @brief constructor
	 */
	Number2NumberTCO(bool isCompare): compare(isCompare){
	}

	/**
	 * @brief destructor
	 */
	virtual  ~Number2NumberTCO(){
	}

	virtual ErrorManagement::ErrorType Convert(uint8 *dest, const uint8 *source,uint32 numberOfElements) const{
		 ErrorManagement::ErrorType ret;
		 bool outOfRange = false;
		 const Tsource *sourcet = reinterpret_cast<const Tsource *>(source);
		 Tdest *destt = reinterpret_cast<Tdest *>(dest);
		 for (uint32 i = 0u;(i < numberOfElements)&&ret;i++){
			 Tsource src = sourcet[i];
			 // more bits in the input format. Might need to saturate
			 if (TypeCharacteristics<Tsource>::UsableBitSize()>TypeCharacteristics<Tdest>::UsableBitSize()){
				 const Tsource maxSource = static_cast<Tsource>(TypeCharacteristics<Tdest>::MaxValue());
				 if (src > maxSource) {
					 src = maxSource;
					 outOfRange = true;
				 }
			 }
			 if (TypeCharacteristics<Tsource>::UsableNegativeBitSize()>TypeCharacteristics<Tdest>::UsableNegativeBitSize()){
				 const Tsource minSource = static_cast<Tsource>(TypeCharacteristics<Tdest>::MinValue());
				 if (src < minSource) {
					 src = minSource;
					 outOfRange = true;
				 }
			 }
			 if (ret){
				 Tdest dst = static_cast<Tdest>(src);
				 if (compare){
					 if (destt[i] != dst){
						 ret.comparisonFailure = true;
						 COMPOSITE_REPORT_ERROR(ret,'@',i,'(',destt[i],',',dst,')');
					 }
				 } else {
					 destt[i] = dst;
				 }
			 }
		 }

		 ret.outOfRange = outOfRange;
		 return ret;
	}

protected:
	/*
	 * @brief whether this is a comparison and not a copy
	 */
	bool compare;

private:
};

template <typename Tsource,typename Tdest>
class BitSet2BitSetTCO: public TypeConversionOperatorI{
public:

	/**
	 * @brief constructor
	 */
	BitSet2BitSetTCO(TypeDescriptor sourceTd,TypeDescriptor destTd,bool isCompare): compare(isCompare){
		srcNumberBitSize  = static_cast<uint8>(sourceTd.GetNumericBitSize());
		srcNumberBitShift = static_cast<uint8>(sourceTd.GetNumericBitOffset());
		srcIsSigned       = sourceTd.IsSigned();
		dstNumberBitSize  = static_cast<uint8>(destTd.GetNumericBitSize());
		dstNumberBitShift = static_cast<uint8>(destTd.GetNumericBitOffset());
		dstIsSigned       = destTd.IsSigned();
	}

	/**
	 * @brief destructor
	 */
	virtual  ~BitSet2BitSetTCO(){
	}
	virtual ErrorManagement::ErrorType Convert(uint8 *dest, const uint8 *source,uint32 numberOfElements) const{
		 ErrorManagement::ErrorType ret;
		 bool outOfRange = false;
		 const Tsource *sourcet = reinterpret_cast<const Tsource *>(source);
		 Tdest *destt = reinterpret_cast<Tdest *>(dest);
		 for (uint32 i = 0u;(i < numberOfElements)&&ret;i++){
			 // saturation is already performed by BStoBS
			 // choosing larger integer should suffice
			 // signedness does not matter as dealt by BStoBS anyway
			 if (sizeof(Tsource)>sizeof(Tdest)){
				 Tsource tmp;
				 BSToBS(&tmp,dstNumberBitShift,dstNumberBitSize,dstIsSigned,sourcet+i,srcNumberBitShift,srcNumberBitSize,srcIsSigned);
				 destt[i] = static_cast<Tdest>(tmp);
			 } else {
				 Tdest tmp = static_cast<Tdest>(sourcet[i]);
				 BSToBS(destt+i,dstNumberBitShift,dstNumberBitSize,dstIsSigned,&tmp,srcNumberBitShift,srcNumberBitSize,srcIsSigned);
			 }
// TODO Add Compare function
		 }

		 ret.outOfRange = outOfRange;
		 return ret;
	}

protected:
	/*
	 * @brief whether this is a comparison and not a copy
	 */
	bool compare;

	/**
	 * @brief bitshift of number
	 */
	uint8 srcNumberBitShift;
	/**
	 * @brief bitsize of number
	 */
	uint8 srcNumberBitSize;
	/**
	 * @brief whether it is signed
	 */
	bool srcIsSigned;
	/**
	 * @brief bitshift of number
	 */
	uint8 dstNumberBitShift;
	/**
	 * @brief bitsize of number
	 */
	uint8 dstNumberBitSize;
	/**
	 * @brief whether it is signed
	 */
	bool dstIsSigned;

};



template <typename Tsource,typename Tdest>
class BitSet2NumberTCO: public TypeConversionOperatorI{
public:

	/**
	 * @brief constructor
	 */
	BitSet2NumberTCO(TypeDescriptor td,bool isCompare): compare(isCompare){
		numberBitSize  = static_cast<uint8>(td.GetNumericBitSize());
		numberBitShift = static_cast<uint8>(td.GetNumericBitOffset());
		isSigned = td.IsSigned();
	}

	/**
	 * @brief destructor
	 */
	virtual  ~BitSet2NumberTCO(){
	}

	virtual ErrorManagement::ErrorType Convert(uint8 *dest, const uint8 *source,uint32 numberOfElements ) const{
		 ErrorManagement::ErrorType ret;
		 bool outOfRange = false;
		 const Tsource *sourcet = reinterpret_cast<const Tsource *>(source);
		 Tdest *destt = reinterpret_cast<Tdest *>(dest);
		 for (uint32 i = 0u;(i < numberOfElements)&&ret;i++){
			 Tsource src;
			 BSToBS(&src,0,sizeof(Tsource)*8,isSigned,sourcet+i,numberBitShift,numberBitSize,isSigned);
			 const uint8 usableBitSize = isSigned ? static_cast<uint8>(numberBitSize-1u) :numberBitSize;
			 const uint8 usableNegativeBitSize = isSigned ? static_cast<uint8>(numberBitSize-1u) :0u;

			 // more bits in the input format. Might need to saturate
			 if (usableBitSize > TypeCharacteristics<Tdest>::UsableBitSize()){
				 const Tsource maxSource = static_cast<Tsource>(TypeCharacteristics<Tdest>::MaxValue());
				 if (src > maxSource) {
					 src = maxSource;
					 outOfRange = true;
				 }
			 }
			 if (usableNegativeBitSize > TypeCharacteristics<Tdest>::UsableNegativeBitSize()){
				 const Tsource minSource = static_cast<Tsource>(TypeCharacteristics<Tdest>::MinValue());
				 if (src < minSource) {
					 src = minSource;
					 outOfRange = true;
				 }
			 }
			 if (ret){
				 Tdest dst = static_cast<Tdest>(src);
				 if (compare){
					 if (destt[i] != dst){
						 ret.comparisonFailure = true;
						 COMPOSITE_REPORT_ERROR(ret,'@',i,'(',destt[i],',',dst,')');

					 }

				 } else {
					 destt[i] = dst;
				 }
			 }
		 }

		 ret.outOfRange = outOfRange;
		 return ret;
	}

protected:
	/*
	 * @brief whether this is a comparison and not a copy
	 */
	bool compare;

	/**
	 * @brief bitshift of number
	 */
	uint8 numberBitShift;
	/**
	 * @brief bitsize of number
	 */
	uint8 numberBitSize;
	/**
	 * @brief whether it is signed
	 */
	bool isSigned;

};

template <typename Tsource,typename Tdest>
class Number2BitSetTCO: public TypeConversionOperatorI{
public:

	/**
	 * @brief constructor
	 */
	Number2BitSetTCO(TypeDescriptor td,bool isCompare): compare(isCompare){
		numberBitSize  = static_cast<uint8>(td.GetNumericBitSize());
		numberBitShift = static_cast<uint8>(td.GetNumericBitOffset());
		isSigned = td.IsSigned();
	}

	/**
	 * @brief destructor
	 */
	virtual  ~Number2BitSetTCO(){
	}

	virtual ErrorManagement::ErrorType Convert(uint8 *dest, const uint8 *source,uint32 numberOfElements ) const{
		 ErrorManagement::ErrorType ret;
		 bool outOfRange = false;
		 const Tsource *sourcet = reinterpret_cast<const Tsource *>(source);
		 Tdest *destt = reinterpret_cast<Tdest *>(dest);
		 for (uint32 i = 0u;(i < numberOfElements)&&ret;i++){
			 Tsource src = sourcet[i];

			 // more bits in the input format. Might need to saturate
			 if (TypeCharacteristics<Tsource>::UsableBitSize() > TypeCharacteristics<Tdest>::UsableBitSize()){
				 const Tsource maxSource = static_cast<Tsource>(TypeCharacteristics<Tdest>::MaxValue());
				 if (src > maxSource) {
					 src = maxSource;
					 outOfRange = true;
				 }
			 }
			 if (TypeCharacteristics<Tsource>::UsableNegativeBitSize() > TypeCharacteristics<Tdest>::UsableNegativeBitSize()){
				 const Tsource minSource = static_cast<Tsource>(TypeCharacteristics<Tdest>::MinValue());
				 if (src < minSource) {
					 src = minSource;
					 outOfRange = true;
				 }
			 }
			 Tdest dst = static_cast<Tdest>(src);

			 if (ret){
				 if (compare){
					 Tdest tmp;
					 BSToBS(&tmp,0,sizeof(Tdest)*8,isSigned,destt+i,numberBitShift,numberBitSize,isSigned);
					 if (tmp != dst){
						 ret.comparisonFailure = true;
						 COMPOSITE_REPORT_ERROR(ret,'@',i,'(',destt[i],',',dst,')');
					 }
				 } else {
					 BSToBS(destt+i,numberBitShift,numberBitSize,isSigned,&dst,0,sizeof(Tdest)*8,isSigned);
				 }
			 }
		 }

		 ret.outOfRange = outOfRange;
		 return ret;
	}

protected:
	/*
	 * @brief whether this is a comparison and not a copy
	 */
	bool compare;

	/**
	 * @brief bitshift of number
	 */
	uint8 numberBitShift;
	/**
	 * @brief bitsize of number
	 */
	uint8 numberBitSize;
	/**
	 * @brief whether it is signed
	 */
	bool isSigned;

};

class NumericConversionFactory: public TypeConversionFactoryI{

public:

	/**
 	 * @brief Default constructor. NOOP.
 	 */
	NumericConversionFactory();

    /**
     * @brief Default destructor.
     */
    virtual ~NumericConversionFactory();

	/**
     * @brief allow access to optimal functor for data conversion
	 *
	 */
	TypeConversionOperatorI *GetOperator(VariableDescriptor &destVd,VariableDescriptor &sourceVd,bool isReader);


private:

} numericConversionFactory;

NumericConversionFactory::NumericConversionFactory(){
}

NumericConversionFactory::~NumericConversionFactory(){
}


/**
 * Resolves only the main type - does not consider the bit ranges - disambiguated the earlier stage
 * To be used in a cascade of templates SelectTCO<SelectTCO<>>
 * Allows implementing a bi-dimensional lookup, from TD to Type
 */
template <class T3>
class FullSelectTCO{
public:

	template <class T1,class T2>
	static inline TypeConversionOperatorI *Do(const TypeDescriptor td,const TypeDescriptor td2,bool isCompare){
		TypeConversionOperatorI *tco = NULL_PTR(TypeConversionOperatorI *);

		TD_FullType fullType = td.GetFullTypeCode();
		uint32 storageSize = td.StorageSize();
		switch(fullType){
		case TDF_UnsignedInteger:{
			switch(storageSize){
			case 1:{
				tco =  T3::template Do<T2,uint8>(td2,td,isCompare);
			}break;
			case 2:{
				tco = T3::template Do<T2,uint16>(td2,td,isCompare);
			}break;
			case 4:{
				tco = T3::template Do<T2,uint32>(td2,td,isCompare);
			}break;
			case 8:{
				tco = T3::template Do<T2,uint64>(td2,td,isCompare);
			}break;
			default:{
			}
			}
		}break;
		case TDF_SignedInteger:{
			switch(storageSize){
			case 1:{
				tco = T3::template Do<T2,int8>(td2,td,isCompare);
			}break;
			case 2:{
				tco = T3::template Do<T2,int16>(td2,td,isCompare);
			}break;
			case 4:{
				tco = T3::template Do<T2,int32>(td2,td,isCompare);
			}break;
			case 8:{
				tco = T3::template Do<T2,int64>(td2,td,isCompare);
			}break;

			default:{
			}
			}
		}break;
		case TDF_Float:{
			switch(storageSize){
			case 4:{
				tco = T3::template Do<T2,float>(td2,td,isCompare);
			}break;
			case 8:{
				tco = T3::template Do<T2,double>(td2,td,isCompare);
			}break;
			default:{
			}
			}
		}break;
		default:{
		}
		}
		return tco;
	}
};

/**
 * Resolves only the main type - does not consider the bit ranges - disambiguated the last stage
 * To be used in a cascade of templates SelectTCO<SelectTCO<>>
 * Allows implementing a bi-dimensional lookup, from TD to Type
 */
template <class T3>
class IntSelectTCO{
public:

	template <class T1,class T2>
	static inline TypeConversionOperatorI *Do(const TypeDescriptor td,const TypeDescriptor td2,bool isCompare){
		TypeConversionOperatorI *tco = NULL_PTR(TypeConversionOperatorI *);

		TD_FullType fullType = td.GetFullTypeCode();
		uint32 storageSize = td.StorageSize();

		switch(fullType){
		case TDF_UnsignedInteger:{
			switch(storageSize){
			case 1:{
				tco = T3::template Do<T2,uint8>(td2,td,isCompare);
			}break;
			case 2:{
				tco = T3::template Do<T2,uint16>(td2,td,isCompare);
			}break;
			case 4:{
				tco = T3::template Do<T2,uint32>(td2,td,isCompare);
			}break;
			case 8:{
				tco = T3::template Do<T2,uint64>(td2,td,isCompare);
			}break;
			default:{
			}
			}
		}break;
		case TDF_SignedInteger:{
			switch(storageSize){
			case 1:{
				tco = T3::template Do<T2,int8>(td2,td,isCompare);
			}break;
			case 2:{
				tco = T3::template Do<T2,int16>(td2,td,isCompare);
			}break;
			case 4:{
				tco = T3::template Do<T2,int32>(td2,td,isCompare);
			}break;
			case 8:{
				tco = T3::template Do<T2,int64>(td2,td,isCompare);
			}break;

			default:{
			}
			}
		}break;
		default:{
		}
		}
		return tco;
	}
};


/**
 * Performs final selection
 */
class N2N_SelectTCO{

public:
	template <class Tdest,class Tsource>
	static inline TypeConversionOperatorI *Do(const TypeDescriptor destTd,const TypeDescriptor sourceTd,bool isCompare){
		TypeConversionOperatorI *tco = NULL_PTR(TypeConversionOperatorI *);

		tco = new Number2NumberTCO<Tsource,Tdest>(isCompare);

		return tco;
	}

};
/**
 * Performs final selection
 */
class B2B_SelectTCO{
public:
	template <class Tdest,class Tsource>
	static inline TypeConversionOperatorI *Do(const TypeDescriptor destTd,const TypeDescriptor sourceTd,bool isCompare){
		TypeConversionOperatorI *tco = NULL_PTR(TypeConversionOperatorI *);

		tco = new BitSet2BitSetTCO<Tsource,Tdest>(sourceTd,destTd,isCompare);

		return tco;
	}
};
/**
 * Performs final selection
 */
class B2N_SelectTCO{
public:
	template <class Tdest,class Tsource>
	static inline TypeConversionOperatorI *Do(const TypeDescriptor destTd,const TypeDescriptor sourceTd,bool isCompare){
		TypeConversionOperatorI *tco = NULL_PTR(TypeConversionOperatorI *);

		tco = new BitSet2NumberTCO<Tsource,Tdest>(sourceTd,isCompare);

		return tco;
	}
};
/**
 * Performs final selection
 */
class N2B_SelectTCO{
public:
	template <class Tdest,class Tsource>
	static inline TypeConversionOperatorI *Do(const TypeDescriptor destTd,const TypeDescriptor sourceTd,bool isCompare){
		TypeConversionOperatorI *tco = NULL_PTR(TypeConversionOperatorI *);

		tco = new Number2BitSetTCO<Tsource,Tdest>(destTd,isCompare);

		return tco;
	}
};

TypeConversionOperatorI *NumericConversionFactory::GetOperator(VariableDescriptor &destVd,VariableDescriptor &sourceVd,bool isCompare){
	TypeConversionOperatorI *tco = NULL_PTR(TypeConversionOperatorI *);

    TypeDescriptor sourceTd = sourceVd.GetFinalTypeDescriptor();
    TypeDescriptor destTd   = destVd.GetFinalTypeDescriptor();

	if (!destTd.SameTypeAndSizeAs(sourceTd))
		if (destTd.IsBasicType() && sourceTd.IsBasicType()){
			if (!destTd.IsBitType() && !sourceTd.IsBitType()){
				tco = FullSelectTCO<FullSelectTCO<N2N_SelectTCO> >::Do<void,void>(destTd,sourceTd,isCompare);
			} else
			if (destTd.IsBitType() && sourceTd.IsBitType()){
				tco = IntSelectTCO<IntSelectTCO<B2B_SelectTCO> >::Do<void,void>(destTd,sourceTd,isCompare);
			} else
			if (destTd.IsBitType() ){
				tco = IntSelectTCO<FullSelectTCO<N2B_SelectTCO> >::Do<void,void>(destTd,sourceTd,isCompare);
			} else {
				tco = FullSelectTCO<IntSelectTCO<B2N_SelectTCO> >::Do<void,void>(destTd,sourceTd,isCompare);
			}
		}

	return tco;
}


INSTALL_STARTUP_MANAGER_INITIALISATION_ENTRY(NumericConversionFactory,("TCMService",emptyString),("TCMDataBase",emptyString))

ErrorManagement::ErrorType NumericConversionFactoryStartup::Init(){
	ErrorManagement::ErrorType ret;
	ret.initialisationError = !TypeConversionManager::Register(&numericConversionFactory);

	return ret;
}

ErrorManagement::ErrorType NumericConversionFactoryStartup::Finish(){
	ErrorManagement::ErrorType ret;

	TypeConversionManager::Clean();

	return ret;
}


} //MARTe
