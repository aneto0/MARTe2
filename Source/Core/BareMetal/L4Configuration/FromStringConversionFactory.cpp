/**
 * @file FromStringConversionFactory.cpp
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
#include "TypeConversionFactoryI.h"
#include "TypeConversionManager.h"
#include "StreamString.h"
#include "ConversionPrivate.h"
#include "BitSetToInteger.h"

namespace MARTe{

/**
 * @brief interface definition
 */
class StringReader{

public:
	/**
	 * @brief to obtain the string to be parsed
	 * and increases pointer to the next object
	 * @return the buffer where the string is written to
	 */
	virtual CCString GetToken(const uint8 *ptr,uint32 index)=0;

	/**
	 * @brief virtual destructor
	 */
	virtual ~StringReader(){}

};

/**
 * extract tokens Streams
 * manages only one Stream as size of actual class is not known
 */
class StreamReader: public StringReader{
public:

	/**
	 * @param [in]  streamSizeIn the size of the actual Stream derived object
	 */
	StreamReader(uint32 streamSizeIn):streamSize(streamSizeIn){}

	~StreamReader(){}

	/**
	 * @brief to be able to use the desired source
	 * @details directly parses the stream here. will not seek to start nor leave position untouched
	 */
	virtual CCString GetToken(const uint8 *ptr,uint32 index){
		const CCString separators=" \r\n\t";
		CCString ret;
		bool ok = true;

		buffer.Truncate(0);
		if ((index == 0) || (streamSize > 0)) {
			uint8 *ptr8 = const_cast<uint8 *>(ptr);
			StreamI *stream = reinterpret_cast<StreamI *>(ptr8+streamSize*index);
			buffer.Truncate(0);
			if (stream != NULL){
				char8 c = 0;
				uint32 size =1 ;
				ok = stream->Read(&c,size);
				while ( ok && (size == 1) && (!separators.In(c))){
					size = 1;
					ok = stream->Read(&c,size);
				}
				if (ok){
					size = 1;
					ok = stream->Read(&c,size);
					while ( ok && (size == 1) && (!separators.In(c))){
						buffer.Append(c);
						size = 1;
						ok = stream->Read(&c,size);
					}
				}
			}
		}
		if (ok){
			ret = CCString(buffer.GetList());
		}

		return ret;
	}

private:
	/**
	 * the actual size of the stream derived class
	 */
	const uint32 streamSize;
	/**
	 *
	 */
	DynamicCString buffer;
};

/**
 * @reads from SString to String
 */
class SStringReader: public StringReader{
public:

	/**
	 * @brief to be able to use the desired source
	 */
	virtual CCString GetToken(const uint8 *ptr,uint32 index){
		uint8 *ptrc = const_cast<uint8 *>(ptr);
		StreamString *ss = reinterpret_cast<StreamString *>(ptrc);
		return CCString(ss[index].Buffer());
	}
};

class CCStringReader: public StringReader{
public:

	/**
	 * @brief to be able to use the desired source
	 */
	virtual CCString GetToken(const uint8 *ptr,uint32 index){
		const CCString *ccs = reinterpret_cast<const CCString *>(ptr);
		return ccs[index];
	}
};


/************************************************************************/
/**                  Type Convertors                                   */
/************************************************************************/

/**
 * @brief copies to strings
 */
class FromStringTCO: public TypeConversionOperatorI{
public:

	/**
	 * @brief constructor
	 */
	FromStringTCO(StringReader *readerIn,bool isCompare): reader(readerIn),compare(isCompare){
	}


	/**
	 * @brief destructor
	 */
	virtual  ~FromStringTCO(){
		delete reader;
	}
protected:
	/*
	 * @brief the writer mechanism
	 */
	StringReader *reader;
	/*
	 * @brief whether this is a comparison and not a copy
	 */
	bool compare;

};

/**
 * @brief copies integer to strings
 */
template <typename integerType>
class StringToIntegerTCO: public FromStringTCO{

public:

	/**
	 * @brief constructor
	 */
	StringToIntegerTCO(StringReader *readerIn,bool isCompare):FromStringTCO(readerIn,isCompare){	}

	/**
	 * @brief destructor
	 */
	virtual  ~StringToIntegerTCO(){}

