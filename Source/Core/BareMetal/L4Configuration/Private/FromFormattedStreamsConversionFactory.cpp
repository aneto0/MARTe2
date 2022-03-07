/**
 * @file FromFormattedStreamsConversionFactory.cpp
 * @brief Header file for class FromFormattedStreamsConversionFactory
 * @date Apr 22, 2020
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
#include "StructuredDataI.h"
#include "StructuredDataParser.h"
#include "StreamMemoryReference.h"
#include "CompositeErrorManagement.h"
#include "TypeConversionManager.h"


namespace MARTe{


/************************************************************************/
/**                  Type Convertors                                   */
/************************************************************************/

/**
 * @brief copies floats to strings
 */
class StreamIToStructuredDataITCO: public TypeConversionOperatorI{

public:

	/**
	 * @brief constructor
	 */
	StreamIToStructuredDataITCO(CCString formatIn,uint32 dSize,uint32 sSize):format(formatIn),inputSize(sSize),outputSize(dSize){	}


	/**
	 * @brief destructor
	 */
	virtual  ~StreamIToStructuredDataITCO(){}

	/**
	 * abstraction of the access to the input data
	 */
	virtual StreamI *GetStream(const uint8 *source) const{
		return reinterpret_cast<StreamI *>(const_cast<uint8 *>(source));
	}

	/**
	 * @brief data conversion method
	 */
	virtual ErrorManagement::ErrorType Convert(uint8 *dest, const uint8 *source,uint32 numberOfElements) const{
		ErrorManagement::ErrorType  ret;

		ret.fatalError = dest == (NULL_PTR(uint8 *));
		REPORT_ERROR(ret,"dest == NULL");

		if (ret){
			ret.fatalError = source == (NULL_PTR(uint8 *));
			REPORT_ERROR(ret,"source == NULL");
		}

		if (ret){
			ret.unsupportedFeature = ((numberOfElements > 1) && ((outputSize==0) || (inputSize==0)) );
			REPORT_ERROR(ret,"cannot handle arrays of conversion without knowledge of the size of the non-abstract output or input ");
		}

		for (uint32 i = 0 ; (i < numberOfElements) && ret; i++){
			StructuredDataI *destSI = reinterpret_cast<StructuredDataI *>(dest);
			StreamI *sourceSI = GetStream(source);

			ParserI *parser = NULL_PTR(ParserI *);
			if (format == "XML"){
			    parser = new StructuredDataParser(*destSI,StructuredDataParser::XMLParser);
			} else
			if (format == "JSON"){
			    parser = new StructuredDataParser(*destSI,StructuredDataParser::JsonParser);
			} else
			if ((format == "CDB") || (format.GetSize()==0)){
				parser = new StructuredDataParser (*destSI,StructuredDataParser::StandardParser);
			} else {
				ret.unsupportedFeature = true;
				COMPOSITE_REPORT_ERROR(ret,"unsupported stream structure format ",format);
			}

			ret.unsupportedFeature = (parser == NULL_PTR(ParserI *));
			COMPOSITE_REPORT_ERROR(ret,"unsupported stream structure format ",format);

			if (ret){
				ret = parser->Parse(*sourceSI);
				REPORT_ERROR(ret,"Parse error");
			}

			if (parser != NULL_PTR(ParserI *)){
				delete parser;
			}

			dest+=outputSize;
			source+=inputSize;
		}

		return ret;
	}


private:

	/**
	 *
	 */
	DynamicCString format;

	/**
	 *
	 */
	uint32 inputSize;

	/**
	 *
	 */
	uint32 outputSize;

};

/**
 * @brief copies floats to strings
 */
class CStringToStructuredDataITCO: public StreamIToStructuredDataITCO{

public:

	/**
	 * @brief constructor
	 */
	CStringToStructuredDataITCO(CCString formatIn,uint32 dSize):StreamIToStructuredDataITCO(formatIn,dSize,sizeof(CCString)){
		smm = NULL;
	}

	/**
	 * @brief destructor
	 */
	virtual  ~CStringToStructuredDataITCO(){
		if (smm != NULL){
			delete smm;
		}
	}

	virtual StreamI *GetStream(const uint8 *source) const{
		if (smm != NULL){
			delete smm;
		}
		CCString *sourceCCS = reinterpret_cast<CCString *>(const_cast<uint8 *>(source));

		smm = new StreamMemoryReference (sourceCCS->GetList(),sourceCCS->GetSize());
		return smm;
	}


private:
	// to allow access to the memory of the string using the StreamI interface
	mutable StreamMemoryReference *smm;

};