	/**
	 * @brief data conversion method
	 */
	virtual ErrorManagement::ErrorType Convert(uint8 *dest, const uint8 *source,uint32 numberOfElements) const{
		ErrorManagement::ErrorType  ok;

		integerType *destI = reinterpret_cast<integerType *>(dest);
		if (reader == NULL_PTR(StringReader *)){
			ok.internalSetupError = true;
		}
		for (int i = 0 ; (i < numberOfElements) && ok; i++){
			CCString token = reader->GetToken(source,i);
			if (token.GetSize() == 0){
				ok.illegalOperation = true;
			} else {
				integerType num;
				ok.fatalError = StringToInteger(token.GetList(),num);
				if (ok){
					if (compare){
						ok.comparisonFailure = (num != destI[i]);
					} else {
						destI[i] = num;
					}
				}
			}
		}

		return ok;
	}

};

/**
 * @brief copies bitset integers to strings
 */
template <typename integerBitsSize>
class StringToBitSetTCO: public FromStringTCO{

public:

	/**
	 * @brief destructor
	 */
	virtual  ~StringToBitSetTCO(){}

	/**
	 * @brief constructor
	 */
	StringToBitSetTCO(StringReader *readerIn,TypeDescriptor td,bool isSignedIn,bool isCompare):FromStringTCO(readerIn,isCompare){
		numberBitSize  = td.numberOfBits;
		numberBitShift = td.bitOffset;
		isSigned       = isSignedIn;
	}

	/**
	 * @brief data conversion method
	 */
	virtual ErrorManagement::ErrorType Convert(uint8 *dest, const uint8 *source,uint32 numberOfElements) const{
		ErrorManagement::ErrorType  ok;

		integerBitsSize *destI;
		if (reader == NULL_PTR(StringReader *)){
			ok.internalSetupError = true;
		}
		for (int i = 0 ; (i < numberOfElements) && ok; i++){
			CCString token = reader->GetToken(source,i);
			if (token.GetSize() == 0){
				ok.illegalOperation = true;
			} else {
				integerBitsSize num;
				ok.fatalError = StringToInteger(token.GetList(),num);
				if (ok){
					if (compare){
						integerBitsSize num2;
						BSToBS(&num2,0,sizeof(num2)*8,isSigned,&destI[i],numberBitShift,numberBitSize,isSigned);
//						ok.fatalError = BitSetToInteger<integerHolderSize,integerBitsSize>(num2,&destI[i],numberBitShift,numberBitSize,isSigned);
//						if (ok){
							ok.comparisonFailure = (num != num2);
//						}
					} else {
						BSToBS(&destI[i],numberBitShift,numberBitSize,isSigned,&num,0,sizeof(num)*8,isSigned);
//						ok.fatalError = IntegerToBitSet<integerHolderSize,integerBitsSize>(&destI[i],numberBitShift,numberBitSize,isSigned,num);
					}
				}
			}
		}
		return ok;
	}
private:
	/**
	 * @brief bitshift of number
	 */
	uint8 numberBitShift;
	/**
	 * @brief bitsize of number
	 */
	uint8 numberBitSize;
	/**
	 * @brief signedness of number
	 */
	bool  isSigned;
};


/**
 * @brief copies floats to strings
 */
template <typename floatType>
class StringToFloatTCO: public FromStringTCO{

public:

	/**
	 * @brief constructor
	 */
	StringToFloatTCO(StringReader *readerIn,bool isCompare):FromStringTCO(readerIn,isCompare){	}


	/**
	 * @brief destructor
	 */
	virtual  ~StringToFloatTCO(){}

	/**
	 * @brief data conversion method
	 */
	virtual ErrorManagement::ErrorType Convert(uint8 *dest, const uint8 *source,uint32 numberOfElements) const{
		ErrorManagement::ErrorType  ok;

		floatType *destF = reinterpret_cast<floatType *>(dest);
		if (reader == NULL_PTR(StringReader *)){
			ok.internalSetupError = true;
		}
		for (int i = 0 ; (i < numberOfElements) && ok; i++){
			CCString token = reader->GetToken(source,i);
			if (token.GetSize() == 0){
				ok.illegalOperation = true;
			} else {
				floatType num;
				ok.fatalError = StringToFloat(token.GetList(),num);
				if (ok){
					if (compare){
						ok.comparisonFailure = (num != destF[i]);
					} else {
						destF[i] = num;
					}
				}
			}
		}

		return ok;
	}
};


class FromStringConversionFactory: TypeConversionFactoryI{

public:

	/**
 	 * @brief Default constructor. NOOP.
 	 */
	FromStringConversionFactory();