/**
 * @brief copies floats to strings
 */
class StreamStringToStructuredDataITCO: public StreamIToStructuredDataITCO{

public:

	/**
	 * @brief constructor
	 */
	StreamStringToStructuredDataITCO(CCString formatIn,uint32 dSize):StreamIToStructuredDataITCO(formatIn,dSize,sizeof(StreamString)){
	}

	/**
	 * @brief destructor
	 */
	virtual  ~StreamStringToStructuredDataITCO(){
	}

	virtual StreamI *GetStream(const uint8 *source) const{
		StreamString *sourceCCS = reinterpret_cast<StreamString *>(const_cast<uint8 *>(source));
		return sourceCCS;
	}


private:

};


/**
 *
 */
class FromFormattedStreamsConversionFactory: public TypeConversionFactoryI{

public:

	/**
 	 * @brief Default constructor. NOOP.
 	 */
	FromFormattedStreamsConversionFactory();

    /**
     * @brief Default destructor.
     */
    virtual ~FromFormattedStreamsConversionFactory();

	/**
     * @brief allow access to optimal functor for data conversion
	 *
	 */
	TypeConversionOperatorI *GetOperator(VariableDescriptor &destVd,VariableDescriptor &sourceVd,bool isReader);


private:

} fromFormattedStreamConversionFactory;

FromFormattedStreamsConversionFactory::FromFormattedStreamsConversionFactory(){
}

FromFormattedStreamsConversionFactory::~FromFormattedStreamsConversionFactory(){
}

//#include <stdio.h>
TypeConversionOperatorI *FromFormattedStreamsConversionFactory::GetOperator(VariableDescriptor &destVd,VariableDescriptor &sourceVd,bool isCompare){
	ErrorManagement::ErrorType  ret;

	TypeConversionOperatorI *tco = NULL_PTR(TypeConversionOperatorI *);

    TypeDescriptor sourceTd  = sourceVd.GetFinalTypeDescriptor();
    TypeDescriptor destTd    = destVd.GetFinalTypeDescriptor();


    bool matching = destTd.SameTypeAs(StructuredDataIType(0));
    if (matching){
        matching = sourceTd.IsFormattedCharStreamType();
    }
    if (matching){
        matching = matching && (isCompare == false);
    }

    /*
    // avoid String to Matrix/Vector  being processed here
    if (matching){
        TypeDescriptor td =InvalidType(0);
        uint32 nOfDimensionsS  = 20;
        sourceVd.GetVariableDimensions(NULL,td,nOfDimensionsS,NULL);
        uint32 nOfDimensionsD  = 20;
        destVd.GetVariableDimensions(NULL,td,nOfDimensionsD,NULL);

        matching = (nOfDimensionsS == nOfDimensionsD);
    }
    */
    // either both have more than one dimensions or neither
    // replaces commented block above requiring less computations
    if (matching){
        matching = !((CCString(sourceVd.GetModifiers()).GetSize()==0) ^ (CCString(destVd.GetModifiers()).GetSize()==0));
    }


	// this implies SString,Stream,DynamicCString and excludes ConstCharString
	if (matching){
		DynamicCString format;
		CStringTool cs = format();

		ret.fatalError = !sourceTd.GetStreamFormat(cs);
		REPORT_ERROR(ret,"GetStreamFormat failed");

//REPORT_ERROR(ErrorManagement::FatalError,"came here");

		if (ret && sourceTd.IsCharString()){
			tco = new CStringToStructuredDataITCO(format,destTd.StorageSize());
		} else
		if (ret && sourceTd.SameTypeAs(StreamIType(0))){
			tco = new StreamIToStructuredDataITCO(format,destTd.StorageSize(),sourceTd.StorageSize());
		} else
		if (ret && sourceTd.SameAs(StreamStringType(sizeof(StreamString)))){
			tco = new StreamStringToStructuredDataITCO(format,destTd.StorageSize());
		}
	}


	return tco;
}

INSTALL_STARTUP_MANAGER_INITIALISATION_ENTRY(FromFormattedStreamsConversionFactory,("TCMService",emptyString),("TCMDataBase",emptyString))

ErrorManagement::ErrorType FromFormattedStreamsConversionFactoryStartup::Init(){
	ErrorManagement::ErrorType ret;
	ret.initialisationError = !TypeConversionManager::Register(&fromFormattedStreamConversionFactory);

	return ret;
}

ErrorManagement::ErrorType FromFormattedStreamsConversionFactoryStartup::Finish(){
	ErrorManagement::ErrorType ret;

	TypeConversionManager::Clean();

	return ret;
}



} //MARTe