    /**
     * @brief Default destructor.
     */
    virtual ~FromStringConversionFactory();

	/**
     * @brief allow access to optimal functor for data conversion
	 *
	 */
	TypeConversionOperatorI *GetOperator(const TypeDescriptor &destTd,const TypeDescriptor &sourceTd,bool isReader);


private:

} sameTypeConversionFactory;

FromStringConversionFactory::FromStringConversionFactory(){
	TypeConversionManager::Instance().Register(this);
}

FromStringConversionFactory::~FromStringConversionFactory(){
}

TypeConversionOperatorI *FromStringConversionFactory::GetOperator(const TypeDescriptor &destTd,const TypeDescriptor &sourceTd,bool isCompare){
	TypeConversionOperatorI *tco = NULL_PTR(TypeConversionOperatorI *);

	StringReader *reader = NULL_PTR(StringReader *);
	if (destTd.SameTypeAs(StreamType(0))){
		reader = new StreamReader(destTd.objectSize);
	} else
	if (destTd.SameAs(StreamStringType(sizeof(StreamString))) ){
		reader = new SStringReader();
	} else
	if (destTd.SameAs(DynamicCharString) ||
		destTd.SameTypeAs(ConstCharString(sizeof(CCString))) ||
	    destTd.SameTypeAs(CharString)){
		reader = new CCStringReader();
	}

	// this implies SString,Stream,DynamicCString and excludes ConstCharString
	if (reader != NULL){
		if (!destTd.isStructuredData){
			switch(destTd.fullType){
			case TDF_UnsignedInteger:{
				switch(sourceTd.basicTypeSize){
				case Size8bit:{
					if (!sourceTd.hasBitSize){
						tco = new StringToIntegerTCO<uint8>(reader,isCompare);
					} else {
						tco = new StringToBitSetTCO<uint8>(reader,destTd,false,isCompare);
					}
				}break;
				case Size16bit:{
					if (!sourceTd.hasBitSize){
						tco = new StringToIntegerTCO<uint16>(reader,isCompare);
					} else {
						tco = new StringToBitSetTCO<uint16>(reader,destTd,false,isCompare);
					}
				}break;
				case Size32bit:{
					if (!sourceTd.hasBitSize){
						tco = new StringToIntegerTCO<uint32>(reader,isCompare);
					} else {
						tco = new StringToBitSetTCO<uint32>(reader,destTd,false,isCompare);
					}
				}break;
				case Size64bit:{
					if (!sourceTd.hasBitSize){
						tco = new StringToIntegerTCO<uint64>(reader,isCompare);
					} else {
						tco = new StringToBitSetTCO<uint64>(reader,destTd,false,isCompare);
					}
				}break;

				default:{
				}
				}
			}break;
			case TDF_SignedInteger:{
				switch(sourceTd.basicTypeSize){
				case Size8bit:{
					if (!sourceTd.hasBitSize){
						tco = new StringToIntegerTCO<int8>(reader,isCompare);
					} else {
						tco = new StringToBitSetTCO<int8>(reader,destTd,true,isCompare);
					}
				}break;
				case Size16bit:{
					if (!sourceTd.hasBitSize){
						tco = new StringToIntegerTCO<int16>(reader,isCompare);
					} else {
						tco = new StringToBitSetTCO<int16>(reader,destTd,true,isCompare);
					}
				}break;
				case Size32bit:{
					if (!sourceTd.hasBitSize){
						tco = new StringToIntegerTCO<int32>(reader,isCompare);
					} else {
						tco = new StringToBitSetTCO<int32>(reader,destTd,true,isCompare);
					}
				}break;
				case Size64bit:{
					if (!sourceTd.hasBitSize){
						tco = new StringToIntegerTCO<int64>(reader,isCompare);
					} else {
						tco = new StringToBitSetTCO<int64>(reader,destTd,true,isCompare);
					}
				}break;

				default:{
				}
				}
			}break;
			case TDF_Float:{
				switch(sourceTd.basicTypeSize){
				case Size32bit:{
					tco = new StringToFloatTCO<float>(reader,isCompare);
				}break;
				case Size64bit:{
					tco = new StringToFloatTCO<double>(reader,isCompare);
				}break;
				default:{
				}
				}
			}break;


			default:{

			}
			}
		}
	}


	if ((tco == NULL) && (reader != NULL)){
		delete reader;
	}

	return tco;
}



} //MARTe
